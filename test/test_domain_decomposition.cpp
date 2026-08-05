#include "domain_decomposition.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
#include <mpi.h>

using namespace fasttree;

void test_splitter_uniform() {
    printf("Running test_splitter_uniform...\n");
    int              num_buckets = 1000;
    int              P           = 4;
    std::vector<int> hist(1024, 0);
    for (int i = 0; i < 1000; ++i)
        hist[i] = 10;

    std::vector<uint32_t> spl = generate_splitters(hist, P, 10);
    for (int i = 0; i <= P; ++i) {
        printf("  Splitter[%d] = %u\n", i, spl[i]);
    }
    assert(spl.size() == P + 1);
    assert(spl[0] == 0);
    assert(spl[1] == 249);
    assert(spl[2] == 499);
    assert(spl[3] == 749);
    assert(spl[4] == 1024);
    printf("  Passed!\n\n");
}

void test_splitter_extreme_clustering() {
    printf("Running test_splitter_extreme_clustering...\n");
    int              P = 4;
    std::vector<int> hist(1024, 0);
    hist[500] = 9900;
    for (int i = 0; i < 100; ++i)
        hist[i] = 1;

    std::vector<uint32_t> spl = generate_splitters(hist, P, 10);
    for (int i = 0; i <= P; ++i) {
        printf("  Splitter[%d] = %u\n", i, spl[i]);
    }

    assert(spl.size() == P + 1);
    assert(spl[0] == 0);
    assert(spl[P] == 1024);
    for (int i = 0; i < P; ++i) {
        assert(spl[i] <= spl[i + 1]);
    }
    printf("  Passed!\n\n");
}

void test_mpi_pipeline(sycl::queue &q, int rank, int size) {
    if (rank == 0) { printf("Running test_mpi_pipeline with %d ranks...\n", size); }

    // Simulate extreme initial load imbalance (Rank 0 has everything)
    size_t             FIXED_N = 10000;
    size_t             local_n = (rank == 0) ? FIXED_N : 0;
    particles<coord_t> p;
    p.pos_x.resize(local_n);
    p.pos_y.resize(local_n);
    p.pos_z.resize(local_n);
    p.id.resize(local_n);
    p.is_ghost.resize(local_n);

    // Spread particles diagonally across 3D space
    if (rank == 0) {
#if defined(FASTTREE_INTEGER_COORDS)
#if defined(POSITIONS_IN_128BIT)
        uint64_t max_c = 0xFFFFFFFFFFFFFFFFull;
#elif defined(POSITIONS_IN_64BIT)
        uint64_t max_c = (1ULL << 42) - 1ULL;
#elif defined(POSITIONS_IN_32BIT)
        uint64_t max_c = (1ULL << 21) - 1ULL;
#else
        uint64_t max_c = 1073741824ULL;
#endif
        for (size_t i = 0; i < local_n; ++i) {
            double frac   = static_cast<double>(i) / local_n;
            p.pos_x[i]    = static_cast<coord_t>(frac * max_c);
            p.pos_y[i]    = static_cast<coord_t>(frac * max_c);
            p.pos_z[i]    = static_cast<coord_t>(frac * max_c);
            p.id[i]       = static_cast<uint32_t>(i);
            p.is_ghost[i] = 0;
        }
#else
        for (size_t i = 0; i < local_n; ++i) {
            double frac   = static_cast<double>(i) / local_n;
            p.pos_x[i]    = static_cast<coord_t>(frac * 100.0);
            p.pos_y[i]    = static_cast<coord_t>(frac * 100.0);
            p.pos_z[i]    = static_cast<coord_t>(frac * 100.0);
            p.id[i]       = static_cast<uint32_t>(i);
            p.is_ghost[i] = 0;
        }
#endif
    }

    // Phase 1: Bounding Box
    BoundingBox<coord_t> bbox = get_global_bounding_box(q, p);
    if (rank == 0) {
        printf(
            "  Phase 1 Bounding Box: [%f, %f] Passed!\n", (double)bbox.min_x, (double)bbox.max_x
        );
    }

#if defined(DCOMPOSITION_TYPE_SAMPLING)
    // Skip Phase 2
    // Phase 3: Splitter Generation via Sampling
    std::vector<sfc_key> splitters = get_deterministic_splitters(q, p, bbox);
    if (rank == 0) {
        for (int i = 0; i < size; ++i)
            assert(!(splitters[i + 1] < splitters[i]));
        printf("  Phase 3 (Sampling) Splitters Monotonicity Passed!\n");
    }
    // Phase 4: Local Binning & Network Routing
    particles<coord_t> redistributed_p = redistribute_particles(q, p, splitters, bbox);
#else
    // Phase 2: Histogram
    int              m           = 20;
    std::vector<int> global_hist = get_global_histogram(q, p, bbox, m);
    long long        total_p     = 0;
    for (int count : global_hist)
        total_p += count;
    assert(total_p == FIXED_N);
    if (rank == 0) printf("  Phase 2 Histogram Verification Passed!\n");

    // Phase 3: Splitter Generation
    std::vector<uint32_t> splitters = generate_splitters(global_hist, size, m);
    if (rank == 0) {
        for (int i = 0; i < size; ++i)
            assert(splitters[i] <= splitters[i + 1]);
        printf("  Phase 3 Splitters Monotonicity Passed!\n");
    }
    if (rank == 0) {
        printf("  Splitters:\n");
        for (int i = 0; i <= size; ++i) {
            printf("    Splitter[%d] = %llu\n", i, (unsigned long long)splitters[i]);
        }
    }

    // Phase 4: Local Binning & Network Routing
    particles<coord_t> redistributed_p = redistribute_particles(q, p, splitters, bbox, m);
#endif

    int local_count  = redistributed_p.pos_x.size();
    int global_count = 0;
    MPI_Allreduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    assert(global_count == FIXED_N);

    // Verify Load Balancing
    if (size > 1) {
        if (size <= 4) {
            assert(local_count > 0); // Rank 0 gave up particles, other ranks received them
            if (size == 4) {
                // Due to uniform distribution, each of the 4 ranks should get ~2500
                assert(local_count > 2000 && local_count < 3000);
            }
        }
    }
    printf(
        "\t\tRank %d: Received %d particles after redistribution.\n",
        rank,
        static_cast<int>(local_count)
    );

#if !defined(FASTTREE_INTEGER_COORDS)
    // Spatial consistency check
    coord_t dx     = bbox.max_x - bbox.min_x;
    coord_t dy     = bbox.max_y - bbox.min_y;
    coord_t dz     = bbox.max_z - bbox.min_z;
    coord_t inv_dx = (dx == 0) ? static_cast<coord_t>(0.0) : (static_cast<coord_t>(1.0) / dx);
    coord_t inv_dy = (dy == 0) ? static_cast<coord_t>(0.0) : (static_cast<coord_t>(1.0) / dy);
    coord_t inv_dz = (dz == 0) ? static_cast<coord_t>(0.0) : (static_cast<coord_t>(1.0) / dz);
#endif

    for (size_t i = 0; i < redistributed_p.pos_x.size(); ++i) {
        sfc_key key;
#if defined(FASTTREE_INTEGER_COORDS)
        key = sfc_encode3D(
            redistributed_p.pos_x[i], redistributed_p.pos_y[i], redistributed_p.pos_z[i]
        );
#else
        coord_t nx = std::min(
            (redistributed_p.pos_x[i] - bbox.min_x) * inv_dx, static_cast<coord_t>(0.999999)
        );
        coord_t ny = std::min(
            (redistributed_p.pos_y[i] - bbox.min_y) * inv_dy, static_cast<coord_t>(0.999999)
        );
        coord_t nz = std::min(
            (redistributed_p.pos_z[i] - bbox.min_z) * inv_dz, static_cast<coord_t>(0.999999)
        );
        sfc1D ix = quantize_coord(nx);
        sfc1D iy = quantize_coord(ny);
        sfc1D iz = quantize_coord(nz);
        key      = sfc_encode3D(ix, iy, iz);
#endif

#if defined(DCOMPOSITION_TYPE_SAMPLING)
        assert(!(key < splitters[rank]) && !(splitters[rank + 1] < key));
#else
        uint32_t bucket_id = extract_bucket_id(key, m);
        assert(bucket_id >= splitters[rank] && bucket_id <= splitters[rank + 1]);
#endif
    }

    // Print the local bounding box for debugging
    if (redistributed_p.pos_x.size() > 0) {
        BoundingBox<coord_t> local_bbox = {
            redistributed_p.pos_x[0],
            redistributed_p.pos_x[0],
            redistributed_p.pos_y[0],
            redistributed_p.pos_y[0],
            redistributed_p.pos_z[0],
            redistributed_p.pos_z[0]
        };
        for (size_t j = 1; j < redistributed_p.pos_x.size(); ++j) {
            local_bbox.min_x = std::min(local_bbox.min_x, redistributed_p.pos_x[j]);
            local_bbox.max_x = std::max(local_bbox.max_x, redistributed_p.pos_x[j]);
            local_bbox.min_y = std::min(local_bbox.min_y, redistributed_p.pos_y[j]);
            local_bbox.max_y = std::max(local_bbox.max_y, redistributed_p.pos_y[j]);
            local_bbox.min_z = std::min(local_bbox.min_z, redistributed_p.pos_z[j]);
            local_bbox.max_z = std::max(local_bbox.max_z, redistributed_p.pos_z[j]);
        }
        printf(
            "\t\tRank %d: Local Bounding Box: [%f, %f] x [%f, %f] x [%f, %f]\n",
            rank,
            (double)local_bbox.min_x,
            (double)local_bbox.max_x,
            (double)local_bbox.min_y,
            (double)local_bbox.max_y,
            (double)local_bbox.min_z,
            (double)local_bbox.max_z
        );
    } else {
        printf("\t\tRank %d: Local Bounding Box: Empty (0 particles)\n", rank);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) printf("  Phase 4 Redistribution & Load Balance Passed!\n");

    // Phase 5: Explicit Halo Exchange
    coord_t h_max           = static_cast<coord_t>(5.0); // 5% of the total domain width
    size_t  pre_ghost_count = redistributed_p.pos_x.size();

    particles<coord_t> ghosted_p        = exchange_halos(q, redistributed_p, h_max);
    size_t             post_ghost_count = ghosted_p.pos_x.size();

    // Verify Ghost Particles
    if (size > 1) {
        printf(
            "\t\tRank %d: Received %d ghost particles during halo exchange.\n",
            rank,
            static_cast<int>(post_ghost_count - pre_ghost_count)
        );
        int local_received_ghosts  = (post_ghost_count > pre_ghost_count) ? 1 : 0;
        int global_received_ghosts = 0;
        MPI_Allreduce(
            &local_received_ghosts, &global_received_ghosts, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD
        );

        // With h_max=5.0 and uniform distribution, boundary overlap is guaranteed
        assert(global_received_ghosts > 0);
    }

    // Verify that the newly appended particles are properly tagged
    for (size_t i = pre_ghost_count; i < post_ghost_count; ++i) {
        assert(ghosted_p.is_ghost[i] == 1);
    }

    // Final sanity check: Build the tree on combined local + ghost particles
    TreeSoA tree(q, ghosted_p.pos_x.size());
    build_bvh(q, ghosted_p, tree);
    q.wait();
    if (rank == 0) { printf("  Phase 5 Halo Exchange & Unified Tree Construction Passed!\n"); }

    // Phase 6: Verify Range Query against Ghost Particles (SPH Density Mock)
    int n_total         = ghosted_p.pos_x.size();
    int local_saw_ghost = 0;

    if (n_total > 0) {
        // 1. Allocate device memory for queries
        coord_t *d_qx   = sycl::malloc_shared<coord_t>(n_total, q);
        coord_t *d_qy   = sycl::malloc_shared<coord_t>(n_total, q);
        coord_t *d_qz   = sycl::malloc_shared<coord_t>(n_total, q);
        coord_t *d_rmin = sycl::malloc_shared<coord_t>(n_total, q);
        coord_t *d_rmax = sycl::malloc_shared<coord_t>(n_total, q);

        // We will query around every particle using the halo radius (h_max)
        q.copy(ghosted_p.pos_x.data(), d_qx, n_total);
        q.copy(ghosted_p.pos_y.data(), d_qy, n_total);
        q.copy(ghosted_p.pos_z.data(), d_qz, n_total);
        q.fill(d_rmin, static_cast<coord_t>(0.0), n_total);
        q.fill(d_rmax, h_max, n_total);
        q.wait();

        // 2. Allocate output buffers for the query results
        int  max_res   = 128; // Generous neighbor limit
        int *d_results = sycl::malloc_shared<int>(n_total * max_res, q);
        int *d_counts  = sycl::malloc_shared<int>(n_total, q);

        // 3. Execute massive parallel range query
        range_query(
            q, tree, d_qx, d_qy, d_qz, d_rmin, d_rmax, n_total, d_results, d_counts, max_res
        );
        q.wait();

        // 4. Bring results back to host for verification
        std::vector<int> h_results(n_total * max_res);
        std::vector<int> h_counts(n_total);
        q.copy(d_results, h_results.data(), n_total * max_res);
        q.copy(d_counts, h_counts.data(), n_total);
        q.wait();

        // 5. Verify local particles can see ghost particles
        for (int i = 0; i < n_total; ++i) {
            int leaf_idx = i + n_total - 1;
            // In SPH, only local particles actively compute their density
            if (tree.is_ghost[leaf_idx] == 0) {
                int num_neighbors = std::min(h_counts[i], max_res);
                for (int j = 0; j < num_neighbors; ++j) {
                    int neighbor_idx      = h_results[i * max_res + j];
                    int neighbor_leaf_idx = neighbor_idx + n_total - 1;

                    // Did our query radius overlap a ghost particle from another rank?
                    if (tree.is_ghost[neighbor_leaf_idx] == 1) { local_saw_ghost = 1; }
                }
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

    // If we are running with >1 rank, boundary overlaps MUST have occurred.
    // Therefore, at least one local particle across the cluster must have seen a ghost.
    if (size > 1) {
        int global_saw_ghost = 0;
        MPI_Allreduce(&local_saw_ghost, &global_saw_ghost, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

        if (rank == 0) {
            assert(
                global_saw_ghost == 1 && "CRITICAL FAILURE: No local particles saw any ghost "
                                         "particles during the range query!"
            );
            std::cout << "  Phase 6 Range Query: Boundary ghost visibility successfully verified!"
                      << std::endl;
        }
    }

    tree.free(q);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) { printf("=== ALL MPI TESTS PASSED SECURELY ===\n\n"); }
}

void test_ghost_visibility(sycl::queue &q) {
    printf("Running test_ghost_visibility...\n");

    // 1. Setup Data: 3 Local Particles, 2 Ghost Particles
    int n_local = 3;
    int n_ghost = 2;
    int n       = n_local + n_ghost;

    particles<coord_t> p;
    p.pos_x.resize(n);
    p.pos_y.resize(n, static_cast<coord_t>(0.0));
    p.pos_z.resize(n, static_cast<coord_t>(0.0));
    p.id.resize(n);
    p.is_ghost.resize(n);

    // Local Particles
    p.pos_x[0]    = static_cast<coord_t>(1.0);
    p.id[0]       = 100;
    p.is_ghost[0] = 0;
    p.pos_x[1]    = static_cast<coord_t>(2.0);
    p.id[1]       = 101;
    p.is_ghost[1] = 0;
    p.pos_x[2]    = static_cast<coord_t>(3.0);
    p.id[2]       = 102;
    p.is_ghost[2] = 0;

    // Ghost Particles (simulate boundary Data received from MPI)
    p.pos_x[3]    = static_cast<coord_t>(-1.0);
    p.id[3]       = 200;
    p.is_ghost[3] = 1; // Left boundary ghost
    p.pos_x[4]    = static_cast<coord_t>(10.0);
    p.id[4]       = 201;
    p.is_ghost[4] = 1; // Right boundary ghost

    // 2. Build the Tree
    TreeSoA tree(q, n);
    build_bvh(q, p, tree); // This will Morton-sort `p` in-place!
    q.wait();

    // Verify the builder actually sorted the ghost tags correctly
    int found_ghosts = 0;
    for (int i = 0; i < n; i++) {
        if (tree.is_ghost[i + n - 1] == 1) found_ghosts++;
    }
    assert(found_ghosts == 2 && "Data corruption: is_ghost tags lost during build_bvh sort!");

    // =========================================================================
    // TEST 1: Ranged Query specifically targeting the left boundary ghost
    // Query at x = -0.5 with radius = 2.0.
    // Should hit Ghost at x = -1.0 (dist 0.5) and Local at x = 1.0 (dist 1.5).
    // =========================================================================
    coord_t *qx = sycl::malloc_shared<coord_t>(1, q);
    coord_t *qy = sycl::malloc_shared<coord_t>(1, q);
    coord_t *qz = sycl::malloc_shared<coord_t>(1, q);
    qx[0]       = static_cast<coord_t>(-0.5);
    qy[0]       = static_cast<coord_t>(0.0);
    qz[0]       = static_cast<coord_t>(0.0);

    coord_t *r_min = sycl::malloc_shared<coord_t>(1, q);
    coord_t *r_max = sycl::malloc_shared<coord_t>(1, q);
    r_min[0]       = static_cast<coord_t>(0.0);
    r_max[0]       = static_cast<coord_t>(2.0);

    int  max_res    = 10;
    int *rq_results = sycl::malloc_shared<int>(max_res, q);
    int *rq_count   = sycl::malloc_shared<int>(1, q);

    range_query(q, tree, qx, qy, qz, r_min, r_max, 1, rq_results, rq_count, max_res);
    q.wait();

    assert(rq_count[0] == 2 && "Range query failed to find exactly 2 particles!");

    bool hit_local = false;
    bool hit_ghost = false;
    for (int i = 0; i < rq_count[0]; ++i) {
        int sorted_idx = rq_results[i]; // This is the index in sorted leaf order
        int leaf_idx   = sorted_idx + tree.num_leaves - 1;
        if (tree.is_ghost[leaf_idx] == 1) {
            assert(tree.id[leaf_idx] == 200 && "Hit the wrong ghost!");
            hit_ghost = true;
        } else {
            assert(tree.id[leaf_idx] == 100 && "Hit the wrong local particle!");
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
    qx[0]               = static_cast<coord_t>(9.5);
    int     k           = 2;
    size_t *knn_results = sycl::malloc_shared<size_t>(k, q);
    dist_t *knn_dists   = sycl::malloc_shared<dist_t>(k, q);

    knn_query(q, tree, qx, qy, qz, k, 1, knn_results, knn_dists);
    q.wait();

    int first_neighbor_idx  = knn_results[0];
    int second_neighbor_idx = knn_results[1];

    int first_leaf_idx  = first_neighbor_idx + tree.num_leaves - 1;
    int second_leaf_idx = second_neighbor_idx + tree.num_leaves - 1;

    // Verify 1st neighbor is the ghost
    assert(tree.is_ghost[first_leaf_idx] == 1 && "kNN 1st neighbor should be a ghost!");
    assert(tree.id[first_leaf_idx] == 201 && "kNN hit the wrong ghost!");
#if !defined(FASTTREE_INTEGER_COORDS)
    assert(
        std::abs(knn_dists[0] - static_cast<coord_t>(0.25)) < 1e-4 &&
        "kNN distance calculation wrong!"
    );
#endif

    // Verify 2nd neighbor is the local particle
    assert(tree.is_ghost[second_leaf_idx] == 0 && "kNN 2nd neighbor should be local!");
    assert(tree.id[second_leaf_idx] == 102 && "kNN hit the wrong local particle!");
#if !defined(FASTTREE_INTEGER_COORDS)
    assert(
        std::abs(knn_dists[1] - static_cast<coord_t>(42.25)) < 1e-4 &&
        "kNN distance calculation wrong!"
    );
#endif

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