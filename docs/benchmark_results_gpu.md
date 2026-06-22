# GPU Benchmarking Results
## Hardware: NVIDIA A100-SXM4-40GB

### Morton Encoding
```
2026-05-31T10:57:50+02:00
Running ./morton_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.16, 1.53, 1.77
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------------
Benchmark                  Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------
MortonEncode/1K        0.025 ms        0.025 ms        22904 PeakRSS_MB=137 items_per_second=40.6944M/s
MortonEncode/10K       0.022 ms        0.016 ms        43457 PeakRSS_MB=137.562 items_per_second=616.191M/s
MortonEncode/100K      0.023 ms        0.022 ms        28104 PeakRSS_MB=141.406 items_per_second=4.51856G/s
MortonEncode/1M        0.030 ms        0.030 ms        20773 PeakRSS_MB=199.227 items_per_second=33.643G/s
MortonEncode/10M       0.167 ms        0.166 ms         4206 PeakRSS_MB=657.25 items_per_second=60.3281G/s
```

### GPU Sort (Zip Iterator)
```
2026-05-31T10:58:16+02:00
Running ./gpu_sort_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.45, 1.57, 1.78
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
GPU_Sort/1K        0.042 ms        0.042 ms        16563 PeakRSS_MB=138.34 items_per_second=23.7413M/s
GPU_Sort/10K       0.106 ms        0.105 ms         6633 PeakRSS_MB=140.027 items_per_second=94.9326M/s
GPU_Sort/100K      0.324 ms        0.321 ms         2188 PeakRSS_MB=147.34 items_per_second=311.944M/s
GPU_Sort/1M         1.11 ms         1.10 ms          634 PeakRSS_MB=184.223 items_per_second=906.687M/s
GPU_Sort/10M        11.5 ms         11.5 ms           59 PeakRSS_MB=283 items_per_second=873.185M/s
```

### Tree Build (HLBVH)
```
2026-05-31T10:58:33+02:00
Running ./tree_build_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.57, 1.59, 1.78
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------------
Benchmark               Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------
TreeBuild/1K         1.30 ms         1.28 ms          552 PeakRSS_MB=135.473 items_per_second=778.518k/s
TreeBuild/10K        2.94 ms         2.78 ms          270 PeakRSS_MB=137.16 items_per_second=3.60175M/s
TreeBuild/100K       10.0 ms         9.94 ms           68 PeakRSS_MB=140.922 items_per_second=10.0622M/s
TreeBuild/1M         78.4 ms         77.5 ms            9 PeakRSS_MB=201.527 items_per_second=12.8971M/s
TreeBuild/10M         874 ms          864 ms            1 PeakRSS_MB=548.059 items_per_second=11.5778M/s
```

### Tree Rebuild
```
2026-06-20T06:31:21+02:00
Running ./rebuild_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 2.55, 3.01, 2.65
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------
TreeRebuild/1K         1.04 ms         1.00 ms          626 PeakRSS_MB=135.523 items_per_second=997.94k/s
TreeRebuild/10K        2.29 ms         2.25 ms          283 PeakRSS_MB=136.648 items_per_second=4.44418M/s
TreeRebuild/100K       9.13 ms         9.11 ms          104 PeakRSS_MB=140.043 items_per_second=10.974M/s
TreeRebuild/1M         46.4 ms         46.4 ms           15 PeakRSS_MB=190.168 items_per_second=21.5702M/s
TreeRebuild/10M         363 ms          362 ms            2 PeakRSS_MB=630.059 items_per_second=27.5941M/s
```


### Range Query
```
2026-05-31T10:58:43+02:00
Running ./range_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.64, 1.61, 1.78
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
RangeQuery/1K/R=0.010000           47.2 us         46.8 us        14963 PeakRSS_MB=135.512 TotalFound=1k
RangeQuery/1K/R=0.100000           46.8 us         46.5 us        14929 PeakRSS_MB=136.074 TotalFound=1.005k
RangeQuery/1K/R=1.000000           59.7 us         59.1 us        11791 PeakRSS_MB=136.074 TotalFound=1.035k
RangeQuery/1K/R=10.000000           101 us          100 us         6953 PeakRSS_MB=136.074 TotalFound=6.591k
RangeQuery/1K/R=100.000000          515 us          511 us         1358 PeakRSS_MB=136.074 TotalFound=459.76k
RangeQuery/1K/R=200.000000          581 us          578 us         1204 PeakRSS_MB=136.074 TotalFound=931.404k
RangeQuery/10K/R=0.010000          58.4 us         58.0 us        12002 PeakRSS_MB=137.137 TotalFound=1k
RangeQuery/10K/R=0.100000          64.0 us         63.4 us        10963 PeakRSS_MB=137.074 TotalFound=1.001k
RangeQuery/10K/R=1.000000          83.3 us         82.8 us         8401 PeakRSS_MB=137.012 TotalFound=1.295k
RangeQuery/10K/R=10.000000          485 us          480 us         1455 PeakRSS_MB=137.012 TotalFound=53.444k
RangeQuery/10K/R=100.000000        6330 us         6191 us          114 PeakRSS_MB=136.949 TotalFound=971.536k
RangeQuery/10K/R=200.000000        5598 us         5541 us          125 PeakRSS_MB=137.512 TotalFound=977.028k
RangeQuery/100K/R=0.010000         72.6 us         72.0 us         9650 PeakRSS_MB=141.055 TotalFound=1k
RangeQuery/100K/R=0.100000         89.0 us         88.1 us         7914 PeakRSS_MB=141.742 TotalFound=1.005k
RangeQuery/100K/R=1.000000          233 us          231 us         3021 PeakRSS_MB=140.867 TotalFound=4.145k
RangeQuery/100K/R=10.000000        4037 us         4014 us          176 PeakRSS_MB=141.242 TotalFound=252.323k
RangeQuery/100K/R=100.000000      69724 us        69347 us           10 PeakRSS_MB=140.867 TotalFound=971.031k
RangeQuery/100K/R=200.000000      88503 us        88130 us            8 PeakRSS_MB=141.055 TotalFound=971.031k
RangeQuery/1M/R=0.010000           79.3 us         78.8 us         8797 PeakRSS_MB=192.004 TotalFound=1k
RangeQuery/1M/R=0.100000            117 us          117 us         5837 PeakRSS_MB=191.988 TotalFound=1.139k
RangeQuery/1M/R=1.000000           1265 us         1255 us          559 PeakRSS_MB=192.305 TotalFound=35.52k
RangeQuery/1M/R=10.000000         47050 us        46770 us           15 PeakRSS_MB=192.367 TotalFound=688.697k
RangeQuery/1M/R=100.000000       894808 us       887005 us            1 PeakRSS_MB=192.305 TotalFound=981.019k
RangeQuery/1M/R=200.000000      1006432 us       998510 us            1 PeakRSS_MB=192.242 TotalFound=981.019k
RangeQuery/10M/R=0.010000          92.4 us         91.5 us         7577 PeakRSS_MB=546.473 TotalFound=1.001k
RangeQuery/10M/R=0.100000           267 us          266 us         2630 PeakRSS_MB=546.254 TotalFound=1.793k
RangeQuery/10M/R=1.000000         12926 us        12777 us           54 PeakRSS_MB=546.457 TotalFound=116.137k
RangeQuery/10M/R=10.000000       475076 us       471200 us            2 PeakRSS_MB=546.109 TotalFound=952.08k
RangeQuery/10M/R=100.000000    11964457 us     11857782 us            1 PeakRSS_MB=546.172 TotalFound=976.025k
RangeQuery/10M/R=200.000000    16929720 us     16788229 us            1 PeakRSS_MB=546.238 TotalFound=976.029k
```

### kNN Query
```
2026-05-31T11:01:31+02:00
Running ./knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 2.02, 1.82, 1.84
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
KNNQuery/1K/k=1           37.2 us         36.7 us        19016 PeakRSS_MB=137.562
KNNQuery/1K/k=2           73.2 us         72.7 us         9624 PeakRSS_MB=137.562
KNNQuery/1K/k=4           94.5 us         93.5 us         7442 PeakRSS_MB=138.125
KNNQuery/1K/k=8            146 us          144 us         4851 PeakRSS_MB=138.125
KNNQuery/1K/k=16           256 us          253 us         2755 PeakRSS_MB=138.125
KNNQuery/1K/k=32           587 us          583 us         1195 PeakRSS_MB=138.125
KNNQuery/1K/k=64          1560 us         1552 us          456 PeakRSS_MB=138.125
KNNQuery/1K/k=128         4576 us         4539 us          155 PeakRSS_MB=137.562
KNNQuery/10K/k=1          50.4 us         50.1 us        13829 PeakRSS_MB=138.688
KNNQuery/10K/k=2           107 us          106 us         6605 PeakRSS_MB=139.188
KNNQuery/10K/k=4           160 us          159 us         4398 PeakRSS_MB=139.125
KNNQuery/10K/k=8           235 us          233 us         3008 PeakRSS_MB=139.062
KNNQuery/10K/k=16          423 us          420 us         1662 PeakRSS_MB=139.062
KNNQuery/10K/k=32          893 us          883 us          787 PeakRSS_MB=139.562
KNNQuery/10K/k=64         2324 us         2304 us          303 PeakRSS_MB=139
KNNQuery/10K/k=128        7617 us         7532 us           92 PeakRSS_MB=139.5
KNNQuery/100K/k=1         67.6 us         67.0 us        10416 PeakRSS_MB=143.68
KNNQuery/100K/k=2          124 us          122 us         5721 PeakRSS_MB=142.742
KNNQuery/100K/k=4          182 us          181 us         3858 PeakRSS_MB=143.859
KNNQuery/100K/k=8          263 us          261 us         2679 PeakRSS_MB=142.922
KNNQuery/100K/k=16         439 us          434 us         1608 PeakRSS_MB=144.078
KNNQuery/100K/k=32         923 us          915 us          767 PeakRSS_MB=143.625
KNNQuery/100K/k=64        2465 us         2441 us          288 PeakRSS_MB=143.453
KNNQuery/100K/k=128       7901 us         7844 us           89 PeakRSS_MB=143.207
KNNQuery/1M/k=1           76.4 us         75.5 us         9222 PeakRSS_MB=195.09
KNNQuery/1M/k=2            151 us          150 us         4660 PeakRSS_MB=194.715
KNNQuery/1M/k=4            212 us          210 us         3326 PeakRSS_MB=194.902
KNNQuery/1M/k=8            281 us          278 us         2506 PeakRSS_MB=194.527
KNNQuery/1M/k=16           487 us          483 us         1451 PeakRSS_MB=194.715
KNNQuery/1M/k=32          1038 us         1029 us          677 PeakRSS_MB=202.109
KNNQuery/1M/k=64          2873 us         2852 us          246 PeakRSS_MB=202.359
KNNQuery/1M/k=128         8857 us         8793 us           79 PeakRSS_MB=202.547
KNNQuery/10M/k=1          82.8 us         82.3 us         8434 PeakRSS_MB=550.133
KNNQuery/10M/k=2           168 us          166 us         4186 PeakRSS_MB=550.445
KNNQuery/10M/k=4           216 us          214 us         3248 PeakRSS_MB=550.203
KNNQuery/10M/k=8           321 us          318 us         2188 PeakRSS_MB=550.035
KNNQuery/10M/k=16          600 us          593 us         1172 PeakRSS_MB=550.289
KNNQuery/10M/k=32         1264 us         1255 us          555 PeakRSS_MB=550.859
KNNQuery/10M/k=64         3101 us         3079 us          228 PeakRSS_MB=550.551
KNNQuery/10M/k=128        9468 us         9403 us           74 PeakRSS_MB=550.742
```

