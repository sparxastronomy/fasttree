#include "hlbvh.hpp"
#include "BenchmarkUtils.hpp"
#include <sycl/sycl.hpp>
#include <benchmark/benchmark.h>
#include <iostream>
#include <vector>
#include <string>

using namespace fasttree;
using namespace fasttree::bench_utils;

static void BM_IntraVoxelSort_Lazy(benchmark::State& state, std::string path) {
    sycl::queue q;
    ParticleData data;
    if (!load_hdf5_data(path, data)) return;
    
    size_t n = data.count;
    uint64_t *morton_keys = sycl::malloc_shared<uint64_t>(n, q);
    size_t *indices = sycl::malloc_shared<size_t>(n, q);
    
    q.fill(morton_keys, 0ULL, n).wait();
    q.parallel_for(sycl::range<1>(n), [=](sycl::id<1> idx) {
        indices[idx] = idx[0];
    }).wait();

    // Warm up
    intra_voxel_sort(q, morton_keys, indices, n);
    q.wait();

    for (auto _ : state) {
        intra_voxel_sort(q, morton_keys, indices, n);
        q.wait();
    }
    
    state.SetItemsProcessed(state.iterations() * n);
    state.counters["PeakRSS_MB"] = (double)get_peak_rss() / 1024.0;
    sycl::free(morton_keys, q);
    sycl::free(indices, q);
}

int main(int argc, char** argv) {
    std::ifstream config("config.txt");
    if (!config.is_open()) return 1;

    std::string line;
    while (std::getline(config, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::stringstream ss(line);
        std::string count_str, file_path;
        ss >> count_str >> file_path;

        benchmark::RegisterBenchmark(("IntraVoxelSort/" + count_str).c_str(), 
            [file_path](benchmark::State& st) { BM_IntraVoxelSort_Lazy(st, file_path); })
            ->Unit(benchmark::kMicrosecond);
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
    return 0;
}
