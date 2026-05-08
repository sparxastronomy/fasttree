# SYCL-HLBVH API Reference

This document outlines the core data structures and functions provided by `src/hlbvh.hpp`. The library is implemented as a single-header C++23/SYCL library designed for extreme performance on GPUs and trivial MPI serialization.

## Core Data Structures

### `TreeSoA`
The foundational structure representing the Hierarchical Linear Bounding Volume Hierarchy.
It strictly adheres to a **Structure of Arrays (SoA)** memory layout and uses integer indices instead of pointers. This pointer-free design allows the tree to be trivially copied and sent over MPI.

```cpp
struct TreeSoA {
  float *min_x, *max_x;
  float *min_y, *max_y;
  float *min_z, *max_z;
  int *left_child;
  int *right_child;
  int *parent;
  size_t num_leaves;
  size_t num_internal;
  
  // Constructors and memory management
  TreeSoA(sycl::queue &q, size_t n);
  void free(sycl::queue &q);
};
```
- **Leaves vs Internal Nodes:** The tree stores exactly $N$ leaves (particles) and $N-1$ internal nodes.
- **Indexing:** Internal nodes are indexed from $0$ to $N-2$. The root is always at index $0$. Leaves are indexed from $N-1$ to $2N-2$.
- **Memory:** All arrays are allocated using `sycl::malloc_shared`, making them accessible from both Host and Device (Unified Shared Memory).

### `particles<T>`
A basic SoA container for input spatial coordinates.
```cpp
template <typename T>
struct particles {
  std::vector<T> pos_x, pos_y, pos_z;
};
```

---

## Core Algorithms

### `build_bvh`
The high-level entry point for constructing the HLBVH.

```cpp
void build_bvh(sycl::queue &q, const particles<float> &p, TreeSoA &tree);
```
**Pipeline Overview:**
1. Computes the global bounding box of all particles.
2. Quantizes coordinates and generates 63-bit **Morton Codes** for each particle (`morton_encode`).
3. Sorts the particles based on their Morton codes (`sort_morton_keys`).
4. Generates the tree topology in parallel using the Karras (2012) binary-radix tree algorithm (`build_tree`).
5. Computes bounding boxes for all internal nodes using a bottom-up traversal with `sycl::memory_order::acq_rel` atomic counters.

---

## Query APIs

All query algorithms are designed to be executed from within a SYCL `parallel_for` kernel over a set of query points. They utilize a non-recursive, stack-based traversal algorithm to prevent stack overflows and minimize divergent thread execution.

### `range_query`
Finds all particles within a specified bounding radius from a query point.

```cpp
void range_query(sycl::queue &q, const TreeSoA &tree, 
                 const float *qx, const float *qy, const float *qz, 
                 const float *r_min, const float *r_max, 
                 int num_queries, int *results, int *result_counts, 
                 int max_results_per_query);
```
- **Traversal Strategy:** Pushes overlapping children onto a local statically-sized stack (`MAX_STACK_DEPTH = 64`).
- **Returns:** The indices of the particles (relative to the original sorted arrays) and the total count found per query.

### `knn_query`
Finds the $k$-nearest neighbors to a given query point.

```cpp
void knn_query(sycl::queue &q, const TreeSoA &tree, 
               const float *qx, const float *qy, const float *qz, 
               int k, int num_queries, int *results, float *result_dists);
```
- **Traversal Strategy:** Maintains a custom, statically sized GPU `PriorityQueue` during tree traversal.
- **Optimization:** To quickly prune the search space, the traversal heuristically visits the child node whose bounding box is geometrically closer to the query point first.