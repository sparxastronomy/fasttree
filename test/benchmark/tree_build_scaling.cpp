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
    std::cout << "Benchmarking Tree Build Scaling on: " << device_name << std::endl;

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
        TreeSoA tree(q, n);

        Timer timer("build_bvh");
        build_bvh(q, p, tree);
        q.wait();
        double elapsed = timer.stop();

        size_t memory = get_peak_rss();
        print_benchmark_row("tree_build_scaling", device_name, n, "-", elapsed, memory);

        tree.free(q);
    }

    return 0;
}
