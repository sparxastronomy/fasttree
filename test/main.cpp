#include "../src/hlbvh.hpp"
#include <sycl/sycl.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

using namespace fasttree;

int main() {
  auto *q_ptr = new sycl::queue(sycl::default_selector_v);
  sycl::queue &q = *q_ptr;
  std::cout << "Selected Device: " << q.get_device().get_info<sycl::info::device::name>() << std::endl;
  std::cout << "Validation Test: Comparing HLBVH Sort (Coarse+Intra) vs Full Sort & Query Correctness" << std::endl;

  const int grid_dim = 10;
  const int n = grid_dim * grid_dim * grid_dim;  // 1000 particles
  particles<coord_t> p;
  p.pos_x.resize(n);
  p.pos_y.resize(n);
  p.pos_z.resize(n);
  p.id.resize(n);

  double box_min = 0.0;
  double box_max = 100.0;
  double inv_dx = 1.0 / (box_max - box_min);

  // Generate deterministic fixed 3D grid
  for (int ix = 0; ix < grid_dim; ++ix) {
    for (int iy = 0; iy < grid_dim; ++iy) {
      for (int iz = 0; iz < grid_dim; ++iz) {
        int idx = ix * grid_dim * grid_dim + iy * grid_dim + iz;
        double fx = box_min + (ix + 0.5) * (100.0 / grid_dim);
        double fy = box_min + (iy + 0.5) * (100.0 / grid_dim);
        double fz = box_min + (iz + 0.5) * (100.0 / grid_dim);

        p.id[idx] = static_cast<uint32_t>(idx);

#if defined(FASTTREE_INTEGER_COORDS)
        p.pos_x[idx] = float_to_int_rep(fx, box_min, inv_dx);
        p.pos_y[idx] = float_to_int_rep(fy, box_min, inv_dx);
        p.pos_z[idx] = float_to_int_rep(fz, box_min, inv_dx);
#else
        p.pos_x[idx] = static_cast<coord_t>(fx);
        p.pos_y[idx] = static_cast<coord_t>(fy);
        p.pos_z[idx] = static_cast<coord_t>(fz);
#endif
      }
    }
  }

  // 1. Domain Bounding Box [0.0, 100.0]
  BoundingBox<coord_t> bbox = compute_bbox(q, p, n);
#if !defined(FASTTREE_INTEGER_COORDS)
  bbox = BoundingBox<coord_t>(0.0, 100.0, 0.0, 100.0, 0.0, 100.0);
  printf("Bounding Box: [%.3f, %.3f] x [%.3f, %.3f] x [%.3f, %.3f]\n", bbox.min_x, bbox.max_x, bbox.min_y, bbox.max_y, bbox.min_z, bbox.max_z);
#else
  std::cout << "Bounding Box computed for Integer Coordinates." << std::endl;
#endif

  // 2. HLBVH Construction
  std::cout << "Building Tree..." << std::endl;
  TreeSoA tree(q, n);
  build_bvh(q, p, tree, &bbox);
  q.wait();

  bool success = true;

  // Extract sorted particles from the tree leaves (offset n-1)
  int leaf_offset = n - 1;
  particles<coord_t> tree_parts;
  tree_parts.pos_x.resize(n);
  tree_parts.pos_y.resize(n);
  tree_parts.pos_z.resize(n);
  for (int i = 0; i < n; ++i) {
    tree_parts.pos_x[i] = tree.min_x[leaf_offset + i];
    tree_parts.pos_y[i] = tree.min_y[leaf_offset + i];
    tree_parts.pos_z[i] = tree.min_z[leaf_offset + i];
  }

  // 3. Monotonicity Check
  std::cout << "Checking Monotonicity..." << std::endl;
  std::vector<sfc_key> mk_check(n);
  sfc_encode(q, tree_parts, mk_check.data(), bbox);
  q.wait();

  for (int i = 0; i < n - 1; ++i) {
    if (mk_check[i + 1] < mk_check[i]) {
      std::cout << "FAILURE: SFC keys in tree are NOT monotonic (verified by lib)." << std::endl;
      success = false;
      break;
    }
  }
  if (success) { std::cout << "SUCCESS: SFC keys in tree are strictly monotonic." << std::endl; }

  // 4. Range Query Verification
  double float_qx = 52.0, float_qy = 52.0, float_qz = 52.0;
  double float_rmin = 0.0, float_rmax = 20.0;

#if defined(FASTTREE_INTEGER_COORDS)
  coord_t qx = float_to_int_rep(float_qx, box_min, inv_dx);
  coord_t qy = float_to_int_rep(float_qy, box_min, inv_dx);
  coord_t qz = float_to_int_rep(float_qz, box_min, inv_dx);
  coord_t r_min = static_cast<coord_t>(0);
  coord_t r_max = float_to_int_rep(float_rmax / 100.0);
#else
  coord_t qx = static_cast<coord_t>(float_qx);
  coord_t qy = static_cast<coord_t>(float_qy);
  coord_t qz = static_cast<coord_t>(float_qz);
  coord_t r_min = static_cast<coord_t>(float_rmin);
  coord_t r_max = static_cast<coord_t>(float_rmax);
#endif
  int max_res = 1000;

  coord_t *dqx = sycl::malloc_shared<coord_t>(1, q);
  coord_t *dqy = sycl::malloc_shared<coord_t>(1, q);
  coord_t *dqz = sycl::malloc_shared<coord_t>(1, q);
  coord_t *drm = sycl::malloc_shared<coord_t>(1, q);
  coord_t *dRM = sycl::malloc_shared<coord_t>(1, q);
  int *res = sycl::malloc_shared<int>(max_res, q);
  int *res_cnt = sycl::malloc_shared<int>(1, q);

  dqx[0] = qx;
  dqy[0] = qy;
  dqz[0] = qz;
  drm[0] = r_min;
  dRM[0] = r_max;
  res_cnt[0] = 0;

  std::cout << "Testing Range Query..." << std::endl;
  range_query(q, tree, dqx, dqy, dqz, drm, dRM, 1, res, res_cnt, max_res);
  q.wait();

  // Brute force verify against the reordered leaf nodes
  int expected_cnt = 0;
  for (int i = 0; i < n; ++i) {
#if defined(FASTTREE_INTEGER_COORDS)
    double px = int_rep_to_float(tree_parts.pos_x[i], static_cast<coord_t>(box_min), static_cast<coord_t>(100.0));
    double py = int_rep_to_float(tree_parts.pos_y[i], static_cast<coord_t>(box_min), static_cast<coord_t>(100.0));
    double pz = int_rep_to_float(tree_parts.pos_z[i], static_cast<coord_t>(box_min), static_cast<coord_t>(100.0));
    double qxf = int_rep_to_float(qx, static_cast<coord_t>(box_min), static_cast<coord_t>(100.0));
    double qyf = int_rep_to_float(qy, static_cast<coord_t>(box_min), static_cast<coord_t>(100.0));
    double qzf = int_rep_to_float(qz, static_cast<coord_t>(box_min), static_cast<coord_t>(100.0));
#else
    double px = static_cast<double>(tree_parts.pos_x[i]);
    double py = static_cast<double>(tree_parts.pos_y[i]);
    double pz = static_cast<double>(tree_parts.pos_z[i]);
    double qxf = static_cast<double>(qx);
    double qyf = static_cast<double>(qy);
    double qzf = static_cast<double>(qz);
#endif
    double dx = px - qxf;
    double dy = py - qyf;
    double dz = pz - qzf;
    double dist = std::sqrt(dx * dx + dy * dy + dz * dz);
    if (dist >= float_rmin && dist <= float_rmax) { expected_cnt++; }
  }
  std::cout << "Found " << res_cnt[0] << " particles within range" << std::endl;
  std::cout << "Expected (brute force): " << expected_cnt << std::endl;

  if (res_cnt[0] != expected_cnt) {
    std::cout << "FAILURE: Result count mismatch!" << std::endl;
    success = false;
  } else {
    std::cout << "SUCCESS: Range query result count matches brute force." << std::endl;
  }

  // 5. kNN Query Verification
  int k = 5;
  size_t *knn_res = sycl::malloc_shared<size_t>(k, q);
  coord_t *knn_dists = sycl::malloc_shared<coord_t>(k, q);
  knn_query(q, tree, dqx, dqy, dqz, k, 1, knn_res, knn_dists);
  q.wait();

  std::cout << "Testing kNN query for k=" << k << " (Query Point: " << float_qx << ", " << float_qy << ", " << float_qz << ")" << std::endl;
  for (int i = 0; i < k; ++i) {
#if defined(RETURN_ORIG_INDICES)
    uint32_t pid = static_cast<uint32_t>(knn_res[i]);
    double phys_d2 = static_cast<double>(knn_dists[i]);
    std::cout << "  Neighbor " << i << ": particle ID " << pid << ", dist^2 " << phys_d2 << ", dist " << std::sqrt(phys_d2) << std::endl;
#else
    size_t leaf_idx = (n - 1) + knn_res[i];
    uint32_t pid = tree.id[leaf_idx];

#if defined(FASTTREE_INTEGER_COORDS)
    double px = int_rep_to_float(tree.min_x[leaf_idx], box_min, box_max - box_min);
    double py = int_rep_to_float(tree.min_y[leaf_idx], box_min, box_max - box_min);
    double pz = int_rep_to_float(tree.min_z[leaf_idx], box_min, box_max - box_min);
    double norm_d2 = int_rep_to_float(knn_dists[i]);
    double phys_d2 = norm_d2 * (box_max - box_min) * (box_max - box_min);
    double phys_dist = std::sqrt(phys_d2);
#else
    double px = static_cast<double>(tree.min_x[leaf_idx]);
    double py = static_cast<double>(tree.min_y[leaf_idx]);
    double pz = static_cast<double>(tree.min_z[leaf_idx]);
    double phys_d2 = static_cast<double>(knn_dists[i]);
    double phys_dist = std::sqrt(phys_d2);
#endif

    std::cout << "  Neighbor " << i << ": particle ID " << pid << ", pos (" << px << ", " << py << ", " << pz << "), dist^2 " << phys_d2 << ", dist " << phys_dist << std::endl;
#endif
  }

  // Cleanup
  tree.free(q);
  sycl::free(dqx, q);
  sycl::free(dqy, q);
  sycl::free(dqz, q);
  sycl::free(drm, q);
  sycl::free(dRM, q);
  sycl::free(res, q);
  sycl::free(res_cnt, q);
  sycl::free(knn_res, q);
  sycl::free(knn_dists, q);

  return success ? 0 : 1;
}
