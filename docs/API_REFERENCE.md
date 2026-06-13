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
- **Leaves vs Internal Nodes:** The tree stores exactly $N$ leaves (representing the actual particles) and $N-1$ internal nodes (representing spatial bounding volumes that group leaves or other internal nodes).
  - **Leaf Nodes:** A leaf node has no children (`left_child` and `right_child` are invalid). Its bounding box `(min_x, min_y, min_z)` to `(max_x, max_y, max_z)` is degenerate and exactly equals the physical $(x, y, z)$ coordinate of the single particle it represents.
  - **Internal Nodes:** An internal node always has exactly two children (either two leaves, two internal nodes, or one of each). Its bounding box geometrically encloses all particles contained within its left and right sub-trees.
- **Indexing & The Flat Array:** Instead of allocating nodes individually in memory, the entire tree is flattened into contiguous arrays of size $2N - 1$.
  - Internal nodes are stored at indices from $0$ to $N-2$. The root of the entire tree is always at index $0$.
  - Leaves are stored continuously after the internal nodes, at indices from $N-1$ to $2N-2$.
- **Architectural Rationale:** 
  1. **GPU Performance (Coalesced Access):** Grouping all node data into contiguous arrays of floats and ints (Structure of Arrays) ensures that when a GPU warp traverses the tree, memory reads are coalesced, maximizing bandwidth utilization.
  2. **MPI Serialization:** Because relationships are defined by integer indices (e.g., `left_child[i] = 5`) rather than memory pointers (e.g., `Node* left`), the entire tree can be trivially copied and transmitted across the network via MPI raw byte transfers without needing complex serialization/deserialization logic.

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
- **Traversal Strategy:** Pushes overlapping children onto a local statically-sized stack (`MAX_STACK_DEPTH = 64`). By avoiding recursion, it prevents stack overflows and minimizes divergent thread execution on the GPU.
- **Returns:** The indices of the particles (relative to the original sorted arrays) and the total count found per query.
- **Algorithm & Pseudo-code:**
  The algorithm iterates through the tree using a stack. For each node, it calculates the minimum bounding box distance to the query point. If this distance is within the maximum search radius, the node is processed. For internal nodes, their children are pushed to the stack. For leaf nodes, the exact distance is checked against both the minimum and maximum radii to determine if the particle should be included in the results.

  ```text
  Initialize stack with the root node index
  Initialize count = 0
  
  while stack is not empty:
      node = pop(stack)
      dist_sq = min_squared_dist(query_point, node.bounding_box)
      
      if dist_sq <= max_radius_squared:
          if node is leaf:
              if dist_sq >= min_radius_squared and count < max_results:
                  add node.particle_index to results
                  increment count
          else:
              push(stack, node.right_child)
              push(stack, node.left_child)
  ```

### `PriorityQueue`
A simple, fixed-capacity priority queue designed specifically for GPU kernels.
- **Need & Usage:** Standard library dynamic memory containers (like `std::priority_queue`) cannot be used within SYCL device kernels due to dynamic allocation constraints. This custom `PriorityQueue` relies on a statically sized array. It maintains the $k$-nearest elements sorted by distance, replacing the furthest element when a closer one is found. Insertion sort is used internally as $k$ is expected to be small, keeping register usage low and execution fast.

### `knn_query`
Finds the $k$-nearest neighbors to a given query point.

```cpp
void knn_query(sycl::queue &q, const TreeSoA &tree, 
               const float *qx, const float *qy, const float *qz, 
               int k, int num_queries, int *results, float *result_dists);
```
- **Traversal Strategy:** Maintains a custom, statically sized GPU `PriorityQueue` during tree traversal.
- **Optimization:** To quickly prune the search space, the traversal heuristically visits the child node whose bounding box is geometrically closer to the query point first.
- **Algorithm & Pseudo-code:**
  The tree is traversed using a statically-sized stack, bounding the search using the $k$-th nearest neighbor's distance currently in the priority queue. If a node's bounding box is closer than the furthest known neighbor (or if fewer than $k$ neighbors have been found), the traversal continues into the node. For internal nodes, the distances to both children are calculated, and the closer child is pushed to the stack *last* so it gets popped and evaluated *first* (LIFO order).
  
  ```text
  Initialize stack with the root node index
  Initialize PriorityQueue pq of size k
  
  while stack is not empty:
      node = pop(stack)
      dist_sq = min_squared_dist(query_point, node.bounding_box)
      
      if pq.size < k or dist_sq < pq.max_distance:
          if node is leaf:
              pq.push(dist_sq, node.particle_index)
          else:
              left_dist = min_squared_dist(query_point, left_child.bounding_box)
              right_dist = min_squared_dist(query_point, right_child.bounding_box)
              
              if left_dist < right_dist:
                  push(stack, right_child)
                  push(stack, left_child)
              else:
                  push(stack, left_child)
                  push(stack, right_child)
                  
  Write pq contents to results array
  ```