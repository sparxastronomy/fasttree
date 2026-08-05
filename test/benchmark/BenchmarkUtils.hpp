#ifndef FASTTREE_BENCHMARK_UTILS_HPP
#define FASTTREE_BENCHMARK_UTILS_HPP

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <hdf5.h>
#include <hdf5_hl.h>
#include <unistd.h>

namespace fasttree {
namespace bench_utils {

struct Timer {
    std::chrono::high_resolution_clock::time_point start_time;
    std::string                                    name;

    Timer(const std::string &n) : name(n) {
        start_time = std::chrono::high_resolution_clock::now();
    }

    double stop() {
        auto                          end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed  = end_time - start_time;
        return elapsed.count();
    }
};

// Simple RSS memory tracking for Linux
size_t get_peak_rss() {
    std::ifstream file("/proc/self/status");
    std::string   line;
    while (std::getline(file, line)) {
        if (line.substr(0, 6) == "VmRSS:") {
            std::stringstream ss(line.substr(6));
            size_t            rss;
            ss >> rss;
            return rss; // in KB
        }
    }
    return 0;
}

struct ParticleData {
    std::vector<float> pos_x, pos_y, pos_z;
    std::vector<float> vel_x, vel_y, vel_z;
    std::vector<float> masses;
    size_t             count;
};

bool load_hdf5_data(const std::string &filename, ParticleData &data) {
    if (filename.empty()) {
        std::cerr << "Error: Empty filename provided to load_hdf5_data" << std::endl;
        return false;
    }
    hid_t file_id = H5Fopen(filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    if (file_id < 0) {
        std::cerr << "Error: Could not open HDF5 file '" << filename << "'" << std::endl;
        return false;
    }

    hid_t group_id = H5Gopen(file_id, "/PartType0", H5P_DEFAULT);
    if (group_id < 0) {
        std::cerr << "Error: Could not open group /PartType0 in " << filename << std::endl;
        H5Fclose(file_id);
        return false;
    }

    // Get number of particles from Coordinates dataset
    hid_t dset_id = H5Dopen(group_id, "Coordinates", H5P_DEFAULT);
    if (dset_id < 0) {
        std::cerr << "Error: Could not open dataset Coordinates" << std::endl;
        H5Gclose(group_id);
        H5Fclose(file_id);
        return false;
    }

    hid_t   space_id = H5Dget_space(dset_id);
    hsize_t dims[2];
    H5Sget_simple_extent_dims(space_id, dims, NULL);
    data.count = dims[0];
    H5Sclose(space_id);
    H5Dclose(dset_id);

    data.pos_x.resize(data.count);
    data.pos_y.resize(data.count);
    data.pos_z.resize(data.count);
    data.vel_x.resize(data.count);
    data.vel_y.resize(data.count);
    data.vel_z.resize(data.count);
    data.masses.resize(data.count);

    // Load Coordinates (Nx3)
    std::vector<float> coords(data.count * 3);
    if (H5LTread_dataset_float(group_id, "Coordinates", coords.data()) < 0) {
        std::cerr << "Error: Could not read Coordinates" << std::endl;
        return false;
    }
    for (size_t i = 0; i < data.count; ++i) {
        data.pos_x[i] = coords[i * 3 + 0];
        data.pos_y[i] = coords[i * 3 + 1];
        data.pos_z[i] = coords[i * 3 + 2];
    }

    // Load Velocities (Nx3)
    std::vector<float> vels(data.count * 3);
    if (H5LTread_dataset_float(group_id, "Velocities", vels.data()) < 0) {
        std::cerr << "Error: Could not read Velocities" << std::endl;
    } else {
        for (size_t i = 0; i < data.count; ++i) {
            data.vel_x[i] = vels[i * 3 + 0];
            data.vel_y[i] = vels[i * 3 + 1];
            data.vel_z[i] = vels[i * 3 + 2];
        }
    }

    // Load Masses (N)
    if (H5LTread_dataset_float(group_id, "Masses", data.masses.data()) < 0) {
        std::cerr << "Error: Could not read Masses" << std::endl;
    }

    H5Gclose(group_id);
    H5Fclose(file_id);
    return true;
}

void print_benchmark_row(
    const std::string &test_name,
    const std::string &hardware,
    size_t             count,
    const std::string &params,
    double             time_sec,
    size_t             memory_kb,
    const std::string &notes = "-"
) {
    std::cout << "| " << std::left << std::setw(25) << test_name << " | " << std::setw(20)
              << hardware << " | " << std::setw(15) << count << " | " << std::setw(25) << params
              << " | " << std::fixed << std::setprecision(6) << std::setw(12) << time_sec << " | "
              << std::fixed << std::setprecision(2) << std::setw(10) << (memory_kb / 1024.0)
              << " | " << std::setw(20) << notes << " |" << std::endl;
}

} // namespace bench_utils
} // namespace fasttree

#endif // FASTTREE_BENCHMARK_UTILS_HPP
