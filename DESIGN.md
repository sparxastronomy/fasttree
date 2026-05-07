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