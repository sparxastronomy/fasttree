#include "BenchmarkUtils.hpp"
#include "hlbvh.hpp"
#include <benchmark/benchmark.h>
#include <sycl/sycl.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace fasttree;
using namespace fasttree::bench_utils;

static void BM_TreeBuild_Lazy(benchmark::State &state, std::string path) {
  sycl::queue q;
  ParticleData data;
  if (!load_hdf5_data(path, data)) return;

  particles<coord_t> p;
  p.pos_x.assign(data.pos_x.begin(), data.pos_x.end());
  p.pos_y.assign(data.pos_y.begin(), data.pos_y.end());
  p.pos_z.assign(data.pos_z.begin(), data.pos_z.end());
  size_t n = p.pos_x.size();

  // Warm up kernel
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

    benchmark::RegisterBenchmark(("TreeBuild/" + count_str).c_str(), [file_path](benchmark::State &st) {
      BM_TreeBuild_Lazy(st, file_path);
    })->Unit(benchmark::kMillisecond);
  }

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
  benchmark::Shutdown();
  return 0;
}
