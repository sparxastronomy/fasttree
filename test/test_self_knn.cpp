#include "hlbvh.hpp"
#include <sycl/sycl.hpp>
#include <cassert>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

using namespace fasttree;

int main() {
    sycl::queue q;
    std::cout << "Running Self-kNN Query Unit Test on device: " 
              << q.get_device().get_info<sycl::info::device::name>() << std::endl;

    const size_t n = 1000;
    const int k = 8;

    particles<coord_t> p;
    p.pos_x.resize(n);
    p.pos_y.resize(n);
    p.pos_z.resize(n);

    std::mt19937 gen(42);
    std::uniform_real_distribution<double> dis(0.1, 0.9);

    for (size_t i = 0; i < n; ++i) {
#if defined(FASTTREE_INTEGER_COORDS)
        p.pos_x[i] = static_cast<coord_t>(dis(gen) * 1000000.0);
        p.pos_y[i] = static_cast<coord_t>(dis(gen) * 1000000.0);
        p.pos_z[i] = static_cast<coord_t>(dis(gen) * 1000000.0);
#else
        p.pos_x[i] = dis(gen);
        p.pos_y[i] = dis(gen);
        p.pos_z[i] = dis(gen);
#endif
    }

    TreeSoA tree(q, n);
    build_bvh(q, p, tree);
    q.wait();

    std::vector<size_t> results(n * k);
    std::vector<dist_t> result_dists(n * k);

    size_t *d_results = sycl::malloc_device<size_t>(n * k, q);
    dist_t *d_dists   = sycl::malloc_device<dist_t>(n * k, q);

    knn_query(q, tree, p.pos_x.data(), p.pos_y.data(), p.pos_z.data(),
              k, n, d_results, d_dists);
    q.wait();

    q.memcpy(results.data(), d_results, n * k * sizeof(size_t)).wait();
    q.memcpy(result_dists.data(), d_dists, n * k * sizeof(dist_t)).wait();

    // Verify self-query accuracy:
    // 1st neighbor for particle i should be particle i itself with distance 0.
    size_t self_match_count = 0;
    for (size_t i = 0; i < n; ++i) {
        size_t first_neighbor = results[i * k];
        dist_t first_dist     = result_dists[i * k];

        if (first_neighbor == i && first_dist == 0) {
            self_match_count++;
        }
    }

    std::cout << "Self-kNN match accuracy: " << self_match_count << "/" << n 
              << " (" << (100.0 * self_match_count / n) << "%)" << std::endl;

    assert(self_match_count == n && "Self-kNN query failed to identify self as 1st neighbor!");

    sycl::free(d_results, q);
    sycl::free(d_dists, q);
    tree.free(q);

    std::cout << "Self-kNN Query Unit Test PASSED SUCCESSFULLY!" << std::endl;
    return 0;
}
