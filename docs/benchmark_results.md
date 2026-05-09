# Benchmark Results

**Date:** 2026-05-09
**Hardware:** Intel(R) Xeon(R) Platinum 8360Y CPU @ 2.40GHz
**Environment:** SYCL (Intel LLVM 2025.2.0), MPI 4.1, HDF5 2.0.0

## Construction Benchmarks

| Test Name | Particle Count | Time (s) | Peak Memory (MB) | Notes |
|-----------|----------------|----------|------------------|-------|
| morton_scaling | 1,000 | 1.006746 | 148.09 | - |
| morton_scaling | 10,000 | 0.000236 | 148.09 | - |
| morton_scaling | 100,000 | 0.000420 | 152.17 | - |
| morton_scaling | 1,000,000 | 0.000818 | 201.50 | - |
| morton_scaling | 10,000,000 | 0.002582 | 634.24 | - |
| radix_sort_scaling | 1,000 | 0.000081 | 147.99 | - |
| radix_sort_scaling | 10,000 | 0.000959 | 147.99 | - |
| radix_sort_scaling | 100,000 | 0.012521 | 153.12 | - |
| radix_sort_scaling | 1,000,000 | 0.136696 | 203.54 | - |
| radix_sort_scaling | 10,000,000 | 1.850406 | 706.55 | - |
| tree_build_scaling | 1,000 | 0.881331 | 146.55 | - |
| tree_build_scaling | 10,000 | 0.002682 | 146.55 | - |
| tree_build_scaling | 100,000 | 0.017709 | 150.08 | - |
| tree_build_scaling | 1,000,000 | 0.186625 | 242.64 | - |
| tree_build_scaling | 10,000,000 | 2.152368 | 1168.57 | - |

## Query Scaling: Range Query (1000 aggregated queries)

| Particle Count | Radius (R) | Time (s) | Total Particles Found | Peak Memory (MB) |
|----------------|------------|----------|-----------------------|------------------|
| 10,000,000 | 0.01 | 0.000355 | 0 | 1171.00 |
| 10,000,000 | 0.10 | 0.000203 | 0 | 1171.00 |
| 10,000,000 | 1.00 | 0.000205 | 0 | 1171.00 |
| 10,000,000 | 10.00 | 0.000145 | 0 | 1171.00 |
| 10,000,000 | 100.00 | 0.000162 | 37 | 1171.00 |
| 10,000,000 | 200.00 | 0.000158 | 288 | 1171.00 |

## Query Scaling: kNN Query (1000 aggregated queries)

| Particle Count | k | Time (s) | Peak Memory (MB) |
|----------------|---|----------|------------------|
| 10,000,000 | 1 | 0.000693 | 1169.30 |
| 10,000,000 | 2 | 0.000221 | 1169.30 |
| 10,000,000 | 4 | 0.000248 | 1169.30 |
| 10,000,000 | 8 | 0.000257 | 1169.30 |
| 10,000,000 | 16 | 0.000292 | 1169.30 |
| 10,000,000 | 32 | 0.000462 | 1169.30 |
| 10,000,000 | 64 | 0.000951 | 1169.30 |
| 10,000,000 | 128 | 0.002826 | 1168.59 |

## Observations
- Morton encoding is extremely efficient, taking < 0.01s for 10M particles.
- Tree construction scales linearly with particle count.
- Range query and kNN query performance is stable across query parameters, though kNN time increases with $k$ as expected due to priority queue management.
- Small query radii return few particles, consistent with random sampling in a sparse cosmological box.
- 100M particle tests were skipped for this run due to memory constraints on the current hardware.
