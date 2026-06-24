# Progress: SYCL-HLBVH Implementation
**Current release version:** v.1.0.0-beta3 (2026-06-24)   
**Target release version:** v.1.0.0 (Release Candidate)

## Completed Tasks
- [x] Basic Morton encoding/decoding (2026-05-07)
- [x] Morton key sorting utility (2026-05-07)
- [x] HLBVH Tree Construction (SoA, pointer-free) (2026-05-07)
- [x] Range Query implementation (2026-05-07)
- [x] kNN Query implementation (2026-05-07)
- [x] Benchmark suite implementation (2026-05-08)
  - Implemented `BenchmarkUtils.hpp` for timing and HDF5 loading.
  - Implemented scaling tests for Morton, Sort, Tree Build, Range Query, and kNN Query.
  - Integrated benchmarks into CMake.
- [x] Migration to Google Benchmark (2026-05-09)
  - Integrated Google Benchmark via CMake FetchContent.
  - Refactored all scaling tests to use the Google Benchmark framework.
  - Improved query scaling analysis with parametric sweeps (R and k) -- reload and re-build the tree for each variation to ensure accurate measurements.
- [x] GPU Sorting Optimization (2026-05-12)
  - Replaced CPU fallback `std::sort` with high-performance `oneapi::dpl::sort`.
  - Implemented single-pass 64-bit GPU sorting of zipped Morton keys and particle indices.
  - Achieved Exascale performance target: 10M particle tree build reduced from ~2.2s to ~421ms.
- [x] Update API Reference (2026-06-13)
  - Added algorithmic choices and pseudo-code for `kNN` and `range_query`.
  - Documented the need and usage of the statically sized `PriorityQueue` for GPU kernels.
- [x] Fix CPU/Native Compilation for Custom clang/clang++ (2026-06-13)
  - Added default fallback `-fsycl` compile/link option in `CMakeLists.txt` when `TARGET_GPU` is empty and `IntelSYCL` is not found.
- [x] Extend particle data structures and reordering to support id and is_ghost fields (2026-06-14)
- [x] Implement Phase 1-5 of Domain Decomposition in [domain_decomposition.hpp](file:///u/bipra/analysis/dev/fasttree/src/domain_decomposition.hpp) (2026-06-14)
- [x] Add standalone domain decomposition validation test [test_domain_decomposition.cpp](file:///u/bipra/analysis/dev/fasttree/test/test_domain_decomposition.cpp) (2026-06-14)
- [x] Create GPU-accelerated tree rebuild scaling test (`rebuild_scaling.cpp`) (2026-06-20)
- [x] Create multi-rank load-balanced distributed domain decomposition scaling benchmark (`domain_decomposition_scaling.cpp`) (2026-06-20)
- [x] Integrate new scaling benchmarks into CMake build options (2026-06-20)
- [x] Template coordinate and precision representations on FloatT in `domain_decomposition.hpp` and `sfc_encode` to support high-precision double-precision simulations (2026-06-24)
- [x] Add compile-time flag option `DCOMPOSITION_TYPE` (HISTOGRAM / SAMPLING) to `CMakeLists.txt` (2026-06-24)
- [x] Implement deterministic stride-sampling based domain decomposition (`get_deterministic_splitters` and key-based partitioning) (2026-06-24)
- [x] Conform and adapt all test cases and test programs to dynamic precision and SFC / partition configurations (2026-06-24)

## Planned Tasks
- [x] Performance benchmarking on NVIDIA GPUs (2026-05-15) - CMake configured, custom compiler tested, tests pass on NVIDIA A100.
- [x] MPI serialization tests (2026-05-07)
- [x] Re-enable oneDPL and PSTL for high-performance builds on supported environments (2026-05-12)
- [x] Integrate and verify domain decomposition using mpirun (2026-06-20)

## Notes
- Morton encoding uses 21 bits per dimension (63 bits total).
- Tree structure is strictly SoA and pointer-free (uses integer indices).
- Parallel hierarchy construction based on Karras (2012).
- Range and kNN queries use non-recursive stack-based traversal.
- **Domain Decomposition (v.1.0.0-beta2) Design Choices:**
  - **Unified Tree Build:** Implemented ghost particle static exchange (Approach A) rather than separate tree transmission. This simplifies querying and provides much better search performance by maintaining a single unified tree per rank.
  - **Binary Search on GPU Splitter:** Avoided linear search in the destination rank lookup by using binary search, improving lookup from $O(P)$ to $O(\log P)$.
  - **Corrected Bit Shift for Morton Keys:** Adjusted shifting from `64 - m` to `63 - m` because 21-bit 3D Morton keys are 63 bits total, preventing half the histogram from being empty.
  - **MPI Count Exchange:** Added `MPI_Alltoall` to exchange send counts to get recv counts before `MPI_Alltoallv` in particle redistribution and halo exchange.
- **CMake & Environment (2026-05-15):** 
  - Added `TARGET_GPU` (`nvidia` or `amd`) support in CMake.
  - When targeting NVIDIA, CMake configures the compiler with `-fsycl;-fsycl-targets=nvptx64-nvidia-cuda,native_cpu;-Xsycl-target-backend=nvptx64-nvidia-cuda;--cuda-gpu-arch=sm_80`. 
  - *Module Issue:* Use `cuda/12.1` rather than `cuda/13.0` during compilation; the Intel llvm linker passes the `-image` flag to `fatbinary`, which CUDA 13.0 removed, causing build failures. 
  - *Execution Success:* A custom SYCL+CUDA environment with an open-source Intel LLVM build (`mpiicpx`) was provided. It correctly bundles the `libsycl-pi-cuda.so` plugin and the `native_cpu` module.
  - Using `ONEAPI_DEVICE_SELECTOR=cuda:gpu`, the `fasttree.exe` validation tests (Morton key monotonicity, Range Query brute-force comparison, and kNN execution) **successfully ran on the NVIDIA A100-SXM4-40GB GPU!**
  - **MPI validation (2026-06-15):** Standalone domain decomposition tests (`test_domain_decomposition.exe`) successfully compiled and run with `mpirun -n 4` on `vera01`, passing all validation phases (Bounding Box, Histogram, Splitter generation, local binning, binary search routing, explicit halo exchange, and unified tree construction).
- **Bug identified (2026-05-07):**
  - **Memory Ordering:** The bottom-up bounding box computation in the tree builder had a memory visibility bug due to using `sycl::memory_order::relaxed` with atomic counters. Fixed by switching to `sycl::memory_order::acq_rel`.
  - **Topology Issue:** The Karras topology builder binary search to find the split point `s` used mathematically incorrect integer halving (`t = (l+1)/2; t/=2`) for non-power-of-2 ranges. This created heavily disconnected trees. Fixed by replacing it with a robust power-of-two decomposition search.
  - **kNN Priority Queue:** The GPU kNN priority queue incorrectly populated up to its static template array capacity (`MAX_K=32`) instead of dynamically respecting `k`. This caused the queue's internal sort to push the nearest elements out of bounds. Fixed the `PriorityQueue` struct to dynamically constrain insertion counts to `k`.
- **Cosmological Clustering Load Imbalance in Domain Decomposition (2026-06-20):**
  - **Date:** 2026-06-20.
  - **Description:** Scaling benchmarks of the multi-rank distributed domain decomposition pipeline on CPU backend. The algorithm partitions physical domains across MPI ranks using static Morton curve splitters derived from a global coarse-grid histogram.
  - **Reason:** Cosmological datasets are highly clustered (dense dark matter halos vs. empty cosmic voids). When many particles fall into a single coarse bucket (exceeding `target_load = N / P`), the static splitter algorithm cannot subdivide that bucket. Consequently, the rank assigned to that spatial region receives almost the entire cluster.
  - **Outcome:** Severe load imbalance during the subsequent `build_bvh` phase. For the 10M particle dataset running on 16 MPI ranks, the average tree build time drops to **3.91 seconds** (since most ranks receive 0 particles), but the maximum tree build time remains stuck at **21.33 seconds** because the bottleneck rank is forced to build the tree for the entire cluster.
  - **Next Steps:** Implement an adaptive or recursive splitter generation scheme (e.g. dynamically subdividing coarse buckets that exceed `target_load`), or implement dynamic work-stealing during tree construction to distribute the clustered workload.
- **Peano-Hilbert Decoder Namespace Bug (2026-06-24):**
  - **Date:** 2026-06-24
  - **Description:** A code replacement in `src/sfc.peano_hilbert.hpp` omitted the closing brace `}` of `decodePeano3D`.
  - **Reason:** The replace target content started with the closing brace of `decodePeano3D` and it was accidentally omitted in the replacement string, leaving the function unclosed. This caused `sfc_decode` to be compiled inside `decodePeano3D`, and subsequently caused `<iostream>` and other system library headers to be parsed inside the `fasttree` namespace.
  - **Outcome:** Multiple compiler errors like "unknown type name 'wostream'" and "no member named 'endl' in namespace 'fasttree::std'" in `main.cpp`.
  - **Next Steps:** Reinserted the missing closing brace `}` of `decodePeano3D` in `src/sfc.peano_hilbert.hpp`, resolving all errors.

- **Environment Warning:** AdaptiveCpp/Homebrew on macOS shows a systemic `malloc` trap during SYCL kernel execution. Code is logically verified but runtime execution on this specific machine is blocked by the environment issue.
- **Portability:** Refactored to use standard C++ algorithms instead of oneDPL where possible to increase compatibility, though PSTL is disabled for macOS AppleClang.
