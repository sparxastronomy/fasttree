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
2026-08-27T08:59:33+02:00
Running ./self_knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 4.71, 4.26, 3.79
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------------------
Benchmark                        Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------
SelfKNNQuery/1K/k=1           45.6 us         45.5 us        13435 PeakRSS_MB=143.004 items_per_second=21.9549M/s
SelfKNNQuery/1K/k=2            200 us          200 us         3509 PeakRSS_MB=143.004 items_per_second=5.00629M/s
SelfKNNQuery/1K/k=4            276 us          276 us         2534 PeakRSS_MB=143.004 items_per_second=3.61947M/s
SelfKNNQuery/1K/k=8            434 us          434 us         1611 PeakRSS_MB=143.004 items_per_second=2.30295M/s
SelfKNNQuery/1K/k=16           903 us          903 us          771 PeakRSS_MB=143.004 items_per_second=1.10759M/s
SelfKNNQuery/1K/k=32          1249 us         1249 us          559 PeakRSS_MB=143.004 items_per_second=800.678k/s
SelfKNNQuery/1K/k=64          3270 us         3269 us          215 PeakRSS_MB=143.004 items_per_second=305.893k/s
SelfKNNQuery/1K/k=128         5107 us         5106 us          137 PeakRSS_MB=143.004 items_per_second=195.842k/s
SelfKNNQuery/10K/k=1          56.2 us         56.2 us        12396 PeakRSS_MB=144.691 items_per_second=177.909M/s
SelfKNNQuery/10K/k=2           328 us          328 us         2136 PeakRSS_MB=144.691 items_per_second=30.508M/s
SelfKNNQuery/10K/k=4           558 us          558 us         1250 PeakRSS_MB=144.691 items_per_second=17.9337M/s
SelfKNNQuery/10K/k=8          1067 us         1067 us          655 PeakRSS_MB=144.691 items_per_second=9.37248M/s
SelfKNNQuery/10K/k=16         2090 us         2089 us          335 PeakRSS_MB=144.129 items_per_second=4.78621M/s
SelfKNNQuery/10K/k=32         4303 us         4302 us          163 PeakRSS_MB=144.129 items_per_second=2.32437M/s
SelfKNNQuery/10K/k=64        10400 us        10398 us           67 PeakRSS_MB=144.691 items_per_second=961.722k/s
SelfKNNQuery/10K/k=128       17416 us        17414 us           40 PeakRSS_MB=144.691 items_per_second=574.261k/s
SelfKNNQuery/100K/k=1          181 us          181 us         3839 PeakRSS_MB=148.062 items_per_second=552.244M/s
SelfKNNQuery/100K/k=2          990 us          990 us          707 PeakRSS_MB=147.98 items_per_second=100.985M/s
SelfKNNQuery/100K/k=4         1678 us         1678 us          419 PeakRSS_MB=148.008 items_per_second=59.6022M/s
SelfKNNQuery/100K/k=8         2904 us         2903 us          240 PeakRSS_MB=148.035 items_per_second=34.4445M/s
SelfKNNQuery/100K/k=16        6138 us         6137 us          113 PeakRSS_MB=147.977 items_per_second=16.2954M/s
SelfKNNQuery/100K/k=32       13432 us        13430 us           52 PeakRSS_MB=148.008 items_per_second=7.44595M/s
SelfKNNQuery/100K/k=64       31604 us        31600 us           22 PeakRSS_MB=148.008 items_per_second=3.16458M/s
SelfKNNQuery/100K/k=128      86619 us        86608 us            8 PeakRSS_MB=148.008 items_per_second=1.15462M/s
SelfKNNQuery/1M/k=1           1299 us         1298 us          538 PeakRSS_MB=191.984 items_per_second=770.194M/s
SelfKNNQuery/1M/k=2           8050 us         8049 us           87 PeakRSS_MB=191.988 items_per_second=124.241M/s
SelfKNNQuery/1M/k=4          14570 us        14568 us           48 PeakRSS_MB=192.105 items_per_second=68.6449M/s
SelfKNNQuery/1M/k=8          25592 us        25588 us           28 PeakRSS_MB=192.668 items_per_second=39.0808M/s
SelfKNNQuery/1M/k=16         48506 us        48500 us           15 PeakRSS_MB=192.117 items_per_second=20.6186M/s
SelfKNNQuery/1M/k=32        103874 us       103862 us            7 PeakRSS_MB=192.117 items_per_second=9.62819M/s
SelfKNNQuery/1M/k=64        235986 us       235956 us            3 PeakRSS_MB=192.117 items_per_second=4.23808M/s
SelfKNNQuery/1M/k=128       733487 us       733398 us            1 PeakRSS_MB=192.117 items_per_second=1.36352M/s
SelfKNNQuery/10M/k=1         13100 us        13098 us           51 PeakRSS_MB=563.539 items_per_second=763.466M/s
SelfKNNQuery/10M/k=2         83636 us        83625 us            8 PeakRSS_MB=563.551 items_per_second=119.582M/s
SelfKNNQuery/10M/k=4        145489 us       145470 us            5 PeakRSS_MB=563.559 items_per_second=68.7426M/s
SelfKNNQuery/10M/k=8        251518 us       251488 us            3 PeakRSS_MB=564.129 items_per_second=39.7633M/s
SelfKNNQuery/10M/k=16       476420 us       476361 us            2 PeakRSS_MB=563.574 items_per_second=20.9925M/s
SelfKNNQuery/10M/k=32      1016157 us      1016031 us            1 PeakRSS_MB=563.578 items_per_second=9.84222M/s
SelfKNNQuery/10M/k=64      2356186 us      2355910 us            1 PeakRSS_MB=563.582 items_per_second=4.24464M/s
SelfKNNQuery/10M/k=128     7456061 us      7455160 us            1 PeakRSS_MB=563.598 items_per_second=1.34135M/s

```
