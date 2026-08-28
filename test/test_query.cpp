#include "hlbvh.hpp"
#include <iostream>

using namespace fasttree;

int main() {
    sycl::queue q;
    const int   n = 10;

    printf("Running on: %s\n", q.get_device().get_info<sycl::info::device::name>().c_str());

    coord_t  *sx  = sycl::malloc_shared<coord_t>(n, q);
    coord_t  *sy  = sycl::malloc_shared<coord_t>(n, q);
    coord_t  *sz  = sycl::malloc_shared<coord_t>(n, q);
    uint64_t *smk = sycl::malloc_shared<uint64_t>(n, q);

    for (int i = 0; i < n; ++i) {
        sx[i]  = static_cast<coord_t>(i);
        sy[i]  = static_cast<coord_t>(0);
        sz[i]  = static_cast<coord_t>(0);
        smk[i] = i; // dummy sorted morton keys
    }

    TreeSoA tree(q, n);
    build_tree(q, tree, smk, sx, sy, sz);
    q.wait();

    coord_t qx = static_cast<coord_t>(5.5), qy = static_cast<coord_t>(0.0),
            qz = static_cast<coord_t>(0.0);
    coord_t rm = static_cast<coord_t>(0.0), RM = static_cast<coord_t>(2.0);

    int  max_res = 10;
    int *res     = sycl::malloc_shared<int>(max_res, q);
    int *res_cnt = sycl::malloc_shared<int>(1, q);
    res_cnt[0]   = 0;

    q.submit([&](sycl::handler &h) {
         h.single_task([=]() {
             int stack[64];
             int stack_ptr      = 0;
             stack[stack_ptr++] = 0;
             int count          = 0;

             while (stack_ptr > 0) {
                 int node_idx = stack[--stack_ptr];

                 coord_t bmin_x = tree.min_x[node_idx], bmax_x = tree.max_x[node_idx];
                 coord_t bmin_y = tree.min_y[node_idx], bmax_y = tree.max_y[node_idx];
                 coord_t bmin_z = tree.min_z[node_idx], bmax_z = tree.max_z[node_idx];

                 coord_t dx =
                     sycl::fmax(bmin_x - qx, sycl::fmax(static_cast<coord_t>(0.0), qx - bmax_x));
                 coord_t dy =
                     sycl::fmax(bmin_y - qy, sycl::fmax(static_cast<coord_t>(0.0), qy - bmax_y));
                 coord_t dz =
                     sycl::fmax(bmin_z - qz, sycl::fmax(static_cast<coord_t>(0.0), qz - bmax_z));
                 coord_t d2 = dx * dx + dy * dy + dz * dz;

                 if (d2 <= RM * RM) {
                     if (node_idx >= n - 1) {
                         if (d2 >= rm * rm && count < max_res) {
                             res[count++] = node_idx - (n - 1);
                         }
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
    for (int i = 0; i < res_cnt[0]; ++i) {
        std::cout << "Particle " << res[i] << " at x=" << sx[res[i]] << "\n";
    }

    sycl::free(sx, q);
    sycl::free(sy, q);
    sycl::free(sz, q);
    sycl::free(smk, q);
    sycl::free(res, q);
    sycl::free(res_cnt, q);
    tree.free(q);

    return 0;
}