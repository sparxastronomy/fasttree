# Graph Report - .  (2026-08-04)

## Corpus Check
- 44 files · ~126,705 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 269 nodes · 513 edges · 29 communities (16 shown, 13 thin omitted)
- Extraction: 85% EXTRACTED · 15% INFERRED · 0% AMBIGUOUS · INFERRED: 79 edges (avg confidence: 0.8)
- Token cost: 0 input · 0 output

## Community Hubs (Navigation)
- Utilities & Benchmark Helpers
- HLBVH Tree Construction
- Morton Encoding & Key Sorting
- Domain Decomposition & MPI Halos
- TreeSoA Topology & Layout
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
- Gemini Integration Guidelines
- FastTree Core Overview

## God Nodes (most connected - your core abstractions)
1. `TreeSoA` - 22 edges
2. `build_bvh()` - 22 edges
3. `sfc_encode()` - 21 edges
4. `sfc_key` - 18 edges
5. `particles` - 16 edges
6. `build_tree()` - 16 edges
7. `range_query()` - 16 edges
8. `BoundingBox` - 14 edges
9. `knn_query()` - 14 edges
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
- `main()` --calls--> `build_tree()`  [INFERRED]
  test/test_bb.cpp → src/hlbvh.hpp

## Import Cycles
- None detected.

## Hyperedges (group relationships)
- **HLBVH Core Architecture Pattern** — design_pointer_free_constraint, design_soa_format, design_csd_pattern, design_intra_voxel_sort [EXTRACTED 1.00]
- **Benchmark Reporting Suite** — docs_benchmark_results_cpu_all_variations_data, docs_benchmark_results_gpu_all_variations_data, docs_mpi_benchmark_results_cpu_all_variations_data, docs_mpi_benchmark_results_gpu_all_variations_data [EXTRACTED 1.00]
- **Tree Construction Pipeline Scaling Benchmarks** — docs_benchmark_scaling_morton_encode, docs_benchmark_scaling_radix_sort, docs_benchmark_scaling_tree_build [INFERRED 0.85]
- **Spatial Traversal & Search Scaling Benchmarks** — docs_benchmark_scaling_chart, docs_benchmark_scaling_knn_query, docs_benchmark_scaling_range_query [INFERRED 0.85]

## Communities (29 total, 13 thin omitted)

### Community 0 - "Utilities & Benchmark Helpers"
Cohesion: 0.09
Nodes (34): vector, get_peak_rss(), string, load_hdf5_data(), print_benchmark_row(), Timer, name, start_time (+26 more)

### Community 1 - "HLBVH Tree Construction"
Cohesion: 0.13
Nodes (35): build_bvh(), build_tree(), compute_bbox(), copy_back_and_free(), encode_to_sfc1d(), ensure_device_readable(), ensure_device_writable(), free_device_readable() (+27 more)

### Community 2 - "Morton Encoding & Key Sorting"
Cohesion: 0.10
Nodes (30): MyIntPosType, PosType, sort_key_t, compact3_u64(), sfc1D, spread3_u64(), convert_to_sfc1d(), convert_to_sfc1d_impl() (+22 more)

### Community 3 - "Domain Decomposition & MPI Halos"
Cohesion: 0.11
Nodes (29): exchange_halos(), generate_splitters(), get_deterministic_splitters(), get_global_bounding_box(), get_global_histogram(), FloatT, MPI_Datatype, queue (+21 more)

### Community 4 - "TreeSoA Topology & Layout"
Cohesion: 0.12
Nodes (15): TreeSoA, id, is_ghost, left_child, max_x, max_y, max_z, min_x (+7 more)

### Community 5 - "Particle SoA & Field Storage"
Cohesion: 0.14
Nodes (13): vector, ParticleData, count, masses, pos_x, pos_y, pos_z, vel_x (+5 more)

### Community 6 - "Bounding Box & Spatial Bounds"
Cohesion: 0.25
Nodes (7): BoundingBox, max_x, max_y, max_z, min_x, min_y, min_z

### Community 7 - "IEEE 754 Floating-Point Traits"
Cohesion: 0.29
Nodes (7): ieee754_traits<double>, mantissa_bits, mantissa_mask, ieee754_traits<float>, mantissa_bits, mantissa_mask, uint_type

### Community 8 - "MPI Datatype Traits & Communication"
Cohesion: 0.33
Nodes (4): MPI_Datatype, mpi_type_traits, mpi_type_traits<double>, mpi_type_traits<float>

### Community 9 - "CPU Profiling Scripts & Options"
Cohesion: 0.38
Nodes (5): check_tool(), OMP_PLACES, OMP_PROC_BIND, profile_cpu.sh script, usage()

### Community 10 - "MPI Testing Suite"
Cohesion: 0.33
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

## Knowledge Gaps
- **79 isolated node(s):** `mpi_type_traits`, `pos_x`, `pos_y`, `pos_z`, `id` (+74 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **13 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `TreeSoA` connect `TreeSoA Topology & Layout` to `HLBVH Tree Construction`?**
  _High betweenness centrality (0.086) - this node is a cross-community bridge._
- **Why does `build_bvh()` connect `HLBVH Tree Construction` to `Utilities & Benchmark Helpers`, `Morton Encoding & Key Sorting`, `Domain Decomposition & MPI Halos`, `TreeSoA Topology & Layout`?**
  _High betweenness centrality (0.076) - this node is a cross-community bridge._
- **Why does `sfc_encode()` connect `HLBVH Tree Construction` to `Utilities & Benchmark Helpers`, `Morton Encoding & Key Sorting`, `Domain Decomposition & MPI Halos`, `Bounding Box & Spatial Bounds`?**
  _High betweenness centrality (0.072) - this node is a cross-community bridge._
- **Are the 10 inferred relationships involving `build_bvh()` (e.g. with `to_sort_key()` and `main()`) actually correct?**
  _`build_bvh()` has 10 INFERRED edges - model-reasoned connections that need verification._
- **Are the 8 inferred relationships involving `sfc_encode()` (e.g. with `get_deterministic_splitters()` and `get_global_histogram()`) actually correct?**
  _`sfc_encode()` has 8 INFERRED edges - model-reasoned connections that need verification._
- **What connects `mpi_type_traits`, `pos_x`, `pos_y` to the rest of the system?**
  _79 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `Utilities & Benchmark Helpers` be split into smaller, more focused modules?**
  _Cohesion score 0.08637873754152824 - nodes in this community are weakly interconnected._