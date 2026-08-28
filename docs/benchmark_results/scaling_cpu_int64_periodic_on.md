# Scaling Benchmark Results: cpu_int64 (Periodic BC: ON)
Date: Fri Aug  7 13:03:30 CEST 2026
SFC Curve: Peano-Hilbert
Periodic BC: ON
Build Directory: build_int64
```
--- Executing ./sfc_encoding_scaling.exe ---
2026-08-07T13:03:30+02:00
Running ./sfc_encoding_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 2.59, 13.24, 15.08
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------
Benchmark                Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------
SFC-Encode/1K        0.052 ms        0.046 ms        15221 PeakRSS_MB=187.277 items_per_second=21.7066M/s
SFC-Encode/10K       0.063 ms        0.050 ms        13977 PeakRSS_MB=186.625 items_per_second=198.947M/s
SFC-Encode/100K      0.174 ms        0.157 ms         4473 PeakRSS_MB=180.945 items_per_second=636.961M/s
SFC-Encode/1M         1.21 ms         1.19 ms          575 PeakRSS_MB=265.965 items_per_second=838.893M/s
SFC-Encode/10M        11.5 ms         11.4 ms           61 PeakRSS_MB=1.1512k items_per_second=874.712M/s

--- Executing ./gpu_sort_scaling.exe ---
2026-08-07T13:03:52+02:00
Running ./gpu_sort_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 6.47, 13.38, 15.08
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
GPU_Sort/1K        0.278 ms        0.199 ms         3495 PeakRSS_MB=181.074 items_per_second=5.01901M/s
GPU_Sort/10K       0.541 ms        0.358 ms         1948 PeakRSS_MB=181.035 items_per_second=27.9215M/s
GPU_Sort/100K       1.44 ms         1.04 ms          670 PeakRSS_MB=187.246 items_per_second=95.7305M/s
GPU_Sort/1M         10.3 ms         9.65 ms           71 PeakRSS_MB=307.641 items_per_second=103.676M/s
GPU_Sort/10M         127 ms          126 ms            6 PeakRSS_MB=1.1218k items_per_second=79.4902M/s

--- Executing ./tree_build_scaling.exe ---
2026-08-07T13:04:14+02:00
Running ./tree_build_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 9.73, 13.72, 15.16
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------------
Benchmark               Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------
TreeBuild/1K         1.49 ms         1.40 ms          485 PeakRSS_MB=172.105 items_per_second=716.57k/s
TreeBuild/10K        3.46 ms         2.40 ms          287 PeakRSS_MB=171.57 items_per_second=4.17311M/s
TreeBuild/100K       7.31 ms         6.67 ms          104 PeakRSS_MB=172.816 items_per_second=14.9965M/s
TreeBuild/1M         34.7 ms         33.6 ms           21 PeakRSS_MB=219.082 items_per_second=29.7574M/s
TreeBuild/10M         277 ms          275 ms            3 PeakRSS_MB=705.164 items_per_second=36.3624M/s

--- Executing ./rebuild_scaling.exe ---
2026-08-07T13:04:32+02:00
Running ./rebuild_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 12.06, 14.03, 15.24
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------
TreeRebuild/1K         1.43 ms         1.33 ms          514 PeakRSS_MB=175.293 items_per_second=750.663k/s
TreeRebuild/10K        2.57 ms         2.28 ms          304 PeakRSS_MB=174.109 items_per_second=4.38527M/s
TreeRebuild/100K       7.74 ms         7.12 ms          100 PeakRSS_MB=176.207 items_per_second=14.0529M/s
TreeRebuild/1M         33.7 ms         32.6 ms           21 PeakRSS_MB=248.91 items_per_second=30.7064M/s
TreeRebuild/10M         264 ms          262 ms            3 PeakRSS_MB=1.01495k items_per_second=38.1929M/s

--- Executing ./range_query_scaling.exe ---
2026-08-07T13:04:51+02:00
Running ./range_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 19.84, 15.52, 15.69
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
RangeQuery/1K/R=0.010000            105 us         97.8 us         7183 PeakRSS_MB=180.992 TotalFound=0
RangeQuery/1K/R=0.100000            105 us         98.1 us         7202 PeakRSS_MB=180.43 TotalFound=0
RangeQuery/1K/R=1.000000            104 us         97.4 us         7141 PeakRSS_MB=179.852 TotalFound=0
RangeQuery/1K/R=10.000000           102 us         95.1 us         7204 PeakRSS_MB=178.703 TotalFound=0
RangeQuery/1K/R=100.000000          102 us         95.7 us         7096 PeakRSS_MB=178.129 TotalFound=0
RangeQuery/1K/R=200.000000          103 us         97.2 us         7143 PeakRSS_MB=177.562 TotalFound=0
RangeQuery/10K/R=0.010000           104 us         96.8 us         7062 PeakRSS_MB=167.676 TotalFound=0
RangeQuery/10K/R=0.100000           103 us         96.3 us         7019 PeakRSS_MB=157.594 TotalFound=0
RangeQuery/10K/R=1.000000           104 us         97.2 us         7124 PeakRSS_MB=165.09 TotalFound=0
RangeQuery/10K/R=10.000000          104 us         96.4 us         7224 PeakRSS_MB=173.699 TotalFound=0
RangeQuery/10K/R=100.000000         104 us         97.1 us         7239 PeakRSS_MB=166.07 TotalFound=0
RangeQuery/10K/R=200.000000         103 us         96.5 us         7092 PeakRSS_MB=161.734 TotalFound=0
RangeQuery/100K/R=0.010000          104 us         96.3 us         7088 PeakRSS_MB=175.754 TotalFound=0
RangeQuery/100K/R=0.100000          102 us         95.6 us         7144 PeakRSS_MB=178.176 TotalFound=0
RangeQuery/100K/R=1.000000          101 us         95.4 us         7125 PeakRSS_MB=177.98 TotalFound=0
RangeQuery/100K/R=10.000000         104 us         97.1 us         7128 PeakRSS_MB=180.547 TotalFound=0
RangeQuery/100K/R=100.000000        104 us         97.1 us         7182 PeakRSS_MB=181.035 TotalFound=0
RangeQuery/100K/R=200.000000        104 us         97.6 us         7102 PeakRSS_MB=179.84 TotalFound=0
RangeQuery/1M/R=0.010000            102 us         96.0 us         7232 PeakRSS_MB=341.445 TotalFound=0
RangeQuery/1M/R=0.100000            103 us         96.7 us         7183 PeakRSS_MB=341.371 TotalFound=0
RangeQuery/1M/R=1.000000            103 us         96.4 us         7098 PeakRSS_MB=341.871 TotalFound=0
RangeQuery/1M/R=10.000000           102 us         95.9 us         7126 PeakRSS_MB=340.113 TotalFound=0
RangeQuery/1M/R=100.000000          105 us         98.1 us         7268 PeakRSS_MB=341.359 TotalFound=0
RangeQuery/1M/R=200.000000          103 us         96.1 us         7254 PeakRSS_MB=340.961 TotalFound=0
RangeQuery/10M/R=0.010000           102 us         94.9 us         7294 PeakRSS_MB=1.86315k TotalFound=0
RangeQuery/10M/R=0.100000           103 us         96.1 us         7112 PeakRSS_MB=1.8632k TotalFound=0
RangeQuery/10M/R=1.000000           104 us         96.9 us         7026 PeakRSS_MB=1.86336k TotalFound=0
RangeQuery/10M/R=10.000000          103 us         96.1 us         6984 PeakRSS_MB=1.86337k TotalFound=0
RangeQuery/10M/R=100.000000         103 us         96.3 us         6979 PeakRSS_MB=1.86348k TotalFound=0
RangeQuery/10M/R=200.000000         103 us         96.2 us         7265 PeakRSS_MB=1.86354k TotalFound=0

--- Executing ./knn_query_scaling.exe ---
2026-08-07T13:07:29+02:00
Running ./knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 6.26, 14.16, 15.41
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
KNNQuery/1K/k=1           78.5 us         66.5 us        10879 PeakRSS_MB=188.621
KNNQuery/1K/k=2           88.0 us         73.7 us         9611 PeakRSS_MB=187.488
KNNQuery/1K/k=4            104 us         88.4 us         7757 PeakRSS_MB=186.355
KNNQuery/1K/k=8            125 us          111 us         6371 PeakRSS_MB=185.211
KNNQuery/1K/k=16           169 us          156 us         4509 PeakRSS_MB=183.48
KNNQuery/1K/k=32           256 us          242 us         2907 PeakRSS_MB=180.172
KNNQuery/1K/k=64          2135 us         2132 us          327 PeakRSS_MB=176.125
KNNQuery/1K/k=128         2534 us         2533 us          276 PeakRSS_MB=169.406
KNNQuery/10K/k=1          74.5 us         65.6 us        10503 PeakRSS_MB=173.102
KNNQuery/10K/k=2          77.7 us         67.0 us        10571 PeakRSS_MB=183.727
KNNQuery/10K/k=4          82.1 us         69.5 us        10124 PeakRSS_MB=176.473
KNNQuery/10K/k=8          94.7 us         81.3 us         8636 PeakRSS_MB=170.844
KNNQuery/10K/k=16          121 us          108 us         6524 PeakRSS_MB=177.977
KNNQuery/10K/k=32          165 us          151 us         4624 PeakRSS_MB=179.152
KNNQuery/10K/k=64         3449 us         3447 us          202 PeakRSS_MB=173.488
KNNQuery/10K/k=128        4499 us         4498 us          156 PeakRSS_MB=178.184
KNNQuery/100K/k=1         82.0 us         70.4 us         9863 PeakRSS_MB=188.238
KNNQuery/100K/k=2         86.4 us         75.3 us         9377 PeakRSS_MB=186.84
KNNQuery/100K/k=4         87.7 us         75.8 us         9360 PeakRSS_MB=188.527
KNNQuery/100K/k=8         94.0 us         81.2 us         8591 PeakRSS_MB=188.082
KNNQuery/100K/k=16         104 us         91.1 us         7613 PeakRSS_MB=186.551
KNNQuery/100K/k=32         139 us          127 us         5575 PeakRSS_MB=188.09
KNNQuery/100K/k=64        1601 us         1600 us          437 PeakRSS_MB=189.594
KNNQuery/100K/k=128       2374 us         2373 us          294 PeakRSS_MB=188.27
KNNQuery/1M/k=1            108 us         92.2 us         7659 PeakRSS_MB=345.742
KNNQuery/1M/k=2            125 us          109 us         6352 PeakRSS_MB=345.867
KNNQuery/1M/k=4            151 us          131 us         5284 PeakRSS_MB=346.793
KNNQuery/1M/k=8            186 us          170 us         4159 PeakRSS_MB=344.324
KNNQuery/1M/k=16           252 us          236 us         2978 PeakRSS_MB=346.637
KNNQuery/1M/k=32           381 us          380 us         1839 PeakRSS_MB=346.387
KNNQuery/1M/k=64          3512 us         3510 us          199 PeakRSS_MB=344.766
KNNQuery/1M/k=128         3742 us         3740 us          187 PeakRSS_MB=345.316
KNNQuery/10M/k=1           107 us         87.5 us         7891 PeakRSS_MB=1.86413k
KNNQuery/10M/k=2           132 us          116 us         6087 PeakRSS_MB=1.86469k
KNNQuery/10M/k=4           155 us          137 us         5150 PeakRSS_MB=1.86445k
KNNQuery/10M/k=8           190 us          171 us         4076 PeakRSS_MB=1.86483k
KNNQuery/10M/k=16          265 us          244 us         2868 PeakRSS_MB=1.86471k
KNNQuery/10M/k=32          434 us          432 us         1624 PeakRSS_MB=1.86494k
KNNQuery/10M/k=64         3261 us         3253 us          216 PeakRSS_MB=1.86486k
KNNQuery/10M/k=128        3954 us         3947 us          177 PeakRSS_MB=1.86492k

--- Executing ./self_knn_query_scaling.exe ---
2026-08-27T15:42:17+02:00
Running ./self_knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 31.38, 22.04, 12.42
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------------------
Benchmark                        Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------
SelfKNNQuery/1K/k=1           89.9 us         78.2 us         8679 PeakRSS_MB=187.184 items_per_second=12.7924M/s
SelfKNNQuery/1K/k=2            153 us          142 us         5022 PeakRSS_MB=186.035 items_per_second=7.0176M/s
SelfKNNQuery/1K/k=4            187 us          177 us         3974 PeakRSS_MB=184.871 items_per_second=5.64034M/s
SelfKNNQuery/1K/k=8            259 us          249 us         2805 PeakRSS_MB=183.723 items_per_second=4.01398M/s
SelfKNNQuery/1K/k=16           412 us          408 us         1725 PeakRSS_MB=181.852 items_per_second=2.44988M/s
SelfKNNQuery/1K/k=32           611 us          610 us         1130 PeakRSS_MB=179.41 items_per_second=1.64003M/s
SelfKNNQuery/1K/k=64           966 us          965 us          724 PeakRSS_MB=175.34 items_per_second=1.03676M/s
SelfKNNQuery/1K/k=128         1588 us         1583 us          442 PeakRSS_MB=168.672 items_per_second=631.85k/s
SelfKNNQuery/10K/k=1           423 us          408 us         1713 PeakRSS_MB=167.984 items_per_second=24.5336M/s
SelfKNNQuery/10K/k=2           957 us          942 us          743 PeakRSS_MB=181.879 items_per_second=10.6211M/s
SelfKNNQuery/10K/k=4          1375 us         1367 us          515 PeakRSS_MB=175.199 items_per_second=7.3162M/s
SelfKNNQuery/10K/k=8          1958 us         1956 us          358 PeakRSS_MB=169.449 items_per_second=5.11159M/s
SelfKNNQuery/10K/k=16         2967 us         2963 us          236 PeakRSS_MB=181.648 items_per_second=3.37468M/s
SelfKNNQuery/10K/k=32         4918 us         4913 us          142 PeakRSS_MB=175.188 items_per_second=2.03522M/s
SelfKNNQuery/10K/k=64         8787 us         8683 us           82 PeakRSS_MB=185.223 items_per_second=1.15173M/s
SelfKNNQuery/10K/k=128       16345 us        15762 us           44 PeakRSS_MB=195.184 items_per_second=634.421k/s
SelfKNNQuery/100K/k=1         4738 us         4706 us          149 PeakRSS_MB=190.297 items_per_second=21.2475M/s
SelfKNNQuery/100K/k=2        10171 us        10162 us           69 PeakRSS_MB=189.715 items_per_second=9.84073M/s
SelfKNNQuery/100K/k=4        14133 us        14123 us           49 PeakRSS_MB=193.824 items_per_second=7.0808M/s
SelfKNNQuery/100K/k=8        20007 us        19995 us           35 PeakRSS_MB=199.266 items_per_second=5.00137M/s
SelfKNNQuery/100K/k=16       29811 us        29798 us           24 PeakRSS_MB=213.469 items_per_second=3.35591M/s
SelfKNNQuery/100K/k=32       48707 us        48687 us           14 PeakRSS_MB=239.141 items_per_second=2.05394M/s
SelfKNNQuery/100K/k=64       83544 us        83299 us            8 PeakRSS_MB=294.473 items_per_second=1.20049M/s
SelfKNNQuery/100K/k=128     156559 us       155386 us            5 PeakRSS_MB=396.926 items_per_second=643.559k/s
SelfKNNQuery/1M/k=1          50286 us        50240 us           14 PeakRSS_MB=352.059 items_per_second=19.9044M/s
SelfKNNQuery/1M/k=2         108357 us       108290 us            6 PeakRSS_MB=371.691 items_per_second=9.23447M/s
SelfKNNQuery/1M/k=4         149877 us       149826 us            5 PeakRSS_MB=403.652 items_per_second=6.67442M/s
SelfKNNQuery/1M/k=8         210232 us       210128 us            3 PeakRSS_MB=467.793 items_per_second=4.759M/s
SelfKNNQuery/1M/k=16        313505 us       313430 us            2 PeakRSS_MB=593.062 items_per_second=3.1905M/s
SelfKNNQuery/1M/k=32        507833 us       507750 us            1 PeakRSS_MB=841.594 items_per_second=1.96947M/s
SelfKNNQuery/1M/k=64        868263 us       867441 us            1 PeakRSS_MB=1.33059k items_per_second=1.15282M/s
SelfKNNQuery/1M/k=128      1617074 us      1613892 us            1 PeakRSS_MB=2.30716k items_per_second=619.62k/s
SelfKNNQuery/10M/k=1        542821 us       542427 us            1 PeakRSS_MB=2.01958k items_per_second=18.4357M/s
SelfKNNQuery/10M/k=2       1182215 us      1182023 us            1 PeakRSS_MB=2.17296k items_per_second=8.46007M/s
SelfKNNQuery/10M/k=4       1621528 us      1621189 us            1 PeakRSS_MB=2.4781k items_per_second=6.16831M/s
SelfKNNQuery/10M/k=8       2257942 us      2257424 us            1 PeakRSS_MB=3.08876k items_per_second=4.42983M/s
SelfKNNQuery/10M/k=16      3348507 us      3347179 us            1 PeakRSS_MB=4.30916k items_per_second=2.98759M/s
SelfKNNQuery/10M/k=32      5380913 us      5378564 us            1 PeakRSS_MB=6.75126k items_per_second=1.85923M/s
SelfKNNQuery/10M/k=64      9194826 us      9174970 us            1 PeakRSS_MB=11.6341k items_per_second=1.08992M/s
SelfKNNQuery/10M/k=128    17037666 us     17027841 us            1 PeakRSS_MB=21.3997k items_per_second=587.273k/s

```
