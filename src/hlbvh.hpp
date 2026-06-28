#ifndef SYCL_FASTTREE_HLBVH_HPP
#define SYCL_FASTTREE_HLBVH_HPP

#include <oneapi/dpl/algorithm>
#include <oneapi/dpl/execution>
#include <oneapi/dpl/iterator>
#include <sycl/sycl.hpp>
#include <algorithm>
#include <cmath>
#include <execution>
#include <limits>
#include <numeric>
#include <vector>

namespace fasttree {

// Particle structure
template <typename T>
struct particles {
  std::vector<T> pos_x, pos_y, pos_z;  // Positions
  std::vector<T> mass;                 // Masses - Will be deprecated in future
  std::vector<uint32_t> id;            // IDs
  std::vector<int8_t> is_ghost;        // 0 = Local, 1 = Ghost
};

template <typename FloatT>
struct BoundingBox {
  FloatT min_x, max_x;
  FloatT min_y, max_y;
  FloatT min_z, max_z;
  // Constructor for static assert float T to eiter float or double
  BoundingBox(FloatT min_x_, FloatT max_x_, FloatT min_y_, FloatT max_y_, FloatT min_z_, FloatT max_z_)
      : min_x(min_x_), max_x(max_x_), min_y(min_y_), max_y(max_y_), min_z(min_z_), max_z(max_z_) {
    static_assert(std::is_same_v<FloatT, float> || std::is_same_v<FloatT, double>, "BoundingBox only supports float or double");
  }
};
}  // namespace fasttree

#if defined(SFC_TYPE_PEANO_HILBERT)
#include "sfc.peano_hilbert.hpp"
#elif defined(SFC_TYPE_MORTON)
#include "sfc.morton.hpp"
#else
#error "Undefined SFC_TYPE - either SFC_TYPE_PEANO_HILBERT or SFC_TYPE_MORTON must be defined"
#endif

namespace fasttree {

/**
 * Helper function to ensure a pointer is readable on the SYCL device.
 *
 * If the pointer is host-allocated, allocates temporary USM device memory,
 * copies the data to the device, and returns the device pointer.
 * Otherwise, returns the original pointer.
 *
 * @param[in] q SYCL queue to perform the copy operation.
 * @param[in] ptr Pointer to verify/copy.
 * @param[in] count Number of elements in the buffer.
 * @param[out] allocated Boolean flag indicating if new memory was allocated.
 * @return Device-accessible pointer.
 */
template <typename T>
inline T* ensure_device_readable(sycl::queue &q, const T* ptr, size_t count, bool &allocated) {
  if (!ptr || count == 0) {
    allocated = false;
    return nullptr;
  }
  auto type = sycl::get_pointer_type(ptr, q.get_context());
  if (type == sycl::usm::alloc::unknown) {
    T* dev_ptr = sycl::malloc_device<T>(count, q);
    q.copy(ptr, dev_ptr, count).wait();
    allocated = true;
    return dev_ptr;
  }
  allocated = false;
  return const_cast<T*>(ptr);
}

/**
 * Helper function to ensure a pointer is writable on the SYCL device.
 *
 * If the pointer is host-allocated, allocates temporary USM device memory,
 * and returns the device pointer.
 * Otherwise, returns the original pointer.
 *
 * @param[in] q SYCL queue.
 * @param[in] ptr Pointer to verify.
 * @param[in] count Number of elements in the buffer.
 * @param[out] allocated Boolean flag indicating if new memory was allocated.
 * @return Device-accessible pointer.
 */
template <typename T>
inline T* ensure_device_writable(sycl::queue &q, T* ptr, size_t count, bool &allocated) {
  if (!ptr || count == 0) {
    allocated = false;
    return nullptr;
  }
  auto type = sycl::get_pointer_type(ptr, q.get_context());
  if (type == sycl::usm::alloc::unknown) {
    T* dev_ptr = sycl::malloc_device<T>(count, q);
    allocated = true;
    return dev_ptr;
  }
  allocated = false;
  return ptr;
}

/**
 * Helper function to copy results back to a host pointer and free the temporary device memory.
 *
 * @param[in] q SYCL queue.
 * @param[in] dev_ptr Temporary device pointer.
 * @param[in] host_ptr Destination host pointer.
 * @param[in] count Number of elements to copy.
 * @param[in] allocated Boolean flag indicating if memory was allocated.
 */
template <typename T>
inline void copy_back_and_free(sycl::queue &q, T* dev_ptr, T* host_ptr, size_t count, bool allocated) {
  if (allocated && dev_ptr && host_ptr && count > 0) {
    q.copy(dev_ptr, host_ptr, count).wait();
    sycl::free(const_cast<void*>(static_cast<const void*>(dev_ptr)), q);
  }
}

/**
 * Helper function to free temporary device memory.
 *
 * @param[in] q SYCL queue.
 * @param[in] dev_ptr Temporary device pointer.
 * @param[in] allocated Boolean flag indicating if memory was allocated.
 */
template <typename T>
inline void free_device_readable(sycl::queue &q, T* dev_ptr, bool allocated) {
  if (allocated && dev_ptr) {
    sycl::free(const_cast<void*>(static_cast<const void*>(dev_ptr)), q);
  }
}

// Define the coordinate type
#if defined(COORDS_TYPE_FLOAT)
using coord_t = float;
#elif defined(COORDS_TYPE_DOUBLE)
using coord_t = double;
#else
#error "Undefined COORDS_TYPE - either COORDS_TYPE_FLOAT or COORDS_TYPE_DOUBLE must be defined"
#endif

/**
 * Traits for IEEE 754 floating point types (float, double).
 * Stores the number of mantissa bits and the mask for extracting the mantissa.
 * Undefined for non-floating point types to prevent misuse.
 */
template <typename FloatT>
struct ieee754_traits;

template <>
struct ieee754_traits<float> {
  using uint_type = uint32_t;
  static constexpr uint_type mantissa_mask = 0x007FFFFFu;  // 23 bits
  static constexpr int mantissa_bits = 23;
  // For f in [1,2): exponent field = 127 (0x7F), stored in bits 23..30
};

template <>
struct ieee754_traits<double> {
  using uint_type = uint64_t;
  static constexpr uint_type mantissa_mask = 0x000FFFFFFFFFFFFFull;  // 52 bits
  static constexpr int mantissa_bits = 52;
  // For d in [1,2): exponent field = 1023 (0x3FF), stored in bits 52..62
};

/**
 *  Generic verstion of domain_double_to_int from Arepo/Gadget
 *
 *  @param val  Any floating point input
 *  @return integer representation of the input value
 *
 *  @note
 *    sycl::bit_cast is valid on device for both float→uint32_t
 *    and double→uint64_t. No union punning needed.
 *
 *  @note
 *    float  has 23 mantissa bits → max useful BITS_PER_DIMENSION = 23
 *    double has 52 mantissa bits → max useful BITS_PER_DIMENSION = 21 (our limit)
 *    For BITS_PER_DIMENSION <= 21 both types have sufficient precision.
 */
template <typename FloatT>
inline sfc1D encode_to_sfc1d(FloatT val) noexcept {
  // Compile-time guard: reject types other than float/double
  static_assert(std::is_same_v<FloatT, float> || std::is_same_v<FloatT, double>, "encode_to_sfc1d only supports float or double");

  // Compile-time guard: ensure the floating point type has enough
  // mantissa bits to represent BITS_PER_DIMENSION distinct levels
  static_assert(BITS_PER_DIMENSION <= ieee754_traits<FloatT>::mantissa_bits,
                "BITS_PER_DIMENSION exceeds the mantissa precision of FloatT. "
                "Use double for BITS_PER_DIMENSION > 23.");

  using traits = ieee754_traits<FloatT>;
  using uint_t = typename traits::uint_type;

  // Reinterpret bits: well-defined via sycl::bit_cast (C++20 std::bit_cast)
  const uint_t bits = sycl::bit_cast<uint_t>(val);

  // Shift right to keep only the top BITS_PER_DIMENSION mantissa bits
  constexpr int shift = traits::mantissa_bits - BITS_PER_DIMENSION;

  return static_cast<sfc1D>((bits & traits::mantissa_mask) >> shift);
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

template <typename FloatT>
inline void sfc_encode(sycl::queue &q, const FloatT *pos_x, const FloatT *pos_y, const FloatT *pos_z, size_t num_particles, sfc_key *keys, const BoundingBox<FloatT> &bbox) {
  if (num_particles == 0) return;

  bool x_alloc = false, y_alloc = false, z_alloc = false, keys_alloc = false;
  const FloatT *dev_pos_x = ensure_device_readable(q, pos_x, num_particles, x_alloc);
  const FloatT *dev_pos_y = ensure_device_readable(q, pos_y, num_particles, y_alloc);
  const FloatT *dev_pos_z = ensure_device_readable(q, pos_z, num_particles, z_alloc);
  sfc_key *dev_keys = ensure_device_writable(q, keys, num_particles, keys_alloc);

  FloatT dx = bbox.max_x - bbox.min_x;
  FloatT dy = bbox.max_y - bbox.min_y;
  FloatT dz = bbox.max_z - bbox.min_z;
  FloatT inv_dx = (dx == 0) ? static_cast<FloatT>(0.0) : (static_cast<FloatT>(1.0) / dx);
  FloatT inv_dy = (dy == 0) ? static_cast<FloatT>(0.0) : (static_cast<FloatT>(1.0) / dy);
  FloatT inv_dz = (dz == 0) ? static_cast<FloatT>(0.0) : (static_cast<FloatT>(1.0) / dz);

  q.parallel_for(sycl::range<1>(num_particles), [=](sycl::id<1> idx) {
    size_t i = idx[0];

    FloatT nx = std::min((dev_pos_x[i] - bbox.min_x) * inv_dx, static_cast<FloatT>(0.999999));
    FloatT ny = std::min((dev_pos_y[i] - bbox.min_y) * inv_dy, static_cast<FloatT>(0.999999));
    FloatT nz = std::min((dev_pos_z[i] - bbox.min_z) * inv_dz, static_cast<FloatT>(0.999999));

    sfc1D ix = encode_to_sfc1d(static_cast<FloatT>(1.0) + nx);
    sfc1D iy = encode_to_sfc1d(static_cast<FloatT>(1.0) + ny);
    sfc1D iz = encode_to_sfc1d(static_cast<FloatT>(1.0) + nz);

#if defined(SFC_TYPE_PEANO_HILBERT)
    dev_keys[i] = sfc_encode3D(ix, iy, iz);
#elif defined(SFC_TYPE_MORTON)
    dev_keys[i] = spread3_u64(ix) | (spread3_u64(iy) << 1) | (spread3_u64(iz) << 2);
#endif
  }).wait();

  free_device_readable(q, dev_pos_x, x_alloc);
  free_device_readable(q, dev_pos_y, y_alloc);
  free_device_readable(q, dev_pos_z, z_alloc);
  copy_back_and_free(q, dev_keys, keys, num_particles, keys_alloc);
}

template <typename FloatT>
inline void sfc_encode(sycl::queue &q, const particles<FloatT> &particles, sfc_key *keys, const BoundingBox<FloatT> &bbox) {
  sfc_encode(q, particles.pos_x.data(), particles.pos_y.data(), particles.pos_z.data(), particles.pos_x.size(), keys, bbox);
}

/**
 * Computes the bounding box of a set of particles in parallel using SYCL.
 *
 * @param q SYCL queue
 * @param pos_x Pointer to the x-coordinates of the particles
 * @param pos_y Pointer to the y-coordinates of the particles
 * @param pos_z Pointer to the z-coordinates of the particles
 * @param n Number of particles
 * @return BoundingBox containing the min and max coordinates in each dimension
 */
template <typename FloatT>
inline BoundingBox<FloatT> compute_bbox(sycl::queue &q, const FloatT *pos_x, const FloatT *pos_y, const FloatT *pos_z, size_t n) {
  if (n == 0) {
    return BoundingBox<FloatT>(std::numeric_limits<FloatT>::max(), -std::numeric_limits<FloatT>::max(),
                               std::numeric_limits<FloatT>::max(), -std::numeric_limits<FloatT>::max(),
                               std::numeric_limits<FloatT>::max(), -std::numeric_limits<FloatT>::max());
  }

  bool x_alloc = false, y_alloc = false, z_alloc = false;
  const FloatT *dev_pos_x = ensure_device_readable(q, pos_x, n, x_alloc);
  const FloatT *dev_pos_y = ensure_device_readable(q, pos_y, n, y_alloc);
  const FloatT *dev_pos_z = ensure_device_readable(q, pos_z, n, z_alloc);

  FloatT *d_min_x = sycl::malloc_shared<FloatT>(1, q);
  FloatT *d_max_x = sycl::malloc_shared<FloatT>(1, q);
  FloatT *d_min_y = sycl::malloc_shared<FloatT>(1, q);
  FloatT *d_max_y = sycl::malloc_shared<FloatT>(1, q);
  FloatT *d_min_z = sycl::malloc_shared<FloatT>(1, q);
  FloatT *d_max_z = sycl::malloc_shared<FloatT>(1, q);

  // Initialize shared memory on host with identity values for reductions
  d_min_x[0] = std::numeric_limits<FloatT>::max();
  d_max_x[0] = -std::numeric_limits<FloatT>::max();
  d_min_y[0] = std::numeric_limits<FloatT>::max();
  d_max_y[0] = -std::numeric_limits<FloatT>::max();
  d_min_z[0] = std::numeric_limits<FloatT>::max();
  d_max_z[0] = -std::numeric_limits<FloatT>::max();

  q.submit([&](sycl::handler &h) {
     h.parallel_for(sycl::range<1>(n), sycl::reduction(d_min_x, std::numeric_limits<FloatT>::max(), sycl::minimum<FloatT>()),
                    sycl::reduction(d_max_x, -std::numeric_limits<FloatT>::max(), sycl::maximum<FloatT>()),
                    sycl::reduction(d_min_y, std::numeric_limits<FloatT>::max(), sycl::minimum<FloatT>()),
                    sycl::reduction(d_max_y, -std::numeric_limits<FloatT>::max(), sycl::maximum<FloatT>()),
                    sycl::reduction(d_min_z, std::numeric_limits<FloatT>::max(), sycl::minimum<FloatT>()),
                    sycl::reduction(d_max_z, -std::numeric_limits<FloatT>::max(), sycl::maximum<FloatT>()),
                    [=](sycl::id<1> idx, auto &r_min_x, auto &r_max_x, auto &r_min_y, auto &r_max_y, auto &r_min_z, auto &r_max_z) {
                      size_t i = idx[0];
                      r_min_x.combine(dev_pos_x[i]);
                      r_max_x.combine(dev_pos_x[i]);
                      r_min_y.combine(dev_pos_y[i]);
                      r_max_y.combine(dev_pos_y[i]);
                      r_min_z.combine(dev_pos_z[i]);
                      r_max_z.combine(dev_pos_z[i]);
                    });
   }).wait();

  BoundingBox<FloatT> bbox = {d_min_x[0], d_max_x[0], d_min_y[0], d_max_y[0], d_min_z[0], d_max_z[0]};

  sycl::free(d_min_x, q);
  sycl::free(d_max_x, q);
  sycl::free(d_min_y, q);
  sycl::free(d_max_y, q);
  sycl::free(d_min_z, q);
  sycl::free(d_max_z, q);

  free_device_readable(q, dev_pos_x, x_alloc);
  free_device_readable(q, dev_pos_y, y_alloc);
  free_device_readable(q, dev_pos_z, z_alloc);

  return bbox;
}

// Overload for Particles struct
template <typename FloatT>
inline BoundingBox<FloatT> compute_bbox(sycl::queue &q, const particles<FloatT> &p, size_t n) {
  return compute_bbox(q, p.pos_x.data(), p.pos_y.data(), p.pos_z.data(), n);
}

// Tree structure in SoA format
struct TreeSoA {
  // TODO: Reorder in decreasing order of size for better memory coalescing
  coord_t *min_x, *max_x;
  coord_t *min_y, *max_y;
  coord_t *min_z, *max_z;
  int *left_child;
  int *right_child;
  int *parent;
  uint32_t *id;
  int8_t *is_ghost;
  size_t num_leaves;
  size_t num_internal;

  // Constructor allocates memory for the tree nodes
  TreeSoA(sycl::queue &q, size_t n) : num_leaves(n), num_internal(n > 0 ? n - 1 : 0), id(nullptr), is_ghost(nullptr) {
    size_t total_nodes = num_leaves + num_internal;
    if (total_nodes == 0) return;

    min_x = sycl::malloc_shared<coord_t>(total_nodes, q);
    max_x = sycl::malloc_shared<coord_t>(total_nodes, q);
    min_y = sycl::malloc_shared<coord_t>(total_nodes, q);
    max_y = sycl::malloc_shared<coord_t>(total_nodes, q);
    min_z = sycl::malloc_shared<coord_t>(total_nodes, q);
    max_z = sycl::malloc_shared<coord_t>(total_nodes, q);
    left_child = sycl::malloc_shared<int>(num_internal, q);
    right_child = sycl::malloc_shared<int>(num_internal, q);
    parent = sycl::malloc_shared<int>(total_nodes, q);
    id = sycl::malloc_shared<uint32_t>(total_nodes, q);
    is_ghost = sycl::malloc_shared<int8_t>(total_nodes, q);
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
    if (id) sycl::free(id, q);
    if (is_ghost) sycl::free(is_ghost, q);
  }
};

inline int sgn(int x) { return (x > 0) - (x < 0); }

inline void build_tree(sycl::queue &q, TreeSoA &tree, const sfc_key *sorted_keys, const coord_t *sorted_x, const coord_t *sorted_y,
                       const coord_t *sorted_z, const uint32_t *sorted_id = nullptr, const int8_t *sorted_is_ghost = nullptr) {
  size_t n = tree.num_leaves;
  if (n == 0) return;

  bool keys_alloc = false, x_alloc = false, y_alloc = false, z_alloc = false;
  bool id_alloc = false, ghost_alloc = false;

  const sfc_key *dev_keys = ensure_device_readable(q, sorted_keys, n, keys_alloc);
  const coord_t *dev_x = ensure_device_readable(q, sorted_x, n, x_alloc);
  const coord_t *dev_y = ensure_device_readable(q, sorted_y, n, y_alloc);
  const coord_t *dev_z = ensure_device_readable(q, sorted_z, n, z_alloc);
  const uint32_t *dev_id = sorted_id ? ensure_device_readable(q, sorted_id, n, id_alloc) : nullptr;
  const int8_t *dev_ghost = sorted_is_ghost ? ensure_device_readable(q, sorted_is_ghost, n, ghost_alloc) : nullptr;

  if (n == 1) {
    q.submit([&](sycl::handler &cgh) {
       cgh.single_task([=]() {
         tree.min_x[0] = tree.max_x[0] = dev_x[0];
         tree.min_y[0] = tree.max_y[0] = dev_y[0];
         tree.min_z[0] = tree.max_z[0] = dev_z[0];
       });
     }).wait();
     
     free_device_readable(q, dev_keys, keys_alloc);
     free_device_readable(q, dev_x, x_alloc);
     free_device_readable(q, dev_y, y_alloc);
     free_device_readable(q, dev_z, z_alloc);
     if (dev_id) free_device_readable(q, dev_id, id_alloc);
     if (dev_ghost) free_device_readable(q, dev_ghost, ghost_alloc);
     return;
  }

  coord_t *p_min_x = tree.min_x, *p_max_x = tree.max_x;
  coord_t *p_min_y = tree.min_y, *p_max_y = tree.max_y;
  coord_t *p_min_z = tree.min_z, *p_max_z = tree.max_z;
  int *p_left_child = tree.left_child, *p_right_child = tree.right_child, *p_parent = tree.parent;

  if (!p_min_x || !p_parent || !p_left_child) {
     free_device_readable(q, dev_keys, keys_alloc);
     free_device_readable(q, dev_x, x_alloc);
     free_device_readable(q, dev_y, y_alloc);
     free_device_readable(q, dev_z, z_alloc);
     if (dev_id) free_device_readable(q, dev_id, id_alloc);
     if (dev_ghost) free_device_readable(q, dev_ghost, ghost_alloc);
     return;
  }

  // Initialize parents to -1
  q.fill(p_parent, -1, 2 * n - 1).wait();

  // 1. Construct internal nodes (Karras 2012)
  auto delta = [=](int i, int j) {
    if (j < 0 || j >= (int)n) return -1;
    std::uint64_t k_i = dev_keys[i];
    std::uint64_t k_j = dev_keys[j];
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
       if (s + t < l && delta(i, i + (s + t) * d) > delta_node) { s += t; }
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
  uint32_t *p_id = tree.id;
  int8_t *p_ghost = tree.is_ghost;
  q.parallel_for(sycl::range<1>(n), [=](sycl::id<1> idx) {
     int i = idx[0];
     int leaf_idx = i + n - 1;
     p_min_x[leaf_idx] = dev_x[i];
     p_max_x[leaf_idx] = dev_x[i];
     p_min_y[leaf_idx] = dev_y[i];
     p_max_y[leaf_idx] = dev_y[i];
     p_min_z[leaf_idx] = dev_z[i];
     p_max_z[leaf_idx] = dev_z[i];
     if (p_id && dev_id) { p_id[leaf_idx] = dev_id[i]; }
     if (p_ghost && dev_ghost) { p_ghost[leaf_idx] = dev_ghost[i]; }
   }).wait();

  // 3. Compute internal bounding boxes (bottom-up)
  int *counters = sycl::malloc_shared<int>(n - 1, q);
  if (!counters) {
     free_device_readable(q, dev_keys, keys_alloc);
     free_device_readable(q, dev_x, x_alloc);
     free_device_readable(q, dev_y, y_alloc);
     free_device_readable(q, dev_z, z_alloc);
     if (dev_id) free_device_readable(q, dev_id, id_alloc);
     if (dev_ghost) free_device_readable(q, dev_ghost, ghost_alloc);
     return;
  }
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

  free_device_readable(q, dev_keys, keys_alloc);
  free_device_readable(q, dev_x, x_alloc);
  free_device_readable(q, dev_y, y_alloc);
  free_device_readable(q, dev_z, z_alloc);
  if (dev_id) free_device_readable(q, dev_id, id_alloc);
  if (dev_ghost) free_device_readable(q, dev_ghost, ghost_alloc);
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
        } else {
          break;
        }
      }
    }
  }
};

inline void knn_query(sycl::queue &q, const TreeSoA &tree, const coord_t *qx, const coord_t *qy, const coord_t *qz, int k, int num_queries,
                      int *results, coord_t *result_dists) {
  size_t n = tree.num_leaves;
  if (n == 0 || num_queries == 0) return;

  bool qx_alloc = false, qy_alloc = false, qz_alloc = false;
  bool res_alloc = false, dist_alloc = false;

  const coord_t *dev_qx = ensure_device_readable(q, qx, num_queries, qx_alloc);
  const coord_t *dev_qy = ensure_device_readable(q, qy, num_queries, qy_alloc);
  const coord_t *dev_qz = ensure_device_readable(q, qz, num_queries, qz_alloc);
  int *dev_results = ensure_device_writable(q, results, num_queries * k, res_alloc);
  coord_t *dev_result_dists = ensure_device_writable(q, result_dists, num_queries * k, dist_alloc);

  coord_t *p_min_x = tree.min_x, *p_max_x = tree.max_x;
  coord_t *p_min_y = tree.min_y, *p_max_y = tree.max_y;
  coord_t *p_min_z = tree.min_z, *p_max_z = tree.max_z;
  int *p_left_child = tree.left_child, *p_right_child = tree.right_child;

  // Use a fixed K for the priority queue in the kernel
  // In a real implementation, K would be a template parameter or handled more dynamically
  q.parallel_for(sycl::range<1>(num_queries), [=](sycl::id<1> idx) {
    int qi = idx[0];
    coord_t px = dev_qx[qi], py = dev_qy[qi], pz = dev_qz[qi];

    int stack[MAX_STACK_DEPTH];
    int stack_ptr = 0;

    if (n == 1) {
      stack[stack_ptr++] = 0;
    } else {
      stack[stack_ptr++] = 0;
    }

    // We'll use a local buffer for PQ. Since K is dynamic in the API but fixed in the struct,
    // we handle it carefully. For this implementation, we assume K <= 128.
    PriorityQueue<coord_t, 128> pq;

    while (stack_ptr > 0) {
      int node_idx = stack[--stack_ptr];

      coord_t bmin_x = p_min_x[node_idx], bmax_x = p_max_x[node_idx];
      coord_t bmin_y = p_min_y[node_idx], bmax_y = p_max_y[node_idx];
      coord_t bmin_z = p_min_z[node_idx], bmax_z = p_max_z[node_idx];

      coord_t dx = sycl::fmax(bmin_x - px, sycl::fmax(coord_t(0.0), px - bmax_x));
      coord_t dy = sycl::fmax(bmin_y - py, sycl::fmax(coord_t(0.0), py - bmax_y));
      coord_t dz = sycl::fmax(bmin_z - pz, sycl::fmax(coord_t(0.0), pz - bmax_z));
      coord_t d2 = dx * dx + dy * dy + dz * dz;

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
          coord_t l_dx = sycl::fmax(p_min_x[l] - px, sycl::fmax(coord_t(0.0), px - p_max_x[l]));
          coord_t l_dy = sycl::fmax(p_min_y[l] - py, sycl::fmax(coord_t(0.0), py - p_max_y[l]));
          coord_t l_dz = sycl::fmax(p_min_z[l] - pz, sycl::fmax(coord_t(0.0), pz - p_max_z[l]));
          coord_t l_d2 = l_dx * l_dx + l_dy * l_dy + l_dz * l_dz;

          coord_t r_dx = sycl::fmax(p_min_x[r] - px, sycl::fmax(coord_t(0.0), px - p_max_x[r]));
          coord_t r_dy = sycl::fmax(p_min_y[r] - py, sycl::fmax(coord_t(0.0), py - p_max_y[r]));
          coord_t r_dz = sycl::fmax(p_min_z[r] - pz, sycl::fmax(coord_t(0.0), pz - p_max_z[r]));
          coord_t r_d2 = r_dx * r_dx + r_dy * r_dy + r_dz * r_dz;

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
        dev_results[qi * k + (k - 1 - i)] = pq.indices[i];
        dev_result_dists[qi * k + (k - 1 - i)] = std::sqrt(pq.data[i]);
      } else {
        dev_results[qi * k + i] = -1;
        dev_result_dists[qi * k + i] = INFINITY;
      }
    }
  }).wait();

  free_device_readable(q, dev_qx, qx_alloc);
  free_device_readable(q, dev_qy, qy_alloc);
  free_device_readable(q, dev_qz, qz_alloc);
  copy_back_and_free(q, dev_results, results, num_queries * k, res_alloc);
  copy_back_and_free(q, dev_result_dists, result_dists, num_queries * k, dist_alloc);
}

#define MAX_STACK_DEPTH 64

inline void range_query(sycl::queue &q, const TreeSoA &tree, const coord_t *qx, const coord_t *qy, const coord_t *qz, const coord_t *r_min,
                        const coord_t *r_max, int num_queries, int *results, int *result_counts, int max_results_per_query) {
  size_t n = tree.num_leaves;
  if (n == 0 || num_queries == 0) return;

  bool qx_alloc = false, qy_alloc = false, qz_alloc = false;
  bool r_min_alloc = false, r_max_alloc = false;
  bool res_alloc = false, counts_alloc = false;

  const coord_t *dev_qx = ensure_device_readable(q, qx, num_queries, qx_alloc);
  const coord_t *dev_qy = ensure_device_readable(q, qy, num_queries, qy_alloc);
  const coord_t *dev_qz = ensure_device_readable(q, qz, num_queries, qz_alloc);
  const coord_t *dev_r_min = ensure_device_readable(q, r_min, num_queries, r_min_alloc);
  const coord_t *dev_r_max = ensure_device_readable(q, r_max, num_queries, r_max_alloc);
  int *dev_results = ensure_device_writable(q, results, num_queries * max_results_per_query, res_alloc);
  int *dev_result_counts = ensure_device_writable(q, result_counts, num_queries, counts_alloc);

  coord_t *p_min_x = tree.min_x, *p_max_x = tree.max_x;
  coord_t *p_min_y = tree.min_y, *p_max_y = tree.max_y;
  coord_t *p_min_z = tree.min_z, *p_max_z = tree.max_z;
  int *p_left_child = tree.left_child, *p_right_child = tree.right_child;

  q.parallel_for(sycl::range<1>(num_queries), [=](sycl::id<1> idx) {
    int qi = idx[0];
    coord_t px = dev_qx[qi], py = dev_qy[qi], pz = dev_qz[qi];
    coord_t rm = dev_r_min[qi], RM = dev_r_max[qi];
    coord_t RM2 = RM * RM;
    coord_t rm2 = rm * rm;

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

      coord_t bmin_x = p_min_x[node_idx], bmax_x = p_max_x[node_idx];
      coord_t bmin_y = p_min_y[node_idx], bmax_y = p_max_y[node_idx];
      coord_t bmin_z = p_min_z[node_idx], bmax_z = p_max_z[node_idx];

      coord_t dx = sycl::fmax(bmin_x - px, sycl::fmax(coord_t(0.0), px - bmax_x));
      coord_t dy = sycl::fmax(bmin_y - py, sycl::fmax(coord_t(0.0), py - bmax_y));
      coord_t dz = sycl::fmax(bmin_z - pz, sycl::fmax(coord_t(0.0), pz - bmax_z));
      coord_t d2 = dx * dx + dy * dy + dz * dz;

      if (d2 <= RM2) {
        if (node_idx >= (int)n - 1 && n > 1) {  // Leaf node
          if (d2 >= rm2) {
            if (count < max_results_per_query) {
              dev_results[qi * max_results_per_query + count] = node_idx - (n - 1);
            }
            count++;
          }
        } else if (n == 1) {  // Single leaf case
          if (d2 >= rm2) {
            if (count < max_results_per_query) {
              dev_results[qi * max_results_per_query + count] = 0;
            }
            count++;
          }
        } else {
          // Internal node, push children
          stack[stack_ptr++] = p_right_child[node_idx];
          stack[stack_ptr++] = p_left_child[node_idx];
        }
      }
    }
    dev_result_counts[qi] = count;
  }).wait();

  free_device_readable(q, dev_qx, qx_alloc);
  free_device_readable(q, dev_qy, qy_alloc);
  free_device_readable(q, dev_qz, qz_alloc);
  free_device_readable(q, dev_r_min, r_min_alloc);
  free_device_readable(q, dev_r_max, r_max_alloc);
  copy_back_and_free(q, dev_results, results, num_queries * max_results_per_query, res_alloc);
  copy_back_and_free(q, dev_result_counts, result_counts, num_queries, counts_alloc);
}

// High-level entry point to build the HLBVH
inline void build_bvh(sycl::queue &q, const particles<coord_t> &p, TreeSoA &tree) {
  size_t n = p.pos_x.size();
  if (n == 0) return;

  // 1. Compute Bounding Box using parallel GPU reduction
  BoundingBox bbox = compute_bbox(q, p, n);

  // 2. Allocate USM memory for keys and indices
  sfc_key *d_smk = sycl::malloc_shared<sfc_key>(n, q);
  size_t *d_indices = sycl::malloc_shared<size_t>(n, q);

  // 3. SFC Encoding and Index Initialization
  sfc_encode(q, p, d_smk, bbox);
  q.parallel_for(sycl::range<1>(n), [=](sycl::id<1> idx) { d_indices[idx] = idx[0]; });
  q.wait();

  // 4. Single-Pass Full GPU Sort using oneDPL
  auto policy = oneapi::dpl::execution::make_device_policy(q);
  auto zip_begin = oneapi::dpl::make_zip_iterator(d_smk, d_indices);
  auto zip_end = zip_begin + n;

  oneapi::dpl::sort(policy, zip_begin, zip_end, [](auto a, auto b) { return std::get<0>(a) < std::get<0>(b); });
  q.wait();

  // 5. Coordinate and Attribute Reordering on GPU
  coord_t *sx = sycl::malloc_shared<coord_t>(n, q);
  coord_t *sy = sycl::malloc_shared<coord_t>(n, q);
  coord_t *sz = sycl::malloc_shared<coord_t>(n, q);
  uint32_t *sid = sycl::malloc_shared<uint32_t>(n, q);
  int8_t *sghost = sycl::malloc_shared<int8_t>(n, q);

  const coord_t *pos_x = p.pos_x.data();
  const coord_t *pos_y = p.pos_y.data();
  const coord_t *pos_z = p.pos_z.data();

  const uint32_t *p_id = nullptr;
  std::vector<uint32_t> temp_id;
  if (p.id.empty()) {
    temp_id.resize(n);
    std::iota(temp_id.begin(), temp_id.end(), 0u);
    p_id = temp_id.data();
  } else {
    p_id = p.id.data();
  }

  const int8_t *p_ghost = nullptr;
  std::vector<int8_t> temp_ghost;
  if (p.is_ghost.empty()) {
    temp_ghost.resize(n, 0);
    p_ghost = temp_ghost.data();
  } else {
    p_ghost = p.is_ghost.data();
  }

  bool x_alloc = false, y_alloc = false, z_alloc = false;
  bool id_alloc = false, ghost_alloc = false;
  const coord_t *dev_pos_x = ensure_device_readable(q, pos_x, n, x_alloc);
  const coord_t *dev_pos_y = ensure_device_readable(q, pos_y, n, y_alloc);
  const coord_t *dev_pos_z = ensure_device_readable(q, pos_z, n, z_alloc);
  const uint32_t *dev_p_id = ensure_device_readable(q, p_id, n, id_alloc);
  const int8_t *dev_p_ghost = ensure_device_readable(q, p_ghost, n, ghost_alloc);

  q.parallel_for(sycl::range<1>(n), [=](sycl::id<1> idx) {
     size_t i = idx[0];
     size_t orig_idx = d_indices[i];
     sx[i] = dev_pos_x[orig_idx];
     sy[i] = dev_pos_y[orig_idx];
     sz[i] = dev_pos_z[orig_idx];
     sid[i] = dev_p_id[orig_idx];
     sghost[i] = dev_p_ghost[orig_idx];
   }).wait();

  // 6. Build Tree
  build_tree(q, tree, d_smk, sx, sy, sz, sid, sghost);

  // Cleanup
  free_device_readable(q, dev_pos_x, x_alloc);
  free_device_readable(q, dev_pos_y, y_alloc);
  free_device_readable(q, dev_pos_z, z_alloc);
  free_device_readable(q, dev_p_id, id_alloc);
  free_device_readable(q, dev_p_ghost, ghost_alloc);

  sycl::free(d_smk, q);
  sycl::free(d_indices, q);
  sycl::free(sx, q);
  sycl::free(sy, q);
  sycl::free(sz, q);
  sycl::free(sid, q);
  sycl::free(sghost, q);
}

}  // namespace fasttree

#endif  // SYCL_FASTTREE_HLBVH_HPPREE_HLBVH_HPP