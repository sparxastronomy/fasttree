#include "domain_decomposition.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
#include <mpi.h>

using namespace fasttree;

void test_splitter_uniform() {
  printf("Running test_splitter_uniform...\n");
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
  printf("  Passed!\n\n");
}

void test_splitter_extreme_clustering() {
  printf("Running test_splitter_extreme_clustering...\n");
  int P = 4;
  std::vector<int> hist(1024, 0);
  hist[500] = 9900;
  for (int i = 0; i < 100; ++i) hist[i] = 1;

  std::vector<uint32_t> spl = generate_splitters(hist, P, 10);
  assert(spl.size() == P + 1);
  assert(spl[0] == 0);
  assert(spl[P] == 1024);
  for (int i = 0; i < P; ++i) { assert(spl[i] <= spl[i + 1]); }
  printf("  Passed!\n\n");
}

void test_mpi_pipeline(sycl::queue &q, int rank, int size) {
  if (rank == 0) { printf("Running test_mpi_pipeline with %d ranks...\n", size); }

  // Simulate extreme initial load imbalance (Rank 0 has everything)
  size_t FIXED_N = 10000;
  size_t local_n = (rank == 0) ? FIXED_N : 0;
  particles<float> p;
  p.pos_x.resize(local_n);
  p.pos_y.resize(local_n);
  p.pos_z.resize(local_n);
  p.id.resize(local_n);
  p.is_ghost.resize(local_n);

  // Spread particles uniformly across the X axis
  if (rank == 0) {
    for (size_t i = 0; i < local_n; ++i) {
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
    printf("  Phase 1 Bounding Box: [%f, %f] Passed!\n", bbox.min_x, bbox.max_x);
  }

  // Phase 2: Histogram
  int m = 10;
  std::vector<int> global_hist = get_global_histogram(q, p, bbox, m);
  long long total_p = 0;
  for (int count : global_hist) total_p += count;
  assert(total_p == FIXED_N);
  if (rank == 0) printf("  Phase 2 Histogram Verification Passed!\n");

  // Phase 3: Splitter Generation
  std::vector<uint32_t> splitters = generate_splitters(global_hist, size, m);
  if (rank == 0) {
    for (int i = 0; i < size; ++i) assert(splitters[i] <= splitters[i + 1]);
    printf("  Phase 3 Splitters Monotonicity Passed!\n");
  }

  // Phase 4: Local Binning & Network Routing
  particles<float> redistributed_p = redistribute_particles(q, p, splitters, bbox, m);

  int local_count = redistributed_p.pos_x.size();
  int global_count = 0;
  MPI_Allreduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  assert(global_count == FIXED_N);

  // Verify Load Balancing
  if (size > 1) {
    assert(local_count > 0);  // Rank 0 gave up particles, other ranks received them
    if (size == 4) {
      // Due to uniform distribution, each of the 4 ranks should get ~2500
      assert(local_count > 2000 && local_count < 3000);
    }
  }
  printf("\t\tRank %d: Received %d particles after redistribution.\n", rank, static_cast<int>(local_count));

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
  if (rank == 0) printf("  Phase 4 Redistribution & Load Balance Passed!\n");

  // Phase 5: Explicit Halo Exchange
  float h_max = 5.0f;  // 5% of the total domain width
  size_t pre_ghost_count = redistributed_p.pos_x.size();

  particles<float> ghosted_p = exchange_halos(q, redistributed_p, h_max);
  size_t post_ghost_count = ghosted_p.pos_x.size();

  // Verify Ghost Particles
  if (size > 1) {
    printf("\t\tRank %d: Received %d ghost particles during halo exchange.\n", rank, static_cast<int>(post_ghost_count - pre_ghost_count));
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
  if (rank == 0) { printf("  Phase 5 Halo Exchange & Unified Tree Construction Passed!\n"); }

  // Phase 6: Verify Range Query against Ghost Particles (SPH Density Mock)
  int n_total = ghosted_p.pos_x.size();
  if (n_total > 0) {
    // 1. Allocate device memory for queries
    float *d_qx = sycl::malloc_shared<float>(n_total, q);
    float *d_qy = sycl::malloc_shared<float>(n_total, q);
    float *d_qz = sycl::malloc_shared<float>(n_total, q);
    float *d_rmin = sycl::malloc_shared<float>(n_total, q);
    float *d_rmax = sycl::malloc_shared<float>(n_total, q);

    // We will query around every particle using the halo radius (h_max)
    q.copy(ghosted_p.pos_x.data(), d_qx, n_total);
    q.copy(ghosted_p.pos_y.data(), d_qy, n_total);
    q.copy(ghosted_p.pos_z.data(), d_qz, n_total);
    q.fill(d_rmin, 0.0f, n_total);
    q.fill(d_rmax, h_max, n_total);
    q.wait();

    // 2. Allocate output buffers for the query results
    int max_res = 128;  // Generous neighbor limit
    int *d_results = sycl::malloc_shared<int>(n_total * max_res, q);
    int *d_counts = sycl::malloc_shared<int>(n_total, q);

    // 3. Execute massive parallel range query
    range_query(q, tree, d_qx, d_qy, d_qz, d_rmin, d_rmax, n_total, d_results, d_counts, max_res);
    q.wait();

    // 4. Bring results back to host for verification
    std::vector<int> h_results(n_total * max_res);
    std::vector<int> h_counts(n_total);
    q.copy(d_results, h_results.data(), n_total * max_res);
    q.copy(d_counts, h_counts.data(), n_total);
    q.wait();

    // 5. Verify local particles can see ghost particles
    int local_saw_ghost = 0;

    for (int i = 0; i < n_total; ++i) {
      // In SPH, only local particles actively compute their density
      if (ghosted_p.is_ghost[i] == 0) {
        int num_neighbors = h_counts[i];
        for (int j = 0; j < num_neighbors; ++j) {
          int neighbor_idx = h_results[i * max_res + j];

          // Did our query radius overlap a ghost particle from another rank?
          if (ghosted_p.is_ghost[neighbor_idx] == 1) { local_saw_ghost = 1; }
        }
      }
    }

    // If we are running with >1 rank, boundary overlaps MUST have occurred.
    // Therefore, at least one local particle across the cluster must have seen a ghost.
    if (size > 1) {
      int global_saw_ghost = 0;
      MPI_Allreduce(&local_saw_ghost, &global_saw_ghost, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

      if (rank == 0) {
        assert(global_saw_ghost == 1 && "CRITICAL FAILURE: No local particles saw any ghost particles during the range query!");
        std::cout << "  Phase 6 Range Query: Boundary ghost visibility successfully verified!" << std::endl;
      }
    }

    sycl::free(d_qx, q);
    sycl::free(d_qy, q);
    sycl::free(d_qz, q);
    sycl::free(d_rmin, q);
    sycl::free(d_rmax, q);
    sycl::free(d_results, q);
    sycl::free(d_counts, q);
  }

  tree.free(q);
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == 0) { printf("=== ALL MPI TESTS PASSED SECURELY ===\n\n"); }
}

/**
 * This function test range_query assuming the halo-exchange and domain decomposition are correctly implemented.
 */
void test_ghost_visibility(sycl::queue &q) {
  printf("Running test_ghost_visibility...\n");

  // 1. Setup Data: 3 Local Particles, 2 Ghost Particles
  int n_local = 3;
  int n_ghost = 2;
  int n = n_local + n_ghost;

  particles<float> p;
  p.pos_x.resize(n);
  p.pos_y.resize(n, 0.0f);
  p.pos_z.resize(n, 0.0f);
  p.id.resize(n);
  p.is_ghost.resize(n);

  // Local Particles
  p.pos_x[0] = 1.0f;
  p.id[0] = 100;
  p.is_ghost[0] = 0;
  p.pos_x[1] = 2.0f;
  p.id[1] = 101;
  p.is_ghost[1] = 0;
  p.pos_x[2] = 3.0f;
  p.id[2] = 102;
  p.is_ghost[2] = 0;

  // Ghost Particles (simulate boundary Data received from MPI)
  p.pos_x[3] = -1.0f;
  p.id[3] = 200;
  p.is_ghost[3] = 1;  // Left boundary ghost
  p.pos_x[4] = 10.0f;
  p.id[4] = 201;
  p.is_ghost[4] = 1;  // Right boundary ghost

  // 2. Build the Tree
  TreeSoA tree(q, n);
  build_bvh(q, p, tree);  // This will Morton-sort `p` in-place!
  q.wait();

  // Verify the builder actually sorted the ghost tags correctly
  int found_ghosts = 0;
  for (int i = 0; i < n; i++) {
    if (p.is_ghost[i] == 1) found_ghosts++;
  }
  assert(found_ghosts == 2 && "Data corruption: is_ghost tags lost during build_bvh sort!");

  // =========================================================================
  // TEST 1: Ranged Query specifically targeting the left boundary ghost
  // Query at x = -0.5 with radius = 2.0.
  // Should hit Ghost at x = -1.0 (dist 0.5) and Local at x = 1.0 (dist 1.5).
  // =========================================================================
  float *qx = sycl::malloc_shared<float>(1, q);
  float *qy = sycl::malloc_shared<float>(1, q);
  float *qz = sycl::malloc_shared<float>(1, q);
  qx[0] = -0.5f;
  qy[0] = 0.0f;
  qz[0] = 0.0f;

  float *r_min = sycl::malloc_shared<float>(1, q);
  float *r_max = sycl::malloc_shared<float>(1, q);
  r_min[0] = 0.0f;
  r_max[0] = 2.0f;

  int max_res = 10;
  int *rq_results = sycl::malloc_shared<int>(max_res, q);
  int *rq_count = sycl::malloc_shared<int>(1, q);

  range_query(q, tree, qx, qy, qz, r_min, r_max, 1, rq_results, rq_count, max_res);
  q.wait();

  assert(rq_count[0] == 2 && "Range query failed to find exactly 2 particles!");

  bool hit_local = false;
  bool hit_ghost = false;
  for (int i = 0; i < rq_count[0]; ++i) {
    int sorted_idx = rq_results[i];  // This is the index into the sorted array `p`
    if (p.is_ghost[sorted_idx] == 1) {
      assert(p.id[sorted_idx] == 200 && "Hit the wrong ghost!");
      hit_ghost = true;
    } else {
      assert(p.id[sorted_idx] == 100 && "Hit the wrong local particle!");
      hit_local = true;
    }
  }
  assert(hit_ghost && "Range query FAILED to see the ghost particle!");
  assert(hit_local && "Range query FAILED to see the local particle!");
  printf("  Range Query: Ghost visibility passed.\n");

  // =========================================================================
  // TEST 2: kNN Query specifically targeting the right boundary ghost
  // Query at x = 9.5, asking for k = 2.
  // Closest is Ghost at x = 10.0 (dist 0.5). Second closest is Local at x = 3.0 (dist 6.5).
  // =========================================================================
  qx[0] = 9.5f;
  int k = 2;
  int *knn_results = sycl::malloc_shared<int>(k, q);
  float *knn_dists = sycl::malloc_shared<float>(k, q);

  knn_query(q, tree, qx, qy, qz, k, 1, knn_results, knn_dists);
  q.wait();

  int first_neighbor_idx = knn_results[0];
  int second_neighbor_idx = knn_results[1];

  // Verify 1st neighbor is the ghost
  assert(p.is_ghost[first_neighbor_idx] == 1 && "kNN 1st neighbor should be a ghost!");
  assert(p.id[first_neighbor_idx] == 201 && "kNN hit the wrong ghost!");
  assert(std::abs(knn_dists[0] - 0.5f) < 1e-4 && "kNN distance calculation wrong!");

  // Verify 2nd neighbor is the local particle
  assert(p.is_ghost[second_neighbor_idx] == 0 && "kNN 2nd neighbor should be local!");
  assert(p.id[second_neighbor_idx] == 102 && "kNN hit the wrong local particle!");
  assert(std::abs(knn_dists[1] - 6.5f) < 1e-4 && "kNN distance calculation wrong!");

  printf("  kNN Query: Ghost visibility passed.\n");

  // Cleanup
  tree.free(q);
  sycl::free(qx, q);
  sycl::free(qy, q);
  sycl::free(qz, q);
  sycl::free(r_min, q);
  sycl::free(r_max, q);
  sycl::free(rq_results, q);
  sycl::free(rq_count, q);
  sycl::free(knn_results, q);
  sycl::free(knn_dists, q);

  printf("SUCCESS: Queries can seamlessly interact with ghost particles.\n\n");
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

  test_ghost_visibility(q);

  MPI_Finalize();
  return 0;
}