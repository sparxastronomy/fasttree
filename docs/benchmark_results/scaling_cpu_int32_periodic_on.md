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
2026-08-27T14:55:57+02:00
Running ./self_knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 26.84, 25.81, 17.03
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------------------
Benchmark                        Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------
SelfKNNQuery/1K/k=1           87.3 us         84.6 us         8170 PeakRSS_MB=172.535 items_per_second=11.8217M/s
SelfKNNQuery/1K/k=2            161 us          160 us         4378 PeakRSS_MB=171.969 items_per_second=6.24156M/s
SelfKNNQuery/1K/k=4            211 us          210 us         3321 PeakRSS_MB=171.969 items_per_second=4.76127M/s
SelfKNNQuery/1K/k=8            316 us          315 us         2214 PeakRSS_MB=170.805 items_per_second=3.1704M/s
SelfKNNQuery/1K/k=16           497 us          497 us         1397 PeakRSS_MB=169.605 items_per_second=2.01259M/s
SelfKNNQuery/1K/k=32           858 us          850 us          825 PeakRSS_MB=167.66 items_per_second=1.17601M/s
SelfKNNQuery/1K/k=64          1077 us         1075 us          662 PeakRSS_MB=164.621 items_per_second=930.418k/s
SelfKNNQuery/1K/k=128         1817 us         1775 us          395 PeakRSS_MB=157.203 items_per_second=563.253k/s
SelfKNNQuery/10K/k=1           312 us          298 us         2353 PeakRSS_MB=149.035 items_per_second=33.5715M/s
SelfKNNQuery/10K/k=2           910 us          905 us          760 PeakRSS_MB=154.598 items_per_second=11.045M/s
SelfKNNQuery/10K/k=4          1391 us         1383 us          504 PeakRSS_MB=171.082 items_per_second=7.23272M/s
SelfKNNQuery/10K/k=8          2082 us         2081 us          335 PeakRSS_MB=162.27 items_per_second=4.80645M/s
SelfKNNQuery/10K/k=16         3385 us         3382 us          207 PeakRSS_MB=156.027 items_per_second=2.95716M/s
SelfKNNQuery/10K/k=32         6161 us         5998 us          117 PeakRSS_MB=169.145 items_per_second=1.66713M/s
SelfKNNQuery/10K/k=64        10405 us         9424 us           72 PeakRSS_MB=164.047 items_per_second=1.06107M/s
SelfKNNQuery/10K/k=128       18772 us        16374 us           41 PeakRSS_MB=183.496 items_per_second=610.706k/s
SelfKNNQuery/100K/k=1         3212 us         3176 us          221 PeakRSS_MB=169.375 items_per_second=31.4834M/s
SelfKNNQuery/100K/k=2         9129 us         9124 us          100 PeakRSS_MB=175.469 items_per_second=10.96M/s
SelfKNNQuery/100K/k=4        13438 us        13431 us           52 PeakRSS_MB=174.977 items_per_second=7.44529M/s
SelfKNNQuery/100K/k=8        20032 us        20025 us           35 PeakRSS_MB=186.23 items_per_second=4.99381M/s
SelfKNNQuery/100K/k=16       31810 us        31792 us           22 PeakRSS_MB=196.383 items_per_second=3.14544M/s
SelfKNNQuery/100K/k=32       54514 us        54089 us           13 PeakRSS_MB=223.738 items_per_second=1.84881M/s
SelfKNNQuery/100K/k=64       88865 us        87225 us            8 PeakRSS_MB=275.688 items_per_second=1.14647M/s
SelfKNNQuery/100K/k=128     167623 us       164392 us            4 PeakRSS_MB=380.602 items_per_second=608.302k/s
SelfKNNQuery/1M/k=1          35269 us        35177 us           20 PeakRSS_MB=290.523 items_per_second=28.4274M/s
SelfKNNQuery/1M/k=2         101605 us       101575 us            7 PeakRSS_MB=308.484 items_per_second=9.8449M/s
SelfKNNQuery/1M/k=4         148925 us       148842 us            5 PeakRSS_MB=345.008 items_per_second=6.71853M/s
SelfKNNQuery/1M/k=8         219432 us       219376 us            3 PeakRSS_MB=409.355 items_per_second=4.55838M/s
SelfKNNQuery/1M/k=16        343592 us       343463 us            2 PeakRSS_MB=533.488 items_per_second=2.91153M/s
SelfKNNQuery/1M/k=32        578844 us       578002 us            1 PeakRSS_MB=781.738 items_per_second=1.7301M/s
SelfKNNQuery/1M/k=64        934116 us       933752 us            1 PeakRSS_MB=1.27089k items_per_second=1.07095M/s
SelfKNNQuery/1M/k=128      1773579 us      1770091 us            1 PeakRSS_MB=2.24746k items_per_second=564.943k/s
SelfKNNQuery/10M/k=1        357113 us       356884 us            2 PeakRSS_MB=1.44386k items_per_second=28.0203M/s
SelfKNNQuery/10M/k=2       1036588 us      1035176 us            1 PeakRSS_MB=1.59781k items_per_second=9.66019M/s
SelfKNNQuery/10M/k=4       1507238 us      1506155 us            1 PeakRSS_MB=1.90344k items_per_second=6.63942M/s
SelfKNNQuery/10M/k=8       2227245 us      2219898 us            1 PeakRSS_MB=2.51347k items_per_second=4.50471M/s
SelfKNNQuery/10M/k=16      3456624 us      3455280 us            1 PeakRSS_MB=3.73481k items_per_second=2.89412M/s
SelfKNNQuery/10M/k=32      5837484 us      5824841 us            1 PeakRSS_MB=6.1756k items_per_second=1.71678M/s
SelfKNNQuery/10M/k=64      9414541 us      9400444 us            1 PeakRSS_MB=11.0584k items_per_second=1.06378M/s
SelfKNNQuery/10M/k=128    17879130 us     17869261 us            1 PeakRSS_MB=20.8241k items_per_second=559.62k/s

```
