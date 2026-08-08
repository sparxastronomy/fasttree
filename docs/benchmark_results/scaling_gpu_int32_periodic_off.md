# Scaling Benchmark Results: gpu_int32 (Periodic BC: OFF)
Date: Sat Aug  8 20:42:20 CEST 2026
Device: GPU
Target GPU: nvidia
SFC Curve: Peano-Hilbert
Periodic BC: OFF
Build Directory: build_int32
```
--- Executing ./sfc_encoding_scaling.exe ---
2026-08-08T20:42:20+02:00
Running ./sfc_encoding_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 2.00, 1.83, 1.42
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------
Benchmark                Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------
SFC-Encode/1K        0.021 ms        0.021 ms        32525 PeakRSS_MB=142.852 items_per_second=46.6552M/s
SFC-Encode/10K       0.021 ms        0.021 ms        32793 PeakRSS_MB=144.477 items_per_second=468.211M/s
SFC-Encode/100K      0.024 ms        0.024 ms        29374 PeakRSS_MB=151.477 items_per_second=4.20348G/s
SFC-Encode/1M        0.051 ms        0.051 ms        11860 PeakRSS_MB=191.852 items_per_second=19.4559G/s
SFC-Encode/10M       0.343 ms        0.343 ms         1992 PeakRSS_MB=555.016 items_per_second=29.1392G/s

--- Executing ./gpu_sort_scaling.exe ---
2026-08-08T20:42:46+02:00
Running ./gpu_sort_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 2.08, 1.86, 1.45
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
GPU_Sort/1K        0.046 ms        0.046 ms        15001 PeakRSS_MB=145.945 items_per_second=21.5771M/s
GPU_Sort/10K       0.111 ms        0.111 ms         6300 PeakRSS_MB=146.508 items_per_second=89.8964M/s
GPU_Sort/100K      0.354 ms        0.354 ms         1979 PeakRSS_MB=153.82 items_per_second=282.663M/s
GPU_Sort/1M         2.71 ms         2.71 ms          258 PeakRSS_MB=191.156 items_per_second=369.602M/s
GPU_Sort/10M        37.4 ms         37.4 ms           18 PeakRSS_MB=290.883 items_per_second=267.279M/s

--- Executing ./tree_build_scaling.exe ---
2026-08-08T20:43:04+02:00
Running ./tree_build_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 2.06, 1.87, 1.46
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------------
Benchmark               Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------
TreeBuild/1K         1.33 ms         1.33 ms          383 PeakRSS_MB=145.457 items_per_second=754.45k/s
TreeBuild/10K        1.92 ms         1.91 ms          368 PeakRSS_MB=147.707 items_per_second=5.22275M/s
TreeBuild/100K       4.12 ms         4.11 ms          170 PeakRSS_MB=150.352 items_per_second=24.346M/s
TreeBuild/1M         22.3 ms         22.2 ms           31 PeakRSS_MB=197.348 items_per_second=45.0075M/s
TreeBuild/10M         205 ms          205 ms            3 PeakRSS_MB=566.695 items_per_second=48.8989M/s

--- Executing ./rebuild_scaling.exe ---
2026-08-08T20:43:18+02:00
Running ./rebuild_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 2.12, 1.89, 1.47
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------
TreeRebuild/1K         1.59 ms         1.58 ms          378 PeakRSS_MB=143.438 items_per_second=631.241k/s
TreeRebuild/10K        1.91 ms         1.91 ms          358 PeakRSS_MB=144.562 items_per_second=5.24696M/s
TreeRebuild/100K       4.10 ms         4.09 ms          173 PeakRSS_MB=151.504 items_per_second=24.4377M/s
TreeRebuild/1M         22.0 ms         22.0 ms           32 PeakRSS_MB=218.824 items_per_second=45.5004M/s
TreeRebuild/10M         222 ms          222 ms            3 PeakRSS_MB=868.633 items_per_second=45.0114M/s

--- Executing ./range_query_scaling.exe ---
2026-08-08T20:43:32+02:00
Running ./range_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 2.09, 1.90, 1.48
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
RangeQuery/1K/R=0.010000           53.6 us         53.6 us        12857 PeakRSS_MB=145.027 TotalFound=1k
RangeQuery/1K/R=0.100000           53.7 us         53.7 us        12986 PeakRSS_MB=145.027 TotalFound=1.005k
RangeQuery/1K/R=1.000000           59.8 us         59.8 us        11639 PeakRSS_MB=145.027 TotalFound=1.035k
RangeQuery/1K/R=10.000000           118 us          118 us         5927 PeakRSS_MB=145.027 TotalFound=6.587k
RangeQuery/1K/R=100.000000          585 us          585 us         1191 PeakRSS_MB=145.027 TotalFound=459.676k
RangeQuery/1K/R=200.000000          656 us          656 us         1063 PeakRSS_MB=145.027 TotalFound=931.389k
RangeQuery/10K/R=0.010000          63.9 us         63.5 us        10914 PeakRSS_MB=147.277 TotalFound=1k
RangeQuery/10K/R=0.100000          72.0 us         72.0 us         9693 PeakRSS_MB=147.84 TotalFound=1k
RangeQuery/10K/R=1.000000          95.8 us         95.8 us         7296 PeakRSS_MB=147.277 TotalFound=1.289k
RangeQuery/10K/R=10.000000          548 us          548 us         1267 PeakRSS_MB=147.84 TotalFound=53.336k
RangeQuery/10K/R=100.000000        6781 us         6780 us          103 PeakRSS_MB=147.84 TotalFound=4.38525M
RangeQuery/10K/R=200.000000        6994 us         6993 us          100 PeakRSS_MB=147.84 TotalFound=9.12135M
RangeQuery/100K/R=0.010000         76.4 us         76.4 us         9104 PeakRSS_MB=150.434 TotalFound=1k
RangeQuery/100K/R=0.100000         85.6 us         85.6 us         8158 PeakRSS_MB=151.008 TotalFound=1.003k
RangeQuery/100K/R=1.000000          246 us          245 us         2847 PeakRSS_MB=151.031 TotalFound=4.03k
RangeQuery/100K/R=10.000000        4532 us         4532 us          154 PeakRSS_MB=151.02 TotalFound=506.23k
RangeQuery/100K/R=100.000000      76009 us        75866 us            9 PeakRSS_MB=155.227 TotalFound=43.2297M
RangeQuery/100K/R=200.000000      93836 us        93821 us            7 PeakRSS_MB=154.664 TotalFound=90.3636M
RangeQuery/1M/R=0.010000           86.6 us         86.6 us         8003 PeakRSS_MB=199.387 TotalFound=1k
RangeQuery/1M/R=0.100000            122 us          122 us         5926 PeakRSS_MB=199.949 TotalFound=1.079k
RangeQuery/1M/R=1.000000           1366 us         1366 us          511 PeakRSS_MB=199.387 TotalFound=37.793k
RangeQuery/1M/R=10.000000         51743 us        51734 us           14 PeakRSS_MB=201.637 TotalFound=5.78006M
RangeQuery/1M/R=100.000000       895563 us       895429 us            1 PeakRSS_MB=201.074 TotalFound=433.482M
RangeQuery/1M/R=200.000000      1098625 us      1098476 us            1 PeakRSS_MB=201.074 TotalFound=907.76M
RangeQuery/10M/R=0.010000          91.5 us         91.5 us         7365 PeakRSS_MB=571.898 TotalFound=1.001k
RangeQuery/10M/R=0.100000           201 us          201 us         3289 PeakRSS_MB=572.473 TotalFound=1.457k
RangeQuery/10M/R=1.000000         13686 us        13684 us           51 PeakRSS_MB=571.914 TotalFound=349.304k
RangeQuery/10M/R=10.000000       518846 us       518770 us            1 PeakRSS_MB=571.918 TotalFound=55.2257M
RangeQuery/10M/R=100.000000    12259438 us     12257803 us            1 PeakRSS_MB=571.918 TotalFound=4.39188G
RangeQuery/10M/R=200.000000    17711983 us     17709492 us            1 PeakRSS_MB=571.922 TotalFound=9.0919G

--- Executing ./knn_query_scaling.exe ---
2026-08-08T20:46:06+02:00
Running ./knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 2.20, 2.00, 1.59
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
KNNQuery/1K/k=1           47.9 us         47.9 us        14467 PeakRSS_MB=143.684
KNNQuery/1K/k=2           88.9 us         88.9 us         7829 PeakRSS_MB=143.684
KNNQuery/1K/k=4            119 us          119 us         5828 PeakRSS_MB=143.684
KNNQuery/1K/k=8            173 us          172 us         4042 PeakRSS_MB=143.684
KNNQuery/1K/k=16           269 us          269 us         2595 PeakRSS_MB=143.684
KNNQuery/1K/k=32           444 us          443 us         1578 PeakRSS_MB=143.684
KNNQuery/1K/k=64          2177 us         2176 us          321 PeakRSS_MB=143.684
KNNQuery/1K/k=128         2323 us         2322 us          301 PeakRSS_MB=143.684
KNNQuery/10K/k=1          63.4 us         63.4 us        10969 PeakRSS_MB=144.809
KNNQuery/10K/k=2           134 us          134 us         5241 PeakRSS_MB=144.246
KNNQuery/10K/k=4           198 us          198 us         3533 PeakRSS_MB=144.246
KNNQuery/10K/k=8           273 us          273 us         2571 PeakRSS_MB=144.809
KNNQuery/10K/k=16          416 us          416 us         1679 PeakRSS_MB=144.246
KNNQuery/10K/k=32          691 us          690 us         1009 PeakRSS_MB=144.809
KNNQuery/10K/k=64         3466 us         3466 us          202 PeakRSS_MB=144.246
KNNQuery/10K/k=128        3511 us         3510 us          199 PeakRSS_MB=144.246
KNNQuery/100K/k=1         82.2 us         82.2 us         8449 PeakRSS_MB=147.719
KNNQuery/100K/k=2          169 us          169 us         4127 PeakRSS_MB=148.301
KNNQuery/100K/k=4          207 us          207 us         3380 PeakRSS_MB=147.75
KNNQuery/100K/k=8          294 us          294 us         2372 PeakRSS_MB=147.723
KNNQuery/100K/k=16         435 us          435 us         1610 PeakRSS_MB=148.555
KNNQuery/100K/k=32         728 us          728 us          959 PeakRSS_MB=147.695
KNNQuery/100K/k=64        4063 us         4062 us          172 PeakRSS_MB=147.738
KNNQuery/100K/k=128       4091 us         4091 us          171 PeakRSS_MB=148.301
KNNQuery/1M/k=1           91.9 us         91.9 us         7579 PeakRSS_MB=195.484
KNNQuery/1M/k=2            187 us          187 us         3715 PeakRSS_MB=195.488
KNNQuery/1M/k=4            229 us          229 us         3054 PeakRSS_MB=195.488
KNNQuery/1M/k=8            314 us          314 us         2238 PeakRSS_MB=194.926
KNNQuery/1M/k=16           492 us          492 us         1405 PeakRSS_MB=195.488
KNNQuery/1M/k=32           890 us          889 us          781 PeakRSS_MB=195.488
KNNQuery/1M/k=64          4055 us         4054 us          173 PeakRSS_MB=195.488
KNNQuery/1M/k=128         4499 us         4499 us          156 PeakRSS_MB=195.488
KNNQuery/10M/k=1          98.2 us         98.2 us         6613 PeakRSS_MB=564.477
KNNQuery/10M/k=2           198 us          198 us         3532 PeakRSS_MB=563.926
KNNQuery/10M/k=4           271 us          270 us         2484 PeakRSS_MB=564.496
KNNQuery/10M/k=8           398 us          398 us         1776 PeakRSS_MB=563.941
KNNQuery/10M/k=16          675 us          675 us          904 PeakRSS_MB=564.504
KNNQuery/10M/k=32         1185 us         1185 us          572 PeakRSS_MB=564.504
KNNQuery/10M/k=64         4425 us         4424 us          159 PeakRSS_MB=564.504
KNNQuery/10M/k=128        5088 us         5087 us          118 PeakRSS_MB=565.066

--- Executing ./self_knn_query_scaling.exe ---
2026-08-08T20:49:14+02:00
Running ./self_knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 2.02, 2.01, 1.67
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------------------
Benchmark                        Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------
SelfKNNQuery/1K/k=1            241 us          240 us         2858 PeakRSS_MB=144.23 items_per_second=4.16034M/s
SelfKNNQuery/1K/k=2            281 us          281 us         2488 PeakRSS_MB=144.23 items_per_second=3.5553M/s
SelfKNNQuery/1K/k=4            307 us          306 us         2270 PeakRSS_MB=144.23 items_per_second=3.26391M/s
SelfKNNQuery/1K/k=8            364 us          364 us         1926 PeakRSS_MB=144.23 items_per_second=2.74791M/s
SelfKNNQuery/1K/k=16           456 us          456 us         1529 PeakRSS_MB=144.23 items_per_second=2.1927M/s
SelfKNNQuery/1K/k=32           641 us          640 us         1083 PeakRSS_MB=144.23 items_per_second=1.56129M/s
SelfKNNQuery/1K/k=64          2595 us         2593 us          269 PeakRSS_MB=144.23 items_per_second=385.586k/s
SelfKNNQuery/1K/k=128         2606 us         2604 us          268 PeakRSS_MB=144.23 items_per_second=383.966k/s
SelfKNNQuery/10K/k=1           284 us          284 us         2471 PeakRSS_MB=144.793 items_per_second=35.2319M/s
SelfKNNQuery/10K/k=2           380 us          379 us         1806 PeakRSS_MB=144.793 items_per_second=26.3601M/s
SelfKNNQuery/10K/k=4           430 us          430 us         1624 PeakRSS_MB=144.793 items_per_second=23.2463M/s
SelfKNNQuery/10K/k=8           538 us          538 us         1278 PeakRSS_MB=144.793 items_per_second=18.5814M/s
SelfKNNQuery/10K/k=16          789 us          788 us          878 PeakRSS_MB=144.793 items_per_second=12.6853M/s
SelfKNNQuery/10K/k=32         1290 us         1290 us          539 PeakRSS_MB=144.793 items_per_second=7.7538M/s
SelfKNNQuery/10K/k=64         8752 us         8749 us           80 PeakRSS_MB=145.355 items_per_second=1.14299M/s
SelfKNNQuery/10K/k=128       10385 us        10381 us           67 PeakRSS_MB=144.793 items_per_second=963.258k/s
SelfKNNQuery/100K/k=1          772 us          772 us          898 PeakRSS_MB=147.977 items_per_second=129.613M/s
SelfKNNQuery/100K/k=2         1124 us         1123 us          625 PeakRSS_MB=148.289 items_per_second=89.038M/s
SelfKNNQuery/100K/k=4         1499 us         1498 us          464 PeakRSS_MB=147.992 items_per_second=66.7336M/s
SelfKNNQuery/100K/k=8         2208 us         2206 us          316 PeakRSS_MB=148.047 items_per_second=45.3207M/s
SelfKNNQuery/100K/k=16        3806 us         3804 us          184 PeakRSS_MB=148.328 items_per_second=26.2897M/s
SelfKNNQuery/100K/k=32        7857 us         7854 us           89 PeakRSS_MB=148.555 items_per_second=12.7321M/s
SelfKNNQuery/100K/k=64       67647 us        67633 us           10 PeakRSS_MB=147.992 items_per_second=1.47858M/s
SelfKNNQuery/100K/k=128      85690 us        85675 us            8 PeakRSS_MB=147.992 items_per_second=1.1672M/s
SelfKNNQuery/1M/k=1          10106 us        10102 us           69 PeakRSS_MB=192.43 items_per_second=98.9952M/s
SelfKNNQuery/1M/k=2          23988 us        23982 us           29 PeakRSS_MB=192.992 items_per_second=41.6982M/s
SelfKNNQuery/1M/k=4          41011 us        41002 us           17 PeakRSS_MB=192.43 items_per_second=24.3892M/s
SelfKNNQuery/1M/k=8          69652 us        69639 us           10 PeakRSS_MB=192.43 items_per_second=14.3598M/s
SelfKNNQuery/1M/k=16        123590 us       123568 us            6 PeakRSS_MB=192.43 items_per_second=8.0927M/s
SelfKNNQuery/1M/k=32        233151 us       233116 us            3 PeakRSS_MB=192.992 items_per_second=4.2897M/s
SelfKNNQuery/1M/k=64        800560 us       800450 us            1 PeakRSS_MB=192.992 items_per_second=1.2493M/s
SelfKNNQuery/1M/k=128       994261 us       994124 us            1 PeakRSS_MB=192.992 items_per_second=1.00591M/s
SelfKNNQuery/10M/k=1        215365 us       215319 us            3 PeakRSS_MB=563.328 items_per_second=46.4427M/s
SelfKNNQuery/10M/k=2        466467 us       466389 us            2 PeakRSS_MB=562.781 items_per_second=21.4413M/s
SelfKNNQuery/10M/k=4        728040 us       727938 us            1 PeakRSS_MB=562.781 items_per_second=13.7374M/s
SelfKNNQuery/10M/k=8       1142345 us      1141769 us            1 PeakRSS_MB=562.781 items_per_second=8.75834M/s
SelfKNNQuery/10M/k=16      1853394 us      1852386 us            1 PeakRSS_MB=562.781 items_per_second=5.39844M/s
SelfKNNQuery/10M/k=32      3172049 us      3171621 us            1 PeakRSS_MB=562.781 items_per_second=3.15296M/s
SelfKNNQuery/10M/k=64      9105528 us      9104237 us            1 PeakRSS_MB=562.797 items_per_second=1.09839M/s
SelfKNNQuery/10M/k=128    11101976 us     11100420 us            1 PeakRSS_MB=562.797 items_per_second=900.867k/s

```
