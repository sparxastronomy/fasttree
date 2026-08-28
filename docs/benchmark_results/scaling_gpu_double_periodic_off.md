# Scaling Benchmark Results: gpu_double (Periodic BC: OFF)
Date: Sat Aug  8 16:26:25 CEST 2026
Device: GPU
Target GPU: nvidia
SFC Curve: Peano-Hilbert
Periodic BC: OFF
Build Directory: build_gpu
```
--- Executing ./sfc_encoding_scaling.exe ---
2026-08-08T16:26:25+02:00
Running ./sfc_encoding_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.01, 1.39, 0.90
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
***WARNING*** Library was built as DEBUG. Timings may be affected.
--------------------------------------------------------------------------
Benchmark                Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------
SFC-Encode/1K        0.023 ms        0.023 ms        30435 PeakRSS_MB=244.234 items_per_second=43.8133M/s
SFC-Encode/10K       0.022 ms        0.022 ms        30734 PeakRSS_MB=245.234 items_per_second=459.508M/s
SFC-Encode/100K      0.024 ms        0.024 ms        28984 PeakRSS_MB=250.766 items_per_second=4.17012G/s
SFC-Encode/1M        0.057 ms        0.057 ms        11169 PeakRSS_MB=301.426 items_per_second=17.6649G/s
SFC-Encode/10M       0.366 ms        0.365 ms         1948 PeakRSS_MB=770.742 items_per_second=27.3611G/s

--- Executing ./gpu_sort_scaling.exe ---
2026-08-08T16:26:51+02:00
Running ./gpu_sort_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.00, 1.36, 0.90
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
***WARNING*** Library was built as DEBUG. Timings may be affected.
------------------------------------------------------------------------
Benchmark              Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------
GPU_Sort/1K        0.046 ms        0.046 ms        15203 PeakRSS_MB=251.715 items_per_second=21.854M/s
GPU_Sort/10K       0.112 ms        0.112 ms         6237 PeakRSS_MB=260.152 items_per_second=89.3784M/s
GPU_Sort/100K      0.360 ms        0.359 ms         1943 PeakRSS_MB=267.465 items_per_second=278.171M/s
GPU_Sort/1M         2.71 ms         2.71 ms          258 PeakRSS_MB=315.812 items_per_second=369.433M/s
GPU_Sort/10M        37.4 ms         37.4 ms           19 PeakRSS_MB=518.691 items_per_second=267.684M/s

--- Executing ./tree_build_scaling.exe ---
2026-08-08T16:27:09+02:00
Running ./tree_build_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.00, 1.33, 0.91
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
***WARNING*** Library was built as DEBUG. Timings may be affected.
-------------------------------------------------------------------------
Benchmark               Time             CPU   Iterations UserCounters...
-------------------------------------------------------------------------
TreeBuild/1K         1.76 ms         1.76 ms          395 PeakRSS_MB=253.398 items_per_second=568.125k/s
TreeBuild/10K        2.64 ms         2.64 ms          265 PeakRSS_MB=261.836 items_per_second=3.78665M/s
TreeBuild/100K       5.67 ms         5.66 ms          122 PeakRSS_MB=265.648 items_per_second=17.679M/s
TreeBuild/1M         29.6 ms         29.5 ms           23 PeakRSS_MB=323.988 items_per_second=33.8434M/s
TreeBuild/10M         290 ms          290 ms            2 PeakRSS_MB=793.082 items_per_second=34.473M/s

--- Executing ./rebuild_scaling.exe ---
2026-08-08T16:27:23+02:00
Running ./rebuild_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.00, 1.32, 0.91
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
***WARNING*** Library was built as DEBUG. Timings may be affected.
---------------------------------------------------------------------------
Benchmark                 Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------
TreeRebuild/1K         2.24 ms         2.23 ms          256 PeakRSS_MB=249.457 items_per_second=448.316k/s
TreeRebuild/10K        2.62 ms         2.61 ms          274 PeakRSS_MB=258.457 items_per_second=3.82859M/s
TreeRebuild/100K       5.65 ms         5.64 ms          123 PeakRSS_MB=265.113 items_per_second=17.7445M/s
TreeRebuild/1M         30.0 ms         30.0 ms           23 PeakRSS_MB=346.398 items_per_second=33.3663M/s
TreeRebuild/10M         285 ms          285 ms            2 PeakRSS_MB=1.08646k items_per_second=35.0885M/s

--- Executing ./range_query_scaling.exe ---
2026-08-08T16:27:37+02:00
Running ./range_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.00, 1.30, 0.91
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
***WARNING*** Library was built as DEBUG. Timings may be affected.
---------------------------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations UserCounters...
---------------------------------------------------------------------------------------
RangeQuery/1K/R=0.010000           86.0 us         85.9 us         7881 PeakRSS_MB=250.051 TotalFound=1k
RangeQuery/1K/R=0.100000           85.7 us         85.6 us         8159 PeakRSS_MB=250.051 TotalFound=1.005k
RangeQuery/1K/R=1.000000            111 us          111 us         6284 PeakRSS_MB=250.051 TotalFound=1.035k
RangeQuery/1K/R=10.000000           212 us          212 us         3299 PeakRSS_MB=250.051 TotalFound=6.591k
RangeQuery/1K/R=100.000000         1108 us         1107 us          631 PeakRSS_MB=250.051 TotalFound=459.76k
RangeQuery/1K/R=200.000000         1317 us         1317 us          532 PeakRSS_MB=250.051 TotalFound=931.404k
RangeQuery/10K/R=0.010000           110 us          110 us         6318 PeakRSS_MB=257.301 TotalFound=1k
RangeQuery/10K/R=0.100000           131 us          131 us         5355 PeakRSS_MB=257.863 TotalFound=1.001k
RangeQuery/10K/R=1.000000           175 us          175 us         3988 PeakRSS_MB=257.801 TotalFound=1.295k
RangeQuery/10K/R=10.000000         1152 us         1152 us          606 PeakRSS_MB=257.738 TotalFound=53.444k
RangeQuery/10K/R=100.000000       17795 us        17791 us           39 PeakRSS_MB=257.738 TotalFound=4.38585M
RangeQuery/10K/R=200.000000       15988 us        15985 us           44 PeakRSS_MB=257.113 TotalFound=9.12136M
RangeQuery/100K/R=0.010000          147 us          147 us         4724 PeakRSS_MB=261.875 TotalFound=1k
RangeQuery/100K/R=0.100000          194 us          194 us         3573 PeakRSS_MB=261.598 TotalFound=1.005k
RangeQuery/100K/R=1.000000          589 us          589 us         1183 PeakRSS_MB=261.535 TotalFound=4.145k
RangeQuery/100K/R=10.000000       11681 us        11679 us           60 PeakRSS_MB=261.645 TotalFound=506.846k
RangeQuery/100K/R=100.000000     185092 us       185057 us            4 PeakRSS_MB=261.535 TotalFound=43.2318M
RangeQuery/100K/R=200.000000     236637 us       236601 us            3 PeakRSS_MB=261.484 TotalFound=90.3647M
RangeQuery/1M/R=0.010000            161 us          161 us         4335 PeakRSS_MB=317.012 TotalFound=1k
RangeQuery/1M/R=0.100000            250 us          250 us         2789 PeakRSS_MB=316.949 TotalFound=1.139k
RangeQuery/1M/R=1.000000           3311 us         3311 us          211 PeakRSS_MB=316.949 TotalFound=39.044k
RangeQuery/1M/R=10.000000        134317 us       134295 us            5 PeakRSS_MB=316.324 TotalFound=5.78161M
RangeQuery/1M/R=100.000000      2830636 us      2828987 us            1 PeakRSS_MB=316.887 TotalFound=433.516M
RangeQuery/1M/R=200.000000      3216322 us      3215797 us            1 PeakRSS_MB=316.324 TotalFound=907.777M
RangeQuery/10M/R=0.010000           184 us          184 us         3703 PeakRSS_MB=789.992 TotalFound=1.001k
RangeQuery/10M/R=0.100000           658 us          656 us         1027 PeakRSS_MB=789.504 TotalFound=1.793k
RangeQuery/10M/R=1.000000         39237 us        39230 us           18 PeakRSS_MB=790.133 TotalFound=363.096k
RangeQuery/10M/R=10.000000      1550440 us      1550219 us            1 PeakRSS_MB=790.137 TotalFound=55.2471M
RangeQuery/10M/R=100.000000    36394287 us     36383802 us            1 PeakRSS_MB=790.137 TotalFound=4.39221G
RangeQuery/10M/R=200.000000    54571233 us     54555407 us            1 PeakRSS_MB=790.137 TotalFound=9.09205G

--- Executing ./knn_query_scaling.exe ---
2026-08-08T16:32:18+02:00
Running ./knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 1.06, 1.14, 0.95
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
***WARNING*** Library was built as DEBUG. Timings may be affected.
------------------------------------------------------------------------------
Benchmark                    Time             CPU   Iterations UserCounters...
------------------------------------------------------------------------------
KNNQuery/1K/k=1           74.6 us         74.5 us         8998 PeakRSS_MB=249.906
KNNQuery/1K/k=2            156 us          156 us         4468 PeakRSS_MB=249.906
KNNQuery/1K/k=4            211 us          211 us         3314 PeakRSS_MB=249.906
KNNQuery/1K/k=8            305 us          305 us         2278 PeakRSS_MB=249.906
KNNQuery/1K/k=16           434 us          434 us         1604 PeakRSS_MB=249.906
KNNQuery/1K/k=32           695 us          695 us         1007 PeakRSS_MB=249.906
KNNQuery/1K/k=64          2096 us         2095 us          333 PeakRSS_MB=249.906
KNNQuery/1K/k=128         2258 us         2254 us          310 PeakRSS_MB=249.906
KNNQuery/10K/k=1           108 us          108 us         6468 PeakRSS_MB=257.719
KNNQuery/10K/k=2           246 us          246 us         2844 PeakRSS_MB=257.719
KNNQuery/10K/k=4           369 us          369 us         1892 PeakRSS_MB=257.656
KNNQuery/10K/k=8           524 us          524 us         1328 PeakRSS_MB=257.594
KNNQuery/10K/k=16          766 us          766 us          910 PeakRSS_MB=257.594
KNNQuery/10K/k=32         1239 us         1239 us          564 PeakRSS_MB=257.531
KNNQuery/10K/k=64         3598 us         3597 us          195 PeakRSS_MB=257.531
KNNQuery/10K/k=128        3970 us         3969 us          176 PeakRSS_MB=257.469
KNNQuery/100K/k=1          140 us          140 us         4982 PeakRSS_MB=262.27
KNNQuery/100K/k=2          289 us          289 us         2427 PeakRSS_MB=262.27
KNNQuery/100K/k=4          420 us          420 us         1671 PeakRSS_MB=262.27
KNNQuery/100K/k=8          557 us          556 us         1252 PeakRSS_MB=262.266
KNNQuery/100K/k=16         786 us          785 us          887 PeakRSS_MB=262.266
KNNQuery/100K/k=32        1260 us         1260 us          555 PeakRSS_MB=262.266
KNNQuery/100K/k=64        4288 us         4287 us          163 PeakRSS_MB=262.266
KNNQuery/100K/k=128       4336 us         4335 us          161 PeakRSS_MB=262.266
KNNQuery/1M/k=1            156 us          156 us         4465 PeakRSS_MB=316.91
KNNQuery/1M/k=2            353 us          353 us         1988 PeakRSS_MB=316.914
KNNQuery/1M/k=4            466 us          466 us         1504 PeakRSS_MB=316.957
KNNQuery/1M/k=8            612 us          612 us         1129 PeakRSS_MB=316.961
KNNQuery/1M/k=16          1059 us         1057 us          663 PeakRSS_MB=317.523
KNNQuery/1M/k=32          1936 us         1935 us          361 PeakRSS_MB=317.523
KNNQuery/1M/k=64          4666 us         4665 us          151 PeakRSS_MB=317.527
KNNQuery/1M/k=128         5757 us         5756 us          121 PeakRSS_MB=317.527
KNNQuery/10M/k=1           166 us          166 us         4044 PeakRSS_MB=791.039
KNNQuery/10M/k=2           366 us          366 us         1897 PeakRSS_MB=791.109
KNNQuery/10M/k=4           530 us          530 us         1264 PeakRSS_MB=791.141
KNNQuery/10M/k=8           887 us          887 us          741 PeakRSS_MB=791.152
KNNQuery/10M/k=16         1421 us         1421 us          487 PeakRSS_MB=790.621
KNNQuery/10M/k=32         2255 us         2251 us          308 PeakRSS_MB=791.215
KNNQuery/10M/k=64         4695 us         4694 us          150 PeakRSS_MB=791.25
KNNQuery/10M/k=128        5404 us         5403 us          122 PeakRSS_MB=790.695

--- Executing ./self_knn_query_scaling.exe ---
2026-08-27T08:51:07+02:00
Running ./self_knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 3.81, 3.40, 3.30
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
----------------------------------------------------------------------------------
Benchmark                        Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------
SelfKNNQuery/1K/k=1           59.1 us         59.0 us        10057 PeakRSS_MB=142.086 items_per_second=16.9363M/s
SelfKNNQuery/1K/k=2            269 us          269 us         2619 PeakRSS_MB=142.086 items_per_second=3.7191M/s
SelfKNNQuery/1K/k=4            401 us          401 us         1755 PeakRSS_MB=142.086 items_per_second=2.49583M/s
SelfKNNQuery/1K/k=8            575 us          575 us         1210 PeakRSS_MB=142.086 items_per_second=1.74056M/s
SelfKNNQuery/1K/k=16           953 us          953 us          734 PeakRSS_MB=142.086 items_per_second=1.04935M/s
SelfKNNQuery/1K/k=32          1641 us         1641 us          427 PeakRSS_MB=142.086 items_per_second=609.462k/s
SelfKNNQuery/1K/k=64          3612 us         3612 us          193 PeakRSS_MB=142.086 items_per_second=276.857k/s
SelfKNNQuery/1K/k=128         5970 us         5969 us          117 PeakRSS_MB=142.086 items_per_second=167.52k/s
SelfKNNQuery/10K/k=1          72.7 us         72.7 us         9585 PeakRSS_MB=143.211 items_per_second=137.486M/s
SelfKNNQuery/10K/k=2           534 us          534 us         1303 PeakRSS_MB=143.211 items_per_second=18.7183M/s
SelfKNNQuery/10K/k=4           772 us          772 us          901 PeakRSS_MB=143.211 items_per_second=12.95M/s
SelfKNNQuery/10K/k=8          1438 us         1437 us          485 PeakRSS_MB=143.211 items_per_second=6.95704M/s
SelfKNNQuery/10K/k=16         2575 us         2575 us          271 PeakRSS_MB=143.211 items_per_second=3.88371M/s
SelfKNNQuery/10K/k=32         4809 us         4808 us          146 PeakRSS_MB=143.211 items_per_second=2.07982M/s
SelfKNNQuery/10K/k=64         9806 us         9805 us           71 PeakRSS_MB=143.211 items_per_second=1.01993M/s
SelfKNNQuery/10K/k=128       16323 us        16321 us           43 PeakRSS_MB=143.211 items_per_second=612.71k/s
SelfKNNQuery/100K/k=1          228 us          228 us         3088 PeakRSS_MB=147.676 items_per_second=439.192M/s
SelfKNNQuery/100K/k=2         1372 us         1372 us          513 PeakRSS_MB=147.676 items_per_second=72.8955M/s
SelfKNNQuery/100K/k=4         2494 us         2494 us          285 PeakRSS_MB=147.676 items_per_second=40.1024M/s
SelfKNNQuery/100K/k=8         4132 us         4131 us          170 PeakRSS_MB=147.676 items_per_second=24.2044M/s
SelfKNNQuery/100K/k=16        7866 us         7865 us           90 PeakRSS_MB=148.238 items_per_second=12.7143M/s
SelfKNNQuery/100K/k=32       16936 us        16934 us           41 PeakRSS_MB=147.676 items_per_second=5.90532M/s
SelfKNNQuery/100K/k=64       34613 us        34609 us           20 PeakRSS_MB=147.676 items_per_second=2.88945M/s
SelfKNNQuery/100K/k=128      95782 us        95769 us            7 PeakRSS_MB=147.676 items_per_second=1.04418M/s
SelfKNNQuery/1M/k=1           1674 us         1674 us          417 PeakRSS_MB=202.352 items_per_second=597.507M/s
SelfKNNQuery/1M/k=2          12906 us        12904 us           53 PeakRSS_MB=202.41 items_per_second=77.495M/s
SelfKNNQuery/1M/k=4          23002 us        22998 us           30 PeakRSS_MB=202.973 items_per_second=43.4811M/s
SelfKNNQuery/1M/k=8          40511 us        40505 us           17 PeakRSS_MB=202.41 items_per_second=24.6882M/s
SelfKNNQuery/1M/k=16         76639 us        76630 us            9 PeakRSS_MB=202.973 items_per_second=13.0498M/s
SelfKNNQuery/1M/k=32        158802 us       158783 us            4 PeakRSS_MB=202.973 items_per_second=6.29791M/s
SelfKNNQuery/1M/k=64        280155 us       280123 us            2 PeakRSS_MB=203.535 items_per_second=3.56986M/s
SelfKNNQuery/1M/k=128       860285 us       860181 us            1 PeakRSS_MB=203.535 items_per_second=1.16255M/s
SelfKNNQuery/10M/k=1         16412 us        16409 us           41 PeakRSS_MB=675.602 items_per_second=609.415M/s
SelfKNNQuery/10M/k=2        137593 us       137570 us            5 PeakRSS_MB=676.191 items_per_second=72.69M/s
SelfKNNQuery/10M/k=4        238491 us       238462 us            3 PeakRSS_MB=676.207 items_per_second=41.9354M/s
SelfKNNQuery/10M/k=8        414016 us       413968 us            2 PeakRSS_MB=675.668 items_per_second=24.1565M/s
SelfKNNQuery/10M/k=16       770529 us       770436 us            1 PeakRSS_MB=675.672 items_per_second=12.9797M/s
SelfKNNQuery/10M/k=32      1583602 us      1583412 us            1 PeakRSS_MB=675.688 items_per_second=6.31548M/s
SelfKNNQuery/10M/k=64      2877456 us      2877114 us            1 PeakRSS_MB=675.691 items_per_second=3.47571M/s
SelfKNNQuery/10M/k=128     8956558 us      8955489 us            1 PeakRSS_MB=675.703 items_per_second=1.11663M/s

```
