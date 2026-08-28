# Scaling Benchmark Results: cpu_int64 (Periodic BC: OFF)
Date: Fri Aug  7 12:54:09 CEST 2026
SFC Curve: Peano-Hilbert
Periodic BC: OFF
Build Directory: build_int64
```
--- Executing ./sfc_encoding_scaling.exe ---
2026-08-07T12:54:09+02:00
Running ./sfc_encoding_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 2.51, 11.62, 13.05
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------
Benchmark                Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------
SFC-Encode/1K        0.050 ms        0.046 ms        15276 PeakRSS_MB=188.203 items_per_second=21.8969M/s
SFC-Encode/10K       0.061 ms        0.052 ms        10000 PeakRSS_MB=187.535 items_per_second=193.27M/s
SFC-Encode/100K      0.173 ms        0.155 ms         4321 PeakRSS_MB=180.824 items_per_second=646.398M/s
SFC-Encode/1M         1.22 ms         1.20 ms          578 PeakRSS_MB=266.461 items_per_second=835.545M/s
SFC-Encode/10M        11.4 ms         11.4 ms           61 PeakRSS_MB=1.15071k items_per_second=874.886M/s

--- Executing ./gpu_sort_scaling.exe ---
2026-08-07T12:54:31+02:00
Running ./gpu_sort_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 12.10, 13.06, 13.49
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
GPU_Sort/1K        0.273 ms        0.200 ms         3454 PeakRSS_MB=179.402 items_per_second=4.99879M/s
GPU_Sort/10K       0.541 ms        0.358 ms         1948 PeakRSS_MB=176.055 items_per_second=27.915M/s
GPU_Sort/100K       1.43 ms         1.04 ms          665 PeakRSS_MB=185.828 items_per_second=96.2475M/s
GPU_Sort/1M         10.3 ms         9.65 ms          130 PeakRSS_MB=312.758 items_per_second=103.658M/s
GPU_Sort/10M         127 ms          126 ms            6 PeakRSS_MB=1.12181k items_per_second=79.2091M/s

--- Executing ./tree_build_scaling.exe ---
2026-08-07T12:54:53+02:00
Running ./tree_build_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 13.76, 13.42, 13.60
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------------
Benchmark               Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------
TreeBuild/1K         1.45 ms         1.36 ms          507 PeakRSS_MB=166.469 items_per_second=735.201k/s
TreeBuild/10K        2.59 ms         2.29 ms          307 PeakRSS_MB=163.223 items_per_second=4.3668M/s
TreeBuild/100K       7.10 ms         6.47 ms          105 PeakRSS_MB=166.781 items_per_second=15.4631M/s
TreeBuild/1M         34.6 ms         33.7 ms           21 PeakRSS_MB=229.355 items_per_second=29.693M/s
TreeBuild/10M         277 ms          274 ms            3 PeakRSS_MB=705.105 items_per_second=36.4976M/s

--- Executing ./rebuild_scaling.exe ---
2026-08-07T12:55:11+02:00
Running ./rebuild_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 20.64, 14.93, 14.09
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------
TreeRebuild/1K         1.51 ms         1.40 ms          486 PeakRSS_MB=175.094 items_per_second=716.022k/s
TreeRebuild/10K        2.61 ms         2.29 ms          304 PeakRSS_MB=172.836 items_per_second=4.36794M/s
TreeRebuild/100K       7.50 ms         6.82 ms          103 PeakRSS_MB=175.82 items_per_second=14.661M/s
TreeRebuild/1M         33.9 ms         32.8 ms           21 PeakRSS_MB=256.211 items_per_second=30.5263M/s
TreeRebuild/10M         264 ms          261 ms            3 PeakRSS_MB=1.01224k items_per_second=38.3151M/s

--- Executing ./range_query_scaling.exe ---
2026-08-07T12:55:30+02:00
Running ./range_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 21.52, 15.41, 14.26
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
RangeQuery/1K/R=0.010000            106 us         97.3 us         7029 PeakRSS_MB=173.93 TotalFound=1k
RangeQuery/1K/R=0.100000            107 us         98.9 us         7225 PeakRSS_MB=185.254 TotalFound=1.005k
RangeQuery/1K/R=1.000000            106 us         97.6 us         7084 PeakRSS_MB=175.184 TotalFound=1.035k
RangeQuery/1K/R=10.000000           116 us          105 us         6618 PeakRSS_MB=181.504 TotalFound=6.591k
RangeQuery/1K/R=100.000000          435 us          433 us         1614 PeakRSS_MB=175.957 TotalFound=459.76k
RangeQuery/1K/R=200.000000          578 us          577 us         1201 PeakRSS_MB=176.012 TotalFound=931.404k
RangeQuery/10K/R=0.010000           109 us         99.3 us         7007 PeakRSS_MB=176.367 TotalFound=1k
RangeQuery/10K/R=0.100000           109 us        100.0 us         6951 PeakRSS_MB=181.66 TotalFound=1.001k
RangeQuery/10K/R=1.000000           113 us          101 us         6773 PeakRSS_MB=181.309 TotalFound=1.295k
RangeQuery/10K/R=10.000000          190 us          178 us         3912 PeakRSS_MB=178.121 TotalFound=53.444k
RangeQuery/10K/R=100.000000        2596 us         2410 us          288 PeakRSS_MB=180.77 TotalFound=4.38585M
RangeQuery/10K/R=200.000000        4156 us         3416 us          203 PeakRSS_MB=178.309 TotalFound=9.12136M
RangeQuery/100K/R=0.010000          119 us          105 us         6614 PeakRSS_MB=191.992 TotalFound=1k
RangeQuery/100K/R=0.100000          168 us          111 us         6612 PeakRSS_MB=191.734 TotalFound=1.005k
RangeQuery/100K/R=1.000000          132 us          115 us         5886 PeakRSS_MB=192.719 TotalFound=4.145k
RangeQuery/100K/R=10.000000         682 us          680 us          982 PeakRSS_MB=195.23 TotalFound=506.846k
RangeQuery/100K/R=100.000000      21523 us        16977 us           35 PeakRSS_MB=191.449 TotalFound=43.2318M
RangeQuery/100K/R=200.000000      38941 us        30061 us           20 PeakRSS_MB=192.141 TotalFound=90.3647M
RangeQuery/1M/R=0.010000            123 us          110 us         6428 PeakRSS_MB=342.152 TotalFound=1k
RangeQuery/1M/R=0.100000            125 us          109 us         6377 PeakRSS_MB=339.637 TotalFound=1.139k
RangeQuery/1M/R=1.000000            234 us          224 us         3108 PeakRSS_MB=340.012 TotalFound=39.044k
RangeQuery/1M/R=10.000000          4783 us         3395 us          209 PeakRSS_MB=339.699 TotalFound=5.78161M
RangeQuery/1M/R=100.000000       209478 us       119025 us            7 PeakRSS_MB=340.008 TotalFound=433.516M
RangeQuery/1M/R=200.000000       374627 us       255115 us            3 PeakRSS_MB=340.133 TotalFound=907.777M
RangeQuery/10M/R=0.010000           123 us          109 us         6367 PeakRSS_MB=1.8656k TotalFound=1.001k
RangeQuery/10M/R=0.100000           134 us          116 us         6071 PeakRSS_MB=1.86568k TotalFound=1.793k
RangeQuery/10M/R=1.000000          1418 us         1362 us          512 PeakRSS_MB=1.8657k TotalFound=363.099k
RangeQuery/10M/R=10.000000        66496 us        26711 us           26 PeakRSS_MB=1.86555k TotalFound=55.2471M
RangeQuery/10M/R=100.000000     2494993 us      1448083 us            1 PeakRSS_MB=1.86572k TotalFound=4.39221G
RangeQuery/10M/R=200.000000     3995941 us      3762066 us            1 PeakRSS_MB=1.86552k TotalFound=9.09205G

--- Executing ./self_knn_query_scaling.exe ---
2026-08-27T15:38:54+02:00
Running ./self_knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 0.49, 0.24, 4.19
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------------------
Benchmark                        Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------
SelfKNNQuery/1K/k=1           88.0 us         78.2 us         9124 PeakRSS_MB=179.414 items_per_second=12.7852M/s
SelfKNNQuery/1K/k=2            155 us          144 us         4870 PeakRSS_MB=178.266 items_per_second=6.93124M/s
SelfKNNQuery/1K/k=4            189 us          179 us         3938 PeakRSS_MB=177.117 items_per_second=5.5834M/s
SelfKNNQuery/1K/k=8            263 us          252 us         2773 PeakRSS_MB=175.984 items_per_second=3.96639M/s
SelfKNNQuery/1K/k=16           412 us          411 us         1705 PeakRSS_MB=174.645 items_per_second=2.43372M/s
SelfKNNQuery/1K/k=32           611 us          610 us         1132 PeakRSS_MB=172.18 items_per_second=1.63878M/s
SelfKNNQuery/1K/k=64           962 us          961 us          722 PeakRSS_MB=168.109 items_per_second=1.04046M/s
SelfKNNQuery/1K/k=128         1585 us         1577 us          439 PeakRSS_MB=161.387 items_per_second=634.03k/s
SelfKNNQuery/10K/k=1           428 us          416 us         1683 PeakRSS_MB=159.48 items_per_second=24.033M/s
SelfKNNQuery/10K/k=2           971 us          958 us          737 PeakRSS_MB=175.316 items_per_second=10.4343M/s
SelfKNNQuery/10K/k=4          1392 us         1386 us          501 PeakRSS_MB=168.332 items_per_second=7.21653M/s
SelfKNNQuery/10K/k=8          1981 us         1980 us          352 PeakRSS_MB=162.82 items_per_second=5.05079M/s
SelfKNNQuery/10K/k=16         3014 us         3012 us          231 PeakRSS_MB=173.906 items_per_second=3.31974M/s
SelfKNNQuery/10K/k=32         4949 us         4947 us          142 PeakRSS_MB=167.91 items_per_second=2.02135M/s
SelfKNNQuery/10K/k=64         8613 us         8578 us           80 PeakRSS_MB=179.184 items_per_second=1.16578M/s
SelfKNNQuery/10K/k=128       16254 us        15803 us           44 PeakRSS_MB=186.508 items_per_second=632.798k/s
SelfKNNQuery/100K/k=1         4827 us         4803 us          146 PeakRSS_MB=179.199 items_per_second=20.8223M/s
SelfKNNQuery/100K/k=2        10348 us        10337 us           68 PeakRSS_MB=180.246 items_per_second=9.6744M/s
SelfKNNQuery/100K/k=4        14396 us        14392 us           49 PeakRSS_MB=182.918 items_per_second=6.94836M/s
SelfKNNQuery/100K/k=8        20309 us        20296 us           34 PeakRSS_MB=192.402 items_per_second=4.92712M/s
SelfKNNQuery/100K/k=16       30320 us        30312 us           23 PeakRSS_MB=205.379 items_per_second=3.299M/s
SelfKNNQuery/100K/k=32       49276 us        49262 us           14 PeakRSS_MB=232.207 items_per_second=2.02995M/s
SelfKNNQuery/100K/k=64       83071 us        82967 us            8 PeakRSS_MB=285.391 items_per_second=1.2053M/s
SelfKNNQuery/100K/k=128     155738 us       154545 us            5 PeakRSS_MB=387.262 items_per_second=647.061k/s
SelfKNNQuery/1M/k=1          51220 us        51176 us           14 PeakRSS_MB=355.195 items_per_second=19.5403M/s
SelfKNNQuery/1M/k=2         110209 us       109988 us            6 PeakRSS_MB=374.375 items_per_second=9.09187M/s
SelfKNNQuery/1M/k=4         152623 us       152586 us            5 PeakRSS_MB=406.445 items_per_second=6.55366M/s
SelfKNNQuery/1M/k=8         213965 us       213919 us            3 PeakRSS_MB=471.117 items_per_second=4.67466M/s
SelfKNNQuery/1M/k=16        319114 us       318952 us            2 PeakRSS_MB=599.109 items_per_second=3.13527M/s
SelfKNNQuery/1M/k=32        515776 us       515531 us            1 PeakRSS_MB=845.27 items_per_second=1.93975M/s
SelfKNNQuery/1M/k=64        869301 us       867322 us            1 PeakRSS_MB=1.33393k items_per_second=1.15297M/s
SelfKNNQuery/1M/k=128      1620824 us      1619370 us            1 PeakRSS_MB=2.3105k items_per_second=617.524k/s
SelfKNNQuery/10M/k=1        550668 us       550468 us            1 PeakRSS_MB=2.01811k items_per_second=18.1664M/s
SelfKNNQuery/10M/k=2       1209975 us      1209695 us            1 PeakRSS_MB=2.17257k items_per_second=8.26655M/s
SelfKNNQuery/10M/k=4       1652445 us      1650826 us            1 PeakRSS_MB=2.47777k items_per_second=6.05758M/s
SelfKNNQuery/10M/k=8       2313517 us      2312950 us            1 PeakRSS_MB=3.08824k items_per_second=4.32348M/s
SelfKNNQuery/10M/k=16      3414388 us      3412518 us            1 PeakRSS_MB=4.30827k items_per_second=2.93039M/s
SelfKNNQuery/10M/k=32      5490692 us      5485678 us            1 PeakRSS_MB=6.74978k items_per_second=1.82293M/s
SelfKNNQuery/10M/k=64      9215056 us      9205548 us            1 PeakRSS_MB=11.6333k items_per_second=1.0863M/s
SelfKNNQuery/10M/k=128    17246498 us     17210306 us            1 PeakRSS_MB=21.3989k items_per_second=581.047k/s

```
