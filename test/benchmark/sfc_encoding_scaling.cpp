#include "BenchmarkUtils.hpp"
#include "hlbvh.hpp"
#include <benchmark/benchmark.h>
#include <sycl/sycl.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace fasttree;
using namespace fasttree::bench_utils;

static void BM_MortonEncode_Lazy(benchmark::State &state, std::string path) {
  sycl::queue q;
  ParticleData data;
  if (!load_hdf5_data(path, data)) return;

  size_t n = data.pos_x.size();
  coord_t *d_x = sycl::malloc_shared<coord_t>(n, q);
  coord_t *d_y = sycl::malloc_shared<coord_t>(n, q);
  coord_t *d_z = sycl::malloc_shared<coord_t>(n, q);
  sfc_key *d_keys = sycl::malloc_shared<sfc_key>(n, q);

  std::copy(data.pos_x.begin(), data.pos_x.end(), d_x);
  std::copy(data.pos_y.begin(), data.pos_y.end(), d_y);
  std::copy(data.pos_z.begin(), data.pos_z.end(), d_z);

  BoundingBox<coord_t> bbox = {d_x[0], d_x[0], d_y[0], d_y[0], d_z[0], d_z[0]};
  for (size_t i = 1; i < n; ++i) {
    bbox.min_x = std::min(bbox.min_x, d_x[i]);
    bbox.max_x = std::max(bbox.max_x, d_x[i]);
    bbox.min_y = std::min(bbox.min_y, d_y[i]);
    bbox.max_y = std::max(bbox.max_y, d_y[i]);
    bbox.min_z = std::min(bbox.min_z, d_z[i]);
    bbox.max_z = std::max(bbox.max_z, d_z[i]);
  }

  // Warm up JIT
  sfc_encode(q, d_x, d_y, d_z, n, d_keys, bbox);
  q.wait();

  for (auto _ : state) {
    sfc_encode(q, d_x, d_y, d_z, n, d_keys, bbox);
    q.wait();
  }

  state.SetItemsProcessed(state.iterations() * n);
  state.counters["PeakRSS_MB"] = (double)get_peak_rss() / 1024.0;

  sycl::free(d_x, q);
  sycl::free(d_y, q);
  sycl::free(d_z, q);
  sycl::free(d_keys, q);
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

    benchmark::RegisterBenchmark(("SFC-Encode/" + count_str).c_str(), [file_path](benchmark::State &st) {
      BM_MortonEncode_Lazy(st, file_path);
    })->Unit(benchmark::kMillisecond);
  }

  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
  benchmark::Shutdown();
  return 0;
}
