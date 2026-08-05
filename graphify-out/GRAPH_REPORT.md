# Graph Report - fasttree  (2026-08-05)

## Corpus Check
- 41 files · ~131,587 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 305 nodes · 598 edges · 29 communities (16 shown, 13 thin omitted)
- Extraction: 87% EXTRACTED · 13% INFERRED · 0% AMBIGUOUS · INFERRED: 78 edges (avg confidence: 0.8)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `f690b4b6`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- Utilities & Benchmark Helpers
- HLBVH Tree Construction
- Morton Encoding & Key Sorting
- Domain Decomposition & MPI Halos
- BoundingBox
- Particle SoA & Field Storage
- Bounding Box & Spatial Bounds
- IEEE 754 Floating-Point Traits
- MPI Datatype Traits & Communication
- CPU Profiling Scripts & Options
- MPI Testing Suite
- Architecture & Design Specifications
- CPU/GPU Benchmark Reports
- Scaling & Performance Benchmarks
- GPU Profiling Tools
- Variation Runner Scripts
- Pointer-Free Tree Architecture
- Structure of Arrays (SoA) Standard
- CMake Build System
- Range & kNN Traversal API
- Graphify Rule System
- Graphify Workflow Operations
- Compress-Sort-Decompress (CSD) Pattern
- Intra-Voxel Local Memory Sort
- SYCL & oneDPL Backend Setup
- Testing Infrastructure Guidelines
- main
- FastTree Core Overview

## God Nodes (most connected - your core abstractions)
1. `TreeSoA` - 24 edges
2. `build_bvh()` - 21 edges
3. `sfc_encode()` - 20 edges
4. `sfc_key` - 18 edges
5. `particles` - 16 edges
6. `build_tree()` - 16 edges
7. `range_query()` - 16 edges
8. `RegisterMaxHeap` - 15 edges
9. `BoundingBox` - 14 edges
10. `test_mpi_pipeline()` - 14 edges

## Surprising Connections (you probably didn't know these)
- `Pointer-Free Tree Topologies` --semantically_similar_to--> `Pointer-Free Prime Directive`  [INFERRED] [semantically similar]
  DESIGN.md → AGENTS.md
- `Structure of Arrays Data Layout` --semantically_similar_to--> `Structure of Arrays (SoA) Mandatory Layout`  [INFERRED] [semantically similar]
  DESIGN.md → AGENTS.md
- `BM_GPUSort_Lazy()` --calls--> `sfc_encode()`  [INFERRED]
  test/benchmark/gpu_sort_scaling.cpp → src/hlbvh.hpp
- `BM_MortonEncode_Lazy()` --calls--> `sfc_encode()`  [INFERRED]
  test/benchmark/sfc_encoding_scaling.cpp → src/hlbvh.hpp
- `main()` --calls--> `sfc_encode()`  [INFERRED]
  test/main.cpp → src/hlbvh.hpp

## Import Cycles
- None detected.

## Hyperedges (group relationships)
- **HLBVH Core Architecture Pattern** — design_pointer_free_constraint, design_soa_format, design_csd_pattern, design_intra_voxel_sort [EXTRACTED 1.00]
- **Benchmark Reporting Suite** — docs_benchmark_results_cpu_all_variations_data, docs_benchmark_results_gpu_all_variations_data, docs_mpi_benchmark_results_cpu_all_variations_data, docs_mpi_benchmark_results_gpu_all_variations_data [EXTRACTED 1.00]
- **Tree Construction Pipeline Scaling Benchmarks** — docs_benchmark_scaling_morton_encode, docs_benchmark_scaling_radix_sort, docs_benchmark_scaling_tree_build [INFERRED 0.85]
- **Spatial Traversal & Search Scaling Benchmarks** — docs_benchmark_scaling_chart, docs_benchmark_scaling_knn_query, docs_benchmark_scaling_range_query [INFERRED 0.85]

## Communities (29 total, 13 thin omitted)

### Community 0 - "Utilities & Benchmark Helpers"
Cohesion: 0.06
Nodes (44): vector, get_peak_rss(), string, vector, load_hdf5_data(), ParticleData, count, masses (+36 more)

### Community 1 - "HLBVH Tree Construction"
Cohesion: 0.10
Nodes (45): build_bvh(), build_tree(), compute_bbox(), copy_back_and_free(), ensure_device_readable(), ensure_device_writable(), free_device_readable(), get_common_prefix_length() (+37 more)

### Community 2 - "Morton Encoding & Key Sorting"
Cohesion: 0.10
Nodes (30): MyIntPosType, PosType, sort_key_t, compact3_u64(), sfc1D, spread3_u64(), convert_to_sfc1d(), convert_to_sfc1d_impl() (+22 more)

### Community 3 - "Domain Decomposition & MPI Halos"
Cohesion: 0.10
Nodes (32): exchange_halos(), generate_splitters(), get_deterministic_splitters(), get_global_bounding_box(), get_global_histogram(), FloatT, MPI_Datatype, queue (+24 more)

### Community 4 - "BoundingBox"
Cohesion: 0.20
Nodes (10): BoundingBox, max_x, max_y, max_z, min_x, min_y, min_z, encode_to_sfc1d() (+2 more)

### Community 5 - "Particle SoA & Field Storage"
Cohesion: 0.16
Nodes (17): DistT, IdxT, local_accessor, nd_item, local_bitonic_sort(), local_heap_sift_down(), local_heap_sift_up(), local_swap() (+9 more)

### Community 7 - "IEEE 754 Floating-Point Traits"
Cohesion: 0.29
Nodes (7): ieee754_traits<double>, mantissa_bits, mantissa_mask, ieee754_traits<float>, mantissa_bits, mantissa_mask, uint_type

### Community 8 - "MPI Datatype Traits & Communication"
Cohesion: 0.32
Nodes (4): MPI_Datatype, mpi_type_traits, mpi_type_traits<double>, mpi_type_traits<float>

### Community 9 - "CPU Profiling Scripts & Options"
Cohesion: 0.38
Nodes (5): check_tool(), OMP_PLACES, OMP_PROC_BIND, profile_cpu.sh script, usage()

### Community 10 - "MPI Testing Suite"
Cohesion: 0.32
Nodes (4): MPI_Datatype, mpi_type_traits, mpi_type_traits<double>, mpi_type_traits<float>

### Community 11 - "Architecture & Design Specifications"
Cohesion: 0.33
Nodes (6): HPC C++ & SYCL Developer Role, HLBVH Pipeline Algorithm, System Design SYCL-HLBVH, HLBVH Pantaleoni & Luebke Paper, FastTree Implementation Roadmap, Project Progress Tracking

### Community 12 - "CPU/GPU Benchmark Reports"
Cohesion: 0.33
Nodes (6): CPU Benchmark Results (All Variations), GPU Benchmark Results (All Variations), MPI CPU Benchmark Results, MPI GPU Benchmark Results, Profiling Guidelines & Setup, Profiling Analysis Report

### Community 13 - "Scaling & Performance Benchmarks"
Cohesion: 0.47
Nodes (6): Benchmark Scaling Chart, KNNQuery Scaling Benchmark, MortonEncode Scaling Benchmark, RadixSort Scaling Benchmark, RangeQuery Scaling Benchmark, TreeBuild Scaling Benchmark

### Community 14 - "GPU Profiling Tools"
Cohesion: 0.60
Nodes (3): check_tool(), profile_gpu.sh script, usage()

### Community 27 - "main"
Cohesion: 0.42
Nodes (8): coord_t, dist_t, int_dist_sq_to_phys(), int_pos_to_phys(), main(), periodic_dist(), phys_pos_to_int(), phys_radius_to_int()

## Knowledge Gaps
- **82 isolated node(s):** `mpi_type_traits`, `pos_x`, `pos_y`, `pos_z`, `id` (+77 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **13 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `build_bvh()` connect `HLBVH Tree Construction` to `Utilities & Benchmark Helpers`, `main`, `Morton Encoding & Key Sorting`, `Domain Decomposition & MPI Halos`?**
  _High betweenness centrality (0.068) - this node is a cross-community bridge._
- **Why does `sfc_encode()` connect `HLBVH Tree Construction` to `Utilities & Benchmark Helpers`, `Morton Encoding & Key Sorting`, `Domain Decomposition & MPI Halos`, `BoundingBox`, `main`?**
  _High betweenness centrality (0.066) - this node is a cross-community bridge._
- **Are the 10 inferred relationships involving `build_bvh()` (e.g. with `to_sort_key()` and `main()`) actually correct?**
  _`build_bvh()` has 10 INFERRED edges - model-reasoned connections that need verification._
- **Are the 8 inferred relationships involving `sfc_encode()` (e.g. with `get_deterministic_splitters()` and `get_global_histogram()`) actually correct?**
  _`sfc_encode()` has 8 INFERRED edges - model-reasoned connections that need verification._
- **What connects `mpi_type_traits`, `pos_x`, `pos_y` to the rest of the system?**
  _82 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `Utilities & Benchmark Helpers` be split into smaller, more focused modules?**
  _Cohesion score 0.06429070580013976 - nodes in this community are weakly interconnected._
- **Should `HLBVH Tree Construction` be split into smaller, more focused modules?**
  _Cohesion score 0.09647058823529411 - nodes in this community are weakly interconnected._