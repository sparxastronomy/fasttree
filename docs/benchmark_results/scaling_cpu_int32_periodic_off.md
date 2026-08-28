# Scaling Benchmark Results: cpu_int32 (Periodic BC: OFF)
Date: Fri Aug  7 12:35:02 CEST 2026
SFC Curve: Peano-Hilbert
Periodic BC: OFF
Build Directory: build_int32
```
--- Executing ./sfc_encoding_scaling.exe ---
2026-08-07T12:35:02+02:00
Running ./sfc_encoding_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 2.41, 11.31, 10.78
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------
Benchmark                Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------
SFC-Encode/1K        0.050 ms        0.047 ms        12815 PeakRSS_MB=168.297 items_per_second=21.112M/s
SFC-Encode/10K       0.057 ms        0.044 ms        15571 PeakRSS_MB=168.297 items_per_second=228.823M/s
SFC-Encode/100K      0.112 ms        0.094 ms         7521 PeakRSS_MB=169.02 items_per_second=1.06184G/s
SFC-Encode/1M        0.630 ms        0.610 ms         1130 PeakRSS_MB=215.098 items_per_second=1.64005G/s
SFC-Encode/10M        5.70 ms         5.68 ms          123 PeakRSS_MB=801.719 items_per_second=1.76209G/s

--- Executing ./gpu_sort_scaling.exe ---
2026-08-07T12:35:23+02:00
Running ./gpu_sort_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 2.01, 10.64, 10.57
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
GPU_Sort/1K        0.392 ms        0.196 ms         3559 PeakRSS_MB=172.543 items_per_second=5.10398M/s
GPU_Sort/10K       0.651 ms        0.327 ms         2183 PeakRSS_MB=171.758 items_per_second=30.5788M/s
GPU_Sort/100K       1.26 ms        0.887 ms         1017 PeakRSS_MB=181.02 items_per_second=112.716M/s
GPU_Sort/1M         8.45 ms         7.36 ms          100 PeakRSS_MB=287.418 items_per_second=135.796M/s
GPU_Sort/10M         109 ms         99.7 ms            7 PeakRSS_MB=891.875 items_per_second=100.305M/s

--- Executing ./tree_build_scaling.exe ---
2026-08-07T12:35:45+02:00
Running ./tree_build_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 11.78, 12.17, 11.07
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------------
Benchmark               Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------
TreeBuild/1K         1.08 ms         1.00 ms          700 PeakRSS_MB=162.34 items_per_second=0.999949M/s
TreeBuild/10K        2.66 ms         2.38 ms          292 PeakRSS_MB=155.07 items_per_second=4.2008M/s
TreeBuild/100K       6.06 ms         5.52 ms          126 PeakRSS_MB=159.309 items_per_second=18.1081M/s
TreeBuild/1M         29.6 ms         28.7 ms           24 PeakRSS_MB=202.711 items_per_second=34.8553M/s
TreeBuild/10M         227 ms          226 ms            3 PeakRSS_MB=588.32 items_per_second=44.332M/s

--- Executing ./rebuild_scaling.exe ---
2026-08-07T12:36:02+02:00
Running ./rebuild_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 14.20, 12.76, 11.29
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------
TreeRebuild/1K         1.11 ms         1.02 ms          686 PeakRSS_MB=158.316 items_per_second=984.498k/s
TreeRebuild/10K        2.65 ms         2.35 ms          297 PeakRSS_MB=158.613 items_per_second=4.2611M/s
TreeRebuild/100K       5.97 ms         5.39 ms          130 PeakRSS_MB=161.406 items_per_second=18.5393M/s
TreeRebuild/1M         29.2 ms         28.3 ms           25 PeakRSS_MB=227.629 items_per_second=35.3868M/s
TreeRebuild/10M         218 ms          187 ms            3 PeakRSS_MB=894.797 items_per_second=53.3965M/s

--- Executing ./range_query_scaling.exe ---
2026-08-07T12:36:19+02:00
Running ./range_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 16.96, 13.36, 11.50
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
RangeQuery/1K/R=0.010000            105 us         95.8 us         7209 PeakRSS_MB=156.867 TotalFound=1k
RangeQuery/1K/R=0.100000            107 us         96.3 us         7141 PeakRSS_MB=166.473 TotalFound=1.005k
RangeQuery/1K/R=1.000000            107 us         97.1 us         7214 PeakRSS_MB=158.758 TotalFound=1.035k
RangeQuery/1K/R=10.000000           116 us          103 us         6747 PeakRSS_MB=161.105 TotalFound=6.587k
RangeQuery/1K/R=100.000000          417 us          414 us         1703 PeakRSS_MB=160.551 TotalFound=459.676k
RangeQuery/1K/R=200.000000          563 us          563 us         1227 PeakRSS_MB=155.402 TotalFound=931.389k
RangeQuery/10K/R=0.010000           111 us         99.9 us         6998 PeakRSS_MB=159.605 TotalFound=1k
RangeQuery/10K/R=0.100000           109 us         98.4 us         6941 PeakRSS_MB=163.48 TotalFound=1k
RangeQuery/10K/R=1.000000           112 us          102 us         6888 PeakRSS_MB=159.32 TotalFound=1.289k
RangeQuery/10K/R=10.000000          187 us          174 us         4010 PeakRSS_MB=160.336 TotalFound=53.336k
RangeQuery/10K/R=100.000000        2499 us         2345 us          299 PeakRSS_MB=164.375 TotalFound=4.38525M
RangeQuery/10K/R=200.000000        3970 us         3289 us          216 PeakRSS_MB=159.836 TotalFound=9.12135M
RangeQuery/100K/R=0.010000          118 us          102 us         6770 PeakRSS_MB=170.457 TotalFound=1k
RangeQuery/100K/R=0.100000          119 us          103 us         6765 PeakRSS_MB=170.918 TotalFound=1.003k
RangeQuery/100K/R=1.000000          130 us          113 us         6168 PeakRSS_MB=169.945 TotalFound=4.03k
RangeQuery/100K/R=10.000000         665 us          657 us         1052 PeakRSS_MB=171.25 TotalFound=506.23k
RangeQuery/100K/R=100.000000      20596 us        15632 us           48 PeakRSS_MB=170.133 TotalFound=43.2297M
RangeQuery/100K/R=200.000000      36770 us        35097 us           20 PeakRSS_MB=171.727 TotalFound=90.3636M
RangeQuery/1M/R=0.010000            124 us          106 us         6053 PeakRSS_MB=273.82 TotalFound=1k
RangeQuery/1M/R=0.100000            125 us          108 us         6410 PeakRSS_MB=274.336 TotalFound=1.079k
RangeQuery/1M/R=1.000000            235 us          222 us         3104 PeakRSS_MB=277.496 TotalFound=37.793k
RangeQuery/1M/R=10.000000          4456 us         3180 us          226 PeakRSS_MB=274.578 TotalFound=5.78007M
RangeQuery/1M/R=100.000000       182072 us        99641 us            8 PeakRSS_MB=274.004 TotalFound=433.482M
RangeQuery/1M/R=200.000000       346658 us       184004 us            4 PeakRSS_MB=275.984 TotalFound=907.76M
RangeQuery/10M/R=0.010000           124 us          106 us         6433 PeakRSS_MB=1.28593k TotalFound=1.001k
RangeQuery/10M/R=0.100000           154 us          116 us         6250 PeakRSS_MB=1.28687k TotalFound=1.457k
RangeQuery/10M/R=1.000000          1244 us         1238 us          566 PeakRSS_MB=1.28639k TotalFound=349.304k
RangeQuery/10M/R=10.000000        56327 us        21685 us           32 PeakRSS_MB=1.28693k TotalFound=55.2257M
RangeQuery/10M/R=100.000000     1815084 us       995283 us            1 PeakRSS_MB=1.28699k TotalFound=4.39188G
RangeQuery/10M/R=200.000000     3511781 us      3449373 us            1 PeakRSS_MB=1.28702k TotalFound=9.0919G

--- Executing ./knn_query_scaling.exe ---
2026-08-07T12:38:34+02:00
Running ./knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 19.19, 15.27, 12.48
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
KNNQuery/1K/k=1           79.0 us         67.3 us        10616 PeakRSS_MB=176.133
KNNQuery/1K/k=2           84.3 us         71.2 us         9905 PeakRSS_MB=176.133
KNNQuery/1K/k=4           94.2 us         79.7 us         8989 PeakRSS_MB=175.551
KNNQuery/1K/k=8            116 us          101 us         7175 PeakRSS_MB=174.34
KNNQuery/1K/k=16           157 us          143 us         4930 PeakRSS_MB=173.105
KNNQuery/1K/k=32           237 us          222 us         3183 PeakRSS_MB=171.051
KNNQuery/1K/k=64          1797 us         1795 us          386 PeakRSS_MB=167.508
KNNQuery/1K/k=128         2155 us         2072 us          329 PeakRSS_MB=160.625
KNNQuery/10K/k=1          86.3 us         71.4 us         8835 PeakRSS_MB=153.922
KNNQuery/10K/k=2          93.3 us         78.3 us         8970 PeakRSS_MB=161.508
KNNQuery/10K/k=4           149 us         96.1 us         7427 PeakRSS_MB=174.633
KNNQuery/10K/k=8           132 us          116 us         6037 PeakRSS_MB=168.391
KNNQuery/10K/k=16          177 us          161 us         4375 PeakRSS_MB=160.715
KNNQuery/10K/k=32          267 us          256 us         2758 PeakRSS_MB=156.48
KNNQuery/10K/k=64         2110 us         2109 us          330 PeakRSS_MB=168.723
KNNQuery/10K/k=128        2625 us         2614 us          267 PeakRSS_MB=164.867
KNNQuery/100K/k=1         94.2 us         75.3 us         9340 PeakRSS_MB=171.496
KNNQuery/100K/k=2          109 us         88.7 us         8013 PeakRSS_MB=174.348
KNNQuery/100K/k=4          128 us          108 us         6613 PeakRSS_MB=170.332
KNNQuery/100K/k=8          153 us          131 us         5242 PeakRSS_MB=175.465
KNNQuery/100K/k=16         202 us          182 us         3796 PeakRSS_MB=170.805
KNNQuery/100K/k=32         303 us          295 us         2363 PeakRSS_MB=172.945
KNNQuery/100K/k=64        2204 us         2203 us          315 PeakRSS_MB=174.586
KNNQuery/100K/k=128       2700 us         2699 us          259 PeakRSS_MB=173.535
KNNQuery/1M/k=1            107 us         82.8 us         8459 PeakRSS_MB=277.105
KNNQuery/1M/k=2            127 us          102 us         6871 PeakRSS_MB=277.258
KNNQuery/1M/k=4            146 us          123 us         5821 PeakRSS_MB=277.34
KNNQuery/1M/k=8            179 us          154 us         4539 PeakRSS_MB=279.09
KNNQuery/1M/k=16           232 us          210 us         3374 PeakRSS_MB=275.168
KNNQuery/1M/k=32           351 us          350 us         1999 PeakRSS_MB=278.969
KNNQuery/1M/k=64          2322 us         2317 us          301 PeakRSS_MB=279.371
KNNQuery/1M/k=128         2874 us         2872 us          245 PeakRSS_MB=281.41
KNNQuery/10M/k=1           105 us         87.0 us         8095 PeakRSS_MB=1.29127k
KNNQuery/10M/k=2           126 us          107 us         6569 PeakRSS_MB=1.29168k
KNNQuery/10M/k=4           146 us          126 us         5578 PeakRSS_MB=1.29192k
KNNQuery/10M/k=8           175 us          152 us         4523 PeakRSS_MB=1.29118k
KNNQuery/10M/k=16          229 us          207 us         3351 PeakRSS_MB=1.29179k
KNNQuery/10M/k=32          356 us          354 us         1980 PeakRSS_MB=1.29191k
KNNQuery/10M/k=64         2579 us         2572 us          272 PeakRSS_MB=1.29139k
KNNQuery/10M/k=128        3152 us         3145 us          221 PeakRSS_MB=1.29153k

--- Executing ./self_knn_query_scaling.exe ---
2026-08-27T14:52:26+02:00
Running ./self_knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.10, 12.24, 10.99
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------------------
Benchmark                        Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------
SelfKNNQuery/1K/k=1           88.7 us         86.7 us         8028 PeakRSS_MB=174.945 items_per_second=11.5284M/s
SelfKNNQuery/1K/k=2            167 us          166 us         4240 PeakRSS_MB=174.945 items_per_second=6.02756M/s
SelfKNNQuery/1K/k=4            218 us          218 us         3205 PeakRSS_MB=174.379 items_per_second=4.593M/s
SelfKNNQuery/1K/k=8            323 us          322 us         2172 PeakRSS_MB=173.809 items_per_second=3.10184M/s
SelfKNNQuery/1K/k=16           503 us          502 us         1385 PeakRSS_MB=172.539 items_per_second=1.99262M/s
SelfKNNQuery/1K/k=32           879 us          870 us          800 PeakRSS_MB=170.578 items_per_second=1.14898M/s
SelfKNNQuery/1K/k=64          1058 us         1053 us          673 PeakRSS_MB=166.754 items_per_second=949.283k/s
SelfKNNQuery/1K/k=128         1790 us         1759 us          400 PeakRSS_MB=159.828 items_per_second=568.349k/s
SelfKNNQuery/10K/k=1           325 us          317 us         2223 PeakRSS_MB=150.41 items_per_second=31.5577M/s
SelfKNNQuery/10K/k=2           950 us          947 us          739 PeakRSS_MB=160.074 items_per_second=10.5586M/s
SelfKNNQuery/10K/k=4          1439 us         1427 us          491 PeakRSS_MB=174.262 items_per_second=7.00756M/s
SelfKNNQuery/10K/k=8          2140 us         2138 us          326 PeakRSS_MB=164.285 items_per_second=4.67689M/s
SelfKNNQuery/10K/k=16         3452 us         3447 us          204 PeakRSS_MB=156.934 items_per_second=2.90147M/s
SelfKNNQuery/10K/k=32         6200 us         6094 us          114 PeakRSS_MB=172.855 items_per_second=1.64106M/s
SelfKNNQuery/10K/k=64        10102 us         9394 us           74 PeakRSS_MB=170.105 items_per_second=1.06448M/s
SelfKNNQuery/10K/k=128       18490 us        16159 us           43 PeakRSS_MB=185.621 items_per_second=618.868k/s
SelfKNNQuery/100K/k=1         3374 us         3340 us          210 PeakRSS_MB=168.895 items_per_second=29.9379M/s
SelfKNNQuery/100K/k=2         9545 us         9499 us           74 PeakRSS_MB=174.785 items_per_second=10.527M/s
SelfKNNQuery/100K/k=4        13926 us        13919 us           50 PeakRSS_MB=179.129 items_per_second=7.18452M/s
SelfKNNQuery/100K/k=8        20633 us        20614 us           34 PeakRSS_MB=183.996 items_per_second=4.85116M/s
SelfKNNQuery/100K/k=16       32502 us        32434 us           21 PeakRSS_MB=196.934 items_per_second=3.08319M/s
SelfKNNQuery/100K/k=32       55501 us        55142 us           13 PeakRSS_MB=227.152 items_per_second=1.8135M/s
SelfKNNQuery/100K/k=64       87665 us        86795 us            8 PeakRSS_MB=276.523 items_per_second=1.15214M/s
SelfKNNQuery/100K/k=128     165350 us       162402 us            4 PeakRSS_MB=379.801 items_per_second=615.756k/s
SelfKNNQuery/1M/k=1          35115 us        35066 us           20 PeakRSS_MB=292.777 items_per_second=28.518M/s
SelfKNNQuery/1M/k=2          99649 us        99573 us            7 PeakRSS_MB=308.094 items_per_second=10.0428M/s
SelfKNNQuery/1M/k=4         146075 us       146040 us            5 PeakRSS_MB=340.699 items_per_second=6.84744M/s
SelfKNNQuery/1M/k=8         215453 us       215389 us            3 PeakRSS_MB=408.672 items_per_second=4.64276M/s
SelfKNNQuery/1M/k=16        337537 us       337461 us            2 PeakRSS_MB=532.828 items_per_second=2.9633M/s
SelfKNNQuery/1M/k=32        569349 us       569042 us            1 PeakRSS_MB=781.141 items_per_second=1.75734M/s
SelfKNNQuery/1M/k=64        896036 us       874647 us            1 PeakRSS_MB=1.2703k items_per_second=1.14332M/s
SelfKNNQuery/1M/k=128      1715627 us      1705113 us            1 PeakRSS_MB=2.24687k items_per_second=586.471k/s
SelfKNNQuery/10M/k=1        374390 us       374302 us            2 PeakRSS_MB=1.4454k items_per_second=26.7164M/s
SelfKNNQuery/10M/k=2       1077438 us      1076543 us            1 PeakRSS_MB=1.59873k items_per_second=9.28899M/s
SelfKNNQuery/10M/k=4       1558230 us      1557295 us            1 PeakRSS_MB=1.90407k items_per_second=6.42139M/s
SelfKNNQuery/10M/k=8       2286758 us      2284540 us            1 PeakRSS_MB=2.51369k items_per_second=4.37725M/s
SelfKNNQuery/10M/k=16      3557481 us      3554769 us            1 PeakRSS_MB=3.73513k items_per_second=2.81312M/s
SelfKNNQuery/10M/k=32      5966844 us      5963197 us            1 PeakRSS_MB=6.17654k items_per_second=1.67695M/s
SelfKNNQuery/10M/k=64      9336259 us      9329612 us            1 PeakRSS_MB=11.0594k items_per_second=1.07186M/s
SelfKNNQuery/10M/k=128    17776611 us     17769404 us            1 PeakRSS_MB=20.8243k items_per_second=562.765k/s

```
