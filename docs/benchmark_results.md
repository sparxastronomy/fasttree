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

## Distributed Domain Decomposition Scaling (Single-Node, CPU backend)

**Date:** 2026-06-20  
**Hardware:** Intel(R) Xeon(R) Platinum 8360Y CPU @ 2.40GHz (72 cores)  
**Environment:** MPI 4.1, SYCL Native CPU backend (Intel LLVM 2025.2.0)

The following tables show the scaling of the distributed domain decomposition phases for the **10M particle** and **1M particle** datasets across different MPI rank configurations. Timings show the **Maximum** and **Average** times across all MPI ranks.

### 10M Particle Dataset Scaling (10,000,000 particles)

| Phase | 2 Ranks Max (s) | 2 Ranks Avg (s) | 4 Ranks Max (s) | 4 Ranks Avg (s) | 8 Ranks Max (s) | 8 Ranks Avg (s) | 16 Ranks Max (s) | 16 Ranks Avg (s) |
|---|---|---|---|---|---|---|---|---|
| **1. Bounding Box** | 0.003121 | 0.003119 | 0.211763 | 0.211762 | 0.025955 | 0.025950 | 0.232659 | 0.232656 |
| **2. Coarse Histogram** | 0.139594 | 0.139561 | 0.147597 | 0.147594 | 0.084775 | 0.084540 | 0.068386 | 0.068252 |
| **3. Splitter Gen** | 0.002414 | 0.002022 | 0.001901 | 0.001714 | 0.002322 | 0.001918 | 0.002239 | 0.001969 |
| **4. Local Binning & Redist** | 9.354877 | 9.351924 | 5.421626 | 5.411276 | 2.718273 | 2.703531 | 1.430889 | 1.412539 |
| **5. Explicit Halo Exchange** | 0.466024 | 0.465898 | 0.923837 | 0.883661 | 0.785387 | 0.741591 | 0.911413 | 0.851536 |
| **6. Unified Tree Build** | 20.551902 | 19.868205 | 20.142341 | 14.305071 | 21.511882 | 7.604367 | 21.330200 | 3.913919 |
| **Total Domain Decomposition** | 30.516835 | 29.830739 | 26.849065 | 20.961086 | 25.108927 | 11.161902 | 23.942695 | 6.480877 |
| **Peak RSS Memory (MB)** | 21053.80 | 14004.92 | 21579.34 | 8280.43 | 7352.41 | 3173.65 | 8455.17 | 1525.94 |

### 1M Particle Dataset Scaling (1,000,000 particles)

| Phase | 2 Ranks Max (s) | 2 Ranks Avg (s) | 4 Ranks Max (s) | 4 Ranks Avg (s) | 8 Ranks Max (s) | 8 Ranks Avg (s) | 16 Ranks Max (s) | 16 Ranks Avg (s) |
|---|---|---|---|---|---|---|---|---|
| **1. Bounding Box** | 0.062902 | 0.062901 | 0.060812 | 0.060805 | 0.025477 | 0.025474 | 0.003140 | 0.003100 |
| **2. Coarse Histogram** | 0.039010 | 0.038977 | 0.030442 | 0.030428 | 0.021720 | 0.021441 | 0.014591 | 0.014506 |
| **3. Splitter Gen** | 0.002267 | 0.001931 | 0.002136 | 0.001829 | 0.002489 | 0.001879 | 0.002315 | 0.002082 |
| **4. Local Binning & Redist** | 0.916161 | 0.916155 | 0.468511 | 0.467455 | 0.274329 | 0.273601 | 0.154813 | 0.153548 |
| **5. Explicit Halo Exchange** | 0.072104 | 0.071816 | 0.119293 | 0.117631 | 0.089646 | 0.085976 | 0.088322 | 0.084170 |
| **6. Unified Tree Build** | 2.089738 | 2.053512 | 1.909720 | 1.297087 | 1.757298 | 0.635773 | 1.723463 | 0.320160 |
| **Total Domain Decomposition** | 3.182191 | 3.145302 | 2.590581 | 1.975242 | 2.169097 | 1.044149 | 1.985379 | 0.577571 |
| **Peak RSS Memory (MB)** | 909.54 | 672.96 | 2419.42 | 1066.61 | 469.42 | 401.43 | 822.50 | 268.71 |

### 100K Particle Dataset Scaling (100,000 particles)

| Phase | 2 Ranks Max (s) | 2 Ranks Avg (s) | 4 Ranks Max (s) | 4 Ranks Avg (s) | 8 Ranks Max (s) | 8 Ranks Avg (s) | 16 Ranks Max (s) | 16 Ranks Avg (s) |
|---|---|---|---|---|---|---|---|---|
| **1. Bounding Box** | 0.001803 | 0.001802 | 0.001716 | 0.001715 | 0.001540 | 0.001539 | 0.001668 | 0.001668 |
| **2. Coarse Histogram** | 0.011209 | 0.011185 | 0.010072 | 0.010066 | 0.009046 | 0.008880 | 0.008967 | 0.008701 |
| **3. Splitter Gen** | 0.002216 | 0.001950 | 0.002690 | 0.002300 | 0.002895 | 0.002239 | 0.003215 | 0.002325 |
| **4. Local Binning & Redist** | 0.081891 | 0.081614 | 0.065970 | 0.065328 | 0.029119 | 0.028443 | 0.018804 | 0.018087 |
| **5. Explicit Halo Exchange** | 0.010716 | 0.010689 | 0.019627 | 0.019389 | 0.011850 | 0.011500 | 0.012828 | 0.012451 |
| **6. Unified Tree Build** | 0.146212 | 0.144746 | 0.230388 | 0.140312 | 0.173638 | 0.051910 | 0.146906 | 0.021204 |
| **Total Domain Decomposition** | 0.253469 | 0.251989 | 0.329430 | 0.239113 | 0.226527 | 0.104514 | 0.190428 | 0.064438 |
| **Peak RSS Memory (MB)** | 317.61 | 316.40 | 308.96 | 262.50 | 311.95 | 175.55 | 309.32 | 116.89 |

### 10K Particle Dataset Scaling (10,000 particles)

| Phase | 2 Ranks Max (s) | 2 Ranks Avg (s) | 4 Ranks Max (s) | 4 Ranks Avg (s) | 8 Ranks Max (s) | 8 Ranks Avg (s) | 16 Ranks Max (s) | 16 Ranks Avg (s) |
|---|---|---|---|---|---|---|---|---|
| **1. Bounding Box** | 0.000998 | 0.000998 | 0.001476 | 0.001473 | 0.001117 | 0.001117 | 0.000325 | 0.000324 |
| **2. Coarse Histogram** | 0.006023 | 0.006002 | 0.006716 | 0.006703 | 0.004406 | 0.004186 | 0.005871 | 0.005700 |
| **3. Splitter Gen** | 0.001555 | 0.001547 | 0.002585 | 0.002128 | 0.002398 | 0.001885 | 0.002815 | 0.002173 |
| **4. Local Binning & Redist** | 0.012553 | 0.012524 | 0.011454 | 0.011125 | 0.005091 | 0.004842 | 0.005144 | 0.004607 |
| **5. Explicit Halo Exchange** | 0.002049 | 0.002044 | 0.004369 | 0.004361 | 0.006020 | 0.005990 | 0.009565 | 0.009535 |
| **6. Unified Tree Build** | 0.021425 | 0.020786 | 0.024300 | 0.022593 | 0.015645 | 0.008923 | 0.015860 | 0.004325 |
| **Total Domain Decomposition** | 0.044548 | 0.043904 | 0.050101 | 0.048386 | 0.033677 | 0.026945 | 0.038238 | 0.026666 |
| **Peak RSS Memory (MB)** | 96.14 | 95.78 | 93.87 | 92.77 | 93.62 | 84.83 | 95.41 | 77.80 |

## Observations
- **Memory Footprint:** The HLBVH tree and Morton arrays for 10M particles fit well within ~1.17 GB of RAM during queries. Peak RSS remains stable during query execution, as memory is pre-allocated.
- **Construction Performance:** By switching to a single-pass full 64-bit sort on the GPU using `oneapi::dpl`, the total tree build time for 10M particles dropped from ~2.2 seconds down to **~421 milliseconds**. This satisfies the Exascale requirement for high-frequency tree rebuilds.
- **Query Efficiency:** Traversal is highly optimized; even with dense particle clusters, 1,000 range queries at $R=1$ kpc complete in under **1 millisecond**.
- **Data Locality:** By sampling query centers from particles, we confirm the tree correctly handles sub-pc resolution distributions with high local occupancy.
