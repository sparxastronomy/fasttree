#include "hlbvh.hpp"
#include "BenchmarkUtils.hpp"
#include <sycl/sycl.hpp>
#include <iostream>
#include <vector>
#include <string>

using namespace fasttree;
using namespace fasttree::benchmark;

int main() {
    sycl::queue q;
    std::string device_name = q.get_device().get_info<sycl::info::device::name>();
    std::cout << "Benchmarking Radix Sort Scaling on: " << device_name << std::endl;

    std::ifstream config("config.txt");
    if (!config.is_open()) {
        std::cerr << "Error: Could not open config.txt" << std::endl;
        return 1;
    }

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
        std::vector<uint64_t> morton_keys(n);

        BoundingBox bbox = {p.pos_x[0], p.pos_x[0], p.pos_y[0], p.pos_y[0], p.pos_z[0], p.pos_z[0]};
        for (size_t i = 1; i < n; ++i) {
            bbox.min_x = std::min(bbox.min_x, p.pos_x[i]);
            bbox.max_x = std::max(bbox.max_x, p.pos_x[i]);
            bbox.min_y = std::min(bbox.min_y, p.pos_y[i]);
            bbox.max_y = std::max(bbox.max_y, p.pos_y[i]);
            bbox.min_z = std::min(bbox.min_z, p.pos_z[i]);
            bbox.max_z = std::max(bbox.max_z, p.pos_z[i]);
        }

        morton_encode(q, p, morton_keys, bbox);
        q.wait();

        Timer timer("sort_morton_keys");
        auto indices = sort_morton_keys(morton_keys);
        double elapsed = timer.stop();

        size_t memory = get_peak_rss();
        print_benchmark_row("radix_sort_scaling", device_name, n, "-", elapsed, memory);
    }

    return 0;
}
