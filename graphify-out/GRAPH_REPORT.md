# Graph Report - fasttree  (2026-08-28)

## Corpus Check
- 62 files · ~173,396 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 462 nodes · 882 edges · 47 communities (18 shown, 29 thin omitted)
- Extraction: 88% EXTRACTED · 12% INFERRED · 0% AMBIGUOUS · INFERRED: 109 edges (avg confidence: 0.8)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `6b8b22b7`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- vector
- hlbvh.hpp
- sfc.peano_hilbert.hpp
- domain_decomposition.hpp
- DistT
- particles
- 4. Comprehensive GPU Benchmark Evaluation (NVIDIA SM_80)
- visualize_benchmarks.py
- advance_mpi.cpp
- test_mpi.cpp
- ieee754_traits<double>
- profile_cpu.sh
- HPC C++ & SYCL Developer Role
- Profiling Analysis Report
- Benchmark Scaling Chart
- profile_gpu.sh
- generate_benchmark_plots.py
- run_all_variations.sh
- run_cpu_scaling.sh
- Pointer-Free Prime Directive
- ponytail.md
- Structure of Arrays (SoA) Mandatory Layout
- Root CMake Build System
- Range and kNN Query Traversal
- scaling_cpu_double_periodic_off.md
- scaling_cpu_double_periodic_on.md
- Benchmark Comparison (GPU Variations)
- scaling_cpu_int32_periodic_off.md
- scaling_cpu_int32_periodic_on.md
- scaling_cpu_int64_periodic_off.md
- scaling_cpu_int64_periodic_on.md
- scaling_gpu_double_periodic_off.md
- scaling_gpu_double_periodic_on.md
- scaling_gpu_int32_periodic_off.md
- scaling_gpu_int32_periodic_on.md
- scaling_gpu_int64_periodic_off.md
- scaling_gpu_int64_periodic_on.md
- run_gpu_scaling.sh
- run_single_scaling.sh
- Graphify Rule Guidelines
- Graphify Workflow
- Compress-Sort-Decompress (CSD) Pattern
- Intra-Voxel Local Memory Sort
- SYCL and oneDPL Heterogeneous Backend
- Testing Framework Documentation
- FastTree Project Overview

## God Nodes (most connected - your core abstractions)
1. `TreeSoA` - 31 edges
2. `build_bvh()` - 26 edges
3. `particles` - 23 edges
4. `sfc_encode()` - 20 edges
5. `sfc_key` - 18 edges
6. `build_tree()` - 16 edges
7. `range_query()` - 16 edges
8. `RegisterMaxHeap` - 15 edges
9. `BoundingBox` - 14 edges
10. `ParticleData` - 14 edges

## Surprising Connections (you probably didn't know these)
- `Pointer-Free Tree Topologies` --semantically_similar_to--> `Pointer-Free Prime Directive`  [INFERRED] [semantically similar]
  DESIGN.md → AGENTS.md
- `Structure of Arrays Data Layout` --semantically_similar_to--> `Structure of Arrays (SoA) Mandatory Layout`  [INFERRED] [semantically similar]
  DESIGN.md → AGENTS.md
- `main()` --calls--> `get_deterministic_splitters()`  [INFERRED]
  test/benchmark/domain_decomposition_scaling.cpp → src/domain_decomposition.hpp
- `test_mpi_pipeline()` --calls--> `get_deterministic_splitters()`  [INFERRED]
  test/test_domain_decomposition.cpp → src/domain_decomposition.hpp
- `BM_GPUSort_Lazy()` --calls--> `sfc_encode()`  [INFERRED]
  test/benchmark/gpu_sort_scaling.cpp → src/hlbvh.hpp

## Import Cycles
- None detected.

## Hyperedges (group relationships)
- **HLBVH Core Architecture Pattern** — design_pointer_free_constraint, design_soa_format, design_csd_pattern, design_intra_voxel_sort [EXTRACTED 1.00]
- **Benchmark Reporting Suite** — docs_benchmark_results_cpu_all_variations_data, docs_benchmark_results_gpu_all_variations_data, docs_mpi_benchmark_results_cpu_all_variations_data, docs_mpi_benchmark_results_gpu_all_variations_data [EXTRACTED 1.00]
- **Tree Construction Pipeline Scaling Benchmarks** — docs_benchmark_scaling_morton_encode, docs_benchmark_scaling_radix_sort, docs_benchmark_scaling_tree_build [INFERRED 0.85]
- **Spatial Traversal & Search Scaling Benchmarks** — docs_benchmark_scaling_chart, docs_benchmark_scaling_knn_query, docs_benchmark_scaling_range_query [INFERRED 0.85]

## Communities (47 total, 29 thin omitted)

### Community 0 - "vector"
Cohesion: 0.06
Nodes (54): vector, compute_optimal_knn_batch_size(), fill_particle_coords(), get_available_device_memory(), get_peak_rss(), coord_t, queue, string (+46 more)

### Community 1 - "hlbvh.hpp"
Cohesion: 0.08
Nodes (59): sort_key_t, build_bvh(), build_tree(), compute_bbox(), copy_back_and_free(), dispatch_self_knn_grouped_large_k(), dispatch_self_knn_grouped_small_k(), encode_to_sfc1d() (+51 more)

### Community 2 - "sfc.peano_hilbert.hpp"
Cohesion: 0.08
Nodes (39): MyIntPosType, MyIntPosTypeSigned, PosType, get_deterministic_splitters(), compact3_u64(), sfc1D, spread3_u64(), convert_to_sfc1d() (+31 more)

### Community 3 - "domain_decomposition.hpp"
Cohesion: 0.09
Nodes (30): exchange_halos(), generate_splitters(), get_global_bounding_box(), get_global_histogram(), FloatT, MPI_Datatype, queue, mpi_type_traits (+22 more)

### Community 4 - "DistT"
Cohesion: 0.14
Nodes (19): DistT, IdxT, local_accessor, nd_item, local_bitonic_sort(), local_swap(), LocalMaxHeap, DIST_MAX (+11 more)

### Community 5 - "particles"
Cohesion: 0.15
Nodes (34): vector, particles, id, is_ghost, pos_x, pos_y, pos_z, BFNeighbor (+26 more)

### Community 6 - "4. Comprehensive GPU Benchmark Evaluation (NVIDIA SM_80)"
Cohesion: 0.06
Nodes (31): 1. `cpu_double` (Periodic BC: OFF), 1. Executive Summary & Highlights, 1. `gpu_double` (Periodic BC: OFF), 1. Space-Filling Curve (SFC) Grouped Self-kNN Traversal, 2. `cpu_double` (Periodic BC: ON), 2. Detailed Architectural Features, 2. `gpu_double` (Periodic BC: ON), 2. SIMD-Aware Dynamic Sub-Group Dispatching (+23 more)

### Community 7 - "visualize_benchmarks.py"
Cohesion: 0.25
Nodes (9): cache_data, get_latest_release(), hex_to_rgba(), load_all_benchmark_files(), parse_benchmark_file(), parse_mpi_benchmark_file(), Scans and parses all markdown benchmark result files., Convert hex color to rgba string for Plotly shaded fills (+1 more)

### Community 8 - "advance_mpi.cpp"
Cohesion: 0.32
Nodes (4): MPI_Datatype, mpi_type_traits, mpi_type_traits<double>, mpi_type_traits<float>

### Community 9 - "test_mpi.cpp"
Cohesion: 0.32
Nodes (4): MPI_Datatype, mpi_type_traits, mpi_type_traits<double>, mpi_type_traits<float>

### Community 10 - "ieee754_traits<double>"
Cohesion: 0.29
Nodes (7): ieee754_traits<double>, mantissa_bits, mantissa_mask, ieee754_traits<float>, mantissa_bits, mantissa_mask, uint_type

### Community 11 - "profile_cpu.sh"
Cohesion: 0.38
Nodes (5): check_tool(), OMP_PLACES, OMP_PROC_BIND, profile_cpu.sh script, usage()

### Community 12 - "HPC C++ & SYCL Developer Role"
Cohesion: 0.33
Nodes (6): HPC C++ & SYCL Developer Role, HLBVH Pipeline Algorithm, System Design SYCL-HLBVH, HLBVH Pantaleoni & Luebke Paper, FastTree Implementation Roadmap, Project Progress Tracking

### Community 13 - "Profiling Analysis Report"
Cohesion: 0.33
Nodes (6): CPU Benchmark Results (All Variations), GPU Benchmark Results (All Variations), MPI CPU Benchmark Results, MPI GPU Benchmark Results, Profiling Guidelines & Setup, Profiling Analysis Report

### Community 14 - "Benchmark Scaling Chart"
Cohesion: 0.47
Nodes (6): Benchmark Scaling Chart, KNNQuery Scaling Benchmark, MortonEncode Scaling Benchmark, RadixSort Scaling Benchmark, RangeQuery Scaling Benchmark, TreeBuild Scaling Benchmark

### Community 15 - "profile_gpu.sh"
Cohesion: 0.60
Nodes (3): check_tool(), profile_gpu.sh script, usage()

### Community 26 - "Benchmark Comparison (GPU Variations)"
Cohesion: 0.09
Nodes (22): 1. `cpu_double` (Periodic BC: OFF), 1. `gpu_double` (Periodic BC: OFF), 2. `cpu_double` (Periodic BC: ON), 2. `gpu_double` (Periodic BC: ON), 3. `cpu_int32` (Periodic BC: OFF), 3. `gpu_int32` (Periodic BC: OFF), 4. `cpu_int32` (Periodic BC: ON), 4. `gpu_int32` (Periodic BC: ON) (+14 more)

## Knowledge Gaps
- **149 isolated node(s):** `mpi_type_traits`, `pos_x`, `pos_y`, `pos_z`, `id` (+144 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **29 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `TreeSoA` connect `hlbvh.hpp` to `particles`?**
  _High betweenness centrality (0.045) - this node is a cross-community bridge._
- **Why does `build_bvh()` connect `hlbvh.hpp` to `vector`, `sfc.peano_hilbert.hpp`, `domain_decomposition.hpp`, `particles`?**
  _High betweenness centrality (0.045) - this node is a cross-community bridge._
- **Are the 15 inferred relationships involving `build_bvh()` (e.g. with `to_sort_key()` and `main()`) actually correct?**
  _`build_bvh()` has 15 INFERRED edges - model-reasoned connections that need verification._
- **What connects `mpi_type_traits`, `pos_x`, `pos_y` to the rest of the system?**
  _149 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `vector` be split into smaller, more focused modules?**
  _Cohesion score 0.05803571428571429 - nodes in this community are weakly interconnected._
- **Should `hlbvh.hpp` be split into smaller, more focused modules?**
  _Cohesion score 0.08076923076923077 - nodes in this community are weakly interconnected._
- **Should `sfc.peano_hilbert.hpp` be split into smaller, more focused modules?**
  _Cohesion score 0.07585568917668825 - nodes in this community are weakly interconnected._