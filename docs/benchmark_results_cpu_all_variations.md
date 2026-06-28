# Benchmark Run: cpu
Date: Fri Jun 26 10:33:44 CEST 2026
Device: cpu
=============================================

## Configuration: SFC=MORTON, Decomp=HISTOGRAM, Coords=FLOAT
```
--- SFC Encoding Scaling ---
2026-06-26T10:34:38+02:00
Running ./sfc_encoding_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 7.18, 5.87, 4.93
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------
Benchmark                Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------
SFC-Encode/1K        0.174 ms        0.165 ms         3491 PeakRSS_MB=15.0898 items_per_second=6.0653M/s
SFC-Encode/10K       0.139 ms        0.135 ms         4936 PeakRSS_MB=15.6523 items_per_second=73.8414M/s
SFC-Encode/100K      0.248 ms        0.178 ms         3719 PeakRSS_MB=21.4102 items_per_second=560.668M/s
SFC-Encode/1M        0.517 ms        0.228 ms         2828 PeakRSS_MB=74.9023 items_per_second=4.38709G/s
SFC-Encode/10M        1.08 ms        0.262 ms         2577 PeakRSS_MB=536.691 items_per_second=38.2229G/s
--- GPU/Radix Sort Scaling ---
2026-06-26T10:35:05+02:00
Running ./gpu_sort_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 14.39, 7.46, 5.48
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
GPU_Sort/1K         1.76 ms         1.74 ms          540 PeakRSS_MB=32.0273 items_per_second=576.333k/s
GPU_Sort/10K        9.81 ms         9.80 ms           72 PeakRSS_MB=69.1523 items_per_second=1.02045M/s
GPU_Sort/100K        148 ms          148 ms            4 PeakRSS_MB=274.758 items_per_second=677.24k/s
GPU_Sort/1M         1471 ms         1462 ms            1 PeakRSS_MB=1.01746k items_per_second=683.781k/s
GPU_Sort/10M       16459 ms        16367 ms            1 PeakRSS_MB=1.18061k items_per_second=610.977k/s
--- Tree Build Scaling ---
2026-06-26T10:35:48+02:00
Running ./tree_build_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 7.91, 6.67, 5.29
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------------
Benchmark               Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------
TreeBuild/1K         5.34 ms         4.44 ms          181 PeakRSS_MB=22.3984 items_per_second=225.168k/s
TreeBuild/10K        17.9 ms         16.4 ms           41 PeakRSS_MB=49.9609 items_per_second=610.13k/s
TreeBuild/100K        209 ms          205 ms            4 PeakRSS_MB=258.973 items_per_second=486.68k/s
TreeBuild/1M         1883 ms         1856 ms            1 PeakRSS_MB=246.773 items_per_second=538.702k/s
TreeBuild/10M       19974 ms        19703 ms            1 PeakRSS_MB=1.93752k items_per_second=507.549k/s
--- Rebuild Scaling ---
2026-06-26T10:36:37+02:00
Running ./rebuild_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 4.04, 5.81, 5.07
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------
TreeRebuild/1K         3.22 ms         2.94 ms          213 PeakRSS_MB=24.0898 items_per_second=340.701k/s
TreeRebuild/10K        14.0 ms         12.9 ms           47 PeakRSS_MB=51.918 items_per_second=772.24k/s
TreeRebuild/100K        217 ms          209 ms            3 PeakRSS_MB=61.6914 items_per_second=478.938k/s
TreeRebuild/1M         1840 ms         1815 ms            1 PeakRSS_MB=2.23189k items_per_second=550.849k/s
TreeRebuild/10M       17407 ms        17112 ms            1 PeakRSS_MB=2.14733k items_per_second=584.384k/s
--- Range Query Scaling ---
2026-06-26T10:37:22+02:00
Running ./range_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 2.64, 5.21, 4.90
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
RangeQuery/1K/R=0.010000            419 us          347 us         2200 PeakRSS_MB=15.3672 TotalFound=1k
RangeQuery/1K/R=0.100000            310 us          273 us         2289 PeakRSS_MB=15.3672 TotalFound=1.005k
RangeQuery/1K/R=1.000000            282 us          269 us         2266 PeakRSS_MB=15.3672 TotalFound=1.035k
RangeQuery/1K/R=10.000000           194 us          189 us         3376 PeakRSS_MB=15.3672 TotalFound=6.591k
RangeQuery/1K/R=100.000000          718 us          717 us          967 PeakRSS_MB=15.3672 TotalFound=459.76k
RangeQuery/1K/R=200.000000          800 us          800 us          872 PeakRSS_MB=15.3672 TotalFound=931.404k
RangeQuery/10K/R=0.010000           166 us          158 us         4476 PeakRSS_MB=16.832 TotalFound=1k
RangeQuery/10K/R=0.100000           164 us          159 us         4436 PeakRSS_MB=35.4727 TotalFound=1.001k
RangeQuery/10K/R=1.000000           178 us          159 us         4435 PeakRSS_MB=36.0352 TotalFound=1.295k
RangeQuery/10K/R=10.000000          425 us          338 us         2165 PeakRSS_MB=36.5977 TotalFound=53.444k
RangeQuery/10K/R=100.000000        5479 us         5448 us          128 PeakRSS_MB=37.1602 TotalFound=971.536k
RangeQuery/10K/R=200.000000        8506 us         8445 us           83 PeakRSS_MB=37.1602 TotalFound=977.028k
RangeQuery/100K/R=0.010000          407 us          300 us         2353 PeakRSS_MB=256.195 TotalFound=1k
RangeQuery/100K/R=0.100000          375 us          274 us         2461 PeakRSS_MB=261.883 TotalFound=1.005k
RangeQuery/100K/R=1.000000          418 us          309 us         2186 PeakRSS_MB=269.199 TotalFound=4.145k
RangeQuery/100K/R=10.000000        1190 us         1185 us          567 PeakRSS_MB=286.473 TotalFound=252.323k
RangeQuery/100K/R=100.000000      48322 us        48302 us           14 PeakRSS_MB=299.973 TotalFound=971.031k
RangeQuery/100K/R=200.000000      94481 us        94448 us            7 PeakRSS_MB=302.785 TotalFound=971.031k
RangeQuery/1M/R=0.010000            297 us          192 us         3696 PeakRSS_MB=425.602 TotalFound=1k
RangeQuery/1M/R=0.100000            326 us          221 us         3291 PeakRSS_MB=542.406 TotalFound=1.139k
RangeQuery/1M/R=1.000000            573 us          352 us         2030 PeakRSS_MB=657.992 TotalFound=35.52k
RangeQuery/1M/R=10.000000          6921 us         6814 us          113 PeakRSS_MB=724.418 TotalFound=688.697k
RangeQuery/1M/R=100.000000       439726 us       439512 us            2 PeakRSS_MB=770.527 TotalFound=981.019k
RangeQuery/1M/R=200.000000       924757 us       923285 us            1 PeakRSS_MB=1.25244k TotalFound=981.019k
RangeQuery/10M/R=0.010000           396 us          295 us         2935 PeakRSS_MB=3.337k TotalFound=1.001k
RangeQuery/10M/R=0.100000           352 us          277 us         2749 PeakRSS_MB=4.60906k TotalFound=1.793k
RangeQuery/10M/R=1.000000          1898 us         1886 us          362 PeakRSS_MB=5.52398k TotalFound=116.137k
RangeQuery/10M/R=10.000000       117582 us       117544 us            6 PeakRSS_MB=10.5222k TotalFound=952.08k
RangeQuery/10M/R=100.000000     5632545 us      5630775 us            1 PeakRSS_MB=6.21189k TotalFound=976.025k
RangeQuery/10M/R=200.000000     9881313 us      9876997 us            1 PeakRSS_MB=6.43901k TotalFound=976.029k
--- kNN Query Scaling ---
2026-06-26T10:46:32+02:00
Running ./knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 10.57, 4.12, 4.11
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
KNNQuery/1K/k=1            159 us          158 us         4425 PeakRSS_MB=16.8242
KNNQuery/1K/k=2            177 us          177 us         4026 PeakRSS_MB=16.8242
KNNQuery/1K/k=4            224 us          209 us         3292 PeakRSS_MB=16.8242
KNNQuery/1K/k=8            273 us          254 us         2722 PeakRSS_MB=16.8242
KNNQuery/1K/k=16           381 us          353 us         1947 PeakRSS_MB=16.8242
KNNQuery/1K/k=32           701 us          689 us         1009 PeakRSS_MB=16.8242
KNNQuery/1K/k=64          1661 us         1660 us          438 PeakRSS_MB=16.8242
KNNQuery/1K/k=128         4727 us         4724 us          148 PeakRSS_MB=16.8242
KNNQuery/10K/k=1           332 us          262 us         3227 PeakRSS_MB=18.4883
KNNQuery/10K/k=2           286 us          239 us         3074 PeakRSS_MB=19.7344
KNNQuery/10K/k=4           314 us          253 us         2693 PeakRSS_MB=21.2656
KNNQuery/10K/k=8           448 us          357 us         2117 PeakRSS_MB=21.9648
KNNQuery/10K/k=16          606 us          522 us         1000 PeakRSS_MB=44.4648
KNNQuery/10K/k=32          826 us          811 us          878 PeakRSS_MB=44.582
KNNQuery/10K/k=64         1903 us         1710 us          411 PeakRSS_MB=46.2695
KNNQuery/10K/k=128        5026 us         5017 us          139 PeakRSS_MB=46.832
KNNQuery/100K/k=1          502 us          398 us         1925 PeakRSS_MB=48.332
KNNQuery/100K/k=2          520 us          420 us         1710 PeakRSS_MB=54.2539
KNNQuery/100K/k=4          540 us          444 us         1575 PeakRSS_MB=59.4609
KNNQuery/100K/k=8          618 us          532 us         1151 PeakRSS_MB=68.9141
KNNQuery/100K/k=16         693 us          648 us         1117 PeakRSS_MB=91.5039
KNNQuery/100K/k=32         990 us          985 us          686 PeakRSS_MB=97.3711
KNNQuery/100K/k=64        2020 us         2017 us          344 PeakRSS_MB=100.93
KNNQuery/100K/k=128       5518 us         5484 us          128 PeakRSS_MB=105.312
KNNQuery/1M/k=1            321 us          256 us         2779 PeakRSS_MB=329.809
KNNQuery/1M/k=2            376 us          312 us         1689 PeakRSS_MB=444.957
KNNQuery/1M/k=4            355 us          316 us         2116 PeakRSS_MB=1.05507k
KNNQuery/1M/k=8            427 us          386 us         1810 PeakRSS_MB=671.113
KNNQuery/1M/k=16           620 us          533 us         1354 PeakRSS_MB=787.988
KNNQuery/1M/k=32           865 us          863 us          608 PeakRSS_MB=879.32
KNNQuery/1M/k=64          2424 us         2181 us          364 PeakRSS_MB=969.664
KNNQuery/1M/k=128         6808 us         5746 us          129 PeakRSS_MB=1.04065k
KNNQuery/10M/k=1           362 us          320 us         2239 PeakRSS_MB=7.01389k
KNNQuery/10M/k=2           420 us          380 us         1844 PeakRSS_MB=7.47166k
KNNQuery/10M/k=4           460 us          432 us         1691 PeakRSS_MB=8.38769k
KNNQuery/10M/k=8           526 us          507 us         1000 PeakRSS_MB=9.07445k
KNNQuery/10M/k=16          874 us          870 us         1038 PeakRSS_MB=9.98982k
KNNQuery/10M/k=32          965 us          964 us          705 PeakRSS_MB=10.9051k
KNNQuery/10M/k=64         2002 us         1999 us          355 PeakRSS_MB=11.8204k
KNNQuery/10M/k=128        6385 us         5570 us          121 PeakRSS_MB=12.5069k
```

## Configuration: SFC=MORTON, Decomp=HISTOGRAM, Coords=DOUBLE
```
--- SFC Encoding Scaling ---
2026-06-26T11:01:19+02:00
Running ./sfc_encoding_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 5.96, 3.27, 3.62
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------
Benchmark                Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------
SFC-Encode/1K        0.398 ms        0.298 ms         2230 PeakRSS_MB=15.0898 items_per_second=3.35437M/s
SFC-Encode/10K       0.192 ms        0.181 ms         3836 PeakRSS_MB=15.6523 items_per_second=55.242M/s
SFC-Encode/100K      0.221 ms        0.188 ms         3617 PeakRSS_MB=20.9414 items_per_second=532.357M/s
SFC-Encode/1M        0.324 ms        0.165 ms         3394 PeakRSS_MB=75.9492 items_per_second=6.07353G/s
SFC-Encode/10M        2.03 ms        0.333 ms         2169 PeakRSS_MB=615.656 items_per_second=30.0456G/s
--- GPU/Radix Sort Scaling ---
2026-06-26T11:01:49+02:00
Running ./gpu_sort_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 7.69, 3.98, 3.85
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
GPU_Sort/1K         1.80 ms         1.77 ms          462 PeakRSS_MB=29.2148 items_per_second=564.694k/s
GPU_Sort/10K        9.86 ms         9.84 ms           73 PeakRSS_MB=66.9023 items_per_second=1.01636M/s
GPU_Sort/100K        161 ms          160 ms            4 PeakRSS_MB=121.719 items_per_second=623.323k/s
GPU_Sort/1M          919 ms          914 ms            1 PeakRSS_MB=2.21586k items_per_second=1.09384M/s
GPU_Sort/10M       20919 ms        20808 ms            1 PeakRSS_MB=9.2173k items_per_second=480.578k/s
--- Tree Build Scaling ---
2026-06-26T11:02:41+02:00
Running ./tree_build_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 3.67, 3.48, 3.69
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------------
Benchmark               Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------
TreeBuild/1K         5.44 ms         4.75 ms          228 PeakRSS_MB=24.6523 items_per_second=210.593k/s
TreeBuild/10K        18.6 ms         16.6 ms           39 PeakRSS_MB=51.4688 items_per_second=601.861k/s
TreeBuild/100K        186 ms          175 ms            5 PeakRSS_MB=66.2852 items_per_second=569.884k/s
TreeBuild/1M         1829 ms         1802 ms            1 PeakRSS_MB=2.48271k items_per_second=554.881k/s
TreeBuild/10M       16202 ms        15853 ms            1 PeakRSS_MB=7.06332k items_per_second=630.786k/s
--- Rebuild Scaling ---
2026-06-26T11:03:26+02:00
Running ./rebuild_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 2.51, 3.20, 3.58
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------
TreeRebuild/1K         3.23 ms         2.80 ms          298 PeakRSS_MB=26.9023 items_per_second=357.331k/s
TreeRebuild/10K        14.0 ms         12.9 ms           51 PeakRSS_MB=60.2617 items_per_second=773.787k/s
TreeRebuild/100K        140 ms          136 ms            5 PeakRSS_MB=275.789 items_per_second=734.195k/s
TreeRebuild/1M         1906 ms         1875 ms            1 PeakRSS_MB=786.551 items_per_second=533.393k/s
TreeRebuild/10M       19013 ms        18780 ms            1 PeakRSS_MB=1.7201k items_per_second=532.482k/s
--- Range Query Scaling ---
2026-06-26T11:04:14+02:00
Running ./range_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.71, 2.89, 3.46
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
RangeQuery/1K/R=0.010000            167 us          166 us         4290 PeakRSS_MB=17.4688 TotalFound=1k
RangeQuery/1K/R=0.100000            163 us          162 us         4308 PeakRSS_MB=18.0312 TotalFound=1.005k
RangeQuery/1K/R=1.000000            165 us          164 us         4277 PeakRSS_MB=18.0312 TotalFound=1.035k
RangeQuery/1K/R=10.000000           166 us          165 us         4160 PeakRSS_MB=18.0312 TotalFound=6.591k
RangeQuery/1K/R=100.000000          662 us          654 us         1054 PeakRSS_MB=18.0312 TotalFound=459.76k
RangeQuery/1K/R=200.000000          808 us          808 us          863 PeakRSS_MB=18.0312 TotalFound=931.404k
RangeQuery/10K/R=0.010000           176 us          175 us         4000 PeakRSS_MB=32.6094 TotalFound=1k
RangeQuery/10K/R=0.100000           176 us          175 us         3891 PeakRSS_MB=33.1719 TotalFound=1.001k
RangeQuery/10K/R=1.000000           169 us          168 us         4146 PeakRSS_MB=33.7344 TotalFound=1.295k
RangeQuery/10K/R=10.000000          350 us          261 us         2806 PeakRSS_MB=34.2969 TotalFound=53.444k
RangeQuery/10K/R=100.000000        5633 us         5585 us          129 PeakRSS_MB=35.4219 TotalFound=971.536k
RangeQuery/10K/R=200.000000        9470 us         9295 us           82 PeakRSS_MB=35.4219 TotalFound=977.028k
RangeQuery/100K/R=0.010000          406 us          287 us         2063 PeakRSS_MB=262.258 TotalFound=1k
RangeQuery/100K/R=0.100000          400 us          298 us         2187 PeakRSS_MB=267.297 TotalFound=1.005k
RangeQuery/100K/R=1.000000          479 us          351 us         2010 PeakRSS_MB=280.984 TotalFound=4.145k
RangeQuery/100K/R=10.000000        1318 us         1316 us          513 PeakRSS_MB=293.359 TotalFound=252.323k
RangeQuery/100K/R=100.000000      51463 us        51442 us           14 PeakRSS_MB=297.859 TotalFound=971.031k
RangeQuery/100K/R=200.000000      99743 us        99707 us            7 PeakRSS_MB=302.547 TotalFound=971.031k
RangeQuery/1M/R=0.010000            279 us          209 us         3016 PeakRSS_MB=406.824 TotalFound=1k
RangeQuery/1M/R=0.100000            283 us          216 us         3427 PeakRSS_MB=2.55723k TotalFound=1.139k
RangeQuery/1M/R=1.000000            413 us          284 us         2205 PeakRSS_MB=637.957 TotalFound=35.52k
RangeQuery/1M/R=10.000000          7121 us         7043 us           91 PeakRSS_MB=707.832 TotalFound=688.697k
RangeQuery/1M/R=100.000000       546062 us       545751 us            1 PeakRSS_MB=729.609 TotalFound=981.019k
RangeQuery/1M/R=200.000000      1055706 us      1055366 us            1 PeakRSS_MB=1.2603k TotalFound=981.019k
RangeQuery/10M/R=0.010000           288 us          228 us         2436 PeakRSS_MB=3.56946k TotalFound=1.001k
RangeQuery/10M/R=0.100000           244 us          238 us         2885 PeakRSS_MB=4.90287k TotalFound=1.793k
RangeQuery/10M/R=1.000000          2052 us         2022 us          344 PeakRSS_MB=5.74242k TotalFound=116.137k
RangeQuery/10M/R=10.000000       138951 us       138888 us            5 PeakRSS_MB=24.4352k TotalFound=952.08k
RangeQuery/10M/R=100.000000     7097125 us      7094474 us            1 PeakRSS_MB=24.6649k TotalFound=976.025k
RangeQuery/10M/R=200.000000    11214937 us     11209668 us            1 PeakRSS_MB=6.60218k TotalFound=976.029k
--- kNN Query Scaling ---
2026-06-26T11:12:38+02:00
Running ./knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 8.57, 4.48, 3.64
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
KNNQuery/1K/k=1            173 us          170 us         4219 PeakRSS_MB=16.8242
KNNQuery/1K/k=2            182 us          180 us         3897 PeakRSS_MB=17.3867
KNNQuery/1K/k=4            246 us          224 us         3337 PeakRSS_MB=17.3867
KNNQuery/1K/k=8            270 us          268 us         2612 PeakRSS_MB=17.3867
KNNQuery/1K/k=16           373 us          365 us         1904 PeakRSS_MB=17.3867
KNNQuery/1K/k=32           718 us          717 us          984 PeakRSS_MB=17.3867
KNNQuery/1K/k=64          1952 us         1637 us          428 PeakRSS_MB=17.3867
KNNQuery/1K/k=128         4793 us         4769 us          152 PeakRSS_MB=17.3867
KNNQuery/10K/k=1           423 us          375 us         2202 PeakRSS_MB=40.4492
KNNQuery/10K/k=2           306 us          273 us         2087 PeakRSS_MB=40.4492
KNNQuery/10K/k=4           503 us          389 us         2697 PeakRSS_MB=40.4492
KNNQuery/10K/k=8           547 us          441 us         1619 PeakRSS_MB=40.4492
KNNQuery/10K/k=16          629 us          553 us         1252 PeakRSS_MB=40.4492
KNNQuery/10K/k=32          868 us          839 us          848 PeakRSS_MB=41.5742
KNNQuery/10K/k=64         1748 us         1745 us          402 PeakRSS_MB=42.1367
KNNQuery/10K/k=128        5061 us         5023 us          139 PeakRSS_MB=42.1367
KNNQuery/100K/k=1          482 us          376 us         2024 PeakRSS_MB=46.8242
KNNQuery/100K/k=2          466 us          361 us         1761 PeakRSS_MB=54.082
KNNQuery/100K/k=4          552 us          460 us         1517 PeakRSS_MB=64.7188
KNNQuery/100K/k=8          599 us          533 us         1393 PeakRSS_MB=79.9844
KNNQuery/100K/k=16         681 us          674 us         1013 PeakRSS_MB=92.2148
KNNQuery/100K/k=32        1118 us         1116 us          622 PeakRSS_MB=93.9648
KNNQuery/100K/k=64        2157 us         2156 us          318 PeakRSS_MB=100.961
KNNQuery/100K/k=128       5670 us         5633 us          124 PeakRSS_MB=105.363
KNNQuery/1M/k=1            287 us          251 us         2356 PeakRSS_MB=915.148
KNNQuery/1M/k=2            332 us          280 us         2530 PeakRSS_MB=503.176
KNNQuery/1M/k=4            361 us          327 us         2057 PeakRSS_MB=614.203
KNNQuery/1M/k=8            437 us          408 us         1742 PeakRSS_MB=730.594
KNNQuery/1M/k=16           571 us          539 us         1266 PeakRSS_MB=824.754
KNNQuery/1M/k=32           922 us          921 us          748 PeakRSS_MB=913.168
KNNQuery/1M/k=64          1947 us         1947 us          366 PeakRSS_MB=1.00759k
KNNQuery/1M/k=128         6761 us         5933 us          125 PeakRSS_MB=1.07422k
KNNQuery/10M/k=1           339 us          285 us         2487 PeakRSS_MB=8.48045k
KNNQuery/10M/k=2           399 us          390 us         1852 PeakRSS_MB=5.27517k
KNNQuery/10M/k=4           497 us          459 us         1580 PeakRSS_MB=6.41961k
KNNQuery/10M/k=8           752 us          582 us          908 PeakRSS_MB=7.19558k
KNNQuery/10M/k=16          679 us          676 us          845 PeakRSS_MB=8.2504k
KNNQuery/10M/k=32         1004 us         1002 us          662 PeakRSS_MB=8.97538k
KNNQuery/10M/k=64         2060 us         2059 us          340 PeakRSS_MB=10.0052k
KNNQuery/10M/k=128        6009 us         5996 us          110 PeakRSS_MB=10.7671k
```

## Configuration: SFC=MORTON, Decomp=SAMPLING, Coords=FLOAT
```
--- SFC Encoding Scaling ---
2026-06-26T11:27:14+02:00
Running ./sfc_encoding_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 4.82, 3.20, 3.03
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------
Benchmark                Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------
SFC-Encode/1K        0.184 ms        0.176 ms         2990 PeakRSS_MB=15.082 items_per_second=5.68181M/s
SFC-Encode/10K       0.177 ms        0.157 ms         4867 PeakRSS_MB=15.6445 items_per_second=63.8059M/s
SFC-Encode/100K      0.196 ms        0.169 ms         3857 PeakRSS_MB=17.6641 items_per_second=591.481M/s
SFC-Encode/1M        0.330 ms        0.162 ms         3467 PeakRSS_MB=72.3594 items_per_second=6.17973G/s
SFC-Encode/10M        1.08 ms        0.278 ms         2491 PeakRSS_MB=534.391 items_per_second=36.0162G/s
--- GPU/Radix Sort Scaling ---
2026-06-26T11:27:41+02:00
Running ./gpu_sort_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 10.79, 4.65, 3.51
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
GPU_Sort/1K         1.79 ms         1.77 ms          526 PeakRSS_MB=31.1484 items_per_second=565.157k/s
GPU_Sort/10K        10.0 ms         10.0 ms           72 PeakRSS_MB=67.7109 items_per_second=998.678k/s
GPU_Sort/100K        159 ms          155 ms            4 PeakRSS_MB=274.418 items_per_second=645.594k/s
GPU_Sort/1M         1602 ms         1597 ms            1 PeakRSS_MB=2.23378k items_per_second=626.097k/s
GPU_Sort/10M       14204 ms        14037 ms            1 PeakRSS_MB=20.7384k items_per_second=712.417k/s
--- Tree Build Scaling ---
2026-06-26T11:28:21+02:00
Running ./tree_build_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 6.02, 4.19, 3.40
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------------
Benchmark               Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------
TreeBuild/1K         3.13 ms         2.85 ms          200 PeakRSS_MB=24.0898 items_per_second=350.959k/s
TreeBuild/10K        14.6 ms         13.6 ms           58 PeakRSS_MB=55.5898 items_per_second=737.705k/s
TreeBuild/100K        208 ms          205 ms            3 PeakRSS_MB=106.332 items_per_second=488.716k/s
TreeBuild/1M         1906 ms         1879 ms            1 PeakRSS_MB=151.328 items_per_second=532.21k/s
TreeBuild/10M       15086 ms        14870 ms            1 PeakRSS_MB=20.2125k items_per_second=672.512k/s
--- Rebuild Scaling ---
2026-06-26T11:29:05+02:00
Running ./rebuild_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 3.57, 3.79, 3.30
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------
TreeRebuild/1K         4.51 ms         3.92 ms          201 PeakRSS_MB=24.6484 items_per_second=255.229k/s
TreeRebuild/10K        19.3 ms         17.4 ms           46 PeakRSS_MB=50.5469 items_per_second=574.254k/s
TreeRebuild/100K        240 ms          232 ms            3 PeakRSS_MB=59.7578 items_per_second=430.275k/s
TreeRebuild/1M         1857 ms         1830 ms            1 PeakRSS_MB=216.316 items_per_second=546.303k/s
TreeRebuild/10M       17780 ms        17522 ms            1 PeakRSS_MB=1.50069k items_per_second=570.71k/s
--- Range Query Scaling ---
2026-06-26T11:29:51+02:00
Running ./range_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 2.24, 3.40, 3.19
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
RangeQuery/1K/R=0.010000            205 us          186 us         3789 PeakRSS_MB=17.7461 TotalFound=1k
RangeQuery/1K/R=0.100000            205 us          187 us         3745 PeakRSS_MB=17.7461 TotalFound=1.005k
RangeQuery/1K/R=1.000000            188 us          177 us         3533 PeakRSS_MB=18.3086 TotalFound=1.035k
RangeQuery/1K/R=10.000000           191 us          190 us         3684 PeakRSS_MB=18.3086 TotalFound=6.591k
RangeQuery/1K/R=100.000000          724 us          722 us          957 PeakRSS_MB=18.3086 TotalFound=459.76k
RangeQuery/1K/R=200.000000          776 us          775 us          896 PeakRSS_MB=18.3086 TotalFound=931.404k
RangeQuery/10K/R=0.010000           414 us          346 us         2060 PeakRSS_MB=37.8477 TotalFound=1k
RangeQuery/10K/R=0.100000           338 us          309 us         2296 PeakRSS_MB=38.9727 TotalFound=1.001k
RangeQuery/10K/R=1.000000           413 us          338 us         2130 PeakRSS_MB=38.9727 TotalFound=1.295k
RangeQuery/10K/R=10.000000          528 us          434 us         1627 PeakRSS_MB=38.9727 TotalFound=53.444k
RangeQuery/10K/R=100.000000        5631 us         5588 us          127 PeakRSS_MB=38.9727 TotalFound=4.38585M
RangeQuery/10K/R=200.000000        8817 us         8712 us           81 PeakRSS_MB=38.9727 TotalFound=9.12136M
RangeQuery/100K/R=0.010000          423 us          342 us         2037 PeakRSS_MB=255.895 TotalFound=1k
RangeQuery/100K/R=0.100000          441 us          355 us         2080 PeakRSS_MB=263.637 TotalFound=1.005k
RangeQuery/100K/R=1.000000          470 us          385 us         1789 PeakRSS_MB=272.637 TotalFound=4.145k
RangeQuery/100K/R=10.000000        1235 us         1231 us          535 PeakRSS_MB=285.504 TotalFound=506.846k
RangeQuery/100K/R=100.000000      48043 us        48024 us           15 PeakRSS_MB=299.598 TotalFound=43.2318M
RangeQuery/100K/R=200.000000      93047 us        93011 us            8 PeakRSS_MB=304.637 TotalFound=90.3647M
RangeQuery/1M/R=0.010000            446 us          344 us         3123 PeakRSS_MB=379.051 TotalFound=1k
RangeQuery/1M/R=0.100000            323 us          230 us         3101 PeakRSS_MB=928.445 TotalFound=1.139k
RangeQuery/1M/R=1.000000            682 us          507 us         2080 PeakRSS_MB=559.035 TotalFound=39.044k
RangeQuery/1M/R=10.000000          6510 us         6447 us           95 PeakRSS_MB=627.504 TotalFound=5.78161M
RangeQuery/1M/R=100.000000       444704 us       444518 us            2 PeakRSS_MB=2.91054k TotalFound=433.516M
RangeQuery/1M/R=200.000000       927622 us       927381 us            1 PeakRSS_MB=697.863 TotalFound=907.777M
RangeQuery/10M/R=0.010000           354 us          249 us         3150 PeakRSS_MB=3.02234k TotalFound=1.001k
RangeQuery/10M/R=0.100000           441 us          355 us         1940 PeakRSS_MB=4.35786k TotalFound=1.793k
RangeQuery/10M/R=1.000000          1919 us         1904 us          367 PeakRSS_MB=5.2702k TotalFound=363.096k
RangeQuery/10M/R=10.000000       124059 us       124012 us            6 PeakRSS_MB=5.73136k TotalFound=55.2472M
RangeQuery/10M/R=100.000000     5669339 us      5666362 us            1 PeakRSS_MB=5.95771k TotalFound=4.39221G
RangeQuery/10M/R=200.000000    10223302 us     10219005 us            1 PeakRSS_MB=6.18611k TotalFound=9.09205G
--- kNN Query Scaling ---
2026-06-26T11:38:35+02:00
Running ./knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 9.14, 4.74, 3.70
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
KNNQuery/1K/k=1            159 us          158 us         4470 PeakRSS_MB=16.8242
KNNQuery/1K/k=2            176 us          176 us         4045 PeakRSS_MB=16.8242
KNNQuery/1K/k=4            206 us          205 us         3542 PeakRSS_MB=16.8242
KNNQuery/1K/k=8            234 us          232 us         3024 PeakRSS_MB=16.8242
KNNQuery/1K/k=16           392 us          355 us         1974 PeakRSS_MB=16.8242
KNNQuery/1K/k=32           684 us          682 us         1018 PeakRSS_MB=16.8242
KNNQuery/1K/k=64          1616 us         1614 us          435 PeakRSS_MB=16.8242
KNNQuery/1K/k=128         4653 us         4641 us          151 PeakRSS_MB=16.8242
KNNQuery/10K/k=1           344 us          283 us         2622 PeakRSS_MB=18.2734
KNNQuery/10K/k=2           367 us          303 us         2230 PeakRSS_MB=19.6758
KNNQuery/10K/k=4           501 us          394 us         1913 PeakRSS_MB=22.4414
KNNQuery/10K/k=8           536 us          437 us         1784 PeakRSS_MB=23.9961
KNNQuery/10K/k=16          608 us          528 us         1000 PeakRSS_MB=24.9297
KNNQuery/10K/k=32          813 us          804 us          895 PeakRSS_MB=26.1367
KNNQuery/10K/k=64         1689 us         1687 us          412 PeakRSS_MB=27.0859
KNNQuery/10K/k=128        5330 us         4998 us          139 PeakRSS_MB=27.5664
KNNQuery/100K/k=1          481 us          373 us         2073 PeakRSS_MB=39.9648
KNNQuery/100K/k=2          466 us          371 us         1674 PeakRSS_MB=38.5312
KNNQuery/100K/k=4          527 us          435 us         1567 PeakRSS_MB=55.4453
KNNQuery/100K/k=8          610 us          532 us         1000 PeakRSS_MB=67.1641
KNNQuery/100K/k=16         700 us          660 us          999 PeakRSS_MB=75.5547
KNNQuery/100K/k=32        1001 us          996 us          686 PeakRSS_MB=79.7539
KNNQuery/100K/k=64        2050 us         2049 us          339 PeakRSS_MB=90.7695
KNNQuery/100K/k=128       5572 us         5513 us          128 PeakRSS_MB=96.6016
KNNQuery/1M/k=1            270 us          236 us         2966 PeakRSS_MB=320.301
KNNQuery/1M/k=2            319 us          280 us         2301 PeakRSS_MB=437.258
KNNQuery/1M/k=4            385 us          335 us         2162 PeakRSS_MB=553.082
KNNQuery/1M/k=8            477 us          438 us         1475 PeakRSS_MB=665.488
KNNQuery/1M/k=16           562 us          533 us         1339 PeakRSS_MB=757.664
KNNQuery/1M/k=32          1149 us         1076 us          641 PeakRSS_MB=853.297
KNNQuery/1M/k=64          2507 us         1938 us          365 PeakRSS_MB=944.148
KNNQuery/1M/k=128         6750 us         5788 us          122 PeakRSS_MB=1.01278k
KNNQuery/10M/k=1           319 us          304 us         2315 PeakRSS_MB=3.36711k
KNNQuery/10M/k=2           389 us          371 us         1972 PeakRSS_MB=4.5112k
KNNQuery/10M/k=4           447 us          436 us         1284 PeakRSS_MB=5.61804k
KNNQuery/10M/k=8           670 us          535 us         1377 PeakRSS_MB=6.76268k
KNNQuery/10M/k=16          637 us          631 us          973 PeakRSS_MB=7.77219k
KNNQuery/10M/k=32         1845 us          991 us          719 PeakRSS_MB=8.51719k
KNNQuery/10M/k=64         1997 us         1997 us          292 PeakRSS_MB=9.61124k
KNNQuery/10M/k=128        5466 us         5464 us          125 PeakRSS_MB=10.1956k
```

## Configuration: SFC=MORTON, Decomp=SAMPLING, Coords=DOUBLE
```
--- SFC Encoding Scaling ---
2026-06-26T11:53:01+02:00
Running ./sfc_encoding_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 4.71, 3.19, 3.25
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------
Benchmark                Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------
SFC-Encode/1K        0.259 ms        0.228 ms         3392 PeakRSS_MB=15.0898 items_per_second=4.39356M/s
SFC-Encode/10K       0.331 ms        0.276 ms         2913 PeakRSS_MB=15.6523 items_per_second=36.1734M/s
SFC-Encode/100K      0.415 ms        0.307 ms         2193 PeakRSS_MB=17.1875 items_per_second=326.089M/s
SFC-Encode/1M        0.654 ms        0.334 ms         2479 PeakRSS_MB=70.9766 items_per_second=2.99546G/s
SFC-Encode/10M        1.99 ms        0.337 ms         2026 PeakRSS_MB=630.289 items_per_second=29.6884G/s
--- GPU/Radix Sort Scaling ---
2026-06-26T11:53:32+02:00
Running ./gpu_sort_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 8.57, 4.20, 3.58
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
GPU_Sort/1K         1.70 ms         1.67 ms          501 PeakRSS_MB=30.9023 items_per_second=597.115k/s
GPU_Sort/10K        9.88 ms         9.87 ms           72 PeakRSS_MB=67.4648 items_per_second=1.0134M/s
GPU_Sort/100K        154 ms          154 ms            4 PeakRSS_MB=123.965 items_per_second=650.697k/s
GPU_Sort/1M         1421 ms         1417 ms            1 PeakRSS_MB=2.21751k items_per_second=705.876k/s
GPU_Sort/10M       16048 ms        15955 ms            1 PeakRSS_MB=7.31005k items_per_second=626.759k/s
--- Tree Build Scaling ---
2026-06-26T11:54:17+02:00
Running ./tree_build_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 4.57, 3.75, 3.46
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------------
Benchmark               Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------
TreeBuild/1K         3.22 ms         2.96 ms          273 PeakRSS_MB=26.8984 items_per_second=338.341k/s
TreeBuild/10K        16.9 ms         15.2 ms           56 PeakRSS_MB=58.8047 items_per_second=656.255k/s
TreeBuild/100K        213 ms          209 ms            5 PeakRSS_MB=267.688 items_per_second=477.758k/s
TreeBuild/1M         1989 ms         1955 ms            1 PeakRSS_MB=735.141 items_per_second=511.44k/s
TreeBuild/10M       19032 ms        18316 ms            1 PeakRSS_MB=5.73649k items_per_second=545.974k/s
--- Rebuild Scaling ---
2026-06-26T11:55:05+02:00
Running ./rebuild_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 2.74, 3.38, 3.34
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------
TreeRebuild/1K         3.67 ms         2.94 ms          318 PeakRSS_MB=27.4648 items_per_second=340.329k/s
TreeRebuild/10K        13.1 ms         12.1 ms           52 PeakRSS_MB=60.0898 items_per_second=826.061k/s
TreeRebuild/100K        233 ms          226 ms            4 PeakRSS_MB=66.5547 items_per_second=443.328k/s
TreeRebuild/1M         2158 ms         2044 ms            1 PeakRSS_MB=317.434 items_per_second=489.33k/s
TreeRebuild/10M       16870 ms        16581 ms            1 PeakRSS_MB=1.66162k items_per_second=603.1k/s
--- Range Query Scaling ---
2026-06-26T11:55:52+02:00
Running ./range_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 7.44, 4.19, 3.60
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
RangeQuery/1K/R=0.010000            266 us          244 us         3228 PeakRSS_MB=17.5586 TotalFound=1k
RangeQuery/1K/R=0.100000            211 us          197 us         2822 PeakRSS_MB=17.5586 TotalFound=1.005k
RangeQuery/1K/R=1.000000            160 us          153 us         4602 PeakRSS_MB=17.5586 TotalFound=1.035k
RangeQuery/1K/R=10.000000           192 us          169 us         3945 PeakRSS_MB=17.5586 TotalFound=6.591k
RangeQuery/1K/R=100.000000          727 us          701 us          980 PeakRSS_MB=17.5586 TotalFound=459.76k
RangeQuery/1K/R=200.000000          780 us          777 us          898 PeakRSS_MB=17.5586 TotalFound=931.404k
RangeQuery/10K/R=0.010000           212 us          192 us         3649 PeakRSS_MB=35.7539 TotalFound=1k
RangeQuery/10K/R=0.100000           154 us          153 us         3707 PeakRSS_MB=37.4414 TotalFound=1.001k
RangeQuery/10K/R=1.000000           228 us          202 us         4478 PeakRSS_MB=37.4414 TotalFound=1.295k
RangeQuery/10K/R=10.000000          328 us          276 us         2667 PeakRSS_MB=37.4414 TotalFound=53.444k
RangeQuery/10K/R=100.000000        5549 us         5494 us          128 PeakRSS_MB=38.0039 TotalFound=4.38585M
RangeQuery/10K/R=200.000000        8663 us         8553 us           82 PeakRSS_MB=38.0039 TotalFound=9.12136M
RangeQuery/100K/R=0.010000          484 us          350 us         1702 PeakRSS_MB=263.98 TotalFound=1k
RangeQuery/100K/R=0.100000          388 us          295 us         2502 PeakRSS_MB=271.973 TotalFound=1.005k
RangeQuery/100K/R=1.000000          463 us          335 us         2088 PeakRSS_MB=283.352 TotalFound=4.145k
RangeQuery/100K/R=10.000000        1295 us         1288 us          538 PeakRSS_MB=289.457 TotalFound=506.846k
RangeQuery/100K/R=100.000000      51399 us        51379 us           14 PeakRSS_MB=298.457 TotalFound=43.2318M
RangeQuery/100K/R=200.000000      98789 us        98754 us            7 PeakRSS_MB=305.906 TotalFound=90.3647M
RangeQuery/1M/R=0.010000            292 us          206 us         3314 PeakRSS_MB=378.199 TotalFound=1k
RangeQuery/1M/R=0.100000            270 us          207 us         3309 PeakRSS_MB=494.887 TotalFound=1.139k
RangeQuery/1M/R=1.000000            415 us          292 us         2529 PeakRSS_MB=610.66 TotalFound=39.044k
RangeQuery/1M/R=10.000000          7009 us         6978 us          100 PeakRSS_MB=677.863 TotalFound=5.78161M
RangeQuery/1M/R=100.000000       564446 us       564048 us            1 PeakRSS_MB=700.91 TotalFound=433.516M
RangeQuery/1M/R=200.000000      1062648 us      1062283 us            1 PeakRSS_MB=1.25764k TotalFound=907.777M
RangeQuery/10M/R=0.010000           262 us          214 us         3164 PeakRSS_MB=3.57338k TotalFound=1.001k
RangeQuery/10M/R=0.100000           253 us          250 us         2607 PeakRSS_MB=4.7168k TotalFound=1.793k
RangeQuery/10M/R=1.000000          2010 us         2001 us          276 PeakRSS_MB=5.82075k TotalFound=363.096k
RangeQuery/10M/R=10.000000       137099 us       137045 us            5 PeakRSS_MB=6.28082k TotalFound=55.2471M
RangeQuery/10M/R=100.000000     6673915 us      6671548 us            1 PeakRSS_MB=6.5082k TotalFound=4.39221G
RangeQuery/10M/R=200.000000    10750178 us     10744582 us            1 PeakRSS_MB=6.73768k TotalFound=9.09205G
--- kNN Query Scaling ---
2026-06-26T12:03:54+02:00
Running ./knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 7.95, 4.58, 3.89
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
KNNQuery/1K/k=1            175 us          173 us         4021 PeakRSS_MB=16.8242
KNNQuery/1K/k=2            187 us          186 us         3741 PeakRSS_MB=16.8242
KNNQuery/1K/k=4            235 us          216 us         3417 PeakRSS_MB=16.8242
KNNQuery/1K/k=8            287 us          266 us         2687 PeakRSS_MB=16.8242
KNNQuery/1K/k=16           378 us          368 us         1863 PeakRSS_MB=16.8242
KNNQuery/1K/k=32           693 us          692 us         1006 PeakRSS_MB=17.3867
KNNQuery/1K/k=64          1622 us         1620 us          421 PeakRSS_MB=17.3867
KNNQuery/1K/k=128         4626 us         4619 us          146 PeakRSS_MB=17.3867
KNNQuery/10K/k=1           380 us          304 us         3121 PeakRSS_MB=40.4492
KNNQuery/10K/k=2           270 us          234 us         2158 PeakRSS_MB=41.0117
KNNQuery/10K/k=4           312 us          261 us         2924 PeakRSS_MB=41.5742
KNNQuery/10K/k=8           399 us          325 us         2272 PeakRSS_MB=41.5742
KNNQuery/10K/k=16          560 us          513 us         1447 PeakRSS_MB=41.5742
KNNQuery/10K/k=32          807 us          802 us          857 PeakRSS_MB=42.1367
KNNQuery/10K/k=64         1732 us         1728 us          402 PeakRSS_MB=42.6992
KNNQuery/10K/k=128        5095 us         5062 us          139 PeakRSS_MB=42.6992
KNNQuery/100K/k=1          479 us          375 us         1945 PeakRSS_MB=44.7344
KNNQuery/100K/k=2          508 us          405 us         1794 PeakRSS_MB=50.0469
KNNQuery/100K/k=4          538 us          458 us         1541 PeakRSS_MB=112.492
KNNQuery/100K/k=8          593 us          535 us         1226 PeakRSS_MB=81.8242
KNNQuery/100K/k=16         737 us          717 us          974 PeakRSS_MB=141.09
KNNQuery/100K/k=32        1133 us         1131 us          637 PeakRSS_MB=93.9492
KNNQuery/100K/k=64        2147 us         2146 us          321 PeakRSS_MB=100.461
KNNQuery/100K/k=128       5708 us         5636 us          121 PeakRSS_MB=106.797
KNNQuery/1M/k=1            282 us          252 us         2075 PeakRSS_MB=390.066
KNNQuery/1M/k=2            288 us          268 us         2508 PeakRSS_MB=501.945
KNNQuery/1M/k=4            348 us          310 us         2172 PeakRSS_MB=616
KNNQuery/1M/k=8            446 us          383 us         1835 PeakRSS_MB=731.805
KNNQuery/1M/k=16           567 us          565 us         1193 PeakRSS_MB=824.363
KNNQuery/1M/k=32           930 us          929 us          771 PeakRSS_MB=913.633
KNNQuery/1M/k=64          1961 us         1960 us          343 PeakRSS_MB=1.00473k
KNNQuery/1M/k=128         6602 us         5786 us          119 PeakRSS_MB=1.07319k
KNNQuery/10M/k=1           317 us          304 us         2119 PeakRSS_MB=3.94123k
KNNQuery/10M/k=2           387 us          368 us         1847 PeakRSS_MB=5.08434k
KNNQuery/10M/k=4           809 us          433 us         1630 PeakRSS_MB=6.2547k
KNNQuery/10M/k=8           524 us          517 us         1301 PeakRSS_MB=7.33498k
KNNQuery/10M/k=16          698 us          698 us         1041 PeakRSS_MB=8.06085k
KNNQuery/10M/k=32         1363 us         1362 us          623 PeakRSS_MB=9.16737k
KNNQuery/10M/k=64         2575 us         2103 us          339 PeakRSS_MB=10.0827k
KNNQuery/10M/k=128        6006 us         5983 us          120 PeakRSS_MB=10.7688k
```

## Configuration: SFC=PEANO_HILBERT, Decomp=HISTOGRAM, Coords=FLOAT
```
--- SFC Encoding Scaling ---
2026-06-26T12:18:00+02:00
Running ./sfc_encoding_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 5.92, 2.94, 3.05
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------
Benchmark                Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------
SFC-Encode/1K        0.174 ms        0.163 ms         4261 PeakRSS_MB=15.082 items_per_second=6.12504M/s
SFC-Encode/10K       0.257 ms        0.193 ms         4569 PeakRSS_MB=15.6445 items_per_second=51.9082M/s
SFC-Encode/100K      0.676 ms        0.215 ms         3187 PeakRSS_MB=17.5234 items_per_second=465.28M/s
SFC-Encode/1M         1.02 ms        0.243 ms         2906 PeakRSS_MB=56.3828 items_per_second=4.1171G/s
SFC-Encode/10M        8.89 ms        0.287 ms         1000 PeakRSS_MB=501.641 items_per_second=34.8838G/s
--- GPU/Radix Sort Scaling ---
2026-06-26T12:18:34+02:00
Running ./gpu_sort_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 19.17, 6.41, 4.20
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
GPU_Sort/1K         1.83 ms         1.81 ms          532 PeakRSS_MB=30.8984 items_per_second=553.767k/s
GPU_Sort/10K        10.1 ms         10.1 ms           72 PeakRSS_MB=68.0234 items_per_second=992.997k/s
GPU_Sort/100K        156 ms          156 ms            4 PeakRSS_MB=132.844 items_per_second=642.502k/s
GPU_Sort/1M         1648 ms         1640 ms            1 PeakRSS_MB=680.719 items_per_second=609.808k/s
GPU_Sort/10M       10393 ms        10314 ms            1 PeakRSS_MB=18.8321k items_per_second=969.588k/s
--- Tree Build Scaling ---
2026-06-26T12:19:12+02:00
Running ./tree_build_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 11.17, 5.82, 4.09
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------------
Benchmark               Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------
TreeBuild/1K         3.26 ms         2.75 ms          288 PeakRSS_MB=27.4609 items_per_second=363.535k/s
TreeBuild/10K        18.8 ms         16.7 ms           46 PeakRSS_MB=53.3242 items_per_second=597.647k/s
TreeBuild/100K        234 ms          227 ms            3 PeakRSS_MB=102.773 items_per_second=440.378k/s
TreeBuild/1M         1846 ms         1797 ms            1 PeakRSS_MB=686.301 items_per_second=556.396k/s
TreeBuild/10M       17803 ms        17531 ms            1 PeakRSS_MB=1.60843k items_per_second=570.421k/s
--- Rebuild Scaling ---
2026-06-26T12:19:59+02:00
Running ./rebuild_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 10.70, 6.25, 4.31
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------
TreeRebuild/1K         4.97 ms         4.12 ms          192 PeakRSS_MB=23.5273 items_per_second=242.637k/s
TreeRebuild/10K        19.8 ms         17.8 ms           39 PeakRSS_MB=49.8398 items_per_second=562.024k/s
TreeRebuild/100K        218 ms          206 ms            3 PeakRSS_MB=266.199 items_per_second=485.992k/s
TreeRebuild/1M         1997 ms         1968 ms            1 PeakRSS_MB=738.91 items_per_second=508.139k/s
TreeRebuild/10M       15612 ms        15390 ms            1 PeakRSS_MB=5.52435k items_per_second=649.77k/s
--- Range Query Scaling ---
2026-06-26T12:20:43+02:00
Running ./range_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 11.32, 6.71, 4.54
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
RangeQuery/1K/R=0.010000            243 us          234 us         2742 PeakRSS_MB=15.4766 TotalFound=1k
RangeQuery/1K/R=0.100000            291 us          280 us         2240 PeakRSS_MB=15.4766 TotalFound=1.005k
RangeQuery/1K/R=1.000000            208 us          191 us         3702 PeakRSS_MB=15.4766 TotalFound=1.035k
RangeQuery/1K/R=10.000000           275 us          216 us         3230 PeakRSS_MB=16.0391 TotalFound=6.591k
RangeQuery/1K/R=100.000000          736 us          735 us          924 PeakRSS_MB=16.0391 TotalFound=459.76k
RangeQuery/1K/R=200.000000          834 us          765 us          910 PeakRSS_MB=16.6016 TotalFound=931.404k
RangeQuery/10K/R=0.010000           338 us          283 us         3407 PeakRSS_MB=33.457 TotalFound=1k
RangeQuery/10K/R=0.100000           471 us          384 us         2377 PeakRSS_MB=34.0195 TotalFound=1.001k
RangeQuery/10K/R=1.000000           463 us          384 us         1823 PeakRSS_MB=34.582 TotalFound=1.295k
RangeQuery/10K/R=10.000000          424 us          342 us         1694 PeakRSS_MB=35.1445 TotalFound=53.444k
RangeQuery/10K/R=100.000000        5611 us         5576 us          126 PeakRSS_MB=36.2695 TotalFound=4.38585M
RangeQuery/10K/R=200.000000        9373 us         8572 us           81 PeakRSS_MB=36.2695 TotalFound=9.12136M
RangeQuery/100K/R=0.010000          457 us          352 us         1945 PeakRSS_MB=255.801 TotalFound=1k
RangeQuery/100K/R=0.100000          484 us          391 us         2013 PeakRSS_MB=262.586 TotalFound=1.005k
RangeQuery/100K/R=1.000000          496 us          391 us         1669 PeakRSS_MB=271.422 TotalFound=4.145k
RangeQuery/100K/R=10.000000        1225 us         1223 us          553 PeakRSS_MB=283 TotalFound=506.846k
RangeQuery/100K/R=100.000000      48150 us        48133 us           15 PeakRSS_MB=298.75 TotalFound=43.2318M
RangeQuery/100K/R=200.000000      94626 us        94594 us            7 PeakRSS_MB=301.562 TotalFound=90.3647M
RangeQuery/1M/R=0.010000            416 us          295 us         2952 PeakRSS_MB=399.691 TotalFound=1k
RangeQuery/1M/R=0.100000            365 us          263 us         3170 PeakRSS_MB=516.418 TotalFound=1.139k
RangeQuery/1M/R=1.000000            554 us          392 us         2061 PeakRSS_MB=629.727 TotalFound=39.044k
RangeQuery/1M/R=10.000000          6965 us         6714 us           84 PeakRSS_MB=629.938 TotalFound=5.78161M
RangeQuery/1M/R=100.000000       416773 us       416003 us            2 PeakRSS_MB=674.527 TotalFound=433.516M
RangeQuery/1M/R=200.000000       922264 us       921989 us            1 PeakRSS_MB=697.566 TotalFound=907.777M
RangeQuery/10M/R=0.010000           392 us          275 us         3601 PeakRSS_MB=3.02136k TotalFound=1.001k
RangeQuery/10M/R=0.100000           279 us          253 us         2626 PeakRSS_MB=4.35659k TotalFound=1.793k
RangeQuery/10M/R=1.000000          1929 us         1923 us          367 PeakRSS_MB=22.9717k TotalFound=363.096k
RangeQuery/10M/R=10.000000       121435 us       121380 us            6 PeakRSS_MB=26.4692k TotalFound=55.2472M
RangeQuery/10M/R=100.000000     5633000 us      5628497 us            1 PeakRSS_MB=23.9257k TotalFound=4.39221G
RangeQuery/10M/R=200.000000     9884332 us      9879529 us            1 PeakRSS_MB=23.9252k TotalFound=9.09205G
--- kNN Query Scaling ---
2026-06-26T12:28:47+02:00
Running ./knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.48, 3.33, 3.84
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
KNNQuery/1K/k=1            164 us          163 us         4036 PeakRSS_MB=16.8242
KNNQuery/1K/k=2            182 us          181 us         3856 PeakRSS_MB=16.8242
KNNQuery/1K/k=4            201 us          199 us         3436 PeakRSS_MB=16.8242
KNNQuery/1K/k=8            263 us          249 us         3000 PeakRSS_MB=16.8242
KNNQuery/1K/k=16           400 us          367 us         1907 PeakRSS_MB=16.8242
KNNQuery/1K/k=32           676 us          673 us         1045 PeakRSS_MB=17.3867
KNNQuery/1K/k=64          1687 us         1686 us          428 PeakRSS_MB=17.3867
KNNQuery/1K/k=128         4775 us         4774 us          147 PeakRSS_MB=17.3867
KNNQuery/10K/k=1           480 us          356 us         1923 PeakRSS_MB=18.5195
KNNQuery/10K/k=2           436 us          331 us         1823 PeakRSS_MB=19.582
KNNQuery/10K/k=4           496 us          388 us         1645 PeakRSS_MB=20.9492
KNNQuery/10K/k=8           548 us          446 us         1568 PeakRSS_MB=22.0664
KNNQuery/10K/k=16          599 us          517 us         1235 PeakRSS_MB=23.5703
KNNQuery/10K/k=32          811 us          797 us          826 PeakRSS_MB=24.6523
KNNQuery/10K/k=64         1672 us         1671 us          416 PeakRSS_MB=25.4883
KNNQuery/10K/k=128        5020 us         5014 us          139 PeakRSS_MB=24.543
KNNQuery/100K/k=1          496 us          385 us         2101 PeakRSS_MB=35.6367
KNNQuery/100K/k=2          481 us          382 us         1660 PeakRSS_MB=46.293
KNNQuery/100K/k=4          548 us          450 us         1530 PeakRSS_MB=56.7031
KNNQuery/100K/k=8          595 us          506 us         1283 PeakRSS_MB=61.3789
KNNQuery/100K/k=16         684 us          647 us         1142 PeakRSS_MB=70.8047
KNNQuery/100K/k=32         965 us          960 us          735 PeakRSS_MB=78.168
KNNQuery/100K/k=64        2004 us         2002 us          351 PeakRSS_MB=88.5195
KNNQuery/100K/k=128       5502 us         5475 us          128 PeakRSS_MB=99.4102
KNNQuery/1M/k=1            270 us          245 us         2887 PeakRSS_MB=324.117
KNNQuery/1M/k=2            331 us          273 us         2638 PeakRSS_MB=438.602
KNNQuery/1M/k=4            368 us          321 us         1964 PeakRSS_MB=552.75
KNNQuery/1M/k=8            408 us          376 us         1832 PeakRSS_MB=665.062
KNNQuery/1M/k=16           565 us          500 us         1295 PeakRSS_MB=1.33777k
KNNQuery/1M/k=32           910 us          905 us          769 PeakRSS_MB=1.43148k
KNNQuery/1M/k=64          2475 us         2165 us          368 PeakRSS_MB=1.52568k
KNNQuery/1M/k=128         7248 us         5717 us          121 PeakRSS_MB=1.59446k
KNNQuery/10M/k=1           359 us          319 us         2182 PeakRSS_MB=3.58061k
KNNQuery/10M/k=2           936 us          422 us         1351 PeakRSS_MB=4.49686k
KNNQuery/10M/k=4           430 us          411 us         1559 PeakRSS_MB=5.45088k
KNNQuery/10M/k=8           532 us          509 us         1431 PeakRSS_MB=7.46067k
KNNQuery/10M/k=16          624 us          618 us          981 PeakRSS_MB=8.07003k
KNNQuery/10M/k=32          972 us          970 us          710 PeakRSS_MB=8.53894k
KNNQuery/10M/k=64         1994 us         1992 us          341 PeakRSS_MB=9.53149k
KNNQuery/10M/k=128        6649 us         5575 us          124 PeakRSS_MB=10.2191k
```

## Configuration: SFC=PEANO_HILBERT, Decomp=HISTOGRAM, Coords=DOUBLE
```
--- SFC Encoding Scaling ---
2026-06-26T12:43:24+02:00
Running ./sfc_encoding_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 5.64, 2.81, 3.05
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------
Benchmark                Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------
SFC-Encode/1K        0.172 ms        0.162 ms         3201 PeakRSS_MB=15.0859 items_per_second=6.15489M/s
SFC-Encode/10K       0.388 ms        0.217 ms         4529 PeakRSS_MB=15.6484 items_per_second=46.1542M/s
SFC-Encode/100K      0.935 ms        0.201 ms         3236 PeakRSS_MB=16.6055 items_per_second=497.261M/s
SFC-Encode/1M         1.08 ms        0.231 ms         3308 PeakRSS_MB=72.7422 items_per_second=4.328G/s
SFC-Encode/10M        9.48 ms        0.283 ms         1000 PeakRSS_MB=617.426 items_per_second=35.3076G/s
--- GPU/Radix Sort Scaling ---
2026-06-26T12:44:01+02:00
Running ./gpu_sort_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 16.03, 5.46, 3.91
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
GPU_Sort/1K         1.74 ms         1.72 ms          501 PeakRSS_MB=30.9062 items_per_second=582.618k/s
GPU_Sort/10K        9.94 ms         9.82 ms           72 PeakRSS_MB=68.0312 items_per_second=1.01833M/s
GPU_Sort/100K        162 ms          161 ms            4 PeakRSS_MB=281.633 items_per_second=619.932k/s
GPU_Sort/1M         1589 ms         1583 ms            1 PeakRSS_MB=934.887 items_per_second=631.533k/s
GPU_Sort/10M       14484 ms        14373 ms            1 PeakRSS_MB=10.1337k items_per_second=695.74k/s
--- Tree Build Scaling ---
2026-06-26T12:44:43+02:00
Running ./tree_build_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 8.71, 4.90, 3.78
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------------
Benchmark               Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------
TreeBuild/1K         5.54 ms         4.42 ms          185 PeakRSS_MB=23.5273 items_per_second=226.429k/s
TreeBuild/10K        20.4 ms         18.3 ms           39 PeakRSS_MB=50.8516 items_per_second=547.91k/s
TreeBuild/100K        209 ms          200 ms            6 PeakRSS_MB=266.395 items_per_second=499.73k/s
TreeBuild/1M         1800 ms         1772 ms            1 PeakRSS_MB=757.871 items_per_second=564.352k/s
TreeBuild/10M       17050 ms        16727 ms            1 PeakRSS_MB=1.5745k items_per_second=597.847k/s
--- Rebuild Scaling ---
2026-06-26T12:45:29+02:00
Running ./rebuild_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 4.78, 4.40, 3.67
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------
TreeRebuild/1K         3.24 ms         3.02 ms          241 PeakRSS_MB=26.3398 items_per_second=330.672k/s
TreeRebuild/10K        16.9 ms         15.2 ms           41 PeakRSS_MB=53.4141 items_per_second=657.023k/s
TreeRebuild/100K        241 ms          236 ms            3 PeakRSS_MB=268.25 items_per_second=424.498k/s
TreeRebuild/1M         2062 ms         2030 ms            1 PeakRSS_MB=2.32313k items_per_second=492.644k/s
TreeRebuild/10M       17646 ms        17435 ms            1 PeakRSS_MB=2.76207k items_per_second=573.572k/s
--- Range Query Scaling ---
2026-06-26T12:46:15+02:00
Running ./range_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 2.88, 3.92, 3.54
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
RangeQuery/1K/R=0.010000            192 us          175 us         3766 PeakRSS_MB=17.9062 TotalFound=1k
RangeQuery/1K/R=0.100000            188 us          176 us         3978 PeakRSS_MB=17.9062 TotalFound=1.005k
RangeQuery/1K/R=1.000000            238 us          227 us         3807 PeakRSS_MB=17.9062 TotalFound=1.035k
RangeQuery/1K/R=10.000000           224 us          200 us         3418 PeakRSS_MB=17.9062 TotalFound=6.591k
RangeQuery/1K/R=100.000000          735 us          734 us          935 PeakRSS_MB=17.9062 TotalFound=459.76k
RangeQuery/1K/R=200.000000          826 us          774 us          900 PeakRSS_MB=18.4688 TotalFound=931.404k
RangeQuery/10K/R=0.010000           227 us          198 us         3755 PeakRSS_MB=38.1562 TotalFound=1k
RangeQuery/10K/R=0.100000           194 us          178 us         3606 PeakRSS_MB=38.7188 TotalFound=1.001k
RangeQuery/10K/R=1.000000           213 us          187 us         3735 PeakRSS_MB=38.7188 TotalFound=1.295k
RangeQuery/10K/R=10.000000          428 us          340 us         2091 PeakRSS_MB=39.2812 TotalFound=53.444k
RangeQuery/10K/R=100.000000        5564 us         5540 us          128 PeakRSS_MB=39.2812 TotalFound=4.38585M
RangeQuery/10K/R=200.000000        8691 us         8647 us           82 PeakRSS_MB=39.2812 TotalFound=9.12136M
RangeQuery/100K/R=0.010000          425 us          326 us         2087 PeakRSS_MB=264.242 TotalFound=1k
RangeQuery/100K/R=0.100000          462 us          353 us         2082 PeakRSS_MB=271.195 TotalFound=1.005k
RangeQuery/100K/R=1.000000          458 us          349 us         1702 PeakRSS_MB=278.871 TotalFound=4.145k
RangeQuery/100K/R=10.000000        1297 us         1295 us          531 PeakRSS_MB=289.559 TotalFound=506.846k
RangeQuery/100K/R=100.000000      49840 us        49820 us           14 PeakRSS_MB=297.434 TotalFound=43.2318M
RangeQuery/100K/R=200.000000      97811 us        97782 us            7 PeakRSS_MB=306.836 TotalFound=90.3647M
RangeQuery/1M/R=0.010000            323 us          227 us         2548 PeakRSS_MB=402.414 TotalFound=1k
RangeQuery/1M/R=0.100000            283 us          224 us         3330 PeakRSS_MB=521.039 TotalFound=1.139k
RangeQuery/1M/R=1.000000            552 us          362 us         2194 PeakRSS_MB=637.012 TotalFound=39.044k
RangeQuery/1M/R=10.000000          8445 us         7287 us          102 PeakRSS_MB=704.668 TotalFound=5.78161M
RangeQuery/1M/R=100.000000       542043 us       541739 us            1 PeakRSS_MB=728.316 TotalFound=433.516M
RangeQuery/1M/R=200.000000      1051332 us      1050944 us            1 PeakRSS_MB=749.23 TotalFound=907.777M
RangeQuery/10M/R=0.010000           297 us          210 us         3479 PeakRSS_MB=3.57045k TotalFound=1.001k
RangeQuery/10M/R=0.100000           267 us          254 us         2045 PeakRSS_MB=9.29168k TotalFound=1.793k
RangeQuery/10M/R=1.000000          2050 us         2049 us          354 PeakRSS_MB=5.81791k TotalFound=363.096k
RangeQuery/10M/R=10.000000       136256 us       136196 us            5 PeakRSS_MB=6.2761k TotalFound=55.2471M
RangeQuery/10M/R=100.000000     6664781 us      6660508 us            1 PeakRSS_MB=11.0462k TotalFound=4.39221G
RangeQuery/10M/R=200.000000    10976043 us     10971109 us            1 PeakRSS_MB=11.9112k TotalFound=9.09205G
--- kNN Query Scaling ---
2026-06-26T12:54:52+02:00
Running ./knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 7.88, 4.09, 3.47
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
KNNQuery/1K/k=1            166 us          166 us         4215 PeakRSS_MB=16.8281
KNNQuery/1K/k=2            182 us          180 us         3968 PeakRSS_MB=16.8281
KNNQuery/1K/k=4            217 us          209 us         3514 PeakRSS_MB=16.8281
KNNQuery/1K/k=8            270 us          256 us         2906 PeakRSS_MB=16.8281
KNNQuery/1K/k=16           383 us          349 us         2005 PeakRSS_MB=16.8281
KNNQuery/1K/k=32           679 us          678 us         1023 PeakRSS_MB=17.3906
KNNQuery/1K/k=64          1643 us         1639 us          427 PeakRSS_MB=17.3906
KNNQuery/1K/k=128         4685 us         4673 us          150 PeakRSS_MB=16.6289
KNNQuery/10K/k=1           264 us          244 us         2421 PeakRSS_MB=18.7969
KNNQuery/10K/k=2           212 us          206 us         3406 PeakRSS_MB=19.5547
KNNQuery/10K/k=4           242 us          229 us         2913 PeakRSS_MB=21.2383
KNNQuery/10K/k=8           393 us          320 us         2356 PeakRSS_MB=22.5938
KNNQuery/10K/k=16          586 us          525 us         1000 PeakRSS_MB=23.8789
KNNQuery/10K/k=32          836 us          817 us          855 PeakRSS_MB=24.5234
KNNQuery/10K/k=64         1715 us         1712 us          410 PeakRSS_MB=25.7109
KNNQuery/10K/k=128        5495 us         5045 us          138 PeakRSS_MB=24.6523
KNNQuery/100K/k=1          524 us          408 us         1983 PeakRSS_MB=28.8984
KNNQuery/100K/k=2          492 us          390 us         1687 PeakRSS_MB=55.4531
KNNQuery/100K/k=4          567 us          479 us         1521 PeakRSS_MB=62.0195
KNNQuery/100K/k=8          610 us          548 us         1321 PeakRSS_MB=63.1875
KNNQuery/100K/k=16         709 us          694 us          954 PeakRSS_MB=82.0938
KNNQuery/100K/k=32        1081 us         1078 us          640 PeakRSS_MB=89.0938
KNNQuery/100K/k=64        2255 us         2239 us          322 PeakRSS_MB=91.6094
KNNQuery/100K/k=128       5751 us         5713 us          123 PeakRSS_MB=108.09
KNNQuery/1M/k=1            248 us          235 us         2739 PeakRSS_MB=392.629
KNNQuery/1M/k=2            275 us          267 us         2608 PeakRSS_MB=505.016
KNNQuery/1M/k=4            347 us          330 us         2077 PeakRSS_MB=620.875
KNNQuery/1M/k=8            575 us          565 us         1590 PeakRSS_MB=737.051
KNNQuery/1M/k=16           569 us          548 us         1309 PeakRSS_MB=826.363
KNNQuery/1M/k=32           917 us          916 us          767 PeakRSS_MB=917.379
KNNQuery/1M/k=64          1981 us         1980 us          360 PeakRSS_MB=1.00988k
KNNQuery/1M/k=128         6949 us         6026 us          124 PeakRSS_MB=1.07848k
KNNQuery/10M/k=1           318 us          280 us         2678 PeakRSS_MB=3.94097k
KNNQuery/10M/k=2           384 us          370 us         1848 PeakRSS_MB=5.12235k
KNNQuery/10M/k=4           818 us          431 us         1625 PeakRSS_MB=6.25473k
KNNQuery/10M/k=8           536 us          518 us         1191 PeakRSS_MB=7.33477k
KNNQuery/10M/k=16          911 us          743 us         1071 PeakRSS_MB=8.25013k
KNNQuery/10M/k=32         1030 us         1029 us          658 PeakRSS_MB=9.03844k
KNNQuery/10M/k=64         3871 us         2099 us          331 PeakRSS_MB=10.0821k
KNNQuery/10M/k=128        7197 us         6121 us          110 PeakRSS_MB=10.7679k
```

## Configuration: SFC=PEANO_HILBERT, Decomp=SAMPLING, Coords=FLOAT
```
--- SFC Encoding Scaling ---
2026-06-26T13:10:33+02:00
Running ./sfc_encoding_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 4.60, 2.58, 2.54
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------
Benchmark                Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------
SFC-Encode/1K        0.295 ms        0.248 ms         4171 PeakRSS_MB=15.0859 items_per_second=4.04M/s
SFC-Encode/10K       0.279 ms        0.199 ms         3484 PeakRSS_MB=15.0859 items_per_second=50.1732M/s
SFC-Encode/100K      0.705 ms        0.213 ms         3259 PeakRSS_MB=18.1523 items_per_second=468.931M/s
SFC-Encode/1M         1.04 ms        0.239 ms         2900 PeakRSS_MB=76.832 items_per_second=4.17748G/s
SFC-Encode/10M        9.80 ms        0.277 ms         1000 PeakRSS_MB=502.715 items_per_second=36.1313G/s
--- GPU/Radix Sort Scaling ---
2026-06-26T13:11:10+02:00
Running ./gpu_sort_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 12.40, 4.70, 3.24
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
GPU_Sort/1K         1.72 ms         1.69 ms          506 PeakRSS_MB=30.2852 items_per_second=590.215k/s
GPU_Sort/10K        9.79 ms         9.78 ms           73 PeakRSS_MB=68.5352 items_per_second=1.02253M/s
GPU_Sort/100K        144 ms          143 ms            4 PeakRSS_MB=160.41 items_per_second=697.917k/s
GPU_Sort/1M          976 ms          971 ms            1 PeakRSS_MB=2.00997k items_per_second=1.02968M/s
GPU_Sort/10M       16168 ms        15956 ms            1 PeakRSS_MB=9.25648k items_per_second=626.742k/s
--- Tree Build Scaling ---
2026-06-26T13:11:51+02:00
Running ./tree_build_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 6.38, 4.18, 3.14
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------------
Benchmark               Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------
TreeBuild/1K         3.41 ms         3.11 ms          240 PeakRSS_MB=24.6523 items_per_second=321.651k/s
TreeBuild/10K        12.0 ms         11.3 ms           48 PeakRSS_MB=58.8359 items_per_second=884.698k/s
TreeBuild/100K        232 ms          223 ms            4 PeakRSS_MB=277.922 items_per_second=447.655k/s
TreeBuild/1M         1772 ms         1744 ms            1 PeakRSS_MB=711.051 items_per_second=573.443k/s
TreeBuild/10M       17227 ms        17011 ms            1 PeakRSS_MB=1.61011k items_per_second=587.86k/s
--- Rebuild Scaling ---
2026-06-26T13:12:37+02:00
Running ./rebuild_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 3.60, 3.75, 3.04
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------
TreeRebuild/1K         4.40 ms         3.71 ms          209 PeakRSS_MB=23.5273 items_per_second=269.533k/s
TreeRebuild/10K        18.0 ms         15.9 ms           45 PeakRSS_MB=51.4492 items_per_second=629.466k/s
TreeRebuild/100K        187 ms          176 ms            6 PeakRSS_MB=270.84 items_per_second=567.519k/s
TreeRebuild/1M         2112 ms         2085 ms            1 PeakRSS_MB=324.715 items_per_second=479.723k/s
TreeRebuild/10M       17092 ms        16798 ms            1 PeakRSS_MB=5.86602k items_per_second=595.307k/s
--- Range Query Scaling ---
2026-06-26T13:13:24+02:00
Running ./range_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 2.30, 3.38, 2.94
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
RangeQuery/1K/R=0.010000            273 us          239 us         3813 PeakRSS_MB=17.6602 TotalFound=1k
RangeQuery/1K/R=0.100000            218 us          191 us         2958 PeakRSS_MB=17.6602 TotalFound=1.005k
RangeQuery/1K/R=1.000000            200 us          189 us         3506 PeakRSS_MB=17.6602 TotalFound=1.035k
RangeQuery/1K/R=10.000000           201 us          191 us         3593 PeakRSS_MB=17.6602 TotalFound=6.591k
RangeQuery/1K/R=100.000000          729 us          726 us          923 PeakRSS_MB=17.6602 TotalFound=459.76k
RangeQuery/1K/R=200.000000          834 us          773 us          906 PeakRSS_MB=18.2227 TotalFound=931.404k
RangeQuery/10K/R=0.010000           178 us          174 us         4084 PeakRSS_MB=12.5859 TotalFound=1k
RangeQuery/10K/R=0.100000           195 us          187 us         4016 PeakRSS_MB=10.2891 TotalFound=1.001k
RangeQuery/10K/R=1.000000           177 us          175 us         3831 PeakRSS_MB=26.5469 TotalFound=1.295k
RangeQuery/10K/R=10.000000          296 us          291 us         2376 PeakRSS_MB=28.7969 TotalFound=53.444k
RangeQuery/10K/R=100.000000        5570 us         5502 us          128 PeakRSS_MB=29.9219 TotalFound=4.38585M
RangeQuery/10K/R=200.000000        8569 us         8499 us           82 PeakRSS_MB=30.4844 TotalFound=9.12136M
RangeQuery/100K/R=0.010000          188 us          181 us         3570 PeakRSS_MB=252.512 TotalFound=1k
RangeQuery/100K/R=0.100000          187 us          179 us         3359 PeakRSS_MB=264.219 TotalFound=1.005k
RangeQuery/100K/R=1.000000          418 us          328 us         2382 PeakRSS_MB=277.004 TotalFound=4.145k
RangeQuery/100K/R=10.000000        1244 us         1238 us          542 PeakRSS_MB=290.02 TotalFound=506.846k
RangeQuery/100K/R=100.000000      48200 us        48179 us           15 PeakRSS_MB=295.645 TotalFound=43.2318M
RangeQuery/100K/R=200.000000      93845 us        93810 us            7 PeakRSS_MB=299.121 TotalFound=90.3647M
RangeQuery/1M/R=0.010000            292 us          207 us         2586 PeakRSS_MB=424.676 TotalFound=1k
RangeQuery/1M/R=0.100000            281 us          193 us         3641 PeakRSS_MB=976.484 TotalFound=1.139k
RangeQuery/1M/R=1.000000            430 us          289 us         2233 PeakRSS_MB=653.816 TotalFound=39.044k
RangeQuery/1M/R=10.000000          6389 us         6338 us          112 PeakRSS_MB=1.15835k TotalFound=5.78161M
RangeQuery/1M/R=100.000000       421363 us       421210 us            2 PeakRSS_MB=768.863 TotalFound=433.516M
RangeQuery/1M/R=200.000000       927319 us       927053 us            1 PeakRSS_MB=792.031 TotalFound=907.777M
RangeQuery/10M/R=0.010000           239 us          234 us         3498 PeakRSS_MB=3.02216k TotalFound=1.001k
RangeQuery/10M/R=0.100000           323 us          258 us         2183 PeakRSS_MB=4.35743k TotalFound=1.793k
RangeQuery/10M/R=1.000000          1886 us         1883 us          374 PeakRSS_MB=5.27272k TotalFound=363.096k
RangeQuery/10M/R=10.000000       116432 us       116398 us            6 PeakRSS_MB=5.73009k TotalFound=55.2472M
RangeQuery/10M/R=100.000000     5438486 us      5436601 us            1 PeakRSS_MB=10.4225k TotalFound=4.39221G
RangeQuery/10M/R=200.000000     9816614 us      9812233 us            1 PeakRSS_MB=6.29173k TotalFound=9.09205G
--- kNN Query Scaling ---
2026-06-26T13:21:33+02:00
Running ./knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 5.66, 3.96, 3.30
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
KNNQuery/1K/k=1            169 us          168 us         3956 PeakRSS_MB=16.8242
KNNQuery/1K/k=2            190 us          189 us         3869 PeakRSS_MB=17.3867
KNNQuery/1K/k=4            206 us          204 us         3369 PeakRSS_MB=17.3867
KNNQuery/1K/k=8            266 us          250 us         2785 PeakRSS_MB=17.3867
KNNQuery/1K/k=16           398 us          363 us         1910 PeakRSS_MB=17.3867
KNNQuery/1K/k=32           692 us          682 us         1017 PeakRSS_MB=17.3867
KNNQuery/1K/k=64          1713 us         1709 us          429 PeakRSS_MB=17.3867
KNNQuery/1K/k=128         4850 us         4816 us          143 PeakRSS_MB=17.3867
KNNQuery/10K/k=1           395 us          311 us         2091 PeakRSS_MB=19.75
KNNQuery/10K/k=2           443 us          336 us         2027 PeakRSS_MB=20.4922
KNNQuery/10K/k=4           540 us          423 us         1717 PeakRSS_MB=21.6367
KNNQuery/10K/k=8           544 us          445 us         1718 PeakRSS_MB=23.4961
KNNQuery/10K/k=16          635 us          551 us         1301 PeakRSS_MB=23.5898
KNNQuery/10K/k=32          817 us          807 us          872 PeakRSS_MB=46.7031
KNNQuery/10K/k=64         1705 us         1697 us          411 PeakRSS_MB=47.2656
KNNQuery/10K/k=128        5155 us         5092 us          138 PeakRSS_MB=47.8281
KNNQuery/100K/k=1          448 us          349 us         1790 PeakRSS_MB=50.6602
KNNQuery/100K/k=2          503 us          402 us         1688 PeakRSS_MB=55.4492
KNNQuery/100K/k=4          552 us          463 us         1653 PeakRSS_MB=58.3125
KNNQuery/100K/k=8          601 us          523 us         1398 PeakRSS_MB=70.3867
KNNQuery/100K/k=16         668 us          631 us         1047 PeakRSS_MB=94.3867
KNNQuery/100K/k=32         983 us          973 us          723 PeakRSS_MB=97.4219
KNNQuery/100K/k=64        2022 us         2020 us          365 PeakRSS_MB=101.766
KNNQuery/100K/k=128       5650 us         5573 us          126 PeakRSS_MB=105.211
KNNQuery/1M/k=1            294 us          242 us         2660 PeakRSS_MB=332.387
KNNQuery/1M/k=2            323 us          285 us         2474 PeakRSS_MB=446.02
KNNQuery/1M/k=4            395 us          345 us         2112 PeakRSS_MB=561.441
KNNQuery/1M/k=8            462 us          401 us         1815 PeakRSS_MB=674.094
KNNQuery/1M/k=16           607 us          524 us         1355 PeakRSS_MB=764.492
KNNQuery/1M/k=32          1012 us          862 us          657 PeakRSS_MB=858.418
KNNQuery/1M/k=64          1914 us         1912 us          363 PeakRSS_MB=946.215
KNNQuery/1M/k=128         5926 us         5747 us          129 PeakRSS_MB=1.01838k
KNNQuery/10M/k=1           327 us          308 us         2276 PeakRSS_MB=9.96704k
KNNQuery/10M/k=2           726 us          379 us         1482 PeakRSS_MB=10.6547k
KNNQuery/10M/k=4           439 us          414 us         1651 PeakRSS_MB=11.1108k
KNNQuery/10M/k=8           559 us          520 us         1211 PeakRSS_MB=11.7606k
KNNQuery/10M/k=16          645 us          642 us         1020 PeakRSS_MB=12.4851k
KNNQuery/10M/k=32          998 us          996 us          713 PeakRSS_MB=13.4007k
KNNQuery/10M/k=64         2125 us         2064 us          342 PeakRSS_MB=14.316k
KNNQuery/10M/k=128        6816 us         5660 us          124 PeakRSS_MB=15.0027k
```

## Configuration: SFC=PEANO_HILBERT, Decomp=SAMPLING, Coords=DOUBLE
```
--- SFC Encoding Scaling ---
2026-06-26T13:35:29+02:00
Running ./sfc_encoding_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 4.91, 3.33, 3.10
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------
Benchmark                Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------
SFC-Encode/1K        0.328 ms        0.272 ms         2453 PeakRSS_MB=15.0898 items_per_second=3.67488M/s
SFC-Encode/10K       0.559 ms        0.342 ms         1996 PeakRSS_MB=15.6523 items_per_second=29.2123M/s
SFC-Encode/100K       1.00 ms        0.220 ms         3136 PeakRSS_MB=15.4102 items_per_second=454.315M/s
SFC-Encode/1M         1.03 ms        0.242 ms         2689 PeakRSS_MB=68.2188 items_per_second=4.12648G/s
SFC-Encode/10M        9.14 ms        0.286 ms         1000 PeakRSS_MB=613.922 items_per_second=34.9153G/s
--- GPU/Radix Sort Scaling ---
2026-06-26T13:36:05+02:00
Running ./gpu_sort_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 4.95, 3.47, 3.15
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
GPU_Sort/1K         1.72 ms         1.70 ms          529 PeakRSS_MB=31.4648 items_per_second=588.465k/s
GPU_Sort/10K        10.8 ms         10.8 ms           69 PeakRSS_MB=66.9023 items_per_second=930.148k/s
GPU_Sort/100K        160 ms          159 ms            4 PeakRSS_MB=138.531 items_per_second=628.262k/s
GPU_Sort/1M         1480 ms         1477 ms            1 PeakRSS_MB=1.1437k items_per_second=677.205k/s
GPU_Sort/10M       14857 ms        14767 ms            1 PeakRSS_MB=6.39482k items_per_second=677.163k/s
--- Tree Build Scaling ---
2026-06-26T13:36:49+02:00
Running ./tree_build_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 2.94, 3.15, 3.06
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------------
Benchmark               Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------
TreeBuild/1K         4.78 ms         4.04 ms          204 PeakRSS_MB=24.6523 items_per_second=247.289k/s
TreeBuild/10K        18.3 ms         16.5 ms           36 PeakRSS_MB=48.7695 items_per_second=606.703k/s
TreeBuild/100K        237 ms          232 ms            3 PeakRSS_MB=57.8711 items_per_second=431.658k/s
TreeBuild/1M         2194 ms         2159 ms            1 PeakRSS_MB=269.652 items_per_second=463.268k/s
TreeBuild/10M       11719 ms        11406 ms            1 PeakRSS_MB=18.7332k items_per_second=876.714k/s
--- Rebuild Scaling ---
2026-06-26T13:37:30+02:00
Running ./rebuild_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 2.34, 2.98, 3.00
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------
TreeRebuild/1K         3.69 ms         3.06 ms          222 PeakRSS_MB=24.6523 items_per_second=326.762k/s
TreeRebuild/10K        17.6 ms         15.4 ms           40 PeakRSS_MB=50.8672 items_per_second=651.459k/s
TreeRebuild/100K        138 ms          132 ms            4 PeakRSS_MB=264.738 items_per_second=756.662k/s
TreeRebuild/1M         2144 ms         2114 ms            1 PeakRSS_MB=2.36865k items_per_second=473.013k/s
TreeRebuild/10M       18461 ms        18199 ms            1 PeakRSS_MB=2.78187k items_per_second=549.495k/s
--- Range Query Scaling ---
2026-06-26T13:38:19+02:00
Running ./range_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.63, 2.69, 2.90
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
RangeQuery/1K/R=0.010000            270 us          240 us         3921 PeakRSS_MB=18.7109 TotalFound=1k
RangeQuery/1K/R=0.100000            315 us          276 us         2804 PeakRSS_MB=18.7109 TotalFound=1.005k
RangeQuery/1K/R=1.000000            356 us          311 us         2738 PeakRSS_MB=18.7109 TotalFound=1.035k
RangeQuery/1K/R=10.000000           324 us          276 us         2611 PeakRSS_MB=19.2734 TotalFound=6.591k
RangeQuery/1K/R=100.000000          934 us          763 us          926 PeakRSS_MB=19.2734 TotalFound=459.76k
RangeQuery/1K/R=200.000000          886 us          876 us          777 PeakRSS_MB=19.2734 TotalFound=931.404k
RangeQuery/10K/R=0.010000           389 us          310 us         2066 PeakRSS_MB=38.9609 TotalFound=1k
RangeQuery/10K/R=0.100000           371 us          315 us         2263 PeakRSS_MB=39.5234 TotalFound=1.001k
RangeQuery/10K/R=1.000000           343 us          294 us         2323 PeakRSS_MB=39.5234 TotalFound=1.295k
RangeQuery/10K/R=10.000000          672 us          500 us         1380 PeakRSS_MB=39.5234 TotalFound=53.444k
RangeQuery/10K/R=100.000000        6187 us         6137 us          112 PeakRSS_MB=39.5234 TotalFound=4.38585M
RangeQuery/10K/R=200.000000        9418 us         9316 us           76 PeakRSS_MB=39.5234 TotalFound=9.12136M
RangeQuery/100K/R=0.010000          454 us          336 us         2101 PeakRSS_MB=264.953 TotalFound=1k
RangeQuery/100K/R=0.100000          408 us          308 us         2242 PeakRSS_MB=271.09 TotalFound=1.005k
RangeQuery/100K/R=1.000000          498 us          389 us         2049 PeakRSS_MB=282.34 TotalFound=4.145k
RangeQuery/100K/R=10.000000        1377 us         1319 us          526 PeakRSS_MB=289.211 TotalFound=506.846k
RangeQuery/100K/R=100.000000      49195 us        49177 us           14 PeakRSS_MB=298.379 TotalFound=43.2318M
RangeQuery/100K/R=200.000000      99021 us        98988 us            7 PeakRSS_MB=302.676 TotalFound=90.3647M
RangeQuery/1M/R=0.010000            219 us          185 us         3254 PeakRSS_MB=2.50037k TotalFound=1k
RangeQuery/1M/R=0.100000            200 us          183 us         3712 PeakRSS_MB=519.273 TotalFound=1.139k
RangeQuery/1M/R=1.000000            329 us          286 us         2425 PeakRSS_MB=632.051 TotalFound=39.044k
RangeQuery/1M/R=10.000000          6918 us         6889 us           92 PeakRSS_MB=699.832 TotalFound=5.78161M
RangeQuery/1M/R=100.000000       477625 us       477420 us            2 PeakRSS_MB=2.89741k TotalFound=433.516M
RangeQuery/1M/R=200.000000      1020943 us      1020533 us            1 PeakRSS_MB=1.30577k TotalFound=907.777M
RangeQuery/10M/R=0.010000           280 us          225 us         3101 PeakRSS_MB=8.15894k TotalFound=1.001k
RangeQuery/10M/R=0.100000           312 us          277 us         2250 PeakRSS_MB=4.95272k TotalFound=1.793k
RangeQuery/10M/R=1.000000          2025 us         2024 us          350 PeakRSS_MB=5.86703k TotalFound=363.096k
RangeQuery/10M/R=10.000000       136179 us       135883 us            5 PeakRSS_MB=6.42698k TotalFound=55.2471M
RangeQuery/10M/R=100.000000     6934902 us      6932448 us            1 PeakRSS_MB=6.42795k TotalFound=4.39221G
RangeQuery/10M/R=200.000000    10964858 us     10959775 us            1 PeakRSS_MB=6.59263k TotalFound=9.09205G
--- kNN Query Scaling ---
2026-06-26T13:46:58+02:00
Running ./knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 12.33, 4.79, 3.40
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
KNNQuery/1K/k=1            178 us          172 us         4117 PeakRSS_MB=16.8281
KNNQuery/1K/k=2            210 us          209 us         3834 PeakRSS_MB=16.8281
KNNQuery/1K/k=4            230 us          228 us         3088 PeakRSS_MB=16.8281
KNNQuery/1K/k=8            274 us          272 us         2581 PeakRSS_MB=16.8281
KNNQuery/1K/k=16           358 us          355 us         1942 PeakRSS_MB=16.8281
KNNQuery/1K/k=32           683 us          683 us         1011 PeakRSS_MB=16.8281
KNNQuery/1K/k=64          1640 us         1637 us          424 PeakRSS_MB=16.8281
KNNQuery/1K/k=128         4999 us         4668 us          150 PeakRSS_MB=16.8281
KNNQuery/10K/k=1           343 us          294 us         2451 PeakRSS_MB=29.3164
KNNQuery/10K/k=2           292 us          262 us         2773 PeakRSS_MB=21.0078
KNNQuery/10K/k=4           241 us          226 us         3070 PeakRSS_MB=21.9102
KNNQuery/10K/k=8           292 us          271 us         2462 PeakRSS_MB=22.875
KNNQuery/10K/k=16          587 us          512 us         1634 PeakRSS_MB=24.0039
KNNQuery/10K/k=32          832 us          824 us          858 PeakRSS_MB=25.1758
KNNQuery/10K/k=64         1732 us         1729 us          404 PeakRSS_MB=25.6562
KNNQuery/10K/k=128        5608 us         5047 us          139 PeakRSS_MB=23.5664
KNNQuery/100K/k=1          445 us          345 us         1899 PeakRSS_MB=31.8945
KNNQuery/100K/k=2          525 us          414 us         1726 PeakRSS_MB=52.6055
KNNQuery/100K/k=4          549 us          446 us         1280 PeakRSS_MB=57.3945
KNNQuery/100K/k=8          600 us          541 us         1259 PeakRSS_MB=65.6484
KNNQuery/100K/k=16         680 us          667 us          962 PeakRSS_MB=73.707
KNNQuery/100K/k=32        1081 us         1078 us          620 PeakRSS_MB=85.0781
KNNQuery/100K/k=64        2651 us         2195 us          318 PeakRSS_MB=93.9453
KNNQuery/100K/k=128       5748 us         5694 us          122 PeakRSS_MB=103.113
KNNQuery/1M/k=1            229 us          225 us         3229 PeakRSS_MB=379.676
KNNQuery/1M/k=2            297 us          273 us         2418 PeakRSS_MB=494.762
KNNQuery/1M/k=4            320 us          299 us         2360 PeakRSS_MB=607.941
KNNQuery/1M/k=8            400 us          382 us         1726 PeakRSS_MB=721.789
KNNQuery/1M/k=16           559 us          509 us         1398 PeakRSS_MB=813.145
KNNQuery/1M/k=32           887 us          885 us          795 PeakRSS_MB=904.336
KNNQuery/1M/k=64          1895 us         1893 us          310 PeakRSS_MB=997.121
KNNQuery/1M/k=128         6564 us         5907 us          124 PeakRSS_MB=1.06684k
KNNQuery/10M/k=1           300 us          266 us         2710 PeakRSS_MB=3.95813k
KNNQuery/10M/k=2           363 us          345 us         1999 PeakRSS_MB=5.29073k
KNNQuery/10M/k=4           436 us          416 us         1704 PeakRSS_MB=6.4174k
KNNQuery/10M/k=8           752 us          626 us         1330 PeakRSS_MB=7.33342k
KNNQuery/10M/k=16          655 us          645 us         1000 PeakRSS_MB=8.16018k
KNNQuery/10M/k=32         1018 us         1016 us          673 PeakRSS_MB=9.16344k
KNNQuery/10M/k=64         2064 us         2061 us          267 PeakRSS_MB=10.0789k
KNNQuery/10M/k=128        6141 us         6119 us          104 PeakRSS_MB=10.6388k
```

