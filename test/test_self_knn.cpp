#include "hlbvh.hpp"
#include <sycl/sycl.hpp>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#if __has_include(<hdf5.h>)
#include <hdf5.h>
#include <hdf5_hl.h>
#define FASTTREE_HDF5_AVAILABLE 1
#endif

using namespace fasttree;

// ================================================================
// Coordinate & Distance Conversion Helpers
// ================================================================
static double int_dist_sq_to_phys(dist_t stored, double box_size) {
#if defined(FASTTREE_INTEGER_COORDS)
    constexpr double int_to_norm =
        1.0 / static_cast<double>(1ULL << (BITS_PER_DIMENSION - _DIST_SHIFT));
    double phys_scale = box_size * int_to_norm;
    return static_cast<double>(stored) * phys_scale * phys_scale;
#else
    (void)box_size;
    return static_cast<double>(stored);
#endif
}

static coord_t phys_pos_to_int(double phys, double box_min, double box_size) {
#if defined(FASTTREE_INTEGER_COORDS)
    double inv_dx = 1.0 / box_size;
    return float_to_int_rep(phys, box_min, inv_dx);
#else
    (void)box_min;
    (void)box_size;
    return static_cast<coord_t>(phys);
#endif
}

static double int_pos_to_phys(coord_t pos, double box_min, double box_size) {
#if defined(FASTTREE_INTEGER_COORDS)
    return int_rep_to_float(pos, box_min, box_size);
#else
    (void)box_min;
    (void)box_size;
    return static_cast<double>(pos);
#endif
}

// Compute squared distance between two 3D points using fasttree's exact metric
static dist_t
compute_point_dist_sq(coord_t x1, coord_t y1, coord_t z1, coord_t x2, coord_t y2, coord_t z2) {
    return static_cast<dist_t>(node_distance_sq(x1, y1, z1, x2, x2, y2, y2, z2, z2));
}

// ================================================================
// Brute Force Reference Calculation (Host O(N^2))
// ================================================================
struct BFNeighbor {
    dist_t  dist_sq;
    size_t  target_idx; // leaf rank or orig_idx
    coord_t x, y, z;
    bool    is_tied;
};

static std::vector<std::vector<BFNeighbor>> compute_host_brute_force_knn(
    const TreeSoA          &tree,
    size_t                  num_leaves,
    const std::vector<int> &query_leaf_ranks,
    int                     k,
    bool                    exclude_self
) {
    size_t                               num_queries = query_leaf_ranks.size();
    std::vector<std::vector<BFNeighbor>> all_neighbors(num_queries);

    size_t         num_internal = tree.num_internal;
    const coord_t *leaf_x       = tree.min_x + num_internal;
    const coord_t *leaf_y       = tree.min_y + num_internal;
    const coord_t *leaf_z       = tree.min_z + num_internal;
    const int     *orig_idx     = tree.orig_idx;

    for (size_t q = 0; q < num_queries; ++q) {
        int     q_leaf = query_leaf_ranks[q];
        coord_t qx     = leaf_x[q_leaf];
        coord_t qy     = leaf_y[q_leaf];
        coord_t qz     = leaf_z[q_leaf];

        std::vector<BFNeighbor> candidates;
        candidates.reserve(num_leaves);

        for (size_t j = 0; j < num_leaves; ++j) {
            if (exclude_self && static_cast<int>(j) == q_leaf) { continue; }
            coord_t tx = leaf_x[j];
            coord_t ty = leaf_y[j];
            coord_t tz = leaf_z[j];
            dist_t  d2 = compute_point_dist_sq(qx, qy, qz, tx, ty, tz);

#ifdef RETURN_ORIG_INDICES
            size_t ret_idx = static_cast<size_t>(orig_idx[j + num_internal]);
#else
            size_t ret_idx = j;
#endif
            candidates.push_back({d2, ret_idx, tx, ty, tz, false});
        }

        std::sort(
            candidates.begin(), candidates.end(), [](const BFNeighbor &a, const BFNeighbor &b) {
                if (a.dist_sq != b.dist_sq) return a.dist_sq < b.dist_sq;
                return a.target_idx < b.target_idx;
            }
        );

        // Robust tie-detection using full sorted candidates before truncation
        for (size_t i = 0; i < candidates.size(); ++i) {
            bool tied = false;
#if defined(FASTTREE_INTEGER_COORDS)
            if (i > 0 && candidates[i].dist_sq == candidates[i - 1].dist_sq) tied = true;
            if (i + 1 < candidates.size() && candidates[i].dist_sq == candidates[i + 1].dist_sq)
                tied = true;
#else
            double d2 = int_dist_sq_to_phys(candidates[i].dist_sq, 100.0);
            if (i > 0) {
                double d2_prev = int_dist_sq_to_phys(candidates[i - 1].dist_sq, 100.0);
                if (std::abs(d2 - d2_prev) <= 1e-4 * std::max(d2, 1.0) + 1e-9) tied = true;
            }
            if (i + 1 < candidates.size()) {
                double d2_next = int_dist_sq_to_phys(candidates[i + 1].dist_sq, 100.0);
                if (std::abs(d2 - d2_next) <= 1e-4 * std::max(d2, 1.0) + 1e-9) tied = true;
            }
#endif
            candidates[i].is_tied = tied;
        }

        if (candidates.size() > static_cast<size_t>(k)) { candidates.resize(k); }
        all_neighbors[q] = std::move(candidates);
    }
    return all_neighbors;
}

// ================================================================
// Verification Helper: Compare GPU Query Results vs Brute Force
// Checks both distances AND indices (with tie-handling & duplicate checks)
// ================================================================
static bool verify_results(
    const std::string                          &test_name,
    const std::vector<size_t>                  &gpu_results,
    const std::vector<dist_t>                  &gpu_dists,
    const std::vector<std::vector<BFNeighbor>> &bf_expected,
    int                                         k,
    size_t                                      num_queries,
    double                                      box_size
) {
    bool   passed         = true;
    size_t mismatch_count = 0;

    for (size_t q = 0; q < num_queries; ++q) {
        const auto &expected  = bf_expected[q];
        size_t      exp_count = expected.size();

        // 1. Check for duplicate indices within a single query result
        std::vector<size_t> seen(gpu_results.begin() + q * k, gpu_results.begin() + q * k + k);
        seen.erase(std::remove(seen.begin(), seen.end(), static_cast<size_t>(-1)), seen.end());
        std::sort(seen.begin(), seen.end());
        if (std::adjacent_find(seen.begin(), seen.end()) != seen.end()) {
            if (mismatch_count < 5) {
                std::cerr << "  [DUPLICATE INDEX ERROR] " << test_name << " Query " << q
                          << " returned duplicate particle indices within its top-" << k
                          << " results!\n";
            }
            mismatch_count++;
            passed = false;
        }

        // 2. Check each returned rank's distance and index
        for (int i = 0; i < k; ++i) {
            size_t offset  = q * k + i;
            size_t gpu_idx = gpu_results[offset];
            dist_t gpu_d   = gpu_dists[offset];

            if (i < static_cast<int>(exp_count)) {
                dist_t exp_d = expected[i].dist_sq;

#if defined(FASTTREE_INTEGER_COORDS)
                // In integer coords, squared distances must match exactly
                if (gpu_d != exp_d) {
                    if (mismatch_count < 5) {
                        std::cerr << "  [DISTANCE MISMATCH] " << test_name << " Query " << q
                                  << " Rank " << i << " | GPU dist=" << gpu_d
                                  << " Exp dist=" << exp_d << " (GPU idx=" << gpu_idx
                                  << " Exp idx=" << expected[i].target_idx << ")\n";
                    }
                    mismatch_count++;
                    passed = false;
                }

                // Index match check: when not tied with any particle, index must match exactly
                if (!expected[i].is_tied && gpu_idx != expected[i].target_idx) {
                    if (mismatch_count < 5) {
                        std::cerr << "  [INDEX MISMATCH] " << test_name << " Query " << q
                                  << " Rank " << i << " | GPU idx=" << gpu_idx
                                  << " Exp idx=" << expected[i].target_idx << " (dist=" << gpu_d
                                  << ")\n";
                    }
                    mismatch_count++;
                    passed = false;
                }
#else
                double gpu_d2 = int_dist_sq_to_phys(gpu_d, box_size);
                double exp_d2 = int_dist_sq_to_phys(exp_d, box_size);
                double tol    = 1e-4 * std::max(exp_d2, 1.0) + 1e-9;

                if (std::abs(gpu_d2 - exp_d2) > tol) {
                    if (mismatch_count < 5) {
                        std::cerr << "  [DISTANCE MISMATCH] " << test_name << " Query " << q
                                  << " Rank " << i << " | GPU d2=" << gpu_d2 << " Exp d2=" << exp_d2
                                  << " (GPU idx=" << gpu_idx
                                  << " Exp idx=" << expected[i].target_idx << ")\n";
                    }
                    mismatch_count++;
                    passed = false;
                }

                // Index match check (float tolerance for ties)
                if (!expected[i].is_tied && gpu_idx != expected[i].target_idx) {
                    if (mismatch_count < 5) {
                        std::cerr << "  [INDEX MISMATCH] " << test_name << " Query " << q
                                  << " Rank " << i << " | GPU idx=" << gpu_idx
                                  << " Exp idx=" << expected[i].target_idx << " (d2=" << gpu_d2
                                  << ")\n";
                    }
                    mismatch_count++;
                    passed = false;
                }
#endif
            } else {
                // Sentinels when available particles < k
                if (gpu_idx != static_cast<size_t>(-1)) {
                    if (mismatch_count < 5) {
                        std::cerr << "  [SENTINEL MISMATCH] " << test_name << " Query " << q
                                  << " Rank " << i << " expected sentinel IDX_NONE, got " << gpu_idx
                                  << "\n";
                    }
                    mismatch_count++;
                    passed = false;
                }
            }
        }
    }

    if (!passed) {
        std::cerr << "  FAILED: " << test_name << " (" << mismatch_count << " mismatches)\n";
    } else {
        std::cout << "  PASSED: " << test_name << " (all " << num_queries
                  << " queries match brute-force indices & distances)\n";
    }
    return passed;
}

// ================================================================
// Synthetic Particle Generators
// ================================================================
static particles<coord_t>
generate_uniform_particles(size_t n, double box_min, double box_size, unsigned int seed = 42) {
    particles<coord_t> p;
    p.pos_x.resize(n);
    p.pos_y.resize(n);
    p.pos_z.resize(n);
    p.id.resize(n);

    std::mt19937                           gen(seed);
    std::uniform_real_distribution<double> dis(box_min, box_min + box_size);

    for (size_t i = 0; i < n; ++i) {
        p.id[i]    = static_cast<uint32_t>(i);
        p.pos_x[i] = phys_pos_to_int(dis(gen), box_min, box_size);
        p.pos_y[i] = phys_pos_to_int(dis(gen), box_min, box_size);
        p.pos_z[i] = phys_pos_to_int(dis(gen), box_min, box_size);
    }
    return p;
}

static particles<coord_t>
generate_clustered_particles(size_t n, double box_min, double box_size, unsigned int seed = 12345) {
    particles<coord_t> p;
    p.pos_x.resize(n);
    p.pos_y.resize(n);
    p.pos_z.resize(n);
    p.id.resize(n);

    std::mt19937                           gen(seed);
    std::normal_distribution<double>       cluster1(box_min + 0.25 * box_size, 0.04 * box_size);
    std::normal_distribution<double>       cluster2(box_min + 0.75 * box_size, 0.04 * box_size);
    std::uniform_real_distribution<double> uniform_dis(box_min, box_min + box_size);

    for (size_t i = 0; i < n; ++i) {
        p.id[i] = static_cast<uint32_t>(i);
        double fx, fy, fz;
        if (i < n * 0.45) {
            fx = cluster1(gen);
            fy = cluster1(gen);
            fz = cluster1(gen);
        } else if (i < n * 0.85) {
            fx = cluster2(gen);
            fy = cluster2(gen);
            fz = cluster2(gen);
        } else {
            fx = uniform_dis(gen);
            fy = uniform_dis(gen);
            fz = uniform_dis(gen);
        }
        // Clamp to box bounds
        fx = std::clamp(fx, box_min + 1e-4, box_min + box_size - 1e-4);
        fy = std::clamp(fy, box_min + 1e-4, box_min + box_size - 1e-4);
        fz = std::clamp(fz, box_min + 1e-4, box_min + box_size - 1e-4);

        p.pos_x[i] = phys_pos_to_int(fx, box_min, box_size);
        p.pos_y[i] = phys_pos_to_int(fy, box_min, box_size);
        p.pos_z[i] = phys_pos_to_int(fz, box_min, box_size);
    }
    return p;
}

static particles<coord_t>
generate_degenerate_particles(size_t n, double box_min, double box_size, unsigned int seed = 999) {
    particles<coord_t> p;
    p.pos_x.resize(n);
    p.pos_y.resize(n);
    p.pos_z.resize(n);
    p.id.resize(n);

    std::mt19937                           gen(seed);
    std::uniform_real_distribution<double> dis(box_min + 0.1 * box_size, box_min + 0.9 * box_size);

    // Create several co-located groups (duplicate coordinates)
    std::vector<std::tuple<double, double, double>> duplicate_centers;
    for (int c = 0; c < 10; ++c) {
        duplicate_centers.push_back({dis(gen), dis(gen), dis(gen)});
    }

    for (size_t i = 0; i < n; ++i) {
        p.id[i] = static_cast<uint32_t>(i);
        double fx, fy, fz;
        if (i < n * 0.6) {
            // Pick one of the duplicate centers
            auto [cx, cy, cz] = duplicate_centers[i % duplicate_centers.size()];
            fx                = cx;
            fy                = cy;
            fz                = cz;
        } else {
            fx = dis(gen);
            fy = dis(gen);
            fz = dis(gen);
        }
        p.pos_x[i] = phys_pos_to_int(fx, box_min, box_size);
        p.pos_y[i] = phys_pos_to_int(fy, box_min, box_size);
        p.pos_z[i] = phys_pos_to_int(fz, box_min, box_size);
    }
    return p;
}

// ================================================================
// SECTION 4: Isolated Unit Tests for Heaps
// Validates RegisterMaxHeap, LocalMaxHeap, and SortedMergeHeap in isolation
// ================================================================
static bool test_isolated_heaps(sycl::queue &q) {
    std::cout << "\n=================================================================\n";
    std::cout << "Testing Isolated Heap Units: RegisterMaxHeap, LocalMaxHeap, SortedMergeHeap\n";
    std::cout << "=================================================================\n";

    bool all_ok = true;

    // 1. Isolated LocalMaxHeap Unit Test
    {
        constexpr int WARP   = 32;
        constexpr int CAP    = 64;
        constexpr int TEST_K = 48;

        int         *d_out_idx  = sycl::malloc_shared<int>(WARP * CAP, q);
        heap_dist_t *d_out_dist = sycl::malloc_shared<heap_dist_t>(WARP * CAP, q);

        q.submit([&](sycl::handler &h) {
             sycl::local_accessor<heap_dist_t, 1> sh_dist(WARP * CAP, h);
             sycl::local_accessor<int, 1>         sh_idx(WARP * CAP, h);

             h.parallel_for(sycl::nd_range<1>(WARP, WARP), [=](sycl::nd_item<1> item) {
                 int lane = static_cast<int>(item.get_local_id(0));
                 LocalMaxHeap<heap_dist_t, int>::init(lane, sh_dist, sh_idx, CAP);
                 int count = 0;

                 // Push 100 pseudo-random numbers per lane (including duplicates)
                 for (int val = 100; val >= 1; --val) {
                     heap_dist_t d   = static_cast<heap_dist_t>((val * 17 + lane * 13) % 73);
                     int         idx = val;
                     if (!LocalMaxHeap<heap_dist_t, int>::should_prune(
                             lane, d, sh_dist, count, TEST_K, CAP
                         )) {
                         LocalMaxHeap<heap_dist_t, int>::push(
                             lane, d, idx, count, sh_dist, sh_idx, TEST_K, CAP
                         );
                     }
                 }

                 LocalMaxHeap<heap_dist_t, int>::sort_in_place(lane, count, sh_dist, sh_idx, CAP);

                 int base = lane * CAP;
                 for (int i = 0; i < CAP; ++i) {
                     d_out_dist[base + i] = sh_dist[base + i];
                     d_out_idx[base + i]  = sh_idx[base + i];
                 }
             });
         }).wait();

        // Host verification for LocalMaxHeap
        bool local_heap_ok = true;
        for (int lane = 0; lane < WARP; ++lane) {
            std::vector<std::pair<heap_dist_t, int>> expected;
            for (int val = 100; val >= 1; --val) {
                heap_dist_t d = static_cast<heap_dist_t>((val * 17 + lane * 13) % 73);
                expected.push_back({d, val});
            }
            std::sort(expected.begin(), expected.end());
            if (expected.size() > TEST_K) expected.resize(TEST_K);

            int base = lane * CAP;
            for (int i = 0; i < TEST_K; ++i) {
                if (d_out_dist[base + i] != expected[i].first) {
                    local_heap_ok = false;
                    break;
                }
            }
        }

        if (local_heap_ok) {
            std::cout << "  PASSED: Isolated LocalMaxHeap test (32 lanes, k=48, bitonic sorted)\n";
        } else {
            std::cerr << "  FAILED: Isolated LocalMaxHeap test\n";
            all_ok = false;
        }

        sycl::free(d_out_idx, q);
        sycl::free(d_out_dist, q);
    }

    // 2. Isolated SortedMergeHeap Unit Test
    {
        constexpr int WARP   = 32;
        constexpr int CAP    = 64;
        constexpr int TEST_K = 48;

        int         *d_out_idx  = sycl::malloc_shared<int>(CAP, q);
        heap_dist_t *d_out_dist = sycl::malloc_shared<heap_dist_t>(CAP, q);

        q.submit([&](sycl::handler &h) {
             sycl::local_accessor<heap_dist_t, 1> sh_dist(CAP, h);
             sycl::local_accessor<int, 1>         sh_idx(CAP, h);
             sycl::local_accessor<heap_dist_t, 1> sh_stage_dist(WARP, h);
             sycl::local_accessor<int, 1>         sh_stage_idx(WARP, h);
             sycl::local_accessor<int, 1>         sh_count(1, h);

             h.parallel_for(sycl::nd_range<1>(WARP, WARP), [=](sycl::nd_item<1> item) {
                 int lid = static_cast<int>(item.get_local_id(0));
                 SortedMergeHeap<heap_dist_t, int>::init(item, sh_dist, sh_idx, sh_count, CAP);

                 // Round 1: Lane lid pushes (31 - lid) * 10
                 heap_dist_t d1   = static_cast<heap_dist_t>((31 - lid) * 10);
                 int         idx1 = lid;
                 SortedMergeHeap<heap_dist_t, int>::merge_batch(
                     item,
                     sh_dist,
                     sh_idx,
                     sh_count,
                     sh_stage_dist,
                     sh_stage_idx,
                     d1,
                     idx1,
                     TEST_K,
                     CAP
                 );

                 // Round 2: Lane lid pushes (lid % 4) * 5 (duplicate values, some smaller than
                 // round 1)
                 heap_dist_t d2   = static_cast<heap_dist_t>((lid % 4) * 5);
                 int         idx2 = 100 + lid;
                 SortedMergeHeap<heap_dist_t, int>::merge_batch(
                     item,
                     sh_dist,
                     sh_idx,
                     sh_count,
                     sh_stage_dist,
                     sh_stage_idx,
                     d2,
                     idx2,
                     TEST_K,
                     CAP
                 );

                 // Round 3: Half lanes inactive (IDX_NONE)
                 heap_dist_t d3 = (lid < 16) ? static_cast<heap_dist_t>(lid + 1)
                                             : SortedMergeHeap<heap_dist_t, int>::DIST_MAX;
                 int idx3 = (lid < 16) ? (200 + lid) : SortedMergeHeap<heap_dist_t, int>::IDX_NONE;
                 SortedMergeHeap<heap_dist_t, int>::merge_batch(
                     item,
                     sh_dist,
                     sh_idx,
                     sh_count,
                     sh_stage_dist,
                     sh_stage_idx,
                     d3,
                     idx3,
                     TEST_K,
                     CAP
                 );

                 for (int i = lid; i < CAP; i += WARP) {
                     d_out_dist[i] = sh_dist[i];
                     d_out_idx[i]  = sh_idx[i];
                 }
             });
         }).wait();

        // Host verification for SortedMergeHeap
        std::vector<std::pair<heap_dist_t, int>> expected;
        for (int lid = 0; lid < WARP; ++lid) {
            expected.push_back({static_cast<heap_dist_t>((31 - lid) * 10), lid});
            expected.push_back({static_cast<heap_dist_t>((lid % 4) * 5), 100 + lid});
            if (lid < 16) { expected.push_back({static_cast<heap_dist_t>(lid + 1), 200 + lid}); }
        }
        std::sort(expected.begin(), expected.end());
        if (expected.size() > TEST_K) expected.resize(TEST_K);

        bool sorted_merge_ok = true;
        for (int i = 0; i < TEST_K; ++i) {
            if (d_out_dist[i] != expected[i].first) {
                sorted_merge_ok = false;
                std::cerr << "  [MISMATCH SortedMergeHeap] Rank " << i
                          << ": got dist=" << d_out_dist[i] << " exp=" << expected[i].first << "\n";
            }
        }

        if (sorted_merge_ok) {
            std::cout << "  PASSED: Isolated SortedMergeHeap test (warp-wide merge_batch across 3 "
                         "rounds)\n";
        } else {
            std::cerr << "  FAILED: Isolated SortedMergeHeap test\n";
            all_ok = false;
        }

        sycl::free(d_out_idx, q);
        sycl::free(d_out_dist, q);
    }

    return all_ok;
}

// ================================================================
// Test Runner: Self-kNN and Subsets across k values
// ================================================================
static bool test_self_knn_dataset(
    sycl::queue              &q,
    const particles<coord_t> &p,
    const std::string        &dist_name,
    const std::vector<int>   &k_list,
    double                    box_size
) {
    size_t n = p.pos_x.size();
    std::cout << "\n=================================================================\n";
    std::cout << "Testing Self-kNN on " << dist_name << " (N = " << n << ")\n";
    std::cout << "=================================================================\n";

    TreeSoA tree(q, n);
    build_bvh(q, p, tree);
    q.wait();

    bool all_ok = true;

    for (int k : k_list) {
        for (bool exclude_self : {true, false}) {
            std::string label = dist_name + " | k=" + std::to_string(k) +
                                " | exclude_self=" + (exclude_self ? "true" : "false");

            std::vector<size_t> results(n * k);
            std::vector<dist_t> result_dists(n * k);
            size_t             *d_res   = sycl::malloc_device<size_t>(n * k, q);
            dist_t             *d_dists = sycl::malloc_device<dist_t>(n * k, q);

            self_knn_query(q, tree, k, d_res, d_dists, exclude_self);
            q.wait();

            q.memcpy(results.data(), d_res, n * k * sizeof(size_t)).wait();
            q.memcpy(result_dists.data(), d_dists, n * k * sizeof(dist_t)).wait();

            std::vector<int> all_leaf_ranks(n);
            std::iota(all_leaf_ranks.begin(), all_leaf_ranks.end(), 0);

            auto bf_expected =
                compute_host_brute_force_knn(tree, n, all_leaf_ranks, k, exclude_self);
            bool ok = verify_results(label, results, result_dists, bf_expected, k, n, box_size);
            if (!ok) all_ok = false;

            sycl::free(d_res, q);
            sycl::free(d_dists, q);
        }

        // Test self_knn_query_subset on a hand-picked subset of leaf_ids (both exclude_self=true
        // and false)
        if (n > 4) {
            std::vector<int> subset_leaf_ids;
            for (size_t i = 0; i < n; i += std::max<size_t>(1, n / 17)) {
                subset_leaf_ids.push_back(static_cast<int>(i));
            }
            // Add boundary elements
            subset_leaf_ids.push_back(0);
            subset_leaf_ids.push_back(static_cast<int>(n - 1));
            subset_leaf_ids.push_back(static_cast<int>(n / 2));
            std::sort(subset_leaf_ids.begin(), subset_leaf_ids.end());
            subset_leaf_ids.erase(
                std::unique(subset_leaf_ids.begin(), subset_leaf_ids.end()), subset_leaf_ids.end()
            );

            size_t num_subset = subset_leaf_ids.size();
            int   *d_leaf_ids = sycl::malloc_device<int>(num_subset, q);
            q.memcpy(d_leaf_ids, subset_leaf_ids.data(), num_subset * sizeof(int)).wait();

            for (bool exclude_self : {true, false}) {
                std::string sub_label = dist_name + " [SUBSET num=" + std::to_string(num_subset) +
                                        "] | k=" + std::to_string(k) +
                                        " | exclude_self=" + (exclude_self ? "true" : "false");

                std::vector<size_t> sub_results(num_subset * k);
                std::vector<dist_t> sub_result_dists(num_subset * k);
                size_t             *d_sub_res   = sycl::malloc_device<size_t>(num_subset * k, q);
                dist_t             *d_sub_dists = sycl::malloc_device<dist_t>(num_subset * k, q);

                self_knn_query_subset(
                    q,
                    tree,
                    d_leaf_ids,
                    static_cast<int>(num_subset),
                    k,
                    d_sub_res,
                    d_sub_dists,
                    exclude_self
                );
                q.wait();

                q.memcpy(sub_results.data(), d_sub_res, num_subset * k * sizeof(size_t)).wait();
                q.memcpy(sub_result_dists.data(), d_sub_dists, num_subset * k * sizeof(dist_t))
                    .wait();

                auto bf_sub_expected =
                    compute_host_brute_force_knn(tree, n, subset_leaf_ids, k, exclude_self);
                bool ok = verify_results(
                    sub_label,
                    sub_results,
                    sub_result_dists,
                    bf_sub_expected,
                    k,
                    num_subset,
                    box_size
                );
                if (!ok) all_ok = false;

                sycl::free(d_sub_res, q);
                sycl::free(d_sub_dists, q);
            }

            sycl::free(d_leaf_ids, q);
        }
    }

    tree.free(q);
    return all_ok;
}

// ================================================================
// Adversarial Interleaved Subset Test
// Interleaves physically separated clusters across warp lanes to maximize divergence
// ================================================================
static bool test_adversarial_interleaved_subsets(
    sycl::queue              &q,
    const particles<coord_t> &p,
    const std::vector<int>   &k_list,
    double                    box_min,
    double                    box_size
) {
    size_t n = p.pos_x.size();
    std::cout << "\n=================================================================\n";
    std::cout << "Testing Adversarial Interleaved Subsets (Max-Divergence across Warp Lanes) (N = "
              << n << ")\n";
    std::cout << "=================================================================\n";

    TreeSoA tree(q, n);
    build_bvh(q, p, tree);
    q.wait();

    bool           all_ok       = true;
    size_t         num_internal = tree.num_internal;
    const coord_t *leaf_x       = tree.min_x + num_internal;

    // Identify actual leaf ranks belonging to Cluster 1 (left side) and Cluster 2 (right side)
    std::vector<int> cluster1_leaves, cluster2_leaves;
    coord_t          mid_x = phys_pos_to_int(box_min + 0.5 * box_size, box_min, box_size);

    for (size_t i = 0; i < n; ++i) {
        if (leaf_x[i] < mid_x) {
            cluster1_leaves.push_back(static_cast<int>(i));
        } else {
            cluster2_leaves.push_back(static_cast<int>(i));
        }
    }

    if (cluster1_leaves.empty() || cluster2_leaves.empty()) {
        std::cerr << "  Warning: could not partition clusters for adversarial interleave.\n";
        tree.free(q);
        return true;
    }

    // Build interleaved leaf_ids where adjacent lanes alternate between Cluster 1 and Cluster 2
    constexpr size_t num_interleaved = 32; // exactly 1 SIMD warp
    std::vector<int> interleaved_ids(num_interleaved);
    for (size_t lane = 0; lane < num_interleaved; ++lane) {
        if (lane % 2 == 0) {
            interleaved_ids[lane] = cluster1_leaves[(lane / 2) % cluster1_leaves.size()];
        } else {
            interleaved_ids[lane] = cluster2_leaves[(lane / 2) % cluster2_leaves.size()];
        }
    }

    int *d_leaf_ids = sycl::malloc_device<int>(num_interleaved, q);
    q.memcpy(d_leaf_ids, interleaved_ids.data(), num_interleaved * sizeof(int)).wait();

    for (int k : k_list) {
        for (bool exclude_self : {true, false}) {
            std::string label =
                "Interleaved Cluster1/2 (Warp Divergence) | k=" + std::to_string(k) +
                " | exclude_self=" + (exclude_self ? "true" : "false");

            std::vector<size_t> results(num_interleaved * k);
            std::vector<dist_t> result_dists(num_interleaved * k);
            size_t             *d_res   = sycl::malloc_device<size_t>(num_interleaved * k, q);
            dist_t             *d_dists = sycl::malloc_device<dist_t>(num_interleaved * k, q);

            self_knn_query_subset(
                q,
                tree,
                d_leaf_ids,
                static_cast<int>(num_interleaved),
                k,
                d_res,
                d_dists,
                exclude_self
            );
            q.wait();

            q.memcpy(results.data(), d_res, num_interleaved * k * sizeof(size_t)).wait();
            q.memcpy(result_dists.data(), d_dists, num_interleaved * k * sizeof(dist_t)).wait();

            auto bf_expected =
                compute_host_brute_force_knn(tree, n, interleaved_ids, k, exclude_self);
            bool ok = verify_results(
                label, results, result_dists, bf_expected, k, num_interleaved, box_size
            );
            if (!ok) all_ok = false;

            sycl::free(d_res, q);
            sycl::free(d_dists, q);
        }
    }

    sycl::free(d_leaf_ids, q);
    tree.free(q);
    return all_ok;
}

// ================================================================
// Test Runner: Old knn_query / knn_query_large_k Regression Check
// ================================================================
static bool test_knn_query_regression(
    sycl::queue &q, const particles<coord_t> &p, const std::vector<int> &k_list, double box_size
) {
    size_t n = p.pos_x.size();
    std::cout << "\n=================================================================\n";
    std::cout << "Testing knn_query Regression (SortedMergeHeap verification) (N = " << n << ")\n";
    std::cout << "=================================================================\n";

    TreeSoA tree(q, n);
    build_bvh(q, p, tree);
    q.wait();

    bool all_ok = true;

    // Pick query points across the box
    size_t               num_q = std::min<size_t>(n, 32);
    std::vector<coord_t> qx(num_q), qy(num_q), qz(num_q);
    for (size_t i = 0; i < num_q; ++i) {
        qx[i] = p.pos_x[i * (n / num_q)];
        qy[i] = p.pos_y[i * (n / num_q)];
        qz[i] = p.pos_z[i * (n / num_q)];
    }

    coord_t *d_qx = sycl::malloc_device<coord_t>(num_q, q);
    coord_t *d_qy = sycl::malloc_device<coord_t>(num_q, q);
    coord_t *d_qz = sycl::malloc_device<coord_t>(num_q, q);
    q.memcpy(d_qx, qx.data(), num_q * sizeof(coord_t)).wait();
    q.memcpy(d_qy, qy.data(), num_q * sizeof(coord_t)).wait();
    q.memcpy(d_qz, qz.data(), num_q * sizeof(coord_t)).wait();

    for (int k : k_list) {
        std::string         label = "knn_query regression | k=" + std::to_string(k);
        std::vector<size_t> results(num_q * k);
        std::vector<dist_t> result_dists(num_q * k);
        size_t             *d_res   = sycl::malloc_device<size_t>(num_q * k, q);
        dist_t             *d_dists = sycl::malloc_device<dist_t>(num_q * k, q);

        knn_query(q, tree, d_qx, d_qy, d_qz, k, static_cast<int>(num_q), d_res, d_dists);
        q.wait();

        q.memcpy(results.data(), d_res, num_q * k * sizeof(size_t)).wait();
        q.memcpy(result_dists.data(), d_dists, num_q * k * sizeof(dist_t)).wait();

        // Host brute-force for arbitrary query points
        std::vector<std::vector<BFNeighbor>> bf_expected(num_q);
        size_t                               num_internal = tree.num_internal;
        const coord_t                       *leaf_x       = tree.min_x + num_internal;
        const coord_t                       *leaf_y       = tree.min_y + num_internal;
        const coord_t                       *leaf_z       = tree.min_z + num_internal;
        const int                           *orig_idx     = tree.orig_idx;

        for (size_t qi = 0; qi < num_q; ++qi) {
            std::vector<BFNeighbor> cand;
            cand.reserve(n);
            for (size_t j = 0; j < n; ++j) {
                dist_t d2 =
                    compute_point_dist_sq(qx[qi], qy[qi], qz[qi], leaf_x[j], leaf_y[j], leaf_z[j]);
#ifdef RETURN_ORIG_INDICES
                size_t ret_idx = static_cast<size_t>(orig_idx[j + num_internal]);
#else
                size_t ret_idx = j;
#endif
                cand.push_back({d2, ret_idx, leaf_x[j], leaf_y[j], leaf_z[j]});
            }
            std::sort(cand.begin(), cand.end(), [](const BFNeighbor &a, const BFNeighbor &b) {
                if (a.dist_sq != b.dist_sq) return a.dist_sq < b.dist_sq;
                return a.target_idx < b.target_idx;
            });
            if (cand.size() > static_cast<size_t>(k)) cand.resize(k);
            bf_expected[qi] = std::move(cand);
        }

        bool ok = verify_results(label, results, result_dists, bf_expected, k, num_q, box_size);
        if (!ok) all_ok = false;

        sycl::free(d_res, q);
        sycl::free(d_dists, q);
    }

    sycl::free(d_qx, q);
    sycl::free(d_qy, q);
    sycl::free(d_qz, q);
    tree.free(q);
    return all_ok;
}

// ================================================================
// Test Runner: Adversarial & Edge Cases (n=1, n=2, partial warps)
// ================================================================
static bool test_edge_cases(sycl::queue &q, double box_min, double box_size) {
    std::cout << "\n=================================================================\n";
    std::cout << "Testing Edge Cases: n=1, n=2, and non-warp multiples\n";
    std::cout << "=================================================================\n";

    bool all_ok = true;

    // Edge Case 1: n = 1 particle tree
    {
        particles<coord_t> p1;
        p1.pos_x = {phys_pos_to_int(box_min + 0.5 * box_size, box_min, box_size)};
        p1.pos_y = {phys_pos_to_int(box_min + 0.5 * box_size, box_min, box_size)};
        p1.pos_z = {phys_pos_to_int(box_min + 0.5 * box_size, box_min, box_size)};
        p1.id    = {0};

        TreeSoA tree1(q, 1);
        build_bvh(q, p1, tree1);
        q.wait();

        for (int k : {1, 2, 33}) {
            for (bool excl : {false, true}) {
                std::vector<size_t> res(k);
                std::vector<dist_t> dists(k);
                size_t             *d_r = sycl::malloc_device<size_t>(k, q);
                dist_t             *d_d = sycl::malloc_device<dist_t>(k, q);

                self_knn_query(q, tree1, k, d_r, d_d, excl);
                q.wait();

                q.memcpy(res.data(), d_r, k * sizeof(size_t)).wait();
                q.memcpy(dists.data(), d_d, k * sizeof(dist_t)).wait();

                auto        bf_exp = compute_host_brute_force_knn(tree1, 1, {0}, k, excl);
                std::string label =
                    "n=1 tree | k=" + std::to_string(k) + " | excl=" + (excl ? "true" : "false");
                bool ok = verify_results(label, res, dists, bf_exp, k, 1, box_size);
                if (!ok) all_ok = false;

                sycl::free(d_r, q);
                sycl::free(d_d, q);
            }
        }
        tree1.free(q);
    }

    // Edge Case 2: n = 2 particle tree
    {
        particles<coord_t> p2;
        p2.pos_x = {
            phys_pos_to_int(box_min + 0.2 * box_size, box_min, box_size),
            phys_pos_to_int(box_min + 0.8 * box_size, box_min, box_size)
        };
        p2.pos_y = {
            phys_pos_to_int(box_min + 0.2 * box_size, box_min, box_size),
            phys_pos_to_int(box_min + 0.8 * box_size, box_min, box_size)
        };
        p2.pos_z = {
            phys_pos_to_int(box_min + 0.2 * box_size, box_min, box_size),
            phys_pos_to_int(box_min + 0.8 * box_size, box_min, box_size)
        };
        p2.id = {0, 1};

        TreeSoA tree2(q, 2);
        build_bvh(q, p2, tree2);
        q.wait();

        for (int k : {1, 2, 5, 33}) {
            for (bool excl : {false, true}) {
                std::vector<size_t> res(2 * k);
                std::vector<dist_t> dists(2 * k);
                size_t             *d_r = sycl::malloc_device<size_t>(2 * k, q);
                dist_t             *d_d = sycl::malloc_device<dist_t>(2 * k, q);

                self_knn_query(q, tree2, k, d_r, d_d, excl);
                q.wait();

                q.memcpy(res.data(), d_r, 2 * k * sizeof(size_t)).wait();
                q.memcpy(dists.data(), d_d, 2 * k * sizeof(dist_t)).wait();

                auto        bf_exp = compute_host_brute_force_knn(tree2, 2, {0, 1}, k, excl);
                std::string label =
                    "n=2 tree | k=" + std::to_string(k) + " | excl=" + (excl ? "true" : "false");
                bool ok = verify_results(label, res, dists, bf_exp, k, 2, box_size);
                if (!ok) all_ok = false;

                sycl::free(d_r, q);
                sycl::free(d_d, q);
            }
        }
        tree2.free(q);
    }

    // Edge Case 3: Non-warp multiples (num_queries % 32 != 0)
    for (size_t test_n : {7, 31, 33, 63, 65, 99}) {
        auto    pn = generate_uniform_particles(test_n, box_min, box_size, 777);
        TreeSoA treen(q, test_n);
        build_bvh(q, pn, treen);
        q.wait();

        for (int k : {4, 32, 33, 64}) {
            std::vector<size_t> res(test_n * k);
            std::vector<dist_t> dists(test_n * k);
            size_t             *d_r = sycl::malloc_device<size_t>(test_n * k, q);
            dist_t             *d_d = sycl::malloc_device<dist_t>(test_n * k, q);

            self_knn_query(q, treen, k, d_r, d_d, true);
            q.wait();

            q.memcpy(res.data(), d_r, test_n * k * sizeof(size_t)).wait();
            q.memcpy(dists.data(), d_d, test_n * k * sizeof(dist_t)).wait();

            std::vector<int> all_ranks(test_n);
            std::iota(all_ranks.begin(), all_ranks.end(), 0);
            auto bf_exp = compute_host_brute_force_knn(treen, test_n, all_ranks, k, true);

            std::string label =
                "Non-warp-multiple n=" + std::to_string(test_n) + " | k=" + std::to_string(k);
            bool ok = verify_results(label, res, dists, bf_exp, k, test_n, box_size);
            if (!ok) all_ok = false;

            sycl::free(d_r, q);
            sycl::free(d_d, q);
        }
        treen.free(q);
    }

    return all_ok;
}

// ================================================================
// Main Test Entry Point
// ================================================================
int main(int argc, char **argv) {
    sycl::queue q{sycl::property::queue::in_order{}};
    std::cout << "=================================================================\n";
    std::cout << "FastTree Self-kNN & SortedMergeHeap Release 1.2.0-beta Test Suite\n";
    std::cout << "Device: " << q.get_device().get_info<sycl::info::device::name>() << "\n";
    auto sg_widths = q.get_device().get_info<sycl::info::device::sub_group_sizes>();
    std::cout << "Supported Sub-Group Sizes: [ ";
    for (auto w : sg_widths)
        std::cout << w << " ";
    std::cout << "] (Active Native Width: " << get_native_sub_group_width(q) << ")\n";
#if defined(FASTTREE_INTEGER_COORDS)
    std::cout << "Coordinate Mode: INTEGER (BITS_PER_DIMENSION = " << BITS_PER_DIMENSION << ")\n";
#else
    std::cout << "Coordinate Mode: FLOATING-POINT (double precision)\n";
#endif
#if defined(PERIODIC_BC)
    std::cout << "Boundary Conditions: PERIODIC\n";
#else
    std::cout << "Boundary Conditions: NON-PERIODIC\n";
#endif
    std::cout << "=================================================================\n";

    const double box_min  = 0.0;
    const double box_max  = 100.0;
    const double box_size = box_max - box_min;

    bool all_passed = true;

    // 0. Isolated Unit Tests for Heaps (LocalMaxHeap, SortedMergeHeap)
    if (!test_isolated_heaps(q)) { all_passed = false; }

    // k-boundary test set hitting both small k (RegisterMaxHeap) and large k (LocalMaxHeap)
    std::vector<int> k_test_set = {16, 32, 33, 64, 128, 256};

    // 1. Edge cases: n=1, n=2, partial warps
    if (!test_edge_cases(q, box_min, box_size)) { all_passed = false; }

    // 2. Uniform random distribution (N = 1000)
    {
        auto p_uni = generate_uniform_particles(1000, box_min, box_size, 42);
        if (!test_self_knn_dataset(q, p_uni, "Uniform Random (N=1000)", k_test_set, box_size)) {
            all_passed = false;
        }
        if (!test_knn_query_regression(q, p_uni, {16, 32, 33, 64, 128}, box_size)) {
            all_passed = false;
        }
    }

    // 3. Clustered cosmological density distribution (N = 1000)
    {
        auto p_clust = generate_clustered_particles(1000, box_min, box_size, 12345);
        if (!test_self_knn_dataset(
                q, p_clust, "Cosmological Clustered (N=1000)", k_test_set, box_size
            )) {
            all_passed = false;
        }
        // Adversarial interleaved subset test (interleaving cluster 1 and 2 within the same warp)
        if (!test_adversarial_interleaved_subsets(q, p_clust, k_test_set, box_min, box_size)) {
            all_passed = false;
        }
    }

    // 4. Degenerate / duplicate points (N = 1000)
    {
        auto p_degen = generate_degenerate_particles(1000, box_min, box_size, 999);
        if (!test_self_knn_dataset(
                q, p_degen, "Degenerate / Duplicate Points (N=1000)", k_test_set, box_size
            )) {
            all_passed = false;
        }
    }

    // 5. Larger N = 10k dataset test
    {
        size_t           n_large = 10000;
        auto             p_large = generate_clustered_particles(n_large, box_min, box_size, 54321);
        std::vector<int> k_large_set = {32, 33, 64, 128};
        if (!test_self_knn_dataset(
                q, p_large, "Cosmological Clustered (N=10000)", k_large_set, box_size
            )) {
            all_passed = false;
        }
        if (!test_adversarial_interleaved_subsets(q, p_large, k_large_set, box_min, box_size)) {
            all_passed = false;
        }
    }

    std::cout << "\n=================================================================\n";
    if (all_passed) {
        std::cout << ">>> ALL SELF-KNN & HEAP UNIT TESTS PASSED SUCCESSFULLY! <<<\n";
    } else {
        std::cerr << ">>> SOME SELF-KNN & HEAP UNIT TESTS FAILED! <<<\n";
    }
    std::cout << "=================================================================\n";

    return all_passed ? 0 : 1;
}
