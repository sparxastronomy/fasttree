# FastTree

**FastTree** is a header-only, massively parallel C++23 / SYCL library for constructing linear Bounding Volume Hierarchies (HLBVH) and executing high-throughput spatial neighbor queries. Designed for cosmological simulations and hydrodynamics, FastTree supports CPU and GPU backends (NVIDIA CUDA, AMD ROCm (untested), Intel/x86 CPUs) and comes with an optional support for domain decomposition. 

[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://en.cppreference.com/w/cpp/23)
[![SYCL](https://img.shields.io/badge/SYCL-2020-red.svg)](https://www.khronos.org/sycl/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Benchmark Dashboard](https://img.shields.io/badge/Dashboard-Live%20Benchmarks-green.svg)](https://sparxastronomy.github.io/fasttree-dashboard/)

---

## 🚀 Interactive Benchmark Dashboard

Comprehensive scaling benchmarks across multiple datasets ($N = 1\text{K}$ to $10\text{M}$ particles), coordinate precisions (`double`, `int32`, `int64`), and boundary conditions (periodic / non-periodic) are available on the live interactive dashboard:

👉 **[https://sparxastronomy.github.io/fasttree-dashboard/](https://sparxastronomy.github.io/fasttree-dashboard/)**

*(Note: The dashboard renders client-side directly in the browser; please allow few seconds (~20s) for initial load.)*

### Key Performance Highlights ($N = 10\text{M}$ Production Scale, cosmological simulation snapshot)

- **Self-$k\text{NN}$ on GPU:** **$864\,\text{M}$ queries/sec** ($18.6\times$ faster than legacy baseline at $k=1$; $3.5\times - 6.1\times$ faster across $k \ge 2$).
- **Self-$k\text{NN}$ on CPU:** Up to **$4.82\times$ faster** for large $k \ge 64$ via native SIMD vector-width alignment and parallel local heaps.
- **Tree Construction:** $10\text{M}$ particle BVH construction in **$\sim 400\,\text{ms}$** on NVIDIA A100 GPUs using oneDPL GPU radix sorting.

---

## ✨ Key Architectural Features

- **Strict Structure-of-Arrays (SoA) & Pointer-Free Design:** Tree nodes and bounding boxes are stored in flat contiguous memory indexed by integer offsets. The tree representation is trivially serializable as raw byte buffers for MPI communication.
- **Space-Filling Curves:** Fast parallel 3D Morton encoding and 3D Peano-Hilbert curves using lookup tables.
- **Multi-Precision Coordinate Support:** Native support for both floating-point (`float`, `double`) and fixed-point integer coordinate representations (`int32`, `int64`, `int128`).
- **SFC-Grouped Lockstep Traversal:** Consecutive threads process particles adjacent on the space-filling curve, turning divergent global memory fetches into unified broadcast loads.
- **Dynamic SIMD Vector-Width Dispatch:** Automatically tunes traversal sub-group width ($W=8, 16, 32$) to hardware register widths (AVX-512 / GPU warps), eliminating CPU lane emulation overhead.
- **Multi-Tier Parallel Heaps:** `RegisterMaxHeap` ($k \le 32$, private registers), `LocalMaxHeap` ($k > 32$, shared memory per-lane heaps), and `SortedMergeHeap` (warp-cooperative bitonic merge).

> ⚠️ **Important: SYCL In-Order Queue Assumption**  
> FastTree kernels do not perform synchronous host-blocking `.wait()` calls internally. Callers **must** pass an in-order SYCL queue (`sycl::property::queue::in_order{}`) or explicitly manage SYCL event dependencies for asynchronous execution pipelines.

> ⚠️ **Domain Decomposition**  
The currently implemented domain decompostion is a proof-of-concept and is very simple based on either partitioning of the SFC of histogram sampling of the SFC key. It has not been extensively tested in production workflows. For now we recomend to use your own domain decomposition strategies for production scale simulations.
Please use with caution and report any issues. 


---

## 📦 Quickstart & Usage Examples

For full API signatures and data structures, see the [Complete API Reference](docs/API_REFERENCE.md).

### 1. Initializing Particles and Building the BVH

```cpp
#include <sycl/sycl.hpp>
#include "hlbvh.hpp"

int main() {
    // 1. Initialize an in-order SYCL queue
    sycl::queue q{sycl::property::queue::in_order{}};

    // 2. Prepare particle Structure of Arrays (SoA)
    size_t n = 1'000'000;
    particles<coord_t> p;
    p.pos_x.resize(n); p.pos_y.resize(n); p.pos_z.resize(n);
    p.id.resize(n);    p.is_ghost.resize(n, 0);

    // ... populate particle coordinates ...

    // 3. Build linear BVH tree
    TreeSoA tree(q, n);
    build_bvh(q, p, tree);

    // 4. Free tree device memory when finished
    tree.free(q);
    return 0;
}
```

---

### 2. SFC-Grouped Self-$k\text{NN}$ Query (Voronoi & SPH Density)

Finds the $k$ nearest neighbors for every particle within the tree.

```cpp
int k = 32;
// Allocate result buffers (Unified Shared Memory or Device memory)
size_t *d_indices = sycl::malloc_device<size_t>(tree.num_leaves * k, q);
dist_t *d_dists   = sycl::malloc_device<dist_t>(tree.num_leaves * k, q);

// Run lockstep grouped self-kNN search (exclude_self=true skips the query particle itself)
self_knn_query(q, tree, k, d_indices, d_dists, /*exclude_self=*/true);

q.wait();

// Cleanup
sycl::free(d_indices, q);
sycl::free(d_dists, q);
```

To query only a specific subset of particles (e.g. active Voronoi particles):
```cpp
// leaf_ids contains indices in leaf-rank order [0, num_leaves - 1]
self_knn_query_subset(q, tree, d_leaf_ids, num_active, k, d_indices, d_dists, /*exclude_self=*/true);
```

---

### 3. Arbitrary Point $k\text{NN}$ Query

Finds the $k$ nearest neighbors for external query coordinates $(qx, qy, qz)$.

```cpp
int num_queries = 10'000;
int k = 64;

// External query coordinate arrays (device or host pointers)
coord_t *d_qx = /* ... */, *d_qy = /* ... */, *d_qz = /* ... */;
size_t  *d_results    = sycl::malloc_device<size_t>(num_queries * k, q);
dist_t  *d_sq_dists   = sycl::malloc_device<dist_t>(num_queries * k, q);

// Automatically dispatches to RegisterMaxHeap (k <= 32) or SortedMergeHeap (k > 32)
knn_query(q, tree, d_qx, d_qy, d_qz, k, num_queries, d_results, d_sq_dists);

q.wait();
```

---

### 4. Spherical Radius / Range Query

Finds all particles within a distance range $[r_{\min}, r_{\max}]$ from query coordinates.

```cpp
int num_queries = 10'000;
int max_results_per_query = 128;

int *d_results = sycl::malloc_device<int>(num_queries * max_results_per_query, q);
int *d_counts  = sycl::malloc_device<int>(num_queries, q);
coord_t *d_rmin = /* ... */, *d_rmax = /* ... */;

range_query(q, tree, d_qx, d_qy, d_qz, d_rmin, d_rmax, num_queries,
            d_results, d_counts, max_results_per_query);

q.wait();
```

---

## ⚙️ CMake Configuration & Build Options

FastTree is configured using standard CMake cache variables:

| CMake Variable | Options | Default | Description |
| :--- | :--- | :--- | :--- |
| `TARGET_GPU` | `nvidia`, `amd`, `""` (CPU) | `""` | Configures device target flags (`nvptx64-nvidia-cuda` for NVIDIA, `amdgcn-amd-amdhsa` for AMD). |
| `SFC_TYPE` | `MORTON`, `PEANO_HILBERT` | `MORTON` | Selects the space-filling curve used for spatial partitioning and leaf ordering. |
| `COORDS_REPRESENTATION` | `FLOAT`, `INTEGER` | `FLOAT` | Chooses between floating-point and integer coordinate representations. |
| `COORDS_TYPE` | `FLOAT`, `DOUBLE` | `FLOAT` | Sets coordinate precision type when `COORDS_REPRESENTATION=FLOAT`. |
| `POSITIONS_PRECISION` | `32`, `64`, `128` | `32` | Sets integer bit precision when `COORDS_REPRESENTATION=INTEGER`. |
| `PERIODIC_BC` | `ON`, `OFF` | `OFF` | Enables minimum image convention for periodic domain boundary conditions. |
| `RETURN_ORIG_INDICES` | `ON`, `OFF` | `OFF` | If `ON`, queries return original particle indices; if `OFF`, returns sorted leaf indices. |
| `DCOMPOSITION_TYPE` | `HISTOGRAM`, `SAMPLING` | `HISTOGRAM` | Selects domain decomposition splitter strategy (histogram vs. stride sampling). |
| `BUILD_BENCHMARKS` | `ON`, `OFF` | `OFF` | Builds Google Benchmark performance suite in `test/benchmark/`. |

### Example Build Commands

```bash
# 1. Build for CPU (double-precision, Peano-Hilbert curve)
cmake -B build_cpu -DCMAKE_CXX_COMPILER=icpx \
      -DCOORDS_REPRESENTATION=FLOAT -DCOORDS_TYPE=DOUBLE \
      -DSFC_TYPE=PEANO_HILBERT -DCMAKE_BUILD_TYPE=Release
cmake --build build_cpu -j

# 2. Build for NVIDIA GPU (sm_80, A100 / H100)
cmake -B build_gpu -DCMAKE_CXX_COMPILER=icpx \
      -DTARGET_GPU=nvidia -DCOORDS_REPRESENTATION=FLOAT -DCOORDS_TYPE=DOUBLE \
      -DSFC_TYPE=PEANO_HILBERT -DCMAKE_BUILD_TYPE=Release
cmake --build build_gpu -j

# 3. Build for 64-bit Integer Coordinates with Periodic Boundaries
cmake -B build_int64 -DCMAKE_CXX_COMPILER=icpx \
      -DCOORDS_REPRESENTATION=INTEGER -DPOSITIONS_PRECISION=64 \
      -DPERIODIC_BC=ON -DCMAKE_BUILD_TYPE=Release
cmake --build build_int64 -j
```
---

## Testing Environment
- Compilers tested: 
    - Intel oneAPI DPC++ Compiler (icpx) 2026.0 (OpenCL acclerated testing) (for CPU benchmarks)
    - OneAPI DPC++ Compiler with CUDA backend (Cuda version: 13.0) (for NVIDIA GPU benchmarks) [[see release tag](https://github.com/intel/llvm/releases/tag/v7.0.0)]
    - Adaptive CPP (version: 25.10.0) [[see release tag](https://github.com/AdaptiveCpp/AdaptiveCpp/releases/tag/v25.10.0)] (tested with OpenMP acclerated and CUDA backend)


---

## 📖 Complete Documentation

- [**API Reference (`docs/API_REFERENCE.md`)**](docs/API_REFERENCE.md): Comprehensive documentation of all data structures, functions, and algorithms.
- [**Benchmark Results (`docs/benchmark_results/`)**](docs/benchmark_results/): Raw scaling logs and performance data across CPU and GPU hardware.
- [**Release Notes (`release_notes/releasenote_1.2.0-beta1.md`)**](release_notes/releasenote_1.2.0-beta1.md): Detailed release change log, optimization breakdown, and benchmark tables.

---

## 📝 Acknowledgment & Citation

If you use FastTree in your research, simulation frameworks, or scientific publications, please acknowledge and cite the project:

```bibtex
@software{fasttree2026,
  author       = {Bipradeep Saha},
  title        = {FastTree: Massively Parallel SYCL Linear Bounding Volume Hierarchy and Spatial Search Framework},
  year         = {2026},
  publisher    = {GitHub},
  url          = {https://github.com/sparxastronomy/fasttree}
}
```
