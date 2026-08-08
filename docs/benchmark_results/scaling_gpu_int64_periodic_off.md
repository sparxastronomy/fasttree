# Scaling Benchmark Results: gpu_int64 (Periodic BC: OFF)
Date: Sat Aug  8 17:56:14 CEST 2026
Device: GPU
Target GPU: nvidia
SFC Curve: Peano-Hilbert
Periodic BC: OFF
Build Directory: build_int64
```
--- Executing ./sfc_encoding_scaling.exe ---
2026-08-08T17:56:14+02:00
Running ./sfc_encoding_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 0.98, 0.76, 0.40
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------
Benchmark                Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------
SFC-Encode/1K        0.024 ms        0.024 ms        29327 PeakRSS_MB=245.824 items_per_second=42.22M/s
SFC-Encode/10K       0.022 ms        0.022 ms        30285 PeakRSS_MB=246.824 items_per_second=451.395M/s
SFC-Encode/100K      0.026 ms        0.026 ms        24794 PeakRSS_MB=251.039 items_per_second=3.87056G/s
SFC-Encode/1M        0.094 ms        0.094 ms         7839 PeakRSS_MB=303.148 items_per_second=10.6857G/s
SFC-Encode/10M       0.710 ms        0.710 ms          795 PeakRSS_MB=770.918 items_per_second=14.0817G/s

--- Executing ./gpu_sort_scaling.exe ---
2026-08-08T17:56:37+02:00
Running ./gpu_sort_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 0.99, 0.78, 0.41
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
GPU_Sort/1K        0.055 ms        0.055 ms        12515 PeakRSS_MB=248.004 items_per_second=18.1109M/s
GPU_Sort/10K       0.125 ms        0.125 ms         5471 PeakRSS_MB=249.129 items_per_second=80.1411M/s
GPU_Sort/100K      0.363 ms        0.363 ms         1929 PeakRSS_MB=256.441 items_per_second=275.781M/s
GPU_Sort/1M         2.68 ms         2.68 ms          260 PeakRSS_MB=304.547 items_per_second=373.636M/s
GPU_Sort/10M        37.3 ms         37.2 ms           18 PeakRSS_MB=506.48 items_per_second=268.963M/s

--- Executing ./tree_build_scaling.exe ---
2026-08-08T17:56:56+02:00
Running ./tree_build_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 0.99, 0.79, 0.42
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------------
Benchmark               Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------
TreeBuild/1K         1.72 ms         1.72 ms          389 PeakRSS_MB=247.535 items_per_second=582.166k/s
TreeBuild/10K        2.39 ms         2.39 ms          291 PeakRSS_MB=248.098 items_per_second=4.19026M/s
TreeBuild/100K       5.76 ms         5.73 ms          122 PeakRSS_MB=252.52 items_per_second=17.4386M/s
TreeBuild/1M         29.5 ms         29.4 ms           22 PeakRSS_MB=310.578 items_per_second=33.9631M/s
TreeBuild/10M         289 ms          289 ms            2 PeakRSS_MB=780.957 items_per_second=34.5862M/s

--- Executing ./rebuild_scaling.exe ---
2026-08-08T17:57:10+02:00
Running ./rebuild_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.06, 0.82, 0.44
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------
TreeRebuild/1K         1.76 ms         1.75 ms          394 PeakRSS_MB=245.617 items_per_second=571.228k/s
TreeRebuild/10K        2.42 ms         2.42 ms          289 PeakRSS_MB=246.18 items_per_second=4.13261M/s
TreeRebuild/100K       5.64 ms         5.63 ms          122 PeakRSS_MB=254.191 items_per_second=17.765M/s
TreeRebuild/1M         30.2 ms         30.1 ms           22 PeakRSS_MB=335.129 items_per_second=33.211M/s
TreeRebuild/10M         296 ms          296 ms            2 PeakRSS_MB=1.07565k items_per_second=33.7691M/s

--- Executing ./range_query_scaling.exe ---
2026-08-08T17:57:25+02:00
Running ./range_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.05, 0.83, 0.45
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
RangeQuery/1K/R=0.010000           72.8 us         72.8 us         9567 PeakRSS_MB=246.672 TotalFound=1k
RangeQuery/1K/R=0.100000           69.9 us         69.9 us         9934 PeakRSS_MB=246.672 TotalFound=1.005k
RangeQuery/1K/R=1.000000           81.5 us         81.5 us         8525 PeakRSS_MB=246.672 TotalFound=1.035k
RangeQuery/1K/R=10.000000           173 us          173 us         4017 PeakRSS_MB=246.672 TotalFound=6.591k
RangeQuery/1K/R=100.000000          887 us          887 us          785 PeakRSS_MB=246.672 TotalFound=459.76k
RangeQuery/1K/R=200.000000         1037 us         1037 us          673 PeakRSS_MB=246.672 TotalFound=931.404k
RangeQuery/10K/R=0.010000          88.0 us         88.0 us         7889 PeakRSS_MB=247.172 TotalFound=1k
RangeQuery/10K/R=0.100000           102 us          102 us         6863 PeakRSS_MB=247.734 TotalFound=1.001k
RangeQuery/10K/R=1.000000           141 us          141 us         4935 PeakRSS_MB=247.672 TotalFound=1.295k
RangeQuery/10K/R=10.000000          858 us          857 us          812 PeakRSS_MB=247.609 TotalFound=53.444k
RangeQuery/10K/R=100.000000       12054 us        12051 us           58 PeakRSS_MB=247.609 TotalFound=4.38585M
RangeQuery/10K/R=200.000000       12198 us        12195 us           57 PeakRSS_MB=246.984 TotalFound=9.12136M
RangeQuery/100K/R=0.010000          107 us          107 us         6532 PeakRSS_MB=252.289 TotalFound=1k
RangeQuery/100K/R=0.100000          123 us          123 us         5658 PeakRSS_MB=252.219 TotalFound=1.005k
RangeQuery/100K/R=1.000000          392 us          391 us         1800 PeakRSS_MB=252.129 TotalFound=4.145k
RangeQuery/100K/R=10.000000        7608 us         7607 us           92 PeakRSS_MB=252.129 TotalFound=506.846k
RangeQuery/100K/R=100.000000     126183 us       126164 us            6 PeakRSS_MB=252.129 TotalFound=43.2318M
RangeQuery/100K/R=200.000000     161874 us       161849 us            4 PeakRSS_MB=252.066 TotalFound=90.3647M
RangeQuery/1M/R=0.010000            153 us          152 us         4604 PeakRSS_MB=307.598 TotalFound=1k
RangeQuery/1M/R=0.100000            182 us          181 us         3854 PeakRSS_MB=307.535 TotalFound=1.139k
RangeQuery/1M/R=1.000000           2202 us         2201 us          318 PeakRSS_MB=306.941 TotalFound=39.044k
RangeQuery/1M/R=10.000000         87075 us        87062 us            8 PeakRSS_MB=307.504 TotalFound=5.78161M
RangeQuery/1M/R=100.000000      1795958 us      1794537 us            1 PeakRSS_MB=307.441 TotalFound=433.516M
RangeQuery/1M/R=200.000000      2203187 us      2202854 us            1 PeakRSS_MB=307.441 TotalFound=907.777M
RangeQuery/10M/R=0.010000           138 us          138 us         4979 PeakRSS_MB=781.246 TotalFound=1.001k
RangeQuery/10M/R=0.100000           434 us          434 us         1616 PeakRSS_MB=780.762 TotalFound=1.793k
RangeQuery/10M/R=1.000000         25408 us        25403 us           28 PeakRSS_MB=781.363 TotalFound=363.099k
RangeQuery/10M/R=10.000000       971948 us       971757 us            1 PeakRSS_MB=780.801 TotalFound=55.2471M
RangeQuery/10M/R=100.000000    22918444 us     22911555 us            1 PeakRSS_MB=781.367 TotalFound=4.39221G
RangeQuery/10M/R=200.000000    34081794 us     34071838 us            1 PeakRSS_MB=781.375 TotalFound=9.09205G

--- Executing ./knn_query_scaling.exe ---
2026-08-08T18:01:03+02:00
Running ./knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.00, 0.92, 0.57
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
KNNQuery/1K/k=1           63.6 us         63.6 us        10682 PeakRSS_MB=246.836
KNNQuery/1K/k=2            126 us          126 us         5580 PeakRSS_MB=246.836
KNNQuery/1K/k=4            169 us          169 us         4142 PeakRSS_MB=246.273
KNNQuery/1K/k=8            240 us          240 us         2915 PeakRSS_MB=246.836
KNNQuery/1K/k=16           364 us          364 us         1924 PeakRSS_MB=246.836
KNNQuery/1K/k=32           578 us          578 us         1208 PeakRSS_MB=246.836
KNNQuery/1K/k=64          2527 us         2526 us          277 PeakRSS_MB=246.273
KNNQuery/1K/k=128         2721 us         2721 us          257 PeakRSS_MB=246.836
KNNQuery/10K/k=1          87.8 us         87.7 us         7967 PeakRSS_MB=247.336
KNNQuery/10K/k=2           196 us          196 us         3569 PeakRSS_MB=247.336
KNNQuery/10K/k=4           288 us          288 us         2429 PeakRSS_MB=247.273
KNNQuery/10K/k=8           383 us          383 us         1822 PeakRSS_MB=247.773
KNNQuery/10K/k=16          573 us          572 us         1218 PeakRSS_MB=247.211
KNNQuery/10K/k=32          914 us          913 us          764 PeakRSS_MB=247.711
KNNQuery/10K/k=64         4167 us         4166 us          168 PeakRSS_MB=247.648
KNNQuery/10K/k=128        4223 us         4222 us          166 PeakRSS_MB=248.211
KNNQuery/100K/k=1          112 us          112 us         6223 PeakRSS_MB=251.973
KNNQuery/100K/k=2          237 us          236 us         2957 PeakRSS_MB=251.973
KNNQuery/100K/k=4          285 us          285 us         2448 PeakRSS_MB=252.535
KNNQuery/100K/k=8          404 us          404 us         1733 PeakRSS_MB=251.965
KNNQuery/100K/k=16         585 us          585 us         1192 PeakRSS_MB=252.527
KNNQuery/100K/k=32         950 us          950 us          735 PeakRSS_MB=251.965
KNNQuery/100K/k=64        4799 us         4798 us          146 PeakRSS_MB=251.965
KNNQuery/100K/k=128       4839 us         4838 us          144 PeakRSS_MB=252.527
KNNQuery/1M/k=1            125 us          125 us         5586 PeakRSS_MB=306.93
KNNQuery/1M/k=2            262 us          262 us         2652 PeakRSS_MB=307.492
KNNQuery/1M/k=4            320 us          320 us         2191 PeakRSS_MB=307.492
KNNQuery/1M/k=8            442 us          442 us         1582 PeakRSS_MB=306.934
KNNQuery/1M/k=16           737 us          737 us          945 PeakRSS_MB=306.934
KNNQuery/1M/k=32          1431 us         1431 us          488 PeakRSS_MB=310.621
KNNQuery/1M/k=64          5206 us         5205 us          134 PeakRSS_MB=306.938
KNNQuery/1M/k=128         5723 us         5722 us          122 PeakRSS_MB=307.5
KNNQuery/10M/k=1           133 us          133 us         4876 PeakRSS_MB=780.828
KNNQuery/10M/k=2           283 us          283 us         2472 PeakRSS_MB=780.895
KNNQuery/10M/k=4           403 us          403 us         1706 PeakRSS_MB=780.902
KNNQuery/10M/k=8           609 us          609 us         1064 PeakRSS_MB=780.918
KNNQuery/10M/k=16         1001 us         1001 us          689 PeakRSS_MB=780.41
KNNQuery/10M/k=32         1690 us         1690 us          406 PeakRSS_MB=780.453
KNNQuery/10M/k=64         5669 us         5668 us          123 PeakRSS_MB=781.016
KNNQuery/10M/k=128        6471 us         6470 us          102 PeakRSS_MB=781.016

--- Executing ./self_knn_query_scaling.exe ---
2026-08-08T18:04:03+02:00
Running ./self_knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.02, 1.00, 0.67
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------------------
Benchmark                        Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------
SelfKNNQuery/1K/k=1            266 us          266 us         2491 PeakRSS_MB=247.961 items_per_second=3.76022M/s
SelfKNNQuery/1K/k=2            330 us          329 us         2121 PeakRSS_MB=247.961 items_per_second=3.03826M/s
SelfKNNQuery/1K/k=4            366 us          366 us         1903 PeakRSS_MB=247.961 items_per_second=2.73253M/s
SelfKNNQuery/1K/k=8            441 us          440 us         1586 PeakRSS_MB=247.961 items_per_second=2.27091M/s
SelfKNNQuery/1K/k=16           559 us          559 us         1240 PeakRSS_MB=247.961 items_per_second=1.78977M/s
SelfKNNQuery/1K/k=32           786 us          785 us          882 PeakRSS_MB=247.961 items_per_second=1.27367M/s
SelfKNNQuery/1K/k=64          3026 us         3025 us          231 PeakRSS_MB=247.961 items_per_second=330.597k/s
SelfKNNQuery/1K/k=128         3032 us         3031 us          231 PeakRSS_MB=247.961 items_per_second=329.934k/s
SelfKNNQuery/10K/k=1           339 us          339 us         2073 PeakRSS_MB=248.523 items_per_second=29.5353M/s
SelfKNNQuery/10K/k=2           480 us          480 us         1436 PeakRSS_MB=248.523 items_per_second=20.836M/s
SelfKNNQuery/10K/k=4           548 us          547 us         1263 PeakRSS_MB=248.523 items_per_second=18.2686M/s
SelfKNNQuery/10K/k=8           688 us          686 us         1013 PeakRSS_MB=248.523 items_per_second=14.5763M/s
SelfKNNQuery/10K/k=16          984 us          983 us          706 PeakRSS_MB=249.086 items_per_second=10.171M/s
SelfKNNQuery/10K/k=32         1550 us         1549 us          448 PeakRSS_MB=249.086 items_per_second=6.45515M/s
SelfKNNQuery/10K/k=64        11071 us        11067 us           63 PeakRSS_MB=248.523 items_per_second=903.602k/s
SelfKNNQuery/10K/k=128       13064 us        13059 us           54 PeakRSS_MB=248.523 items_per_second=765.736k/s
SelfKNNQuery/100K/k=1         1109 us         1108 us          631 PeakRSS_MB=252.922 items_per_second=90.2495M/s
SelfKNNQuery/100K/k=2         1531 us         1530 us          458 PeakRSS_MB=253.512 items_per_second=65.3563M/s
SelfKNNQuery/100K/k=4         1959 us         1958 us          357 PeakRSS_MB=253.539 items_per_second=51.0852M/s
SelfKNNQuery/100K/k=8         2776 us         2775 us          252 PeakRSS_MB=253.539 items_per_second=36.0379M/s
SelfKNNQuery/100K/k=16        4676 us         4674 us          150 PeakRSS_MB=253.594 items_per_second=21.395M/s
SelfKNNQuery/100K/k=32        9341 us         9320 us           74 PeakRSS_MB=253.039 items_per_second=10.7293M/s
SelfKNNQuery/100K/k=64       87729 us        87712 us            8 PeakRSS_MB=253.543 items_per_second=1.1401M/s
SelfKNNQuery/100K/k=128     109650 us       109624 us            6 PeakRSS_MB=252.98 items_per_second=912.208k/s
SelfKNNQuery/1M/k=1          14465 us        14456 us           49 PeakRSS_MB=308.27 items_per_second=69.1733M/s
SelfKNNQuery/1M/k=2          28512 us        28504 us           25 PeakRSS_MB=308.867 items_per_second=35.0822M/s
SelfKNNQuery/1M/k=4          45560 us        45549 us           15 PeakRSS_MB=308.867 items_per_second=21.9543M/s
SelfKNNQuery/1M/k=8          72913 us        72896 us           10 PeakRSS_MB=308.883 items_per_second=13.7183M/s
SelfKNNQuery/1M/k=16        122220 us       122194 us            6 PeakRSS_MB=308.883 items_per_second=8.18371M/s
SelfKNNQuery/1M/k=32        220443 us       220403 us            3 PeakRSS_MB=309.445 items_per_second=4.53715M/s
SelfKNNQuery/1M/k=64       1152303 us      1152115 us            1 PeakRSS_MB=308.32 items_per_second=867.969k/s
SelfKNNQuery/1M/k=128      1408802 us      1407353 us            1 PeakRSS_MB=308.32 items_per_second=710.554k/s
SelfKNNQuery/10M/k=1        243526 us       243474 us            3 PeakRSS_MB=780.691 items_per_second=41.0721M/s
SelfKNNQuery/10M/k=2        460743 us       460661 us            2 PeakRSS_MB=781.305 items_per_second=21.7079M/s
SelfKNNQuery/10M/k=4        682322 us       682209 us            1 PeakRSS_MB=781.309 items_per_second=14.6583M/s
SelfKNNQuery/10M/k=8       1027875 us      1026505 us            1 PeakRSS_MB=781.309 items_per_second=9.74179M/s
SelfKNNQuery/10M/k=16      1621254 us      1620995 us            1 PeakRSS_MB=781.316 items_per_second=6.16905M/s
SelfKNNQuery/10M/k=32      2727488 us      2726996 us            1 PeakRSS_MB=781.336 items_per_second=3.66704M/s
SelfKNNQuery/10M/k=64     12481747 us     12478393 us            1 PeakRSS_MB=781.336 items_per_second=801.385k/s
SelfKNNQuery/10M/k=128    15121600 us     15117929 us            1 PeakRSS_MB=781.336 items_per_second=661.466k/s

```
