#include "../src/hlbvh.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <sycl/sycl.hpp>

using namespace fasttree;

int main() {
    sycl::queue q;
    std::cout << "Validation Test: Comparing HLBVH Sort (Coarse+Intra) vs Full Sort & Query Correctness" << std::endl;

    const int n = 1000;
    particles<float> p;
    p.pos_x.resize(n); p.pos_y.resize(n); p.pos_z.resize(n);

    // Random distribution
    for (int i = 0; i < n; ++i) {
        p.pos_x[i] = static_cast<float>(rand()) / RAND_MAX * 100.0f;
        p.pos_y[i] = static_cast<float>(rand()) / RAND_MAX * 100.0f;
        p.pos_z[i] = static_cast<float>(rand()) / RAND_MAX * 100.0f;
    }

    // 1. Calculate Bounding Box
    BoundingBox bbox = {p.pos_x[0], p.pos_x[0], p.pos_y[0], p.pos_y[0], p.pos_z[0], p.pos_z[0]};
    for (size_t i = 1; i < n; ++i) {
        bbox.min_x = std::min(bbox.min_x, p.pos_x[i]); bbox.max_x = std::max(bbox.max_x, p.pos_x[i]);
        bbox.min_y = std::min(bbox.min_y, p.pos_y[i]); bbox.max_y = std::max(bbox.max_y, p.pos_y[i]);
        bbox.min_z = std::min(bbox.min_z, p.pos_z[i]); bbox.max_z = std::max(bbox.max_z, p.pos_z[i]);
    }

    // 2. HLBVH Construction (Coarse + Intra-Voxel)
    std::cout << "Building Tree..." << std::endl;
    TreeSoA tree(q, n);
    build_bvh(q, p, tree); 
    q.wait();

    bool success = true;

    // Extract sorted particles from the tree leaves (offset n-1)
    int leaf_offset = n - 1;
    particles<float> tree_parts;
    tree_parts.pos_x.resize(n); tree_parts.pos_y.resize(n); tree_parts.pos_z.resize(n);
    for(int i=0; i<n; ++i) {
        tree_parts.pos_x[i] = tree.min_x[leaf_offset + i];
        tree_parts.pos_y[i] = tree.min_y[leaf_offset + i];
        tree_parts.pos_z[i] = tree.min_z[leaf_offset + i];
    }

    // 3. Monotonicity Check
    std::cout << "Checking Monotonicity..." << std::endl;
    std::vector<uint64_t> mk_check(n);
    morton_encode(q, tree_parts, mk_check, bbox);
    q.wait();
    
    for(int i=0; i<n-1; ++i) {
        if(mk_check[i] > mk_check[i+1]) {
            std::cout << "FAILURE: Morton keys in tree are NOT monotonic (verified by lib)." << std::endl;
            std::cout << "  MK[" << i << "]: " << mk_check[i] << std::endl;
            std::cout << "  MK[" << i+1 << "]: " << mk_check[i+1] << std::endl;
            success = false;
            break;
        }
    }
    if (success) {
        std::cout << "SUCCESS: Morton keys in tree are strictly monotonic." << std::endl;
    }

    // 4. Range Query Verification
    float qx = 50.5f, qy = 50.0f, qz = 50.0f;
    float r_min = 0.0f, r_max = 20.0f;
    int max_res = 1000;

    float *dqx = sycl::malloc_shared<float>(1, q);
    float *dqy = sycl::malloc_shared<float>(1, q);
    float *dqz = sycl::malloc_shared<float>(1, q);
    float *drm = sycl::malloc_shared<float>(1, q);
    float *dRM = sycl::malloc_shared<float>(1, q);
    int *res = sycl::malloc_shared<int>(max_res, q);
    int *res_cnt = sycl::malloc_shared<int>(1, q);

    dqx[0] = qx; dqy[0] = qy; dqz[0] = qz;
    drm[0] = r_min; dRM[0] = r_max;
    res_cnt[0] = 0;

    std::cout << "Testing Range Query..." << std::endl;
    range_query(q, tree, dqx, dqy, dqz, drm, dRM, 1, res, res_cnt, max_res);
    q.wait();

    // Brute force verify against the reordered leaf nodes
    int expected_cnt = 0;
    for (int i = 0; i < n; ++i) {
        float dx = tree_parts.pos_x[i] - qx;
        float dy = tree_parts.pos_y[i] - qy;
        float dz = tree_parts.pos_z[i] - qz;
        float dist = std::sqrt(dx*dx + dy*dy + dz*dz);
        if (dist >= r_min && dist <= r_max) {
            expected_cnt++;
        }
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
    float *knn_dists = sycl::malloc_shared<float>(k, q);
    knn_query(q, tree, dqx, dqy, dqz, k, 1, knn_res, knn_dists);
    q.wait();

    std::cout << "Testing kNN query for k=" << k << " around (" << qx << ", " << qy << ", " << qz << "):" << std::endl;
    for (int i = 0; i < k; ++i) {
        std::cout << "  Neighbor " << i << ": index " << knn_res[i] << ", dist " << knn_dists[i] << std::endl;
    }

    // Cleanup
    tree.free(q);
    sycl::free(dqx, q); sycl::free(dqy, q); sycl::free(dqz, q);
    sycl::free(drm, q); sycl::free(dRM, q);
    sycl::free(res, q); sycl::free(res_cnt, q);
    sycl::free(knn_res, q); sycl::free(knn_dists, q);

    return success ? 0 : 1;
}
