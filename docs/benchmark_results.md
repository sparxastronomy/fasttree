# Benchmark Results (Google Benchmark Suite)

**Date:** 2026-05-12
**Hardware:** Intel(R) Xeon(R) Platinum 8360Y CPU @ 2.40GHz (72 cores)
**Environment:** SYCL (Intel LLVM 2025.2.0), oneDPL 2022.9, Google Benchmark v1.9.1, MPI 4.1, HDF5 2.0.0

The following results include memory usage (Peak RSS) and query occupancy metrics. Query points are sampled from actual particle positions.

## Construction Benchmarks

| Benchmark Name | Particle Count | Time (CPU) | Items/s | Peak RSS (MB) |
|----------------|----------------|------------|---------|---------------|
| MortonEncode/10M | 10,000,000 | 1.33 ms | 7.54 G/s | 634.55 |
| GPU_Sort/10M | 10,000,000 | 421.0 ms | 29.7 M/s | 567.52 |

*Note: The GPU_Sort benchmark includes Morton encoding, full zipped sorting of keys and indices using oneDPL, coordinate reordering, and tree construction.*

## Query Scaling: Range Query (1000 aggregated queries)
*Note: Result counts are capped at 1000 per query. Peak RSS reflects the entire query process state.*

| Particle Count | Radius (R) | Time (CPU) | Total Particles Found (sum of 1000) | Peak RSS (MB) |
|----------------|------------|------------|-----------------------|---------------|
| 10,000,000 | 0.01 | 141 us | 1,001 | 1,175.47 |
| 10,000,000 | 0.10 | 170 us | 1,793 | 1,159.17 |
| 10,000,000 | 1.00 | 902 us | 116,137 | 1,175.54 |
| 10,000,000 | 10.00 | 17.5 ms | 952,080 | 1,159.53 |
| 10,000,000 | 100.00 | 823 ms | 976,025 (capped) | 1,159.93 |
| 10,000,000 | 200.00 | 1661 ms | 976,029 (capped) | 1,176.28 |

## Query Scaling: kNN Query (1000 aggregated queries)

| Particle Count | k | Time (CPU) | Peak RSS (MB) |
|----------------|---|------------|---------------|
| 10,000,000 | 1 | 138 us | 1,156.56 |
| 10,000,000 | 128 | 2782 us | 1,156.62 |

## Observations
- **Memory Footprint:** The HLBVH tree and Morton arrays for 10M particles fit well within ~1.17 GB of RAM during queries. Peak RSS remains stable during query execution, as memory is pre-allocated.
- **Construction Performance:** By switching to a single-pass full 64-bit sort on the GPU using `oneapi::dpl`, the total tree build time for 10M particles dropped from ~2.2 seconds down to **~421 milliseconds**. This satisfies the Exascale requirement for high-frequency tree rebuilds.
- **Query Efficiency:** Traversal is highly optimized; even with dense particle clusters, 1,000 range queries at $R=1$ kpc complete in under **1 millisecond**.
- **Data Locality:** By sampling query centers from particles, we confirm the tree correctly handles sub-pc resolution distributions with high local occupancy.
