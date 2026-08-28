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
2026-08-27T08:56:45+02:00
Running ./self_knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 4.15, 4.03, 3.63
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------------------
Benchmark                        Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------
SelfKNNQuery/1K/k=1           42.4 us         42.4 us        13943 PeakRSS_MB=143.973 items_per_second=23.5843M/s
SelfKNNQuery/1K/k=2            172 us          172 us         4060 PeakRSS_MB=143.973 items_per_second=5.80906M/s
SelfKNNQuery/1K/k=4            239 us          239 us         2940 PeakRSS_MB=143.973 items_per_second=4.19134M/s
SelfKNNQuery/1K/k=8            381 us          381 us         1838 PeakRSS_MB=144.535 items_per_second=2.62791M/s
SelfKNNQuery/1K/k=16           804 us          804 us          870 PeakRSS_MB=143.973 items_per_second=1.24392M/s
SelfKNNQuery/1K/k=32          1146 us         1146 us          609 PeakRSS_MB=143.973 items_per_second=872.373k/s
SelfKNNQuery/1K/k=64          2925 us         2925 us          240 PeakRSS_MB=143.973 items_per_second=341.884k/s
SelfKNNQuery/1K/k=128         4857 us         4856 us          144 PeakRSS_MB=143.973 items_per_second=205.918k/s
SelfKNNQuery/10K/k=1          51.0 us         50.9 us        13669 PeakRSS_MB=145.098 items_per_second=196.278M/s
SelfKNNQuery/10K/k=2           290 us          290 us         2413 PeakRSS_MB=145.66 items_per_second=34.4991M/s
SelfKNNQuery/10K/k=4           490 us          490 us         1426 PeakRSS_MB=145.66 items_per_second=20.4055M/s
SelfKNNQuery/10K/k=8           823 us          823 us          849 PeakRSS_MB=145.66 items_per_second=12.146M/s
SelfKNNQuery/10K/k=16         1668 us         1667 us          419 PeakRSS_MB=145.098 items_per_second=5.99769M/s
SelfKNNQuery/10K/k=32         3437 us         3437 us          204 PeakRSS_MB=145.098 items_per_second=2.90976M/s
SelfKNNQuery/10K/k=64         8167 us         8165 us           86 PeakRSS_MB=145.66 items_per_second=1.22469M/s
SelfKNNQuery/10K/k=128       13603 us        13601 us           51 PeakRSS_MB=145.66 items_per_second=735.246k/s
SelfKNNQuery/100K/k=1          156 us          156 us         4436 PeakRSS_MB=148.812 items_per_second=639.734M/s
SelfKNNQuery/100K/k=2          931 us          931 us          751 PeakRSS_MB=148.484 items_per_second=107.395M/s
SelfKNNQuery/100K/k=4         1595 us         1595 us          439 PeakRSS_MB=149.438 items_per_second=62.6925M/s
SelfKNNQuery/100K/k=8         2765 us         2765 us          253 PeakRSS_MB=148.93 items_per_second=36.1673M/s
SelfKNNQuery/100K/k=16        5653 us         5652 us          121 PeakRSS_MB=148.934 items_per_second=17.6937M/s
SelfKNNQuery/100K/k=32       12515 us        12513 us           55 PeakRSS_MB=149.523 items_per_second=7.99167M/s
SelfKNNQuery/100K/k=64       30756 us        30751 us           23 PeakRSS_MB=148.906 items_per_second=3.25188M/s
SelfKNNQuery/100K/k=128      83377 us        83366 us            8 PeakRSS_MB=149.469 items_per_second=1.19953M/s
SelfKNNQuery/1M/k=1           1151 us         1151 us          606 PeakRSS_MB=200.719 items_per_second=868.646M/s
SelfKNNQuery/1M/k=2           7830 us         7829 us           89 PeakRSS_MB=200.176 items_per_second=127.73M/s
SelfKNNQuery/1M/k=4          14142 us        14140 us           50 PeakRSS_MB=200.199 items_per_second=70.7212M/s
SelfKNNQuery/1M/k=8          24801 us        24797 us           28 PeakRSS_MB=200.762 items_per_second=40.3267M/s
SelfKNNQuery/1M/k=16         47257 us        47251 us           15 PeakRSS_MB=200.203 items_per_second=21.1637M/s
SelfKNNQuery/1M/k=32        102034 us       102021 us            7 PeakRSS_MB=200.766 items_per_second=9.80188M/s
SelfKNNQuery/1M/k=64        236618 us       236588 us            3 PeakRSS_MB=200.203 items_per_second=4.22676M/s
SelfKNNQuery/1M/k=128       727075 us       726983 us            1 PeakRSS_MB=200.766 items_per_second=1.37555M/s
SelfKNNQuery/10M/k=1         11571 us        11569 us           61 PeakRSS_MB=565.812 items_per_second=864.382M/s
SelfKNNQuery/10M/k=2         81395 us        81385 us            9 PeakRSS_MB=565.812 items_per_second=122.873M/s
SelfKNNQuery/10M/k=4        142020 us       142002 us            5 PeakRSS_MB=566.375 items_per_second=70.4216M/s
SelfKNNQuery/10M/k=8        245948 us       245915 us            3 PeakRSS_MB=566.379 items_per_second=40.6645M/s
SelfKNNQuery/10M/k=16       468434 us       468377 us            2 PeakRSS_MB=566.383 items_per_second=21.3503M/s
SelfKNNQuery/10M/k=32      1003680 us      1003550 us            1 PeakRSS_MB=566.387 items_per_second=9.96463M/s
SelfKNNQuery/10M/k=64      2387512 us      2387224 us            1 PeakRSS_MB=565.828 items_per_second=4.18897M/s
SelfKNNQuery/10M/k=128     7451074 us      7450207 us            1 PeakRSS_MB=565.828 items_per_second=1.34224M/s

```
