# Scaling Benchmark Results: cpu_double (Periodic BC: OFF)
Date: Fri Aug  7 12:17:08 CEST 2026
SFC Curve: Peano-Hilbert
Periodic BC: OFF
Build Directory: build_cpu
```
--- Executing ./sfc_encoding_scaling.exe ---
2026-08-07T12:17:08+02:00
Running ./sfc_encoding_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 0.92, 0.59, 2.81
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------
Benchmark                Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------
SFC-Encode/1K        0.053 ms        0.050 ms        13817 PeakRSS_MB=168.359 items_per_second=20.168M/s
SFC-Encode/10K       0.057 ms        0.046 ms        15337 PeakRSS_MB=167.734 items_per_second=216.925M/s
SFC-Encode/100K      0.114 ms        0.099 ms         7168 PeakRSS_MB=165.609 items_per_second=1.01084G/s
SFC-Encode/1M        0.645 ms        0.620 ms         1087 PeakRSS_MB=234.426 items_per_second=1.61339G/s
SFC-Encode/10M        5.97 ms         5.92 ms          118 PeakRSS_MB=1.02533k items_per_second=1.68852G/s

--- Executing ./gpu_sort_scaling.exe ---
2026-08-07T12:17:26+02:00
Running ./gpu_sort_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 0.94, 0.62, 2.77
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
GPU_Sort/1K        0.238 ms        0.188 ms         3686 PeakRSS_MB=176.371 items_per_second=5.32507M/s
GPU_Sort/10K       0.497 ms        0.335 ms         2079 PeakRSS_MB=175.871 items_per_second=29.8598M/s
GPU_Sort/100K       1.22 ms        0.846 ms          823 PeakRSS_MB=185.168 items_per_second=118.143M/s
GPU_Sort/1M         8.37 ms         7.75 ms           89 PeakRSS_MB=302.324 items_per_second=128.983M/s
GPU_Sort/10M         108 ms          107 ms            7 PeakRSS_MB=1.01005k items_per_second=93.6485M/s

--- Executing ./tree_build_scaling.exe ---
2026-08-07T12:17:46+02:00
Running ./tree_build_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 5.77, 1.78, 3.11
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------------
Benchmark               Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------
TreeBuild/1K         1.55 ms         1.45 ms          479 PeakRSS_MB=161.758 items_per_second=691.373k/s
TreeBuild/10K        2.76 ms         2.46 ms          286 PeakRSS_MB=157.57 items_per_second=4.07204M/s
TreeBuild/100K       7.77 ms         7.17 ms           97 PeakRSS_MB=164.855 items_per_second=13.9383M/s
TreeBuild/1M         32.3 ms         31.3 ms           22 PeakRSS_MB=219.168 items_per_second=31.9252M/s
TreeBuild/10M         250 ms          248 ms            3 PeakRSS_MB=706.824 items_per_second=40.3657M/s

--- Executing ./rebuild_scaling.exe ---
2026-08-07T12:18:03+02:00
Running ./rebuild_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 15.63, 4.08, 3.83
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------
TreeRebuild/1K         1.54 ms         1.43 ms          486 PeakRSS_MB=160.195 items_per_second=697.093k/s
TreeRebuild/10K        2.62 ms         2.35 ms          297 PeakRSS_MB=156.098 items_per_second=4.25476M/s
TreeRebuild/100K       7.07 ms         6.43 ms          108 PeakRSS_MB=163.965 items_per_second=15.5609M/s
TreeRebuild/1M         32.3 ms         31.2 ms           23 PeakRSS_MB=244.949 items_per_second=32.0979M/s
TreeRebuild/10M         235 ms          233 ms            3 PeakRSS_MB=1.01018k items_per_second=43.0013M/s

--- Executing ./range_query_scaling.exe ---
2026-08-07T12:18:20+02:00
Running ./range_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 11.48, 3.88, 3.77
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
RangeQuery/1K/R=0.010000            107 us         96.7 us         7148 PeakRSS_MB=158.375 TotalFound=1k
RangeQuery/1K/R=0.100000            148 us          101 us         7049 PeakRSS_MB=165.301 TotalFound=1.005k
RangeQuery/1K/R=1.000000            111 us         97.1 us         7094 PeakRSS_MB=159.297 TotalFound=1.035k
RangeQuery/1K/R=10.000000           117 us          103 us         6785 PeakRSS_MB=163.391 TotalFound=6.591k
RangeQuery/1K/R=100.000000          303 us          295 us         2352 PeakRSS_MB=160.172 TotalFound=459.76k
RangeQuery/1K/R=200.000000          332 us          327 us         2138 PeakRSS_MB=161.988 TotalFound=931.404k
RangeQuery/10K/R=0.010000           115 us          100 us         7001 PeakRSS_MB=158.547 TotalFound=1k
RangeQuery/10K/R=0.100000           116 us         99.8 us         6960 PeakRSS_MB=162.344 TotalFound=1.001k
RangeQuery/10K/R=1.000000           117 us          101 us         6848 PeakRSS_MB=166.617 TotalFound=1.295k
RangeQuery/10K/R=10.000000          174 us          151 us         4596 PeakRSS_MB=165.344 TotalFound=53.444k
RangeQuery/10K/R=100.000000        2156 us         2115 us          331 PeakRSS_MB=160.277 TotalFound=4.38585M
RangeQuery/10K/R=200.000000        3178 us         2775 us          254 PeakRSS_MB=165.875 TotalFound=9.12136M
RangeQuery/100K/R=0.010000          172 us          108 us         6738 PeakRSS_MB=181.051 TotalFound=1k
RangeQuery/100K/R=0.100000          125 us          102 us         6847 PeakRSS_MB=180.742 TotalFound=1.005k
RangeQuery/100K/R=1.000000          134 us          111 us         6378 PeakRSS_MB=179.562 TotalFound=4.145k
RangeQuery/100K/R=10.000000         589 us          588 us         1138 PeakRSS_MB=179.766 TotalFound=506.846k
RangeQuery/100K/R=100.000000      18548 us        14005 us           48 PeakRSS_MB=181.047 TotalFound=43.2318M
RangeQuery/100K/R=200.000000      32488 us        25415 us           30 PeakRSS_MB=181.535 TotalFound=90.3647M
RangeQuery/1M/R=0.010000            129 us          103 us         6686 PeakRSS_MB=337.637 TotalFound=1k
RangeQuery/1M/R=0.100000            130 us          108 us         6595 PeakRSS_MB=337.211 TotalFound=1.139k
RangeQuery/1M/R=1.000000            212 us          189 us         3694 PeakRSS_MB=338.504 TotalFound=39.044k
RangeQuery/1M/R=10.000000          4173 us         3016 us          230 PeakRSS_MB=338.422 TotalFound=5.78161M
RangeQuery/1M/R=100.000000       190827 us       143340 us            4 PeakRSS_MB=336.129 TotalFound=433.516M
RangeQuery/1M/R=200.000000       322086 us       171828 us            4 PeakRSS_MB=338.387 TotalFound=907.777M
RangeQuery/10M/R=0.010000           129 us          107 us         6456 PeakRSS_MB=1.86306k TotalFound=1.001k
RangeQuery/10M/R=0.100000           135 us          112 us         6231 PeakRSS_MB=1.86319k TotalFound=1.793k
RangeQuery/10M/R=1.000000          1228 us         1220 us          572 PeakRSS_MB=1.86328k TotalFound=363.096k
RangeQuery/10M/R=10.000000        59493 us        26316 us           30 PeakRSS_MB=1.86326k TotalFound=55.2471M
RangeQuery/10M/R=100.000000     2505372 us      1467598 us            1 PeakRSS_MB=1.86299k TotalFound=4.39221G
RangeQuery/10M/R=200.000000     3685151 us      2034705 us            1 PeakRSS_MB=1.86329k TotalFound=9.09205G

--- Executing ./knn_query_scaling.exe ---
2026-08-07T12:20:26+02:00
Running ./knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 23.25, 11.64, 6.73
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
KNNQuery/1K/k=1           72.4 us         66.9 us        10565 PeakRSS_MB=177.344
KNNQuery/1K/k=2           77.8 us         69.4 us         9998 PeakRSS_MB=176.773
KNNQuery/1K/k=4           85.8 us         76.9 us         9043 PeakRSS_MB=176.191
KNNQuery/1K/k=8            105 us         95.5 us         7514 PeakRSS_MB=174.988
KNNQuery/1K/k=16           141 us          132 us         5396 PeakRSS_MB=173.234
KNNQuery/1K/k=32           219 us          208 us         3369 PeakRSS_MB=169.98
KNNQuery/1K/k=64          3215 us         3208 us          218 PeakRSS_MB=165.949
KNNQuery/1K/k=128         3797 us         3789 us          184 PeakRSS_MB=159.27
KNNQuery/10K/k=1          79.9 us         70.9 us         9897 PeakRSS_MB=153.773
KNNQuery/10K/k=2          87.5 us         77.8 us         9170 PeakRSS_MB=173.449
KNNQuery/10K/k=4           101 us         89.6 us         7892 PeakRSS_MB=165.918
KNNQuery/10K/k=8           123 us          111 us         6431 PeakRSS_MB=155.68
KNNQuery/10K/k=16          168 us          154 us         4615 PeakRSS_MB=168.121
KNNQuery/10K/k=32          251 us          240 us         2941 PeakRSS_MB=168.32
KNNQuery/10K/k=64         3855 us         3851 us          182 PeakRSS_MB=161.133
KNNQuery/10K/k=128        4727 us         4714 us          149 PeakRSS_MB=168.859
KNNQuery/100K/k=1         92.3 us         77.4 us         8936 PeakRSS_MB=180.566
KNNQuery/100K/k=2          105 us         91.3 us         7685 PeakRSS_MB=178.898
KNNQuery/100K/k=4          122 us          108 us         6579 PeakRSS_MB=179.238
KNNQuery/100K/k=8          148 us          131 us         5260 PeakRSS_MB=181.629
KNNQuery/100K/k=16         194 us          180 us         3885 PeakRSS_MB=180.309
KNNQuery/100K/k=32         295 us          286 us         2423 PeakRSS_MB=179.223
KNNQuery/100K/k=64        4214 us         4213 us          166 PeakRSS_MB=184.012
KNNQuery/100K/k=128       5021 us         5017 us          139 PeakRSS_MB=183.391
KNNQuery/1M/k=1            101 us         85.4 us         8348 PeakRSS_MB=338.555
KNNQuery/1M/k=2            114 us          101 us         6984 PeakRSS_MB=338.844
KNNQuery/1M/k=4            132 us          116 us         6080 PeakRSS_MB=337.988
KNNQuery/1M/k=8            160 us          143 us         4904 PeakRSS_MB=340.141
KNNQuery/1M/k=16           210 us          193 us         3603 PeakRSS_MB=339.012
KNNQuery/1M/k=32           322 us          316 us         2155 PeakRSS_MB=338.195
KNNQuery/1M/k=64          4497 us         4491 us          156 PeakRSS_MB=338.703
KNNQuery/1M/k=128         5426 us         5421 us          129 PeakRSS_MB=338.855
KNNQuery/10M/k=1           102 us         89.7 us         7689 PeakRSS_MB=1.86963k
KNNQuery/10M/k=2           120 us          107 us         6577 PeakRSS_MB=1.86968k
KNNQuery/10M/k=4           138 us          124 us         5673 PeakRSS_MB=1.86987k
KNNQuery/10M/k=8           168 us          153 us         4641 PeakRSS_MB=1.86991k
KNNQuery/10M/k=16          232 us          216 us         3276 PeakRSS_MB=1.86993k
KNNQuery/10M/k=32          390 us          388 us         1802 PeakRSS_MB=1.86996k
KNNQuery/10M/k=64         4920 us         4868 us          145 PeakRSS_MB=1.86998k
KNNQuery/10M/k=128        5842 us         5837 us          120 PeakRSS_MB=1.86999k

--- Executing ./self_knn_query_scaling.exe ---
2026-08-08T20:02:34+02:00
Running ./self_knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.36, 0.76, 0.47
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------------------
Benchmark                        Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------
SelfKNNQuery/1K/k=1            231 us          175 us         3873 PeakRSS_MB=174.578 items_per_second=5.72801M/s
SelfKNNQuery/1K/k=2            242 us          181 us         3850 PeakRSS_MB=174.578 items_per_second=5.53773M/s
SelfKNNQuery/1K/k=4            253 us          186 us         3797 PeakRSS_MB=173.41 items_per_second=5.36786M/s
SelfKNNQuery/1K/k=8            270 us          204 us         3459 PeakRSS_MB=172.254 items_per_second=4.89255M/s
SelfKNNQuery/1K/k=16           307 us          244 us         2863 PeakRSS_MB=170.418 items_per_second=4.09315M/s
SelfKNNQuery/1K/k=32           368 us          334 us         2104 PeakRSS_MB=167.387 items_per_second=2.99577M/s
SelfKNNQuery/1K/k=64          3374 us         3350 us          209 PeakRSS_MB=163.348 items_per_second=298.549k/s
SelfKNNQuery/1K/k=128         3975 us         3949 us          177 PeakRSS_MB=156.102 items_per_second=253.229k/s
SelfKNNQuery/10K/k=1           905 us          879 us          811 PeakRSS_MB=165.121 items_per_second=11.3777M/s
SelfKNNQuery/10K/k=2          1299 us         1269 us          553 PeakRSS_MB=162.051 items_per_second=7.87761M/s
SelfKNNQuery/10K/k=4          1610 us         1578 us          447 PeakRSS_MB=162.84 items_per_second=6.33547M/s
SelfKNNQuery/10K/k=8          2162 us         2127 us          330 PeakRSS_MB=162.469 items_per_second=4.70131M/s
SelfKNNQuery/10K/k=16         3310 us         3072 us          341 PeakRSS_MB=162.32 items_per_second=3.25545M/s
SelfKNNQuery/10K/k=32         5339 us         4718 us          150 PeakRSS_MB=167.094 items_per_second=2.1195M/s
SelfKNNQuery/10K/k=64        35431 us        35338 us           20 PeakRSS_MB=173.168 items_per_second=282.983k/s
SelfKNNQuery/10K/k=128       43340 us        43268 us           16 PeakRSS_MB=182.355 items_per_second=231.118k/s
SelfKNNQuery/100K/k=1         6518 us         6396 us          109 PeakRSS_MB=177.105 items_per_second=15.6354M/s
SelfKNNQuery/100K/k=2         9798 us         9385 us           74 PeakRSS_MB=180.984 items_per_second=10.6558M/s
SelfKNNQuery/100K/k=4        12764 us        11984 us           59 PeakRSS_MB=184.918 items_per_second=8.34467M/s
SelfKNNQuery/100K/k=8        18120 us        16510 us           43 PeakRSS_MB=191.324 items_per_second=6.05691M/s
SelfKNNQuery/100K/k=16       28373 us        25092 us           28 PeakRSS_MB=201.094 items_per_second=3.98529M/s
SelfKNNQuery/100K/k=32       48873 us        43245 us           16 PeakRSS_MB=223.312 items_per_second=2.31241M/s
SelfKNNQuery/100K/k=64      377105 us       376745 us            2 PeakRSS_MB=279.375 items_per_second=265.432k/s
SelfKNNQuery/100K/k=128     458014 us       457801 us            2 PeakRSS_MB=382.516 items_per_second=218.435k/s
SelfKNNQuery/1M/k=1          57997 us        57030 us           13 PeakRSS_MB=348.523 items_per_second=17.5346M/s
SelfKNNQuery/1M/k=2          92663 us        90985 us            8 PeakRSS_MB=367.812 items_per_second=10.9908M/s
SelfKNNQuery/1M/k=4         123346 us       119495 us            6 PeakRSS_MB=392.758 items_per_second=8.36857M/s
SelfKNNQuery/1M/k=8         174249 us       168155 us            4 PeakRSS_MB=455.566 items_per_second=5.94689M/s
SelfKNNQuery/1M/k=16        277682 us       269250 us            3 PeakRSS_MB=579.535 items_per_second=3.71402M/s
SelfKNNQuery/1M/k=32        476520 us       469743 us            2 PeakRSS_MB=827.176 items_per_second=2.12882M/s
SelfKNNQuery/1M/k=64       4006652 us      4005383 us            1 PeakRSS_MB=1.31934k items_per_second=249.664k/s
SelfKNNQuery/1M/k=128      4851468 us      4849809 us            1 PeakRSS_MB=2.29754k items_per_second=206.194k/s
SelfKNNQuery/10M/k=1        648396 us       642661 us            1 PeakRSS_MB=2.01331k items_per_second=15.5603M/s
SelfKNNQuery/10M/k=2       1112883 us      1103471 us            1 PeakRSS_MB=2.1674k items_per_second=9.06231M/s
SelfKNNQuery/10M/k=4       1481564 us      1463190 us            1 PeakRSS_MB=2.47277k items_per_second=6.83438M/s
SelfKNNQuery/10M/k=8       2078360 us      2060790 us            1 PeakRSS_MB=3.08221k items_per_second=4.85251M/s
SelfKNNQuery/10M/k=16      3201552 us      3168931 us            1 PeakRSS_MB=4.30541k items_per_second=3.15564M/s
SelfKNNQuery/10M/k=32      5353894 us      5304833 us            1 PeakRSS_MB=6.74507k items_per_second=1.88507M/s
SelfKNNQuery/10M/k=64     43782908 us     43739284 us            1 PeakRSS_MB=11.6284k items_per_second=228.627k/s
SelfKNNQuery/10M/k=128    52361577 us     52336894 us            1 PeakRSS_MB=21.3945k items_per_second=191.07k/s

```
