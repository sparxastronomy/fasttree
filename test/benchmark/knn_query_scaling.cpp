#include "hlbvh.hpp"
#include "BenchmarkUtils.hpp"
#include <sycl/sycl.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <random>

using namespace fasttree;
using namespace fasttree::benchmark;

int main() {
    sycl::queue q;
    std::string device_name = q.get_device().get_info<sycl::info::device::name>();
    std::cout << "Benchmarking kNN Query Scaling on: " << device_name << std::endl;

    std::ifstream config("config.txt");
    if (!config.is_open()) {
        std::cerr << "Error: Could not open config.txt" << std::endl;
        return 1;
    }

    std::vector<int> k_values = {1, 2, 4, 8, 16, 32, 64, 128};
    const int num_queries = 1000;

    std::string line;
    while (std::getline(config, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::stringstream ss(line);
        std::string count_str, file_path;
        ss >> count_str >> file_path;

        ParticleData data;
        if (!load_hdf5_data(file_path, data)) {
            continue;
        }

        particles<float> p;
        p.pos_x = data.pos_x;
        p.pos_y = data.pos_y;
        p.pos_z = data.pos_z;

        size_t n = p.pos_x.size();
        TreeSoA tree(q, n);
        build_bvh(q, p, tree);
        q.wait();

        // Bounding box for random query points
        float min_x = p.pos_x[0], max_x = p.pos_x[0];
        float min_y = p.pos_y[0], max_y = p.pos_y[0];
        float min_z = p.pos_z[0], max_z = p.pos_z[0];
        for(size_t i=0; i<n; ++i) {
            min_x = std::min(min_x, p.pos_x[i]); max_x = std::max(max_x, p.pos_x[i]);
            min_y = std::min(min_y, p.pos_y[i]); max_y = std::max(max_y, p.pos_y[i]);
            min_z = std::min(min_z, p.pos_z[i]); max_z = std::max(max_z, p.pos_z[i]);
        }
        
        std::mt19937 gen(42);
        std::uniform_real_distribution<float> dis_x(min_x, max_x);
        std::uniform_real_distribution<float> dis_y(min_y, max_y);
        std::uniform_real_distribution<float> dis_z(min_z, max_z);

        float *qx = sycl::malloc_shared<float>(num_queries, q);
        float *qy = sycl::malloc_shared<float>(num_queries, q);
        float *qz = sycl::malloc_shared<float>(num_queries, q);

        for (int i = 0; i < num_queries; ++i) {
            qx[i] = dis_x(gen);
            qy[i] = dis_y(gen);
            qz[i] = dis_z(gen);
        }

        for (int k : k_values) {
            int *results = sycl::malloc_shared<int>(num_queries * k, q);
            float *result_dists = sycl::malloc_shared<float>(num_queries * k, q);

            Timer timer("knn_query");
            knn_query(q, tree, qx, qy, qz, k, num_queries, results, result_dists);
            q.wait();
            double elapsed = timer.stop();

            size_t memory = get_peak_rss();
            std::string params = "k=" + std::to_string(k);
            print_benchmark_row("knn_query_scaling", device_name, n, params, elapsed, memory);

            sycl::free(results, q);
            sycl::free(result_dists, q);
        }

        sycl::free(qx, q); sycl::free(qy, q); sycl::free(qz, q);
        tree.free(q);
    }

    return 0;
}
