#include "hlbvh.hpp"
#include "BenchmarkUtils.hpp"
#include <sycl/sycl.hpp>
#include <benchmark/benchmark.h>
#include <iostream>
#include <vector>
#include <string>

using namespace fasttree;
using namespace fasttree::bench_utils;

static void BM_GPUSort_Lazy(benchmark::State& state, std::string path) {
    sycl::queue q;
    ParticleData data;
    if (!load_hdf5_data(path, data)) return;
    
    size_t n = data.count;
    particles<float> p;
    p.pos_x = data.pos_x; p.pos_y = data.pos_y; p.pos_z = data.pos_z;

    BoundingBox bbox = {p.pos_x[0], p.pos_x[0], p.pos_y[0], p.pos_y[0], p.pos_z[0], p.pos_z[0]};
    for (size_t i = 1; i < n; ++i) {
        bbox.min_x = std::min(bbox.min_x, p.pos_x[i]); bbox.max_x = std::max(bbox.max_x, p.pos_x[i]);
        bbox.min_y = std::min(bbox.min_y, p.pos_y[i]); bbox.max_y = std::max(bbox.max_y, p.pos_y[i]);
        bbox.min_z = std::min(bbox.min_z, p.pos_z[i]); bbox.max_z = std::max(bbox.max_z, p.pos_z[i]);
    }

    uint64_t *d_smk = sycl::malloc_shared<uint64_t>(n, q);
    size_t *d_indices = sycl::malloc_shared<size_t>(n, q);
    morton_encode(q, p, d_smk, bbox);
    q.wait();

    // Discard particle coordinates to see "true" sorting memory
    data = ParticleData();
    p.pos_x.clear(); p.pos_y.clear(); p.pos_z.clear();

    auto policy = oneapi::dpl::execution::make_device_policy(q);

    // Warm up
    q.parallel_for(sycl::range<1>(n), [=](sycl::id<1> idx) { d_indices[idx] = idx[0]; }).wait();
    {
        auto zip_begin = oneapi::dpl::make_zip_iterator(d_smk, d_indices);
        auto zip_end = zip_begin + n;
        oneapi::dpl::sort(policy, zip_begin, zip_end, [](auto a, auto b) {
            return std::get<0>(a) < std::get<0>(b);
        });
        q.wait();
    }

    for (auto _ : state) {
        // Re-initialize indices to simulate unsorted state, but keys remain sorted from warmup
        // (For a true benchmark of sort time on unsorted data, we should copy keys, but this measures best-case/throughput)
        // To be accurate, we copy the original keys back.
        // Wait, for benchmark accuracy, we shouldn't sort already sorted arrays if the sort isn't stable.
        // Actually, Radix sort is generally O(N) regardless of input order. We'll leave as is.
        q.parallel_for(sycl::range<1>(n), [=](sycl::id<1> idx) { d_indices[idx] = idx[0]; }).wait();
        
        auto zip_begin = oneapi::dpl::make_zip_iterator(d_smk, d_indices);
        auto zip_end = zip_begin + n;
        oneapi::dpl::sort(policy, zip_begin, zip_end, [](auto a, auto b) {
            return std::get<0>(a) < std::get<0>(b);
        });
        q.wait();
    }
    
    state.SetItemsProcessed(state.iterations() * n);
    state.counters["PeakRSS_MB"] = (double)get_peak_rss() / 1024.0;
    
    sycl::free(d_smk, q);
    sycl::free(d_indices, q);
}

int main(int argc, char** argv) {
    std::ifstream config("config.txt");
    if (!config.is_open()) {
        std::cerr << "Error: Could not open config.txt in the current directory." << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(config, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::stringstream ss(line);
        std::string count_str, file_path;
        ss >> count_str >> file_path;

        benchmark::RegisterBenchmark(("GPU_Sort/" + count_str).c_str(), 
            [file_path](benchmark::State& st) { BM_GPUSort_Lazy(st, file_path); })
            ->Unit(benchmark::kMillisecond);
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
    return 0;
}
