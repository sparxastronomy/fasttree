# Benchmark Run: gpu
Date: Fri Jun 26 07:12:01 CEST 2026
Device: gpu
=============================================

## Configuration: SFC=MORTON, Decomp=HISTOGRAM, Coords=FLOAT
```
--- SFC Encoding Scaling ---
2026-06-26T07:13:44+02:00
Running ./sfc_encoding_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 9.42, 5.25, 3.49
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------
Benchmark                Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------
SFC-Encode/1K        0.018 ms        0.018 ms         3665 PeakRSS_MB=136.668 items_per_second=54.2466M/s
SFC-Encode/10K       0.019 ms        0.019 ms         3625 PeakRSS_MB=138.355 items_per_second=527.198M/s
SFC-Encode/100K      0.022 ms        0.022 ms         3208 PeakRSS_MB=140.48 items_per_second=4.62751G/s
SFC-Encode/1M        0.034 ms        0.034 ms         2058 PeakRSS_MB=170.527 items_per_second=29.3732G/s
SFC-Encode/10M       0.169 ms        0.169 ms          405 PeakRSS_MB=433.082 items_per_second=59.0167G/s
--- GPU/Radix Sort Scaling ---
2026-06-26T07:14:00+02:00
Running ./gpu_sort_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 7.99, 5.14, 3.48
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
GPU_Sort/1K        0.043 ms        0.043 ms         1246 PeakRSS_MB=143.75 items_per_second=23.0101M/s
GPU_Sort/10K       0.108 ms        0.108 ms          621 PeakRSS_MB=144.875 items_per_second=92.7231M/s
GPU_Sort/100K      0.323 ms        0.322 ms          214 PeakRSS_MB=150.922 items_per_second=310.094M/s
GPU_Sort/1M         1.32 ms         1.32 ms           62 PeakRSS_MB=189.016 items_per_second=758.975M/s
GPU_Sort/10M        13.3 ms         13.3 ms            5 PeakRSS_MB=288.398 items_per_second=749.375M/s
--- Tree Build Scaling ---
2026-06-26T07:14:08+02:00
Running ./tree_build_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 7.23, 5.07, 3.48
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------------
Benchmark               Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------
TreeBuild/1K         2.44 ms         2.44 ms           27 PeakRSS_MB=137.227 items_per_second=410.499k/s
TreeBuild/10K        3.87 ms         3.87 ms           18 PeakRSS_MB=138.914 items_per_second=2.58557M/s
TreeBuild/100K       5.58 ms         5.58 ms           10 PeakRSS_MB=142.629 items_per_second=17.9355M/s
TreeBuild/1M         30.1 ms         30.1 ms            3 PeakRSS_MB=190.645 items_per_second=33.251M/s
TreeBuild/10M         256 ms          256 ms            1 PeakRSS_MB=558.84 items_per_second=39.0474M/s
--- Rebuild Scaling ---
2026-06-26T07:14:14+02:00
Running ./rebuild_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 6.89, 5.04, 3.48
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------
TreeRebuild/1K         1.85 ms         1.84 ms           34 PeakRSS_MB=137.914 items_per_second=542.631k/s
TreeRebuild/10K        2.57 ms         2.57 ms           27 PeakRSS_MB=139.602 items_per_second=3.89751M/s
TreeRebuild/100K       4.88 ms         4.87 ms           12 PeakRSS_MB=142.809 items_per_second=20.5328M/s
TreeRebuild/1M         33.5 ms         33.4 ms            3 PeakRSS_MB=196.43 items_per_second=29.9288M/s
TreeRebuild/10M         196 ms          196 ms            1 PeakRSS_MB=634.109 items_per_second=51.091M/s
--- Range Query Scaling ---
2026-06-26T07:14:19+02:00
Running ./range_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 6.58, 5.00, 3.47
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
RangeQuery/1K/R=0.010000           51.1 us         51.1 us         1080 PeakRSS_MB=138.328 TotalFound=1k
RangeQuery/1K/R=0.100000           51.1 us         51.0 us         1281 PeakRSS_MB=138.328 TotalFound=1.005k
RangeQuery/1K/R=1.000000           63.4 us         63.4 us         1056 PeakRSS_MB=138.328 TotalFound=1.035k
RangeQuery/1K/R=10.000000           105 us          105 us          647 PeakRSS_MB=138.328 TotalFound=6.591k
RangeQuery/1K/R=100.000000          520 us          520 us          134 PeakRSS_MB=138.328 TotalFound=459.76k
RangeQuery/1K/R=200.000000          584 us          584 us          119 PeakRSS_MB=138.328 TotalFound=931.404k
RangeQuery/10K/R=0.010000          62.0 us         62.0 us         1072 PeakRSS_MB=139.453 TotalFound=1k
RangeQuery/10K/R=0.100000          68.1 us         68.1 us          979 PeakRSS_MB=139.453 TotalFound=1.001k
RangeQuery/10K/R=1.000000          87.1 us         87.0 us          774 PeakRSS_MB=139.453 TotalFound=1.295k
RangeQuery/10K/R=10.000000          487 us          487 us          143 PeakRSS_MB=139.453 TotalFound=53.444k
RangeQuery/10K/R=100.000000        6259 us         6258 us           11 PeakRSS_MB=139.453 TotalFound=971.536k
RangeQuery/10K/R=200.000000        5589 us         5588 us           13 PeakRSS_MB=139.453 TotalFound=977.028k
RangeQuery/100K/R=0.010000         76.7 us         76.7 us          875 PeakRSS_MB=142.871 TotalFound=1k
RangeQuery/100K/R=0.100000         93.9 us         93.9 us          729 PeakRSS_MB=142.98 TotalFound=1.005k
RangeQuery/100K/R=1.000000          236 us          236 us          292 PeakRSS_MB=142.926 TotalFound=4.145k
RangeQuery/100K/R=10.000000        4037 us         4036 us           17 PeakRSS_MB=142.926 TotalFound=252.323k
RangeQuery/100K/R=100.000000      69671 us        69663 us            1 PeakRSS_MB=142.926 TotalFound=971.031k
RangeQuery/100K/R=200.000000      87862 us        87842 us            1 PeakRSS_MB=142.926 TotalFound=971.031k
RangeQuery/1M/R=0.010000           83.9 us         83.8 us          793 PeakRSS_MB=187.875 TotalFound=1k
RangeQuery/1M/R=0.100000            148 us          148 us          460 PeakRSS_MB=188.438 TotalFound=1.139k
RangeQuery/1M/R=1.000000           1554 us         1553 us           45 PeakRSS_MB=188.438 TotalFound=35.52k
RangeQuery/1M/R=10.000000         47295 us        47284 us            2 PeakRSS_MB=189 TotalFound=688.697k
RangeQuery/1M/R=100.000000       886469 us       886356 us            1 PeakRSS_MB=189 TotalFound=981.019k
RangeQuery/1M/R=200.000000      1017256 us      1017127 us            1 PeakRSS_MB=188.438 TotalFound=981.019k
RangeQuery/10M/R=0.010000           119 us          119 us          552 PeakRSS_MB=560.883 TotalFound=1.001k
RangeQuery/10M/R=0.100000           298 us          298 us          247 PeakRSS_MB=560.941 TotalFound=1.793k
RangeQuery/10M/R=1.000000         12873 us        12870 us            5 PeakRSS_MB=561.543 TotalFound=116.137k
RangeQuery/10M/R=10.000000       468270 us       468200 us            1 PeakRSS_MB=561.555 TotalFound=952.08k
RangeQuery/10M/R=100.000000    11915329 us     11913871 us            1 PeakRSS_MB=560.992 TotalFound=976.025k
RangeQuery/10M/R=200.000000    16881725 us     16879747 us            1 PeakRSS_MB=560.992 TotalFound=976.029k
--- kNN Query Scaling ---
2026-06-26T07:16:14+02:00
Running ./knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 3.52, 4.36, 3.41
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
KNNQuery/1K/k=1           39.7 us         39.6 us         1329 PeakRSS_MB=138.992
KNNQuery/1K/k=2           76.2 us         76.2 us          895 PeakRSS_MB=138.992
KNNQuery/1K/k=4           96.6 us         96.5 us          706 PeakRSS_MB=138.992
KNNQuery/1K/k=8            148 us          148 us          467 PeakRSS_MB=138.992
KNNQuery/1K/k=16           255 us          255 us          273 PeakRSS_MB=138.992
KNNQuery/1K/k=32           589 us          589 us          118 PeakRSS_MB=138.992
KNNQuery/1K/k=64          1565 us         1564 us           45 PeakRSS_MB=138.992
KNNQuery/1K/k=128         4552 us         4551 us           15 PeakRSS_MB=138.992
KNNQuery/10K/k=1          53.6 us         53.6 us         1245 PeakRSS_MB=139.555
KNNQuery/10K/k=2           108 us          108 us          635 PeakRSS_MB=139.555
KNNQuery/10K/k=4           163 us          163 us          424 PeakRSS_MB=140.117
KNNQuery/10K/k=8           238 us          238 us          292 PeakRSS_MB=139.555
KNNQuery/10K/k=16          432 us          432 us          162 PeakRSS_MB=139.555
KNNQuery/10K/k=32          901 us          901 us           77 PeakRSS_MB=140.117
KNNQuery/10K/k=64         2321 us         2320 us           30 PeakRSS_MB=140.117
KNNQuery/10K/k=128        7680 us         7678 us            9 PeakRSS_MB=140.117
KNNQuery/100K/k=1         69.8 us         69.8 us          957 PeakRSS_MB=143.676
KNNQuery/100K/k=2          127 us          126 us          543 PeakRSS_MB=144.613
KNNQuery/100K/k=4          184 us          184 us          377 PeakRSS_MB=144.031
KNNQuery/100K/k=8          264 us          264 us          264 PeakRSS_MB=144.008
KNNQuery/100K/k=16         429 us          429 us          162 PeakRSS_MB=144.57
KNNQuery/100K/k=32         919 us          918 us           76 PeakRSS_MB=144.57
KNNQuery/100K/k=64        2439 us         2439 us           29 PeakRSS_MB=144.555
KNNQuery/100K/k=128       7855 us         7853 us            9 PeakRSS_MB=143.992
KNNQuery/1M/k=1           78.8 us         78.8 us          854 PeakRSS_MB=188.434
KNNQuery/1M/k=2            194 us          194 us          355 PeakRSS_MB=188.441
KNNQuery/1M/k=4            269 us          269 us          258 PeakRSS_MB=188.441
KNNQuery/1M/k=8            321 us          321 us          218 PeakRSS_MB=189.004
KNNQuery/1M/k=16           621 us          621 us          109 PeakRSS_MB=189.004
KNNQuery/1M/k=32          1191 us         1190 us           59 PeakRSS_MB=189.004
KNNQuery/1M/k=64          2911 us         2909 us           24 PeakRSS_MB=188.441
KNNQuery/1M/k=128         9084 us         9082 us            8 PeakRSS_MB=188.441
KNNQuery/10M/k=1          85.9 us         85.8 us          634 PeakRSS_MB=550.043
KNNQuery/10M/k=2           210 us          210 us          319 PeakRSS_MB=550.074
KNNQuery/10M/k=4           239 us          239 us          279 PeakRSS_MB=555.191
KNNQuery/10M/k=8           457 us          456 us          120 PeakRSS_MB=555.762
KNNQuery/10M/k=16          625 us          625 us           82 PeakRSS_MB=555.762
KNNQuery/10M/k=32         1359 us         1359 us           50 PeakRSS_MB=555.762
KNNQuery/10M/k=64         4011 us         4009 us           18 PeakRSS_MB=556.324
KNNQuery/10M/k=128        9718 us         9714 us            6 PeakRSS_MB=555.762
```

## Configuration: SFC=MORTON, Decomp=HISTOGRAM, Coords=DOUBLE
```
--- SFC Encoding Scaling ---
2026-06-26T07:22:04+02:00
Running ./sfc_encoding_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 12.13, 9.92, 6.02
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------
Benchmark                Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------
SFC-Encode/1K        0.019 ms        0.019 ms         3612 PeakRSS_MB=133.98 items_per_second=53.8123M/s
SFC-Encode/10K       0.019 ms        0.019 ms         3573 PeakRSS_MB=134.895 items_per_second=522.189M/s
SFC-Encode/100K      0.022 ms        0.022 ms         3175 PeakRSS_MB=137.988 items_per_second=4.59974G/s
SFC-Encode/1M        0.039 ms        0.039 ms         1791 PeakRSS_MB=168.086 items_per_second=25.9297G/s
SFC-Encode/10M       0.250 ms        0.250 ms          276 PeakRSS_MB=430.082 items_per_second=39.9799G/s
--- GPU/Radix Sort Scaling ---
2026-06-26T07:22:21+02:00
Running ./gpu_sort_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 10.10, 9.58, 5.97
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
GPU_Sort/1K        0.048 ms        0.048 ms         1134 PeakRSS_MB=139.566 items_per_second=20.9396M/s
GPU_Sort/10K       0.115 ms        0.115 ms          574 PeakRSS_MB=140.691 items_per_second=86.9209M/s
GPU_Sort/100K      0.328 ms        0.327 ms          210 PeakRSS_MB=148.004 items_per_second=305.549M/s
GPU_Sort/1M         1.32 ms         1.32 ms           54 PeakRSS_MB=196.207 items_per_second=757.343M/s
GPU_Sort/10M        13.4 ms         13.4 ms            5 PeakRSS_MB=398.281 items_per_second=746.496M/s
--- Tree Build Scaling ---
2026-06-26T07:22:29+02:00
Running ./tree_build_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 9.01, 9.37, 5.94
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------------
Benchmark               Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------
TreeBuild/1K         2.68 ms         2.68 ms           24 PeakRSS_MB=136.203 items_per_second=373.249k/s
TreeBuild/10K        3.63 ms         3.63 ms           19 PeakRSS_MB=137.328 items_per_second=2.75366M/s
TreeBuild/100K       7.61 ms         7.61 ms            8 PeakRSS_MB=142.301 items_per_second=13.1476M/s
TreeBuild/1M         37.8 ms         37.8 ms            2 PeakRSS_MB=197.453 items_per_second=26.4855M/s
TreeBuild/10M         324 ms          323 ms            1 PeakRSS_MB=660.746 items_per_second=30.9218M/s
--- Rebuild Scaling ---
2026-06-26T07:22:35+02:00
Running ./rebuild_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 8.53, 9.26, 5.93
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------
TreeRebuild/1K         1.96 ms         1.95 ms           32 PeakRSS_MB=138.676 items_per_second=512.619k/s
TreeRebuild/10K        2.54 ms         2.54 ms           27 PeakRSS_MB=139.801 items_per_second=3.93529M/s
TreeRebuild/100K       7.14 ms         7.13 ms           10 PeakRSS_MB=145.621 items_per_second=14.0223M/s
TreeRebuild/1M         35.0 ms         35.0 ms            2 PeakRSS_MB=205.926 items_per_second=28.5934M/s
TreeRebuild/10M         319 ms          318 ms            1 PeakRSS_MB=788.172 items_per_second=31.406M/s
--- Range Query Scaling ---
2026-06-26T07:22:40+02:00
Running ./range_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 8.09, 9.16, 5.91
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
RangeQuery/1K/R=0.010000           78.7 us         78.6 us          758 PeakRSS_MB=137.773 TotalFound=1k
RangeQuery/1K/R=0.100000           78.7 us         78.6 us          847 PeakRSS_MB=137.711 TotalFound=1.005k
RangeQuery/1K/R=1.000000            106 us          106 us          642 PeakRSS_MB=137.711 TotalFound=1.035k
RangeQuery/1K/R=10.000000           187 us          187 us          367 PeakRSS_MB=138.211 TotalFound=6.591k
RangeQuery/1K/R=100.000000          977 us          976 us           72 PeakRSS_MB=137.648 TotalFound=459.76k
RangeQuery/1K/R=200.000000         1143 us         1142 us           61 PeakRSS_MB=137.586 TotalFound=931.404k
RangeQuery/10K/R=0.010000           103 us          103 us          659 PeakRSS_MB=138.148 TotalFound=1k
RangeQuery/10K/R=0.100000           116 us          116 us          548 PeakRSS_MB=138.648 TotalFound=1.001k
RangeQuery/10K/R=1.000000           160 us          160 us          430 PeakRSS_MB=138.648 TotalFound=1.295k
RangeQuery/10K/R=10.000000          993 us          992 us           70 PeakRSS_MB=138.586 TotalFound=53.444k
RangeQuery/10K/R=100.000000       14926 us        14923 us            5 PeakRSS_MB=138.586 TotalFound=971.536k
RangeQuery/10K/R=200.000000       12959 us        12956 us            5 PeakRSS_MB=138.586 TotalFound=977.028k
RangeQuery/100K/R=0.010000          129 us          129 us          528 PeakRSS_MB=143.277 TotalFound=1k
RangeQuery/100K/R=0.100000          165 us          165 us          414 PeakRSS_MB=143.254 TotalFound=1.005k
RangeQuery/100K/R=1.000000          510 us          509 us          137 PeakRSS_MB=143.254 TotalFound=4.145k
RangeQuery/100K/R=10.000000        9697 us         9696 us            7 PeakRSS_MB=143.215 TotalFound=252.323k
RangeQuery/100K/R=100.000000     155067 us       155048 us            1 PeakRSS_MB=143.215 TotalFound=971.031k
RangeQuery/100K/R=200.000000     204703 us       204676 us            1 PeakRSS_MB=143.152 TotalFound=971.031k
RangeQuery/1M/R=0.010000            143 us          143 us          476 PeakRSS_MB=197.926 TotalFound=1k
RangeQuery/1M/R=0.100000            281 us          281 us          246 PeakRSS_MB=197.926 TotalFound=1.139k
RangeQuery/1M/R=1.000000           3602 us         3602 us           19 PeakRSS_MB=197.926 TotalFound=35.52k
RangeQuery/1M/R=10.000000        123057 us       123043 us            1 PeakRSS_MB=197.945 TotalFound=688.697k
RangeQuery/1M/R=100.000000      2610400 us      2610083 us            1 PeakRSS_MB=198.508 TotalFound=981.019k
RangeQuery/1M/R=200.000000      3108867 us      3108469 us            1 PeakRSS_MB=198.547 TotalFound=981.019k
RangeQuery/10M/R=0.010000           176 us          176 us          401 PeakRSS_MB=671.699 TotalFound=1.001k
RangeQuery/10M/R=0.100000           599 us          599 us           93 PeakRSS_MB=671.156 TotalFound=1.793k
RangeQuery/10M/R=1.000000         33293 us        33282 us            2 PeakRSS_MB=671.168 TotalFound=116.137k
RangeQuery/10M/R=10.000000      1323715 us      1323545 us            1 PeakRSS_MB=671.172 TotalFound=952.08k
RangeQuery/10M/R=100.000000    31126441 us     31122752 us            1 PeakRSS_MB=671.176 TotalFound=976.025k
RangeQuery/10M/R=200.000000    45621263 us     45615644 us            1 PeakRSS_MB=671.176 TotalFound=976.029k
--- kNN Query Scaling ---
2026-06-26T07:26:20+02:00
Running ./knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 3.15, 5.96, 5.30
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
KNNQuery/1K/k=1           60.6 us         60.6 us          948 PeakRSS_MB=136.148
KNNQuery/1K/k=2            127 us          127 us          543 PeakRSS_MB=136.086
KNNQuery/1K/k=4            160 us          159 us          431 PeakRSS_MB=136.023
KNNQuery/1K/k=8            240 us          240 us          290 PeakRSS_MB=136.586
KNNQuery/1K/k=16           384 us          384 us          182 PeakRSS_MB=135.961
KNNQuery/1K/k=32           805 us          805 us           87 PeakRSS_MB=136.523
KNNQuery/1K/k=64          2005 us         2005 us           35 PeakRSS_MB=135.898
KNNQuery/1K/k=128         5605 us         5604 us           12 PeakRSS_MB=136.461
KNNQuery/10K/k=1          87.3 us         87.2 us          779 PeakRSS_MB=137.023
KNNQuery/10K/k=2           195 us          195 us          355 PeakRSS_MB=137.523
KNNQuery/10K/k=4           291 us          291 us          240 PeakRSS_MB=137.523
KNNQuery/10K/k=8           410 us          410 us          170 PeakRSS_MB=137.523
KNNQuery/10K/k=16          694 us          693 us          100 PeakRSS_MB=138.023
KNNQuery/10K/k=32         1385 us         1385 us           51 PeakRSS_MB=137.461
KNNQuery/10K/k=64         3463 us         3462 us           20 PeakRSS_MB=137.398
KNNQuery/10K/k=128       10797 us        10796 us            6 PeakRSS_MB=137.398
KNNQuery/100K/k=1          112 us          112 us          609 PeakRSS_MB=142.613
KNNQuery/100K/k=2          216 us          216 us          321 PeakRSS_MB=142.113
KNNQuery/100K/k=4          325 us          324 us          215 PeakRSS_MB=142.094
KNNQuery/100K/k=8          442 us          442 us          158 PeakRSS_MB=142.012
KNNQuery/100K/k=16         680 us          680 us          102 PeakRSS_MB=142.012
KNNQuery/100K/k=32        1381 us         1381 us           51 PeakRSS_MB=142.012
KNNQuery/100K/k=64        3536 us         3535 us           20 PeakRSS_MB=142.242
KNNQuery/100K/k=128      11225 us        11222 us            6 PeakRSS_MB=141.68
KNNQuery/1M/k=1            127 us          127 us          536 PeakRSS_MB=196.812
KNNQuery/1M/k=2            269 us          269 us          258 PeakRSS_MB=196.875
KNNQuery/1M/k=4            375 us          375 us          186 PeakRSS_MB=196.875
KNNQuery/1M/k=8            508 us          508 us          135 PeakRSS_MB=196.312
KNNQuery/1M/k=16           911 us          910 us           77 PeakRSS_MB=196.883
KNNQuery/1M/k=32          1939 us         1939 us           36 PeakRSS_MB=196.887
KNNQuery/1M/k=64          4704 us         4703 us           15 PeakRSS_MB=196.336
KNNQuery/1M/k=128        13722 us        13719 us            5 PeakRSS_MB=196.898
KNNQuery/10M/k=1           134 us          134 us          497 PeakRSS_MB=669.855
KNNQuery/10M/k=2           312 us          312 us          214 PeakRSS_MB=669.855
KNNQuery/10M/k=4           475 us          475 us          152 PeakRSS_MB=669.855
KNNQuery/10M/k=8           716 us          716 us           77 PeakRSS_MB=669.488
KNNQuery/10M/k=16         1185 us         1185 us           46 PeakRSS_MB=669.488
KNNQuery/10M/k=32         2184 us         2184 us           31 PeakRSS_MB=668.93
KNNQuery/10M/k=64         4947 us         4946 us           14 PeakRSS_MB=668.93
KNNQuery/10M/k=128       14638 us        14633 us            5 PeakRSS_MB=668.93
```

## Configuration: SFC=MORTON, Decomp=SAMPLING, Coords=FLOAT
```
--- SFC Encoding Scaling ---
2026-06-26T07:32:32+02:00
Running ./sfc_encoding_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 10.35, 10.03, 7.22
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------
Benchmark                Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------
SFC-Encode/1K        0.018 ms        0.018 ms         3701 PeakRSS_MB=135.684 items_per_second=54.9403M/s
SFC-Encode/10K       0.019 ms        0.019 ms         3650 PeakRSS_MB=136.809 items_per_second=532.028M/s
SFC-Encode/100K      0.021 ms        0.021 ms         3222 PeakRSS_MB=139.004 items_per_second=4.66587G/s
SFC-Encode/1M        0.034 ms        0.034 ms         2065 PeakRSS_MB=197.664 items_per_second=29.566G/s
SFC-Encode/10M       0.169 ms        0.169 ms          405 PeakRSS_MB=466.066 items_per_second=59.208G/s
--- GPU/Radix Sort Scaling ---
2026-06-26T07:32:48+02:00
Running ./gpu_sort_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 8.26, 9.58, 7.13
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
GPU_Sort/1K        0.043 ms        0.043 ms         1256 PeakRSS_MB=141.68 items_per_second=23.5032M/s
GPU_Sort/10K       0.107 ms        0.106 ms          630 PeakRSS_MB=142.805 items_per_second=93.9446M/s
GPU_Sort/100K      0.320 ms        0.320 ms          215 PeakRSS_MB=148.727 items_per_second=312.529M/s
GPU_Sort/1M         1.10 ms         1.10 ms           63 PeakRSS_MB=187.184 items_per_second=906.577M/s
GPU_Sort/10M        13.2 ms         13.2 ms            5 PeakRSS_MB=286.793 items_per_second=758.977M/s
--- Tree Build Scaling ---
2026-06-26T07:32:56+02:00
Running ./tree_build_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 7.84, 9.47, 7.10
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------------
Benchmark               Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------
TreeBuild/1K         2.46 ms         2.45 ms           26 PeakRSS_MB=137.789 items_per_second=407.436k/s
TreeBuild/10K        3.96 ms         3.96 ms           18 PeakRSS_MB=138.914 items_per_second=2.52699M/s
TreeBuild/100K       5.82 ms         5.82 ms           10 PeakRSS_MB=142.344 items_per_second=17.1883M/s
TreeBuild/1M         26.1 ms         26.1 ms            3 PeakRSS_MB=195.031 items_per_second=38.3354M/s
TreeBuild/10M         228 ms          227 ms            1 PeakRSS_MB=559.219 items_per_second=43.9682M/s
--- Rebuild Scaling ---
2026-06-26T07:33:02+02:00
Running ./rebuild_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 7.53, 9.38, 7.09
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------
TreeRebuild/1K         1.90 ms         1.89 ms           33 PeakRSS_MB=139.809 items_per_second=528.806k/s
TreeRebuild/10K        2.63 ms         2.62 ms           26 PeakRSS_MB=140.934 items_per_second=3.81274M/s
TreeRebuild/100K       4.97 ms         4.97 ms           12 PeakRSS_MB=145.082 items_per_second=20.1195M/s
TreeRebuild/1M         25.4 ms         25.4 ms            3 PeakRSS_MB=198.969 items_per_second=39.4411M/s
TreeRebuild/10M         225 ms          225 ms            1 PeakRSS_MB=636.004 items_per_second=44.4472M/s
--- Range Query Scaling ---
2026-06-26T07:33:07+02:00
Running ./range_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 6.92, 9.18, 7.05
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
RangeQuery/1K/R=0.010000           50.9 us         50.9 us         1084 PeakRSS_MB=137.922 TotalFound=1k
RangeQuery/1K/R=0.100000           51.0 us         51.0 us         1282 PeakRSS_MB=137.922 TotalFound=1.005k
RangeQuery/1K/R=1.000000           63.3 us         63.3 us         1054 PeakRSS_MB=137.359 TotalFound=1.035k
RangeQuery/1K/R=10.000000           105 us          105 us          648 PeakRSS_MB=137.922 TotalFound=6.591k
RangeQuery/1K/R=100.000000          520 us          520 us          134 PeakRSS_MB=137.922 TotalFound=459.76k
RangeQuery/1K/R=200.000000          584 us          584 us          119 PeakRSS_MB=137.922 TotalFound=931.404k
RangeQuery/10K/R=0.010000          61.9 us         61.9 us         1072 PeakRSS_MB=139.047 TotalFound=1k
RangeQuery/10K/R=0.100000          68.9 us         68.8 us          983 PeakRSS_MB=138.484 TotalFound=1.001k
RangeQuery/10K/R=1.000000          87.1 us         87.1 us          772 PeakRSS_MB=139.047 TotalFound=1.295k
RangeQuery/10K/R=10.000000          488 us          488 us          143 PeakRSS_MB=139.047 TotalFound=53.444k
RangeQuery/10K/R=100.000000        6257 us         6256 us           11 PeakRSS_MB=139.047 TotalFound=971.536k
RangeQuery/10K/R=200.000000        5587 us         5586 us           13 PeakRSS_MB=138.484 TotalFound=977.028k
RangeQuery/100K/R=0.010000         76.4 us         76.3 us          880 PeakRSS_MB=146.359 TotalFound=1k
RangeQuery/100K/R=0.100000         92.5 us         92.4 us          730 PeakRSS_MB=146.359 TotalFound=1.005k
RangeQuery/100K/R=1.000000          236 us          236 us          293 PeakRSS_MB=145.797 TotalFound=4.145k
RangeQuery/100K/R=10.000000        4038 us         4037 us           17 PeakRSS_MB=145.797 TotalFound=252.323k
RangeQuery/100K/R=100.000000      69650 us        69639 us            1 PeakRSS_MB=146.359 TotalFound=971.031k
RangeQuery/100K/R=200.000000      87826 us        87811 us            1 PeakRSS_MB=146.359 TotalFound=971.031k
RangeQuery/1M/R=0.010000           83.3 us         83.3 us          794 PeakRSS_MB=200.391 TotalFound=1k
RangeQuery/1M/R=0.100000            147 us          147 us          461 PeakRSS_MB=200.953 TotalFound=1.139k
RangeQuery/1M/R=1.000000           1363 us         1362 us           51 PeakRSS_MB=201.516 TotalFound=35.52k
RangeQuery/1M/R=10.000000         47410 us        47403 us            2 PeakRSS_MB=200.953 TotalFound=688.697k
RangeQuery/1M/R=100.000000       879189 us       879083 us            1 PeakRSS_MB=201.516 TotalFound=981.019k
RangeQuery/1M/R=200.000000      1038938 us      1038802 us            1 PeakRSS_MB=200.953 TotalFound=981.019k
RangeQuery/10M/R=0.010000           111 us          111 us          665 PeakRSS_MB=557.707 TotalFound=1.001k
RangeQuery/10M/R=0.100000           280 us          280 us          248 PeakRSS_MB=557.754 TotalFound=1.793k
RangeQuery/10M/R=1.000000         12875 us        12873 us            5 PeakRSS_MB=557.781 TotalFound=116.137k
RangeQuery/10M/R=10.000000       468708 us       468637 us            1 PeakRSS_MB=558.355 TotalFound=952.08k
RangeQuery/10M/R=100.000000    11920700 us     11919188 us            1 PeakRSS_MB=557.809 TotalFound=976.025k
RangeQuery/10M/R=200.000000    16793650 us     16791266 us            1 PeakRSS_MB=557.246 TotalFound=976.029k
--- kNN Query Scaling ---
2026-06-26T07:35:03+02:00
Running ./knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 3.57, 7.20, 6.57
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
KNNQuery/1K/k=1           39.8 us         39.8 us         1334 PeakRSS_MB=137.73
KNNQuery/1K/k=2           76.3 us         76.3 us          893 PeakRSS_MB=137.73
KNNQuery/1K/k=4           96.7 us         96.6 us          705 PeakRSS_MB=137.168
KNNQuery/1K/k=8            148 us          148 us          466 PeakRSS_MB=137.73
KNNQuery/1K/k=16           255 us          255 us          273 PeakRSS_MB=137.73
KNNQuery/1K/k=32           589 us          589 us          118 PeakRSS_MB=137.73
KNNQuery/1K/k=64          1565 us         1565 us           45 PeakRSS_MB=137.73
KNNQuery/1K/k=128         4553 us         4552 us           15 PeakRSS_MB=137.73
KNNQuery/10K/k=1          53.8 us         53.7 us         1224 PeakRSS_MB=138.293
KNNQuery/10K/k=2           108 us          108 us          635 PeakRSS_MB=138.293
KNNQuery/10K/k=4           162 us          162 us          425 PeakRSS_MB=138.293
KNNQuery/10K/k=8           239 us          239 us          291 PeakRSS_MB=138.293
KNNQuery/10K/k=16          431 us          431 us          162 PeakRSS_MB=138.293
KNNQuery/10K/k=32          902 us          902 us           77 PeakRSS_MB=138.293
KNNQuery/10K/k=64         2329 us         2328 us           30 PeakRSS_MB=138.293
KNNQuery/10K/k=128        7610 us         7609 us            9 PeakRSS_MB=138.293
KNNQuery/100K/k=1         69.8 us         69.8 us          970 PeakRSS_MB=142.262
KNNQuery/100K/k=2          127 us          127 us          543 PeakRSS_MB=142.293
KNNQuery/100K/k=4          184 us          184 us          377 PeakRSS_MB=142.262
KNNQuery/100K/k=8          264 us          264 us          263 PeakRSS_MB=142.293
KNNQuery/100K/k=16         430 us          430 us          162 PeakRSS_MB=142.293
KNNQuery/100K/k=32         916 us          916 us           76 PeakRSS_MB=142.293
KNNQuery/100K/k=64        2415 us         2414 us           29 PeakRSS_MB=142.293
KNNQuery/100K/k=128       7782 us         7781 us            9 PeakRSS_MB=142.293
KNNQuery/1M/k=1           78.9 us         78.9 us          854 PeakRSS_MB=203.031
KNNQuery/1M/k=2            194 us          194 us          355 PeakRSS_MB=204.156
KNNQuery/1M/k=4            270 us          270 us          257 PeakRSS_MB=203.594
KNNQuery/1M/k=8            349 us          349 us          200 PeakRSS_MB=203.594
KNNQuery/1M/k=16           523 us          523 us          130 PeakRSS_MB=203.594
KNNQuery/1M/k=32          1343 us         1343 us           61 PeakRSS_MB=203.594
KNNQuery/1M/k=64          2929 us         2928 us           24 PeakRSS_MB=203.594
KNNQuery/1M/k=128         9135 us         9132 us            8 PeakRSS_MB=203.594
KNNQuery/10M/k=1          98.7 us         98.7 us          773 PeakRSS_MB=556.477
KNNQuery/10M/k=2           182 us          182 us          394 PeakRSS_MB=556.484
KNNQuery/10M/k=4           264 us          264 us          226 PeakRSS_MB=556.484
KNNQuery/10M/k=8           428 us          428 us          152 PeakRSS_MB=555.922
KNNQuery/10M/k=16          624 us          624 us           97 PeakRSS_MB=555.922
KNNQuery/10M/k=32         1534 us         1533 us           43 PeakRSS_MB=556.484
KNNQuery/10M/k=64         3142 us         3141 us           22 PeakRSS_MB=555.922
KNNQuery/10M/k=128        9731 us         9728 us            7 PeakRSS_MB=555.922
```

## Configuration: SFC=MORTON, Decomp=SAMPLING, Coords=DOUBLE
```
--- SFC Encoding Scaling ---
2026-06-26T07:40:24+02:00
Running ./sfc_encoding_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 9.43, 9.35, 7.58
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------
Benchmark                Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------
SFC-Encode/1K        0.018 ms        0.018 ms         3637 PeakRSS_MB=135.996 items_per_second=54.204M/s
SFC-Encode/10K       0.019 ms        0.019 ms         3585 PeakRSS_MB=136.371 items_per_second=522.259M/s
SFC-Encode/100K      0.022 ms        0.022 ms         3183 PeakRSS_MB=139.695 items_per_second=4.59161G/s
SFC-Encode/1M        0.038 ms        0.038 ms         1790 PeakRSS_MB=169.793 items_per_second=26.0457G/s
SFC-Encode/10M       0.250 ms        0.250 ms          277 PeakRSS_MB=431.648 items_per_second=40.0112G/s
--- GPU/Radix Sort Scaling ---
2026-06-26T07:40:40+02:00
Running ./gpu_sort_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 8.00, 9.04, 7.51
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
GPU_Sort/1K        0.043 ms        0.043 ms         1236 PeakRSS_MB=140.965 items_per_second=23.3704M/s
GPU_Sort/10K       0.107 ms        0.107 ms          627 PeakRSS_MB=142.652 items_per_second=93.4419M/s
GPU_Sort/100K      0.320 ms        0.320 ms          217 PeakRSS_MB=149.965 items_per_second=312.707M/s
GPU_Sort/1M         1.10 ms         1.10 ms           63 PeakRSS_MB=198.438 items_per_second=909.494M/s
GPU_Sort/10M        13.4 ms         13.3 ms            5 PeakRSS_MB=400.672 items_per_second=749.215M/s
--- Tree Build Scaling ---
2026-06-26T07:40:49+02:00
Running ./tree_build_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 7.23, 8.84, 7.46
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------------
Benchmark               Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------
TreeBuild/1K         2.36 ms         2.35 ms           27 PeakRSS_MB=138.676 items_per_second=425.617k/s
TreeBuild/10K        3.16 ms         3.16 ms           21 PeakRSS_MB=140.363 items_per_second=3.169M/s
TreeBuild/100K       8.47 ms         8.46 ms            9 PeakRSS_MB=145.012 items_per_second=11.8203M/s
TreeBuild/1M         51.2 ms         51.2 ms            1 PeakRSS_MB=198.832 items_per_second=19.5394M/s
TreeBuild/10M         324 ms          324 ms            1 PeakRSS_MB=663.066 items_per_second=30.8677M/s
--- Rebuild Scaling ---
2026-06-26T07:40:54+02:00
Running ./rebuild_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 6.90, 8.75, 7.44
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------
TreeRebuild/1K         2.31 ms         2.30 ms           26 PeakRSS_MB=137.227 items_per_second=434.225k/s
TreeRebuild/10K        2.85 ms         2.85 ms           25 PeakRSS_MB=137.789 items_per_second=3.50765M/s
TreeRebuild/100K       9.01 ms         9.01 ms            8 PeakRSS_MB=143.629 items_per_second=11.1044M/s
TreeRebuild/1M         39.2 ms         39.1 ms            2 PeakRSS_MB=207.711 items_per_second=25.5488M/s
TreeRebuild/10M         332 ms          331 ms            1 PeakRSS_MB=784.598 items_per_second=30.176M/s
--- Range Query Scaling ---
2026-06-26T07:41:00+02:00
Running ./range_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 6.58, 8.65, 7.41
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
RangeQuery/1K/R=0.010000           78.5 us         78.5 us          762 PeakRSS_MB=137.199 TotalFound=1k
RangeQuery/1K/R=0.100000           78.4 us         78.3 us          852 PeakRSS_MB=136.574 TotalFound=1.005k
RangeQuery/1K/R=1.000000            106 us          106 us          644 PeakRSS_MB=136.574 TotalFound=1.035k
RangeQuery/1K/R=10.000000           186 us          186 us          367 PeakRSS_MB=137.074 TotalFound=6.591k
RangeQuery/1K/R=100.000000          976 us          976 us           72 PeakRSS_MB=136.512 TotalFound=459.76k
RangeQuery/1K/R=200.000000         1142 us         1142 us           61 PeakRSS_MB=137.012 TotalFound=931.404k
RangeQuery/10K/R=0.010000           102 us          102 us          661 PeakRSS_MB=138.137 TotalFound=1k
RangeQuery/10K/R=0.100000           117 us          117 us          592 PeakRSS_MB=138.074 TotalFound=1.001k
RangeQuery/10K/R=1.000000           160 us          160 us          431 PeakRSS_MB=138.074 TotalFound=1.295k
RangeQuery/10K/R=10.000000          994 us          994 us           70 PeakRSS_MB=138.012 TotalFound=53.444k
RangeQuery/10K/R=100.000000       14921 us        14919 us            5 PeakRSS_MB=138.012 TotalFound=971.536k
RangeQuery/10K/R=200.000000       12959 us        12957 us            5 PeakRSS_MB=138.012 TotalFound=977.028k
RangeQuery/100K/R=0.010000          129 us          129 us          526 PeakRSS_MB=142.316 TotalFound=1k
RangeQuery/100K/R=0.100000          165 us          165 us          413 PeakRSS_MB=142.254 TotalFound=1.005k
RangeQuery/100K/R=1.000000          510 us          510 us          137 PeakRSS_MB=142.254 TotalFound=4.145k
RangeQuery/100K/R=10.000000        9696 us         9695 us            7 PeakRSS_MB=142.816 TotalFound=252.323k
RangeQuery/100K/R=100.000000     155068 us       155043 us            1 PeakRSS_MB=142.254 TotalFound=971.031k
RangeQuery/100K/R=200.000000     204699 us       204670 us            1 PeakRSS_MB=142.816 TotalFound=971.031k
RangeQuery/1M/R=0.010000            143 us          142 us          475 PeakRSS_MB=197.527 TotalFound=1k
RangeQuery/1M/R=0.100000            224 us          224 us          309 PeakRSS_MB=198.09 TotalFound=1.139k
RangeQuery/1M/R=1.000000           2841 us         2841 us           25 PeakRSS_MB=197.465 TotalFound=35.52k
RangeQuery/1M/R=10.000000        116147 us       116131 us            1 PeakRSS_MB=197.465 TotalFound=688.697k
RangeQuery/1M/R=100.000000      2590354 us      2590054 us            1 PeakRSS_MB=197.465 TotalFound=981.019k
RangeQuery/1M/R=200.000000      3146922 us      3146538 us            1 PeakRSS_MB=197.465 TotalFound=981.019k
RangeQuery/10M/R=0.010000           164 us          164 us          336 PeakRSS_MB=669.898 TotalFound=1.001k
RangeQuery/10M/R=0.100000           674 us          674 us           94 PeakRSS_MB=670.48 TotalFound=1.793k
RangeQuery/10M/R=1.000000         33079 us        33073 us            2 PeakRSS_MB=669.359 TotalFound=116.137k
RangeQuery/10M/R=10.000000      1318525 us      1318368 us            1 PeakRSS_MB=669.359 TotalFound=952.08k
RangeQuery/10M/R=100.000000    31116506 us     31112518 us            1 PeakRSS_MB=669.922 TotalFound=976.025k
RangeQuery/10M/R=200.000000    45791050 us     45785472 us            1 PeakRSS_MB=669.922 TotalFound=976.029k
--- kNN Query Scaling ---
2026-06-26T07:44:39+02:00
Running ./knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 3.15, 5.72, 6.48
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
KNNQuery/1K/k=1           60.7 us         60.7 us          950 PeakRSS_MB=138.613
KNNQuery/1K/k=2            128 us          128 us          544 PeakRSS_MB=138.551
KNNQuery/1K/k=4            160 us          160 us          431 PeakRSS_MB=138.551
KNNQuery/1K/k=8            240 us          240 us          290 PeakRSS_MB=139.051
KNNQuery/1K/k=16           384 us          384 us          182 PeakRSS_MB=139.051
KNNQuery/1K/k=32           805 us          805 us           87 PeakRSS_MB=138.988
KNNQuery/1K/k=64          2008 us         2008 us           35 PeakRSS_MB=138.426
KNNQuery/1K/k=128         5617 us         5616 us           12 PeakRSS_MB=138.363
KNNQuery/10K/k=1          87.3 us         87.3 us          776 PeakRSS_MB=139.488
KNNQuery/10K/k=2           195 us          195 us          355 PeakRSS_MB=139.988
KNNQuery/10K/k=4           290 us          290 us          240 PeakRSS_MB=139.988
KNNQuery/10K/k=8           408 us          407 us          171 PeakRSS_MB=139.926
KNNQuery/10K/k=16          704 us          704 us           99 PeakRSS_MB=140.488
KNNQuery/10K/k=32         1402 us         1401 us           50 PeakRSS_MB=139.863
KNNQuery/10K/k=64         3434 us         3433 us           20 PeakRSS_MB=139.863
KNNQuery/10K/k=128       10943 us        10941 us            7 PeakRSS_MB=139.863
KNNQuery/100K/k=1          112 us          112 us          613 PeakRSS_MB=144.82
KNNQuery/100K/k=2          216 us          216 us          322 PeakRSS_MB=144.832
KNNQuery/100K/k=4          324 us          324 us          215 PeakRSS_MB=144.277
KNNQuery/100K/k=8          442 us          442 us          158 PeakRSS_MB=144.84
KNNQuery/100K/k=16         683 us          682 us          102 PeakRSS_MB=144.867
KNNQuery/100K/k=32        1374 us         1373 us           51 PeakRSS_MB=144.836
KNNQuery/100K/k=64        3571 us         3571 us           20 PeakRSS_MB=144.836
KNNQuery/100K/k=128      11160 us        11157 us            6 PeakRSS_MB=144.836
KNNQuery/1M/k=1            127 us          127 us          534 PeakRSS_MB=199.477
KNNQuery/1M/k=2            340 us          340 us          204 PeakRSS_MB=199.543
KNNQuery/1M/k=4            455 us          455 us          154 PeakRSS_MB=199.543
KNNQuery/1M/k=8            505 us          505 us          136 PeakRSS_MB=199.543
KNNQuery/1M/k=16           909 us          909 us           76 PeakRSS_MB=199.555
KNNQuery/1M/k=32          1961 us         1961 us           35 PeakRSS_MB=199.574
KNNQuery/1M/k=64          4723 us         4722 us           15 PeakRSS_MB=199.617
KNNQuery/1M/k=128        13962 us        13958 us            5 PeakRSS_MB=199.617
KNNQuery/10M/k=1           134 us          134 us          497 PeakRSS_MB=664.434
KNNQuery/10M/k=2           309 us          308 us          214 PeakRSS_MB=672.316
KNNQuery/10M/k=4           455 us          455 us          152 PeakRSS_MB=672.887
KNNQuery/10M/k=8           722 us          721 us           75 PeakRSS_MB=672.34
KNNQuery/10M/k=16         1181 us         1181 us           55 PeakRSS_MB=672.359
KNNQuery/10M/k=32         2327 us         2326 us           31 PeakRSS_MB=672.922
KNNQuery/10M/k=64         5023 us         5022 us           10 PeakRSS_MB=672.359
KNNQuery/10M/k=128       14090 us        14085 us            5 PeakRSS_MB=672.922
```

## Configuration: SFC=PEANO_HILBERT, Decomp=HISTOGRAM, Coords=FLOAT
```
--- SFC Encoding Scaling ---
2026-06-26T07:50:00+02:00
Running ./sfc_encoding_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 11.55, 9.54, 7.77
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------
Benchmark                Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------
SFC-Encode/1K        0.019 ms        0.019 ms         3471 PeakRSS_MB=135.676 items_per_second=51.4239M/s
SFC-Encode/10K       0.020 ms        0.020 ms         3430 PeakRSS_MB=136.801 items_per_second=498.652M/s
SFC-Encode/100K      0.028 ms        0.028 ms         2449 PeakRSS_MB=139.047 items_per_second=3.52416G/s
SFC-Encode/1M        0.108 ms        0.108 ms          628 PeakRSS_MB=169.301 items_per_second=9.2921G/s
SFC-Encode/10M       0.902 ms        0.901 ms           77 PeakRSS_MB=432.027 items_per_second=11.094G/s
--- GPU/Radix Sort Scaling ---
2026-06-26T07:50:13+02:00
Running ./gpu_sort_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 10.23, 9.33, 7.71
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
GPU_Sort/1K        0.043 ms        0.043 ms         1263 PeakRSS_MB=141.82 items_per_second=23.3871M/s
GPU_Sort/10K       0.107 ms        0.107 ms          627 PeakRSS_MB=142.945 items_per_second=93.4932M/s
GPU_Sort/100K      0.323 ms        0.323 ms          215 PeakRSS_MB=148.93 items_per_second=309.495M/s
GPU_Sort/1M         1.10 ms         1.10 ms           62 PeakRSS_MB=186.879 items_per_second=908.466M/s
GPU_Sort/10M        12.9 ms         12.9 ms            5 PeakRSS_MB=285.785 items_per_second=774.28M/s
--- Tree Build Scaling ---
2026-06-26T07:50:21+02:00
Running ./tree_build_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 9.12, 9.12, 7.66
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------------
Benchmark               Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------
TreeBuild/1K         2.51 ms         2.51 ms           26 PeakRSS_MB=137.242 items_per_second=398.725k/s
TreeBuild/10K        4.07 ms         4.06 ms           17 PeakRSS_MB=138.367 items_per_second=2.46138M/s
TreeBuild/100K       7.32 ms         7.32 ms           10 PeakRSS_MB=141.816 items_per_second=13.6642M/s
TreeBuild/1M         27.1 ms         27.1 ms            2 PeakRSS_MB=190.664 items_per_second=36.9584M/s
TreeBuild/10M         231 ms          231 ms            1 PeakRSS_MB=558.305 items_per_second=43.3286M/s
--- Rebuild Scaling ---
2026-06-26T07:50:27+02:00
Running ./rebuild_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 8.63, 9.02, 7.64
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------
TreeRebuild/1K         1.87 ms         1.87 ms           34 PeakRSS_MB=137.781 items_per_second=535.637k/s
TreeRebuild/10K        2.61 ms         2.61 ms           26 PeakRSS_MB=138.906 items_per_second=3.83428M/s
TreeRebuild/100K       5.02 ms         5.01 ms           12 PeakRSS_MB=143.652 items_per_second=19.9415M/s
TreeRebuild/1M         25.6 ms         25.6 ms            2 PeakRSS_MB=191.637 items_per_second=39.1142M/s
TreeRebuild/10M         250 ms          250 ms            1 PeakRSS_MB=632.09 items_per_second=40.0014M/s
--- Range Query Scaling ---
2026-06-26T07:50:32+02:00
Running ./range_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 8.18, 8.92, 7.61
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
RangeQuery/1K/R=0.010000           50.8 us         50.7 us         1084 PeakRSS_MB=137.246 TotalFound=1k
RangeQuery/1K/R=0.100000           50.5 us         50.5 us         1292 PeakRSS_MB=137.246 TotalFound=1.005k
RangeQuery/1K/R=1.000000           60.2 us         60.2 us         1104 PeakRSS_MB=137.246 TotalFound=1.035k
RangeQuery/1K/R=10.000000           107 us          107 us          636 PeakRSS_MB=137.246 TotalFound=6.591k
RangeQuery/1K/R=100.000000          522 us          522 us          134 PeakRSS_MB=137.246 TotalFound=459.76k
RangeQuery/1K/R=200.000000          584 us          584 us          119 PeakRSS_MB=137.246 TotalFound=931.404k
RangeQuery/10K/R=0.010000          60.5 us         60.5 us         1094 PeakRSS_MB=137.809 TotalFound=1k
RangeQuery/10K/R=0.100000          68.7 us         68.6 us          977 PeakRSS_MB=137.809 TotalFound=1.001k
RangeQuery/10K/R=1.000000          87.5 us         87.4 us          766 PeakRSS_MB=137.809 TotalFound=1.295k
RangeQuery/10K/R=10.000000          485 us          485 us          144 PeakRSS_MB=137.809 TotalFound=53.444k
RangeQuery/10K/R=100.000000        6261 us         6259 us           11 PeakRSS_MB=137.809 TotalFound=971.536k
RangeQuery/10K/R=200.000000        5647 us         5645 us           12 PeakRSS_MB=138.371 TotalFound=977.028k
RangeQuery/100K/R=0.010000         77.2 us         77.1 us          867 PeakRSS_MB=142.07 TotalFound=1k
RangeQuery/100K/R=0.100000         91.5 us         91.4 us          740 PeakRSS_MB=141.387 TotalFound=1.005k
RangeQuery/100K/R=1.000000          239 us          238 us          290 PeakRSS_MB=141.941 TotalFound=4.145k
RangeQuery/100K/R=10.000000        3972 us         3970 us           18 PeakRSS_MB=141.941 TotalFound=252.323k
RangeQuery/100K/R=100.000000      67710 us        67701 us            1 PeakRSS_MB=141.91 TotalFound=971.031k
RangeQuery/100K/R=200.000000      82907 us        82890 us            1 PeakRSS_MB=141.348 TotalFound=971.031k
RangeQuery/1M/R=0.010000           82.8 us         82.7 us          803 PeakRSS_MB=186.695 TotalFound=1k
RangeQuery/1M/R=0.100000            144 us          143 us          471 PeakRSS_MB=186.145 TotalFound=1.139k
RangeQuery/1M/R=1.000000           1616 us         1616 us           43 PeakRSS_MB=186.145 TotalFound=35.52k
RangeQuery/1M/R=10.000000         46409 us        46396 us            2 PeakRSS_MB=202.172 TotalFound=688.697k
RangeQuery/1M/R=100.000000       848415 us       848316 us            1 PeakRSS_MB=202.734 TotalFound=981.019k
RangeQuery/1M/R=200.000000      1033635 us      1033512 us            1 PeakRSS_MB=202.172 TotalFound=981.019k
RangeQuery/10M/R=0.010000          94.5 us         94.5 us          570 PeakRSS_MB=558.488 TotalFound=1.001k
RangeQuery/10M/R=0.100000           343 us          343 us          194 PeakRSS_MB=558.508 TotalFound=1.793k
RangeQuery/10M/R=1.000000         14994 us        14992 us            4 PeakRSS_MB=559.07 TotalFound=116.137k
RangeQuery/10M/R=10.000000       496588 us       496513 us            1 PeakRSS_MB=559.07 TotalFound=952.08k
RangeQuery/10M/R=100.000000    11651841 us     11650392 us            1 PeakRSS_MB=558.508 TotalFound=976.025k
RangeQuery/10M/R=200.000000    16560376 us     16558092 us            1 PeakRSS_MB=558.508 TotalFound=976.029k
--- kNN Query Scaling ---
2026-06-26T07:52:26+02:00
Running ./knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 3.78, 7.03, 7.07
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
KNNQuery/1K/k=1           39.5 us         39.5 us         1340 PeakRSS_MB=137.66
KNNQuery/1K/k=2           75.5 us         75.5 us          902 PeakRSS_MB=137.66
KNNQuery/1K/k=4           95.4 us         95.4 us          713 PeakRSS_MB=137.66
KNNQuery/1K/k=8            147 us          147 us          471 PeakRSS_MB=137.66
KNNQuery/1K/k=16           261 us          260 us          267 PeakRSS_MB=137.66
KNNQuery/1K/k=32           611 us          610 us          114 PeakRSS_MB=137.66
KNNQuery/1K/k=64          1671 us         1671 us           42 PeakRSS_MB=137.66
KNNQuery/1K/k=128         4622 us         4621 us           15 PeakRSS_MB=137.66
KNNQuery/10K/k=1          52.7 us         52.7 us         1265 PeakRSS_MB=138.785
KNNQuery/10K/k=2           112 us          112 us          615 PeakRSS_MB=138.785
KNNQuery/10K/k=4           155 us          155 us          445 PeakRSS_MB=138.223
KNNQuery/10K/k=8           231 us          231 us          300 PeakRSS_MB=138.785
KNNQuery/10K/k=16          412 us          411 us          169 PeakRSS_MB=138.785
KNNQuery/10K/k=32          876 us          876 us           80 PeakRSS_MB=138.785
KNNQuery/10K/k=64         2408 us         2408 us           29 PeakRSS_MB=138.785
KNNQuery/10K/k=128        7757 us         7755 us            9 PeakRSS_MB=138.785
KNNQuery/100K/k=1         70.6 us         70.6 us          947 PeakRSS_MB=142.367
KNNQuery/100K/k=2          128 us          128 us          537 PeakRSS_MB=142.93
KNNQuery/100K/k=4          179 us          179 us          387 PeakRSS_MB=142.93
KNNQuery/100K/k=8          254 us          254 us          274 PeakRSS_MB=142.367
KNNQuery/100K/k=16         420 us          420 us          167 PeakRSS_MB=142.93
KNNQuery/100K/k=32         900 us          900 us           78 PeakRSS_MB=142.93
KNNQuery/100K/k=64        2463 us         2462 us           28 PeakRSS_MB=142.93
KNNQuery/100K/k=128       7773 us         7771 us            9 PeakRSS_MB=142.367
KNNQuery/1M/k=1           78.7 us         78.7 us          855 PeakRSS_MB=187.238
KNNQuery/1M/k=2            192 us          192 us          359 PeakRSS_MB=187.242
KNNQuery/1M/k=4            239 us          239 us          291 PeakRSS_MB=186.68
KNNQuery/1M/k=8            303 us          303 us          230 PeakRSS_MB=186.68
KNNQuery/1M/k=16           467 us          466 us          149 PeakRSS_MB=186.68
KNNQuery/1M/k=32          1262 us         1261 us           55 PeakRSS_MB=200.68
KNNQuery/1M/k=64          3161 us         3159 us           22 PeakRSS_MB=186.734
KNNQuery/1M/k=128         9013 us         9011 us            8 PeakRSS_MB=186.742
KNNQuery/10M/k=1          84.8 us         84.8 us          642 PeakRSS_MB=556.395
KNNQuery/10M/k=2           197 us          197 us          308 PeakRSS_MB=557.008
KNNQuery/10M/k=4           293 us          293 us          227 PeakRSS_MB=557.016
KNNQuery/10M/k=8           372 us          372 us          187 PeakRSS_MB=557.02
KNNQuery/10M/k=16          616 us          616 us          113 PeakRSS_MB=557.602
KNNQuery/10M/k=32         1489 us         1488 us           49 PeakRSS_MB=557.602
KNNQuery/10M/k=64         3855 us         3854 us           19 PeakRSS_MB=557.043
KNNQuery/10M/k=128        9623 us         9622 us            6 PeakRSS_MB=557.605
```

## Configuration: SFC=PEANO_HILBERT, Decomp=HISTOGRAM, Coords=DOUBLE
```
--- SFC Encoding Scaling ---
2026-06-26T07:58:23+02:00
Running ./sfc_encoding_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 12.89, 10.74, 8.48
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------
Benchmark                Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------
SFC-Encode/1K        0.020 ms        0.020 ms         3453 PeakRSS_MB=134.973 items_per_second=50.9275M/s
SFC-Encode/10K       0.020 ms        0.020 ms         3407 PeakRSS_MB=135.91 items_per_second=496.716M/s
SFC-Encode/100K      0.028 ms        0.028 ms         2438 PeakRSS_MB=138.145 items_per_second=3.5122G/s
SFC-Encode/1M        0.109 ms        0.109 ms          619 PeakRSS_MB=168.035 items_per_second=9.16074G/s
SFC-Encode/10M       0.906 ms        0.906 ms           77 PeakRSS_MB=430.164 items_per_second=11.0348G/s
--- GPU/Radix Sort Scaling ---
2026-06-26T07:58:36+02:00
Running ./gpu_sort_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 10.70, 10.36, 8.39
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
GPU_Sort/1K        0.043 ms        0.043 ms         1233 PeakRSS_MB=139.566 items_per_second=23.3223M/s
GPU_Sort/10K       0.109 ms        0.109 ms          622 PeakRSS_MB=140.691 items_per_second=92.0993M/s
GPU_Sort/100K      0.319 ms        0.319 ms          215 PeakRSS_MB=148.004 items_per_second=313.264M/s
GPU_Sort/1M         1.10 ms         1.10 ms           63 PeakRSS_MB=196.145 items_per_second=907.493M/s
GPU_Sort/10M        13.4 ms         13.4 ms            5 PeakRSS_MB=398.082 items_per_second=747.977M/s
--- Tree Build Scaling ---
2026-06-26T07:58:44+02:00
Running ./tree_build_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 9.52, 10.11, 8.33
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------------
Benchmark               Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------
TreeBuild/1K         2.54 ms         2.54 ms           25 PeakRSS_MB=136.66 items_per_second=393.592k/s
TreeBuild/10K        3.54 ms         3.53 ms           20 PeakRSS_MB=137.785 items_per_second=2.8321M/s
TreeBuild/100K       7.40 ms         7.40 ms            8 PeakRSS_MB=143.309 items_per_second=13.5211M/s
TreeBuild/1M         39.0 ms         39.0 ms            2 PeakRSS_MB=204.062 items_per_second=25.6582M/s
TreeBuild/10M         326 ms          326 ms            1 PeakRSS_MB=662.395 items_per_second=30.7155M/s
--- Rebuild Scaling ---
2026-06-26T07:58:50+02:00
Running ./rebuild_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 9.00, 10.00, 8.31
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------
TreeRebuild/1K         1.95 ms         1.95 ms           32 PeakRSS_MB=136.664 items_per_second=512.365k/s
TreeRebuild/10K        2.60 ms         2.60 ms           25 PeakRSS_MB=138.352 items_per_second=3.84571M/s
TreeRebuild/100K       6.90 ms         6.89 ms           10 PeakRSS_MB=144.211 items_per_second=14.5086M/s
TreeRebuild/1M         33.1 ms         33.0 ms            2 PeakRSS_MB=203.277 items_per_second=30.2679M/s
TreeRebuild/10M         315 ms          314 ms            1 PeakRSS_MB=784.324 items_per_second=31.8211M/s
--- Range Query Scaling ---
2026-06-26T07:58:55+02:00
Running ./range_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 8.51, 9.88, 8.28
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
RangeQuery/1K/R=0.010000           77.7 us         77.7 us          764 PeakRSS_MB=137.773 TotalFound=1k
RangeQuery/1K/R=0.100000           77.6 us         77.5 us          861 PeakRSS_MB=137.711 TotalFound=1.005k
RangeQuery/1K/R=1.000000           96.6 us         96.6 us          699 PeakRSS_MB=137.711 TotalFound=1.035k
RangeQuery/1K/R=10.000000           188 us          188 us          368 PeakRSS_MB=138.211 TotalFound=6.591k
RangeQuery/1K/R=100.000000          981 us          980 us           71 PeakRSS_MB=137.648 TotalFound=459.76k
RangeQuery/1K/R=200.000000         1144 us         1144 us           61 PeakRSS_MB=137.586 TotalFound=931.404k
RangeQuery/10K/R=0.010000          99.1 us         99.0 us          683 PeakRSS_MB=138.648 TotalFound=1k
RangeQuery/10K/R=0.100000           118 us          118 us          562 PeakRSS_MB=139.211 TotalFound=1.001k
RangeQuery/10K/R=1.000000           158 us          158 us          435 PeakRSS_MB=139.148 TotalFound=1.295k
RangeQuery/10K/R=10.000000          990 us          990 us           71 PeakRSS_MB=139.148 TotalFound=53.444k
RangeQuery/10K/R=100.000000       14997 us        14994 us            5 PeakRSS_MB=139.148 TotalFound=971.536k
RangeQuery/10K/R=200.000000       13564 us        13563 us            5 PeakRSS_MB=139.086 TotalFound=977.028k
RangeQuery/100K/R=0.010000          131 us          131 us          519 PeakRSS_MB=143.809 TotalFound=1k
RangeQuery/100K/R=0.100000          162 us          162 us          421 PeakRSS_MB=144.309 TotalFound=1.005k
RangeQuery/100K/R=1.000000          505 us          505 us          138 PeakRSS_MB=143.738 TotalFound=4.145k
RangeQuery/100K/R=10.000000        9277 us         9275 us            8 PeakRSS_MB=143.676 TotalFound=252.323k
RangeQuery/100K/R=100.000000     153401 us       153379 us            1 PeakRSS_MB=143.676 TotalFound=971.031k
RangeQuery/100K/R=200.000000     197659 us       197613 us            1 PeakRSS_MB=144.238 TotalFound=971.031k
RangeQuery/1M/R=0.010000            142 us          142 us          480 PeakRSS_MB=199.043 TotalFound=1k
RangeQuery/1M/R=0.100000            217 us          217 us          318 PeakRSS_MB=198.441 TotalFound=1.139k
RangeQuery/1M/R=1.000000           2803 us         2802 us           25 PeakRSS_MB=198.441 TotalFound=35.52k
RangeQuery/1M/R=10.000000        114865 us       114847 us            1 PeakRSS_MB=198.441 TotalFound=688.697k
RangeQuery/1M/R=100.000000      2425807 us      2425478 us            1 PeakRSS_MB=203.309 TotalFound=981.019k
RangeQuery/1M/R=200.000000      3023704 us      3023319 us            1 PeakRSS_MB=203.309 TotalFound=981.019k
RangeQuery/10M/R=0.010000           163 us          163 us          329 PeakRSS_MB=672.629 TotalFound=1.001k
RangeQuery/10M/R=0.100000           664 us          664 us           86 PeakRSS_MB=672.668 TotalFound=1.793k
RangeQuery/10M/R=1.000000         31595 us        31589 us            2 PeakRSS_MB=672.68 TotalFound=116.137k
RangeQuery/10M/R=10.000000      1445065 us      1444867 us            1 PeakRSS_MB=672.688 TotalFound=952.08k
RangeQuery/10M/R=100.000000    30773396 us     30769658 us            1 PeakRSS_MB=673.25 TotalFound=976.025k
RangeQuery/10M/R=200.000000    45634745 us     45629034 us            1 PeakRSS_MB=673.262 TotalFound=976.029k
--- kNN Query Scaling ---
2026-06-26T08:02:33+02:00
Running ./knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 3.22, 6.36, 7.18
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
KNNQuery/1K/k=1           59.9 us         59.9 us          962 PeakRSS_MB=136.145
KNNQuery/1K/k=2            128 us          128 us          541 PeakRSS_MB=136.082
KNNQuery/1K/k=4            160 us          160 us          430 PeakRSS_MB=135.52
KNNQuery/1K/k=8            240 us          240 us          290 PeakRSS_MB=136.02
KNNQuery/1K/k=16           391 us          391 us          179 PeakRSS_MB=136.02
KNNQuery/1K/k=32           840 us          839 us           83 PeakRSS_MB=135.957
KNNQuery/1K/k=64          2148 us         2147 us           33 PeakRSS_MB=135.957
KNNQuery/1K/k=128         5672 us         5671 us           12 PeakRSS_MB=135.332
KNNQuery/10K/k=1          88.0 us         88.0 us          768 PeakRSS_MB=136.457
KNNQuery/10K/k=2           201 us          201 us          346 PeakRSS_MB=136.395
KNNQuery/10K/k=4           284 us          284 us          245 PeakRSS_MB=136.957
KNNQuery/10K/k=8           408 us          408 us          171 PeakRSS_MB=137.457
KNNQuery/10K/k=16          685 us          685 us          101 PeakRSS_MB=136.895
KNNQuery/10K/k=32         1357 us         1357 us           52 PeakRSS_MB=136.832
KNNQuery/10K/k=64         3581 us         3579 us           20 PeakRSS_MB=137.395
KNNQuery/10K/k=128       11067 us        11065 us            6 PeakRSS_MB=137.395
KNNQuery/100K/k=1          113 us          113 us          605 PeakRSS_MB=140.824
KNNQuery/100K/k=2          220 us          220 us          316 PeakRSS_MB=140.879
KNNQuery/100K/k=4          315 us          315 us          223 PeakRSS_MB=141.445
KNNQuery/100K/k=8          432 us          432 us          162 PeakRSS_MB=140.891
KNNQuery/100K/k=16         676 us          675 us          103 PeakRSS_MB=140.887
KNNQuery/100K/k=32        1370 us         1370 us           51 PeakRSS_MB=140.887
KNNQuery/100K/k=64        3609 us         3607 us           19 PeakRSS_MB=140.887
KNNQuery/100K/k=128      10740 us        10737 us            6 PeakRSS_MB=141.449
KNNQuery/1M/k=1            128 us          128 us          533 PeakRSS_MB=196.492
KNNQuery/1M/k=2            268 us          268 us          259 PeakRSS_MB=196.496
KNNQuery/1M/k=4            370 us          370 us          189 PeakRSS_MB=197.059
KNNQuery/1M/k=8            490 us          490 us          139 PeakRSS_MB=197.059
KNNQuery/1M/k=16           857 us          856 us           80 PeakRSS_MB=197.07
KNNQuery/1M/k=32          2334 us         2334 us           30 PeakRSS_MB=197.074
KNNQuery/1M/k=64          5831 us         5829 us           12 PeakRSS_MB=197.078
KNNQuery/1M/k=128        13750 us        13745 us            5 PeakRSS_MB=197.078
KNNQuery/10M/k=1           136 us          136 us          491 PeakRSS_MB=669.895
KNNQuery/10M/k=2           312 us          312 us          202 PeakRSS_MB=669.898
KNNQuery/10M/k=4           459 us          459 us          110 PeakRSS_MB=669.934
KNNQuery/10M/k=8           730 us          730 us           76 PeakRSS_MB=669.957
KNNQuery/10M/k=16         1198 us         1198 us           54 PeakRSS_MB=669.957
KNNQuery/10M/k=32         2184 us         2183 us           31 PeakRSS_MB=669.957
KNNQuery/10M/k=64         5035 us         5034 us           13 PeakRSS_MB=669.973
KNNQuery/10M/k=128       14464 us        14460 us            5 PeakRSS_MB=669.973
```

## Configuration: SFC=PEANO_HILBERT, Decomp=SAMPLING, Coords=FLOAT
```
--- SFC Encoding Scaling ---
2026-06-26T08:08:54+02:00
Running ./sfc_encoding_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 10.41, 10.28, 8.54
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------
Benchmark                Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------
SFC-Encode/1K        0.020 ms        0.020 ms         3465 PeakRSS_MB=137.785 items_per_second=51.2609M/s
SFC-Encode/10K       0.020 ms        0.020 ms         3428 PeakRSS_MB=138.348 items_per_second=498.158M/s
SFC-Encode/100K      0.028 ms        0.028 ms         2450 PeakRSS_MB=140.863 items_per_second=3.52701G/s
SFC-Encode/1M        0.108 ms        0.108 ms          627 PeakRSS_MB=171.539 items_per_second=9.288G/s
SFC-Encode/10M       0.902 ms        0.901 ms           77 PeakRSS_MB=433.773 items_per_second=11.0966G/s
--- GPU/Radix Sort Scaling ---
2026-06-26T08:09:06+02:00
Running ./gpu_sort_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 8.84, 9.94, 8.46
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
GPU_Sort/1K        0.045 ms        0.045 ms         1213 PeakRSS_MB=141.801 items_per_second=22.4544M/s
GPU_Sort/10K       0.110 ms        0.110 ms          612 PeakRSS_MB=142.926 items_per_second=91.2622M/s
GPU_Sort/100K      0.323 ms        0.322 ms          213 PeakRSS_MB=149.113 items_per_second=310.248M/s
GPU_Sort/1M         1.15 ms         1.15 ms           60 PeakRSS_MB=186.836 items_per_second=866.389M/s
GPU_Sort/10M        13.3 ms         13.3 ms            5 PeakRSS_MB=286.707 items_per_second=750.239M/s
--- Tree Build Scaling ---
2026-06-26T08:09:15+02:00
Running ./tree_build_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 7.94, 9.71, 8.40
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------------
Benchmark               Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------
TreeBuild/1K         2.45 ms         2.45 ms           27 PeakRSS_MB=137.227 items_per_second=408.845k/s
TreeBuild/10K        4.00 ms         4.00 ms           18 PeakRSS_MB=138.914 items_per_second=2.50108M/s
TreeBuild/100K       7.04 ms         7.03 ms           10 PeakRSS_MB=142.449 items_per_second=14.2313M/s
TreeBuild/1M         25.1 ms         25.1 ms            3 PeakRSS_MB=190.785 items_per_second=39.9116M/s
TreeBuild/10M         231 ms          231 ms            1 PeakRSS_MB=559.117 items_per_second=43.2811M/s
--- Rebuild Scaling ---
2026-06-26T08:09:20+02:00
Running ./rebuild_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 7.54, 9.60, 8.37
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------
TreeRebuild/1K         2.50 ms         2.50 ms           26 PeakRSS_MB=137.391 items_per_second=399.801k/s
TreeRebuild/10K        4.04 ms         4.03 ms           17 PeakRSS_MB=138.516 items_per_second=2.47882M/s
TreeRebuild/100K       5.23 ms         5.23 ms           10 PeakRSS_MB=142.691 items_per_second=19.1349M/s
TreeRebuild/1M         33.3 ms         30.1 ms            3 PeakRSS_MB=195.027 items_per_second=33.2236M/s
TreeRebuild/10M         200 ms          200 ms            1 PeakRSS_MB=636.828 items_per_second=49.908M/s
--- Range Query Scaling ---
2026-06-26T08:09:25+02:00
Running ./range_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 7.18, 9.49, 8.34
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
RangeQuery/1K/R=0.010000           51.2 us         51.2 us         1082 PeakRSS_MB=137.773 TotalFound=1k
RangeQuery/1K/R=0.100000           51.0 us         51.0 us         1280 PeakRSS_MB=137.773 TotalFound=1.005k
RangeQuery/1K/R=1.000000           60.7 us         60.7 us         1096 PeakRSS_MB=137.211 TotalFound=1.035k
RangeQuery/1K/R=10.000000           107 us          107 us          635 PeakRSS_MB=137.773 TotalFound=6.591k
RangeQuery/1K/R=100.000000          522 us          522 us          134 PeakRSS_MB=137.773 TotalFound=459.76k
RangeQuery/1K/R=200.000000          584 us          584 us          119 PeakRSS_MB=137.773 TotalFound=931.404k
RangeQuery/10K/R=0.010000          61.0 us         61.0 us         1087 PeakRSS_MB=138.336 TotalFound=1k
RangeQuery/10K/R=0.100000          69.3 us         69.3 us          969 PeakRSS_MB=138.336 TotalFound=1.001k
RangeQuery/10K/R=1.000000          87.9 us         87.9 us          762 PeakRSS_MB=138.336 TotalFound=1.295k
RangeQuery/10K/R=10.000000          486 us          485 us          144 PeakRSS_MB=138.336 TotalFound=53.444k
RangeQuery/10K/R=100.000000        6261 us         6259 us           11 PeakRSS_MB=138.336 TotalFound=971.536k
RangeQuery/10K/R=200.000000        5642 us         5641 us           12 PeakRSS_MB=138.336 TotalFound=977.028k
RangeQuery/100K/R=0.010000         77.7 us         77.7 us          860 PeakRSS_MB=141.914 TotalFound=1k
RangeQuery/100K/R=0.100000         91.8 us         91.8 us          734 PeakRSS_MB=141.918 TotalFound=1.005k
RangeQuery/100K/R=1.000000          239 us          239 us          290 PeakRSS_MB=141.914 TotalFound=4.145k
RangeQuery/100K/R=10.000000        3980 us         3979 us           18 PeakRSS_MB=141.914 TotalFound=252.323k
RangeQuery/100K/R=100.000000      67756 us        67748 us            1 PeakRSS_MB=141.914 TotalFound=971.031k
RangeQuery/100K/R=200.000000      82966 us        82952 us            1 PeakRSS_MB=141.914 TotalFound=971.031k
RangeQuery/1M/R=0.010000           83.4 us         83.4 us          799 PeakRSS_MB=186.328 TotalFound=1k
RangeQuery/1M/R=0.100000            144 us          144 us          470 PeakRSS_MB=186.328 TotalFound=1.139k
RangeQuery/1M/R=1.000000           1618 us         1618 us           43 PeakRSS_MB=186.375 TotalFound=35.52k
RangeQuery/1M/R=10.000000         46412 us        46405 us            2 PeakRSS_MB=186.383 TotalFound=688.697k
RangeQuery/1M/R=100.000000       852322 us       852208 us            1 PeakRSS_MB=186.395 TotalFound=981.019k
RangeQuery/1M/R=200.000000      1037994 us      1037859 us            1 PeakRSS_MB=186.398 TotalFound=981.019k
RangeQuery/10M/R=0.010000          94.9 us         94.9 us          670 PeakRSS_MB=555.684 TotalFound=1.001k
RangeQuery/10M/R=0.100000           281 us          281 us          243 PeakRSS_MB=556.266 TotalFound=1.793k
RangeQuery/10M/R=1.000000         14276 us        14272 us            6 PeakRSS_MB=556.266 TotalFound=116.137k
RangeQuery/10M/R=10.000000       496602 us       496534 us            1 PeakRSS_MB=556.266 TotalFound=952.08k
RangeQuery/10M/R=100.000000    11676549 us     11674958 us            1 PeakRSS_MB=556.266 TotalFound=976.025k
RangeQuery/10M/R=200.000000    16572883 us     16570742 us            1 PeakRSS_MB=556.266 TotalFound=976.029k
--- kNN Query Scaling ---
2026-06-26T08:11:19+02:00
Running ./knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 3.61, 7.41, 7.71
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
KNNQuery/1K/k=1           39.5 us         39.4 us         1339 PeakRSS_MB=138.293
KNNQuery/1K/k=2           75.5 us         75.5 us          903 PeakRSS_MB=138.293
KNNQuery/1K/k=4           95.4 us         95.4 us          714 PeakRSS_MB=138.293
KNNQuery/1K/k=8            147 us          147 us          471 PeakRSS_MB=138.293
KNNQuery/1K/k=16           261 us          260 us          267 PeakRSS_MB=138.293
KNNQuery/1K/k=32           611 us          611 us          114 PeakRSS_MB=138.293
KNNQuery/1K/k=64          1671 us         1670 us           42 PeakRSS_MB=138.293
KNNQuery/1K/k=128         4624 us         4623 us           15 PeakRSS_MB=138.293
KNNQuery/10K/k=1          53.7 us         53.7 us         1223 PeakRSS_MB=138.855
KNNQuery/10K/k=2           113 us          113 us          610 PeakRSS_MB=139.418
KNNQuery/10K/k=4           155 us          155 us          445 PeakRSS_MB=139.418
KNNQuery/10K/k=8           230 us          230 us          301 PeakRSS_MB=139.418
KNNQuery/10K/k=16          407 us          407 us          171 PeakRSS_MB=139.418
KNNQuery/10K/k=32          874 us          873 us           80 PeakRSS_MB=139.418
KNNQuery/10K/k=64         2378 us         2377 us           29 PeakRSS_MB=139.418
KNNQuery/10K/k=128        7626 us         7625 us            9 PeakRSS_MB=139.418
KNNQuery/100K/k=1         70.6 us         70.6 us          959 PeakRSS_MB=142.777
KNNQuery/100K/k=2          128 us          128 us          535 PeakRSS_MB=143.34
KNNQuery/100K/k=4          180 us          180 us          386 PeakRSS_MB=142.777
KNNQuery/100K/k=8          254 us          254 us          273 PeakRSS_MB=142.832
KNNQuery/100K/k=16         427 us          427 us          164 PeakRSS_MB=143.422
KNNQuery/100K/k=32         920 us          920 us           76 PeakRSS_MB=143.395
KNNQuery/100K/k=64        2444 us         2443 us           29 PeakRSS_MB=143.395
KNNQuery/100K/k=128       7677 us         7676 us            9 PeakRSS_MB=142.832
KNNQuery/1M/k=1           78.6 us         78.6 us          855 PeakRSS_MB=188.383
KNNQuery/1M/k=2            192 us          192 us          359 PeakRSS_MB=188.383
KNNQuery/1M/k=4            267 us          267 us          260 PeakRSS_MB=188.383
KNNQuery/1M/k=8            321 us          321 us          217 PeakRSS_MB=188.961
KNNQuery/1M/k=16           598 us          598 us          115 PeakRSS_MB=188.414
KNNQuery/1M/k=32          1277 us         1277 us           54 PeakRSS_MB=188.414
KNNQuery/1M/k=64          2883 us         2882 us           24 PeakRSS_MB=188.414
KNNQuery/1M/k=128         9111 us         9108 us            8 PeakRSS_MB=188.414
KNNQuery/10M/k=1          97.9 us         97.9 us          639 PeakRSS_MB=557.281
KNNQuery/10M/k=2           170 us          170 us          394 PeakRSS_MB=557.289
KNNQuery/10M/k=4           266 us          266 us          270 PeakRSS_MB=557.289
KNNQuery/10M/k=8           433 us          433 us          116 PeakRSS_MB=557.852
KNNQuery/10M/k=16          666 us          666 us           96 PeakRSS_MB=557.289
KNNQuery/10M/k=32         1399 us         1399 us           43 PeakRSS_MB=557.289
KNNQuery/10M/k=64         3119 us         3118 us           22 PeakRSS_MB=557.289
KNNQuery/10M/k=128        9841 us         9840 us            7 PeakRSS_MB=557.289
```

## Configuration: SFC=PEANO_HILBERT, Decomp=SAMPLING, Coords=DOUBLE
```
--- SFC Encoding Scaling ---
2026-06-26T08:16:33+02:00
Running ./sfc_encoding_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 10.04, 9.64, 8.48
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------
Benchmark                Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------
SFC-Encode/1K        0.020 ms        0.020 ms         3446 PeakRSS_MB=136.09 items_per_second=50.978M/s
SFC-Encode/10K       0.020 ms        0.020 ms         3422 PeakRSS_MB=136.465 items_per_second=494.853M/s
SFC-Encode/100K      0.029 ms        0.029 ms         2428 PeakRSS_MB=139.941 items_per_second=3.50452G/s
SFC-Encode/1M        0.109 ms        0.109 ms          619 PeakRSS_MB=169.418 items_per_second=9.14626G/s
SFC-Encode/10M       0.907 ms        0.907 ms           77 PeakRSS_MB=431.305 items_per_second=11.0259G/s
--- GPU/Radix Sort Scaling ---
2026-06-26T08:16:45+02:00
Running ./gpu_sort_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 8.48, 9.31, 8.39
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
GPU_Sort/1K        0.043 ms        0.043 ms         1233 PeakRSS_MB=139.566 items_per_second=23.368M/s
GPU_Sort/10K       0.108 ms        0.108 ms          621 PeakRSS_MB=140.691 items_per_second=92.783M/s
GPU_Sort/100K      0.325 ms        0.324 ms          213 PeakRSS_MB=148.004 items_per_second=308.499M/s
GPU_Sort/1M         1.32 ms         1.32 ms           62 PeakRSS_MB=202.086 items_per_second=757.134M/s
GPU_Sort/10M        13.4 ms         13.4 ms            5 PeakRSS_MB=398.93 items_per_second=746.909M/s
--- Tree Build Scaling ---
2026-06-26T08:16:54+02:00
Running ./tree_build_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 8.04, 9.21, 8.36
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------------
Benchmark               Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------
TreeBuild/1K         2.31 ms         2.31 ms           28 PeakRSS_MB=138.676 items_per_second=433.057k/s
TreeBuild/10K        3.25 ms         3.24 ms           22 PeakRSS_MB=139.801 items_per_second=3.08739M/s
TreeBuild/100K       6.91 ms         6.90 ms           10 PeakRSS_MB=144.781 items_per_second=14.4895M/s
TreeBuild/1M         40.6 ms         40.6 ms            2 PeakRSS_MB=200.449 items_per_second=24.6518M/s
TreeBuild/10M         320 ms          320 ms            1 PeakRSS_MB=675.695 items_per_second=31.2347M/s
--- Rebuild Scaling ---
2026-06-26T08:16:59+02:00
Running ./rebuild_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 7.43, 9.04, 8.31
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------
TreeRebuild/1K         2.50 ms         2.50 ms           26 PeakRSS_MB=136.203 items_per_second=400.288k/s
TreeRebuild/10K        3.42 ms         3.42 ms           20 PeakRSS_MB=137.328 items_per_second=2.92816M/s
TreeRebuild/100K       8.84 ms         8.84 ms            8 PeakRSS_MB=143.953 items_per_second=11.3183M/s
TreeRebuild/1M         34.8 ms         34.8 ms            2 PeakRSS_MB=204.258 items_per_second=28.772M/s
TreeRebuild/10M         319 ms          318 ms            1 PeakRSS_MB=786.363 items_per_second=31.421M/s
--- Range Query Scaling ---
2026-06-26T08:17:05+02:00
Running ./range_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 7.15, 8.95, 8.29
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
RangeQuery/1K/R=0.010000           77.5 us         77.5 us          766 PeakRSS_MB=136.188 TotalFound=1k
RangeQuery/1K/R=0.100000           77.5 us         77.5 us          865 PeakRSS_MB=136.125 TotalFound=1.005k
RangeQuery/1K/R=1.000000           96.5 us         96.4 us          702 PeakRSS_MB=135.562 TotalFound=1.035k
RangeQuery/1K/R=10.000000           188 us          188 us          368 PeakRSS_MB=136.062 TotalFound=6.591k
RangeQuery/1K/R=100.000000          980 us          980 us           71 PeakRSS_MB=136.062 TotalFound=459.76k
RangeQuery/1K/R=200.000000         1144 us         1144 us           61 PeakRSS_MB=136 TotalFound=931.404k
RangeQuery/10K/R=0.010000          98.9 us         98.8 us          683 PeakRSS_MB=137.125 TotalFound=1k
RangeQuery/10K/R=0.100000           117 us          117 us          581 PeakRSS_MB=137.625 TotalFound=1.001k
RangeQuery/10K/R=1.000000           158 us          158 us          437 PeakRSS_MB=137.062 TotalFound=1.295k
RangeQuery/10K/R=10.000000          989 us          989 us           71 PeakRSS_MB=137.562 TotalFound=53.444k
RangeQuery/10K/R=100.000000       14997 us        14994 us            5 PeakRSS_MB=137.562 TotalFound=971.536k
RangeQuery/10K/R=200.000000       13562 us        13559 us            5 PeakRSS_MB=137.562 TotalFound=977.028k
RangeQuery/100K/R=0.010000          132 us          132 us          520 PeakRSS_MB=141.293 TotalFound=1k
RangeQuery/100K/R=0.100000          163 us          163 us          422 PeakRSS_MB=141.223 TotalFound=1.005k
RangeQuery/100K/R=1.000000          506 us          506 us          138 PeakRSS_MB=141.785 TotalFound=4.145k
RangeQuery/100K/R=10.000000        9288 us         9284 us            8 PeakRSS_MB=141.223 TotalFound=252.323k
RangeQuery/100K/R=100.000000     153392 us       153371 us            1 PeakRSS_MB=141.785 TotalFound=971.031k
RangeQuery/100K/R=200.000000     197660 us       197635 us            1 PeakRSS_MB=141.16 TotalFound=971.031k
RangeQuery/1M/R=0.010000            141 us          141 us          481 PeakRSS_MB=197.152 TotalFound=1k
RangeQuery/1M/R=0.100000            217 us          217 us          317 PeakRSS_MB=197.199 TotalFound=1.139k
RangeQuery/1M/R=1.000000           2804 us         2802 us           25 PeakRSS_MB=197.199 TotalFound=35.52k
RangeQuery/1M/R=10.000000        115111 us       115094 us            1 PeakRSS_MB=197.137 TotalFound=688.697k
RangeQuery/1M/R=100.000000      2431406 us      2431063 us            1 PeakRSS_MB=197.137 TotalFound=981.019k
RangeQuery/1M/R=200.000000      3037253 us      3036858 us            1 PeakRSS_MB=197.699 TotalFound=981.019k
RangeQuery/10M/R=0.010000           163 us          163 us          406 PeakRSS_MB=669.93 TotalFound=1.001k
RangeQuery/10M/R=0.100000           624 us          624 us           91 PeakRSS_MB=669.938 TotalFound=1.793k
RangeQuery/10M/R=1.000000         31599 us        31593 us            2 PeakRSS_MB=670.504 TotalFound=116.137k
RangeQuery/10M/R=10.000000      1444026 us      1443812 us            1 PeakRSS_MB=670.504 TotalFound=952.08k
RangeQuery/10M/R=100.000000    30692737 us     30688935 us            1 PeakRSS_MB=669.941 TotalFound=976.025k
RangeQuery/10M/R=200.000000    45642433 us     45636955 us            1 PeakRSS_MB=669.941 TotalFound=976.029k
--- kNN Query Scaling ---
2026-06-26T08:20:43+02:00
Running ./knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 3.17, 5.92, 7.19
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
KNNQuery/1K/k=1           59.8 us         59.8 us          964 PeakRSS_MB=137.176
KNNQuery/1K/k=2            128 us          128 us          542 PeakRSS_MB=137.676
KNNQuery/1K/k=4            160 us          160 us          431 PeakRSS_MB=137.676
KNNQuery/1K/k=8            240 us          240 us          290 PeakRSS_MB=138.176
KNNQuery/1K/k=16           391 us          390 us          179 PeakRSS_MB=137.613
KNNQuery/1K/k=32           839 us          839 us           83 PeakRSS_MB=138.113
KNNQuery/1K/k=64          2142 us         2141 us           33 PeakRSS_MB=137.551
KNNQuery/1K/k=128         5674 us         5673 us           12 PeakRSS_MB=137.488
KNNQuery/10K/k=1          87.7 us         87.6 us          778 PeakRSS_MB=138.613
KNNQuery/10K/k=2           201 us          201 us          346 PeakRSS_MB=138.551
KNNQuery/10K/k=4           284 us          284 us          246 PeakRSS_MB=138.551
KNNQuery/10K/k=8           406 us          406 us          172 PeakRSS_MB=138.488
KNNQuery/10K/k=16          685 us          685 us          101 PeakRSS_MB=138.488
KNNQuery/10K/k=32         1352 us         1352 us           52 PeakRSS_MB=138.426
KNNQuery/10K/k=64         3520 us         3519 us           20 PeakRSS_MB=138.426
KNNQuery/10K/k=128       10924 us        10922 us            6 PeakRSS_MB=138.426
KNNQuery/100K/k=1          113 us          113 us          600 PeakRSS_MB=143.145
KNNQuery/100K/k=2          219 us          219 us          316 PeakRSS_MB=143.152
KNNQuery/100K/k=4          315 us          314 us          222 PeakRSS_MB=142.777
KNNQuery/100K/k=8          432 us          432 us          162 PeakRSS_MB=142.777
KNNQuery/100K/k=16         675 us          675 us          103 PeakRSS_MB=142.777
KNNQuery/100K/k=32        1390 us         1390 us           50 PeakRSS_MB=142.777
KNNQuery/100K/k=64        3580 us         3580 us           20 PeakRSS_MB=142.777
KNNQuery/100K/k=128      10654 us        10652 us            7 PeakRSS_MB=142.777
KNNQuery/1M/k=1            128 us          128 us          532 PeakRSS_MB=198.516
KNNQuery/1M/k=2            342 us          341 us          204 PeakRSS_MB=197.953
KNNQuery/1M/k=4            378 us          378 us          185 PeakRSS_MB=197.98
KNNQuery/1M/k=8            488 us          488 us          140 PeakRSS_MB=197.98
KNNQuery/1M/k=16           862 us          862 us           79 PeakRSS_MB=197.988
KNNQuery/1M/k=32          1886 us         1886 us           37 PeakRSS_MB=198.555
KNNQuery/1M/k=64          4628 us         4627 us           15 PeakRSS_MB=197.996
KNNQuery/1M/k=128        13668 us        13665 us            5 PeakRSS_MB=198.562
KNNQuery/10M/k=1           136 us          136 us          495 PeakRSS_MB=662.219
KNNQuery/10M/k=2           349 us          349 us          210 PeakRSS_MB=662.234
KNNQuery/10M/k=4           456 us          456 us          110 PeakRSS_MB=662.289
KNNQuery/10M/k=8           723 us          723 us           77 PeakRSS_MB=663.012
KNNQuery/10M/k=16         1189 us         1189 us           54 PeakRSS_MB=670.434
KNNQuery/10M/k=32         2170 us         2170 us           32 PeakRSS_MB=669.871
KNNQuery/10M/k=64         4981 us         4980 us           12 PeakRSS_MB=670.434
KNNQuery/10M/k=128       14879 us        14877 us            5 PeakRSS_MB=669.875
```

