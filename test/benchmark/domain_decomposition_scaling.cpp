#include "BenchmarkUtils.hpp"
#include "domain_decomposition.hpp"
#include <sycl/sycl.hpp>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <hdf5.h>
#include <hdf5_hl.h>
#include <mpi.h>
#include <unistd.h>

using namespace fasttree;
using namespace fasttree::bench_utils;

// Helper to get total number of particles from HDF5 file
size_t get_hdf5_dataset_size(const std::string &filename) {
    hid_t file_id = H5Fopen(filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    if (file_id < 0) return 0;
    hid_t group_id = H5Gopen(file_id, "/PartType0", H5P_DEFAULT);
    if (group_id < 0) {
        H5Fclose(file_id);
        return 0;
    }
    hid_t dset_id = H5Dopen(group_id, "Coordinates", H5P_DEFAULT);
    if (dset_id < 0) {
        H5Gclose(group_id);
        H5Fclose(file_id);
        return 0;
    }
    hid_t   space_id = H5Dget_space(dset_id);
    hsize_t dims[2];
    H5Sget_simple_extent_dims(space_id, dims, NULL);
    size_t count = dims[0];
    H5Sclose(space_id);
    H5Dclose(dset_id);
    H5Gclose(group_id);
    H5Fclose(file_id);
    return count;
}

// Helper to load a slice of HDF5 data
bool load_hdf5_data_slice(
    const std::string &filename, size_t offset, size_t count, ParticleData &data
) {
    if (filename.empty()) return false;
    hid_t file_id = H5Fopen(filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    if (file_id < 0) return false;

    hid_t group_id = H5Gopen(file_id, "/PartType0", H5P_DEFAULT);
    if (group_id < 0) {
        H5Fclose(file_id);
        return false;
    }

    data.count = count;
    data.pos_x.resize(count, 0.0f);
    data.pos_y.resize(count, 0.0f);
    data.pos_z.resize(count, 0.0f);
    data.vel_x.resize(count, 0.0f);
    data.vel_y.resize(count, 0.0f);
    data.vel_z.resize(count, 0.0f);

    if (count == 0) {
        H5Gclose(group_id);
        H5Fclose(file_id);
        return true;
    }

    // Read slice of Coordinates (count x 3)
    hid_t dset_coords = H5Dopen(group_id, "Coordinates", H5P_DEFAULT);
    if (dset_coords >= 0) {
        hid_t   filespace    = H5Dget_space(dset_coords);
        hsize_t offset_3d[2] = {offset, 0};
        hsize_t count_3d[2]  = {count, 3};
        H5Sselect_hyperslab(filespace, H5S_SELECT_SET, offset_3d, NULL, count_3d, NULL);

        hid_t              memspace = H5Screate_simple(2, count_3d, NULL);
        std::vector<float> coords(count * 3);
        if (H5Dread(
                dset_coords, H5T_NATIVE_FLOAT, memspace, filespace, H5P_DEFAULT, coords.data()
            ) >= 0) {
            for (size_t i = 0; i < count; ++i) {
                data.pos_x[i] = coords[i * 3 + 0];
                data.pos_y[i] = coords[i * 3 + 1];
                data.pos_z[i] = coords[i * 3 + 2];
            }
        }
        H5Sclose(memspace);
        H5Sclose(filespace);
        H5Dclose(dset_coords);
    }

    // Read slice of Velocities (count x 3)
    hid_t dset_vels = H5Dopen(group_id, "Velocities", H5P_DEFAULT);
    if (dset_vels >= 0) {
        hid_t   filespace    = H5Dget_space(dset_vels);
        hsize_t offset_3d[2] = {offset, 0};
        hsize_t count_3d[2]  = {count, 3};
        H5Sselect_hyperslab(filespace, H5S_SELECT_SET, offset_3d, NULL, count_3d, NULL);

        hid_t              memspace = H5Screate_simple(2, count_3d, NULL);
        std::vector<float> vels(count * 3);
        if (H5Dread(dset_vels, H5T_NATIVE_FLOAT, memspace, filespace, H5P_DEFAULT, vels.data()) >=
            0) {
            for (size_t i = 0; i < count; ++i) {
                data.vel_x[i] = vels[i * 3 + 0];
                data.vel_y[i] = vels[i * 3 + 1];
                data.vel_z[i] = vels[i * 3 + 2];
            }
        }
        H5Sclose(memspace);
        H5Sclose(filespace);
        H5Dclose(dset_vels);
    }

    H5Gclose(group_id);
    H5Fclose(file_id);
    return true;
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    sycl::queue q;

    std::string config_path = "config.txt";
    if (argc > 1) { config_path = argv[1]; }

    std::ifstream config(config_path);
    if (!config.is_open()) {
        if (rank == 0)
            std::cerr << "Error: Could not open config file " << config_path << std::endl;
        MPI_Finalize();
        return 1;
    }

    if (rank == 0) {
        std::cout << "============================================================================="
                     "===========\n";
        std::cout << "                 DISTRIBUTED DOMAIN DECOMPOSITION SCALING BENCHMARK\n";
        std::cout << "============================================================================="
                     "===========\n";
        std::cout << "MPI Ranks: " << size << "\n";
        std::cout << "Device:    " << q.get_device().get_info<sycl::info::device::name>() << "\n\n";
    }

    std::string line;
    while (std::getline(config, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::stringstream ss(line);
        std::string       label, file_path;
        ss >> label >> file_path;

        std::vector<std::string> file_paths;
        if (rank == 0) {
            // Discover all split files if present
            std::string base_path = file_path;
            if (base_path.size() >= 5 && base_path.substr(base_path.size() - 5) == ".hdf5") {
                base_path = base_path.substr(0, base_path.size() - 5);
            }

            int file_idx = 0;
            while (true) {
                char suffix[64];
                sprintf(suffix, ".%04d.hdf5", file_idx);
                std::string test_path = base_path + suffix;
                if (access(test_path.c_str(), F_OK) == 0) {
                    file_paths.push_back(test_path);
                    file_idx++;
                } else {
                    break;
                }
            }

            // If no split files are found, use the single file path
            if (file_paths.empty()) { file_paths.push_back(file_path); }
        }

        // Broadcast file paths from Rank 0 to all ranks
        size_t num_files = 0;
        if (rank == 0) { num_files = file_paths.size(); }
        MPI_Bcast(&num_files, 1, MPI_UINT64_T, 0, MPI_COMM_WORLD);

        if (rank != 0) { file_paths.resize(num_files); }

        for (size_t i = 0; i < num_files; ++i) {
            int len = 0;
            if (rank == 0) { len = file_paths[i].size(); }
            MPI_Bcast(&len, 1, MPI_INT, 0, MPI_COMM_WORLD);
            if (rank != 0) { file_paths[i].resize(len); }
            MPI_Bcast(file_paths[i].data(), len, MPI_CHAR, 0, MPI_COMM_WORLD);
        }

        // Get file sizes and total particle count on Rank 0 and broadcast
        std::vector<size_t> file_sizes(num_files, 0);
        size_t              total_n = 0;
        if (rank == 0) {
            for (size_t i = 0; i < num_files; ++i) {
                file_sizes[i] = get_hdf5_dataset_size(file_paths[i]);
                total_n += file_sizes[i];
            }
        }

        MPI_Bcast(&total_n, 1, MPI_UINT64_T, 0, MPI_COMM_WORLD);
        if (num_files > 0) {
            file_sizes.resize(num_files);
            MPI_Bcast(file_sizes.data(), num_files, MPI_UINT64_T, 0, MPI_COMM_WORLD);
        }

        if (total_n == 0) continue;

        // Calculate load-balanced particle range for this rank
        size_t chunk    = total_n / size;
        size_t my_start = rank * chunk;
        size_t my_end   = (rank == size - 1) ? total_n : (rank + 1) * chunk;
        size_t my_count = my_end - my_start;

        // Load slices from intersecting files
        ParticleData hdf5_data;
        hdf5_data.count   = 0;
        size_t file_start = 0;
        for (size_t i = 0; i < num_files; ++i) {
            size_t file_end        = file_start + file_sizes[i];
            size_t intersect_start = std::max(my_start, file_start);
            size_t intersect_end   = std::min(my_end, file_end);

            if (intersect_start < intersect_end) {
                size_t read_offset = intersect_start - file_start;
                size_t read_count  = intersect_end - intersect_start;

                ParticleData temp_data;
                if (load_hdf5_data_slice(file_paths[i], read_offset, read_count, temp_data)) {
                    hdf5_data.pos_x.insert(
                        hdf5_data.pos_x.end(), temp_data.pos_x.begin(), temp_data.pos_x.end()
                    );
                    hdf5_data.pos_y.insert(
                        hdf5_data.pos_y.end(), temp_data.pos_y.begin(), temp_data.pos_y.end()
                    );
                    hdf5_data.pos_z.insert(
                        hdf5_data.pos_z.end(), temp_data.pos_z.begin(), temp_data.pos_z.end()
                    );
                    hdf5_data.vel_x.insert(
                        hdf5_data.vel_x.end(), temp_data.vel_x.begin(), temp_data.vel_x.end()
                    );
                    hdf5_data.vel_y.insert(
                        hdf5_data.vel_y.end(), temp_data.vel_y.begin(), temp_data.vel_y.end()
                    );
                    hdf5_data.vel_z.insert(
                        hdf5_data.vel_z.end(), temp_data.vel_z.begin(), temp_data.vel_z.end()
                    );
                    hdf5_data.count += read_count;
                }
            }
            file_start = file_end;
        }

        particles<coord_t> p_orig;
        p_orig.pos_x.assign(hdf5_data.pos_x.begin(), hdf5_data.pos_x.end());
        p_orig.pos_y.assign(hdf5_data.pos_y.begin(), hdf5_data.pos_y.end());
        p_orig.pos_z.assign(hdf5_data.pos_z.begin(), hdf5_data.pos_z.end());
        p_orig.id.resize(my_count);
        p_orig.is_ghost.resize(my_count, 0);
        for (size_t i = 0; i < my_count; ++i) {
            p_orig.id[i] = static_cast<uint32_t>(my_start + i);
        }

        // Run warm up
        constexpr int TOP_BITS = 24;
        {
            particles<coord_t>   p    = p_orig;
            BoundingBox<coord_t> bbox = get_global_bounding_box(q, p);
#if defined(DCOMPOSITION_TYPE_SAMPLING)
            std::vector<uint64_t> splitters       = get_deterministic_splitters(q, p, bbox);
            particles<coord_t>    redistributed_p = redistribute_particles(q, p, splitters, bbox);
#else
            std::vector<int>      hist      = get_global_histogram(q, p, bbox, TOP_BITS);
            std::vector<uint32_t> splitters = generate_splitters(hist, size, TOP_BITS);
            particles<coord_t>    redistributed_p =
                redistribute_particles(q, p, splitters, bbox, TOP_BITS);
#endif
            coord_t            h_max     = static_cast<coord_t>(2.0);
            particles<coord_t> ghosted_p = exchange_halos(q, redistributed_p, h_max);
            if (ghosted_p.pos_x.size() > 0) {
                TreeSoA tree(q, ghosted_p.pos_x.size());
                build_bvh(q, ghosted_p, tree);
                q.wait();
                tree.free(q);
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);

        // Measured runs
        int    num_iterations = 3;
        double times_bbox[3] = {0.0}, times_hist[3] = {0.0}, times_splitters[3] = {0.0};
        double times_redist[3] = {0.0}, times_halo[3] = {0.0}, times_tree[3] = {0.0},
               times_total[3] = {0.0};

        for (int iter = 0; iter < num_iterations; ++iter) {
            particles<coord_t> p = p_orig;
            MPI_Barrier(MPI_COMM_WORLD);

            double t_start = MPI_Wtime();

            // Phase 1: Bounding Box
            double               t0   = MPI_Wtime();
            BoundingBox<coord_t> bbox = get_global_bounding_box(q, p);
            times_bbox[iter]          = MPI_Wtime() - t0;

#if defined(DCOMPOSITION_TYPE_SAMPLING)
            // Phase 3: Splitter Generation via Sampling
            double                t2        = MPI_Wtime();
            std::vector<uint64_t> splitters = get_deterministic_splitters(q, p, bbox);
            times_splitters[iter]           = MPI_Wtime() - t2;
            times_hist[iter]                = 0.0; // Bypassed
#else
            // Phase 2: Histogram
            double           t1   = MPI_Wtime();
            std::vector<int> hist = get_global_histogram(q, p, bbox, TOP_BITS);
            times_hist[iter]      = MPI_Wtime() - t1;

            // Phase 3: Splitter Generation
            double                t2        = MPI_Wtime();
            std::vector<uint32_t> splitters = generate_splitters(hist, size, TOP_BITS);
            times_splitters[iter]           = MPI_Wtime() - t2;
#endif

            // Phase 4: Local Binning & Redistribution
            double t3 = MPI_Wtime();
#if defined(DCOMPOSITION_TYPE_SAMPLING)
            particles<coord_t> redistributed_p = redistribute_particles(q, p, splitters, bbox);
#else
            particles<coord_t> redistributed_p =
                redistribute_particles(q, p, splitters, bbox, TOP_BITS);
#endif
            times_redist[iter] = MPI_Wtime() - t3;

            // Phase 5: Explicit Halo Exchange
            // Approximate SPH smoothing length: 2% of the bounding box size
            coord_t h_max = static_cast<coord_t>(0.02) * (bbox.max_x - bbox.min_x);
            if (h_max <= static_cast<coord_t>(0.0))
                h_max = static_cast<coord_t>(2.0); // safety fallback

            double             t4        = MPI_Wtime();
            particles<coord_t> ghosted_p = exchange_halos(q, redistributed_p, h_max);
            times_halo[iter]             = MPI_Wtime() - t4;

            // Phase 6: Unified Tree Construction
            double t5            = MPI_Wtime();
            size_t total_ghosted = ghosted_p.pos_x.size();
            if (total_ghosted > 0) {
                TreeSoA tree(q, total_ghosted);
                build_bvh(q, ghosted_p, tree);
                q.wait();
                tree.free(q);
            }
            times_tree[iter] = MPI_Wtime() - t5;

            times_total[iter] = MPI_Wtime() - t_start;
        }

        // Calculate average times across iterations locally
        double local_avg_bbox = 0.0, local_avg_hist = 0.0, local_avg_splitters = 0.0;
        double local_avg_redist = 0.0, local_avg_halo = 0.0, local_avg_tree = 0.0,
               local_avg_total = 0.0;

        for (int iter = 0; iter < num_iterations; ++iter) {
            local_avg_bbox += times_bbox[iter];
            local_avg_hist += times_hist[iter];
            local_avg_splitters += times_splitters[iter];
            local_avg_redist += times_redist[iter];
            local_avg_halo += times_halo[iter];
            local_avg_tree += times_tree[iter];
            local_avg_total += times_total[iter];
        }
        local_avg_bbox /= num_iterations;
        local_avg_hist /= num_iterations;
        local_avg_splitters /= num_iterations;
        local_avg_redist /= num_iterations;
        local_avg_halo /= num_iterations;
        local_avg_tree /= num_iterations;
        local_avg_total /= num_iterations;

        // Perform reductions to get max and average timings across ranks
        double avg_times[7] = {
            local_avg_bbox,
            local_avg_hist,
            local_avg_splitters,
            local_avg_redist,
            local_avg_halo,
            local_avg_tree,
            local_avg_total
        };
        double max_times[7] = {0.0};
        double min_times[7] = {0.0};
        double sum_times[7] = {0.0};

        MPI_Reduce(avg_times, max_times, 7, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
        MPI_Reduce(avg_times, min_times, 7, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
        MPI_Reduce(avg_times, sum_times, 7, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        double local_rss = static_cast<double>(get_peak_rss()) / 1024.0; // in MB
        double max_rss = 0.0, sum_rss = 0.0;
        MPI_Reduce(&local_rss, &max_rss, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
        MPI_Reduce(&local_rss, &sum_rss, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            std::cout << "Dataset: " << label << " (" << total_n << " particles in " << num_files
                      << " file(s))\n";
            std::cout << "-------------------------------------------------------------------------"
                         "---------------\n";
            std::cout << std::left << std::setw(30) << "Phase" << std::right << std::setw(15)
                      << "Min Time (s)" << std::setw(15) << "Max Time (s)" << std::setw(15)
                      << "Avg Time (s)" << "\n";
            std::cout << "-------------------------------------------------------------------------"
                         "---------------\n";

            const char *phase_names[7] = {
                "1. Bounding Box",
                "2. Coarse Histogram",
                "3. Splitter Gen",
                "4. Local Binning & Redist",
                "5. Explicit Halo Exchange",
                "6. Unified Tree Build",
                "Total Domain Decomposition"
            };

            for (int i = 0; i < 7; ++i) {
                std::cout << std::left << std::setw(30) << phase_names[i] << std::right
                          << std::fixed << std::setprecision(6) << std::setw(15) << min_times[i]
                          << std::setw(15) << max_times[i] << std::setw(15) << (sum_times[i] / size)
                          << "\n";
            }
            std::cout << "-------------------------------------------------------------------------"
                         "---------------\n";
            std::cout << "Peak RSS Memory per Rank:  Max: " << std::fixed << std::setprecision(2)
                      << max_rss << " MB, Avg: " << (sum_rss / size) << " MB\n";
            std::cout << "========================================================================="
                         "===============\n\n";
        }
    }

    MPI_Finalize();
    return 0;
}
