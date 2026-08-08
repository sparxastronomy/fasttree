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
2026-08-08T18:58:18+02:00
Running ./self_knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.12, 0.66, 0.37
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
***WARNING*** Library was built as DEBUG. Timings may be affected.
----------------------------------------------------------------------------------
Benchmark                        Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------
SelfKNNQuery/1K/k=1            306 us          305 us         1886 PeakRSS_MB=248.465 items_per_second=3.27534M/s
SelfKNNQuery/1K/k=2            360 us          360 us         1925 PeakRSS_MB=249.027 items_per_second=2.77734M/s
SelfKNNQuery/1K/k=4            463 us          462 us         1696 PeakRSS_MB=249.027 items_per_second=2.16446M/s
SelfKNNQuery/1K/k=8            504 us          504 us         1307 PeakRSS_MB=248.465 items_per_second=1.98594M/s
SelfKNNQuery/1K/k=16           638 us          638 us         1083 PeakRSS_MB=248.465 items_per_second=1.56782M/s
SelfKNNQuery/1K/k=32           885 us          885 us          787 PeakRSS_MB=248.465 items_per_second=1.13037M/s
SelfKNNQuery/1K/k=64          2396 us         2395 us          292 PeakRSS_MB=249.027 items_per_second=417.51k/s
SelfKNNQuery/1K/k=128         2436 us         2435 us          287 PeakRSS_MB=249.027 items_per_second=410.628k/s
SelfKNNQuery/10K/k=1           361 us          360 us         1929 PeakRSS_MB=256.34 items_per_second=27.7419M/s
SelfKNNQuery/10K/k=2           547 us          547 us         1274 PeakRSS_MB=256.34 items_per_second=18.2947M/s
SelfKNNQuery/10K/k=4           663 us          662 us         1052 PeakRSS_MB=256.34 items_per_second=15.1038M/s
SelfKNNQuery/10K/k=8           825 us          825 us          847 PeakRSS_MB=256.34 items_per_second=12.1213M/s
SelfKNNQuery/10K/k=16         1213 us         1212 us          587 PeakRSS_MB=256.34 items_per_second=8.25309M/s
SelfKNNQuery/10K/k=32         1892 us         1888 us          368 PeakRSS_MB=256.34 items_per_second=5.29698M/s
SelfKNNQuery/10K/k=64        10431 us        10426 us           67 PeakRSS_MB=255.777 items_per_second=959.15k/s
SelfKNNQuery/10K/k=128       12003 us        11999 us           58 PeakRSS_MB=256.34 items_per_second=833.386k/s
SelfKNNQuery/100K/k=1         1119 us         1118 us          621 PeakRSS_MB=260.918 items_per_second=89.46M/s
SelfKNNQuery/100K/k=2         1684 us         1683 us          415 PeakRSS_MB=260.891 items_per_second=59.4118M/s
SelfKNNQuery/100K/k=4         2197 us         2196 us          316 PeakRSS_MB=260.918 items_per_second=45.5458M/s
SelfKNNQuery/100K/k=8         3169 us         3167 us          221 PeakRSS_MB=260.891 items_per_second=31.5748M/s
SelfKNNQuery/100K/k=16        5214 us         5211 us          133 PeakRSS_MB=260.891 items_per_second=19.1894M/s
SelfKNNQuery/100K/k=32       10061 us        10056 us           70 PeakRSS_MB=260.891 items_per_second=9.94405M/s
SelfKNNQuery/100K/k=64       78273 us        78257 us            9 PeakRSS_MB=260.355 items_per_second=1.27784M/s
SelfKNNQuery/100K/k=128      98084 us        97899 us            7 PeakRSS_MB=260.918 items_per_second=1.02146M/s
SelfKNNQuery/1M/k=1          17290 us        17283 us           40 PeakRSS_MB=315.227 items_per_second=57.8615M/s
SelfKNNQuery/1M/k=2          36062 us        36053 us           19 PeakRSS_MB=315.23 items_per_second=27.7371M/s
SelfKNNQuery/1M/k=4          59783 us        59770 us           12 PeakRSS_MB=315.23 items_per_second=16.7309M/s
SelfKNNQuery/1M/k=8          98963 us        98942 us            7 PeakRSS_MB=315.23 items_per_second=10.1069M/s
SelfKNNQuery/1M/k=16        170128 us       170096 us            4 PeakRSS_MB=315.23 items_per_second=5.87905M/s
SelfKNNQuery/1M/k=32        305634 us       305576 us            2 PeakRSS_MB=315.23 items_per_second=3.2725M/s
SelfKNNQuery/1M/k=64        975126 us       974951 us            1 PeakRSS_MB=315.23 items_per_second=1.02569M/s
SelfKNNQuery/1M/k=128      1201054 us      1200849 us            1 PeakRSS_MB=315.23 items_per_second=832.744k/s
SelfKNNQuery/10M/k=1        300490 us       300433 us            2 PeakRSS_MB=788.227 items_per_second=33.2853M/s
SelfKNNQuery/10M/k=2        593187 us       593079 us            1 PeakRSS_MB=788.258 items_per_second=16.8612M/s
SelfKNNQuery/10M/k=4        903082 us       902930 us            1 PeakRSS_MB=788.273 items_per_second=11.0751M/s
SelfKNNQuery/10M/k=8       1403128 us      1401736 us            1 PeakRSS_MB=788.316 items_per_second=7.13401M/s
SelfKNNQuery/10M/k=16      2275237 us      2274789 us            1 PeakRSS_MB=788.328 items_per_second=4.39601M/s
SelfKNNQuery/10M/k=32      3869908 us      3869343 us            1 PeakRSS_MB=788.34 items_per_second=2.58442M/s
SelfKNNQuery/10M/k=64     10517456 us     10515053 us            1 PeakRSS_MB=788.344 items_per_second=951.018k/s
SelfKNNQuery/10M/k=128    12856153 us     12853179 us            1 PeakRSS_MB=788.344 items_per_second=778.018k/s

```
