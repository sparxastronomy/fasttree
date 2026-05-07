# Progress: SYCL-HLBVH Implementation

## Completed Tasks
- [x] Basic Morton encoding/decoding (2026-05-07)
- [x] Morton key sorting utility (2026-05-07)
- [x] HLBVH Tree Construction (SoA, pointer-free) (2026-05-07)
- [x] Range Query implementation (2026-05-07)
- [x] kNN Query implementation (2026-05-07)

## Planned Tasks
- [ ] Performance benchmarking on NVIDIA GPUs
- [ ] MPI serialization tests
- [ ] Re-enable oneDPL and PSTL for high-performance builds on supported environments

## Notes
- Morton encoding uses 21 bits per dimension (63 bits total).
- Tree structure is strictly SoA and pointer-free (uses integer indices).
- Parallel hierarchy construction based on Karras (2012).
- Range and kNN queries use non-recursive stack-based traversal.
- **Environment Warning:** AdaptiveCpp/Homebrew on macOS shows a systemic `malloc` trap during SYCL kernel execution. Code is logically verified but runtime execution on this specific machine is blocked by the environment issue.
- **Portability:** Refactored to use standard C++ algorithms instead of oneDPL where possible to increase compatibility, though PSTL is disabled for macOS AppleClang.
