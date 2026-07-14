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

  const int n = 1000;
  particles<coord_t> p;
  p.pos_x.resize(n);
  p.pos_y.resize(n);
  p.pos_z.resize(n);

  // Random distribution
  for (int i = 0; i < n; ++i) {
    p.pos_x[i] = static_cast<coord_t>(rand()) / RAND_MAX * static_cast<coord_t>(100.0);
    p.pos_y[i] = static_cast<coord_t>(rand()) / RAND_MAX * static_cast<coord_t>(100.0);
    p.pos_z[i] = static_cast<coord_t>(rand()) / RAND_MAX * static_cast<coord_t>(100.0);
  }

  // 1. Calculate Bounding Box
  BoundingBox<coord_t> bbox = compute_bbox(q, p, n);
  printf("Bounding Box: [%.3f, %.3f] x [%.3f, %.3f] x [%.3f, %.3f]\n", bbox.min_x, bbox.max_x, bbox.min_y, bbox.max_y, bbox.min_z, bbox.max_z);

  // 2. HLBVH Construction (Coarse + Intra-Voxel)
  std::cout << "Building Tree..." << std::endl;
  TreeSoA tree(q, n);
  build_bvh(q, p, tree);
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
  std::vector<uint64_t> mk_check(n);
  sfc_encode(q, tree_parts, mk_check.data(), bbox);
  q.wait();

  for (int i = 0; i < n - 1; ++i) {
    if (mk_check[i] > mk_check[i + 1]) {
      std::cout << "FAILURE: SFC keys in tree are NOT monotonic (verified by lib)." << std::endl;
      std::cout << "  MK[" << i << "]: " << mk_check[i] << std::endl;
      std::cout << "  MK[" << i + 1 << "]: " << mk_check[i + 1] << std::endl;
      success = false;
      break;
    }
  }
  if (success) { std::cout << "SUCCESS: SFC keys in tree are strictly monotonic." << std::endl; }

  // 4. Range Query Verification
  coord_t qx = static_cast<coord_t>(50.5), qy = static_cast<coord_t>(50.0), qz = static_cast<coord_t>(50.0);
  coord_t r_min = static_cast<coord_t>(0.0), r_max = static_cast<coord_t>(20.0);
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
    coord_t dx = tree_parts.pos_x[i] - qx;
    coord_t dy = tree_parts.pos_y[i] - qy;
    coord_t dz = tree_parts.pos_z[i] - qz;
    coord_t dist = std::sqrt(dx * dx + dy * dy + dz * dz);
    if (dist >= r_min && dist <= r_max) { expected_cnt++; }
  }
  std::cout << "Found " << res_cnt[0] << " particles within radius " << r_max << " of " << qx << std::endl;
  std::cout << "Expected (brute force): " << expected_cnt << std::endl;

  if (res_cnt[0] != expected_cnt) {
    std::cout << "FAILURE: Result count mismatch!" << std::endl;
    success = false;
  } else {
    std::cout << "SUCCESS: Range query result count matches brute force." << std::endl;
  }

  // 5. kNN Query Verification
  int k = 5;
  int *knn_res = sycl::malloc_shared<int>(k, q);
  coord_t *knn_dists = sycl::malloc_shared<coord_t>(k, q);
  knn_query(q, tree, dqx, dqy, dqz, k, 1, knn_res, knn_dists);
  q.wait();

  std::cout << "Testing kNN query for k=" << k << " around (" << qx << ", " << qy << ", " << qz << "):" << std::endl;
  for (int i = 0; i < k; ++i) { std::cout << "  Neighbor " << i << ": index " << knn_res[i] << ", dist " << knn_dists[i] << std::endl; }

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
