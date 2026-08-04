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

/**
 * @brief Structure of Arrays (SoA) container for particle properties.
 *
 * This struct represents a set of particles in SoA format to optimize memory
 * coalescing and cache utilization during GPU execution.
 *
 * @tparam T Floating-point precision (float or double) for spatial coordinates.
 */
template <typename T>
struct particles {
  /// Local/ghost particle positions in x, y, and z dimensions.
  std::vector<T> pos_x, pos_y, pos_z;
  /// Unique 32-bit identifier for tracking particles during spatial sorting and domain decomposition.
  std::vector<uint32_t> id;
  /// Ghost particle flag: 0 for locally owned particles, 1 for boundary ghost particles.
  std::vector<int8_t> is_ghost;
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

template <typename T>
inline T type_identity_min() {
  if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
    return std::numeric_limits<T>::max();
  } else if constexpr (std::is_same_v<T, uint32_t>) {
    return 0xFFFFFFFFu;
  } else if constexpr (std::is_same_v<T, uint64_t>) {
    return 0xFFFFFFFFFFFFFFFFull;
  } else {  // uint128_t
    return uint128_t(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull);
  }
}

template <typename T>
inline T type_identity_max() {
  if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
    return -std::numeric_limits<T>::max();
  } else if constexpr (std::is_same_v<T, uint32_t>) {
    return 0u;
  } else if constexpr (std::is_same_v<T, uint64_t>) {
    return 0ull;
  } else {  // uint128_t
    return uint128_t(0ull, 0ull);
  }
}

/**
 * @brief Represents a 3D axis-aligned bounding box.
 *
 * Geometrically bounds a collection of particles or nodes in three dimensions.
 *
 * @tparam FloatT Floating-point precision (float or double).
 */
template <typename FloatT>
struct BoundingBox {
  FloatT min_x, max_x;  ///< Bounds in x-dimension
  FloatT min_y, max_y;  ///< Bounds in y-dimension
  FloatT min_z, max_z;  ///< Bounds in z-dimension

  /**
   * @brief Construct a new Bounding Box.
   *
   * @param min_x_ Minimum value in x.
   * @param max_x_ Maximum value in x.
   * @param min_y_ Minimum value in y.
   * @param max_y_ Maximum value in y.
   * @param min_z_ Minimum value in z.
   * @param max_z_ Maximum value in z.
   */
  BoundingBox(FloatT min_x_, FloatT max_x_, FloatT min_y_, FloatT max_y_, FloatT min_z_, FloatT max_z_)
      : min_x(min_x_), max_x(max_x_), min_y(min_y_), max_y(max_y_), min_z(min_z_), max_z(max_z_) {
    static_assert(std::is_same_v<FloatT, float> || std::is_same_v<FloatT, double> || std::is_same_v<FloatT, uint32_t> ||
                      std::is_same_v<FloatT, uint64_t> || std::is_same_v<FloatT, uint128_t>,
                  "BoundingBox supports float, double, uint32_t, uint64_t, and uint128_t");
  }

  // Overload, initialize with std::numeric_limits for empty bounding box
  BoundingBox()
      : min_x(type_identity_min<FloatT>()),
        max_x(type_identity_max<FloatT>()),
        min_y(type_identity_min<FloatT>()),
        max_y(type_identity_max<FloatT>()),
        min_z(type_identity_min<FloatT>()),
        max_z(type_identity_max<FloatT>()) {}
};

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
inline T *ensure_device_readable(sycl::queue &q, const T *ptr, size_t count, bool &allocated) {
  if (!ptr || count == 0) {
    allocated = false;
    return nullptr;
  }
  auto type = sycl::get_pointer_type(ptr, q.get_context());
  if (type == sycl::usm::alloc::unknown) {
    T *dev_ptr = sycl::malloc_device<T>(count, q);
    q.copy(ptr, dev_ptr, count).wait();
    allocated = true;
    return dev_ptr;
  }
  allocated = false;
  return const_cast<T *>(ptr);
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
inline T *ensure_device_writable(sycl::queue &q, T *ptr, size_t count, bool &allocated) {
  if (!ptr || count == 0) {
    allocated = false;
    return nullptr;
  }
  auto type = sycl::get_pointer_type(ptr, q.get_context());
  if (type == sycl::usm::alloc::unknown) {
    T *dev_ptr = sycl::malloc_device<T>(count, q);
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
inline void copy_back_and_free(sycl::queue &q, T *dev_ptr, T *host_ptr, size_t count, bool allocated) {
  if (allocated && dev_ptr && host_ptr && count > 0) {
    q.copy(dev_ptr, host_ptr, count).wait();
    sycl::free(const_cast<void *>(static_cast<const void *>(dev_ptr)), q);
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
inline void free_device_readable(sycl::queue &q, T *dev_ptr, bool allocated) {
  if (allocated && dev_ptr) { sycl::free(const_cast<void *>(static_cast<const void *>(dev_ptr)), q); }
}

// Define the coordinate type
#if defined(FASTTREE_INTEGER_COORDS)
using coord_t = MyIntPosType;
#elif defined(COORDS_TYPE_DOUBLE)
using coord_t = double;
#else
using coord_t = float;
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
 * @brief Generic version of domain_double_to_int from Arepo/Gadget.
 *
 * Encodes a normalized floating-point coordinate val (expected to be in [1.0, 2.0))
 * into an integer representation by extracting its top mantissa bits.
 *
 * @tparam FloatT Floating-point precision (float or double).
 * @param[in] val Normalized coordinate value in [1.0, 2.0).
 * @return sfc1D Integer representation of the coordinates.
 *
 * @note sycl::bit_cast is valid on device for float -> uint32_t and double -> uint64_t.
 * @note Single-precision float has 23 mantissa bits; double has 52 mantissa bits.
 *       For BITS_PER_DIMENSION <= 21, both types provide sufficient precision.
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

/**
 * @brief Computes the number of leading common bits between two 64-bit keys.
 *
 * Used during Karras tree construction to find common prefix lengths delta(i, j)
 * which dictate tree hierarchy/split points.
 *
 * @param[in] c1 First 64-bit space-filling curve key.
 * @param[in] c2 Second 64-bit space-filling curve key.
 * @return int Number of leading common bits (0 to 64).
 */
inline int get_common_prefix_length(std::uint64_t c1, std::uint64_t c2) {
  if (c1 == c2) return 64;
  uint32_t hi = static_cast<uint32_t>((c1 ^ c2) >> 32);
  uint32_t lo = static_cast<uint32_t>(c1 ^ c2);
  if (hi != 0) return sycl::clz(hi);
  return 32 + sycl::clz(lo);
}

#if defined(SFC_TYPE_PEANO_HILBERT)
inline int get_common_prefix_length(const sfc_key &k1, const sfc_key &k2) {
  if (k1 == k2) return KEY_TOTAL_BITS;

  auto count_clz = [](auto val) -> int {
    using T = decltype(val);
    if constexpr (std::is_same_v<T, uint32_t>) {
      return sycl::clz(val);
    } else if constexpr (std::is_same_v<T, uint64_t>) {
      uint32_t hi = static_cast<uint32_t>(val >> 32);
      uint32_t lo = static_cast<uint32_t>(val);
      if (hi != 0) return sycl::clz(hi);
      return 32 + sycl::clz(lo);
    } else if constexpr (std::is_same_v<T, uint128_t>) {
      if (val.hi != 0) {
        uint32_t hi32 = static_cast<uint32_t>(val.hi >> 32);
        uint32_t lo32 = static_cast<uint32_t>(val.hi);
        if (hi32 != 0) return sycl::clz(hi32);
        return 32 + sycl::clz(lo32);
      }
      uint32_t hi32 = static_cast<uint32_t>(val.lo >> 32);
      uint32_t lo32 = static_cast<uint32_t>(val.lo);
      if (hi32 != 0) return 64 + sycl::clz(hi32);
      return 96 + sycl::clz(lo32);
    }
    return 0;
  };

  constexpr int bits_in_hs = (KEY_TOTAL_BITS > 2 * BITS_FOR_POSITIONS) ? (KEY_TOTAL_BITS - 2 * BITS_FOR_POSITIONS) : 0;
  constexpr int unused_hs = BITS_FOR_POSITIONS - bits_in_hs;

  if (k1.hs != k2.hs) { return count_clz(k1.hs ^ k2.hs) - unused_hs; }

  constexpr int bits_above_ls = KEY_TOTAL_BITS - BITS_FOR_POSITIONS;
  constexpr int bits_in_is = (bits_above_ls > 0) ? ((bits_above_ls > BITS_FOR_POSITIONS) ? BITS_FOR_POSITIONS : bits_above_ls) : 0;
  constexpr int unused_is = BITS_FOR_POSITIONS - bits_in_is;

  if (k1.is != k2.is) { return bits_in_hs + count_clz(k1.is ^ k2.is) - unused_is; }

  constexpr int bits_in_ls = KEY_TOTAL_BITS - bits_in_hs - bits_in_is;
  constexpr int unused_ls = BITS_FOR_POSITIONS - bits_in_ls;

  if (k1.ls != k2.ls) { return bits_in_hs + bits_in_is + count_clz(k1.ls ^ k2.ls) - unused_ls; }
  return KEY_TOTAL_BITS;
}
#endif

/**
 * @brief Encodes particle coordinates into 3D Space-Filling Curve (SFC) keys.
 *
 * Quantizes positions relative to a bounding box and encodes them into either
 * 3D Morton keys or 3D Peano-Hilbert keys depending on the SFC_TYPE compilation flag.
 *
 * @tparam FloatT Floating-point precision (float or double).
 * @param[in] q SYCL queue to submit the parallel encoding kernel.
 * @param[in] pos_x Pointer to local particle x-coordinates.
 * @param[in] pos_y Pointer to local particle y-coordinates.
 * @param[in] pos_z Pointer to local particle z-coordinates.
 * @param[in] num_particles Number of particles to encode.
 * @param[out] keys Pointer to the output sfc_key array.
 * @param[in] bbox Coordinate bounding box for normalization.
 */
template <typename FloatT>
inline void sfc_encode(sycl::queue &q, const FloatT *pos_x, const FloatT *pos_y, const FloatT *pos_z, size_t num_particles, sfc_key *keys,
                       const BoundingBox<FloatT> &bbox) {
  if (num_particles == 0) return;

  bool x_alloc = false, y_alloc = false, z_alloc = false, keys_alloc = false;
  const FloatT *dev_pos_x = ensure_device_readable(q, pos_x, num_particles, x_alloc);
  const FloatT *dev_pos_y = ensure_device_readable(q, pos_y, num_particles, y_alloc);
  const FloatT *dev_pos_z = ensure_device_readable(q, pos_z, num_particles, z_alloc);
  sfc_key *dev_keys = ensure_device_writable(q, keys, num_particles, keys_alloc);

#if defined(FASTTREE_INTEGER_COORDS)
  q.parallel_for(sycl::range<1>(num_particles), [=](sycl::id<1> idx) {
     size_t i = idx[0];
     sfc1D ix = static_cast<sfc1D>(dev_pos_x[i]);
     sfc1D iy = static_cast<sfc1D>(dev_pos_y[i]);
     sfc1D iz = static_cast<sfc1D>(dev_pos_z[i]);
#if defined(SFC_TYPE_PEANO_HILBERT)
     dev_keys[i] = sfc_encode3D(ix, iy, iz);
#elif defined(SFC_TYPE_MORTON)
     dev_keys[i] = spread3_u64(ix) | (spread3_u64(iy) << 1) | (spread3_u64(iz) << 2);
#endif
   }).wait();
#else
  FloatT dx = bbox.max_x - bbox.min_x;
  FloatT dy = bbox.max_y - bbox.min_y;
  FloatT dz = bbox.max_z - bbox.min_z;
  FloatT inv_dx = (dx == 0) ? static_cast<FloatT>(0.0) : (static_cast<FloatT>(1.0) / dx);
  FloatT inv_dy = (dy == 0) ? static_cast<FloatT>(0.0) : (static_cast<FloatT>(1.0) / dy);
  FloatT inv_dz = (dz == 0) ? static_cast<FloatT>(0.0) : (static_cast<FloatT>(1.0) / dz);
  FloatT clamp_upper = std::nextafter(static_cast<FloatT>(1.0), static_cast<FloatT>(0.0));
  FloatT clamp_lower = static_cast<FloatT>(0.0);

  q.parallel_for(sycl::range<1>(num_particles), [=](sycl::id<1> idx) {
     size_t i = idx[0];

     FloatT nx = sycl::clamp((dev_pos_x[i] - bbox.min_x) * inv_dx, clamp_lower, clamp_upper);
     FloatT ny = sycl::clamp((dev_pos_y[i] - bbox.min_y) * inv_dy, clamp_lower, clamp_upper);
     FloatT nz = sycl::clamp((dev_pos_z[i] - bbox.min_z) * inv_dz, clamp_lower, clamp_upper);

#if defined(SFC_TYPE_PEANO_HILBERT)
     sfc1D ix = quantize_coord(nx);
     sfc1D iy = quantize_coord(ny);
     sfc1D iz = quantize_coord(nz);
     dev_keys[i] = sfc_encode3D(ix, iy, iz);
#elif defined(SFC_TYPE_MORTON)
     sfc1D ix = encode_to_sfc1d(static_cast<FloatT>(1.0) + nx);
     sfc1D iy = encode_to_sfc1d(static_cast<FloatT>(1.0) + ny);
     sfc1D iz = encode_to_sfc1d(static_cast<FloatT>(1.0) + nz);
     dev_keys[i] = spread3_u64(ix) | (spread3_u64(iy) << 1) | (spread3_u64(iz) << 2);
#endif
   }).wait();
#endif

  free_device_readable(q, dev_pos_x, x_alloc);
  free_device_readable(q, dev_pos_y, y_alloc);
  free_device_readable(q, dev_pos_z, z_alloc);
  copy_back_and_free(q, dev_keys, keys, num_particles, keys_alloc);
}

/**
 * @brief Overload of sfc_encode accepting a particles SoA struct.
 *
 * @tparam FloatT Floating-point precision (float or double).
 * @param[in] q SYCL queue.
 * @param[in] particles The input particle dataset in SoA format.
 * @param[out] keys Pointer to the output sfc_key array.
 * @param[in] bbox Coordinate bounding box for normalization.
 */
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
    return BoundingBox<FloatT>(type_identity_min<FloatT>(), type_identity_max<FloatT>(), type_identity_min<FloatT>(), type_identity_max<FloatT>(),
                               type_identity_min<FloatT>(), type_identity_max<FloatT>());
  }

  bool x_alloc = false, y_alloc = false, z_alloc = false;
  const FloatT *dev_pos_x = ensure_device_readable(q, pos_x, n, x_alloc);
  const FloatT *dev_pos_y = ensure_device_readable(q, pos_y, n, y_alloc);
  const FloatT *dev_pos_z = ensure_device_readable(q, pos_z, n, z_alloc);

  FloatT *d_bbox_reduction = sycl::malloc_shared<FloatT>(6, q);

  // Initialize shared memory on host with identity values for reductions
  d_bbox_reduction[0] = type_identity_min<FloatT>();
  d_bbox_reduction[1] = type_identity_max<FloatT>();
  d_bbox_reduction[2] = type_identity_min<FloatT>();
  d_bbox_reduction[3] = type_identity_max<FloatT>();
  d_bbox_reduction[4] = type_identity_min<FloatT>();
  d_bbox_reduction[5] = type_identity_max<FloatT>();

  if constexpr (std::is_same_v<FloatT, float> || std::is_same_v<FloatT, double> || std::is_same_v<FloatT, uint32_t> ||
                std::is_same_v<FloatT, uint64_t>) {
    q.submit([&](sycl::handler &h) {
       h.parallel_for(sycl::range<1>(n), sycl::reduction(d_bbox_reduction + 0, type_identity_min<FloatT>(), sycl::minimum<FloatT>()),
                      sycl::reduction(d_bbox_reduction + 1, type_identity_max<FloatT>(), sycl::maximum<FloatT>()),
                      sycl::reduction(d_bbox_reduction + 2, type_identity_min<FloatT>(), sycl::minimum<FloatT>()),
                      sycl::reduction(d_bbox_reduction + 3, type_identity_max<FloatT>(), sycl::maximum<FloatT>()),
                      sycl::reduction(d_bbox_reduction + 4, type_identity_min<FloatT>(), sycl::minimum<FloatT>()),
                      sycl::reduction(d_bbox_reduction + 5, type_identity_max<FloatT>(), sycl::maximum<FloatT>()),
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
  } else {
    q.single_task([=]() {
       FloatT min_x = dev_pos_x[0], max_x = dev_pos_x[0];
       FloatT min_y = dev_pos_y[0], max_y = dev_pos_y[0];
       FloatT min_z = dev_pos_z[0], max_z = dev_pos_z[0];
       for (size_t i = 1; i < n; ++i) {
         if (dev_pos_x[i] < min_x) min_x = dev_pos_x[i];
         if (max_x < dev_pos_x[i]) max_x = dev_pos_x[i];
         if (dev_pos_y[i] < min_y) min_y = dev_pos_y[i];
         if (max_y < dev_pos_y[i]) max_y = dev_pos_y[i];
         if (dev_pos_z[i] < min_z) min_z = dev_pos_z[i];
         if (max_z < dev_pos_z[i]) max_z = dev_pos_z[i];
       }
       d_bbox_reduction[0] = min_x;
       d_bbox_reduction[1] = max_x;
       d_bbox_reduction[2] = min_y;
       d_bbox_reduction[3] = max_y;
       d_bbox_reduction[4] = min_z;
       d_bbox_reduction[5] = max_z;
     }).wait();
  }

  BoundingBox<FloatT> bbox = {d_bbox_reduction[0], d_bbox_reduction[1], d_bbox_reduction[2],
                              d_bbox_reduction[3], d_bbox_reduction[4], d_bbox_reduction[5]};

  sycl::free(d_bbox_reduction, q);

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

template <typename T>
inline T sfc_min(T a, T b) {
  if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
    return sycl::fmin(a, b);
  } else {
    return (a < b) ? a : b;
  }
}

template <typename T>
inline T sfc_max(T a, T b) {
  if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
    return sycl::fmax(a, b);
  } else {
    return (a < b) ? b : a;
  }
}

/**
 * @brief Flattened, pointer-free bounding volume hierarchy in SoA format.
 *
 * Adheres to a Structure of Arrays (SoA) layout to allow memory-coalesced accesses.
 * Since parent/child links are integer indices, the tree contains no raw pointers
 * and can be trivially copied and sent across the network via MPI.
 *
 * Flat Array Indexing Layout:
 * - The root node is stored at index 0.
 * - Internal nodes are stored at indices [0, num_internal - 1].
 * - Leaves (actual particles) are stored at indices [num_internal, num_leaves + num_internal - 1]
 *   which is equivalent to [n - 1, 2n - 2].
 */
struct TreeSoA {
  coord_t *min_x, *max_x;  ///< Node minimum and maximum x-coordinates. Size: num_leaves + num_internal.
  coord_t *min_y, *max_y;  ///< Node minimum and maximum y-coordinates. Size: num_leaves + num_internal.
  coord_t *min_z, *max_z;  ///< Node minimum and maximum z-coordinates. Size: num_leaves + num_internal.
  int *left_child;         ///< Left child index. Size: num_internal.
  int *right_child;        ///< Right child index. Size: num_internal.
  int *parent;             ///< Parent node index. Size: num_leaves + num_internal.
  uint32_t *id;            ///< Particle IDs (e.g. AREPO unique IDs). Size: num_leaves + num_internal.
  int8_t *is_ghost;        ///< Flags for local vs ghost particles. Size: num_leaves + num_internal.
  int *orig_idx;           ///< Original input array index of sorted particles. Size: num_leaves + num_internal.
  size_t num_leaves;       ///< Total leaf nodes (equivalent to particle count).
  size_t num_internal;     ///< Total internal nodes (equal to num_leaves - 1).

  /**
   * @brief Construct a TreeSoA, allocating USM shared memory.
   *
   * @param[in] q SYCL queue used for allocating USM memory.
   * @param[in] n Number of leaf nodes (particles) in the tree.
   */
  TreeSoA(sycl::queue &q, size_t n) : num_leaves(n), num_internal(n > 0 ? n - 1 : 0), id(nullptr), is_ghost(nullptr), orig_idx(nullptr) {
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
    orig_idx = sycl::malloc_shared<int>(total_nodes, q);
  }

  /**
   * @brief Releases all allocated USM memory for this tree.
   *
   * @param[in] q SYCL queue.
   */
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
    if (orig_idx) sycl::free(orig_idx, q);
  }
};

/**
 * @brief Returns the sign of an integer.
 *
 * @param[in] x The integer value.
 * @return int 1 if positive, -1 if negative, 0 if zero.
 */
inline int sgn(int x) { return (x > 0) - (x < 0); }

/**
 * @brief Generates the pointer-free binary tree topology and computes bounding boxes.
 *
 * Implements the Karras (2012) parallel radix tree builder to determine parent-child links.
 * Then runs a bottom-up parallel reduction to compute bounding boxes for all internal nodes
 * using seq_cst memory ordering for atomic-based parent traversal.
 *
 * @param[in] q SYCL queue for kernel execution.
 * @param[in,out] tree The TreeSoA to populate.
 * @param[in] sorted_keys Sorted Space-Filling Curve keys for the particles.
 * @param[in] sorted_x Sorted x-coordinates.
 * @param[in] sorted_y Sorted y-coordinates.
 * @param[in] sorted_z Sorted z-coordinates.
 * @param[in] sorted_id Sorted unique particle identifiers (optional).
 * @param[in] sorted_is_ghost Sorted ghost particle flags (optional).
 */
inline void build_tree(sycl::queue &q, TreeSoA &tree, const sfc_key *sorted_keys, const coord_t *sorted_x = nullptr,
                       const coord_t *sorted_y = nullptr, const coord_t *sorted_z = nullptr, const uint32_t *sorted_id = nullptr,
                       const int8_t *sorted_is_ghost = nullptr, const int *sorted_orig_idx = nullptr) {
  size_t n = tree.num_leaves;
  if (n == 0) return;

  bool keys_alloc = false, x_alloc = false, y_alloc = false, z_alloc = false;
  bool id_alloc = false, ghost_alloc = false, orig_alloc = false;

  const sfc_key *dev_keys = ensure_device_readable(q, sorted_keys, n, keys_alloc);
  const coord_t *dev_x = sorted_x ? ensure_device_readable(q, sorted_x, n, x_alloc) : nullptr;
  const coord_t *dev_y = sorted_y ? ensure_device_readable(q, sorted_y, n, y_alloc) : nullptr;
  const coord_t *dev_z = sorted_z ? ensure_device_readable(q, sorted_z, n, z_alloc) : nullptr;
  const uint32_t *dev_id = sorted_id ? ensure_device_readable(q, sorted_id, n, id_alloc) : nullptr;
  const int8_t *dev_ghost = sorted_is_ghost ? ensure_device_readable(q, sorted_is_ghost, n, ghost_alloc) : nullptr;
  const int *dev_orig_idx = sorted_orig_idx ? ensure_device_readable(q, sorted_orig_idx, n, orig_alloc) : nullptr;

  if (n == 1) {
    uint32_t *p_id = tree.id;
    int8_t *p_ghost = tree.is_ghost;
    int *p_orig_idx = tree.orig_idx;
    q.submit([&](sycl::handler &cgh) {
       cgh.single_task([=]() {
         if (dev_x) {
           tree.min_x[0] = tree.max_x[0] = dev_x[0];
           tree.min_y[0] = tree.max_y[0] = dev_y[0];
           tree.min_z[0] = tree.max_z[0] = dev_z[0];
         }
         if (p_id && dev_id) { p_id[0] = dev_id[0]; }
         if (p_ghost && dev_ghost) { p_ghost[0] = dev_ghost[0]; }
         if (p_orig_idx && dev_orig_idx) { p_orig_idx[0] = dev_orig_idx[0]; }
       });
     }).wait();

    free_device_readable(q, dev_keys, keys_alloc);
    if (dev_x) {
      free_device_readable(q, dev_x, x_alloc);
      free_device_readable(q, dev_y, y_alloc);
      free_device_readable(q, dev_z, z_alloc);
    }
    if (dev_id) free_device_readable(q, dev_id, id_alloc);
    if (dev_ghost) free_device_readable(q, dev_ghost, ghost_alloc);
    if (dev_orig_idx) free_device_readable(q, dev_orig_idx, orig_alloc);
    return;
  }

  coord_t *p_min_x = tree.min_x, *p_max_x = tree.max_x;
  coord_t *p_min_y = tree.min_y, *p_max_y = tree.max_y;
  coord_t *p_min_z = tree.min_z, *p_max_z = tree.max_z;
  int *p_left_child = tree.left_child, *p_right_child = tree.right_child, *p_parent = tree.parent;

  if (!p_min_x || !p_parent || !p_left_child) {
    free_device_readable(q, dev_keys, keys_alloc);
    if (dev_x && dev_y && dev_z) {
      free_device_readable(q, dev_x, x_alloc);
      free_device_readable(q, dev_y, y_alloc);
      free_device_readable(q, dev_z, z_alloc);
    }
    if (dev_id) free_device_readable(q, dev_id, id_alloc);
    if (dev_ghost) free_device_readable(q, dev_ghost, ghost_alloc);
    if (dev_orig_idx) free_device_readable(q, dev_orig_idx, orig_alloc);
    return;
  }

  // Initialize parents to -1
  q.fill(p_parent, -1, 2 * n - 1).wait();

  // 1. Construct internal nodes (Karras 2012)
  auto delta = [=](int i, int j) {
    if (j < 0 || static_cast<size_t>(j) >= n) return -1;
    sfc_key k_i = dev_keys[i];
    sfc_key k_j = dev_keys[j];
    if (k_i != k_j) return get_common_prefix_length(k_i, k_j);
    // Tie-breaker using indices as per Karras 2012
    return KEY_TOTAL_BITS + get_common_prefix_length((std::uint64_t)i, (std::uint64_t)j);
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
  if (dev_x && dev_y && dev_z) {
    uint32_t *p_id = tree.id;
    int8_t *p_ghost = tree.is_ghost;
    int *p_orig_idx = tree.orig_idx;
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
       if (p_orig_idx && dev_orig_idx) { p_orig_idx[leaf_idx] = dev_orig_idx[i]; }
     }).wait();
  }

  // 3. Compute internal bounding boxes (bottom-up)
  int *counters = sycl::malloc_shared<int>(n - 1, q);
  if (!counters) {
    free_device_readable(q, dev_keys, keys_alloc);
    if (dev_x && dev_y && dev_z) {
      free_device_readable(q, dev_x, x_alloc);
      free_device_readable(q, dev_y, y_alloc);
      free_device_readable(q, dev_z, z_alloc);
    }
    if (dev_id) free_device_readable(q, dev_id, id_alloc);
    if (dev_ghost) free_device_readable(q, dev_ghost, ghost_alloc);
    if (dev_orig_idx) free_device_readable(q, dev_orig_idx, orig_alloc);
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

       // Fence: ensure all writes to child BBs are visible before reading them
       sycl::atomic_fence(sycl::memory_order::acquire, sycl::memory_scope::device);

       // Second child arrived, compute BB using sfc_min/sfc_max
       int l = p_left_child[p];
       int r = p_right_child[p];
       p_min_x[p] = sfc_min(p_min_x[l], p_min_x[r]);
       p_max_x[p] = sfc_max(p_max_x[l], p_max_x[r]);
       p_min_y[p] = sfc_min(p_min_y[l], p_min_y[r]);
       p_max_y[p] = sfc_max(p_max_y[l], p_max_y[r]);
       p_min_z[p] = sfc_min(p_min_z[l], p_min_z[r]);
       p_max_z[p] = sfc_max(p_max_z[l], p_max_z[r]);
       curr = p;
     }
   }).wait();

  sycl::free(counters, q);

  free_device_readable(q, dev_keys, keys_alloc);
  if (dev_x && dev_y && dev_z) {
    free_device_readable(q, dev_x, x_alloc);
    free_device_readable(q, dev_y, y_alloc);
    free_device_readable(q, dev_z, z_alloc);
  }
  if (dev_id) free_device_readable(q, dev_id, id_alloc);
  if (dev_ghost) free_device_readable(q, dev_ghost, ghost_alloc);
  if (dev_orig_idx) free_device_readable(q, dev_orig_idx, orig_alloc);
}

#define MAX_STACK_DEPTH 64

/**
 * @brief A fixed-capacity priority queue designed for GPU device kernels.
 *
 * Since dynamic allocations are prohibited inside SYCL GPU kernels, this queue
 * maintains the k-nearest particles sorted by distance using a statically sized array.
 * Uses insertion sort for fast execution with small K values, minimizing register usage.
 *
 * @tparam T Coordinate value precision (float or double).
 * @tparam MAX_K Maximum capacity of the priority queue.
 */
template <typename T, int MAX_K>
struct PriorityQueue {
  T data[MAX_K];       ///< Squared distance values.
  int indices[MAX_K];  ///< Particle indices corresponding to distances.
  int count;           ///< Number of valid items in the queue.

  /**
   * @brief Construct an empty PriorityQueue.
   */
  PriorityQueue() : count(0) {}

  /**
   * @brief Inserts a new element if it is closer than the furthest known element.
   *
   * @param[in] val Squared distance value to insert.
   * @param[in] idx Particle index.
   * @param[in] k Active number of nearest neighbors requested (k <= MAX_K).
   */
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

/**
 * @brief Computes the minimum squared distance between a 3D query point and an axis-aligned bounding box.
 *
 * @tparam T Coordinate type (float, double, or integer position type).
 * @param[in] px Query point x-coordinate.
 * @param[in] py Query point y-coordinate.
 * @param[in] pz Query point z-coordinate.
 * @param[in] bmin_x Box minimum x.
 * @param[in] bmax_x Box maximum x.
 * @param[in] bmin_y Box minimum y.
 * @param[in] bmax_y Box maximum y.
 * @param[in] bmin_z Box minimum z.
 * @param[in] bmax_z Box maximum z.
 * @return Squared distance from the query point to the closest point on the bounding box.
 * @note In integer coordinate mode (FASTTREE_INTEGER_COORDS), coordinates are converted to normalized doubles in [0, 1)
 *       via int_rep_to_float() before computing differences, returning normalized squared distance in [0, 3].
 */
template <typename T>
inline auto node_distance_sq(T px, T py, T pz, T bmin_x, T bmax_x, T bmin_y, T bmax_y, T bmin_z, T bmax_z) {
  if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
    T dx = sycl::fmax(bmin_x - px, sycl::fmax(T(0.0), px - bmax_x));
    T dy = sycl::fmax(bmin_y - py, sycl::fmax(T(0.0), py - bmax_y));
    T dz = sycl::fmax(bmin_z - pz, sycl::fmax(T(0.0), pz - bmax_z));
    return dx * dx + dy * dy + dz * dz;
  } else {
    // Step 1: Map integer coordinates to normalized double space [0, 1)
    auto diff = [](T val, T bmin, T bmax) -> double {
      double val_f = int_rep_to_float(val);
      double bmin_f = int_rep_to_float(bmin);
      double bmax_f = int_rep_to_float(bmax);
      if (val_f < bmin_f) return bmin_f - val_f;
      if (val_f > bmax_f) return val_f - bmax_f;
      return 0.0;
    };
    // Step 2: Compute Euclidean coordinate differences in normalized space
    double dx = diff(px, bmin_x, bmax_x);
    double dy = diff(py, bmin_y, bmax_y);
    double dz = diff(pz, bmin_z, bmax_z);
    return dx * dx + dy * dy + dz * dz;
  }
}

/**
 * @brief Finds the k-nearest neighbor particles for multiple query points.
 *
 * Executes a parallel non-recursive stack-based tree traversal. To prune the search
 * space, it visits the geometrically closer child node first and updates search bounds
 * using a local PriorityQueue.
 *
 * @param[in] q SYCL queue to run the query kernel.
 * @param[in] tree The built spatial tree.
 * @param[in] qx Pointer to query point x-coordinates.
 * @param[in] qy Pointer to query point y-coordinates.
 * @param[in] qz Pointer to query point z-coordinates.
 * @param[in] k Number of nearest neighbors to find.
 * @param[in] num_queries Total number of query points.
 * @param[out] results Output buffer for nearest neighbor indices. Size: num_queries * k.
 * @param[out] result_dists Output buffer for neighbor Euclidean distances. Size: num_queries * k.
 */
template <int _MAX_K_ = 128>
inline void knn_query(sycl::queue &q, const TreeSoA &tree, const coord_t *qx, const coord_t *qy, const coord_t *qz, int k, int num_queries,
                      size_t *results, coord_t *result_dists) {
  size_t n = tree.num_leaves;
  if (n == 0 || num_queries == 0) return;

  bool qx_alloc = false, qy_alloc = false, qz_alloc = false;
  bool res_alloc = false, dist_alloc = false;

  const coord_t *dev_qx = ensure_device_readable(q, qx, num_queries, qx_alloc);
  const coord_t *dev_qy = ensure_device_readable(q, qy, num_queries, qy_alloc);
  const coord_t *dev_qz = ensure_device_readable(q, qz, num_queries, qz_alloc);
  size_t *dev_results = ensure_device_writable(q, results, static_cast<size_t>(num_queries) * static_cast<size_t>(k), res_alloc);
  coord_t *dev_result_dists = ensure_device_writable(q, result_dists, static_cast<size_t>(num_queries) * static_cast<size_t>(k), dist_alloc);

  coord_t *p_min_x = tree.min_x, *p_max_x = tree.max_x;
  coord_t *p_min_y = tree.min_y, *p_max_y = tree.max_y;
  coord_t *p_min_z = tree.min_z, *p_max_z = tree.max_z;
  int *p_left_child = tree.left_child, *p_right_child = tree.right_child;
  int *p_orig_idx = tree.orig_idx;

  // Use a fixed K for the priority queue in the kernel
  // In a real implementation, K would be a template parameter or handled more dynamically
  q.parallel_for(sycl::range<1>(num_queries), [=](sycl::id<1> idx) {
     size_t qi = idx[0];
     coord_t px = dev_qx[qi], py = dev_qy[qi], pz = dev_qz[qi];

     int stack[MAX_STACK_DEPTH];
     int stack_ptr = 0;

     if (n == 1) {
       stack[stack_ptr++] = 0;
     } else {
       stack[stack_ptr++] = 0;
     }

     // We'll use a local buffer for PQ. Since K is dynamic in the API but fixed in the struct,
     // we handle with a compile-time constant _MAX_K_ for the priority queue.
     PriorityQueue<double, _MAX_K_> pq;

     while (stack_ptr > 0) {
       int node_idx = stack[--stack_ptr];

       coord_t bmin_x = p_min_x[node_idx], bmax_x = p_max_x[node_idx];
       coord_t bmin_y = p_min_y[node_idx], bmax_y = p_max_y[node_idx];
       coord_t bmin_z = p_min_z[node_idx], bmax_z = p_max_z[node_idx];

       auto d2 = node_distance_sq(px, py, pz, bmin_x, bmax_x, bmin_y, bmax_y, bmin_z, bmax_z);

       if (pq.count < k || d2 < pq.data[0]) {
         if (node_idx >= (int)n - 1 && n > 1) {  // Leaf
#ifdef RETURN_ORIG_INDICES
           pq.push(d2, p_orig_idx[node_idx], k);
#else
           pq.push(d2, node_idx - (n - 1), k);
#endif
         } else if (n == 1) {
#ifdef RETURN_ORIG_INDICES
           pq.push(d2, p_orig_idx[0], k);
#else
           pq.push(d2, 0, k);
#endif
         } else {
           // Internal node
           int l = p_left_child[node_idx];
           int r = p_right_child[node_idx];

           // Heuristic: push the closer child last so it's processed first
           auto l_d2 = node_distance_sq(px, py, pz, p_min_x[l], p_max_x[l], p_min_y[l], p_max_y[l], p_min_z[l], p_max_z[l]);
           auto r_d2 = node_distance_sq(px, py, pz, p_min_x[r], p_max_x[r], p_min_y[r], p_max_y[r], p_min_z[r], p_max_z[r]);

           if (stack_ptr < MAX_STACK_DEPTH - 2) {
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
     }

     size_t offset = qi * static_cast<size_t>(k);
     for (int i = 0; i < k; ++i) {
       int out_idx = k - 1 - i;
       if (i < pq.count) {
         dev_results[offset + out_idx] = pq.indices[i];
#if defined(FASTTREE_INTEGER_COORDS)
          dev_result_dists[offset + out_idx] = float_to_int_rep(pq.data[i]);
#else
          dev_result_dists[offset + out_idx] = static_cast<coord_t>(pq.data[i]);
#endif
       } else {
         dev_results[offset + out_idx] = -1;
         dev_result_dists[offset + out_idx] = type_identity_max<coord_t>();
       }
     }
   }).wait();

  free_device_readable(q, dev_qx, qx_alloc);
  free_device_readable(q, dev_qy, qy_alloc);
  free_device_readable(q, dev_qz, qz_alloc);
  copy_back_and_free(q, dev_results, results, static_cast<size_t>(num_queries) * static_cast<size_t>(k), res_alloc);
  copy_back_and_free(q, dev_result_dists, result_dists, static_cast<size_t>(num_queries) * static_cast<size_t>(k), dist_alloc);
}

/**
 * @brief Finds all particles within a specified distance range from multiple query points.
 *
 * Executes a parallel non-recursive stack-based tree traversal. If a node's bounding box
 * overlaps the search range [r_min, r_max], traversal descends into it. Leaves within
 * the range are appended to the results list.
 *
 * @param[in] q SYCL queue.
 * @param[in] tree The built spatial tree.
 * @param[in] qx Pointer to query point x-coordinates.
 * @param[in] qy Pointer to query point y-coordinates.
 * @param[in] qz Pointer to query point z-coordinates.
 * @param[in] r_min Pointer to query minimum radius.
 * @param[in] r_max Pointer to query maximum radius.
 * @param[in] num_queries Number of query points.
 * @param[out] results Flat array containing results. Size: num_queries * max_results_per_query.
 * @param[out] result_counts Counts of matching particles found for each query. Size: num_queries.
 * @param[in] max_results_per_query Maximum number of matches to store per query.
 * @note result_counts[i] records total matching particles found. If result_counts[i] > max_results_per_query,
 *       the output results buffer is truncated to max_results_per_query.
 */
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
  int *dev_results = ensure_device_writable(q, results, static_cast<size_t>(num_queries) * static_cast<size_t>(max_results_per_query), res_alloc);
  int *dev_result_counts = ensure_device_writable(q, result_counts, static_cast<size_t>(num_queries), counts_alloc);

  coord_t *p_min_x = tree.min_x, *p_max_x = tree.max_x;
  coord_t *p_min_y = tree.min_y, *p_max_y = tree.max_y;
  coord_t *p_min_z = tree.min_z, *p_max_z = tree.max_z;
  int *p_left_child = tree.left_child, *p_right_child = tree.right_child;
  int *p_orig_idx = tree.orig_idx;

  q.parallel_for(sycl::range<1>(num_queries), [=](sycl::id<1> idx) {
     size_t qi = idx[0];
     coord_t px = dev_qx[qi], py = dev_qy[qi], pz = dev_qz[qi];
#if defined(FASTTREE_INTEGER_COORDS)
     double rm_d = int_rep_to_float(dev_r_min[qi]);
     double RM_d = int_rep_to_float(dev_r_max[qi]);
#else
     double rm_d = static_cast<double>(dev_r_min[qi]);
     double RM_d = static_cast<double>(dev_r_max[qi]);
#endif
     double RM2 = RM_d * RM_d;
     double rm2 = rm_d * rm_d;

     int stack[MAX_STACK_DEPTH];
     int stack_ptr = 0;

     if (n == 1) {
       stack[stack_ptr++] = 0;  // Only one leaf
     } else {
       stack[stack_ptr++] = 0;  // Root is internal node 0
     }

     int count = 0;
     size_t offset = qi * static_cast<size_t>(max_results_per_query);
     while (stack_ptr > 0) {
       int node_idx = stack[--stack_ptr];

       coord_t bmin_x = p_min_x[node_idx], bmax_x = p_max_x[node_idx];
       coord_t bmin_y = p_min_y[node_idx], bmax_y = p_max_y[node_idx];
       coord_t bmin_z = p_min_z[node_idx], bmax_z = p_max_z[node_idx];

       auto d2 = node_distance_sq(px, py, pz, bmin_x, bmax_x, bmin_y, bmax_y, bmin_z, bmax_z);

       if (d2 <= RM2) {
         if (node_idx >= (int)n - 1 && n > 1) {  // Leaf node
           if (d2 >= rm2) {
             if (count < max_results_per_query) {
#if defined(RETURN_ORIG_INDICES)
               dev_results[offset + count] = p_orig_idx[node_idx];
#else
               dev_results[offset + count] = node_idx - (n - 1);
#endif
             }
             count++;
           }
         } else if (n == 1) {  // Single leaf case
           if (d2 >= rm2) {
             if (count < max_results_per_query) {
#if defined(RETURN_ORIG_INDICES)
               dev_results[offset + count] = p_orig_idx[0];
#else
               dev_results[offset + count] = 0;
#endif
             }
             count++;
           }
         } else {
           // Internal node: traverse both children
           int l = p_left_child[node_idx];
           int r = p_right_child[node_idx];

           if (stack_ptr < MAX_STACK_DEPTH - 2) {
             stack[stack_ptr++] = r;
             stack[stack_ptr++] = l;
           }
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
  copy_back_and_free(q, dev_results, results, static_cast<size_t>(num_queries) * static_cast<size_t>(max_results_per_query), res_alloc);
  copy_back_and_free(q, dev_result_counts, result_counts, static_cast<size_t>(num_queries), counts_alloc);
}

/**
 * @brief High-level entry point to build the Hierarchical Linear Bounding Volume Hierarchy (HLBVH).
 *
 * Pipeline steps:
 * 1. Computes the global bounding box of all particles using parallel reductions.
 * 2. Encodes coordinates into space-filling curve keys (Morton/Peano-Hilbert).
 * 3. Sorts particles by key on the GPU using oneapi::dpl::sort on zipped keys and indices.
 * 4. Reorders particle properties according to the sorted index array on the GPU.
 * 5. Constructs the binary radix tree layout and computes internal node bounding boxes.
 *
 * @param[in] q SYCL queue.
 * @param[in] p SoA struct of input particles.
 * @param[in] bbox Optional precomputed bounding box. If nullptr, it will be computed.
 * @param[in,out] tree The output tree structure to build.
 */
inline void build_bvh(sycl::queue &q, const particles<coord_t> &p, TreeSoA &tree, BoundingBox<coord_t> *bbox = nullptr) {
  size_t n = p.pos_x.size();
  if (n == 0) return;

  if (n > static_cast<size_t>(std::numeric_limits<int>::max() / 2)) {
    throw std::runtime_error("Particle count n exceeds maximum supported tree size (INT_MAX / 2).");
  }

  // 1. Single-Pass Host-to-Device Staging Buffer
  bool x_alloc = false, y_alloc = false, z_alloc = false;
  const coord_t *dev_pos_x = ensure_device_readable(q, p.pos_x.data(), n, x_alloc);
  const coord_t *dev_pos_y = ensure_device_readable(q, p.pos_y.data(), n, y_alloc);
  const coord_t *dev_pos_z = ensure_device_readable(q, p.pos_z.data(), n, z_alloc);

  // Compute Bounding Box using parallel GPU reduction if not provided
  BoundingBox<coord_t> local_bbox(0, 0, 0, 0, 0, 0);
  if (bbox == nullptr) {
    local_bbox = compute_bbox(q, dev_pos_x, dev_pos_y, dev_pos_z, n);
    bbox = &local_bbox;
  }

  // 2. Allocate USM memory for keys and indices
  sfc_key *d_smk = sycl::malloc_shared<sfc_key>(n, q);
  size_t *d_indices = sycl::malloc_shared<size_t>(n, q);

  // 3. SFC Encoding and Index Initialization
  sfc_encode(q, dev_pos_x, dev_pos_y, dev_pos_z, n, d_smk, *bbox);
  q.parallel_for(sycl::range<1>(n), [=](sycl::id<1> idx) { d_indices[idx] = idx[0]; });
  q.wait();

  // 4. Single-Pass Full GPU Sort using oneDPL
#if (3 * BITS_PER_DIMENSION) <= 64
  uint64_t *d_sort_keys = sycl::malloc_shared<uint64_t>(n, q);
  sfc_key *d_smk_ptr = d_smk;
  q.parallel_for(sycl::range<1>(n), [=](sycl::id<1> idx) {
     size_t i = idx[0];
     d_sort_keys[i] = to_sort_key(d_smk_ptr[i]);
   }).wait();

  auto zip2_begin = oneapi::dpl::make_zip_iterator(d_sort_keys, d_smk, d_indices);
  auto zip2_end = zip2_begin + n;
  oneapi::dpl::sort(oneapi::dpl::execution::make_device_policy(q), zip2_begin, zip2_end,
                    [](auto a, auto b) { return oneapi::dpl::get<0>(a) < oneapi::dpl::get<0>(b); });
  q.wait();
  sycl::free(d_sort_keys, q);
#else
  auto policy = oneapi::dpl::execution::make_device_policy(q);
  auto zip_begin = oneapi::dpl::make_zip_iterator(d_smk, d_indices);
  auto zip_end = zip_begin + n;

  oneapi::dpl::sort(policy, zip_begin, zip_end, [](auto a, auto b) { return oneapi::dpl::get<0>(a) < oneapi::dpl::get<0>(b); });
  q.wait();
#endif

  // Prepare ID and Ghost arrays
  const uint32_t *p_id = nullptr;
  std::vector<uint32_t> temp_id;
  bool id_alloc = false;
  if (p.id.empty()) {
    temp_id.resize(n);
    std::iota(temp_id.begin(), temp_id.end(), 0u);
    p_id = temp_id.data();
  } else {
    p_id = p.id.data();
  }
  const uint32_t *dev_p_id = ensure_device_readable(q, p_id, n, id_alloc);

  const int8_t *p_ghost = nullptr;
  std::vector<int8_t> temp_ghost;
  bool ghost_alloc = false;
  if (p.is_ghost.empty()) {
    temp_ghost.resize(n, 0);
    p_ghost = temp_ghost.data();
  } else {
    p_ghost = p.is_ghost.data();
  }
  const int8_t *dev_p_ghost = ensure_device_readable(q, p_ghost, n, ghost_alloc);

  // 5. Direct Coordinate and Attribute Reordering into Tree Leaves on GPU
  coord_t *tree_min_x = tree.min_x, *tree_max_x = tree.max_x;
  coord_t *tree_min_y = tree.min_y, *tree_max_y = tree.max_y;
  coord_t *tree_min_z = tree.min_z, *tree_max_z = tree.max_z;
  uint32_t *tree_id = tree.id;
  int8_t *tree_ghost = tree.is_ghost;
  int *tree_orig_idx = tree.orig_idx;

  q.parallel_for(sycl::range<1>(n), [=](sycl::id<1> idx) {
     size_t i = idx[0];
     size_t orig_idx = d_indices[i];
     int leaf_idx = i + n - 1;
     tree_min_x[leaf_idx] = dev_pos_x[orig_idx];
     tree_max_x[leaf_idx] = dev_pos_x[orig_idx];
     tree_min_y[leaf_idx] = dev_pos_y[orig_idx];
     tree_max_y[leaf_idx] = dev_pos_y[orig_idx];
     tree_min_z[leaf_idx] = dev_pos_z[orig_idx];
     tree_max_z[leaf_idx] = dev_pos_z[orig_idx];
     if (tree_id) { tree_id[leaf_idx] = dev_p_id[orig_idx]; }
     if (tree_ghost) { tree_ghost[leaf_idx] = dev_p_ghost[orig_idx]; }
     if (tree_orig_idx) { tree_orig_idx[leaf_idx] = static_cast<int>(orig_idx); }
   }).wait();

  // 6. Build Tree Topology & Compute Internal Node BBoxes (leaving leaf arrays already set)
  build_tree(q, tree, d_smk);

  // Cleanup
  free_device_readable(q, dev_pos_x, x_alloc);
  free_device_readable(q, dev_pos_y, y_alloc);
  free_device_readable(q, dev_pos_z, z_alloc);
  free_device_readable(q, dev_p_id, id_alloc);
  free_device_readable(q, dev_p_ghost, ghost_alloc);

  sycl::free(d_smk, q);
  sycl::free(d_indices, q);
}

}  // namespace fasttree

#endif  // SYCL_FASTTREE_HLBVH_HPPREE_HLBVH_HPP