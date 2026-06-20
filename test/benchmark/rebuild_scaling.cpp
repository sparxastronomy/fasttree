#include "hlbvh.hpp"
#include "BenchmarkUtils.hpp"
#include <sycl/sycl.hpp>
#include <benchmark/benchmark.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace fasttree;
using namespace fasttree::bench_utils;

static void BM_Rebuild_Lazy(benchmark::State& state, std::string path) {
    sycl::queue q;
    ParticleData data;
    if (!load_hdf5_data(path, data)) return;
    
    particles<float> p;
    p.pos_x = data.pos_x; p.pos_y = data.pos_y; p.pos_z = data.pos_z;
    p.mass = data.masses;
    size_t n = p.pos_x.size();

    // Fill IDs and ghosts
    p.id.resize(n);
    p.is_ghost.resize(n, 0);
    for (size_t i = 0; i < n; ++i) {
        p.id[i] = static_cast<uint32_t>(i);
    }

    // Warm up / initial build
    {
        TreeSoA tree(q, n);
        build_bvh(q, p, tree);
        q.wait();
        tree.free(q);
    }

    // Perturb coordinates using loaded velocities to simulate one time-step update
    float dt = 0.01f;
    for (size_t i = 0; i < n; ++i) {
        float vx = (i < data.vel_x.size()) ? data.vel_x[i] : 1.0f;
        float vy = (i < data.vel_y.size()) ? data.vel_y[i] : 1.0f;
        float vz = (i < data.vel_z.size()) ? data.vel_z[i] : 1.0f;
        p.pos_x[i] += vx * dt;
        p.pos_y[i] += vy * dt;
        p.pos_z[i] += vz * dt;
    }

    for (auto _ : state) {
        TreeSoA tree(q, n);
        build_bvh(q, p, tree);
        q.wait();
        tree.free(q);
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

        benchmark::RegisterBenchmark(("TreeRebuild/" + count_str).c_str(), 
            [file_path](benchmark::State& st) { BM_Rebuild_Lazy(st, file_path); })
            ->Unit(benchmark::kMillisecond);
    }

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
    return 0;
}
