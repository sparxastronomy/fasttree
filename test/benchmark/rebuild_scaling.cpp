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

  size_t n = data.count;
  ParticleData perturbed_data = data;
  float dt = 0.01f;
  for (size_t i = 0; i < n; ++i) {
    float vx = (i < data.vel_x.size()) ? data.vel_x[i] : 1.0f;
    float vy = (i < data.vel_y.size()) ? data.vel_y[i] : 1.0f;
    float vz = (i < data.vel_z.size()) ? data.vel_z[i] : 1.0f;
    perturbed_data.pos_x[i] += vx * dt;
    perturbed_data.pos_y[i] += vy * dt;
    perturbed_data.pos_z[i] += vz * dt;
  }

  particles<coord_t> p;
  double box_min = 0.0, box_size = 1.0;
  fill_particle_coords(perturbed_data, p, box_min, box_size);

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
