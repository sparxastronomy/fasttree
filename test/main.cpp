#include "../src/hlbvh.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <sycl/sycl.hpp>

using namespace fasttree;

int main() {
    sycl::queue q;
    std::cout << "Running on: " << q.get_device().get_info<sycl::info::device::name>() << std::endl;

    const int n = 1000;
    
    // Use USM for particles
    float *pos_x = sycl::malloc_shared<float>(n, q);
    float *pos_y = sycl::malloc_shared<float>(n, q);
    float *pos_z = sycl::malloc_shared<float>(n, q);

    for (int i = 0; i < n; ++i) {
        pos_x[i] = static_cast<float>(i);
        pos_y[i] = 0.0f;
        pos_z[i] = 0.0f;
    }

    BoundingBox bbox = {0.0f, static_cast<float>(n), -1.0f, 1.0f, -1.0f, 1.0f};

    uint64_t *morton_keys = sycl::malloc_shared<uint64_t>(n, q);
    
    std::cout << "Step 1: Morton Encoding..." << std::endl;
    // Manual morton_encode call to handle raw pointers
    float dx = bbox.max_x - bbox.min_x;
    float dy = bbox.max_y - bbox.min_y;
    float dz = bbox.max_z - bbox.min_z;

    q.parallel_for(sycl::range<1>(n), [=](sycl::id<1> idx) {
        size_t i = idx[0];
        auto normalize = [](float v, float min, float delta) {
            if (delta == 0) return 0ULL;
            float t = (v - min) / delta;
            t = std::max(0.0f, std::min(1.0f, t));
            return static_cast<std::uint64_t>(t * ((1ULL << BITS_PER_DIM) - 1)) | 0ULL;
        };
        uint64_t ix = normalize(pos_x[i], bbox.min_x, dx);
        uint64_t iy = normalize(pos_y[i], bbox.min_y, dy);
        uint64_t iz = normalize(pos_z[i], bbox.min_z, dz);
        morton_keys[i] = spread3_u64(ix) | (spread3_u64(iy) << 1) | (spread3_u64(iz) << 2);
    }).wait();
    std::cout << "Step 1: Done." << std::endl;

    std::cout << "Step 2: Sorting Morton Keys..." << std::endl;
    std::vector<size_t> indices(n);
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(), [&](size_t i1, size_t i2) {
        return morton_keys[i1] < morton_keys[i2];
    });
    std::cout << "Step 2: Done." << std::endl;

    // Prepare memory for building the tree
    float *sx = sycl::malloc_shared<float>(n, q);
    float *sy = sycl::malloc_shared<float>(n, q);
    float *sz = sycl::malloc_shared<float>(n, q);
    uint64_t *smk = sycl::malloc_shared<uint64_t>(n, q);

    for (int i = 0; i < n; ++i) {
        sx[i] = pos_x[indices[i]];
        sy[i] = pos_y[indices[i]];
        sz[i] = pos_z[indices[i]];
        smk[i] = morton_keys[indices[i]];
    }

    std::cout << "Step 3: Building Tree..." << std::endl;
    TreeSoA tree(q, n);
    build_tree(q, tree, smk, sx, sy, sz);
    q.wait();
    std::cout << "Step 3: Done." << std::endl;

    // Range query
    float qx = 500.5f, qy = 0.0f, qz = 0.0f;
    float r_min = 0.0f, r_max = 10.0f;
    int max_res = 100;

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

    std::cout << "Step 4: Range Query..." << std::endl;
    range_query(q, tree, dqx, dqy, dqz, drm, dRM, 1, res, res_cnt, max_res);
    q.wait();
    std::cout << "Step 4: Done." << std::endl;

    std::cout << "Found " << res_cnt[0] << " particles within radius " << r_max << " of " << qx << std::endl;
    bool success = true;
    
    // Brute force verify
    int expected_cnt = 0;
    for (int i = 0; i < n; ++i) {
        float dx = sx[i] - qx;
        float dy = sy[i] - qy;
        float dz = sz[i] - qz;
        float dist = std::sqrt(dx*dx + dy*dy + dz*dz);
        if (dist >= r_min && dist <= r_max) {
            expected_cnt++;
        }
    }
    std::cout << "Expected (brute force): " << expected_cnt << std::endl;

    if (res_cnt[0] != expected_cnt) {
        std::cout << "FAILURE: Result count mismatch!" << std::endl;
        success = false;
    } else {
        std::cout << "SUCCESS: Result count matches." << std::endl;
    }

    // kNN query test
    int k = 5;
    int *knn_res = sycl::malloc_shared<int>(k, q);
    float *knn_dists = sycl::malloc_shared<float>(k, q);
    knn_query(q, tree, dqx, dqy, dqz, k, 1, knn_res, knn_dists);
    q.wait();

    std::cout << "kNN results for k=" << k << " around " << qx << ":" << std::endl;
    for (int i = 0; i < k; ++i) {
        std::cout << "  Neighbor " << i << ": index " << knn_res[i] << ", dist " << knn_dists[i] << std::endl;
    }

    // Cleanup
    tree.free(q);
    sycl::free(pos_x, q);
    sycl::free(pos_y, q);
    sycl::free(pos_z, q);
    sycl::free(morton_keys, q);
    sycl::free(sx, q);
    sycl::free(sy, q);
    sycl::free(sz, q);
    sycl::free(smk, q);
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

