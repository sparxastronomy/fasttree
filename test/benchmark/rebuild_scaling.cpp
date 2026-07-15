#include "BenchmarkUtils.hpp"
#include "hlbvh.hpp"
#include <benchmark/benchmark.h>
#include <sycl/sycl.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace fasttree;
using namespace fasttree::bench_utils;

static void BM_Rebuild_Lazy(benchmark::State &state, std::string path) {
  sycl::queue q;
  ParticleData data;
  if (!load_hdf5_data(path, data)) return;

  particles<coord_t> p;
  p.pos_x.assign(data.pos_x.begin(), data.pos_x.end());
  p.pos_y.assign(data.pos_y.begin(), data.pos_y.end());
  p.pos_z.assign(data.pos_z.begin(), data.pos_z.end());
  size_t n = p.pos_x.size();

  // Fill IDs and ghosts
  p.id.resize(n);
  p.is_ghost.resize(n, 0);
  for (size_t i = 0; i < n; ++i) { p.id[i] = static_cast<uint32_t>(i); }

  // Warm up / initial build
  {
    TreeSoA tree(q, n);
    build_bvh(q, p, tree);
    q.wait();
    tree.free(q);
  }

  // Perturb coordinates using loaded velocities to simulate one time-step update
  coord_t dt = static_cast<coord_t>(0.01);
  for (size_t i = 0; i < n; ++i) {
    coord_t vx = (i < data.vel_x.size()) ? static_cast<coord_t>(data.vel_x[i]) : static_cast<coord_t>(1.0);
    coord_t vy = (i < data.vel_y.size()) ? static_cast<coord_t>(data.vel_y[i]) : static_cast<coord_t>(1.0);
    coord_t vz = (i < data.vel_z.size()) ? static_cast<coord_t>(data.vel_z[i]) : static_cast<coord_t>(1.0);
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

int main(int argc, char **argv) {
  std::ifstream config("config.txt");
  if (!config.is_open()) return 1;

  std::string line;
  while (std::getline(config, line)) {
    if (line.empty() || line[0] == '#') continue;
    std::stringstream ss(line);
    std::string count_str, file_path;
    ss >> count_str >> file_path;

    benchmark::RegisterBenchmark(("TreeRebuild/" + count_str).c_str(), [file_path](benchmark::State &st) {
      BM_Rebuild_Lazy(st, file_path);
    })->Unit(benchmark::kMillisecond);
  }

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
  benchmark::Shutdown();
  return 0;
}
