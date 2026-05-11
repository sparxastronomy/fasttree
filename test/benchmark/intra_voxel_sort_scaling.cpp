#include "hlbvh.hpp"
#include "BenchmarkUtils.hpp"
#include <sycl/sycl.hpp>
#include <benchmark/benchmark.h>
#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace fasttree;
using namespace fasttree::bench_utils;

std::map<std::string, size_t> counts;

static void BM_IntraVoxelSort(benchmark::State& state, const std::string& label) {
    sycl::queue q;
    size_t n = counts[label];
    uint64_t *morton_keys = sycl::malloc_shared<uint64_t>(n, q);
    q.fill(morton_keys, 0ULL, n).wait();

    for (auto _ : state) {
        intra_voxel_sort(q, morton_keys, n);
        q.wait();
    }
    
    state.SetItemsProcessed(state.iterations() * n);
    sycl::free(morton_keys, q);
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

        // For this placeholder, we just need the count
        // We'll estimate count from the string (1K -> 1000) or just load metadata
        ParticleData data;
        if (load_hdf5_data(file_path, data)) {
            counts[count_str] = data.count;
            benchmark::RegisterBenchmark(("IntraVoxelSort/" + count_str).c_str(), BM_IntraVoxelSort, count_str)
                ->Unit(benchmark::kMicrosecond);
        }
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
    return 0;
}
