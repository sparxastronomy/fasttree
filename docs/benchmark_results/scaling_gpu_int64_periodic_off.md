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
2026-08-27T09:02:26+02:00
Running ./self_knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 4.35, 4.26, 3.87
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------------------
Benchmark                        Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------
SelfKNNQuery/1K/k=1           59.7 us         59.7 us         9772 PeakRSS_MB=144.367 items_per_second=16.7477M/s
SelfKNNQuery/1K/k=2            276 us          276 us         2531 PeakRSS_MB=144.367 items_per_second=3.61823M/s
SelfKNNQuery/1K/k=4            375 us          375 us         1873 PeakRSS_MB=144.367 items_per_second=2.66484M/s
SelfKNNQuery/1K/k=8            581 us          581 us         1203 PeakRSS_MB=144.367 items_per_second=1.72001M/s
SelfKNNQuery/1K/k=16          1117 us         1117 us          624 PeakRSS_MB=144.367 items_per_second=895.645k/s
SelfKNNQuery/1K/k=32          1486 us         1486 us          471 PeakRSS_MB=144.367 items_per_second=672.933k/s
SelfKNNQuery/1K/k=64          3686 us         3685 us          189 PeakRSS_MB=144.367 items_per_second=271.364k/s
SelfKNNQuery/1K/k=128         4808 us         4807 us          146 PeakRSS_MB=144.367 items_per_second=208.02k/s
SelfKNNQuery/10K/k=1          74.5 us         74.5 us         9318 PeakRSS_MB=145.492 items_per_second=134.159M/s
SelfKNNQuery/10K/k=2           475 us          474 us         1474 PeakRSS_MB=144.93 items_per_second=21.0763M/s
SelfKNNQuery/10K/k=4           876 us          876 us          797 PeakRSS_MB=145.492 items_per_second=11.4148M/s
SelfKNNQuery/10K/k=8          1321 us         1321 us          528 PeakRSS_MB=145.492 items_per_second=7.57093M/s
SelfKNNQuery/10K/k=16         2359 us         2359 us          296 PeakRSS_MB=144.93 items_per_second=4.23989M/s
SelfKNNQuery/10K/k=32         4722 us         4722 us          148 PeakRSS_MB=144.93 items_per_second=2.11787M/s
SelfKNNQuery/10K/k=64        11214 us        11213 us           62 PeakRSS_MB=144.93 items_per_second=891.84k/s
SelfKNNQuery/10K/k=128       15966 us        15963 us           44 PeakRSS_MB=145.492 items_per_second=626.437k/s
SelfKNNQuery/100K/k=1          235 us          235 us         2981 PeakRSS_MB=149.719 items_per_second=425.984M/s
SelfKNNQuery/100K/k=2         1265 us         1265 us          545 PeakRSS_MB=150.184 items_per_second=79.0378M/s
SelfKNNQuery/100K/k=4         2008 us         2008 us          345 PeakRSS_MB=150.234 items_per_second=49.7972M/s
SelfKNNQuery/100K/k=8         3593 us         3592 us          196 PeakRSS_MB=149.668 items_per_second=27.8373M/s
SelfKNNQuery/100K/k=16        7854 us         7853 us           90 PeakRSS_MB=149.695 items_per_second=12.7338M/s
SelfKNNQuery/100K/k=32       17326 us        17323 us           41 PeakRSS_MB=149.695 items_per_second=5.77264M/s
SelfKNNQuery/100K/k=64       39026 us        39021 us           18 PeakRSS_MB=149.695 items_per_second=2.5627M/s
SelfKNNQuery/100K/k=128     109730 us       109716 us            6 PeakRSS_MB=150.258 items_per_second=911.448k/s
SelfKNNQuery/1M/k=1           1730 us         1730 us          404 PeakRSS_MB=205.445 items_per_second=578.164M/s
SelfKNNQuery/1M/k=2          11315 us        11313 us           62 PeakRSS_MB=204.883 items_per_second=88.3937M/s
SelfKNNQuery/1M/k=4          19510 us        19507 us           36 PeakRSS_MB=205.473 items_per_second=51.2624M/s
SelfKNNQuery/1M/k=8          34889 us        34885 us           20 PeakRSS_MB=205.473 items_per_second=28.666M/s
SelfKNNQuery/1M/k=16         66417 us        66408 us           10 PeakRSS_MB=205.48 items_per_second=15.0584M/s
SelfKNNQuery/1M/k=32        139168 us       139150 us            5 PeakRSS_MB=206.043 items_per_second=7.18651M/s
SelfKNNQuery/1M/k=64        328863 us       328822 us            2 PeakRSS_MB=205.48 items_per_second=3.04116M/s
SelfKNNQuery/1M/k=128      1009312 us      1009172 us            1 PeakRSS_MB=205.48 items_per_second=990.912k/s
SelfKNNQuery/10M/k=1         17149 us        17146 us           41 PeakRSS_MB=677.195 items_per_second=583.215M/s
SelfKNNQuery/10M/k=2        118589 us       118573 us            6 PeakRSS_MB=676.648 items_per_second=84.336M/s
SelfKNNQuery/10M/k=4        200995 us       200964 us            3 PeakRSS_MB=676.676 items_per_second=49.7602M/s
SelfKNNQuery/10M/k=8        352660 us       352616 us            2 PeakRSS_MB=677.246 items_per_second=28.3594M/s
SelfKNNQuery/10M/k=16       664413 us       664332 us            1 PeakRSS_MB=677.254 items_per_second=15.0527M/s
SelfKNNQuery/10M/k=32      1373001 us      1372834 us            1 PeakRSS_MB=677.258 items_per_second=7.2842M/s
SelfKNNQuery/10M/k=64      3381797 us      3381328 us            1 PeakRSS_MB=677.258 items_per_second=2.95742M/s
SelfKNNQuery/10M/k=128    10615818 us     10614592 us            1 PeakRSS_MB=677.258 items_per_second=942.099k/s

```
