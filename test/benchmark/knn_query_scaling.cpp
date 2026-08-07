#include "BenchmarkUtils.hpp"
#include "hlbvh.hpp"
#include <benchmark/benchmark.h>
#include <sycl/sycl.hpp>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace fasttree;
using namespace fasttree::bench_utils;

static void BM_KNNQuery_Lazy(benchmark::State &state, std::string path, int k) {
    sycl::queue  q;
    ParticleData data;
    if (!load_hdf5_data(path, data)) return;

    size_t             n = data.count;
    particles<coord_t> p;
    double box_min = 0.0, box_size = 1.0;
    fill_particle_coords(data, p, box_min, box_size);

    TreeSoA tree(q, n);
    build_bvh(q, p, tree);
    q.wait();

    const int num_queries = 1000;
    coord_t  *qx          = sycl::malloc_shared<coord_t>(num_queries, q);
    coord_t  *qy          = sycl::malloc_shared<coord_t>(num_queries, q);
    coord_t  *qz          = sycl::malloc_shared<coord_t>(num_queries, q);

    std::mt19937                          gen(42);
    std::uniform_int_distribution<size_t> dis_idx(0, n - 1);
    for (int i = 0; i < num_queries; ++i) {
        size_t p_idx = dis_idx(gen);
        qx[i]        = p.pos_x[p_idx];
        qy[i]        = p.pos_y[p_idx];
        qz[i]        = p.pos_z[p_idx];
    }

    size_t *results      = sycl::malloc_shared<size_t>(num_queries * k, q);
    dist_t *result_dists = sycl::malloc_shared<dist_t>(num_queries * k, q);

    // Warm up
    knn_query(q, tree, qx, qy, qz, k, num_queries, results, result_dists);
    q.wait();

    for (auto _ : state) {
        knn_query(q, tree, qx, qy, qz, k, num_queries, results, result_dists);
        q.wait();
    }

    state.counters["PeakRSS_MB"] = (double)get_peak_rss() / 1024.0;

    sycl::free(qx, q);
    sycl::free(qy, q);
    sycl::free(qz, q);
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
            std::string b_name = "KNNQuery/" + count_str + "/k=" + std::to_string(k);
            benchmark::RegisterBenchmark(b_name.c_str(), [file_path, k](benchmark::State &st) {
                BM_KNNQuery_Lazy(st, file_path, k);
            })->Unit(benchmark::kMicrosecond);
        }
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
    return 0;
}
