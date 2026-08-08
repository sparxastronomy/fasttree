# Scaling Benchmark Results: gpu_int64 (Periodic BC: ON)
Date: Sat Aug  8 18:13:25 CEST 2026
Device: GPU
Target GPU: nvidia
SFC Curve: Peano-Hilbert
Periodic BC: ON
Build Directory: build_int64
```
--- Executing ./sfc_encoding_scaling.exe ---
2026-08-08T18:13:25+02:00
Running ./sfc_encoding_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.06, 1.02, 0.85
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
--------------------------------------------------------------------------
Benchmark                Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------
SFC-Encode/1K        0.023 ms        0.023 ms        29284 PeakRSS_MB=245.613 items_per_second=42.5792M/s
SFC-Encode/10K       0.022 ms        0.022 ms        31623 PeakRSS_MB=246.613 items_per_second=451.081M/s
SFC-Encode/100K      0.025 ms        0.025 ms        24421 PeakRSS_MB=250.895 items_per_second=3.95684G/s
SFC-Encode/1M        0.089 ms        0.089 ms         7830 PeakRSS_MB=303.113 items_per_second=11.2182G/s
SFC-Encode/10M       0.712 ms        0.712 ms          885 PeakRSS_MB=770.016 items_per_second=14.0536G/s

--- Executing ./gpu_sort_scaling.exe ---
2026-08-08T18:13:47+02:00
Running ./gpu_sort_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.04, 1.02, 0.86
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
GPU_Sort/1K        0.053 ms        0.053 ms        12491 PeakRSS_MB=250.008 items_per_second=18.9372M/s
GPU_Sort/10K       0.121 ms        0.121 ms         5782 PeakRSS_MB=251.133 items_per_second=82.9067M/s
GPU_Sort/100K      0.360 ms        0.360 ms         1941 PeakRSS_MB=258.445 items_per_second=277.809M/s
GPU_Sort/1M         2.67 ms         2.67 ms          261 PeakRSS_MB=307.066 items_per_second=374.34M/s
GPU_Sort/10M        37.3 ms         37.2 ms           18 PeakRSS_MB=509.461 items_per_second=268.509M/s

--- Executing ./tree_build_scaling.exe ---
2026-08-08T18:14:06+02:00
Running ./tree_build_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.03, 1.02, 0.86
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------------
Benchmark               Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------
TreeBuild/1K         1.76 ms         1.75 ms          393 PeakRSS_MB=247.508 items_per_second=570.509k/s
TreeBuild/10K        3.40 ms         3.39 ms          288 PeakRSS_MB=248.633 items_per_second=2.94616M/s
TreeBuild/100K       5.72 ms         5.72 ms          118 PeakRSS_MB=252.645 items_per_second=17.4965M/s
TreeBuild/1M         31.1 ms         31.0 ms           22 PeakRSS_MB=308.168 items_per_second=32.2166M/s
TreeBuild/10M         280 ms          280 ms            2 PeakRSS_MB=780.078 items_per_second=35.7121M/s

--- Executing ./rebuild_scaling.exe ---
2026-08-08T18:14:21+02:00
Running ./rebuild_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.02, 1.02, 0.87
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------
TreeRebuild/1K         1.76 ms         1.75 ms          397 PeakRSS_MB=248.867 items_per_second=570.097k/s
TreeRebuild/10K        2.41 ms         2.41 ms          288 PeakRSS_MB=250.555 items_per_second=4.14901M/s
TreeRebuild/100K       5.67 ms         5.65 ms          122 PeakRSS_MB=258.449 items_per_second=17.6856M/s
TreeRebuild/1M         29.5 ms         29.4 ms           23 PeakRSS_MB=336.875 items_per_second=33.9813M/s
TreeRebuild/10M         302 ms          302 ms            2 PeakRSS_MB=1.08733k items_per_second=33.1291M/s

--- Executing ./range_query_scaling.exe ---
2026-08-08T18:14:36+02:00
Running ./range_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.02, 1.02, 0.87
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
RangeQuery/1K/R=0.010000           29.0 us         29.0 us        22244 PeakRSS_MB=246.324 TotalFound=0
RangeQuery/1K/R=0.100000           30.5 us         30.5 us        22878 PeakRSS_MB=246.324 TotalFound=0
RangeQuery/1K/R=1.000000           30.8 us         30.8 us        22791 PeakRSS_MB=246.324 TotalFound=0
RangeQuery/1K/R=10.000000          30.4 us         30.4 us        22874 PeakRSS_MB=246.324 TotalFound=0
RangeQuery/1K/R=100.000000         30.5 us         30.5 us        22976 PeakRSS_MB=246.324 TotalFound=0
RangeQuery/1K/R=200.000000         30.4 us         30.4 us        22897 PeakRSS_MB=246.324 TotalFound=0
RangeQuery/10K/R=0.010000          30.6 us         30.6 us        22645 PeakRSS_MB=247.449 TotalFound=0
RangeQuery/10K/R=0.100000          30.8 us         30.8 us        22742 PeakRSS_MB=246.824 TotalFound=0
RangeQuery/10K/R=1.000000          30.6 us         30.6 us        22667 PeakRSS_MB=247.324 TotalFound=0
RangeQuery/10K/R=10.000000         30.7 us         30.7 us        22728 PeakRSS_MB=247.262 TotalFound=0
RangeQuery/10K/R=100.000000        30.9 us         30.9 us        22808 PeakRSS_MB=247.199 TotalFound=0
RangeQuery/10K/R=200.000000        30.9 us         30.8 us        22722 PeakRSS_MB=247.137 TotalFound=0
RangeQuery/100K/R=0.010000         30.8 us         30.8 us        22763 PeakRSS_MB=251.707 TotalFound=0
RangeQuery/100K/R=0.100000         30.7 us         30.6 us        22761 PeakRSS_MB=251.703 TotalFound=0
RangeQuery/100K/R=1.000000         30.8 us         30.8 us        22644 PeakRSS_MB=251.641 TotalFound=0
RangeQuery/100K/R=10.000000        30.7 us         30.7 us        22711 PeakRSS_MB=252.141 TotalFound=0
RangeQuery/100K/R=100.000000       30.9 us         30.9 us        22704 PeakRSS_MB=251.516 TotalFound=0
RangeQuery/100K/R=200.000000       30.6 us         30.6 us        22748 PeakRSS_MB=252.016 TotalFound=0
RangeQuery/1M/R=0.010000           30.7 us         30.7 us        22656 PeakRSS_MB=307.93 TotalFound=0
RangeQuery/1M/R=0.100000           30.6 us         30.6 us        22719 PeakRSS_MB=307.336 TotalFound=0
RangeQuery/1M/R=1.000000           30.7 us         30.7 us        22688 PeakRSS_MB=307.215 TotalFound=0
RangeQuery/1M/R=10.000000          30.7 us         30.7 us        22753 PeakRSS_MB=307.777 TotalFound=0
RangeQuery/1M/R=100.000000         30.7 us         30.7 us        22809 PeakRSS_MB=307.156 TotalFound=0
RangeQuery/1M/R=200.000000         30.7 us         30.7 us        22745 PeakRSS_MB=307.723 TotalFound=0
RangeQuery/10M/R=0.010000          28.2 us         28.2 us        24514 PeakRSS_MB=781.203 TotalFound=0
RangeQuery/10M/R=0.100000          28.3 us         28.3 us        24408 PeakRSS_MB=781.234 TotalFound=0
RangeQuery/10M/R=1.000000          28.3 us         28.3 us        24554 PeakRSS_MB=780.695 TotalFound=0
RangeQuery/10M/R=10.000000         28.2 us         28.2 us        24721 PeakRSS_MB=781.285 TotalFound=0
RangeQuery/10M/R=100.000000        28.3 us         28.2 us        24395 PeakRSS_MB=781.406 TotalFound=0
RangeQuery/10M/R=200.000000        28.4 us         28.4 us        24645 PeakRSS_MB=780.445 TotalFound=0

--- Executing ./knn_query_scaling.exe ---
2026-08-08T18:17:43+02:00
Running ./knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.13, 1.03, 0.91
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
KNNQuery/1K/k=1           75.9 us         75.7 us         8972 PeakRSS_MB=246.82
KNNQuery/1K/k=2            167 us          167 us         4192 PeakRSS_MB=247.383
KNNQuery/1K/k=4            229 us          229 us         3060 PeakRSS_MB=247.383
KNNQuery/1K/k=8            318 us          318 us         2206 PeakRSS_MB=247.383
KNNQuery/1K/k=16           470 us          470 us         1490 PeakRSS_MB=247.383
KNNQuery/1K/k=32           748 us          748 us          933 PeakRSS_MB=247.383
KNNQuery/1K/k=64          2575 us         2574 us          272 PeakRSS_MB=247.383
KNNQuery/1K/k=128         2891 us         2891 us          242 PeakRSS_MB=247.383
KNNQuery/10K/k=1          88.5 us         88.5 us         7891 PeakRSS_MB=248.445
KNNQuery/10K/k=2           109 us          109 us         6382 PeakRSS_MB=248.445
KNNQuery/10K/k=4           140 us          140 us         4988 PeakRSS_MB=248.383
KNNQuery/10K/k=8           198 us          198 us         3521 PeakRSS_MB=248.32
KNNQuery/10K/k=16          305 us          305 us         2292 PeakRSS_MB=247.695
KNNQuery/10K/k=32          484 us          483 us         1450 PeakRSS_MB=248.258
KNNQuery/10K/k=64         5465 us         5464 us          128 PeakRSS_MB=248.195
KNNQuery/10K/k=128        5513 us         5512 us          127 PeakRSS_MB=248.195
KNNQuery/100K/k=1         98.3 us         98.2 us         7093 PeakRSS_MB=252.742
KNNQuery/100K/k=2          146 us          146 us         4777 PeakRSS_MB=252.742
KNNQuery/100K/k=4          186 us          186 us         3762 PeakRSS_MB=252.742
KNNQuery/100K/k=8          225 us          225 us         3110 PeakRSS_MB=252.742
KNNQuery/100K/k=16         306 us          306 us         2291 PeakRSS_MB=252.742
KNNQuery/100K/k=32         525 us          525 us         1327 PeakRSS_MB=252.742
KNNQuery/100K/k=64        3450 us         3450 us          203 PeakRSS_MB=252.742
KNNQuery/100K/k=128       3858 us         3858 us          181 PeakRSS_MB=252.742
KNNQuery/1M/k=1            283 us          283 us         2473 PeakRSS_MB=312.02
KNNQuery/1M/k=2            368 us          368 us         1900 PeakRSS_MB=312.914
KNNQuery/1M/k=4            490 us          490 us         1428 PeakRSS_MB=314.039
KNNQuery/1M/k=8            703 us          703 us          990 PeakRSS_MB=313.477
KNNQuery/1M/k=16          1069 us         1069 us          654 PeakRSS_MB=313.477
KNNQuery/1M/k=32          1680 us         1680 us          416 PeakRSS_MB=314.039
KNNQuery/1M/k=64          5591 us         5590 us          126 PeakRSS_MB=314.039
KNNQuery/1M/k=128         5662 us         5661 us          123 PeakRSS_MB=314.039
KNNQuery/10M/k=1           232 us          232 us         3012 PeakRSS_MB=779.898
KNNQuery/10M/k=2           329 us          329 us         2132 PeakRSS_MB=781.039
KNNQuery/10M/k=4           458 us          458 us         1509 PeakRSS_MB=780.477
KNNQuery/10M/k=8           690 us          689 us          991 PeakRSS_MB=781.039
KNNQuery/10M/k=16         1125 us         1125 us          618 PeakRSS_MB=780.477
KNNQuery/10M/k=32         1788 us         1784 us          394 PeakRSS_MB=780.477
KNNQuery/10M/k=64         5322 us         5321 us          128 PeakRSS_MB=780.477
KNNQuery/10M/k=128        6121 us         6120 us          114 PeakRSS_MB=781.039

--- Executing ./self_knn_query_scaling.exe ---
2026-08-08T18:20:43+02:00
Running ./self_knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.17, 1.10, 0.97
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------------------
Benchmark                        Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------
SelfKNNQuery/1K/k=1            301 us          301 us         2037 PeakRSS_MB=246.742 items_per_second=3.32778M/s
SelfKNNQuery/1K/k=2            369 us          369 us         1884 PeakRSS_MB=246.742 items_per_second=2.7109M/s
SelfKNNQuery/1K/k=4            417 us          417 us         1679 PeakRSS_MB=246.742 items_per_second=2.40025M/s
SelfKNNQuery/1K/k=8            515 us          515 us         1340 PeakRSS_MB=246.742 items_per_second=1.94214M/s
SelfKNNQuery/1K/k=16           668 us          667 us         1038 PeakRSS_MB=246.742 items_per_second=1.49901M/s
SelfKNNQuery/1K/k=32           953 us          951 us          732 PeakRSS_MB=246.742 items_per_second=1.052M/s
SelfKNNQuery/1K/k=64          3135 us         3134 us          223 PeakRSS_MB=246.742 items_per_second=319.104k/s
SelfKNNQuery/1K/k=128         3171 us         3170 us          221 PeakRSS_MB=246.742 items_per_second=315.502k/s
SelfKNNQuery/10K/k=1           336 us          336 us         2069 PeakRSS_MB=247.867 items_per_second=29.7407M/s
SelfKNNQuery/10K/k=2           350 us          350 us         1995 PeakRSS_MB=247.305 items_per_second=28.6051M/s
SelfKNNQuery/10K/k=4           409 us          408 us         1711 PeakRSS_MB=247.305 items_per_second=24.4809M/s
SelfKNNQuery/10K/k=8           462 us          462 us         1517 PeakRSS_MB=247.305 items_per_second=21.6642M/s
SelfKNNQuery/10K/k=16          583 us          583 us         1189 PeakRSS_MB=247.305 items_per_second=17.1635M/s
SelfKNNQuery/10K/k=32          895 us          894 us          775 PeakRSS_MB=247.867 items_per_second=11.1821M/s
SelfKNNQuery/10K/k=64        17022 us        17016 us           41 PeakRSS_MB=247.867 items_per_second=587.666k/s
SelfKNNQuery/10K/k=128       20954 us        20947 us           34 PeakRSS_MB=247.867 items_per_second=477.387k/s
SelfKNNQuery/100K/k=1          941 us          940 us          753 PeakRSS_MB=252.266 items_per_second=106.39M/s
SelfKNNQuery/100K/k=2          971 us          970 us          717 PeakRSS_MB=252.211 items_per_second=103.109M/s
SelfKNNQuery/100K/k=4         1019 us         1018 us          680 PeakRSS_MB=252.238 items_per_second=98.2124M/s
SelfKNNQuery/100K/k=8         1185 us         1185 us          588 PeakRSS_MB=252.828 items_per_second=84.4227M/s
SelfKNNQuery/100K/k=16        1531 us         1530 us          455 PeakRSS_MB=252.801 items_per_second=65.3409M/s
SelfKNNQuery/100K/k=32        2438 us         2437 us          287 PeakRSS_MB=252.266 items_per_second=41.0406M/s
SelfKNNQuery/100K/k=64       58421 us        58408 us           12 PeakRSS_MB=252.828 items_per_second=1.7121M/s
SelfKNNQuery/100K/k=128      89408 us        89381 us            8 PeakRSS_MB=252.773 items_per_second=1.1188M/s
SelfKNNQuery/1M/k=1           8772 us         8767 us           81 PeakRSS_MB=306.672 items_per_second=114.06M/s
SelfKNNQuery/1M/k=2          11431 us        11425 us           58 PeakRSS_MB=307.234 items_per_second=87.5238M/s
SelfKNNQuery/1M/k=4          16628 us        16621 us           42 PeakRSS_MB=307.273 items_per_second=60.1658M/s
SelfKNNQuery/1M/k=8          25570 us        25561 us           27 PeakRSS_MB=307.293 items_per_second=39.1218M/s
SelfKNNQuery/1M/k=16         48340 us        48326 us           14 PeakRSS_MB=307.293 items_per_second=20.6928M/s
SelfKNNQuery/1M/k=32        100961 us       100939 us            7 PeakRSS_MB=307.293 items_per_second=9.90696M/s
SelfKNNQuery/1M/k=64       1369462 us      1369223 us            1 PeakRSS_MB=306.73 items_per_second=730.341k/s
SelfKNNQuery/1M/k=128      1455931 us      1455707 us            1 PeakRSS_MB=307.312 items_per_second=686.951k/s
SelfKNNQuery/10M/k=1        270159 us       270096 us            3 PeakRSS_MB=778.852 items_per_second=37.0239M/s
SelfKNNQuery/10M/k=2        507594 us       507493 us            1 PeakRSS_MB=778.863 items_per_second=19.7047M/s
SelfKNNQuery/10M/k=4        775583 us       775435 us            1 PeakRSS_MB=778.867 items_per_second=12.896M/s
SelfKNNQuery/10M/k=8       1187315 us      1187124 us            1 PeakRSS_MB=778.887 items_per_second=8.42372M/s
SelfKNNQuery/10M/k=16      1891595 us      1890082 us            1 PeakRSS_MB=778.898 items_per_second=5.29078M/s
SelfKNNQuery/10M/k=32      3202365 us      3200017 us            1 PeakRSS_MB=778.906 items_per_second=3.12498M/s
SelfKNNQuery/10M/k=64     11474368 us     11471441 us            1 PeakRSS_MB=782.289 items_per_second=871.73k/s
SelfKNNQuery/10M/k=128    14059182 us     14055592 us            1 PeakRSS_MB=787.363 items_per_second=711.461k/s

```
