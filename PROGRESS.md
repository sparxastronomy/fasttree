# Progress: SYCL-HLBVH Implementation
**Current release version:** v.1.1.0 (Released)
**Target release version:** v.1.2.0 (Planned)

## Completed Tasks
- [x] Release v1.2.0-beta2: GPU Scaling Benchmark Comparison & NVIDIA SYCL Target Fix (2026-08-27): Fixed `GPU_SYCL_FLAGS` in `CMakeLists.txt` to remove experimental `,native_cpu` for `TARGET_GPU=nvidia`, resolving LLVM `vecz` vector packetizer assertion crashes during CUDA linking. Completed full GPU scaling benchmark comparisons for all 6 configurations (`double`, `int32`, `int64` with `PERIODIC_BC=OFF/ON`) on NVIDIA SM_80 GPUs. Documented complete comparative tables and architectural analyses in `release_1.2.0-beta1.md`, demonstrating up to **$18.6\times$ speedups** at $k=1$, **$2.4\times - 6.1\times$ speedups** for intermediate $k \in [2, 32]$, and **$3.5\times - 4.3\times$ speedups** for large $k \ge 64$ across all 10M-particle production datasets.
- [x] Release v1.2.0-beta2: CPU Scaling Benchmark Comparison & Sub-Group Architectural Analysis (2026-08-26): Formatted benchmark comparison tables across all 6 CPU configurations (`double`, `int32`, `int64` with `PERIODIC_BC=OFF/ON`) and documented the performance characteristics in `release_1.2.0-beta1.md`.
- [x] Release v1.2.0-beta2: SFC-Grouped Self-kNN Query & Warp-Merge Heap Test Suite (2026-08-26): Created comprehensive test harness in `test/test_self_knn.cpp` validating `self_knn_query` ($k \le 32$ and $k > 32$), `self_knn_query_subset`, and `SortedMergeHeap` / `LocalMaxHeap` against host $O(N^2)$ brute-force references across $k \in \{16, 32, 33, 64, 128, 256\}$ with both `exclude_self = true` and `false`. Implemented: (1) exact index verification with robust full-dataset tie-detection, (2) per-query duplicate index detection, (3) isolated unit tests for `LocalMaxHeap` and `SortedMergeHeap`, (4) adversarial interleaved subset tests creating maximum divergence across warp lanes, and (5) multi-precision verification across `double`, `int32`, and `int64` coordinate representations in both non-periodic and periodic boundary modes, with both `RETURN_ORIG_INDICES=OFF` and `ON`. Updated `test/benchmark/self_knn_query_scaling.cpp` and `CMakeLists.txt` build targets.
- [x] Release v1.2.0: Scaling Benchmark Multi-Precision Compatibility & Automation (2026-08-08): Refactored `test/benchmark/` code (`BenchmarkUtils.hpp`, `knn_query_scaling.cpp`, `range_query_scaling.cpp`, `gpu_sort_scaling.cpp`, `tree_build_scaling.cpp`, `rebuild_scaling.cpp`, `sfc_encoding_scaling.cpp`, `self_knn_query_scaling.cpp`) to support both double-precision floating-point and integer coordinate representations (`int32`, `int64`). Updated `knn_query_scaling.cpp` results buffer pointer to `size_t*`. Added `-Wno-c2y-extensions` flag to `test/benchmark/CMakeLists.txt` for `icpx` under C++23. Created automated scaling test execution scripts `test/benchmark/run_cpu_scaling.sh` (CPU), `test/benchmark/run_gpu_scaling.sh` (GPU), and `test/benchmark/run_self_knn_scaling.sh` (dedicated Self-kNN) covering 6 CPU & 6 GPU configurations (`double`, `int32`, `int64` with `PERIODIC_BC=OFF/ON`) outputting distinct Markdown result files to `docs/benchmark_results/`.
- [x] Release v1.2.0: Periodic Boundary & Multi-Precision Bug Fixes (2026-08-05): Guarded periodic kNN and range query tests with FASTTREE_INTEGER_COORDS to align with coordinate torus logic. Fixed RM2/rm2 redefinition compiler errors in range query. Migrated kNN query API output distances from coord_t to dist_t to prevent distance truncation in 32-bit integer builds. Fixed uint128_t coordinate mapping from hi to lo word to resolve SFC encoding and distance evaluation errors in 128-bit integer builds.
- [x] Release v1.2.0: MaxHeap Migration & Unified KNN Query Dispatch (2026-08-04): Created `src/maxheap.hpp` containing `RegisterMaxHeap` ($k \le 32$, register-resident) and `SharedMaxHeap` ($k > 32$, shared-memory with parallel bitonic sorting). Refactored `knn_query` in `src/hlbvh.hpp` to dispatch based on $k$, returning squared distance $d^2$ directly. Validated across `build_cpu`, `build_int32`, `build_int64`, and `build_int128` target configurations.
- [x] Release v1.2.0: Caller-Managed Distance Scaling (2026-08-04): Removed internal `/ 3.0` division in `knn_query` to eliminate non-exact binary floating-point division round-off error, leaving distance scaling to caller code. Updated `docs/API_REFERENCE.md` and `test/main.cpp`.
- [x] Release v1.2.0: API Reference Documentation Update for MaxHeap & Distance Squared Normalization (2026-08-04): Updated Section 6 of `docs/API_REFERENCE.md` to document `RegisterMaxHeap`, `SharedMaxHeap`, squared distance return format, and integer coordinate 1/3.0 normalization factor decoding formulas.
- [x] Release v1.1.0 Phase 1: AREPO Peano-Hilbert SFC using lookup tables (`rottable3`, `subpix3`, multi-word `sfc_key`, `uint128_t` emulation struct) (2026-08-02)
- [x] Release v1.1.0 Phase 1: Compile-time choice between floating-point vs integer coordinate representations (`FLOAT` / `INTEGER`) via CMake (`FASTTREE_INTEGER_COORDS`, `POSITIONS_IN_32BIT`, `POSITIONS_IN_64BIT`, `POSITIONS_IN_128BIT`) (2026-08-02)
- [x] Release v1.1.0 Phase 1: Multi-precision tree construction (`hlbvh.hpp`), domain decomposition (`domain_decomposition.hpp`), and test suite (`main.cpp`, `test_domain_decomposition.cpp`) support for multi-word `sfc_key` and integer positions (2026-08-02)
- [x] Release v1.1.0 Phase 1: Full domain decomposition bucket extraction & lookup table SFC keys for 32-bit, 64-bit, and 128-bit integer coordinates (2026-08-02)
- [x] Release v1.1.0 Phase 1: Comprehensive Code Review Fixes (2026-08-02): Resolved `uint128_t` shift edge cases & missing operators (`+`, `-`, `>`, `<=`, `>=`), `to_sort_key` bit packing for 32-bit positions, SYCL device-safe `sycl::clz`/`sycl::sqrt`, bottom-up memory acquire fence, and KNN result ordering (2026-08-02)
- [x] Release v1.1.0 Phase 1: Coordinate Representation Conversion Utilities (`float_to_int_rep`, `int_rep_to_float`) and physical floating-point coordinate & distance output verification across floating-point, 32-bit, 64-bit, and 128-bit integer builds (2026-08-03)
- [x] Release v1.1.0 Phase 1: Final Code Review Resolution (2026-08-03): Fixed delta lambda `size_t` overflow guard (Bug 4), added max particle size guard (`n < INT_MAX / 2`) (Bug 5), documented 64-bit float conversion precision limit for `BITS_PER_DIMENSION > 52` (Bug 6), unified `phys_dist` calculation via `int_rep_to_float` in `main.cpp` (Bug 7), and verified exact brute-force range query match across all targets.
- [x] Release v1.1.0 Phase 1: API & Testing Documentation Updates (2026-08-03): Updated `docs/API_REFERENCE.md` and `docs/TESTING.md` with coordinate representation CMake flags, `float_to_int_rep` / `int_rep_to_float` utility specifications, precision notes, and multi-build execution instructions.
- [x] Release v1.1.0 Phase 1: Complete In-Code Function Docstrings & Step Notes (2026-08-03): Added Doxygen docstrings to all functions/structs in `hlbvh.hpp` and `sfc.peano_hilbert.hpp`, succinct step comments for algorithms, and developer notes on precision limits, stack depth guards, and USM pointer handling.
- [x] Release v1.1.0 Phase 1: kNN Query Direct Distance Squared ($d^2$) Return Optimization (2026-08-04): Updated `knn_query` in `hlbvh.hpp` to store $d^2$ directly in `result_dists` without GPU `sycl::sqrt` overhead, and updated `test/main.cpp` to convert integer/float $d^2$ to physical distance squared and physical distance.
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
- [x] Add comprehensive VTune, Intel APS, and NVIDIA Nsight profiling scripts, documentation, and performance report (2026-06-28)
- [x] Create comprehensive API documentation for `hlbvh.hpp` and `domain_decomposition.hpp` in `docs/API_REFERENCE.md` (2026-06-29)
- [x] Add Doxygen-compatible docstrings to all functions and helper structures (2026-06-29)
- [x] Add compile-time macro `RETURN_ORIG_INDICES` to control original vs. sorted index returning in `knn_query` and `range_query` (2026-07-21)
- [x] Optimize memory allocations and data copying in `hlbvh.hpp` (2026-07-21)
  - Consolidated 6 bounding box reduction buffers into a single 6-element allocation.
  - Eliminated all intermediate sorted shared memory buffers (`sx`, `sy`, `sz`, etc.) by directly writing sorted data to TreeSoA leaves.
  - Replaced duplicate host-to-device transfers with a single-pass staging buffer.
  - Fixed signature parameter order of `build_bvh` to resolve standard C++ compilation error.

## Planned Tasks
- [x] Performance benchmarking on NVIDIA GPUs (2026-05-15) - CMake configured, custom compiler tested, tests pass on NVIDIA A100.
- [x] MPI serialization tests (2026-05-07)
- [x] Re-enable oneDPL and PSTL for high-performance builds on supported environments (2026-05-12)
- [x] Integrate and verify domain decomposition using mpirun (2026-06-27)

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

- **compute_bbox Compilation & Reduction Bugs (2026-06-25):**
  - **Date:** 2026-06-25
  - **Description:** A compilation error occurred when calling `compute_bbox` with `particles` vectors, and subsequently the parallel reduction for computing the bounding box returned incorrect min/max coordinates (e.g., minimums were incorrectly zero-valued).
  - **Reason:** 
    1. The overload `compute_bbox(queue, particles, n)` was passing raw `std::vector` objects to the pointer overload instead of pointer addresses using `.data()`.
    2. The USM shared memory pointers allocated for the reductions (`d_min_x`, `d_max_x`, etc.) were not initialized on the host before kernel execution. In SYCL, the final reduction result combines the reduction tree values with the initial values in the output pointers. Because these pointers were uninitialized (or zeroed), the minimum was always computed as 0.
  - **Outcome:** Monotonicity test failed due to incorrect bounding boxes.
  - **Next Steps:** Templated both `compute_bbox` functions on `FloatT` to support dynamic precision, added `.data()` to vector arguments, and explicitly initialized all USM reduction variables to their corresponding identity values (`max()` for minimum reductions and `-max()` for maximum reductions) before kernel submission. All tests and domain decomposition MPI validations now compile and pass.

- **GPU Node Hangs due to Host Pointer Access in Kernels (2026-06-26):**
  - **Date:** 2026-06-26
  - **Description:** Executing GPU scaling benchmarks (such as `./sfc_encoding_scaling.exe` and `./gpu_sort_scaling.exe`) under Google Benchmark's repeated iterations hung the GPU node with 100% GPU utilization and minimal memory footprint.
  - **Reason:** Pointers retrieved from host-allocated vectors (`std::vector::data()`) were passed directly into SYCL device kernels (e.g., `sfc_encode`, `build_bvh` reordering, `exchange_halos` packing, etc.). On GPU platforms without stable Heterogeneous Memory Management (HMM), dereferencing host pointers on the GPU causes unhandled page faults or page-table thrashing, locking the GPU driver.
  - **Outcome:** Unstable GPU execution and context lockup under tight timing loops.
  - **Next Steps:** 
    1. Implemented USM validation helper templates (`ensure_device_readable`, `ensure_device_writable`, etc.) in [hlbvh.hpp](file:///u/bipra/analysis/dev/fasttree/src/hlbvh.hpp) using `sycl::get_pointer_type`. If a pointer type is `sycl::usm::alloc::unknown`, the helper automatically allocates temporary USM device memory, copies data, executes the kernel, and synchronizes back.
    2. Wrapped all kernel entry points in `hlbvh.hpp` and `domain_decomposition.hpp` with these USM redirection helpers.
    3. Refactored the `sfc_encoding_scaling` benchmark to pre-allocate USM memory, avoiding allocation and PCIe copy overhead inside the timed loop.
    4. Confirmed compilation passes successfully on the GPU target.

- **Option to Run MPI Benchmarks Separately (2026-06-26):**
  - **Date:** 2026-06-26
  - **Description:** Added a second parameter `mpi` (true/false) to `test/benchmark/run_all_variations.sh` to optionally run MPI benchmarks and save results separately.
  - **Outcome:** The automation script now accepts `<cpu|gpu>` followed by `<true|false>` to toggle running MPI benchmarks. When `mpi=true`, it saves execution metrics to `mpi_benchmark_results_*_all_variations.md`. When `mpi=false`, it runs only single-node benchmarks and saves to `benchmark_results_*_all_variations.md`.

- **Collective Desynchronization during File Discovery (2026-06-27):**
  - **Date:** 2026-06-27
  - **Description:** MPI ranks were independently executing `access()` checks and `get_hdf5_dataset_size()` queries on the parallel filesystem to discover files and calculate particle bounds.
  - **Reason:** It was assumed that concurrent read-only filesystem calls from all nodes would succeed without issues.
  - **Outcome:** Filesystem metadata latencies caused random ranks to fail to open files, returning `total_n = 0` and skipping the benchmark line. The remaining ranks proceeded to call MPI collectives, causing a mismatch and throwing `MPI_Alltoall: Message truncated` or hanging.
  - **Next Steps:** Refactored `domain_decomposition_scaling.cpp` so that Rank 0 alone queries the filesystem, and then broadcasts (`MPI_Bcast`) the file paths, sizes, and total count to all other ranks to keep them in sync.

- **OOM USM Allocation Failure and Silent Device Queue Hangs (2026-06-27):**
  - **Date:** 2026-06-27
  - **Description:** In `exchange_halos`, the matched indices USM buffer size `max_sends` was allocated as the pessimistic worst case: `n * neighbor_ranks.size() + 1`.
  - **Reason:** Morton curves have large discontinuities, causing bounding boxes to span the entire domain. Hence, `neighbor_ranks.size()` equals `P - 1` (7 neighbors for 8 ranks). For large datasets in double-precision, this $7 \times n$ allocation exceeded limits.
  - **Outcome:** `sycl::malloc_shared` returned `nullptr`, and dereferencing it inside the device kernels caused a silent page fault/illegal access, permanently locking the queue at `ranks = 8` and causing a deadlock.
  - **Next Steps:** Implemented a two-pass "count-then-allocate" filtering algorithm. Pass 1 counts the exact matches for each neighbor, then allocates `d_matched_indices` with the exact size, and Pass 2 writes the indices. This reduced memory usage of the matched index buffer by over 90%.

- [x] **Compile-time `RETURN_ORIG_INDICES` Macro (2026-07-21):**
  - Added dedicated `int *orig_idx` mapping array to [`TreeSoA`](file:///Users/bisaha/Codes/donotbackup/fasttree/src/hlbvh.hpp#L421) so that `tree.id` remains exclusively dedicated to unique particle IDs (e.g. AREPO IDs).
  - Added `#if defined(RETURN_ORIG_INDICES)` blocks in [`knn_query()`](file:///Users/bisaha/Codes/donotbackup/fasttree/src/hlbvh.hpp#L742) and [`range_query()`](file:///Users/bisaha/Codes/donotbackup/fasttree/src/hlbvh.hpp#L863) within [src/hlbvh.hpp](file:///Users/bisaha/Codes/donotbackup/fasttree/src/hlbvh.hpp).
  - When `RETURN_ORIG_INDICES` is defined, queries return the original input array index (`tree.orig_idx[node_idx]`). Otherwise, queries return the sorted particle indices (`node_idx - (n - 1)`).

- **Integer Coordinate Distance Overflow in Range Query (2026-08-02):**
  - **Date:** 2026-08-02
  - **Description:** Range query returned 0 matches for 32-bit integer coordinate representations despite particles existing within range.
  - **Reason:** In `range_query`, `RM2 = RM * RM` was evaluated directly in `coord_t` (`uint32_t`). For `RM = 524288`, `524288 * 524288 = 274877906944`, which exceeds `uint32_t::max()`, overflowing to 0 and forcing `d2 <= RM2` to fail for all particles.
  - **Outcome:** `range_query` test failed with 0 matches found vs. 69 expected.
  - **Next Steps:** Explicitly cast `RM` and `rm` to `double` (`double RM_d = static_cast<double>(dev_r_max[qi]); double RM2 = RM_d * RM_d;`) before squaring in both `range_query` and `PriorityQueue` in `knn_query`. All tests passed cleanly.

- **Domain Decomposition Sampling Splitter Fallback on Empty Ranks (2026-08-02):**
  - **Date:** 2026-08-02
  - **Description:** When running `test_domain_decomposition.exe` with initial load imbalance (Rank 0 holding 10,000 particles and Ranks 1, 2, 3 holding 0 particles), all splitters evaluated to 0, causing Rank 3 to receive all 10,000 particles after redistribution while Ranks 0, 1, and 2 received 0 particles.
  - **Reason:** `local_samples` vectors were initialized using `std::numeric_limits<sfc_key>::max()`, which is not specialized for custom struct `sfc_key` and evaluated to default-initialized `sfc_key{0,0,0}`. On empty ranks (where `n == 0`), `local_samples` remained filled with zeros. When Rank 0 gathered and sorted `global_samples`, 3/4 of the gathered samples were zeros, driving all percentile splitters to 0 and sending all particles to the maximum rank ($P - 1$). Additionally, `splitters[P]` was uninitialized.
  - **Outcome:** One rank spanned the entire domain while all other ranks were starved of particles.
  - **Next Steps:** Added `sfc_key_max()` helper function with explicit overloads for 32-bit, 64-bit, and 128-bit `MyIntPosType`. Initialized `local_samples` and `splitters[P]` with `sfc_key_max()`. Updated Rank 0 percentile splitter selection to filter out `sfc_key_max()` sentinel samples from empty ranks before taking percentiles. Re-testing verified near-perfect load balance (**~2500 particles per rank** across all 4 ranks).

- **`uint32_t` Bit Shift Truncation in `to_sort_key` (2026-08-02):**
  - **Date:** 2026-08-02
  - **Description:** `to_sort_key` for 32-bit position types returned `get_lo_word(k.ls)`, which only contained the bottom 21 bits of the 63-bit SFC key.
  - **Reason:** For `POSITIONS_IN_32BIT`, `BITS_PER_DIMENSION = 21`, total key length is 63 bits. The 63 bits are packed into `k.ls` (bottom 32 bits, bits 0..31) and `k.is` (top 31 bits, bits 32..62).
  - **Outcome:** `to_sort_key(k)` discarded the top 31 bits stored in `k.is`, causing histogram domain decomposition to place all particles in bucket 0.
  - **Next Steps:** Updated `to_sort_key` for 32-bit positions to assemble `(uint64_t(k.is) << 32) | uint64_t(k.ls)`.

- **C++ Preprocessor Macro vs. `constexpr` in `#if` Directives (2026-08-02):**
  - **Date:** 2026-08-02
  - **Description:** `#if KEY_TOTAL_BITS <= 64` in `extract_bucket_id` evaluated to true for all precision modes (including 126-bit and 192-bit keys).
  - **Reason:** `KEY_TOTAL_BITS` was defined as `constexpr int KEY_TOTAL_BITS = 3 * BITS_PER_DIMENSION;`. The C preprocessor does not evaluate C++ `constexpr` variables and treats un-defined preprocessor symbols as `0`, causing `#if 0 <= 64` to evaluate to true.
  - **Outcome:** High-precision keys entered the 64-bit branch and extracted incorrect word bits for histogram buckets.
  - **Next Steps:** Changed `constexpr int KEY_TOTAL_BITS` to `#define KEY_TOTAL_BITS (3 * BITS_PER_DIMENSION)`.

- **Multi-Word `sfc_key` Bucket Shift Offset per Precision Level (2026-08-02):**
  - **Date:** 2026-08-02
  - **Description:** Coarse histogram bucket extraction `extract_bucket_id(k, m)` required extracting top $m$ bits across 32-bit, 64-bit, and 128-bit `MyIntPosType` word structures.
  - **Reason:** Due to 3-bit chunk shift-register accumulation in `key_push_3bits`:
    - 32-bit (`POSITIONS_IN_32BIT`, 63 bits): Top bits live in `k.is` (bits 32..62). Shift: `(k.is << 32 | k.ls) >> 43`.
    - 64-bit (`POSITIONS_IN_64BIT`, 126 bits): Top bits live in `k.is` (bits 0..62 of `k.is`). Shift: `k.is >> 43`.
    - 128-bit (`POSITIONS_IN_128BIT`, 192 bits): Top bits live in `k.is.lo` (bits 0..63 of `k.is.lo`). Shift: `k.is.lo >> 44`.
  - **Outcome:** `extract_bucket_id` accurately extracts coarse histogram bucket IDs across all coordinate representation modes.

- **RM2 and rm2 Redefinition Error (2026-08-05):**
  - **Date:** 2026-08-05
  - **Description:** Redefinition of variables `RM2` and `rm2` after the `#endif` block in `range_query` in `src/hlbvh.hpp`.
  - **Reason:** Accidental duplication of assignment logic outside the `#if`/`#else` conditional preprocessor block.
  - **Outcome:** C++ compiler error: "redefinition of 'RM2'" / "redefinition of 'rm2'".
  - **Next Steps:** Removed the duplicate assignments after the `#endif`.

- **kNN Output Distance Truncation in 32-bit Integer Coordinates (2026-08-05):**
  - **Date:** 2026-08-05
  - **Description:** kNN query returned incorrect squared distances that were too small when using 32-bit integer coordinates.
  - **Reason:** `result_dists` was of type `coord_t*` (`uint32_t*`), but squared distances in coordinate space require 43 bits and overflowed `uint32_t::max()`.
  - **Outcome:** kNN test verification failed with distance check mismatches.
  - **Next Steps:** Changed `result_dists` type in `knn_query` to `dist_t*` (which is `uint64_t` in integer mode and `double` in float mode), and updated all test allocations.

- **uint128_t Coordinate Type Conversion and Zero-Distance Evaluation Bugs (2026-08-05):**
  - **Date:** 2026-08-05
  - **Description:** Build errors when casting `uint128_t` directly to `uint64_t`, followed by incorrect zero-distance evaluations and all-0 SFC keys in 128-bit mode.
  - **Reason:**
    1. `uint128_t` has no implicit conversion operator to `uint64_t`.
    2. Quantization stored 128-bit coordinates in `result.hi`, but `sfc_encode3D` and `get_lo_word(lo - val)` read from `result.lo`, causing all bits to evaluate to `0`.
  - **Outcome:** Compiler errors followed by 100% false range query matches and `d2 = 0` nearest neighbor results.
  - **Next Steps:** Replaced `static_cast<uint64_t>` with `get_lo_word()` in `hlbvh.hpp`. Modified `convert_to_sfc1d_impl` and `int_rep_to_float` for `uint128_t` to store and read coordinate bits from the `lo` word instead of `hi`.

- **64-bit Integer Distance Precision Loss in Host Test Harness (2026-08-26):**
  - **Date:** 2026-08-26
  - **Description:** Distance mismatch failures in `test/test_self_knn.cpp` when validating 64-bit integer coordinate representations against host brute force reference on $N=10,000$ clustered particles ($k \in \{33, 64, 128\}$).
  - **Reason:** In `test/test_self_knn.cpp`, the host brute force struct `BFNeighbor::dist_sq` and function `compute_point_dist_sq` used `double` instead of `dist_t` (`uint64_t`). Large squared integer distances in 64-bit coordinate space ($> 2^{53} \approx 9 \times 10^{15}$) suffered precision truncation in the lower mantissa bits when converted to `double`, causing the exact integer equality check against GPU `dist_t` to fail.
  - **Outcome:** False positive test failures with small low-bit mismatches on integer distance comparisons.
  - **Next Steps:** Replaced `double` with `dist_t` throughout the host brute-force calculation in `test/test_self_knn.cpp`. All tests in `build_cpu`, `build_int32`, and `build_int64` now pass with 100% accuracy.



