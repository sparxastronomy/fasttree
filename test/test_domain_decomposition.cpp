#include "domain_decomposition.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
#include <mpi.h>

using namespace fasttree;

void test_splitter_uniform() {
  std::cout << "Running test_splitter_uniform..." << std::endl;
  int num_buckets = 1000;
  int P = 4;
  std::vector<int> hist(1024, 0);
  for (int i = 0; i < 1000; ++i) hist[i] = 10;

  std::vector<uint32_t> spl = generate_splitters(hist, P, 10);
  assert(spl.size() == P + 1);
  assert(spl[0] == 0);
  assert(spl[1] == 250);
  assert(spl[2] == 500);
  assert(spl[3] == 750);
  assert(spl[4] == 1024);
  std::cout << "  Passed!" << std::endl;
}

void test_splitter_extreme_clustering() {
  std::cout << "Running test_splitter_extreme_clustering..." << std::endl;
  int P = 4;
  std::vector<int> hist(1024, 0);
  hist[500] = 9900;
  for (int i = 0; i < 100; ++i) hist[i] = 1;

  std::vector<uint32_t> spl = generate_splitters(hist, P, 10);
  assert(spl.size() == P + 1);
  assert(spl[0] == 0);
  assert(spl[P] == 1024);
  for (int i = 0; i < P; ++i) { assert(spl[i] <= spl[i + 1]); }
  std::cout << "  Passed!" << std::endl;
}

void test_mpi_pipeline(sycl::queue &q, int rank, int size) {
  if (rank == 0) { std::cout << "Running test_mpi_pipeline with " << size << " ranks..." << std::endl; }

  // Simulate extreme initial load imbalance (Rank 0 has everything)
  size_t local_n = (rank == 0) ? 10000 : 0;
  particles<float> p;
  p.pos_x.resize(local_n);
  p.pos_y.resize(local_n);
  p.pos_z.resize(local_n);
  p.id.resize(local_n);
  p.is_ghost.resize(local_n);

  if (rank == 0) {
    for (size_t i = 0; i < local_n; ++i) {
      // Spread particles uniformly across the X axis
      p.pos_x[i] = static_cast<float>(i) / local_n * 100.0f;
      p.pos_y[i] = 50.0f;
      p.pos_z[i] = 50.0f;
      p.id[i] = static_cast<uint32_t>(i);
      p.is_ghost[i] = 0;
    }
  }

  // Phase 1: Bounding Box
  BoundingBox bbox = get_global_bounding_box(q, p);
  if (rank == 0) {
    assert(std::abs(bbox.min_x - 0.0f) < 1e-4);
    assert(std::abs(bbox.max_x - 100.0f * (local_n - 1) / local_n) < 1e-2);
    std::cout << "  Phase 1 Bounding Box: [" << bbox.min_x << ", " << bbox.max_x << "] Passed!" << std::endl;
  }

  // Phase 2: Histogram
  int m = 10;
  std::vector<int> global_hist = get_global_histogram(q, p, bbox, m);
  long long total_p = 0;
  for (int count : global_hist) total_p += count;
  assert(total_p == 10000);
  if (rank == 0) std::cout << "  Phase 2 Histogram Verification Passed!" << std::endl;

  // Phase 3: Splitter Generation
  std::vector<uint32_t> splitters = generate_splitters(global_hist, size, m);
  if (rank == 0) {
    for (int i = 0; i < size; ++i) assert(splitters[i] <= splitters[i + 1]);
    std::cout << "  Phase 3 Splitters Monotonicity Passed!" << std::endl;
  }

  // Phase 4: Local Binning & Network Routing
  particles<float> redistributed_p = redistribute_particles(q, p, splitters, bbox, m);

  int local_count = redistributed_p.pos_x.size();
  int global_count = 0;
  MPI_Allreduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  assert(global_count == 10000);

  // Verify Load Balancing
  if (size > 1) {
    assert(local_count > 0);  // Rank 0 gave up particles, other ranks received them
    if (size == 4) {
      // Due to uniform distribution, each of the 4 ranks should get ~2500
      assert(local_count > 2000 && local_count < 3000);
    }
  }

  // Spatial consistency check
  for (size_t i = 0; i < redistributed_p.pos_x.size(); ++i) {
    float nx = std::min((redistributed_p.pos_x[i] - bbox.min_x) / (bbox.max_x - bbox.min_x), 0.999999f);
    float ny = std::min((redistributed_p.pos_y[i] - bbox.min_y) / (bbox.max_y - bbox.min_y), 0.999999f);
    float nz = std::min((redistributed_p.pos_z[i] - bbox.min_z) / (bbox.max_z - bbox.min_z), 0.999999f);
    std::uint64_t ix = float_to_int(1.0f + nx);
    std::uint64_t iy = float_to_int(1.0f + ny);
    std::uint64_t iz = float_to_int(1.0f + nz);
    std::uint64_t key = spread3_u64(ix) | (spread3_u64(iy) << 1) | (spread3_u64(iz) << 2);
    uint32_t bucket_id = static_cast<uint32_t>(key >> (63 - m));

    assert(bucket_id >= splitters[rank] && bucket_id <= splitters[rank + 1]);
  }

  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == 0) std::cout << "  Phase 4 Redistribution & Load Balance Passed!" << std::endl;

  // Phase 5: Explicit Halo Exchange
  float h_max = 5.0f;  // 5% of the total domain width
  size_t pre_ghost_count = redistributed_p.pos_x.size();

  particles<float> ghosted_p = exchange_halos(q, redistributed_p, h_max);
  size_t post_ghost_count = ghosted_p.pos_x.size();

  // Verify Ghost Particles
  if (size > 1) {
    int local_received_ghosts = (post_ghost_count > pre_ghost_count) ? 1 : 0;
    int global_received_ghosts = 0;
    MPI_Allreduce(&local_received_ghosts, &global_received_ghosts, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    // With h_max=5.0 and uniform distribution, boundary overlap is guaranteed
    assert(global_received_ghosts > 0);
  }

  // Verify that the newly appended particles are properly tagged
  for (size_t i = pre_ghost_count; i < post_ghost_count; ++i) { assert(ghosted_p.is_ghost[i] == 1); }

  // Final sanity check: Build the tree on combined local + ghost particles
  TreeSoA tree(q, ghosted_p.pos_x.size());
  build_bvh(q, ghosted_p, tree);
  q.wait();

  tree.free(q);
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == 0) {
    std::cout << "  Phase 5 Halo Exchange & Unified Tree Construction Passed!" << std::endl;
    std::cout << "=== ALL MPI TESTS PASSED SECURELY ===" << std::endl;
  }
}

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  sycl::queue q;

  if (rank == 0) {
    std::cout << "=== Standalone CPU/GPU Splitter Tests ===" << std::endl;
    test_splitter_uniform();
    test_splitter_extreme_clustering();
  }
  MPI_Barrier(MPI_COMM_WORLD);

  test_mpi_pipeline(q, rank, size);

  MPI_Finalize();
  return 0;
}