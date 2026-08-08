# Scaling Benchmark Results: gpu_int32 (Periodic BC: ON)
Date: Sat Aug  8 20:57:55 CEST 2026
Device: GPU
Target GPU: nvidia
SFC Curve: Peano-Hilbert
Periodic BC: ON
Build Directory: build_int32
```
--- Executing ./sfc_encoding_scaling.exe ---
2026-08-08T20:57:55+02:00
Running ./sfc_encoding_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 2.02, 2.07, 1.89
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------
Benchmark                Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------
SFC-Encode/1K        0.022 ms        0.022 ms        32390 PeakRSS_MB=141.742 items_per_second=46.4568M/s
SFC-Encode/10K       0.022 ms        0.022 ms        32712 PeakRSS_MB=143.242 items_per_second=464.552M/s
SFC-Encode/100K      0.024 ms        0.024 ms        29329 PeakRSS_MB=150.43 items_per_second=4.18759G/s
SFC-Encode/1M        0.050 ms        0.050 ms        11822 PeakRSS_MB=194.359 items_per_second=19.8251G/s
SFC-Encode/10M       0.335 ms        0.335 ms         2067 PeakRSS_MB=554.051 items_per_second=29.8791G/s

--- Executing ./gpu_sort_scaling.exe ---
2026-08-08T20:58:21+02:00
Running ./gpu_sort_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 2.01, 2.06, 1.89
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
GPU_Sort/1K        0.046 ms        0.046 ms        15083 PeakRSS_MB=145.922 items_per_second=21.7084M/s
GPU_Sort/10K       0.111 ms        0.111 ms         6309 PeakRSS_MB=147.047 items_per_second=90.2877M/s
GPU_Sort/100K      0.356 ms        0.355 ms         1988 PeakRSS_MB=153.797 items_per_second=282.045M/s
GPU_Sort/1M         2.71 ms         2.71 ms          258 PeakRSS_MB=190.77 items_per_second=368.981M/s
GPU_Sort/10M        37.4 ms         37.4 ms           18 PeakRSS_MB=290.902 items_per_second=267.627M/s

--- Executing ./tree_build_scaling.exe ---
2026-08-08T20:58:39+02:00
Running ./tree_build_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 2.01, 2.06, 1.90
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------------
Benchmark               Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------
TreeBuild/1K         1.26 ms         1.26 ms          547 PeakRSS_MB=143.391 items_per_second=795.67k/s
TreeBuild/10K        1.96 ms         1.96 ms          353 PeakRSS_MB=144.516 items_per_second=5.1115M/s
TreeBuild/100K       4.14 ms         4.13 ms          165 PeakRSS_MB=148.418 items_per_second=24.2152M/s
TreeBuild/1M         22.3 ms         22.2 ms           32 PeakRSS_MB=206.867 items_per_second=44.9552M/s
TreeBuild/10M         224 ms          224 ms            3 PeakRSS_MB=563.609 items_per_second=44.6745M/s

--- Executing ./rebuild_scaling.exe ---
2026-08-08T20:58:53+02:00
Running ./rebuild_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 2.07, 2.07, 1.91
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------
TreeRebuild/1K         1.60 ms         1.59 ms          367 PeakRSS_MB=145.492 items_per_second=627.191k/s
TreeRebuild/10K        2.02 ms         2.02 ms          345 PeakRSS_MB=146.617 items_per_second=4.96028M/s
TreeRebuild/100K       4.17 ms         4.16 ms          168 PeakRSS_MB=152.758 items_per_second=24.0212M/s
TreeRebuild/1M         22.3 ms         22.3 ms           30 PeakRSS_MB=221.406 items_per_second=44.8151M/s
TreeRebuild/10M         223 ms          223 ms            3 PeakRSS_MB=870.219 items_per_second=44.9179M/s

--- Executing ./range_query_scaling.exe ---
2026-08-08T20:59:07+02:00
Running ./range_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 2.12, 2.08, 1.91
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
RangeQuery/1K/R=0.010000           30.1 us         30.1 us        22947 PeakRSS_MB=145.016 TotalFound=0
RangeQuery/1K/R=0.100000           31.2 us         31.2 us        23146 PeakRSS_MB=145.016 TotalFound=0
RangeQuery/1K/R=1.000000           32.3 us         32.3 us        21622 PeakRSS_MB=145.016 TotalFound=0
RangeQuery/1K/R=10.000000          32.4 us         32.4 us        21614 PeakRSS_MB=145.016 TotalFound=0
RangeQuery/1K/R=100.000000         32.4 us         32.4 us        21512 PeakRSS_MB=145.016 TotalFound=0
RangeQuery/1K/R=200.000000         32.4 us         32.4 us        21501 PeakRSS_MB=145.016 TotalFound=0
RangeQuery/10K/R=0.010000          32.2 us         32.2 us        21551 PeakRSS_MB=146.141 TotalFound=0
RangeQuery/10K/R=0.100000          32.4 us         32.3 us        21546 PeakRSS_MB=146.141 TotalFound=0
RangeQuery/10K/R=1.000000          32.4 us         32.4 us        21506 PeakRSS_MB=146.141 TotalFound=0
RangeQuery/10K/R=10.000000         32.5 us         32.5 us        21548 PeakRSS_MB=146.141 TotalFound=0
RangeQuery/10K/R=100.000000        32.5 us         32.5 us        21677 PeakRSS_MB=146.141 TotalFound=0
RangeQuery/10K/R=200.000000        32.5 us         32.5 us        21505 PeakRSS_MB=146.141 TotalFound=0
RangeQuery/100K/R=0.010000         32.5 us         32.5 us        21325 PeakRSS_MB=149.363 TotalFound=0
RangeQuery/100K/R=0.100000         32.6 us         32.6 us        21346 PeakRSS_MB=149.359 TotalFound=0
RangeQuery/100K/R=1.000000         32.8 us         32.8 us        21384 PeakRSS_MB=149.617 TotalFound=0
RangeQuery/100K/R=10.000000        32.7 us         32.7 us        21433 PeakRSS_MB=149.617 TotalFound=0
RangeQuery/100K/R=100.000000       32.6 us         32.6 us        21413 PeakRSS_MB=149.348 TotalFound=0
RangeQuery/100K/R=200.000000       32.6 us         32.6 us        21378 PeakRSS_MB=149.344 TotalFound=0
RangeQuery/1M/R=0.010000           32.6 us         32.6 us        21499 PeakRSS_MB=198.543 TotalFound=0
RangeQuery/1M/R=0.100000           32.6 us         32.6 us        21448 PeakRSS_MB=199.105 TotalFound=0
RangeQuery/1M/R=1.000000           32.6 us         32.6 us        21431 PeakRSS_MB=210.734 TotalFound=0
RangeQuery/1M/R=10.000000          32.6 us         32.6 us        21408 PeakRSS_MB=211.297 TotalFound=0
RangeQuery/1M/R=100.000000         32.6 us         32.6 us        21472 PeakRSS_MB=211.297 TotalFound=0
RangeQuery/1M/R=200.000000         32.6 us         32.6 us        21428 PeakRSS_MB=211.297 TotalFound=0
RangeQuery/10M/R=0.010000          30.2 us         30.2 us        23016 PeakRSS_MB=592.828 TotalFound=0
RangeQuery/10M/R=0.100000          30.4 us         30.4 us        22833 PeakRSS_MB=593.402 TotalFound=0
RangeQuery/10M/R=1.000000          30.2 us         30.2 us        23145 PeakRSS_MB=593.414 TotalFound=0
RangeQuery/10M/R=10.000000         30.3 us         30.3 us        23238 PeakRSS_MB=593.414 TotalFound=0
RangeQuery/10M/R=100.000000        30.3 us         30.3 us        22875 PeakRSS_MB=593.977 TotalFound=0
RangeQuery/10M/R=200.000000        30.3 us         30.3 us        22882 PeakRSS_MB=593.414 TotalFound=0

--- Executing ./knn_query_scaling.exe ---
2026-08-08T21:02:10+02:00
Running ./knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 2.08, 2.06, 1.94
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
KNNQuery/1K/k=1           69.7 us         69.7 us         9964 PeakRSS_MB=144.582
KNNQuery/1K/k=2            151 us          151 us         4633 PeakRSS_MB=144.582
KNNQuery/1K/k=4            207 us          207 us         3366 PeakRSS_MB=144.582
KNNQuery/1K/k=8            286 us          286 us         2441 PeakRSS_MB=144.582
KNNQuery/1K/k=16           427 us          427 us         1638 PeakRSS_MB=144.582
KNNQuery/1K/k=32           685 us          685 us         1019 PeakRSS_MB=144.582
KNNQuery/1K/k=64          2425 us         2424 us          288 PeakRSS_MB=144.582
KNNQuery/1K/k=128         2688 us         2687 us          260 PeakRSS_MB=144.582
KNNQuery/10K/k=1          80.4 us         80.4 us         8677 PeakRSS_MB=145.145
KNNQuery/10K/k=2          99.0 us         99.0 us         7048 PeakRSS_MB=145.707
KNNQuery/10K/k=4           127 us          127 us         5511 PeakRSS_MB=145.145
KNNQuery/10K/k=8           179 us          179 us         3903 PeakRSS_MB=145.145
KNNQuery/10K/k=16          274 us          274 us         2549 PeakRSS_MB=145.707
KNNQuery/10K/k=32          437 us          437 us         1599 PeakRSS_MB=145.145
KNNQuery/10K/k=64         5124 us         5123 us          137 PeakRSS_MB=145.145
KNNQuery/10K/k=128        5158 us         5157 us          136 PeakRSS_MB=145.145
KNNQuery/100K/k=1         87.2 us         87.2 us         8018 PeakRSS_MB=148.945
KNNQuery/100K/k=2          130 us          130 us         5387 PeakRSS_MB=149.559
KNNQuery/100K/k=4          165 us          165 us         4239 PeakRSS_MB=148.656
KNNQuery/100K/k=8          200 us          200 us         3500 PeakRSS_MB=149.203
KNNQuery/100K/k=16         273 us          273 us         2569 PeakRSS_MB=149.184
KNNQuery/100K/k=32         469 us          469 us         1493 PeakRSS_MB=148.621
KNNQuery/100K/k=64        3209 us         3209 us          218 PeakRSS_MB=149.125
KNNQuery/100K/k=128       3589 us         3588 us          195 PeakRSS_MB=149.121
KNNQuery/1M/k=1            245 us          245 us         2855 PeakRSS_MB=207.598
KNNQuery/1M/k=2            326 us          326 us         2144 PeakRSS_MB=193.051
KNNQuery/1M/k=4            431 us          431 us         1629 PeakRSS_MB=193.613
KNNQuery/1M/k=8            596 us          596 us         1169 PeakRSS_MB=193.055
KNNQuery/1M/k=16           931 us          931 us          750 PeakRSS_MB=193.617
KNNQuery/1M/k=32          1460 us         1460 us          478 PeakRSS_MB=193.617
KNNQuery/1M/k=64          5188 us         5187 us          136 PeakRSS_MB=193.055
KNNQuery/1M/k=128         5247 us         5246 us          134 PeakRSS_MB=193.055
KNNQuery/10M/k=1           208 us          208 us         3104 PeakRSS_MB=565
KNNQuery/10M/k=2           301 us          301 us         2317 PeakRSS_MB=565.031
KNNQuery/10M/k=4           385 us          385 us         1851 PeakRSS_MB=565.047
KNNQuery/10M/k=8           552 us          552 us         1226 PeakRSS_MB=565.125
KNNQuery/10M/k=16          896 us          895 us          762 PeakRSS_MB=565.145
KNNQuery/10M/k=32         1516 us         1515 us          433 PeakRSS_MB=565.145
KNNQuery/10M/k=64         4759 us         4758 us          131 PeakRSS_MB=565.145
KNNQuery/10M/k=128        5438 us         5437 us          129 PeakRSS_MB=564.582

--- Executing ./self_knn_query_scaling.exe ---
2026-08-08T21:05:06+02:00
Running ./self_knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 2.06, 2.07, 1.97
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------------------
Benchmark                        Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------
SelfKNNQuery/1K/k=1            263 us          263 us         2629 PeakRSS_MB=145.656 items_per_second=3.80687M/s
SelfKNNQuery/1K/k=2            341 us          341 us         2052 PeakRSS_MB=146.219 items_per_second=2.93296M/s
SelfKNNQuery/1K/k=4            383 us          382 us         1824 PeakRSS_MB=146.219 items_per_second=2.61571M/s
SelfKNNQuery/1K/k=8            475 us          475 us         1475 PeakRSS_MB=146.219 items_per_second=2.10555M/s
SelfKNNQuery/1K/k=16           614 us          614 us         1128 PeakRSS_MB=146.219 items_per_second=1.62903M/s
SelfKNNQuery/1K/k=32           878 us          878 us          792 PeakRSS_MB=146.219 items_per_second=1.13957M/s
SelfKNNQuery/1K/k=64          2927 us         2926 us          239 PeakRSS_MB=145.656 items_per_second=341.748k/s
SelfKNNQuery/1K/k=128         2957 us         2956 us          236 PeakRSS_MB=145.656 items_per_second=338.26k/s
SelfKNNQuery/10K/k=1           296 us          296 us         2358 PeakRSS_MB=146.781 items_per_second=33.821M/s
SelfKNNQuery/10K/k=2           310 us          309 us         2258 PeakRSS_MB=146.781 items_per_second=32.32M/s
SelfKNNQuery/10K/k=4           362 us          362 us         1933 PeakRSS_MB=146.781 items_per_second=27.6154M/s
SelfKNNQuery/10K/k=8           410 us          410 us         1706 PeakRSS_MB=146.781 items_per_second=24.4177M/s
SelfKNNQuery/10K/k=16          521 us          520 us         1323 PeakRSS_MB=146.781 items_per_second=19.2206M/s
SelfKNNQuery/10K/k=32          804 us          803 us          860 PeakRSS_MB=146.781 items_per_second=12.4504M/s
SelfKNNQuery/10K/k=64        15558 us        15552 us           45 PeakRSS_MB=146.781 items_per_second=643k/s
SelfKNNQuery/10K/k=128       18848 us        18843 us           37 PeakRSS_MB=146.781 items_per_second=530.704k/s
SelfKNNQuery/100K/k=1          587 us          587 us         1174 PeakRSS_MB=149.859 items_per_second=170.353M/s
SelfKNNQuery/100K/k=2          636 us          635 us         1106 PeakRSS_MB=150.68 items_per_second=157.441M/s
SelfKNNQuery/100K/k=4          667 us          667 us         1037 PeakRSS_MB=150.117 items_per_second=149.977M/s
SelfKNNQuery/100K/k=8          807 us          806 us          857 PeakRSS_MB=150.074 items_per_second=124.039M/s
SelfKNNQuery/100K/k=16        1151 us         1151 us          604 PeakRSS_MB=150.148 items_per_second=86.8966M/s
SelfKNNQuery/100K/k=32        2020 us         2019 us          340 PeakRSS_MB=150.098 items_per_second=49.5202M/s
SelfKNNQuery/100K/k=64       51719 us        51707 us           14 PeakRSS_MB=150.156 items_per_second=1.93396M/s
SelfKNNQuery/100K/k=128      79986 us        79971 us            9 PeakRSS_MB=150.121 items_per_second=1.25045M/s
SelfKNNQuery/1M/k=1           6981 us         6960 us          103 PeakRSS_MB=201.391 items_per_second=143.672M/s
SelfKNNQuery/1M/k=2           9582 us         9577 us           74 PeakRSS_MB=201.398 items_per_second=104.414M/s
SelfKNNQuery/1M/k=4          14831 us        14825 us           48 PeakRSS_MB=201.406 items_per_second=67.4532M/s
SelfKNNQuery/1M/k=8          24090 us        24084 us           29 PeakRSS_MB=201.41 items_per_second=41.5217M/s
SelfKNNQuery/1M/k=16         49452 us        49439 us           14 PeakRSS_MB=201.422 items_per_second=20.2268M/s
SelfKNNQuery/1M/k=32        106188 us       106170 us            7 PeakRSS_MB=201.422 items_per_second=9.41882M/s
SelfKNNQuery/1M/k=64       1214692 us      1214522 us            1 PeakRSS_MB=201.422 items_per_second=823.369k/s
SelfKNNQuery/1M/k=128      1294456 us      1294274 us            1 PeakRSS_MB=201.422 items_per_second=772.634k/s
SelfKNNQuery/10M/k=1        232262 us       232216 us            3 PeakRSS_MB=568.336 items_per_second=43.0634M/s
SelfKNNQuery/10M/k=2        512935 us       512842 us            1 PeakRSS_MB=568.336 items_per_second=19.4992M/s
SelfKNNQuery/10M/k=4        806073 us       805953 us            1 PeakRSS_MB=568.336 items_per_second=12.4077M/s
SelfKNNQuery/10M/k=8       1276766 us      1276583 us            1 PeakRSS_MB=568.336 items_per_second=7.83341M/s
SelfKNNQuery/10M/k=16      2064062 us      2063206 us            1 PeakRSS_MB=568.336 items_per_second=4.84682M/s
SelfKNNQuery/10M/k=32      3536078 us      3535546 us            1 PeakRSS_MB=568.898 items_per_second=2.82842M/s
SelfKNNQuery/10M/k=64     10049937 us     10048531 us            1 PeakRSS_MB=568.336 items_per_second=995.17k/s
SelfKNNQuery/10M/k=128    12351859 us     12350163 us            1 PeakRSS_MB=568.336 items_per_second=809.706k/s

```
