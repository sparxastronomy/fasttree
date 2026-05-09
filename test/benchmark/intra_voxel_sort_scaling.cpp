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
    std::cout << "Benchmarking Intra-Voxel Sort Scaling on: " << device_name << std::endl;

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

        size_t n = data.count;
        uint64_t *morton_keys = sycl::malloc_shared<uint64_t>(n, q);

        // Fill with dummy morton keys for sorting benchmark
        q.fill(morton_keys, 0ULL, n).wait();

        Timer timer("intra_voxel_sort");
        intra_voxel_sort(q, morton_keys, n);
        q.wait();
        double elapsed = timer.stop();

        size_t memory = get_peak_rss();
        print_benchmark_row("intra_voxel_sort_scaling", device_name, n, "-", elapsed, memory);

        sycl::free(morton_keys, q);
    }

    return 0;
}
