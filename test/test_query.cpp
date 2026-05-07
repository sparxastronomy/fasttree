#include "../src/hlbvh.hpp"
#include <iostream>

using namespace fasttree;

int main() {
  sycl::queue q;
  const int n = 10;

  printf("Running on: %s\n", q.get_device().get_info<sycl::info::device::name>().c_str());

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

  float qx = 5.5f, qy = 0.0f, qz = 0.0f;
  float rm = 0.0f, RM = 2.0f;

  int max_res = 10;
  int *res = sycl::malloc_shared<int>(max_res, q);
  int *res_cnt = sycl::malloc_shared<int>(1, q);
  res_cnt[0] = 0;

  q.submit([&](sycl::handler &h) {
     h.single_task([=]() {
       int stack[64];
       int stack_ptr = 0;
       stack[stack_ptr++] = 0;
       int count = 0;

       // Temporary for debugging output
       // We shouldn't use cout in single_task on GPU but it's okay for host/CPU or simple tests,
       // Better to use sycl::ext::oneapi::experimental::printf

       while (stack_ptr > 0) {
         int node_idx = stack[--stack_ptr];

         float bmin_x = tree.min_x[node_idx], bmax_x = tree.max_x[node_idx];
         float bmin_y = tree.min_y[node_idx], bmax_y = tree.max_y[node_idx];
         float bmin_z = tree.min_z[node_idx], bmax_z = tree.max_z[node_idx];

         float dx = std::max(bmin_x - qx, std::max(0.0f, qx - bmax_x));
         float dy = std::max(bmin_y - qy, std::max(0.0f, qy - bmax_y));
         float dz = std::max(bmin_z - qz, std::max(0.0f, qz - bmax_z));
         float d2 = dx * dx + dy * dy + dz * dz;

         // if it were printf
         // sycl::ext::oneapi::experimental::printf("Visit %d, BB[%.1f, %.1f], d2=%.2f\n", node_idx, bmin_x, bmax_x, d2);

         if (d2 <= RM * RM) {
           if (node_idx >= n - 1) {
             if (d2 >= rm * rm && count < max_res) { res[count++] = node_idx - (n - 1); }
           } else {
             stack[stack_ptr++] = tree.right_child[node_idx];
             stack[stack_ptr++] = tree.left_child[node_idx];
           }
         }
       }
       res_cnt[0] = count;
     });
   }).wait();

  std::cout << "Found " << res_cnt[0] << " particles\n";
  for (int i = 0; i < res_cnt[0]; ++i) { std::cout << "Particle " << res[i] << " at x=" << sx[res[i]] << "\n"; }

  return 0;
}