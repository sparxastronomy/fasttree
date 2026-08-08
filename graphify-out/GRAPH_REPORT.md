# Graph Report - fasttree  (2026-08-08)

## Corpus Check
- 51 files · ~141,547 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 340 nodes · 522 edges · 68 communities (16 shown, 52 thin omitted)
- Extraction: 84% EXTRACTED · 16% INFERRED · 0% AMBIGUOUS · INFERRED: 83 edges (avg confidence: 0.8)
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `8570383c`
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
- SYCL Test Suite
- Graphify Rule System
- Graphify Workflow Operations
- Compress-Sort-Decompress (CSD) Pattern
- Intra-Voxel Local Memory Sort
- SYCL & oneDPL Backend Setup
- Testing Infrastructure Guidelines
- main
- FastTree Core Overview
- scaling_cpu_double_periodic_off.md
- scaling_cpu_double_periodic_on.md
- scaling_cpu_int32_periodic_off.md
- scaling_cpu_int32_periodic_on.md
- scaling_cpu_int64_periodic_off.md
- scaling_cpu_int64_periodic_on.md
- run_gpu_scaling.sh
- ponytail.md
- FloatT
- MPI_Datatype
- queue
- coord_t
- dist_t
- FloatT
- queue
- sfc1D
- T
- vector
- sfc1D
- FloatT
- MPI_Datatype
- string
- State
- string
- State
- string
- State
- string
- State
- string
- State
- string
- State
- string
- coord_t
- dist_t
- queue
- MPI_Datatype

## God Nodes (most connected - your core abstractions)
1. `TreeSoA` - 23 edges
2. `build_bvh()` - 19 edges
3. `sfc_encode()` - 17 edges
4. `RegisterMaxHeap` - 15 edges
5. `build_tree()` - 14 edges
6. `range_query()` - 14 edges
7. `ParticleData` - 14 edges
8. `BoundingBox` - 13 edges
9. `sfc_key` - 12 edges
10. `test_mpi_pipeline()` - 12 edges

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

## Communities (68 total, 52 thin omitted)

### Community 0 - "Utilities & Benchmark Helpers"
Cohesion: 0.08
Nodes (36): fill_particle_coords(), get_peak_rss(), coord_t, string, T, vector, load_hdf5_data(), ParticleData (+28 more)

### Community 1 - "HLBVH Tree Construction"
Cohesion: 0.07
Nodes (46): uint64_t, build_bvh(), build_tree(), compute_bbox(), copy_back_and_free(), encode_to_sfc1d(), ensure_device_readable(), ensure_device_writable() (+38 more)

### Community 2 - "Morton Encoding & Key Sorting"
Cohesion: 0.11
Nodes (27): MyIntPosType, MyIntPosTypeSigned, PosType, sort_key_t, convert_to_sfc1d(), convert_to_sfc1d_impl(), float_to_int_rep(), get_lo_word() (+19 more)

### Community 3 - "Domain Decomposition & MPI Halos"
Cohesion: 0.08
Nodes (28): exchange_halos(), generate_splitters(), get_deterministic_splitters(), get_global_bounding_box(), get_global_histogram(), mpi_type_traits, mpi_type_traits<double>, mpi_type_traits<float> (+20 more)

### Community 5 - "Particle SoA & Field Storage"
Cohesion: 0.16
Nodes (17): DistT, IdxT, local_accessor, nd_item, local_bitonic_sort(), local_heap_sift_down(), local_heap_sift_up(), local_swap() (+9 more)

### Community 6 - "Bounding Box & Spatial Bounds"
Cohesion: 0.25
Nodes (6): hex_to_rgba(), load_all_benchmark_files(), parse_benchmark_file(), Scans and parses all markdown benchmark result files., Convert hex color to rgba string for Plotly shaded fills, Parses Google Benchmark results from markdown files in docs/benchmark_results/

### Community 7 - "IEEE 754 Floating-Point Traits"
Cohesion: 0.29
Nodes (7): uint_type, ieee754_traits<double>, mantissa_bits, mantissa_mask, ieee754_traits<float>, mantissa_bits, mantissa_mask

### Community 8 - "MPI Datatype Traits & Communication"
Cohesion: 0.25
Nodes (4): main(), mpi_type_traits, mpi_type_traits<double>, mpi_type_traits<float>

### Community 9 - "CPU Profiling Scripts & Options"
Cohesion: 0.29
Nodes (3): mpi_type_traits, mpi_type_traits<double>, mpi_type_traits<float>

### Community 10 - "MPI Testing Suite"
Cohesion: 0.38
Nodes (5): check_tool(), OMP_PLACES, OMP_PROC_BIND, profile_cpu.sh script, usage()

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
Cohesion: 0.52
Nodes (6): int_dist_sq_to_phys(), int_pos_to_phys(), main(), periodic_dist(), phys_pos_to_int(), phys_radius_to_int()

### Community 15 - "Variation Runner Scripts"
Cohesion: 0.60
Nodes (3): check_tool(), profile_gpu.sh script, usage()

## Knowledge Gaps
- **91 isolated node(s):** `mpi_type_traits`, `pos_x`, `pos_y`, `pos_z`, `id` (+86 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **52 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **Why does `build_bvh()` connect `HLBVH Tree Construction` to `Utilities & Benchmark Helpers`, `Morton Encoding & Key Sorting`, `Domain Decomposition & MPI Halos`, `GPU Profiling Tools`?**
  _High betweenness centrality (0.045) - this node is a cross-community bridge._
- **Are the 10 inferred relationships involving `build_bvh()` (e.g. with `to_sort_key()` and `main()`) actually correct?**
  _`build_bvh()` has 10 INFERRED edges - model-reasoned connections that need verification._
- **Are the 8 inferred relationships involving `sfc_encode()` (e.g. with `get_deterministic_splitters()` and `get_global_histogram()`) actually correct?**
  _`sfc_encode()` has 8 INFERRED edges - model-reasoned connections that need verification._
- **What connects `mpi_type_traits`, `pos_x`, `pos_y` to the rest of the system?**
  _91 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `Utilities & Benchmark Helpers` be split into smaller, more focused modules?**
  _Cohesion score 0.0786308973172988 - nodes in this community are weakly interconnected._
- **Should `HLBVH Tree Construction` be split into smaller, more focused modules?**
  _Cohesion score 0.07127882599580712 - nodes in this community are weakly interconnected._
- **Should `Morton Encoding & Key Sorting` be split into smaller, more focused modules?**
  _Cohesion score 0.10606060606060606 - nodes in this community are weakly interconnected._