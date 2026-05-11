#include "hlbvh.hpp"
#include "BenchmarkUtils.hpp"
#include <sycl/sycl.hpp>
#include <benchmark/benchmark.h>
#include <iostream>
#include <vector>
#include <string>

using namespace fasttree;
using namespace fasttree::bench_utils;

static void BM_RadixSort_Lazy(benchmark::State& state, std::string path) {
    sycl::queue q;
    ParticleData data;
    if (!load_hdf5_data(path, data)) return;
    
    particles<float> p;
    p.pos_x = data.pos_x; p.pos_y = data.pos_y; p.pos_z = data.pos_z;
    size_t n = p.pos_x.size();
    
    BoundingBox bbox = {p.pos_x[0], p.pos_x[0], p.pos_y[0], p.pos_y[0], p.pos_z[0], p.pos_z[0]};
    for (size_t i = 1; i < n; ++i) {
        bbox.min_x = std::min(bbox.min_x, p.pos_x[i]); bbox.max_x = std::max(bbox.max_x, p.pos_x[i]);
        bbox.min_y = std::min(bbox.min_y, p.pos_y[i]); bbox.max_y = std::max(bbox.max_y, p.pos_y[i]);
        bbox.min_z = std::min(bbox.min_z, p.pos_z[i]); bbox.max_z = std::max(bbox.max_z, p.pos_z[i]);
    }

    std::vector<uint64_t> keys(n);
    morton_encode(q, p, keys, bbox);
    q.wait();

    // Discard particle coordinates to see "true" sorting memory
    data = ParticleData();
    p.pos_x.clear(); p.pos_y.clear(); p.pos_z.clear();

    for (auto _ : state) {
        std::vector<uint64_t> keys_copy = keys;
        auto indices = sort_morton_keys(keys_copy);
        benchmark::DoNotOptimize(indices);
    }
    
    state.SetItemsProcessed(state.iterations() * n);
    state.counters["PeakRSS_MB"] = (double)get_peak_rss() / 1024.0;
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

        benchmark::RegisterBenchmark(("RadixSort/" + count_str).c_str(), 
            [file_path](benchmark::State& st) { BM_RadixSort_Lazy(st, file_path); })
            ->Unit(benchmark::kMillisecond);
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
    return 0;
}
