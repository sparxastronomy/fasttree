#ifndef SYCL_FASTTREE_HLBVH_HPP
#define SYCL_FASTTREE_HLBVH_HPP

#include <sycl/sycl.hpp>
#include <algorithm>
#include <bit>
#include <cmath>
#include <execution>
#include <numeric>
#include <vector>

namespace fasttree {
// Using 21 bits per dimension allows us to fit 3D coordinates (float) into a 64-bit Morton key (63 bits total)
#define BITS_PER_DIM 21

// Particle structure
template <typename T>
struct particles {
  std::vector<T> pos_x, pos_y, pos_z;  // Positions
};

struct BoundingBox {
  float min_x, max_x;
  float min_y, max_y;
  float min_z, max_z;
};

// Utilities for Morton keys
static constexpr std::uint64_t MASK_21 = 0x1FFFFFULL;
static constexpr std::uint64_t M1 = 0x1F00000000FFFFULL;
static constexpr std::uint64_t M2 = 0x1F0000FF0000FFULL;
static constexpr std::uint64_t M3 = 0x100F00F00F00F00FULL;
static constexpr std::uint64_t M4 = 0x10C30C30C30C30C3ULL;
static constexpr std::uint64_t M5 = 0x1249249249249249ULL;

// Spread/compact 21-bit values into/out of 64-bit Morton layout (3D)
inline std::uint64_t spread3_u64(std::uint64_t x) {
  x &= MASK_21;
  x = (x | (x << 32)) & M1;
  x = (x | (x << 16)) & M2;
  x = (x | (x << 8)) & M3;
  x = (x | (x << 4)) & M4;
  x = (x | (x << 2)) & M5;
  return x;
}

inline std::uint64_t compact3_u64(std::uint64_t x) {
  x &= M5;
  x = (x | (x >> 2)) & M4;
  x = (x | (x >> 4)) & M3;
  x = (x | (x >> 8)) & M2;
  x = (x | (x >> 16)) & M1;
  x = (x | (x >> 32)) & MASK_21;
  return x;
}

inline int get_common_prefix_length(std::uint64_t c1, std::uint64_t c2) {
  if (c1 == c2) return 64;
#if defined(__GNUC__) || defined(__clang__)
  // leading common bits = number of leading zeros in xor
  return __builtin_clzll(c1 ^ c2);
#else
  // portable fallback
  std::uint64_t x = c1 ^ c2;
  int n = 0;
  for (int i = 63; i >= 0; --i) {
    if ((x >> i) & 1ULL) break;
    ++n;
  }
  return n;
#endif
}

uint64_t float_to_int(float f) {
  std::uint32_t bits = std::bit_cast<std::uint32_t>(f);
  std::uint64_t ix = (bits & 0x7FFFFFu) >> (23 - BITS_PER_DIM);
  return ix;
}

// Return indices that would sort the Morton keys
inline std::vector<size_t> sort_morton_keys(const std::vector<std::uint64_t> &morton_keys) {
  std::vector<size_t> indices(morton_keys.size());

  // Iota
  std::iota(indices.begin(), indices.end(), 0);

  // Sort by Morton keys
  std::sort(indices.begin(), indices.end(), [&morton_keys](size_t i1, size_t i2) { return morton_keys[i1] < morton_keys[i2]; });
  return indices;
}

// Tree structure in SoA format
struct TreeSoA {
  float *min_x, *max_x;
  float *min_y, *max_y;
  float *min_z, *max_z;
  int *left_child;
  int *right_child;
  int *parent;
  size_t num_leaves;
  size_t num_internal;

  // Constructor allocates memory for the tree nodes
  TreeSoA(sycl::queue &q, size_t n) : num_leaves(n), num_internal(n > 0 ? n - 1 : 0) {
    size_t total_nodes = num_leaves + num_internal;
    if (total_nodes == 0) return;

    min_x = sycl::malloc_shared<float>(total_nodes, q);
    max_x = sycl::malloc_shared<float>(total_nodes, q);
    min_y = sycl::malloc_shared<float>(total_nodes, q);
    max_y = sycl::malloc_shared<float>(total_nodes, q);
    min_z = sycl::malloc_shared<float>(total_nodes, q);
    max_z = sycl::malloc_shared<float>(total_nodes, q);
    left_child = sycl::malloc_shared<int>(num_internal, q);
    right_child = sycl::malloc_shared<int>(num_internal, q);
    parent = sycl::malloc_shared<int>(total_nodes, q);
  }

  // Free memory for the tree nodes
  void free(sycl::queue &q) {
    if (num_leaves + num_internal == 0) return;
    sycl::free(min_x, q);
    sycl::free(max_x, q);
    sycl::free(min_y, q);
    sycl::free(max_y, q);
    sycl::free(min_z, q);
    sycl::free(max_z, q);
    sycl::free(left_child, q);
    sycl::free(right_child, q);
    sycl::free(parent, q);
  }
};

inline int sgn(int x) { return (x > 0) - (x < 0); }

inline void build_tree(sycl::queue &q, TreeSoA &tree, const std::uint64_t *sorted_morton_keys, const float *sorted_x, const float *sorted_y,
                       const float *sorted_z) {
  size_t n = tree.num_leaves;
  if (n == 1) {
    q.submit([&](sycl::handler &cgh) {
       cgh.single_task([=]() {
         tree.min_x[0] = tree.max_x[0] = sorted_x[0];
         tree.min_y[0] = tree.max_y[0] = sorted_y[0];
         tree.min_z[0] = tree.max_z[0] = sorted_z[0];
       });
     }).wait();
    return;
  }

  float *p_min_x = tree.min_x, *p_max_x = tree.max_x;
  float *p_min_y = tree.min_y, *p_max_y = tree.max_y;
  float *p_min_z = tree.min_z, *p_max_z = tree.max_z;
  int *p_left_child = tree.left_child, *p_right_child = tree.right_child, *p_parent = tree.parent;

  if (!p_min_x || !p_parent || !p_left_child) return;

  // Initialize parents to -1
  q.fill(p_parent, -1, 2 * n - 1).wait();

  // 1. Construct internal nodes (Karras 2012)
  auto delta = [=](int i, int j) {
    if (j < 0 || j >= (int)n) return -1;
    std::uint64_t k_i = sorted_morton_keys[i];
    std::uint64_t k_j = sorted_morton_keys[j];
    if (k_i != k_j) return get_common_prefix_length(k_i, k_j);
    // Tie-breaker using indices as per Karras 2012
    return 64 + get_common_prefix_length((std::uint64_t)i, (std::uint64_t)j);
  };

  q.parallel_for(sycl::range<1>(n - 1), [=](sycl::id<1> idx) {
     int i = idx[0];

     // Determine direction of the range (+1 or -1)
     int d = sgn(delta(i, i + 1) - delta(i, i - 1));

     // Compute upper bound for the length of the range
     int delta_min = delta(i, i - d);
     int l_max = 2;
     while (delta(i, i + l_max * d) > delta_min) { l_max *= 2; }

     // Find the other end using binary search
     int l = 0;
     for (int t = l_max / 2; t >= 1; t /= 2) {
       if (delta(i, i + (l + t) * d) > delta_min) { l += t; }
     }
     int j = i + l * d;

     // Find the split point using binary search
     int delta_node = delta(i, j);
     int s = 0;
     int t_split = 1;
     while (t_split <= l) { t_split *= 2; }
     t_split /= 2;
     
     for (int t = t_split; t >= 1; t /= 2) {
       if (s + t < l && delta(i, i + (s + t) * d) > delta_node) {
         s += t;
       }
     }
     int split = i + s * d + std::min(d, 0);

     // Select children
     int left_idx = (std::min(i, j) == split) ? (split + n - 1) : split;
     int right_idx = (std::max(i, j) == split + 1) ? (split + 1 + n - 1) : (split + 1);

     p_left_child[i] = left_idx;
     p_right_child[i] = right_idx;
     p_parent[left_idx] = i;
     p_parent[right_idx] = i;
   }).wait();

  // 2. Initialize leaf bounding boxes
  q.parallel_for(sycl::range<1>(n), [=](sycl::id<1> idx) {
     int i = idx[0];
     int leaf_idx = i + n - 1;
     p_min_x[leaf_idx] = sorted_x[i];
     p_max_x[leaf_idx] = sorted_x[i];
     p_min_y[leaf_idx] = sorted_y[i];
     p_max_y[leaf_idx] = sorted_y[i];
     p_min_z[leaf_idx] = sorted_z[i];
     p_max_z[leaf_idx] = sorted_z[i];
   }).wait();

  // 3. Compute internal bounding boxes (bottom-up)
  int *counters = sycl::malloc_shared<int>(n - 1, q);
  if (!counters) return;
  q.fill(counters, 0, n - 1).wait();

  q.parallel_for(sycl::range<1>(n), [=](sycl::id<1> idx) {
     int curr = idx[0] + n - 1;  // Start from leaf
     while (curr != 0) {         // Until root (internal node 0)
       int p = p_parent[curr];
       if (p < 0 || p >= (int)n - 1) break;

       auto atomic_ref = sycl::atomic_ref<int,
                                          sycl::memory_order::seq_cst,  // Strictly sequence memory operations
                                          sycl::memory_scope::system,   // Force system-wide (cross-core) cache coherency
                                          sycl::access::address_space::global_space>(counters[p]);

       if (atomic_ref.fetch_add(1) == 0) return;  // First child to arrive

       // Second child arrived, compute BB using sycl::fmin/fmax
       int l = p_left_child[p];
       int r = p_right_child[p];
       p_min_x[p] = sycl::fmin(p_min_x[l], p_min_x[r]);
       p_max_x[p] = sycl::fmax(p_max_x[l], p_max_x[r]);
       p_min_y[p] = sycl::fmin(p_min_y[l], p_min_y[r]);
       p_max_y[p] = sycl::fmax(p_max_y[l], p_max_y[r]);
       p_min_z[p] = sycl::fmin(p_min_z[l], p_min_z[r]);
       p_max_z[p] = sycl::fmax(p_max_z[l], p_max_z[r]);
       curr = p;
     }
   }).wait();

  sycl::free(counters, q);
}

#define MAX_STACK_DEPTH 64

// Simple priority queue for kNN on GPU
template <typename T, int MAX_K>
struct PriorityQueue {
  T data[MAX_K];
  int indices[MAX_K];
  int count;

  PriorityQueue() : count(0) {}

  void push(T val, int idx, int k) {
    if (count < k) {
      data[count] = val;
      indices[count] = idx;
      count++;
      // Insertion sort (small K)
      for (int i = count - 1; i > 0; --i) {
        if (data[i] > data[i - 1]) {
          std::swap(data[i], data[i - 1]);
          std::swap(indices[i], indices[i - 1]);
        }
      }
    } else if (val < data[0]) {
      data[0] = val;
      indices[0] = idx;
      // Insertion sort
      for (int i = 0; i < k - 1; ++i) {
        if (data[i] < data[i + 1]) {
          std::swap(data[i], data[i + 1]);
          std::swap(indices[i], indices[i + 1]);
        } else
          break;
      }
    }
  }
};

inline void knn_query(sycl::queue &q, const TreeSoA &tree, const float *qx, const float *qy, const float *qz, int k, int num_queries, int *results,
                      float *result_dists) {
  size_t n = tree.num_leaves;
  if (n == 0) return;

  float *p_min_x = tree.min_x, *p_max_x = tree.max_x;
  float *p_min_y = tree.min_y, *p_max_y = tree.max_y;
  float *p_min_z = tree.min_z, *p_max_z = tree.max_z;
  int *p_left_child = tree.left_child, *p_right_child = tree.right_child;

  // Use a fixed K for the priority queue in the kernel
  // In a real implementation, K would be a template parameter or handled more dynamically
  q.parallel_for(sycl::range<1>(num_queries), [=](sycl::id<1> idx) {
    int qi = idx[0];
    float px = qx[qi], py = qy[qi], pz = qz[qi];

    int stack[MAX_STACK_DEPTH];
    int stack_ptr = 0;

    if (n == 1) {
      stack[stack_ptr++] = 0;
    } else {
      stack[stack_ptr++] = 0;
    }

    // We'll use a local buffer for PQ. Since K is dynamic in the API but fixed in the struct,
    // we handle it carefully. For this implementation, we assume K <= 32.
    PriorityQueue<float, 32> pq;

    while (stack_ptr > 0) {
      int node_idx = stack[--stack_ptr];

      float bmin_x = p_min_x[node_idx], bmax_x = p_max_x[node_idx];
      float bmin_y = p_min_y[node_idx], bmax_y = p_max_y[node_idx];
      float bmin_z = p_min_z[node_idx], bmax_z = p_max_z[node_idx];

      float dx = sycl::fmax(bmin_x - px, sycl::fmax(0.0f, px - bmax_x));
      float dy = sycl::fmax(bmin_y - py, sycl::fmax(0.0f, py - bmax_y));
      float dz = sycl::fmax(bmin_z - pz, sycl::fmax(0.0f, pz - bmax_z));
      float d2 = dx * dx + dy * dy + dz * dz;

      if (pq.count < k || d2 < pq.data[0]) {
        if (node_idx >= (int)n - 1 && n > 1) {  // Leaf
          pq.push(d2, node_idx - (n - 1), k);
        } else if (n == 1) {
          pq.push(d2, 0, k);
        } else {
          // Internal node
          int l = p_left_child[node_idx];
          int r = p_right_child[node_idx];

          // Heuristic: push the closer child last so it's processed first
          float l_dx = sycl::fmax(p_min_x[l] - px, sycl::fmax(0.0f, px - p_max_x[l]));
          float l_dy = sycl::fmax(p_min_y[l] - py, sycl::fmax(0.0f, py - p_max_y[l]));
          float l_dz = sycl::fmax(p_min_z[l] - pz, sycl::fmax(0.0f, pz - p_max_z[l]));
          float l_d2 = l_dx * l_dx + l_dy * l_dy + l_dz * l_dz;

          float r_dx = sycl::fmax(p_min_x[r] - px, sycl::fmax(0.0f, px - p_max_x[r]));
          float r_dy = sycl::fmax(p_min_y[r] - py, sycl::fmax(0.0f, py - p_max_y[r]));
          float r_dz = sycl::fmax(p_min_z[r] - pz, sycl::fmax(0.0f, pz - p_max_z[r]));
          float r_d2 = r_dx * r_dx + r_dy * r_dy + r_dz * r_dz;

          if (l_d2 < r_d2) {
            stack[stack_ptr++] = r;
            stack[stack_ptr++] = l;
          } else {
            stack[stack_ptr++] = l;
            stack[stack_ptr++] = r;
          }
        }
      }
    }

    for (int i = 0; i < k; ++i) {
      if (i < pq.count) {
        results[qi * k + (k - 1 - i)] = pq.indices[i];
        result_dists[qi * k + (k - 1 - i)] = std::sqrt(pq.data[i]);
      } else {
        results[qi * k + i] = -1;
        result_dists[qi * k + i] = INFINITY;
      }
    }
  });
}

#define MAX_STACK_DEPTH 64

inline void range_query(sycl::queue &q, const TreeSoA &tree, const float *qx, const float *qy, const float *qz, const float *r_min,
                        const float *r_max, int num_queries, int *results, int *result_counts, int max_results_per_query) {
  size_t n = tree.num_leaves;
  if (n == 0) return;

  float *p_min_x = tree.min_x, *p_max_x = tree.max_x;
  float *p_min_y = tree.min_y, *p_max_y = tree.max_y;
  float *p_min_z = tree.min_z, *p_max_z = tree.max_z;
  int *p_left_child = tree.left_child, *p_right_child = tree.right_child;

  q.parallel_for(sycl::range<1>(num_queries), [=](sycl::id<1> idx) {
    int qi = idx[0];
    float px = qx[qi], py = qy[qi], pz = qz[qi];
    float rm = r_min[qi], RM = r_max[qi];
    float RM2 = RM * RM;
    float rm2 = rm * rm;

    int stack[MAX_STACK_DEPTH];
    int stack_ptr = 0;

    if (n == 1) {
      stack[stack_ptr++] = 0;  // Only one leaf
    } else {
      stack[stack_ptr++] = 0;  // Root is internal node 0
    }

    int count = 0;
    while (stack_ptr > 0) {
      int node_idx = stack[--stack_ptr];

      float bmin_x = p_min_x[node_idx], bmax_x = p_max_x[node_idx];
      float bmin_y = p_min_y[node_idx], bmax_y = p_max_y[node_idx];
      float bmin_z = p_min_z[node_idx], bmax_z = p_max_z[node_idx];

      float dx = sycl::fmax(bmin_x - px, sycl::fmax(0.0f, px - bmax_x));
      float dy = sycl::fmax(bmin_y - py, sycl::fmax(0.0f, py - bmax_y));
      float dz = sycl::fmax(bmin_z - pz, sycl::fmax(0.0f, pz - bmax_z));
      float d2 = dx * dx + dy * dy + dz * dz;

      if (d2 <= RM2) {
        if (node_idx >= (int)n - 1 && n > 1) {  // Leaf node
          if (d2 >= rm2) {
            if (count < max_results_per_query) {
              results[qi * max_results_per_query + count] = node_idx - (n - 1);
              count++;
            }
          }
        } else if (n == 1) {  // Single leaf case
          if (d2 >= rm2) {
            if (count < max_results_per_query) {
              results[qi * max_results_per_query + count] = 0;
              count++;
            }
          }
        } else {
          // Internal node, push children
          stack[stack_ptr++] = p_right_child[node_idx];
          stack[stack_ptr++] = p_left_child[node_idx];
        }
      }
    }
    result_counts[qi] = count;
  });
}

inline void morton_encode(sycl::queue &q,                           // SYCL queue for offloading
                          const particles<float> &particles,        // Input particle data
                          std::vector<std::uint64_t> &morton_keys,  // USM compatible vector for output
                          const BoundingBox &bbox                   // Bounding box for normalization
) {
  size_t num_particles = particles.pos_x.size();
  const float *pos_x = particles.pos_x.data();
  const float *pos_y = particles.pos_y.data();
  const float *pos_z = particles.pos_z.data();
  std::uint64_t *keys = morton_keys.data();

  float dx = bbox.max_x - bbox.min_x;
  float dy = bbox.max_y - bbox.min_y;
  float dz = bbox.max_z - bbox.min_z;
  float inv_dx = (dx == 0) ? 0.0f : (1.0f / dx);
  float inv_dy = (dy == 0) ? 0.0f : (1.0f / dy);
  float inv_dz = (dz == 0) ? 0.0f : (1.0f / dz);

  q.parallel_for(sycl::range<1>(num_particles), [=](sycl::id<1> idx) {
    size_t i = idx[0];

    // Clamp the multiplier to 0.999999f to prevent 2.0f wraparound
    float nx = std::min((pos_x[i] - bbox.min_x) * inv_dx, 0.999999f);
    float ny = std::min((pos_y[i] - bbox.min_y) * inv_dy, 0.999999f);
    float nz = std::min((pos_z[i] - bbox.min_z) * inv_dz, 0.999999f);

    std::uint64_t ix = float_to_int(1.0f + nx);
    std::uint64_t iy = float_to_int(1.0f + ny);
    std::uint64_t iz = float_to_int(1.0f + nz);

    keys[i] = spread3_u64(ix) | (spread3_u64(iy) << 1) | (spread3_u64(iz) << 2);
  });
}

inline void morton_decode(sycl::queue &q, const std::vector<std::uint64_t> &morton_keys, const BoundingBox &bbox, particles<float> &particles) {
  size_t num_particles = morton_keys.size();
  // Make sure the output particle arrays are resized to hold the decoded positions
  particles.pos_x.resize(num_particles);
  particles.pos_y.resize(num_particles);
  particles.pos_z.resize(num_particles);

  const std::uint64_t *keys = morton_keys.data();
  float *pos_x = particles.pos_x.data();
  float *pos_y = particles.pos_y.data();
  float *pos_z = particles.pos_z.data();

  float dx = bbox.max_x - bbox.min_x;
  float dy = bbox.max_y - bbox.min_y;
  float dz = bbox.max_z - bbox.min_z;
  float scale = 1.0f / ((1ULL << BITS_PER_DIM) - 1);

  q.parallel_for(sycl::range<1>(num_particles), [=](sycl::id<1> idx) {
    size_t i = idx[0];
    std::uint64_t morton_key = keys[i];

    std::uint64_t ix = compact3_u64(morton_key);
    std::uint64_t iy = compact3_u64(morton_key >> 1);
    std::uint64_t iz = compact3_u64(morton_key >> 2);

    pos_x[i] = bbox.min_x + static_cast<float>(ix) * scale * dx;
    pos_y[i] = bbox.min_y + static_cast<float>(iy) * scale * dy;
    pos_z[i] = bbox.min_z + static_cast<float>(iz) * scale * dz;
  });
}

// High-level entry point to build the HLBVH
inline void build_bvh(sycl::queue &q, const particles<float> &p, TreeSoA &tree) {
  size_t n = p.pos_x.size();
  if (n == 0) return;

  // 1. Compute Bounding Box
  // For production, this should be done in parallel (sycl::reduce)
  BoundingBox bbox = {p.pos_x[0], p.pos_x[0], p.pos_y[0], p.pos_y[0], p.pos_z[0], p.pos_z[0]};
  for (size_t i = 1; i < n; ++i) {
    bbox.min_x = std::min(bbox.min_x, p.pos_x[i]);
    bbox.max_x = std::max(bbox.max_x, p.pos_x[i]);
    bbox.min_y = std::min(bbox.min_y, p.pos_y[i]);
    bbox.max_y = std::max(bbox.max_y, p.pos_y[i]);
    bbox.min_z = std::min(bbox.min_z, p.pos_z[i]);
    bbox.max_z = std::max(bbox.max_z, p.pos_z[i]);
  }

  // 2. Morton Encoding
  std::vector<std::uint64_t> morton_keys(n);
  morton_encode(q, p, morton_keys, bbox);
  q.wait();

  // 3. Sort Morton Keys
  auto sorted_indices = sort_morton_keys(morton_keys);

  // 4. Prepare sorted positions for tree building (re-order input)
  std::vector<float> sx(n), sy(n), sz(n);
  std::vector<std::uint64_t> smk(n);
  for (size_t i = 0; i < n; ++i) {
    sx[i] = p.pos_x[sorted_indices[i]];
    sy[i] = p.pos_y[sorted_indices[i]];
    sz[i] = p.pos_z[sorted_indices[i]];
    smk[i] = morton_keys[sorted_indices[i]];
  }

  // 5. Build Tree
  build_tree(q, tree, smk.data(), sx.data(), sy.data(), sz.data());
}

}  // namespace fasttree

#endif  // SYCL_FASTTREE_HLBVH_HPPREE_HLBVH_HPP