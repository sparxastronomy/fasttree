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

--- Executing ./knn_query_scaling.exe ---
2026-08-07T12:57:42+02:00
Running ./knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 28.10, 19.95, 16.13
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
KNNQuery/1K/k=1           76.5 us         66.2 us        10566 PeakRSS_MB=180.367
KNNQuery/1K/k=2           83.6 us         71.3 us         9997 PeakRSS_MB=179.805
KNNQuery/1K/k=4           95.7 us         82.3 us         8432 PeakRSS_MB=178.652
KNNQuery/1K/k=8            127 us          105 us         6744 PeakRSS_MB=178.07
KNNQuery/1K/k=16           158 us          146 us         4830 PeakRSS_MB=176.297
KNNQuery/1K/k=32           238 us          224 us         3109 PeakRSS_MB=172.992
KNNQuery/1K/k=64          2068 us         2067 us          337 PeakRSS_MB=168.953
KNNQuery/1K/k=128         2458 us         2458 us          284 PeakRSS_MB=161.688
KNNQuery/10K/k=1          82.4 us         68.9 us        10211 PeakRSS_MB=163.18
KNNQuery/10K/k=2          92.2 us         78.3 us         8866 PeakRSS_MB=174.902
KNNQuery/10K/k=4           107 us         93.6 us         7430 PeakRSS_MB=167.496
KNNQuery/10K/k=8           131 us          117 us         6144 PeakRSS_MB=162.98
KNNQuery/10K/k=16          178 us          164 us         4288 PeakRSS_MB=171.219
KNNQuery/10K/k=32          274 us          261 us         2699 PeakRSS_MB=170.582
KNNQuery/10K/k=64         2503 us         2492 us          281 PeakRSS_MB=166.605
KNNQuery/10K/k=128        3057 us         3056 us          229 PeakRSS_MB=171.793
KNNQuery/100K/k=1         92.8 us         75.3 us         9373 PeakRSS_MB=181.641
KNNQuery/100K/k=2          110 us         93.2 us         7508 PeakRSS_MB=179.551
KNNQuery/100K/k=4          129 us          111 us         6297 PeakRSS_MB=182.238
KNNQuery/100K/k=8          158 us          140 us         5027 PeakRSS_MB=182.805
KNNQuery/100K/k=16         209 us          191 us         3667 PeakRSS_MB=181.387
KNNQuery/100K/k=32         316 us          310 us         2255 PeakRSS_MB=183.117
KNNQuery/100K/k=64        2631 us         2615 us          269 PeakRSS_MB=180.359
KNNQuery/100K/k=128       3185 us         3183 us          220 PeakRSS_MB=182.617
KNNQuery/1M/k=1            103 us         81.8 us         8525 PeakRSS_MB=342.227
KNNQuery/1M/k=2            119 us          101 us         6974 PeakRSS_MB=342.527
KNNQuery/1M/k=4            139 us          119 us         5876 PeakRSS_MB=341.781
KNNQuery/1M/k=8            170 us          151 us         4578 PeakRSS_MB=344.73
KNNQuery/1M/k=16           226 us          206 us         3371 PeakRSS_MB=343.41
KNNQuery/1M/k=32           349 us          347 us         1992 PeakRSS_MB=341.988
KNNQuery/1M/k=64          2791 us         2785 us          251 PeakRSS_MB=343.453
KNNQuery/1M/k=128         3476 us         3453 us          203 PeakRSS_MB=342.797
KNNQuery/10M/k=1           104 us         88.7 us         7886 PeakRSS_MB=1.86796k
KNNQuery/10M/k=2           126 us          109 us         6391 PeakRSS_MB=1.86808k
KNNQuery/10M/k=4           147 us          130 us         5467 PeakRSS_MB=1.8681k
KNNQuery/10M/k=8           178 us          161 us         4400 PeakRSS_MB=1.86818k
KNNQuery/10M/k=16          244 us          223 us         3119 PeakRSS_MB=1.86821k
KNNQuery/10M/k=32          404 us          402 us         1733 PeakRSS_MB=1.86821k
KNNQuery/10M/k=64         3152 us         3142 us          222 PeakRSS_MB=1.86792k
KNNQuery/10M/k=128        3833 us         3827 us          183 PeakRSS_MB=1.86813k

--- Executing ./self_knn_query_scaling.exe ---
2026-08-08T21:13:21+02:00
Running ./self_knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 0.74, 0.23, 0.08
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------------------
Benchmark                        Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------
SelfKNNQuery/1K/k=1            219 us          178 us         3903 PeakRSS_MB=189.969 items_per_second=5.60757M/s
SelfKNNQuery/1K/k=2            228 us          186 us         3780 PeakRSS_MB=188.824 items_per_second=5.36487M/s
SelfKNNQuery/1K/k=4            242 us          197 us         3543 PeakRSS_MB=188.254 items_per_second=5.08767M/s
SelfKNNQuery/1K/k=8            262 us          217 us         3211 PeakRSS_MB=186.527 items_per_second=4.61146M/s
SelfKNNQuery/1K/k=16           303 us          259 us         2691 PeakRSS_MB=184.738 items_per_second=3.85556M/s
SelfKNNQuery/1K/k=32           386 us          350 us         1984 PeakRSS_MB=182.07 items_per_second=2.85631M/s
SelfKNNQuery/1K/k=64          2223 us         2203 us          317 PeakRSS_MB=177.746 items_per_second=453.886k/s
SelfKNNQuery/1K/k=128         2619 us         2597 us          268 PeakRSS_MB=171.004 items_per_second=385.093k/s
SelfKNNQuery/10K/k=1           910 us          873 us          796 PeakRSS_MB=177.438 items_per_second=11.4485M/s
SelfKNNQuery/10K/k=2          1272 us         1234 us          570 PeakRSS_MB=177.051 items_per_second=8.1023M/s
SelfKNNQuery/10K/k=4          1543 us         1515 us          461 PeakRSS_MB=178.133 items_per_second=6.60106M/s
SelfKNNQuery/10K/k=8          2087 us         2055 us          340 PeakRSS_MB=175.789 items_per_second=4.86645M/s
SelfKNNQuery/10K/k=16         3228 us         3032 us          232 PeakRSS_MB=181.199 items_per_second=3.29808M/s
SelfKNNQuery/10K/k=32         5303 us         4764 us          147 PeakRSS_MB=181.664 items_per_second=2.09892M/s
SelfKNNQuery/10K/k=64        22427 us        22382 us           31 PeakRSS_MB=187.723 items_per_second=446.791k/s
SelfKNNQuery/10K/k=128       27956 us        27892 us           25 PeakRSS_MB=197.703 items_per_second=358.527k/s
SelfKNNQuery/100K/k=1         6375 us         6255 us          112 PeakRSS_MB=191.77 items_per_second=15.9862M/s
SelfKNNQuery/100K/k=2         9625 us         9225 us           77 PeakRSS_MB=193.211 items_per_second=10.8406M/s
SelfKNNQuery/100K/k=4        12741 us        12008 us           58 PeakRSS_MB=196.293 items_per_second=8.3279M/s
SelfKNNQuery/100K/k=8        17937 us        16550 us           42 PeakRSS_MB=202.941 items_per_second=6.04243M/s
SelfKNNQuery/100K/k=16       28048 us        25044 us           28 PeakRSS_MB=215.602 items_per_second=3.99292M/s
SelfKNNQuery/100K/k=32       48408 us        42964 us           16 PeakRSS_MB=238.516 items_per_second=2.32754M/s
SelfKNNQuery/100K/k=64      229613 us       229451 us            3 PeakRSS_MB=290.57 items_per_second=435.822k/s
SelfKNNQuery/100K/k=128     285420 us       285181 us            2 PeakRSS_MB=397.719 items_per_second=350.654k/s
SelfKNNQuery/1M/k=1          57609 us        56713 us           12 PeakRSS_MB=357.809 items_per_second=17.6325M/s
SelfKNNQuery/1M/k=2          92274 us        90527 us            8 PeakRSS_MB=369.945 items_per_second=11.0465M/s
SelfKNNQuery/1M/k=4         122882 us       120243 us            6 PeakRSS_MB=402.176 items_per_second=8.3165M/s
SelfKNNQuery/1M/k=8         175950 us       172521 us            4 PeakRSS_MB=460.887 items_per_second=5.79641M/s
SelfKNNQuery/1M/k=16        273409 us       267410 us            3 PeakRSS_MB=583.656 items_per_second=3.73958M/s
SelfKNNQuery/1M/k=32        473388 us       459339 us            2 PeakRSS_MB=831.359 items_per_second=2.17704M/s
SelfKNNQuery/1M/k=64       2489266 us      2488376 us            1 PeakRSS_MB=1.32632k items_per_second=401.868k/s
SelfKNNQuery/1M/k=128      3078234 us      3077344 us            1 PeakRSS_MB=2.30535k items_per_second=324.956k/s
SelfKNNQuery/10M/k=1        648059 us       640867 us            1 PeakRSS_MB=2.02094k items_per_second=15.6039M/s
SelfKNNQuery/10M/k=2       1111830 us      1101325 us            1 PeakRSS_MB=2.17314k items_per_second=9.07997M/s
SelfKNNQuery/10M/k=4       1492379 us      1461846 us            1 PeakRSS_MB=2.47832k items_per_second=6.84066M/s
SelfKNNQuery/10M/k=8       2075094 us      2052838 us            1 PeakRSS_MB=3.0892k items_per_second=4.8713M/s
SelfKNNQuery/10M/k=16      3177457 us      3140519 us            1 PeakRSS_MB=4.30817k items_per_second=3.18419M/s
SelfKNNQuery/10M/k=32      5312788 us      5309265 us            1 PeakRSS_MB=6.75078k items_per_second=1.8835M/s
SelfKNNQuery/10M/k=64     28038371 us     28007438 us            1 PeakRSS_MB=11.6324k items_per_second=357.048k/s
SelfKNNQuery/10M/k=128    34111489 us     34098030 us            1 PeakRSS_MB=21.3982k items_per_second=293.272k/s

```
