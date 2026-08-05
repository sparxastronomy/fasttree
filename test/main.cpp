#include "../src/hlbvh.hpp"
#include <sycl/sycl.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

using namespace fasttree;

// ----------------------------------------------------------------
// Helper: convert a stored knn_dists entry back to physical dist^2
// ----------------------------------------------------------------
static double int_dist_sq_to_phys(dist_t stored, double box_size) {
#if defined(FASTTREE_INTEGER_COORDS)
    // stored = sum of ((axis_gap >> _DIST_SHIFT)^2) over 3 axes
    // axis_gap is in integer units where full box = 2^BITS_PER_DIMENSION
    // physical_gap = axis_gap * box_size / 2^BITS_PER_DIMENSION
    // after shift: shifted_gap = axis_gap >> _DIST_SHIFT
    // physical_gap = shifted_gap * box_size * 2^_DIST_SHIFT / 2^BITS_PER_DIMENSION
    //              = shifted_gap * box_size / 2^(BITS_PER_DIMENSION - _DIST_SHIFT)
    constexpr double int_to_norm =
        1.0 / static_cast<double>(1ULL << (BITS_PER_DIMENSION - _DIST_SHIFT));
    double phys_scale = box_size * int_to_norm;
    return static_cast<double>(stored) * phys_scale * phys_scale;
#else
    (void)box_size;
    return static_cast<double>(stored);
#endif
}

// Convert a physical radius to an integer coordinate difference
static coord_t phys_radius_to_int(double r, double box_size) {
#if defined(FASTTREE_INTEGER_COORDS)
    double norm = r / box_size;
    // integer units: norm * 2^BITS_PER_DIMENSION
    // clamp to avoid overflow
    double int_val = norm * std::ldexp(1.0, BITS_PER_DIMENSION);
    return static_cast<coord_t>(int_val);
#else
    return static_cast<coord_t>(r);
#endif
}

// Convert integer coordinate position to physical
static double int_pos_to_phys(coord_t pos, double box_min, double box_size) {
#if defined(FASTTREE_INTEGER_COORDS)
    return int_rep_to_float(pos, box_min, box_size);
#else
    return static_cast<double>(pos);
#endif
}

// Convert physical position to integer coordinate
static coord_t phys_pos_to_int(double phys, double box_min, double box_size) {
#if defined(FASTTREE_INTEGER_COORDS)
    double inv_dx = 1.0 / box_size;
    return float_to_int_rep(phys, box_min, inv_dx);
#else
    return static_cast<coord_t>(phys);
#endif
}

// Periodic distance on [box_min, box_max) in physical coordinates
static double periodic_dist(double a, double b, double box_size) {
    double d = std::abs(a - b);
    if (d > 0.5 * box_size) d = box_size - d;
    return d;
}

// ----------------------------------------------------------------
int main() {
    auto        *q_ptr = new sycl::queue(sycl::default_selector_v);
    sycl::queue &q     = *q_ptr;
    std::cout << "Selected Device: " << q.get_device().get_info<sycl::info::device::name>() << "\n";

    const double box_min  = 0.0;
    const double box_max  = 100.0;
    const double box_size = box_max - box_min;

    // ================================================================
    // TEST SETUP: 10x10x10 regular grid
    // ================================================================
    const int grid_dim = 10;
    const int n        = grid_dim * grid_dim * grid_dim;

    particles<coord_t> p;
    p.pos_x.resize(n);
    p.pos_y.resize(n);
    p.pos_z.resize(n);
    p.id.resize(n);

    for (int ix = 0; ix < grid_dim; ++ix) {
        for (int iy = 0; iy < grid_dim; ++iy) {
            for (int iz = 0; iz < grid_dim; ++iz) {
                int    idx   = ix * grid_dim * grid_dim + iy * grid_dim + iz;
                double fx    = box_min + (ix + 0.5) * (box_size / grid_dim);
                double fy    = box_min + (iy + 0.5) * (box_size / grid_dim);
                double fz    = box_min + (iz + 0.5) * (box_size / grid_dim);
                p.id[idx]    = static_cast<uint32_t>(idx);
                p.pos_x[idx] = phys_pos_to_int(fx, box_min, box_size);
                p.pos_y[idx] = phys_pos_to_int(fy, box_min, box_size);
                p.pos_z[idx] = phys_pos_to_int(fz, box_min, box_size);
            }
        }
    }

    // ================================================================
    // BOUNDING BOX & TREE CONSTRUCTION
    // ================================================================
    BoundingBox<coord_t> bbox = compute_bbox(q, p, n);
#if !defined(FASTTREE_INTEGER_COORDS)
    bbox = BoundingBox<coord_t>(
        static_cast<coord_t>(box_min),
        static_cast<coord_t>(box_max),
        static_cast<coord_t>(box_min),
        static_cast<coord_t>(box_max),
        static_cast<coord_t>(box_min),
        static_cast<coord_t>(box_max)
    );
    printf(
        "Bounding Box: [%.3f, %.3f] x [%.3f, %.3f] x [%.3f, %.3f]\n",
        (double)bbox.min_x,
        (double)bbox.max_x,
        (double)bbox.min_y,
        (double)bbox.max_y,
        (double)bbox.min_z,
        (double)bbox.max_z
    );
#else
    std::cout << "Bounding Box computed for Integer Coordinates.\n";
#endif

    std::cout << "Building Tree...\n";
    TreeSoA tree(q, n);
    build_bvh(q, p, tree, &bbox);
    q.wait();

    bool success = true;

    // ================================================================
    // TEST 1: SFC Monotonicity
    // ================================================================
    {
        std::cout << "\n[TEST 1] SFC Monotonicity\n";
        int                leaf_offset = n - 1;
        particles<coord_t> tree_parts;
        tree_parts.pos_x.resize(n);
        tree_parts.pos_y.resize(n);
        tree_parts.pos_z.resize(n);
        for (int i = 0; i < n; ++i) {
            tree_parts.pos_x[i] = tree.min_x[leaf_offset + i];
            tree_parts.pos_y[i] = tree.min_y[leaf_offset + i];
            tree_parts.pos_z[i] = tree.min_z[leaf_offset + i];
        }
        std::vector<sfc_key> mk_check(n);
        sfc_encode(q, tree_parts, mk_check.data(), bbox);
        q.wait();

        bool mono = true;
        for (int i = 0; i < n - 1; ++i) {
            if (mk_check[i + 1] < mk_check[i]) {
                mono = false;
                break;
            }
        }
        if (mono) {
            std::cout << "  SUCCESS: SFC keys are monotonic.\n";
        } else {
            std::cout << "  FAILURE: SFC keys are NOT monotonic.\n";
            success = false;
        }
    }

    // ================================================================
    // TEST 2: Range Query (non-periodic)
    // ================================================================
    {
        std::cout << "\n[TEST 2] Range Query (non-periodic)\n";

        const double float_qx = 52.0, float_qy = 52.0, float_qz = 52.0;
        const double float_rmin = 0.0, float_rmax = 20.0;

        coord_t qx    = phys_pos_to_int(float_qx, box_min, box_size);
        coord_t qy    = phys_pos_to_int(float_qy, box_min, box_size);
        coord_t qz    = phys_pos_to_int(float_qz, box_min, box_size);
        coord_t r_min = phys_radius_to_int(float_rmin, box_size);
        coord_t r_max = phys_radius_to_int(float_rmax, box_size);

        coord_t *dqx    = sycl::malloc_shared<coord_t>(1, q);
        coord_t *dqy    = sycl::malloc_shared<coord_t>(1, q);
        coord_t *dqz    = sycl::malloc_shared<coord_t>(1, q);
        coord_t *drm    = sycl::malloc_shared<coord_t>(1, q);
        coord_t *dRM    = sycl::malloc_shared<coord_t>(1, q);
        int     *res    = sycl::malloc_shared<int>(n, q);
        int     *rescnt = sycl::malloc_shared<int>(1, q);

        dqx[0]    = qx;
        dqy[0]    = qy;
        dqz[0]    = qz;
        drm[0]    = r_min;
        dRM[0]    = r_max;
        rescnt[0] = 0;

        range_query(q, tree, dqx, dqy, dqz, drm, dRM, 1, res, rescnt, n);
        q.wait();

        // Brute-force reference (no periodic wrapping)
        int leaf_offset = n - 1;
        int expected    = 0;
        for (int i = 0; i < n; ++i) {
            double px = int_pos_to_phys(tree.min_x[leaf_offset + i], box_min, box_size);
            double py = int_pos_to_phys(tree.min_y[leaf_offset + i], box_min, box_size);
            double pz = int_pos_to_phys(tree.min_z[leaf_offset + i], box_min, box_size);
            double dx = px - float_qx, dy = py - float_qy, dz = pz - float_qz;
            double dist = std::sqrt(dx * dx + dy * dy + dz * dz);
            if (dist >= float_rmin && dist <= float_rmax) expected++;
        }

        std::cout << "  Found: " << rescnt[0] << "  Expected: " << expected << "\n";
        if (rescnt[0] != expected) {
            std::cout << "  FAILURE: Count mismatch.\n";
            success = false;
        } else {
            std::cout << "  SUCCESS.\n";
        }

        sycl::free(dqx, q);
        sycl::free(dqy, q);
        sycl::free(dqz, q);
        sycl::free(drm, q);
        sycl::free(dRM, q);
        sycl::free(res, q);
        sycl::free(rescnt, q);
    }

    // ================================================================
    // TEST 3: kNN Query — small k=5, large k=48
    // ================================================================
    {
        std::cout << "\n[TEST 3] kNN Query\n";

        const double float_qx = 52.0, float_qy = 52.0, float_qz = 52.0;
        coord_t      qx = phys_pos_to_int(float_qx, box_min, box_size);
        coord_t      qy = phys_pos_to_int(float_qy, box_min, box_size);
        coord_t      qz = phys_pos_to_int(float_qz, box_min, box_size);

        coord_t *dqx = sycl::malloc_shared<coord_t>(1, q);
        coord_t *dqy = sycl::malloc_shared<coord_t>(1, q);
        coord_t *dqz = sycl::malloc_shared<coord_t>(1, q);
        dqx[0]       = qx;
        dqy[0]       = qy;
        dqz[0]       = qz;

        int leaf_offset = n - 1;

        for (int k : {5, 48}) {
            size_t *knn_res   = sycl::malloc_shared<size_t>(k, q);
            dist_t *knn_dists = sycl::malloc_shared<dist_t>(k, q);

            knn_query<128>(q, tree, dqx, dqy, dqz, k, 1, knn_res, knn_dists);
            q.wait();

            // Brute-force: sort all particles by non-periodic distance
            std::vector<std::pair<double, int>> all_dists(n);
            for (int i = 0; i < n; ++i) {
                double px = int_pos_to_phys(tree.min_x[leaf_offset + i], box_min, box_size);
                double py = int_pos_to_phys(tree.min_y[leaf_offset + i], box_min, box_size);
                double pz = int_pos_to_phys(tree.min_z[leaf_offset + i], box_min, box_size);
                double dx = px - float_qx, dy = py - float_qy, dz = pz - float_qz;
                all_dists[i] = {dx * dx + dy * dy + dz * dz, i};
            }
            std::sort(all_dists.begin(), all_dists.end());

            std::cout << "  k=" << k << ":\n";
            bool knn_ok = true;
            for (int i = 0; i < k; ++i) {
                size_t returned_leaf = knn_res[i];
                double phys_d2       = int_dist_sq_to_phys(knn_dists[i], box_size);
                double bf_d2         = all_dists[i].first;
                // Allow 0.1% relative tolerance for quantization error
                double tol = 1e-3 * bf_d2 + 1e-10;
                if (std::abs(phys_d2 - bf_d2) > tol) {
                    std::cout << "    FAILURE at rank " << i << ": got d2=" << phys_d2
                              << " expected d2=" << bf_d2 << "\n";
                    knn_ok = false;
                }

                if (i < 5) {
                    uint32_t pid;
                    double   px, py, pz;
#if defined(RETURN_ORIG_INDICES)
                    int orig_idx = static_cast<int>(returned_leaf);
                    pid          = p.id[orig_idx];
                    px           = int_pos_to_phys(p.pos_x[orig_idx], box_min, box_size);
                    py           = int_pos_to_phys(p.pos_y[orig_idx], box_min, box_size);
                    pz           = int_pos_to_phys(p.pos_z[orig_idx], box_min, box_size);
#else
                    int li = leaf_offset + static_cast<int>(returned_leaf);
                    pid    = tree.id[li];
                    px     = int_pos_to_phys(tree.min_x[li], box_min, box_size);
                    py     = int_pos_to_phys(tree.min_y[li], box_min, box_size);
                    pz     = int_pos_to_phys(tree.min_z[li], box_min, box_size);
#endif
                    std::cout << "    [" << i << "] id=" << pid << " pos=(" << px << "," << py
                              << "," << pz << ")"
                              << " dist=" << std::sqrt(phys_d2) << "\n";
                }
            }
            if (knn_ok)
                std::cout << "  SUCCESS: all distances match brute force.\n";
            else
                success = false;

            sycl::free(knn_res, q);
            sycl::free(knn_dists, q);
        }

        sycl::free(dqx, q);
        sycl::free(dqy, q);
        sycl::free(dqz, q);
    }

#if defined(PERIODIC_BC) && defined(FASTTREE_INTEGER_COORDS)
    // ================================================================
    // TEST 4: Periodic Range Query
    //
    // Query point near the domain boundary so that the nearest images
    // of some particles are across the wrap-around.
    // Example: query at (2, 50, 50), r_max=10.
    // The particle at (95, 50, 50) is 7 units away periodically
    // (100 - 95 + 2 = 7) but 93 units away non-periodically.
    // ================================================================
    {
        std::cout << "\n[TEST 4] Periodic Range Query\n";

        const double float_qx = 2.0, float_qy = 50.0, float_qz = 50.0;
        const double float_rmin = 0.0, float_rmax = 10.0;

        coord_t qx    = phys_pos_to_int(float_qx, box_min, box_size);
        coord_t qy    = phys_pos_to_int(float_qy, box_min, box_size);
        coord_t qz    = phys_pos_to_int(float_qz, box_min, box_size);
        coord_t r_min = phys_radius_to_int(float_rmin, box_size);
        coord_t r_max = phys_radius_to_int(float_rmax, box_size);

        coord_t *dqx    = sycl::malloc_shared<coord_t>(1, q);
        coord_t *dqy    = sycl::malloc_shared<coord_t>(1, q);
        coord_t *dqz    = sycl::malloc_shared<coord_t>(1, q);
        coord_t *drm    = sycl::malloc_shared<coord_t>(1, q);
        coord_t *dRM    = sycl::malloc_shared<coord_t>(1, q);
        int     *res    = sycl::malloc_shared<int>(n, q);
        int     *rescnt = sycl::malloc_shared<int>(1, q);

        dqx[0]    = qx;
        dqy[0]    = qy;
        dqz[0]    = qz;
        drm[0]    = r_min;
        dRM[0]    = r_max;
        rescnt[0] = 0;

        range_query(q, tree, dqx, dqy, dqz, drm, dRM, 1, res, rescnt, n);
        q.wait();

        // Brute-force with periodic distance
        int leaf_offset = n - 1;
        int expected    = 0;
        for (int i = 0; i < n; ++i) {
            double px   = int_pos_to_phys(tree.min_x[leaf_offset + i], box_min, box_size);
            double py   = int_pos_to_phys(tree.min_y[leaf_offset + i], box_min, box_size);
            double pz   = int_pos_to_phys(tree.min_z[leaf_offset + i], box_min, box_size);
            double dx   = periodic_dist(px, float_qx, box_size);
            double dy   = periodic_dist(py, float_qy, box_size);
            double dz   = periodic_dist(pz, float_qz, box_size);
            double dist = std::sqrt(dx * dx + dy * dy + dz * dz);
            if (dist >= float_rmin && dist <= float_rmax) expected++;
        }

        std::cout << "  Query: (" << float_qx << "," << float_qy << "," << float_qz << ")"
                  << "  r_max=" << float_rmax << "\n";
        std::cout << "  Found: " << rescnt[0] << "  Expected (periodic BF): " << expected << "\n";

        // Show which particles were found and whether the wrap-around ones are included
        std::cout << "  Returned particles (physical coords):\n";
        for (int i = 0; i < rescnt[0]; ++i) {
            int    li   = leaf_offset + res[i];
            double px   = int_pos_to_phys(tree.min_x[li], box_min, box_size);
            double py   = int_pos_to_phys(tree.min_y[li], box_min, box_size);
            double pz   = int_pos_to_phys(tree.min_z[li], box_min, box_size);
            double dx   = periodic_dist(px, float_qx, box_size);
            double dy   = periodic_dist(py, float_qy, box_size);
            double dz   = periodic_dist(pz, float_qz, box_size);
            double dist = std::sqrt(dx * dx + dy * dy + dz * dz);
            std::cout << "    id=" << tree.id[li] << " pos=(" << px << "," << py << "," << pz << ")"
                      << " periodic_dist=" << dist << "\n";
        }

        if (rescnt[0] != expected) {
            std::cout << "  FAILURE: Count mismatch.\n";
            success = false;
        } else {
            std::cout << "  SUCCESS.\n";
        }

        sycl::free(dqx, q);
        sycl::free(dqy, q);
        sycl::free(dqz, q);
        sycl::free(drm, q);
        sycl::free(dRM, q);
        sycl::free(res, q);
        sycl::free(rescnt, q);
    }

    // ================================================================
    // TEST 5: Periodic kNN Query
    //
    // Query near boundary — k nearest neighbors should include
    // particles on the other side of the periodic boundary.
    // ================================================================
    {
        std::cout << "\n[TEST 5] Periodic kNN Query\n";

        const double float_qx = 2.0, float_qy = 50.0, float_qz = 50.0;
        const int    k = 5;

        coord_t qx = phys_pos_to_int(float_qx, box_min, box_size);
        coord_t qy = phys_pos_to_int(float_qy, box_min, box_size);
        coord_t qz = phys_pos_to_int(float_qz, box_min, box_size);

        coord_t *dqx     = sycl::malloc_shared<coord_t>(1, q);
        coord_t *dqy     = sycl::malloc_shared<coord_t>(1, q);
        coord_t *dqz     = sycl::malloc_shared<coord_t>(1, q);
        size_t  *knn_res = sycl::malloc_shared<size_t>(k, q);
        dist_t  *knn_d   = sycl::malloc_shared<dist_t>(k, q);

        dqx[0] = qx;
        dqy[0] = qy;
        dqz[0] = qz;

        knn_query<128>(q, tree, dqx, dqy, dqz, k, 1, knn_res, knn_d);
        q.wait();

        // Brute-force with periodic distance
        int                                 leaf_offset = n - 1;
        std::vector<std::pair<double, int>> all_dists(n);
        for (int i = 0; i < n; ++i) {
            double px    = int_pos_to_phys(tree.min_x[leaf_offset + i], box_min, box_size);
            double py    = int_pos_to_phys(tree.min_y[leaf_offset + i], box_min, box_size);
            double pz    = int_pos_to_phys(tree.min_z[leaf_offset + i], box_min, box_size);
            double dx    = periodic_dist(px, float_qx, box_size);
            double dy    = periodic_dist(py, float_qy, box_size);
            double dz    = periodic_dist(pz, float_qz, box_size);
            all_dists[i] = {dx * dx + dy * dy + dz * dz, i};
        }
        std::sort(all_dists.begin(), all_dists.end());

        std::cout << "  Query: (" << float_qx << "," << float_qy << "," << float_qz << ")  k=" << k
                  << "\n";
        std::cout << "  BVH results vs brute-force:\n";

        bool knn_ok = true;
        for (int i = 0; i < k; ++i) {
            double phys_d2 = int_dist_sq_to_phys(knn_d[i], box_size);
            double bf_d2   = all_dists[i].first;
            double tol     = 1e-3 * bf_d2 + 1e-10;

            uint32_t pid;
            double   px, py, pz;
#if defined(RETURN_ORIG_INDICES)
            int orig_idx = static_cast<int>(knn_res[i]);
            pid          = p.id[orig_idx];
            px           = int_pos_to_phys(p.pos_x[orig_idx], box_min, box_size);
            py           = int_pos_to_phys(p.pos_y[orig_idx], box_min, box_size);
            pz           = int_pos_to_phys(p.pos_z[orig_idx], box_min, box_size);
#else
            int li = leaf_offset + static_cast<int>(knn_res[i]);
            pid    = tree.id[li];
            px     = int_pos_to_phys(tree.min_x[li], box_min, box_size);
            py     = int_pos_to_phys(tree.min_y[li], box_min, box_size);
            pz     = int_pos_to_phys(tree.min_z[li], box_min, box_size);
#endif

            // Flag particles that came from across the periodic boundary
            bool is_wrapped =
                (std::abs(px - float_qx) > 0.5 * box_size ||
                 std::abs(py - float_qy) > 0.5 * box_size ||
                 std::abs(pz - float_qz) > 0.5 * box_size);

            std::cout << "    [" << i << "] id=" << pid << " pos=(" << px << "," << py << "," << pz
                      << ")"
                      << " dist=" << std::sqrt(phys_d2)
                      << (is_wrapped ? "  <-- periodic image" : "") << "\n";

            if (std::abs(phys_d2 - bf_d2) > tol) {
                std::cout << "    FAILURE at rank " << i << ": got d2=" << phys_d2
                          << " bf d2=" << bf_d2 << "\n";
                knn_ok = false;
            }
        }

        if (knn_ok)
            std::cout << "  SUCCESS: periodic kNN matches brute force.\n";
        else
            success = false;

        sycl::free(dqx, q);
        sycl::free(dqy, q);
        sycl::free(dqz, q);
        sycl::free(knn_res, q);
        sycl::free(knn_d, q);
    }
#endif // PERIODIC_BC

    // ================================================================
    tree.free(q);
    delete q_ptr;

    std::cout << "\n" << (success ? "ALL TESTS PASSED" : "SOME TESTS FAILED") << "\n";
    return success ? 0 : 1;
}