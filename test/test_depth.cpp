#include "../src/hlbvh.hpp"
#include <iostream>

using namespace fasttree;

int main() {
  sycl::queue q;
  const int n = 1000;

  float *sx = sycl::malloc_shared<float>(n, q);
  float *sy = sycl::malloc_shared<float>(n, q);
  float *sz = sycl::malloc_shared<float>(n, q);
  uint64_t *smk = sycl::malloc_shared<uint64_t>(n, q);

  for (int i = 0; i < n; ++i) {
    sx[i] = i;
    sy[i] = 0;
    sz[i] = 0;
    smk[i] = i;  // dummy sorted morton keys
  }

  TreeSoA tree(q, n);
  build_tree(q, tree, smk, sx, sy, sz);
  q.wait();

  // compute depth
  int max_depth = 0;
  for (int i = n - 1; i < 2 * n - 1; ++i) {
    int depth = 0;
    int curr = i;
    while (curr != 0) {
      curr = tree.parent[curr];
      depth++;
    }
    if (depth > max_depth) max_depth = depth;
  }
  std::cout << "Max depth: " << max_depth << "\n";

  return 0;
}