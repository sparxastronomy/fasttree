# System Design: SYCL-HLBVH for Cosmological Simulations

## 1. Project Overview
This project provides a highly optimized, high-performance tree utility for spatial search and ranged queries (also kNN) used in cosmological hydrodynamical simulations (like AREPO and Gadget). 

The core algorithm is based on the **Hierarchical Linear Bounding Volume Hierarchy (HLBVH)** introduced by NVIDIA (Pantaleoni & Luebke, 2010).

## 2. Core Architectural Constraints
To meet the demands of distributed MPI environments and modern GPU hardware, the design adheres to the following absolute constraints:

### 2.1. Strictly Pointer-Free
The tree structure must contain **zero memory pointers** (`*`, `std::shared_ptr`, `std::unique_ptr`). 
* **Why:** The tree must be trivially serializable for MPI transmission to construct Locally Essential Trees (LETs).
* **How:** Tree topologies are defined using contiguous arrays where parent-child relationships are tracked via integer indices (e.g., `int left_child_idx`, `int right_child_idx`).

### 2.2. Structure of Arrays (SoA)
Data must be laid out in SoA format, strictly avoiding Array of Structures (AoS).
* **Why:** SoA guarantees perfectly coalesced memory access across SIMT warps on NVIDIA GPUs, drastically improving cache hit rates during Morton code generation and tree traversal.
* **How:** Instead of `std::vector<Node>`, we use `struct NodesSoA { std::vector<float> min_x; ... std::vector<int> left_child; };`.

### 2.3. SYCL & oneDPL Backend
The code is written in SYCL (DPC++) targeting NVIDIA GPUs via the Codeplay plugin.
* **Why:** Provides a single-source C++ standard for heterogeneous computing.
* **How:** We utilize `oneapi::dpl` as the direct equivalent to NVIDIA Thrust for parallel primitives (sort, compact, scan) and `sycl::local_accessor` to map to GPU Shared Memory.

### 2.4. Naming Conventions & Code Organization
* The core implementation should be confined to either a single file (`src/hlbvh.hpp`) or a strict `.h`/`.cpp` pair to facilitate easy inclusion as a Git submodule in downstream projects.
* Executables should follow the naming convention: `file.hpp`/`file.cpp` -> `file.exe` to ensure clarity and ease of use.
* Code that are compiled for a gpu target should be suffixed with `.gpu_<vendor>`, eg. : `file.hpp`/`file.cpp` -> `file.gpu_nvidia.exe` to clearly indicate the target platform and avoid confusion in multi-target environments.

## 3. Algorithm: HLBVH Pipeline
The tree builder executes a modified LBVH pipeline optimized for temporal coherence in dynamic datasets:
1.  **Morton Code Generation:** 30-bit spatial hashing of particle coordinates.
2.  **Compress-Sort-Decompress (CSD):** * *Compress:* Run-length encode particles into coarse spatial grids (top 18 bits).
    * *Sort:* Sort only the highly compressed block descriptors using `oneapi::dpl::sort`.
    * *Decompress:* Expand the sorted blocks back out.
3.  **Intra-Voxel Sort:** Perform fine-grained odd-even sorting (bottom 12 bits) entirely within SYCL local memory (`__shared__` memory on NVIDIA) to minimize global memory bandwidth.
4.  **Treelet Emission:** Emit internal nodes by analyzing bit-planes without expanding a massive intermediate node arena.
5.  Reference idea in the following paper: "HLBVH: Hierarchical LBVH Construction for Real-Time Ray Tracing of Dynamic Geometry" (Pantaleoni & Luebke, 2010). (also available locally at `docs/HLBVH-final.pdf`)

## 4. Querying
- The end goal is to have efficient range queries (e.g., `range_query(point, radius)`) using a non-recursive stack-based traversal of the HLBVH. The query logic will be designed to minimize divergent execution paths across threads and maximize occupancy on GPUs / vectorization on CPUs.
- There should be two query modes:
  1. Range query: returns all particles within a specified radius of a query point, call-sign `range_query` that takes a point and radii r_min, r_max, and returns a list of particle indices.
  2. kNN query: returns the k nearest neighbors to a query point, call-sign `knn_query` that takes a point and an integer k, and returns the indices of the k nearest particles.


## 5. Integration
The project is designed to be included as a Git submodule. The core implementation is confined to a single, easily included header file (`src/hlbvh.hpp`) to prevent complex CMake linking chains in the downstream simulation frameworks.

## 6. Testing and Benchmarking
- A comprehensive suite of unit tests will be implemented in the `test/` directory
- At `test/benchmark/`, all the benchmarking test should reside. The sole purpose of this sub-directory is to do scaling tests and performance benchmarks. Here are minimal scaling tests that shold be checked off:
  - `morton_scaling.cpp`: Scaling of morton code generation with increasing particle counts sampled from a cosmological simulations (1K, 10K, 100K, 1M, 10M, 100M).
  - `radix_sort_scaling.cpp`: Scaling of radix sort with increasing particle counts (1K, 10K, 100K, 1M, 10M, 100M).
  - `intra_voxel_sort_scaling.cpp`: Scaling of intra-voxel odd-even sort with increasing particle counts (1K, 10K, 100K, 1M, 10M, 100M).
  - `tree_build_scaling.cpp`: Scaling of the entire tree build pipeline with increasing particle counts (1K, 10K, 100K, 1M, 10M, 100M).
  - `range_query_scaling.cpp`: Scaling of range query performance with increasing particle counts (1K, 10K, 100K, 1M, 10M) aggreagated over different query location and different radius values (0.01, 0.1, 1.0, 10.0, 100.0, 200.0).
  - `knn_query_scaling.cpp`: Scaling of kNN query performance with increasing k values (1, 2, 4, 8, 16, 32, 64, 128) agreegated over different query location and different particle counts (1K, 10K, 100K, 1M, 10M).
- The particle distribution for the tests are listed in the `test/benchmark/config.txt` file with the following format: `Count path_to_hdf5_file`.
  - Counts should be in the format of 1K, 10K, 100K, 1M, 10M, 100M.
  - The HDF5 files should contain the following datasets: `Coordinates` (Nx3), `Velocities` (Nx3), `Masses` (N) under the group `/PartType0/`

- All the benchmark should also store the memory usage relevant for the test (e.g. for morton_scaling, the memory used for the morton code array and the intermediate integer arrays should be stored. )
- All the test timings should be recorded in a tabular format in `docs/benchmark_results.md` with the following columns:
  - Test Name
  - Hardware Used (e.g., NVIDIA A100, AMD MI250, Intel Xe along with number of GPUs/CPUs used)
  - Particle Count
  - Query Parameters (e.g., radius for range query, k for knn query and number of particles for aggregation)
  - Time Taken (seconds)
  - Memory Usage (MB)
  - Notes (e.g. any anomalies observed)

### What not to profile / benchmark:
- For the first release, we will not be focusing on micro-optimization of the code. 
- Therefore things such as GPU occupancy, register usage, shared memory usage, and other low-level performance metrics will not be profiled or benchmarked in the initial release.
- For now the GPU scaling is only limited to 1 GPU, so all the GPU benchmarks should be run on a single GPU. Multi-GPU scaling will be considered in future releases.
- No MPI benchmark for the initial release 
  - No scaling test for Tree communication time over MPI
  - No scaling test for LET construction time over MPI
  - No scaling test for overall simulation time with and without the tree utility in a distributed setting

- No scaling of the Heirarchial consitruction i.e time to rebuild the tree after the particles have slightly moved. This will be considered in the next release (i.e Version 2.0+) after we have the initial tree build and query working.


## 7. Release Plan
- [x] **v.1.0.0-beta1 :** Initial implmentation of the HLBVH tree builder, range_query and kNN query and benchmark suite.
  - This release will focus on the core implementation of the HLBVH tree builder and the query functions, along with a basic benchmark suite to validate performance on either a single NVIDIA GPU or a single AMD GPU or a single compute node.
  - The tree sturcture will be stirctly pointer-free and should support basic MPI serialization tests.

- [ ] **v.1.0.0-beta2 :** This release should focus on implementing domain decomposition.
  - The initial plan is listed in `PLAN.md` where domain decomposition is carried out to top level splitting. 