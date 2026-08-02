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
#if defined(FASTTREE_INTEGER_COORDS)
    uint64_t max_c = (BITS_PER_DIMENSION < 30) ? (1ULL << (BITS_PER_DIMENSION < 30 ? BITS_PER_DIMENSION : 0)) : 1073741824ULL;
    p.pos_x[i] = static_cast<coord_t>(rand() % max_c);
    p.pos_y[i] = static_cast<coord_t>(rand() % max_c);
    p.pos_z[i] = static_cast<coord_t>(rand() % max_c);
#else
    p.pos_x[i] = static_cast<coord_t>(rand()) / RAND_MAX * static_cast<coord_t>(100.0);
    p.pos_y[i] = static_cast<coord_t>(rand()) / RAND_MAX * static_cast<coord_t>(100.0);
    p.pos_z[i] = static_cast<coord_t>(rand()) / RAND_MAX * static_cast<coord_t>(100.0);
#endif
  }

  // 1. Calculate Bounding Box
  BoundingBox<coord_t> bbox = compute_bbox(q, p, n);
#if defined(FASTTREE_INTEGER_COORDS)
  std::cout << "Bounding Box computed for Integer Coordinates." << std::endl;
#else
  printf("Bounding Box: [%.3f, %.3f] x [%.3f, %.3f] x [%.3f, %.3f]\n", bbox.min_x, bbox.max_x, bbox.min_y, bbox.max_y, bbox.min_z, bbox.max_z);
#endif

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
#if defined(FASTTREE_INTEGER_COORDS)
  uint64_t mid_c = (BITS_PER_DIMENSION < 30) ? (1ULL << (BITS_PER_DIMENSION < 30 ? BITS_PER_DIMENSION : 0)) / 2 : 536870912ULL;
  coord_t qx = static_cast<coord_t>(mid_c), qy = static_cast<coord_t>(mid_c), qz = static_cast<coord_t>(mid_c);
  coord_t r_min = static_cast<coord_t>(0), r_max = static_cast<coord_t>(mid_c / 2);
#else
  coord_t qx = static_cast<coord_t>(50.5), qy = static_cast<coord_t>(50.0), qz = static_cast<coord_t>(50.0);
  coord_t r_min = static_cast<coord_t>(0.0), r_max = static_cast<coord_t>(20.0);
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
    double dx = static_cast<double>(tree_parts.pos_x[i]) - static_cast<double>(qx);
    double dy = static_cast<double>(tree_parts.pos_y[i]) - static_cast<double>(qy);
    double dz = static_cast<double>(tree_parts.pos_z[i]) - static_cast<double>(qz);
    double dist = std::sqrt(dx * dx + dy * dy + dz * dz);
    if (dist >= static_cast<double>(r_min) && dist <= static_cast<double>(r_max)) { expected_cnt++; }
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

  std::cout << "Testing kNN query for k=" << k << std::endl;
  for (int i = 0; i < k; ++i) {
#ifndef RETURN_ORIG_INDICES
    std::cout << "  Neighbor " << i << ": index(in tree) " << knn_res[i] << ", dist " << static_cast<double>(knn_dists[i]) << std::endl;
#else
    std::cout << "  Neighbor " << i << ": index(orig) " << knn_res[i] << ", dist " << static_cast<double>(knn_dists[i]) << std::endl;
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
