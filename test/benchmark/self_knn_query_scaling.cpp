#include "BenchmarkUtils.hpp"
#include "hlbvh.hpp"
#include <benchmark/benchmark.h>
#include <sycl/sycl.hpp>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace fasttree;
using namespace fasttree::bench_utils;

static void BM_SelfKNNQuery(benchmark::State &state, std::string path, int k) {
    sycl::queue q;
    try {
        q = sycl::queue(sycl::default_selector_v);
    } catch (const sycl::exception &e) {
        try {
            q = sycl::queue(sycl::cpu_selector_v);
        } catch (...) {
            state.SkipWithError(e.what());
            return;
        }
    }

    ParticleData data;
    if (!load_hdf5_data(path, data)) return;

    size_t             n = data.count;
    particles<coord_t> p;
    double box_min = 0.0, box_size = 1.0;
    fill_particle_coords(data, p, box_min, box_size);

    TreeSoA tree(q, n);
    build_bvh(q, p, tree);
    q.wait();

    // Dynamically calculate optimal batch size based on available system/GPU memory.
    // If available memory > required memory (e.g. 40GB A100 or 250GB CPU), executes in a SINGLE pass!
    // Environment variable FASTTREE_MAX_MEM_GB can override available memory budget.
    size_t chunk_size = compute_optimal_knn_batch_size(q, n, k);
    size_t alloc_queries = chunk_size;

    size_t *results      = sycl::malloc_shared<size_t>(alloc_queries * k, q);
    dist_t *result_dists = sycl::malloc_shared<dist_t>(alloc_queries * k, q);

    if (!results || !result_dists) {
        if (results) sycl::free(results, q);
        if (result_dists) sycl::free(result_dists, q);
        tree.free(q);
        state.SkipWithError("Memory allocation failed for self-kNN query output buffers!");
        return;
    }

    // Warm-up pass over all n particles in optimal batches
    for (size_t offset = 0; offset < n; offset += chunk_size) {
        size_t batch_size = std::min(chunk_size, n - offset);
        knn_query(q, tree, 
                  p.pos_x.data() + offset, 
                  p.pos_y.data() + offset, 
                  p.pos_z.data() + offset, 
                  k, batch_size, results, result_dists);
        q.wait();
    }

    for (auto _ : state) {
        for (size_t offset = 0; offset < n; offset += chunk_size) {
            size_t batch_size = std::min(chunk_size, n - offset);
            knn_query(q, tree, 
                      p.pos_x.data() + offset, 
                      p.pos_y.data() + offset, 
                      p.pos_z.data() + offset, 
                      k, batch_size, results, result_dists);
            q.wait();
        }
    }

    state.counters["PeakRSS_MB"] = (double)get_peak_rss() / 1024.0;
    state.counters["items_per_second"] = benchmark::Counter(
        double(n * state.iterations()), benchmark::Counter::kIsRate
    );

    sycl::free(results, q);
    sycl::free(result_dists, q);
    tree.free(q);
}

int main(int argc, char **argv) {
    std::ifstream config("config.txt");
    if (!config.is_open()) return 1;

    std::vector<int> k_values = {1, 2, 4, 8, 16, 32, 64, 128};

    std::string line;
    while (std::getline(config, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::stringstream ss(line);
        std::string       count_str, file_path;
        ss >> count_str >> file_path;

        for (int k : k_values) {
            std::string b_name = "SelfKNNQuery/" + count_str + "/k=" + std::to_string(k);
            benchmark::RegisterBenchmark(b_name.c_str(), [file_path, k](benchmark::State &st) {
                BM_SelfKNNQuery(st, file_path, k);
            })->Unit(benchmark::kMicrosecond);
        }
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
    return 0;
}
