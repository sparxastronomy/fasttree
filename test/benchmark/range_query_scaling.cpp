#include "hlbvh.hpp"
#include "BenchmarkUtils.hpp"
#include <sycl/sycl.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <numeric>

using namespace fasttree;
using namespace fasttree::benchmark;

int main() {
    sycl::queue q;
    std::string device_name = q.get_device().get_info<sycl::info::device::name>();
    std::cout << "Benchmarking Range Query Scaling on: " << device_name << std::endl;

    std::ifstream config("config.txt");
    if (!config.is_open()) {
        std::cerr << "Error: Could not open config.txt" << std::endl;
        return 1;
    }

    std::vector<float> radii = {0.01f, 0.1f, 1.0f, 10.0f, 100.0f, 200.0f};
    const int num_queries = 1000;
    const int max_results_per_query = 1000; // Increased to capture more for large radii

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
        float *r_min = sycl::malloc_shared<float>(num_queries, q);
        float *r_max = sycl::malloc_shared<float>(num_queries, q);
        int *results = sycl::malloc_shared<int>(num_queries * max_results_per_query, q);
        int *result_counts = sycl::malloc_shared<int>(num_queries, q);

        for (int i = 0; i < num_queries; ++i) {
            qx[i] = dis_x(gen);
            qy[i] = dis_y(gen);
            qz[i] = dis_z(gen);
            r_min[i] = 0.0f;
        }

        for (float radius : radii) {
            for (int i = 0; i < num_queries; ++i) r_max[i] = radius;
            q.fill(result_counts, 0, num_queries).wait();

            Timer timer("range_query");
            range_query(q, tree, qx, qy, qz, r_min, r_max, num_queries, results, result_counts, max_results_per_query);
            q.wait();
            double elapsed = timer.stop();

            long long total_found = 0;
            for(int i=0; i<num_queries; ++i) total_found += result_counts[i];

            size_t memory = get_peak_rss();
            std::string params = "R=" + std::to_string(radius);
            std::string notes = "Found " + std::to_string(total_found);
            print_benchmark_row("range_query_scaling", device_name, n, params, elapsed, memory, notes);
        }

        sycl::free(qx, q); sycl::free(qy, q); sycl::free(qz, q);
        sycl::free(r_min, q); sycl::free(r_max, q);
        sycl::free(results, q); sycl::free(result_counts, q);
        tree.free(q);
    }

    return 0;
}
