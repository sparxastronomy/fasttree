# Profiling Guide for cosmological SYCL-HLBVH

This guide explains how to compile the FastTree codebase for profiling and use the provided profiling scripts to locate bottlenecks on multi-socket CPUs and NVIDIA GPUs using Intel VTune/APS and NVIDIA Nsight tools.

---

## 1. Compilation for Profiling

To obtain line-by-line source mapping and meaningful function call-stacks in profiling reports, the binaries must be compiled with optimization enabled (`-O3`) but with full debug symbols (`-g`).

In CMake, configure this using the `RelWithDebInfo` build type:

### For CPU Profiling (Intel Compiler Suite / DPC++)
```bash
mkdir -p build_cpu && cd build_cpu
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo \
      -DSFC_TYPE=PEANO_HILBERT \
      -DDCOMPOSITION_TYPE=HISTOGRAM \
      -DCOORDS_TYPE=DOUBLE \
      -DBENCHMARK=ON \
      -DCMAKE_C_COMPILER=clang \
      -DCMAKE_CXX_COMPILER=clang++ \
      -DMPI_C_COMPILER=mpiicx \
      -DMPI_CXX_COMPILER=mpiicpx \
      ..
make -j
```

### For GPU Profiling (NVIDIA A100/H100 GPUs)
```bash
mkdir -p build_gpu && cd build_gpu
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo \
      -DSFC_TYPE=MORTON \
      -DDCOMPOSITION_TYPE=HISTOGRAM \
      -DCOORDS_TYPE=DOUBLE \
      -DTARGET_GPU=nvidia \
      -DBENCHMARK=ON \
      -DCMAKE_C_COMPILER=clang \
      -DCMAKE_CXX_COMPILER=clang++ \
      -DMPI_C_COMPILER=mpiicx \
      -DMPI_CXX_COMPILER=mpiicpx \
      ..
make -j
```

---

## 2. CPU Profiling: Intel APS & VTune

The `profile_cpu.sh` script automates profiling via Intel's **Application Performance Snapshot (APS)** and **VTune Profiler**.

Location: `test/benchmark/profile_cpu.sh`

### A. Intel APS (High-Level Overview)
APS is designed to quickly capture MPI communication vs. computation balance, memory bandwidth utilization, and vectorization intensity.

To profile only the largest dataset (`N=10M`), specify the `-f 10M` option. This filters the configuration file so only the `10M` snapshot is loaded:
```bash
# Run the domain decomposition scaling test under APS with 8 ranks for 10M dataset
./test/benchmark/profile_cpu.sh -n 8 -t aps -f 10M domain_decomposition_scaling.exe
```
This generates an HTML report at:
`profile_data/aps_result_<executable>_<ranks>ranks_<timestamp>/aps_report.html`

### B. Intel VTune (Detailed CPU Analysis)
You can run VTune with three specific collection modes:
* **`hotspots`**: Identify which C++ source lines or OpenMP loops consume the most CPU cycles.
* **`memory`**: Detect DRAM bandwidth bottlenecks and L1/L2/L3 cache miss rates.
* **`threading`**: Identify thread synchronization overhead, locks, and load imbalances between cores.

For profiling, always target the biggest dataset (`10M`) using the `-f` filter to isolate the run and avoid profiling multiple smaller sweeps:
```bash
# Profile hotspots on a single-node tree build for the 10M dataset
./test/benchmark/profile_cpu.sh -t hotspots -f 10M tree_build_scaling.exe

# Profile memory-bandwidth on a 4-rank domain decomposition for the 10M dataset
./test/benchmark/profile_cpu.sh -n 4 -t memory -f 10M domain_decomposition_scaling.exe
```
VTune results are stored in `profile_data/`. You can view them by launching the VTune GUI:
```bash
vtune-gui profile_data/vtune_hotspots_tree_build_scaling.exe_* &
```

---

## 3. GPU Profiling: NVIDIA Nsight Systems & Compute

The `profile_gpu.sh` script automates system-wide profiling via **Nsight Systems (`nsys`)** and detailed kernel-level profiling via **Nsight Compute (`ncu`)**.

Location: `test/benchmark/profile_gpu.sh`

### A. NVIDIA Nsight Systems (`nsys`)
`nsys` profiles the timeline of host launches, memory copies (PCIe, H2D, D2H), GPU kernel execution times, NVTX ranges, and MPI exchanges.

To profile only the largest dataset (`10M`), specify the `-f 10M` option:
```bash
# System-wide trace of single-node tree build for the 10M dataset
./test/benchmark/profile_gpu.sh -t nsys -f 10M tree_build_scaling.exe

# System-wide trace of a 4-rank distributed run for the 10M dataset
./test/benchmark/profile_gpu.sh -n 4 -t nsys -f 10M domain_decomposition_scaling.exe
```
The report is saved as a `.nsys-rep` file in `profile_data/`. Open this file in the Nsight Systems GUI application to view the timeline.

### B. NVIDIA Nsight Compute (`ncu`)
`ncu` provides micro-architectural profiling of individual SYCL device kernels. It measures Speed-of-Light (SOL) percentages, memory pipeline details, register counts, and occupancy.

NCU adds significant profiling overhead. To avoid massive trace files, it is **highly recommended to filter by kernel name** using the `-k` option and isolate the run to the `10M` dataset:
```bash
# Profile only the Morton space-filling curve encoding kernel for the 10M dataset
./test/benchmark/profile_gpu.sh -t ncu -k "sfc_encode" -f 10M tree_build_scaling.exe

# Profile the local tree topology builder kernel for the 10M dataset
./test/benchmark/profile_gpu.sh -t ncu -k "build_tree" -f 10M tree_build_scaling.exe
```
Open the generated `.ncu-rep` report in the NVIDIA Nsight Compute GUI to analyze memory vs. compute boundaries and get optimization recommendations.

---

## 4. Key Performance Bottlenecks to Watch For

1. **MPI Communication vs. Binning (APS/VTune):**
   Compare the time spent in `MPI_Alltoallv` against local sorting/binning. A high percentage of MPI time indicates network saturation or domain load imbalances.
2. **GPU Memory Transfers (Nsight Systems):**
   Look for CPU-GPU memory copies inside loops. Ensure that particle copies happen outside timed benchmark sweeps and that device-side operations use USM pointers directly.
3. **SYCL Kernel Occupancy (Nsight Compute):**
   Check if kernels (e.g. `sfc_encode` or `build_tree`) are compute-bound or memory-bound. If occupancy is low, look at register usage or shared memory allocation sizes.
