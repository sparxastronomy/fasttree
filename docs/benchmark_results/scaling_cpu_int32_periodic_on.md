# Scaling Benchmark Results: cpu_int32 (Periodic BC: ON)
Date: Fri Aug  7 12:44:19 CEST 2026
SFC Curve: Peano-Hilbert
Periodic BC: ON
Build Directory: build_int32
```
--- Executing ./sfc_encoding_scaling.exe ---
2026-08-07T12:44:19+02:00
Running ./sfc_encoding_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.27, 8.36, 10.80
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------
Benchmark                Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------
SFC-Encode/1K        0.052 ms        0.049 ms        13984 PeakRSS_MB=168.562 items_per_second=20.5456M/s
SFC-Encode/10K       0.059 ms        0.044 ms        15801 PeakRSS_MB=167.977 items_per_second=229.052M/s
SFC-Encode/100K      0.111 ms        0.090 ms         7828 PeakRSS_MB=168.988 items_per_second=1.11137G/s
SFC-Encode/1M        0.626 ms        0.603 ms         1142 PeakRSS_MB=215.848 items_per_second=1.65744G/s
SFC-Encode/10M        5.72 ms         5.69 ms          123 PeakRSS_MB=802.477 items_per_second=1.75788G/s

--- Executing ./gpu_sort_scaling.exe ---
2026-08-07T12:44:40+02:00
Running ./gpu_sort_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 11.29, 10.07, 11.30
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
GPU_Sort/1K        0.237 ms        0.189 ms         3664 PeakRSS_MB=169.582 items_per_second=5.29882M/s
GPU_Sort/10K       0.506 ms        0.343 ms         2050 PeakRSS_MB=172.164 items_per_second=29.1775M/s
GPU_Sort/100K       1.26 ms        0.887 ms          780 PeakRSS_MB=186.02 items_per_second=112.751M/s
GPU_Sort/1M         8.37 ms         7.74 ms           91 PeakRSS_MB=285.145 items_per_second=129.216M/s
GPU_Sort/10M         107 ms          106 ms            7 PeakRSS_MB=892.258 items_per_second=94.0577M/s

--- Executing ./tree_build_scaling.exe ---
2026-08-07T12:45:00+02:00
Running ./tree_build_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 18.80, 11.78, 11.83
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------------
Benchmark               Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------
TreeBuild/1K         1.10 ms         1.01 ms          683 PeakRSS_MB=160.141 items_per_second=993.356k/s
TreeBuild/10K        2.57 ms         2.25 ms          309 PeakRSS_MB=153.742 items_per_second=4.43513M/s
TreeBuild/100K       6.10 ms         5.54 ms          125 PeakRSS_MB=160.078 items_per_second=18.056M/s
TreeBuild/1M         30.2 ms         29.3 ms           24 PeakRSS_MB=197.18 items_per_second=34.1103M/s
TreeBuild/10M         228 ms          227 ms            3 PeakRSS_MB=589.168 items_per_second=44.1458M/s

--- Executing ./rebuild_scaling.exe ---
2026-08-07T12:45:17+02:00
Running ./rebuild_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 19.67, 12.39, 12.04
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------
TreeRebuild/1K         1.09 ms        0.995 ms          701 PeakRSS_MB=157.828 items_per_second=1.005M/s
TreeRebuild/10K        2.53 ms         2.26 ms          309 PeakRSS_MB=150.934 items_per_second=4.43346M/s
TreeRebuild/100K       6.61 ms         5.98 ms          116 PeakRSS_MB=163.117 items_per_second=16.734M/s
TreeRebuild/1M         29.1 ms         28.2 ms           25 PeakRSS_MB=225.195 items_per_second=35.4075M/s
TreeRebuild/10M         213 ms          211 ms            3 PeakRSS_MB=890.801 items_per_second=47.3435M/s

--- Executing ./range_query_scaling.exe ---
2026-08-07T12:45:35+02:00
Running ./range_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 26.45, 14.17, 12.62
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
RangeQuery/1K/R=0.010000            102 us         95.4 us         7255 PeakRSS_MB=167.941 TotalFound=0
RangeQuery/1K/R=0.100000           99.3 us         94.9 us         7328 PeakRSS_MB=167.367 TotalFound=0
RangeQuery/1K/R=1.000000            101 us         94.9 us         7306 PeakRSS_MB=167.367 TotalFound=0
RangeQuery/1K/R=10.000000          99.5 us         93.9 us         7306 PeakRSS_MB=167.367 TotalFound=0
RangeQuery/1K/R=100.000000         99.6 us         94.3 us         7152 PeakRSS_MB=166.801 TotalFound=0
RangeQuery/1K/R=200.000000          161 us         99.4 us         7109 PeakRSS_MB=166.801 TotalFound=0
RangeQuery/10K/R=0.010000           101 us         94.8 us         7208 PeakRSS_MB=160.551 TotalFound=0
RangeQuery/10K/R=0.100000           102 us         95.3 us         7248 PeakRSS_MB=152.805 TotalFound=0
RangeQuery/10K/R=1.000000           102 us         95.2 us         7217 PeakRSS_MB=146.141 TotalFound=0
RangeQuery/10K/R=10.000000          101 us         95.2 us         7275 PeakRSS_MB=150.734 TotalFound=0
RangeQuery/10K/R=100.000000         101 us         95.0 us         7313 PeakRSS_MB=163.191 TotalFound=0
RangeQuery/10K/R=200.000000         123 us         97.2 us         7305 PeakRSS_MB=160.484 TotalFound=0
RangeQuery/100K/R=0.010000          102 us         95.7 us         7276 PeakRSS_MB=169.254 TotalFound=0
RangeQuery/100K/R=0.100000          100 us         94.5 us         7299 PeakRSS_MB=169.988 TotalFound=0
RangeQuery/100K/R=1.000000          101 us         94.6 us         7281 PeakRSS_MB=166.184 TotalFound=0
RangeQuery/100K/R=10.000000         115 us         95.7 us         7345 PeakRSS_MB=169.035 TotalFound=0
RangeQuery/100K/R=100.000000        101 us         95.1 us         7326 PeakRSS_MB=168.16 TotalFound=0
RangeQuery/100K/R=200.000000        101 us         94.7 us         7431 PeakRSS_MB=166.184 TotalFound=0
RangeQuery/1M/R=0.010000            102 us         95.1 us         7238 PeakRSS_MB=269.973 TotalFound=0
RangeQuery/1M/R=0.100000            101 us         95.0 us         7123 PeakRSS_MB=271.059 TotalFound=0
RangeQuery/1M/R=1.000000            102 us         94.6 us         7234 PeakRSS_MB=271.484 TotalFound=0
RangeQuery/1M/R=10.000000           101 us         94.4 us         7234 PeakRSS_MB=271.023 TotalFound=0
RangeQuery/1M/R=100.000000          102 us         94.9 us         7270 PeakRSS_MB=272.598 TotalFound=0
RangeQuery/1M/R=200.000000          101 us         94.8 us         7252 PeakRSS_MB=270.191 TotalFound=0
RangeQuery/10M/R=0.010000           101 us         95.2 us         7209 PeakRSS_MB=1.28755k TotalFound=0
RangeQuery/10M/R=0.100000           102 us         95.4 us         7243 PeakRSS_MB=1.28732k TotalFound=0
RangeQuery/10M/R=1.000000           100 us         94.1 us         7205 PeakRSS_MB=1.28753k TotalFound=0
RangeQuery/10M/R=10.000000          100 us         94.5 us         7203 PeakRSS_MB=1.28769k TotalFound=0
RangeQuery/10M/R=100.000000         101 us         95.3 us         7198 PeakRSS_MB=1.2877k TotalFound=0
RangeQuery/10M/R=200.000000         102 us         95.6 us         7198 PeakRSS_MB=1.2875k TotalFound=0

--- Executing ./knn_query_scaling.exe ---
2026-08-07T12:48:14+02:00
Running ./knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 10.92, 13.60, 12.80
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
KNNQuery/1K/k=1           77.4 us         66.3 us        10683 PeakRSS_MB=175.438
KNNQuery/1K/k=2           85.5 us         73.0 us         9750 PeakRSS_MB=175.438
KNNQuery/1K/k=4           98.6 us         84.3 us         8438 PeakRSS_MB=174.855
KNNQuery/1K/k=8            122 us          110 us         6586 PeakRSS_MB=174.293
KNNQuery/1K/k=16           163 us          152 us         4619 PeakRSS_MB=173.059
KNNQuery/1K/k=32           246 us          234 us         3000 PeakRSS_MB=170.453
KNNQuery/1K/k=64          2126 us         2122 us          330 PeakRSS_MB=166.629
KNNQuery/1K/k=128         2530 us         2520 us          278 PeakRSS_MB=159.688
KNNQuery/10K/k=1          71.9 us         65.5 us        10618 PeakRSS_MB=152.707
KNNQuery/10K/k=2          76.6 us         66.0 us        10688 PeakRSS_MB=161.691
KNNQuery/10K/k=4          79.8 us         68.6 us        10060 PeakRSS_MB=172.023
KNNQuery/10K/k=8          91.5 us         79.3 us         8897 PeakRSS_MB=166
KNNQuery/10K/k=16          116 us          105 us         6890 PeakRSS_MB=157.652
KNNQuery/10K/k=32          158 us          147 us         4800 PeakRSS_MB=158.352
KNNQuery/10K/k=64         3426 us         3424 us          204 PeakRSS_MB=167.828
KNNQuery/10K/k=128        4449 us         4445 us          157 PeakRSS_MB=163.93
KNNQuery/100K/k=1         79.9 us         69.7 us         9933 PeakRSS_MB=175.121
KNNQuery/100K/k=2         84.3 us         73.4 us         9537 PeakRSS_MB=169.957
KNNQuery/100K/k=4         86.4 us         76.3 us         9031 PeakRSS_MB=173.223
KNNQuery/100K/k=8         91.3 us         79.5 us         8876 PeakRSS_MB=172.039
KNNQuery/100K/k=16         102 us         89.5 us         7975 PeakRSS_MB=172.027
KNNQuery/100K/k=32         135 us          121 us         5786 PeakRSS_MB=170.48
KNNQuery/100K/k=64        1584 us         1583 us          440 PeakRSS_MB=174.031
KNNQuery/100K/k=128       2357 us         2356 us          297 PeakRSS_MB=173.957
KNNQuery/1M/k=1            108 us         95.9 us         7317 PeakRSS_MB=278.125
KNNQuery/1M/k=2            125 us          113 us         6262 PeakRSS_MB=279.176
KNNQuery/1M/k=4            148 us          135 us         5198 PeakRSS_MB=276.488
KNNQuery/1M/k=8            185 us          172 us         4120 PeakRSS_MB=278.945
KNNQuery/1M/k=16           251 us          239 us         2945 PeakRSS_MB=278.586
KNNQuery/1M/k=32           384 us          383 us         1825 PeakRSS_MB=277.324
KNNQuery/1M/k=64          3523 us         3521 us          198 PeakRSS_MB=281.48
KNNQuery/1M/k=128         3755 us         3754 us          186 PeakRSS_MB=283.23
KNNQuery/10M/k=1           108 us         89.3 us         7941 PeakRSS_MB=1.29264k
KNNQuery/10M/k=2           131 us          113 us         6202 PeakRSS_MB=1.29255k
KNNQuery/10M/k=4           151 us          137 us         5159 PeakRSS_MB=1.29242k
KNNQuery/10M/k=8           184 us          168 us         4127 PeakRSS_MB=1.29304k
KNNQuery/10M/k=16          245 us          230 us         3096 PeakRSS_MB=1.2922k
KNNQuery/10M/k=32          389 us          387 us         1802 PeakRSS_MB=1.29297k
KNNQuery/10M/k=64         3083 us         3065 us          228 PeakRSS_MB=1.29286k
KNNQuery/10M/k=128        3754 us         3731 us          188 PeakRSS_MB=1.29235k

--- Executing ./self_knn_query_scaling.exe ---
2026-08-08T20:18:19+02:00
Running ./self_knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 40.13, 47.80, 33.42
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------------------
Benchmark                        Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------
SelfKNNQuery/1K/k=1            226 us          183 us         3767 PeakRSS_MB=175.52 items_per_second=5.47294M/s
SelfKNNQuery/1K/k=2            237 us          190 us         3698 PeakRSS_MB=175.52 items_per_second=5.26213M/s
SelfKNNQuery/1K/k=4            250 us          203 us         3398 PeakRSS_MB=174.941 items_per_second=4.92677M/s
SelfKNNQuery/1K/k=8            271 us          223 us         3110 PeakRSS_MB=174.332 items_per_second=4.47718M/s
SelfKNNQuery/1K/k=16           315 us          274 us         2565 PeakRSS_MB=173.145 items_per_second=3.64492M/s
SelfKNNQuery/1K/k=32           407 us          376 us         1864 PeakRSS_MB=171.219 items_per_second=2.66292M/s
SelfKNNQuery/1K/k=64          2270 us         2255 us          308 PeakRSS_MB=167.66 items_per_second=443.519k/s
SelfKNNQuery/1K/k=128         2667 us         2654 us          264 PeakRSS_MB=160.227 items_per_second=376.82k/s
SelfKNNQuery/10K/k=1           600 us          572 us         1223 PeakRSS_MB=158.332 items_per_second=17.479M/s
SelfKNNQuery/10K/k=2           664 us          637 us         1071 PeakRSS_MB=163.312 items_per_second=15.698M/s
SelfKNNQuery/10K/k=4           800 us          771 us          903 PeakRSS_MB=165.145 items_per_second=12.9628M/s
SelfKNNQuery/10K/k=8          1076 us         1043 us          673 PeakRSS_MB=163.176 items_per_second=9.59165M/s
SelfKNNQuery/10K/k=16         1592 us         1564 us          449 PeakRSS_MB=166.332 items_per_second=6.39189M/s
SelfKNNQuery/10K/k=32         2595 us         2490 us          283 PeakRSS_MB=167.703 items_per_second=4.01668M/s
SelfKNNQuery/10K/k=64        30024 us        29979 us           23 PeakRSS_MB=171.891 items_per_second=333.569k/s
SelfKNNQuery/10K/k=128       41122 us        41020 us           17 PeakRSS_MB=184 items_per_second=243.783k/s
SelfKNNQuery/100K/k=1         3997 us         3954 us          177 PeakRSS_MB=173.637 items_per_second=25.2901M/s
SelfKNNQuery/100K/k=2         4207 us         4166 us          167 PeakRSS_MB=176.332 items_per_second=24.0053M/s
SelfKNNQuery/100K/k=4         4691 us         4648 us          150 PeakRSS_MB=178.938 items_per_second=21.5133M/s
SelfKNNQuery/100K/k=8         5888 us         5809 us          120 PeakRSS_MB=188.426 items_per_second=17.2145M/s
SelfKNNQuery/100K/k=16        8756 us         8075 us          117 PeakRSS_MB=194.965 items_per_second=12.3835M/s
SelfKNNQuery/100K/k=32       16766 us        15438 us           45 PeakRSS_MB=219.66 items_per_second=6.47734M/s
SelfKNNQuery/100K/k=64      136847 us       136706 us            5 PeakRSS_MB=279.375 items_per_second=731.495k/s
SelfKNNQuery/100K/k=128     211045 us       210945 us            3 PeakRSS_MB=381.934 items_per_second=474.058k/s
SelfKNNQuery/1M/k=1          65521 us        64213 us           11 PeakRSS_MB=294.031 items_per_second=15.5732M/s
SelfKNNQuery/1M/k=2          83700 us        81678 us            8 PeakRSS_MB=308.059 items_per_second=12.2431M/s
SelfKNNQuery/1M/k=4         114844 us       110407 us            6 PeakRSS_MB=335.93 items_per_second=9.05739M/s
SelfKNNQuery/1M/k=8         176279 us       172025 us            4 PeakRSS_MB=400.953 items_per_second=5.81311M/s
SelfKNNQuery/1M/k=16        295614 us       288020 us            2 PeakRSS_MB=528.32 items_per_second=3.47198M/s
SelfKNNQuery/1M/k=32        511459 us       497390 us            2 PeakRSS_MB=776.691 items_per_second=2.01049M/s
SelfKNNQuery/1M/k=64       3097739 us      3093615 us            1 PeakRSS_MB=1.26858k items_per_second=323.246k/s
SelfKNNQuery/1M/k=128      3302474 us      3301924 us            1 PeakRSS_MB=2.24468k items_per_second=302.854k/s
SelfKNNQuery/10M/k=1        537210 us       534341 us            1 PeakRSS_MB=1.43769k items_per_second=18.7147M/s
SelfKNNQuery/10M/k=2        900306 us       898202 us            1 PeakRSS_MB=1.59055k items_per_second=11.1334M/s
SelfKNNQuery/10M/k=4       1220784 us      1204147 us            1 PeakRSS_MB=1.89591k items_per_second=8.30464M/s
SelfKNNQuery/10M/k=8       1761470 us      1726374 us            1 PeakRSS_MB=2.50773k items_per_second=5.79249M/s
SelfKNNQuery/10M/k=16      2766565 us      2748005 us            1 PeakRSS_MB=3.72775k items_per_second=3.639M/s
SelfKNNQuery/10M/k=32      4726542 us      4694130 us            1 PeakRSS_MB=6.16976k items_per_second=2.13032M/s
SelfKNNQuery/10M/k=64     27501640 us     27492248 us            1 PeakRSS_MB=11.0538k items_per_second=363.739k/s
SelfKNNQuery/10M/k=128    33623144 us     33558701 us            1 PeakRSS_MB=20.8187k items_per_second=297.985k/s

```
