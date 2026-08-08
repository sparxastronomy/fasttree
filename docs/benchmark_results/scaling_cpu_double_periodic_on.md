# Scaling Benchmark Results: cpu_double (Periodic BC: ON)
Date: Fri Aug  7 12:25:59 CEST 2026
SFC Curve: Peano-Hilbert
Periodic BC: ON
Build Directory: build_cpu
```
--- Executing ./sfc_encoding_scaling.exe ---
2026-08-07T12:25:59+02:00
Running ./sfc_encoding_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 2.23, 8.40, 7.28
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------
Benchmark                Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------
SFC-Encode/1K        0.054 ms        0.050 ms        13659 PeakRSS_MB=163.297 items_per_second=20.1757M/s
SFC-Encode/10K       0.060 ms        0.045 ms        15492 PeakRSS_MB=162.637 items_per_second=223.505M/s
SFC-Encode/100K      0.114 ms        0.091 ms         7349 PeakRSS_MB=162.477 items_per_second=1.09361G/s
SFC-Encode/1M        0.652 ms        0.620 ms         1096 PeakRSS_MB=233.633 items_per_second=1.61329G/s
SFC-Encode/10M        5.93 ms         5.91 ms          118 PeakRSS_MB=1.02379k items_per_second=1.69282G/s

--- Executing ./gpu_sort_scaling.exe ---
2026-08-07T12:26:18+02:00
Running ./gpu_sort_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 6.49, 9.11, 7.53
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
GPU_Sort/1K        0.237 ms        0.189 ms         3670 PeakRSS_MB=173.176 items_per_second=5.29068M/s
GPU_Sort/10K       0.505 ms        0.340 ms         2050 PeakRSS_MB=177.438 items_per_second=29.4048M/s
GPU_Sort/100K       1.24 ms        0.862 ms          819 PeakRSS_MB=187.426 items_per_second=116.053M/s
GPU_Sort/1M         8.31 ms         7.69 ms           90 PeakRSS_MB=299.516 items_per_second=130.016M/s
GPU_Sort/10M         107 ms          106 ms            7 PeakRSS_MB=1.00928k items_per_second=94.1293M/s

--- Executing ./tree_build_scaling.exe ---
2026-08-07T12:26:38+02:00
Running ./tree_build_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 10.62, 9.76, 7.77
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------------
Benchmark               Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------
TreeBuild/1K         1.32 ms         1.23 ms          577 PeakRSS_MB=155.07 items_per_second=811.484k/s
TreeBuild/10K        2.83 ms         2.55 ms          275 PeakRSS_MB=154.039 items_per_second=3.92194M/s
TreeBuild/100K       7.14 ms         6.54 ms          107 PeakRSS_MB=160.281 items_per_second=15.2975M/s
TreeBuild/1M         32.6 ms         30.6 ms           22 PeakRSS_MB=218.281 items_per_second=32.6608M/s
TreeBuild/10M         247 ms          246 ms            3 PeakRSS_MB=703.605 items_per_second=40.7155M/s

--- Executing ./rebuild_scaling.exe ---
2026-08-07T12:26:55+02:00
Running ./rebuild_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 12.63, 10.32, 8.00
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------
TreeRebuild/1K         1.43 ms         1.34 ms          518 PeakRSS_MB=160.043 items_per_second=744.672k/s
TreeRebuild/10K        2.60 ms         2.34 ms          298 PeakRSS_MB=157.172 items_per_second=4.27153M/s
TreeRebuild/100K       7.37 ms         6.72 ms          104 PeakRSS_MB=167.211 items_per_second=14.8775M/s
TreeRebuild/1M         31.7 ms         30.7 ms           23 PeakRSS_MB=245.809 items_per_second=32.5852M/s
TreeRebuild/10M         235 ms          233 ms            3 PeakRSS_MB=1.01157k items_per_second=42.928M/s

--- Executing ./range_query_scaling.exe ---
2026-08-07T12:27:12+02:00
Running ./range_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 14.87, 11.00, 8.26
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
RangeQuery/1K/R=0.010000            106 us         97.1 us         7097 PeakRSS_MB=153.273 TotalFound=1k
RangeQuery/1K/R=0.100000            107 us         97.2 us         7179 PeakRSS_MB=164.129 TotalFound=1.005k
RangeQuery/1K/R=1.000000            107 us         97.9 us         6957 PeakRSS_MB=154.707 TotalFound=1.035k
RangeQuery/1K/R=10.000000           116 us          102 us         6820 PeakRSS_MB=163.258 TotalFound=6.591k
RangeQuery/1K/R=100.000000          303 us          297 us         2351 PeakRSS_MB=156.637 TotalFound=459.76k
RangeQuery/1K/R=200.000000          331 us          327 us         2130 PeakRSS_MB=160.168 TotalFound=931.404k
RangeQuery/10K/R=0.010000           113 us          101 us         6926 PeakRSS_MB=155.652 TotalFound=1k
RangeQuery/10K/R=0.100000           112 us          101 us         6924 PeakRSS_MB=160.465 TotalFound=1.001k
RangeQuery/10K/R=1.000000           113 us          100 us         6823 PeakRSS_MB=163.68 TotalFound=1.295k
RangeQuery/10K/R=10.000000          169 us          155 us         4597 PeakRSS_MB=159.074 TotalFound=53.444k
RangeQuery/10K/R=100.000000        2110 us         2087 us          336 PeakRSS_MB=160.879 TotalFound=4.38585M
RangeQuery/10K/R=200.000000        3134 us         2758 us          249 PeakRSS_MB=160.332 TotalFound=9.12136M
RangeQuery/100K/R=0.010000          121 us          103 us         6575 PeakRSS_MB=176.73 TotalFound=1k
RangeQuery/100K/R=0.100000          119 us          104 us         6650 PeakRSS_MB=174.016 TotalFound=1.005k
RangeQuery/100K/R=1.000000          131 us          112 us         6179 PeakRSS_MB=178.203 TotalFound=4.145k
RangeQuery/100K/R=10.000000         591 us          591 us         1143 PeakRSS_MB=176.984 TotalFound=506.846k
RangeQuery/100K/R=100.000000      18710 us        14237 us           49 PeakRSS_MB=174.949 TotalFound=43.2318M
RangeQuery/100K/R=200.000000      32589 us        27469 us           26 PeakRSS_MB=177.922 TotalFound=90.3647M
RangeQuery/1M/R=0.010000            125 us          107 us         6484 PeakRSS_MB=334.777 TotalFound=1k
RangeQuery/1M/R=0.100000            126 us          110 us         6419 PeakRSS_MB=333.027 TotalFound=1.139k
RangeQuery/1M/R=1.000000            213 us          194 us         3618 PeakRSS_MB=332.855 TotalFound=39.044k
RangeQuery/1M/R=10.000000          4171 us         3047 us          227 PeakRSS_MB=330.445 TotalFound=5.78161M
RangeQuery/1M/R=100.000000       186530 us       108644 us            7 PeakRSS_MB=333.891 TotalFound=433.516M
RangeQuery/1M/R=200.000000       320551 us       245021 us            4 PeakRSS_MB=332.203 TotalFound=907.777M
RangeQuery/10M/R=0.010000           126 us          109 us         6326 PeakRSS_MB=1.86016k TotalFound=1.001k
RangeQuery/10M/R=0.100000           132 us          114 us         6070 PeakRSS_MB=1.86077k TotalFound=1.793k
RangeQuery/10M/R=1.000000          1233 us         1227 us          568 PeakRSS_MB=1.86086k TotalFound=363.096k
RangeQuery/10M/R=10.000000        59563 us        23717 us           36 PeakRSS_MB=1.86087k TotalFound=55.2471M
RangeQuery/10M/R=100.000000     2475490 us      1815845 us            1 PeakRSS_MB=1.86087k TotalFound=4.39221G
RangeQuery/10M/R=200.000000     3818311 us      1871316 us            1 PeakRSS_MB=1.86085k TotalFound=9.09205G

--- Executing ./knn_query_scaling.exe ---
2026-08-07T12:29:18+02:00
Running ./knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 18.04, 14.92, 10.16
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
KNNQuery/1K/k=1           77.7 us         65.6 us        10715 PeakRSS_MB=173.219
KNNQuery/1K/k=2           79.8 us         66.9 us        10659 PeakRSS_MB=172.07
KNNQuery/1K/k=4           87.0 us         74.1 us         9866 PeakRSS_MB=172.055
KNNQuery/1K/k=8            104 us         89.9 us         7790 PeakRSS_MB=170.348
KNNQuery/1K/k=16           141 us          128 us         5534 PeakRSS_MB=168.496
KNNQuery/1K/k=32           224 us          205 us         3420 PeakRSS_MB=165.887
KNNQuery/1K/k=64          3197 us         3194 us          219 PeakRSS_MB=161.594
KNNQuery/1K/k=128         3798 us         3796 us          185 PeakRSS_MB=154.895
KNNQuery/10K/k=1          81.4 us         67.9 us        10375 PeakRSS_MB=155.039
KNNQuery/10K/k=2          88.7 us         75.1 us         9354 PeakRSS_MB=170.195
KNNQuery/10K/k=4           129 us         88.4 us         8417 PeakRSS_MB=161.531
KNNQuery/10K/k=8           122 us          106 us         6720 PeakRSS_MB=154.043
KNNQuery/10K/k=16          166 us          149 us         4739 PeakRSS_MB=164.777
KNNQuery/10K/k=32          251 us          236 us         2981 PeakRSS_MB=163.902
KNNQuery/10K/k=64         3856 us         3854 us          181 PeakRSS_MB=157.781
KNNQuery/10K/k=128        4706 us         4703 us          149 PeakRSS_MB=166.617
KNNQuery/100K/k=1         90.6 us         74.0 us         9566 PeakRSS_MB=175.766
KNNQuery/100K/k=2          105 us         87.8 us         7981 PeakRSS_MB=177.801
KNNQuery/100K/k=4          121 us          104 us         6869 PeakRSS_MB=177.961
KNNQuery/100K/k=8          146 us          129 us         5428 PeakRSS_MB=179.633
KNNQuery/100K/k=16         194 us          179 us         3925 PeakRSS_MB=177.98
KNNQuery/100K/k=32         294 us          286 us         2435 PeakRSS_MB=178.324
KNNQuery/100K/k=64        4209 us         4200 us          167 PeakRSS_MB=176.227
KNNQuery/100K/k=128       5019 us         5016 us          136 PeakRSS_MB=178.613
KNNQuery/1M/k=1            101 us         82.5 us         8689 PeakRSS_MB=336.734
KNNQuery/1M/k=2            114 us         99.4 us         7250 PeakRSS_MB=336.801
KNNQuery/1M/k=4            130 us          114 us         6275 PeakRSS_MB=338.148
KNNQuery/1M/k=8            156 us          139 us         5054 PeakRSS_MB=336.66
KNNQuery/1M/k=16           211 us          193 us         3599 PeakRSS_MB=337.203
KNNQuery/1M/k=32           324 us          318 us         2208 PeakRSS_MB=338.18
KNNQuery/1M/k=64          4490 us         4479 us          156 PeakRSS_MB=338.707
KNNQuery/1M/k=128         5436 us         5427 us          129 PeakRSS_MB=338.25
KNNQuery/10M/k=1           102 us         88.7 us         7937 PeakRSS_MB=1.8657k
KNNQuery/10M/k=2           119 us          106 us         6042 PeakRSS_MB=1.86539k
KNNQuery/10M/k=4           137 us          122 us         5726 PeakRSS_MB=1.86575k
KNNQuery/10M/k=8           167 us          151 us         4613 PeakRSS_MB=1.86577k
KNNQuery/10M/k=16          230 us          214 us         3290 PeakRSS_MB=1.86584k
KNNQuery/10M/k=32          416 us          372 us         1801 PeakRSS_MB=1.86583k
KNNQuery/10M/k=64         4796 us         4782 us          145 PeakRSS_MB=1.86589k
KNNQuery/10M/k=128        5832 us         5819 us          121 PeakRSS_MB=1.8659k

--- Executing ./self_knn_query_scaling.exe ---
2026-08-08T20:08:23+02:00
Running ./self_knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 43.43, 38.39, 17.89
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------------------
Benchmark                        Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------
SelfKNNQuery/1K/k=1            226 us          175 us         4001 PeakRSS_MB=173.199 items_per_second=5.71124M/s
SelfKNNQuery/1K/k=2            235 us          181 us         3931 PeakRSS_MB=172.613 items_per_second=5.52429M/s
SelfKNNQuery/1K/k=4            243 us          186 us         3754 PeakRSS_MB=171.469 items_per_second=5.36367M/s
SelfKNNQuery/1K/k=8            265 us          203 us         3450 PeakRSS_MB=170.285 items_per_second=4.91848M/s
SelfKNNQuery/1K/k=16           302 us          244 us         2864 PeakRSS_MB=169.113 items_per_second=4.10047M/s
SelfKNNQuery/1K/k=32           377 us          333 us         2100 PeakRSS_MB=165.906 items_per_second=2.99862M/s
SelfKNNQuery/1K/k=64          3377 us         3349 us          209 PeakRSS_MB=161.871 items_per_second=298.604k/s
SelfKNNQuery/1K/k=128         3992 us         3956 us          178 PeakRSS_MB=155.199 items_per_second=252.754k/s
SelfKNNQuery/10K/k=1           905 us          868 us          815 PeakRSS_MB=159.934 items_per_second=11.5148M/s
SelfKNNQuery/10K/k=2          1298 us         1262 us          553 PeakRSS_MB=161.293 items_per_second=7.92561M/s
SelfKNNQuery/10K/k=4          1610 us         1572 us          445 PeakRSS_MB=163.812 items_per_second=6.36299M/s
SelfKNNQuery/10K/k=8          2156 us         2119 us          330 PeakRSS_MB=162.473 items_per_second=4.71955M/s
SelfKNNQuery/10K/k=16         3291 us         3057 us          231 PeakRSS_MB=163.41 items_per_second=3.2716M/s
SelfKNNQuery/10K/k=32         5362 us         4722 us          149 PeakRSS_MB=164.926 items_per_second=2.11755M/s
SelfKNNQuery/10K/k=64        35443 us        35326 us           20 PeakRSS_MB=170.262 items_per_second=283.081k/s
SelfKNNQuery/10K/k=128       43212 us        43119 us           16 PeakRSS_MB=181.672 items_per_second=231.917k/s
SelfKNNQuery/100K/k=1         6455 us         6332 us          109 PeakRSS_MB=179.742 items_per_second=15.7916M/s
SelfKNNQuery/100K/k=2         9784 us         9333 us           74 PeakRSS_MB=179.125 items_per_second=10.7144M/s
SelfKNNQuery/100K/k=4        12821 us        12022 us           58 PeakRSS_MB=183.285 items_per_second=8.31796M/s
SelfKNNQuery/100K/k=8        18042 us        16413 us           42 PeakRSS_MB=187.484 items_per_second=6.09278M/s
SelfKNNQuery/100K/k=16       28161 us        25386 us           28 PeakRSS_MB=199.605 items_per_second=3.93923M/s
SelfKNNQuery/100K/k=32       49382 us        43649 us           16 PeakRSS_MB=225.301 items_per_second=2.29098M/s
SelfKNNQuery/100K/k=64      376408 us       375921 us            2 PeakRSS_MB=282.07 items_per_second=266.013k/s
SelfKNNQuery/100K/k=128     458010 us       457799 us            2 PeakRSS_MB=382.461 items_per_second=218.436k/s
SelfKNNQuery/1M/k=1          57866 us        56932 us           12 PeakRSS_MB=351.152 items_per_second=17.5649M/s
SelfKNNQuery/1M/k=2          92341 us        89747 us            8 PeakRSS_MB=365.605 items_per_second=11.1425M/s
SelfKNNQuery/1M/k=4         122439 us       118961 us            6 PeakRSS_MB=395.629 items_per_second=8.40612M/s
SelfKNNQuery/1M/k=8         174943 us       170392 us            4 PeakRSS_MB=461.996 items_per_second=5.86882M/s
SelfKNNQuery/1M/k=16        277551 us       272081 us            3 PeakRSS_MB=583.852 items_per_second=3.67537M/s
SelfKNNQuery/1M/k=32        474087 us       458781 us            2 PeakRSS_MB=831.391 items_per_second=2.17969M/s
SelfKNNQuery/1M/k=64       4002778 us      3999135 us            1 PeakRSS_MB=1.3237k items_per_second=250.054k/s
SelfKNNQuery/1M/k=128      4852303 us      4849191 us            1 PeakRSS_MB=2.30049k items_per_second=206.22k/s
SelfKNNQuery/10M/k=1        650834 us       643640 us            1 PeakRSS_MB=2.0148k items_per_second=15.5366M/s
SelfKNNQuery/10M/k=2       1116360 us      1095626 us            1 PeakRSS_MB=2.16938k items_per_second=9.1272M/s
SelfKNNQuery/10M/k=4       1491538 us      1476121 us            1 PeakRSS_MB=2.47329k items_per_second=6.77451M/s
SelfKNNQuery/10M/k=8       2086588 us      2053378 us            1 PeakRSS_MB=3.08319k items_per_second=4.87002M/s
SelfKNNQuery/10M/k=16      3199726 us      3140145 us            1 PeakRSS_MB=4.30528k items_per_second=3.18457M/s
SelfKNNQuery/10M/k=32      5345218 us      5288688 us            1 PeakRSS_MB=6.74549k items_per_second=1.89083M/s
SelfKNNQuery/10M/k=64     43498765 us     43455452 us            1 PeakRSS_MB=11.6303k items_per_second=230.121k/s
SelfKNNQuery/10M/k=128    51960212 us     51919546 us            1 PeakRSS_MB=21.395k items_per_second=192.606k/s

```
