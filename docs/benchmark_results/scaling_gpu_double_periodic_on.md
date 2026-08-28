# Scaling Benchmark Results: gpu_double (Periodic BC: ON)
Date: Sat Aug  8 16:40:35 CEST 2026
Device: GPU
Target GPU: nvidia
SFC Curve: Peano-Hilbert
Periodic BC: ON
Build Directory: build_gpu
```
--- Executing ./sfc_encoding_scaling.exe ---
2026-08-08T16:40:35+02:00
Running ./sfc_encoding_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.15, 1.09, 1.01
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
***WARNING*** Library was built as DEBUG. Timings may be affected.
--------------------------------------------------------------------------
Benchmark                Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------
SFC-Encode/1K        0.021 ms        0.021 ms        32611 PeakRSS_MB=244.953 items_per_second=46.9176M/s
SFC-Encode/10K       0.021 ms        0.021 ms        32784 PeakRSS_MB=246.516 items_per_second=469.065M/s
SFC-Encode/100K      0.024 ms        0.024 ms        28960 PeakRSS_MB=250.996 items_per_second=4.16529G/s
SFC-Encode/1M        0.055 ms        0.055 ms        11291 PeakRSS_MB=301.789 items_per_second=18.1252G/s
SFC-Encode/10M       0.360 ms        0.360 ms         1918 PeakRSS_MB=770.523 items_per_second=27.8026G/s

--- Executing ./gpu_sort_scaling.exe ---
2026-08-08T16:40:59+02:00
Running ./gpu_sort_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.10, 1.08, 1.01
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
***WARNING*** Library was built as DEBUG. Timings may be affected.
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
GPU_Sort/1K        0.046 ms        0.046 ms        15176 PeakRSS_MB=252.84 items_per_second=21.8251M/s
GPU_Sort/10K       0.112 ms        0.112 ms         6241 PeakRSS_MB=260.715 items_per_second=89.2523M/s
GPU_Sort/100K      0.359 ms        0.358 ms         1950 PeakRSS_MB=268.027 items_per_second=279.017M/s
GPU_Sort/1M         2.71 ms         2.71 ms          259 PeakRSS_MB=318.781 items_per_second=369.607M/s
GPU_Sort/10M        36.8 ms         36.8 ms           18 PeakRSS_MB=517.344 items_per_second=271.641M/s

--- Executing ./tree_build_scaling.exe ---
2026-08-08T16:41:17+02:00
Running ./tree_build_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.07, 1.08, 1.01
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
***WARNING*** Library was built as DEBUG. Timings may be affected.
-------------------------------------------------------------------------
Benchmark               Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------
TreeBuild/1K         1.78 ms         1.77 ms          323 PeakRSS_MB=250.008 items_per_second=564.191k/s
TreeBuild/10K        4.07 ms         4.06 ms          211 PeakRSS_MB=257.883 items_per_second=2.4644M/s
TreeBuild/100K       5.72 ms         5.71 ms          120 PeakRSS_MB=262.461 items_per_second=17.5197M/s
TreeBuild/1M         30.3 ms         30.3 ms           22 PeakRSS_MB=315.969 items_per_second=33.014M/s
TreeBuild/10M         296 ms          295 ms            2 PeakRSS_MB=785.668 items_per_second=33.8703M/s

--- Executing ./rebuild_scaling.exe ---
2026-08-08T16:41:31+02:00
Running ./rebuild_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.06, 1.07, 1.00
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
***WARNING*** Library was built as DEBUG. Timings may be affected.
---------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------
TreeRebuild/1K         2.11 ms         2.11 ms          243 PeakRSS_MB=249.594 items_per_second=474.576k/s
TreeRebuild/10K        3.26 ms         3.25 ms          267 PeakRSS_MB=257.469 items_per_second=3.07635M/s
TreeRebuild/100K       6.88 ms         6.86 ms           80 PeakRSS_MB=264.414 items_per_second=14.5764M/s
TreeRebuild/1M         29.5 ms         29.5 ms           24 PeakRSS_MB=346.695 items_per_second=33.9428M/s
TreeRebuild/10M         294 ms          294 ms            2 PeakRSS_MB=1.09566k items_per_second=34.0446M/s

--- Executing ./range_query_scaling.exe ---
2026-08-08T16:41:44+02:00
Running ./range_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.04, 1.07, 1.00
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
***WARNING*** Library was built as DEBUG. Timings may be affected.
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
RangeQuery/1K/R=0.010000           86.5 us         86.5 us         7836 PeakRSS_MB=249.949 TotalFound=1k
RangeQuery/1K/R=0.100000           85.9 us         85.9 us         8122 PeakRSS_MB=249.949 TotalFound=1.005k
RangeQuery/1K/R=1.000000            111 us          111 us         6272 PeakRSS_MB=249.949 TotalFound=1.035k
RangeQuery/1K/R=10.000000           212 us          212 us         3297 PeakRSS_MB=249.949 TotalFound=6.591k
RangeQuery/1K/R=100.000000         1109 us         1107 us          630 PeakRSS_MB=249.949 TotalFound=459.76k
RangeQuery/1K/R=200.000000         1318 us         1318 us          532 PeakRSS_MB=249.949 TotalFound=931.404k
RangeQuery/10K/R=0.010000           111 us          111 us         6279 PeakRSS_MB=257.199 TotalFound=1k
RangeQuery/10K/R=0.100000           131 us          131 us         5331 PeakRSS_MB=257.762 TotalFound=1.001k
RangeQuery/10K/R=1.000000           175 us          175 us         3990 PeakRSS_MB=257.699 TotalFound=1.295k
RangeQuery/10K/R=10.000000         1153 us         1153 us          606 PeakRSS_MB=257.637 TotalFound=53.444k
RangeQuery/10K/R=100.000000       17799 us        17795 us           39 PeakRSS_MB=257.637 TotalFound=4.38585M
RangeQuery/10K/R=200.000000       15992 us        15989 us           44 PeakRSS_MB=257.012 TotalFound=9.12136M
RangeQuery/100K/R=0.010000          148 us          148 us         4704 PeakRSS_MB=262.285 TotalFound=1k
RangeQuery/100K/R=0.100000          194 us          194 us         3617 PeakRSS_MB=262.223 TotalFound=1.005k
RangeQuery/100K/R=1.000000          591 us          591 us         1181 PeakRSS_MB=261.75 TotalFound=4.145k
RangeQuery/100K/R=10.000000       11676 us        11654 us           60 PeakRSS_MB=262.16 TotalFound=506.846k
RangeQuery/100K/R=100.000000     185117 us       185085 us            4 PeakRSS_MB=262.18 TotalFound=43.2318M
RangeQuery/100K/R=200.000000     236634 us       236596 us            3 PeakRSS_MB=262.117 TotalFound=90.3647M
RangeQuery/1M/R=0.010000            161 us          161 us         4333 PeakRSS_MB=316.668 TotalFound=1k
RangeQuery/1M/R=0.100000            251 us          250 us         2778 PeakRSS_MB=316.605 TotalFound=1.139k
RangeQuery/1M/R=1.000000           3313 us         3313 us          212 PeakRSS_MB=317.168 TotalFound=39.044k
RangeQuery/1M/R=10.000000        134259 us       134238 us            5 PeakRSS_MB=317.73 TotalFound=5.78161M
RangeQuery/1M/R=100.000000      2823825 us      2823314 us            1 PeakRSS_MB=317.73 TotalFound=433.516M
RangeQuery/1M/R=200.000000      3239663 us      3239131 us            1 PeakRSS_MB=317.73 TotalFound=907.777M
RangeQuery/10M/R=0.010000           186 us          186 us         3720 PeakRSS_MB=790.754 TotalFound=1.001k
RangeQuery/10M/R=0.100000           656 us          656 us          977 PeakRSS_MB=790.793 TotalFound=1.793k
RangeQuery/10M/R=1.000000         38813 us        38806 us           18 PeakRSS_MB=790.238 TotalFound=363.096k
RangeQuery/10M/R=10.000000      1557998 us      1557725 us            1 PeakRSS_MB=790.824 TotalFound=55.2471M
RangeQuery/10M/R=100.000000    36439782 us     36430238 us            1 PeakRSS_MB=798.707 TotalFound=4.39221G
RangeQuery/10M/R=200.000000    54005291 us     53990986 us            1 PeakRSS_MB=798.145 TotalFound=9.09205G

--- Executing ./knn_query_scaling.exe ---
2026-08-08T16:46:24+02:00
Running ./knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.04, 1.06, 1.00
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
***WARNING*** Library was built as DEBUG. Timings may be affected.
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
KNNQuery/1K/k=1           74.8 us         74.7 us         9043 PeakRSS_MB=253.543
KNNQuery/1K/k=2            156 us          156 us         4465 PeakRSS_MB=253.543
KNNQuery/1K/k=4            211 us          211 us         3312 PeakRSS_MB=253.543
KNNQuery/1K/k=8            306 us          305 us         2278 PeakRSS_MB=254.105
KNNQuery/1K/k=16           435 us          435 us         1603 PeakRSS_MB=253.543
KNNQuery/1K/k=32           696 us          695 us         1006 PeakRSS_MB=254.105
KNNQuery/1K/k=64          2096 us         2095 us          333 PeakRSS_MB=253.543
KNNQuery/1K/k=128         2254 us         2254 us          310 PeakRSS_MB=253.543
KNNQuery/10K/k=1           108 us          108 us         6466 PeakRSS_MB=261.418
KNNQuery/10K/k=2           247 us          246 us         2844 PeakRSS_MB=264.73
KNNQuery/10K/k=4           369 us          369 us         1896 PeakRSS_MB=264.668
KNNQuery/10K/k=8           525 us          525 us         1331 PeakRSS_MB=264.668
KNNQuery/10K/k=16          767 us          766 us          909 PeakRSS_MB=264.043
KNNQuery/10K/k=32         1238 us         1238 us          565 PeakRSS_MB=263.98
KNNQuery/10K/k=64         3597 us         3596 us          194 PeakRSS_MB=264.543
KNNQuery/10K/k=128        3972 us         3971 us          176 PeakRSS_MB=264.48
KNNQuery/100K/k=1          140 us          140 us         4983 PeakRSS_MB=268.277
KNNQuery/100K/k=2          290 us          289 us         2424 PeakRSS_MB=268.84
KNNQuery/100K/k=4          420 us          420 us         1671 PeakRSS_MB=268.84
KNNQuery/100K/k=8          559 us          555 us         1253 PeakRSS_MB=273.902
KNNQuery/100K/k=16         788 us          787 us          885 PeakRSS_MB=273.34
KNNQuery/100K/k=32        1259 us         1258 us          555 PeakRSS_MB=273.902
KNNQuery/100K/k=64        4286 us         4285 us          163 PeakRSS_MB=273.902
KNNQuery/100K/k=128       4328 us         4327 us          162 PeakRSS_MB=273.34
KNNQuery/1M/k=1            156 us          156 us         4462 PeakRSS_MB=328.32
KNNQuery/1M/k=2            352 us          352 us         1992 PeakRSS_MB=333.82
KNNQuery/1M/k=4            465 us          465 us         1495 PeakRSS_MB=333.258
KNNQuery/1M/k=8            617 us          617 us         1130 PeakRSS_MB=333.82
KNNQuery/1M/k=16          1059 us         1059 us          658 PeakRSS_MB=333.82
KNNQuery/1M/k=32          1958 us         1958 us          360 PeakRSS_MB=333.258
KNNQuery/1M/k=64          4666 us         4665 us          152 PeakRSS_MB=333.82
KNNQuery/1M/k=128         5721 us         5720 us          123 PeakRSS_MB=333.82
KNNQuery/10M/k=1           163 us          163 us         4268 PeakRSS_MB=803.594
KNNQuery/10M/k=2           366 us          366 us         1906 PeakRSS_MB=803.594
KNNQuery/10M/k=4           524 us          524 us         1269 PeakRSS_MB=803.594
KNNQuery/10M/k=8           886 us          886 us          777 PeakRSS_MB=803.594
KNNQuery/10M/k=16         1421 us         1420 us          487 PeakRSS_MB=804.156
KNNQuery/10M/k=32         2251 us         2250 us          308 PeakRSS_MB=803.594
KNNQuery/10M/k=64         4686 us         4685 us          150 PeakRSS_MB=803.594
KNNQuery/10M/k=128        5396 us         5395 us          130 PeakRSS_MB=803.594

--- Executing ./self_knn_query_scaling.exe ---
2026-08-27T08:53:54+02:00
Running ./self_knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 4.75, 3.92, 3.52
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------------------
Benchmark                        Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------
SelfKNNQuery/1K/k=1           56.9 us         56.9 us        10048 PeakRSS_MB=143.711 items_per_second=17.5676M/s
SelfKNNQuery/1K/k=2            269 us          269 us         2617 PeakRSS_MB=143.711 items_per_second=3.71786M/s
SelfKNNQuery/1K/k=4            401 us          401 us         1755 PeakRSS_MB=143.711 items_per_second=2.49456M/s
SelfKNNQuery/1K/k=8            575 us          575 us         1210 PeakRSS_MB=143.711 items_per_second=1.7382M/s
SelfKNNQuery/1K/k=16           953 us          953 us          734 PeakRSS_MB=143.711 items_per_second=1.04961M/s
SelfKNNQuery/1K/k=32          1642 us         1642 us          426 PeakRSS_MB=143.711 items_per_second=609.152k/s
SelfKNNQuery/1K/k=64          3605 us         3605 us          193 PeakRSS_MB=143.711 items_per_second=277.428k/s
SelfKNNQuery/1K/k=128         5975 us         5975 us          117 PeakRSS_MB=143.711 items_per_second=167.373k/s
SelfKNNQuery/10K/k=1          73.0 us         73.0 us         9525 PeakRSS_MB=144.836 items_per_second=137.008M/s
SelfKNNQuery/10K/k=2           534 us          534 us         1302 PeakRSS_MB=144.836 items_per_second=18.718M/s
SelfKNNQuery/10K/k=4           773 us          772 us          905 PeakRSS_MB=144.836 items_per_second=12.9464M/s
SelfKNNQuery/10K/k=8          1439 us         1439 us          486 PeakRSS_MB=144.836 items_per_second=6.94825M/s
SelfKNNQuery/10K/k=16         2575 us         2575 us          271 PeakRSS_MB=144.836 items_per_second=3.88377M/s
SelfKNNQuery/10K/k=32         4810 us         4809 us          146 PeakRSS_MB=144.836 items_per_second=2.07945M/s
SelfKNNQuery/10K/k=64         9803 us         9801 us           71 PeakRSS_MB=145.398 items_per_second=1.02027M/s
SelfKNNQuery/10K/k=128       16323 us        16321 us           43 PeakRSS_MB=144.836 items_per_second=612.697k/s
SelfKNNQuery/100K/k=1          228 us          228 us         3089 PeakRSS_MB=148.898 items_per_second=439.027M/s
SelfKNNQuery/100K/k=2         1370 us         1369 us          510 PeakRSS_MB=148.812 items_per_second=73.0234M/s
SelfKNNQuery/100K/k=4         2490 us         2490 us          284 PeakRSS_MB=148.895 items_per_second=40.1665M/s
SelfKNNQuery/100K/k=8         4134 us         4133 us          169 PeakRSS_MB=149.348 items_per_second=24.1933M/s
SelfKNNQuery/100K/k=16        7862 us         7861 us           89 PeakRSS_MB=149.457 items_per_second=12.7211M/s
SelfKNNQuery/100K/k=32       16843 us        16841 us           41 PeakRSS_MB=148.895 items_per_second=5.93802M/s
SelfKNNQuery/100K/k=64       34622 us        34618 us           20 PeakRSS_MB=148.895 items_per_second=2.88866M/s
SelfKNNQuery/100K/k=128      95871 us        95859 us            7 PeakRSS_MB=148.895 items_per_second=1.0432M/s
SelfKNNQuery/1M/k=1           1676 us         1676 us          417 PeakRSS_MB=208.875 items_per_second=596.706M/s
SelfKNNQuery/1M/k=2          12970 us        12969 us           54 PeakRSS_MB=209.438 items_per_second=77.1091M/s
SelfKNNQuery/1M/k=4          23017 us        23014 us           30 PeakRSS_MB=210 items_per_second=43.4518M/s
SelfKNNQuery/1M/k=8          40540 us        40535 us           17 PeakRSS_MB=209.438 items_per_second=24.6703M/s
SelfKNNQuery/1M/k=16         76585 us        76576 us            9 PeakRSS_MB=210 items_per_second=13.059M/s
SelfKNNQuery/1M/k=32        158142 us       158122 us            4 PeakRSS_MB=209.438 items_per_second=6.32422M/s
SelfKNNQuery/1M/k=64        280539 us       280507 us            2 PeakRSS_MB=210 items_per_second=3.56498M/s
SelfKNNQuery/1M/k=128       860028 us       859929 us            1 PeakRSS_MB=210 items_per_second=1.16289M/s
SelfKNNQuery/10M/k=1         16433 us        16431 us           43 PeakRSS_MB=677.789 items_per_second=608.619M/s
SelfKNNQuery/10M/k=2        137606 us       137590 us            5 PeakRSS_MB=683.422 items_per_second=72.6798M/s
SelfKNNQuery/10M/k=4        238400 us       238367 us            3 PeakRSS_MB=683.434 items_per_second=41.952M/s
SelfKNNQuery/10M/k=8        413183 us       413131 us            2 PeakRSS_MB=683.434 items_per_second=24.2054M/s
SelfKNNQuery/10M/k=16       770747 us       770654 us            1 PeakRSS_MB=683.434 items_per_second=12.976M/s
SelfKNNQuery/10M/k=32      1583668 us      1583474 us            1 PeakRSS_MB=683.434 items_per_second=6.31523M/s
SelfKNNQuery/10M/k=64      2860895 us      2860563 us            1 PeakRSS_MB=683.434 items_per_second=3.49582M/s
SelfKNNQuery/10M/k=128     8941075 us      8940044 us            1 PeakRSS_MB=683.996 items_per_second=1.11856M/s

```
