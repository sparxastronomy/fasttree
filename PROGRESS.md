# Progress: SYCL-HLBVH Implementation

## Completed Tasks
- [x] Basic Morton encoding/decoding (2026-05-07)
- [x] Morton key sorting utility (2026-05-07)
- [x] HLBVH Tree Construction (SoA, pointer-free) (2026-05-07)
- [x] Range Query implementation (2026-05-07)
- [x] kNN Query implementation (2026-05-07)

## Planned Tasks
- [ ] Performance benchmarking on NVIDIA GPUs
- [x] MPI serialization tests (2026-05-07)
- [ ] Re-enable oneDPL and PSTL for high-performance builds on supported environments

## Notes
- Morton encoding uses 21 bits per dimension (63 bits total).
- Tree structure is strictly SoA and pointer-free (uses integer indices).
- Parallel hierarchy construction based on Karras (2012).
- Range and kNN queries use non-recursive stack-based traversal.
- **Bug identified (2026-05-07):**
  - **Memory Ordering:** The bottom-up bounding box computation in the tree builder had a memory visibility bug due to using `sycl::memory_order::relaxed` with atomic counters. Fixed by switching to `sycl::memory_order::acq_rel`.
  - **Topology Issue:** The Karras topology builder binary search to find the split point `s` used mathematically incorrect integer halving (`t = (l+1)/2; t/=2`) for non-power-of-2 ranges. This created heavily disconnected trees. Fixed by replacing it with a robust power-of-two decomposition search.
  - **kNN Priority Queue:** The GPU kNN priority queue incorrectly populated up to its static template array capacity (`MAX_K=32`) instead of dynamically respecting `k`. This caused the queue's internal sort to push the nearest elements out of bounds. Fixed the `PriorityQueue` struct to dynamically constrain insertion counts to `k`.
- **Environment Warning:** AdaptiveCpp/Homebrew on macOS shows a systemic `malloc` trap during SYCL kernel execution. Code is logically verified but runtime execution on this specific machine is blocked by the environment issue.
- **Portability:** Refactored to use standard C++ algorithms instead of oneDPL where possible to increase compatibility, though PSTL is disabled for macOS AppleClang.
