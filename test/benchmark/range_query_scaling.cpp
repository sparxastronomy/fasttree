#include "hlbvh.hpp"
#include "BenchmarkUtils.hpp"
#include <sycl/sycl.hpp>
#include <benchmark/benchmark.h>
#include <iostream>
#include <vector>
#include <string>
#include <random>

using namespace fasttree;
using namespace fasttree::bench_utils;

static void BM_RangeQuery_Lazy(benchmark::State& state, std::string path, float radius) {
    sycl::queue q;
    ParticleData data;
    if (!load_hdf5_data(path, data)) return;
    
    size_t n = data.count;
    particles<float> p;
    p.pos_x = data.pos_x; p.pos_y = data.pos_y; p.pos_z = data.pos_z;
    
    TreeSoA tree(q, n);
    build_bvh(q, p, tree);
    q.wait();

    const int num_queries = 1000;
    const int max_results_per_query = 1000;
    float *qx = sycl::malloc_shared<float>(num_queries, q);
    float *qy = sycl::malloc_shared<float>(num_queries, q);
    float *qz = sycl::malloc_shared<float>(num_queries, q);
    float *r_min = sycl::malloc_shared<float>(num_queries, q);
    float *r_max = sycl::malloc_shared<float>(num_queries, q);
    int *results = sycl::malloc_shared<int>(num_queries * max_results_per_query, q);
    int *result_counts = sycl::malloc_shared<int>(num_queries, q);

    std::mt19937 gen(42);
    std::uniform_int_distribution<size_t> dis_idx(0, n - 1);
    for (int i = 0; i < num_queries; ++i) {
        size_t p_idx = dis_idx(gen);
        qx[i] = p.pos_x[p_idx]; qy[i] = p.pos_y[p_idx]; qz[i] = p.pos_z[p_idx];
        r_min[i] = 0.0f;
        r_max[i] = radius;
    }

    // Warm up
    range_query(q, tree, qx, qy, qz, r_min, r_max, num_queries, results, result_counts, max_results_per_query);
    q.wait();

    for (auto _ : state) {
        q.fill(result_counts, 0, num_queries).wait();
        range_query(q, tree, qx, qy, qz, r_min, r_max, num_queries, results, result_counts, max_results_per_query);
        q.wait();
    }
    
    long long total_found = 0;
    for(int i=0; i<num_queries; ++i) total_found += result_counts[i];
    state.counters["TotalFound"] = (double)total_found;
    state.counters["PeakRSS_MB"] = (double)get_peak_rss() / 1024.0;

    sycl::free(qx, q); sycl::free(qy, q); sycl::free(qz, q);
    sycl::free(r_min, q); sycl::free(r_max, q);
    sycl::free(results, q); sycl::free(result_counts, q);
    tree.free(q);
}

int main(int argc, char** argv) {
    std::ifstream config("config.txt");
    if (!config.is_open()) return 1;

    std::vector<float> radii = {0.01f, 0.1f, 1.0f, 10.0f, 100.0f, 200.0f};

    std::string line;
    while (std::getline(config, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::stringstream ss(line);
        std::string count_str, file_path;
        ss >> count_str >> file_path;

        for (float radius : radii) {
            std::string b_name = "RangeQuery/" + count_str + "/R=" + std::to_string(radius);
            benchmark::RegisterBenchmark(b_name.c_str(), 
                [file_path, radius](benchmark::State& st) { BM_RangeQuery_Lazy(st, file_path, radius); })
                ->Unit(benchmark::kMicrosecond);
        }
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
    return 0;
}
