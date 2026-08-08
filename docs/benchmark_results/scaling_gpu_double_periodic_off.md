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
2026-08-08T18:54:57+02:00
Running ./self_knn_query_scaling.exe
Run on (72 X 2400 MHz CPU s)
CPU Caches:
  L1 Data 48 KiB (x72)
  L1 Instruction 32 KiB (x72)
  L2 Unified 1280 KiB (x72)
  L3 Unified 55296 KiB (x2)
Load Average: 0.66, 0.20, 0.18
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
***WARNING*** Library was built as DEBUG. Timings may be affected.
----------------------------------------------------------------------------------
Benchmark                        Time             CPU   Iterations UserCounters...
----------------------------------------------------------------------------------
SelfKNNQuery/1K/k=1            280 us          280 us         2316 PeakRSS_MB=249.914 items_per_second=3.57442M/s
SelfKNNQuery/1K/k=2            358 us          358 us         1922 PeakRSS_MB=250.477 items_per_second=2.79212M/s
SelfKNNQuery/1K/k=4            410 us          409 us         1662 PeakRSS_MB=250.477 items_per_second=2.44202M/s
SelfKNNQuery/1K/k=8            502 us          502 us         1382 PeakRSS_MB=250.477 items_per_second=1.99235M/s
SelfKNNQuery/1K/k=16           637 us          636 us         1085 PeakRSS_MB=250.477 items_per_second=1.57158M/s
SelfKNNQuery/1K/k=32           883 us          883 us          788 PeakRSS_MB=250.477 items_per_second=1.13241M/s
SelfKNNQuery/1K/k=64          2394 us         2394 us          292 PeakRSS_MB=251.039 items_per_second=417.791k/s
SelfKNNQuery/1K/k=128         2459 us         2435 us          287 PeakRSS_MB=254.414 items_per_second=410.596k/s
SelfKNNQuery/10K/k=1           357 us          357 us         1945 PeakRSS_MB=261.727 items_per_second=27.9999M/s
SelfKNNQuery/10K/k=2           541 us          539 us         1283 PeakRSS_MB=261.727 items_per_second=18.5443M/s
SelfKNNQuery/10K/k=4           654 us          653 us         1060 PeakRSS_MB=261.727 items_per_second=15.309M/s
SelfKNNQuery/10K/k=8           820 us          820 us          846 PeakRSS_MB=261.727 items_per_second=12.2016M/s
SelfKNNQuery/10K/k=16         1175 us         1175 us          589 PeakRSS_MB=261.164 items_per_second=8.51175M/s
SelfKNNQuery/10K/k=32         1891 us         1890 us          367 PeakRSS_MB=261.164 items_per_second=5.28998M/s
SelfKNNQuery/10K/k=64        10446 us        10442 us           67 PeakRSS_MB=261.164 items_per_second=957.71k/s
SelfKNNQuery/10K/k=128       12015 us        12010 us           59 PeakRSS_MB=261.727 items_per_second=832.631k/s
SelfKNNQuery/100K/k=1         1157 us         1156 us          601 PeakRSS_MB=266.301 items_per_second=86.484M/s
SelfKNNQuery/100K/k=2         1714 us         1712 us          404 PeakRSS_MB=266.301 items_per_second=58.3977M/s
SelfKNNQuery/100K/k=4         2281 us         2255 us          311 PeakRSS_MB=270.801 items_per_second=44.3545M/s
SelfKNNQuery/100K/k=8         3210 us         3203 us          219 PeakRSS_MB=270.801 items_per_second=31.2204M/s
SelfKNNQuery/100K/k=16        5265 us         5262 us          133 PeakRSS_MB=271.363 items_per_second=19.0036M/s
SelfKNNQuery/100K/k=32       10046 us        10041 us           70 PeakRSS_MB=271.363 items_per_second=9.95869M/s
SelfKNNQuery/100K/k=64       78350 us        78333 us            9 PeakRSS_MB=270.801 items_per_second=1.27659M/s
SelfKNNQuery/100K/k=128      98157 us        98131 us            7 PeakRSS_MB=271.363 items_per_second=1.01904M/s
SelfKNNQuery/1M/k=1          17351 us        17344 us           40 PeakRSS_MB=325.266 items_per_second=57.6582M/s
SelfKNNQuery/1M/k=2          35918 us        35906 us           19 PeakRSS_MB=325.828 items_per_second=27.8507M/s
SelfKNNQuery/1M/k=4          59772 us        59756 us           12 PeakRSS_MB=325.266 items_per_second=16.7348M/s
SelfKNNQuery/1M/k=8          98997 us        98977 us            7 PeakRSS_MB=325.828 items_per_second=10.1034M/s
SelfKNNQuery/1M/k=16        172687 us       172004 us            4 PeakRSS_MB=325.266 items_per_second=5.81382M/s
SelfKNNQuery/1M/k=32        305593 us       305539 us            2 PeakRSS_MB=324.891 items_per_second=3.2729M/s
SelfKNNQuery/1M/k=64        976686 us       976514 us            1 PeakRSS_MB=324.891 items_per_second=1.02405M/s
SelfKNNQuery/1M/k=128      1202328 us      1200961 us            1 PeakRSS_MB=324.891 items_per_second=832.667k/s
SelfKNNQuery/10M/k=1        300553 us       300487 us            2 PeakRSS_MB=799.211 items_per_second=33.2793M/s
SelfKNNQuery/10M/k=2        591711 us       591593 us            1 PeakRSS_MB=799.227 items_per_second=16.9035M/s
SelfKNNQuery/10M/k=4        904310 us       904085 us            1 PeakRSS_MB=799.254 items_per_second=11.0609M/s
SelfKNNQuery/10M/k=8       1403965 us      1403765 us            1 PeakRSS_MB=799.266 items_per_second=7.1237M/s
SelfKNNQuery/10M/k=16      2276180 us      2275810 us            1 PeakRSS_MB=799.273 items_per_second=4.39404M/s
SelfKNNQuery/10M/k=32      3872268 us      3870557 us            1 PeakRSS_MB=799.289 items_per_second=2.58361M/s
SelfKNNQuery/10M/k=64     10536649 us     10533974 us            1 PeakRSS_MB=798.742 items_per_second=949.309k/s
SelfKNNQuery/10M/k=128    12853821 us     12850756 us            1 PeakRSS_MB=799.336 items_per_second=778.164k/s

```
