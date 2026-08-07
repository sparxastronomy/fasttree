#ifndef FASTTREE_BENCHMARK_UTILS_HPP
#define FASTTREE_BENCHMARK_UTILS_HPP

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <hdf5.h>
#include <hdf5_hl.h>
#include <unistd.h>

#include "hlbvh.hpp"

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
inline size_t get_peak_rss() {
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
    size_t             count = 0;
};

inline bool load_hdf5_data(const std::string &filename, ParticleData &data) {
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

template <typename T>
inline void fill_particle_coords(const ParticleData &data, particles<T> &p, double &out_box_min, double &out_box_size) {
    size_t n = data.count;
    p.pos_x.resize(n);
    p.pos_y.resize(n);
    p.pos_z.resize(n);

#if defined(FASTTREE_INTEGER_COORDS)
    if (n == 0) {
        out_box_min = 0.0;
        out_box_size = 1.0;
        return;
    }
    double min_x = data.pos_x[0], max_x = data.pos_x[0];
    double min_y = data.pos_y[0], max_y = data.pos_y[0];
    double min_z = data.pos_z[0], max_z = data.pos_z[0];
    for (size_t i = 1; i < n; ++i) {
        min_x = std::min(min_x, static_cast<double>(data.pos_x[i]));
        max_x = std::max(max_x, static_cast<double>(data.pos_x[i]));
        min_y = std::min(min_y, static_cast<double>(data.pos_y[i]));
        max_y = std::max(max_y, static_cast<double>(data.pos_y[i]));
        min_z = std::min(min_z, static_cast<double>(data.pos_z[i]));
        max_z = std::max(max_z, static_cast<double>(data.pos_z[i]));
    }
    out_box_min = std::min({min_x, min_y, min_z});
    double max_val = std::max({max_x, max_y, max_z});
    out_box_size = (max_val > out_box_min) ? (max_val - out_box_min) * 1.0001 : 1.0;
    double inv_dx = 1.0 / out_box_size;

    for (size_t i = 0; i < n; ++i) {
        p.pos_x[i] = float_to_int_rep<double>(data.pos_x[i], out_box_min, inv_dx);
        p.pos_y[i] = float_to_int_rep<double>(data.pos_y[i], out_box_min, inv_dx);
        p.pos_z[i] = float_to_int_rep<double>(data.pos_z[i], out_box_min, inv_dx);
    }
#else
    out_box_min = 0.0;
    out_box_size = 1.0;
    for (size_t i = 0; i < n; ++i) {
        p.pos_x[i] = static_cast<T>(data.pos_x[i]);
        p.pos_y[i] = static_cast<T>(data.pos_y[i]);
        p.pos_z[i] = static_cast<T>(data.pos_z[i]);
    }
#endif
}

inline coord_t scale_distance_to_coord(double r, double box_size) {
#if defined(FASTTREE_INTEGER_COORDS)
    double norm = r / box_size;
    double int_val = norm * std::ldexp(1.0, BITS_PER_DIMENSION);
    return static_cast<coord_t>(int_val);
#else
    return static_cast<coord_t>(r);
#endif
}

inline void print_benchmark_row(
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
