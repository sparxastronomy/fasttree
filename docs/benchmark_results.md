# Benchmark Results (Google Benchmark Suite)

**Date:** 2026-05-09
**Hardware:** Intel(R) Xeon(R) Platinum 8360Y CPU @ 2.40GHz (72 cores)
**Environment:** SYCL (Intel LLVM 2025.2.0), Google Benchmark v1.9.1, MPI 4.1, HDF5 2.0.0

The following results include memory usage (Peak RSS) and query occupancy metrics. Query points are sampled from actual particle positions.

## Construction Benchmarks

| Benchmark Name | Particle Count | Time (CPU) | Items/s | Peak RSS (MB) |
|----------------|----------------|------------|---------|---------------|
| MortonEncode/10M | 10,000,000 | 1.52 ms | 6.59 G/s | 657.13 |
| RadixSort/10M | 10,000,000 | 1818 ms | 5.50 M/s | 267.22 |
| TreeBuild/10M | 10,000,000 | 2248 ms | 4.45 M/s | 576.46 |

## Query Scaling: Range Query (1000 aggregated queries)
*Note: Result counts are capped at 1000 per query. Peak RSS reflects the entire query process state.*

| Particle Count | Radius (R) | Time (CPU) | Total Particles Found (sum of 1000) | Peak RSS (MB) |
|----------------|------------|------------|-----------------------|---------------|
| 10,000,000 | 0.01 | 130 us | 1,001 | 869.13 |
| 10,000,000 | 0.10 | 136 us | 1,793 | 869.13 |
| 10,000,000 | 1.00 | 892 us | 116,137 | 869.13 |
| 10,000,000 | 10.00 | 20.8 ms | 952,080 | 869.13 |
| 10,000,000 | 100.00 | 699 ms | 976,025 (capped) | 869.13 |
| 10,000,000 | 200.00 | 1667 ms | 976,029 (capped) | 869.13 |

## Query Scaling: kNN Query (1000 aggregated queries)

| Particle Count | k | Time (CPU) | Peak RSS (MB) |
|----------------|---|------------|---------------|
| 10,000,000 | 1 | 110 us | 839.59 |
| 10,000,000 | 128 | 2757 us | 848.35 |

## Observations
- **Memory Footprint:** The HLBVH tree and Morton arrays for 10M particles fit well within ~1 GB of RAM. The Peak RSS remains stable during query execution, as memory is pre-allocated.
- **Construction Bottleneck:** Radix sorting (CPU-based `std::sort` for now) is the primary time-consuming stage, taking ~1.8 seconds for 10M keys.
- **Query Efficiency:** Traversal is highly optimized; even with dense particle clusters, 1,000 range queries at $R=1$ kpc complete in under **1 millisecond**.
- **Data Locality:** By sampling query centers from particles, we confirm the tree correctly handles sub-pc resolution distributions with high local occupancy.
