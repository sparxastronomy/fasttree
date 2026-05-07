#include "../src/hlbvh.hpp"
#include <iostream>
#include <vector>
#include <cmath>

using namespace fasttree;

int main() {
    sycl::queue q;
    std::cout << "Running on: " << q.get_device().get_info<sycl::info::device::name>() << std::endl;

    const int n = 1000;
    particles<float> p;
    p.pos_x.resize(n);
    p.pos_y.resize(n);
    p.pos_z.resize(n);

    for (int i = 0; i < n; ++i) {
        p.pos_x[i] = static_cast<float>(i);
        p.pos_y[i] = 0.0f;
        p.pos_z[i] = 0.0f;
    }

    BoundingBox bbox = {0.0f, static_cast<float>(n), -1.0f, 1.0f, -1.0f, 1.0f};

    std::vector<uint64_t> morton_keys(n);
    std::cout << "Step 1: Morton Encoding..." << std::endl;
    morton_encode(q, p, morton_keys, bbox);
    q.wait();
    std::cout << "Step 1: Done." << std::endl;

    std::cout << "Step 2: Sorting Morton Keys..." << std::endl;
    auto sorted_indices = sort_morton_keys(morton_keys);
    std::cout << "Step 2: Done." << std::endl;

    // Prepare memory for building the tree
    std::vector<float> sx(n), sy(n), sz(n);
    std::vector<uint64_t> smk(n);

    for (int i = 0; i < n; ++i) {
        sx[i] = p.pos_x[sorted_indices[i]];
        sy[i] = p.pos_y[sorted_indices[i]];
        sz[i] = p.pos_z[sorted_indices[i]];
        smk[i] = morton_keys[sorted_indices[i]];
    }

    std::cout << "Step 3: Building Tree..." << std::endl;
    TreeSoA tree(q, n);
    build_tree(q, tree, smk.data(), sx.data(), sy.data(), sz.data());
    std::cout << "Step 3: Done." << std::endl;

    // Range query
    float qx = 500.5f, qy = 0.0f, qz = 0.0f;
    float r_min = 0.0f, r_max = 10.0f;
    int max_res = 100;

    std::vector<float> dqx = {qx};
    std::vector<float> dqy = {qy};
    std::vector<float> dqz = {qz};
    std::vector<float> drm = {r_min};
    std::vector<float> dRM = {r_max};
    std::vector<int> res(max_res);
    std::vector<int> res_cnt(1);

    std::cout << "Step 4: Range Query..." << std::endl;
    range_query(q, tree, dqx.data(), dqy.data(), dqz.data(), drm.data(), dRM.data(), 1, res.data(), res_cnt.data(), max_res);
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
    std::vector<int> knn_res(k);
    std::vector<float> knn_dists(k);
    knn_query(q, tree, dqx.data(), dqy.data(), dqz.data(), k, 1, knn_res.data(), knn_dists.data());
    q.wait();

    std::cout << "kNN results for k=" << k << " around " << qx << ":" << std::endl;
    for (int i = 0; i < k; ++i) {
        std::cout << "  Neighbor " << i << ": index " << knn_res[i] << ", dist " << knn_dists[i] << std::endl;
    }

    tree.free(q);
    return success ? 0 : 1;
}
