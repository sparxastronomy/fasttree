#include "../src/hlbvh.hpp"
#include <iostream>

using namespace fasttree;

int main() {
  sycl::queue q;
  const int n = 10;

  coord_t *sx = sycl::malloc_shared<coord_t>(n, q);
  coord_t *sy = sycl::malloc_shared<coord_t>(n, q);
  coord_t *sz = sycl::malloc_shared<coord_t>(n, q);
  uint64_t *smk = sycl::malloc_shared<uint64_t>(n, q);

  for (int i = 0; i < n; ++i) {
    sx[i] = static_cast<coord_t>(i);
    sy[i] = static_cast<coord_t>(0);
    sz[i] = static_cast<coord_t>(0);
    smk[i] = i;  // dummy sorted morton keys
  }

  TreeSoA tree(q, n);
  build_tree(q, tree, smk, sx, sy, sz);
  q.wait();

  std::cout << "Root BB: X[" << tree.min_x[0] << ", " << tree.max_x[0] << "]\n";
  for (int i = 0; i < n - 1; ++i) {
    std::cout << "Node " << i << " (Parent " << tree.parent[i] << ") L:" << tree.left_child[i] << " R:" << tree.right_child[i] << " BB: X["
              << tree.min_x[i] << ", " << tree.max_x[i] << "]\n";
  }
  for (int i = n - 1; i < 2 * n - 1; ++i) {
    std::cout << "Leaf " << i << " (Parent " << tree.parent[i] << ") BB: X[" << tree.min_x[i] << ", " << tree.max_x[i] << "]\n";
  }

  sycl::free(sx, q);
  sycl::free(sy, q);
  sycl::free(sz, q);
  sycl::free(smk, q);
  tree.free(q);

  return 0;
}