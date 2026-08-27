#ifndef SYCL_FASTTREE_HLBVH_HPP
#define SYCL_FASTTREE_HLBVH_HPP

#include "maxheap.hpp"
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
template <typename T> struct particles {
    /// Local/ghost particle positions in x, y, and z dimensions.
    std::vector<T> pos_x, pos_y, pos_z;
    /// Unique 32-bit identifier for tracking particles during spatial sorting and domain
    /// decomposition.
    std::vector<uint32_t> id;
    /// Ghost particle flag: 0 for locally owned particles, 1 for boundary ghost particles.
    std::vector<int8_t> is_ghost;
};
} // namespace fasttree

#if defined(SFC_TYPE_PEANO_HILBERT)
#include "sfc.peano_hilbert.hpp"
#elif defined(SFC_TYPE_MORTON)
#include "sfc.morton.hpp"
#else
#error "Undefined SFC_TYPE - either SFC_TYPE_PEANO_HILBERT or SFC_TYPE_MORTON must be defined"
#endif

namespace fasttree {

template <typename T> inline T type_identity_min() {
    if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
        return std::numeric_limits<T>::max();
    } else if constexpr (std::is_same_v<T, uint32_t>) {
        return 0xFFFFFFFFu;
    } else if constexpr (std::is_same_v<T, uint64_t>) {
        return 0xFFFFFFFFFFFFFFFFull;
    } else { // uint128_t
        return uint128_t(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull);
    }
}

template <typename T> inline T type_identity_max() {
    if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
        return -std::numeric_limits<T>::max();
    } else if constexpr (std::is_same_v<T, uint32_t>) {
        return 0u;
    } else if constexpr (std::is_same_v<T, uint64_t>) {
        return 0ull;
    } else { // uint128_t
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
template <typename FloatT> struct BoundingBox {
    FloatT min_x, max_x; ///< Bounds in x-dimension
    FloatT min_y, max_y; ///< Bounds in y-dimension
    FloatT min_z, max_z; ///< Bounds in z-dimension

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
    BoundingBox(
        FloatT min_x_, FloatT max_x_, FloatT min_y_, FloatT max_y_, FloatT min_z_, FloatT max_z_
    )
        : min_x(min_x_), max_x(max_x_), min_y(min_y_), max_y(max_y_), min_z(min_z_), max_z(max_z_) {
        static_assert(
            std::is_same_v<FloatT, float> || std::is_same_v<FloatT, double> ||
                std::is_same_v<FloatT, uint32_t> || std::is_same_v<FloatT, uint64_t> ||
                std::is_same_v<FloatT, uint128_t>,
            "BoundingBox supports float, double, uint32_t, uint64_t, and uint128_t"
        );
    }

    // Overload, initialize with std::numeric_limits for empty bounding box
    BoundingBox()
        : min_x(type_identity_min<FloatT>()), max_x(type_identity_max<FloatT>()),
          min_y(type_identity_min<FloatT>()), max_y(type_identity_max<FloatT>()),
          min_z(type_identity_min<FloatT>()), max_z(type_identity_max<FloatT>()) {}
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
        allocated  = true;
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
inline void
copy_back_and_free(sycl::queue &q, T *dev_ptr, T *host_ptr, size_t count, bool allocated) {
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
template <typename T> inline void free_device_readable(sycl::queue &q, T *dev_ptr, bool allocated) {
    if (allocated && dev_ptr) {
        sycl::free(const_cast<void *>(static_cast<const void *>(dev_ptr)), q);
    }
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
template <typename FloatT> struct ieee754_traits;

template <> struct ieee754_traits<float> {
    using uint_type                          = uint32_t;
    static constexpr uint_type mantissa_mask = 0x007FFFFFu; // 23 bits
    static constexpr int       mantissa_bits = 23;
    // For f in [1,2): exponent field = 127 (0x7F), stored in bits 23..30
};

template <> struct ieee754_traits<double> {
    using uint_type                          = uint64_t;
    static constexpr uint_type mantissa_mask = 0x000FFFFFFFFFFFFFull; // 52 bits
    static constexpr int       mantissa_bits = 52;
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
template <typename FloatT> inline sfc1D encode_to_sfc1d(FloatT val) noexcept {
    // Compile-time guard: reject types other than float/double
    static_assert(
        std::is_same_v<FloatT, float> || std::is_same_v<FloatT, double>,
        "encode_to_sfc1d only supports float or double"
    );

    // Compile-time guard: ensure the floating point type has enough
    // mantissa bits to represent BITS_PER_DIMENSION distinct levels
    static_assert(
        BITS_PER_DIMENSION <= ieee754_traits<FloatT>::mantissa_bits,
        "BITS_PER_DIMENSION exceeds the mantissa precision of FloatT. "
        "Use double for BITS_PER_DIMENSION > 23."
    );

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

    constexpr int bits_in_hs =
        (KEY_TOTAL_BITS > 2 * BITS_FOR_POSITIONS) ? (KEY_TOTAL_BITS - 2 * BITS_FOR_POSITIONS) : 0;
    constexpr int unused_hs = BITS_FOR_POSITIONS - bits_in_hs;

    if (k1.hs != k2.hs) { return count_clz(k1.hs ^ k2.hs) - unused_hs; }

    constexpr int bits_above_ls = KEY_TOTAL_BITS - BITS_FOR_POSITIONS;
    constexpr int bits_in_is =
        (bits_above_ls > 0)
            ? ((bits_above_ls > BITS_FOR_POSITIONS) ? BITS_FOR_POSITIONS : bits_above_ls)
            : 0;
    constexpr int unused_is = BITS_FOR_POSITIONS - bits_in_is;

    if (k1.is != k2.is) { return bits_in_hs + count_clz(k1.is ^ k2.is) - unused_is; }

    constexpr int bits_in_ls = KEY_TOTAL_BITS - bits_in_hs - bits_in_is;
    constexpr int unused_ls  = BITS_FOR_POSITIONS - bits_in_ls;

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
inline void sfc_encode(
    sycl::queue               &q,
    const FloatT              *pos_x,
    const FloatT              *pos_y,
    const FloatT              *pos_z,
    size_t                     num_particles,
    sfc_key                   *keys,
    const BoundingBox<FloatT> &bbox
) {
    if (num_particles == 0) return;

    bool          x_alloc = false, y_alloc = false, z_alloc = false, keys_alloc = false;
    const FloatT *dev_pos_x = ensure_device_readable(q, pos_x, num_particles, x_alloc);
    const FloatT *dev_pos_y = ensure_device_readable(q, pos_y, num_particles, y_alloc);
    const FloatT *dev_pos_z = ensure_device_readable(q, pos_z, num_particles, z_alloc);
    sfc_key      *dev_keys  = ensure_device_writable(q, keys, num_particles, keys_alloc);

#if defined(FASTTREE_INTEGER_COORDS)
    constexpr uint64_t sfc_mask = (1ULL << BITS_PER_DIMENSION) - 1ULL;
    q.parallel_for(sycl::range<1>(num_particles), [=](sycl::id<1> idx) {
         size_t i  = idx[0];
         sfc1D  ix = static_cast<sfc1D>(dev_pos_x[i] & sfc_mask);
         sfc1D  iy = static_cast<sfc1D>(dev_pos_y[i] & sfc_mask);
         sfc1D  iz = static_cast<sfc1D>(dev_pos_z[i] & sfc_mask);
#if defined(SFC_TYPE_PEANO_HILBERT)
         dev_keys[i] = sfc_encode3D(ix, iy, iz);
#elif defined(SFC_TYPE_MORTON)
         dev_keys[i] = spread3_u64(ix) | (spread3_u64(iy) << 1) | (spread3_u64(iz) << 2);
#endif
     }).wait();
#else
    FloatT dx          = bbox.max_x - bbox.min_x;
    FloatT dy          = bbox.max_y - bbox.min_y;
    FloatT dz          = bbox.max_z - bbox.min_z;
    FloatT inv_dx      = (dx == 0) ? static_cast<FloatT>(0.0) : (static_cast<FloatT>(1.0) / dx);
    FloatT inv_dy      = (dy == 0) ? static_cast<FloatT>(0.0) : (static_cast<FloatT>(1.0) / dy);
    FloatT inv_dz      = (dz == 0) ? static_cast<FloatT>(0.0) : (static_cast<FloatT>(1.0) / dz);
    FloatT clamp_upper = std::nextafter(static_cast<FloatT>(1.0), static_cast<FloatT>(0.0));
    FloatT clamp_lower = static_cast<FloatT>(0.0);

    q.parallel_for(sycl::range<1>(num_particles), [=](sycl::id<1> idx) {
         size_t i = idx[0];

         FloatT nx = sycl::clamp((dev_pos_x[i] - bbox.min_x) * inv_dx, clamp_lower, clamp_upper);
         FloatT ny = sycl::clamp((dev_pos_y[i] - bbox.min_y) * inv_dy, clamp_lower, clamp_upper);
         FloatT nz = sycl::clamp((dev_pos_z[i] - bbox.min_z) * inv_dz, clamp_lower, clamp_upper);

#if defined(SFC_TYPE_PEANO_HILBERT)
         sfc1D ix    = quantize_coord(nx);
         sfc1D iy    = quantize_coord(ny);
         sfc1D iz    = quantize_coord(nz);
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
inline void sfc_encode(
    sycl::queue               &q,
    const particles<FloatT>   &particles,
    sfc_key                   *keys,
    const BoundingBox<FloatT> &bbox
) {
    sfc_encode(
        q,
        particles.pos_x.data(),
        particles.pos_y.data(),
        particles.pos_z.data(),
        particles.pos_x.size(),
        keys,
        bbox
    );
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
inline BoundingBox<FloatT> compute_bbox(
    sycl::queue &q, const FloatT *pos_x, const FloatT *pos_y, const FloatT *pos_z, size_t n
) {
    if (n == 0) {
        return BoundingBox<FloatT>(
            type_identity_min<FloatT>(),
            type_identity_max<FloatT>(),
            type_identity_min<FloatT>(),
            type_identity_max<FloatT>(),
            type_identity_min<FloatT>(),
            type_identity_max<FloatT>()
        );
    }

    bool          x_alloc = false, y_alloc = false, z_alloc = false;
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

    if constexpr (
        std::is_same_v<FloatT, float> || std::is_same_v<FloatT, double> ||
        std::is_same_v<FloatT, uint32_t> || std::is_same_v<FloatT, uint64_t>
    ) {
        q.submit([&](sycl::handler &h) {
             h.parallel_for(
                 sycl::range<1>(n),
                 sycl::reduction(
                     d_bbox_reduction + 0, type_identity_min<FloatT>(), sycl::minimum<FloatT>()
                 ),
                 sycl::reduction(
                     d_bbox_reduction + 1, type_identity_max<FloatT>(), sycl::maximum<FloatT>()
                 ),
                 sycl::reduction(
                     d_bbox_reduction + 2, type_identity_min<FloatT>(), sycl::minimum<FloatT>()
                 ),
                 sycl::reduction(
                     d_bbox_reduction + 3, type_identity_max<FloatT>(), sycl::maximum<FloatT>()
                 ),
                 sycl::reduction(
                     d_bbox_reduction + 4, type_identity_min<FloatT>(), sycl::minimum<FloatT>()
                 ),
                 sycl::reduction(
                     d_bbox_reduction + 5, type_identity_max<FloatT>(), sycl::maximum<FloatT>()
                 ),
                 [=](sycl::id<1> idx,
                     auto       &r_min_x,
                     auto       &r_max_x,
                     auto       &r_min_y,
                     auto       &r_max_y,
                     auto       &r_min_z,
                     auto       &r_max_z) {
                     size_t i = idx[0];
                     r_min_x.combine(dev_pos_x[i]);
                     r_max_x.combine(dev_pos_x[i]);
                     r_min_y.combine(dev_pos_y[i]);
                     r_max_y.combine(dev_pos_y[i]);
                     r_min_z.combine(dev_pos_z[i]);
                     r_max_z.combine(dev_pos_z[i]);
                 }
             );
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

    BoundingBox<FloatT> bbox = {
        d_bbox_reduction[0],
        d_bbox_reduction[1],
        d_bbox_reduction[2],
        d_bbox_reduction[3],
        d_bbox_reduction[4],
        d_bbox_reduction[5]
    };

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

template <typename T> inline T sfc_min(T a, T b) {
    if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
        return sycl::fmin(a, b);
    } else {
        return (a < b) ? a : b;
    }
}

template <typename T> inline T sfc_max(T a, T b) {
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
    coord_t *min_x,
        *max_x; ///< Node minimum and maximum x-coordinates. Size: num_leaves + num_internal.
    coord_t *min_y,
        *max_y; ///< Node minimum and maximum y-coordinates. Size: num_leaves + num_internal.
    coord_t *min_z,
        *max_z; ///< Node minimum and maximum z-coordinates. Size: num_leaves + num_internal.
    int      *left_child;  ///< Left child index. Size: num_internal.
    int      *right_child; ///< Right child index. Size: num_internal.
    int      *parent;      ///< Parent node index. Size: num_leaves + num_internal.
    uint32_t *id;        ///< Particle IDs (e.g. AREPO unique IDs). Size: num_leaves + num_internal.
    int8_t   *is_ghost;  ///< Flags for local vs ghost particles. Size: num_leaves + num_internal.
    int      *orig_idx;  ///< Original input array index of sorted particles. Size: num_leaves +
                         ///< num_internal.
    size_t num_leaves;   ///< Total leaf nodes (equivalent to particle count).
    size_t num_internal; ///< Total internal nodes (equal to num_leaves - 1).

    /**
     * @brief Construct a TreeSoA, allocating USM shared memory.
     *
     * @param[in] q SYCL queue used for allocating USM memory.
     * @param[in] n Number of leaf nodes (particles) in the tree.
     */
    TreeSoA(sycl::queue &q, size_t n)
        : num_leaves(n), num_internal(n > 0 ? n - 1 : 0), id(nullptr), is_ghost(nullptr),
          orig_idx(nullptr) {
        size_t total_nodes = num_leaves + num_internal;
        if (total_nodes == 0) return;

        min_x       = sycl::malloc_shared<coord_t>(total_nodes, q);
        max_x       = sycl::malloc_shared<coord_t>(total_nodes, q);
        min_y       = sycl::malloc_shared<coord_t>(total_nodes, q);
        max_y       = sycl::malloc_shared<coord_t>(total_nodes, q);
        min_z       = sycl::malloc_shared<coord_t>(total_nodes, q);
        max_z       = sycl::malloc_shared<coord_t>(total_nodes, q);
        left_child  = sycl::malloc_shared<int>(num_internal, q);
        right_child = sycl::malloc_shared<int>(num_internal, q);
        parent      = sycl::malloc_shared<int>(total_nodes, q);
        id          = sycl::malloc_shared<uint32_t>(total_nodes, q);
        is_ghost    = sycl::malloc_shared<int8_t>(total_nodes, q);
        orig_idx    = sycl::malloc_shared<int>(total_nodes, q);
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
inline int sgn(int x) {
    return (x > 0) - (x < 0);
}

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
inline void build_tree(
    sycl::queue    &q,
    TreeSoA        &tree,
    const sfc_key  *sorted_keys,
    const coord_t  *sorted_x        = nullptr,
    const coord_t  *sorted_y        = nullptr,
    const coord_t  *sorted_z        = nullptr,
    const uint32_t *sorted_id       = nullptr,
    const int8_t   *sorted_is_ghost = nullptr,
    const int      *sorted_orig_idx = nullptr
) {
    size_t n = tree.num_leaves;
    if (n == 0) return;

    bool keys_alloc = false, x_alloc = false, y_alloc = false, z_alloc = false;
    bool id_alloc = false, ghost_alloc = false, orig_alloc = false;

    const sfc_key  *dev_keys = ensure_device_readable(q, sorted_keys, n, keys_alloc);
    const coord_t  *dev_x    = sorted_x ? ensure_device_readable(q, sorted_x, n, x_alloc) : nullptr;
    const coord_t  *dev_y    = sorted_y ? ensure_device_readable(q, sorted_y, n, y_alloc) : nullptr;
    const coord_t  *dev_z    = sorted_z ? ensure_device_readable(q, sorted_z, n, z_alloc) : nullptr;
    const uint32_t *dev_id =
        sorted_id ? ensure_device_readable(q, sorted_id, n, id_alloc) : nullptr;
    const int8_t *dev_ghost =
        sorted_is_ghost ? ensure_device_readable(q, sorted_is_ghost, n, ghost_alloc) : nullptr;
    const int *dev_orig_idx =
        sorted_orig_idx ? ensure_device_readable(q, sorted_orig_idx, n, orig_alloc) : nullptr;

    if (n == 1) {
        uint32_t *p_id       = tree.id;
        int8_t   *p_ghost    = tree.is_ghost;
        int      *p_orig_idx = tree.orig_idx;
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
         int l_max     = 2;
         while (delta(i, i + l_max * d) > delta_min) {
             l_max *= 2;
         }

         // Find the other end using binary search
         int l = 0;
         for (int t = l_max / 2; t >= 1; t /= 2) {
             if (delta(i, i + (l + t) * d) > delta_min) { l += t; }
         }
         int j = i + l * d;

         // Find the split point using binary search
         int delta_node = delta(i, j);
         int s          = 0;
         int t_split    = 1;
         while (t_split <= l) {
             t_split *= 2;
         }
         t_split /= 2;

         for (int t = t_split; t >= 1; t /= 2) {
             if (s + t < l && delta(i, i + (s + t) * d) > delta_node) { s += t; }
         }
         int split = i + s * d + std::min(d, 0);

         // Select children
         int left_idx  = (std::min(i, j) == split) ? (split + n - 1) : split;
         int right_idx = (std::max(i, j) == split + 1) ? (split + 1 + n - 1) : (split + 1);

         p_left_child[i]     = left_idx;
         p_right_child[i]    = right_idx;
         p_parent[left_idx]  = i;
         p_parent[right_idx] = i;
     }).wait();

    // 2. Initialize leaf bounding boxes
    if (dev_x && dev_y && dev_z) {
        uint32_t *p_id       = tree.id;
        int8_t   *p_ghost    = tree.is_ghost;
        int      *p_orig_idx = tree.orig_idx;
        q.parallel_for(sycl::range<1>(n), [=](sycl::id<1> idx) {
             int i             = idx[0];
             int leaf_idx      = i + n - 1;
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
         int curr = idx[0] + n - 1; // Start from leaf
         while (curr != 0) {        // Until root (internal node 0)
             int p = p_parent[curr];
             if (p < 0 || p >= (int)n - 1) break;

             auto atomic_ref = sycl::atomic_ref<
                 int,
                 sycl::memory_order::seq_cst, // Strictly sequence memory operations
                 sycl::memory_scope::system,  // Force system-wide (cross-core) cache coherency
                 sycl::access::address_space::global_space>(counters[p]);

             if (atomic_ref.fetch_add(1) == 0) return; // First child to arrive

             // Fence: ensure all writes to child BBs are visible before reading them
             sycl::atomic_fence(sycl::memory_order::acquire, sycl::memory_scope::device);

             // Second child arrived, compute BB using sfc_min/sfc_max
             int l      = p_left_child[p];
             int r      = p_right_child[p];
             p_min_x[p] = sfc_min(p_min_x[l], p_min_x[r]);
             p_max_x[p] = sfc_max(p_max_x[l], p_max_x[r]);
             p_min_y[p] = sfc_min(p_min_y[l], p_min_y[r]);
             p_max_y[p] = sfc_max(p_max_y[l], p_max_y[r]);
             p_min_z[p] = sfc_min(p_min_z[l], p_min_z[r]);
             p_max_z[p] = sfc_max(p_max_z[l], p_max_z[r]);
             curr       = p;
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
 * @brief Computes the minimum squared distance between a 3D query point
 *        and an axis-aligned bounding box.
 *
 * Return type:
 *   - float/double coordinates → double (squared Euclidean distance)
 *   - integer coordinates, PERIODIC_BC defined   → uint64_t (shifted integer squared distance)
 *   - integer coordinates, PERIODIC_BC undefined → uint64_t (shifted integer squared distance)
 *
 * The _DIST_SHIFT pre-shift on each axis distance prevents uint64_t overflow
 * when squaring and summing three axis components.
 */
template <typename T>
inline auto
node_distance_sq(T px, T py, T pz, T bmin_x, T bmax_x, T bmin_y, T bmax_y, T bmin_z, T bmax_z) {
    if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
        // ── Floating-point path (unchanged) ───────────────────────
        T dx = sycl::fmax(bmin_x - px, sycl::fmax(T(0.0), px - bmax_x));
        T dy = sycl::fmax(bmin_y - py, sycl::fmax(T(0.0), py - bmax_y));
        T dz = sycl::fmax(bmin_z - pz, sycl::fmax(T(0.0), pz - bmax_z));
        return static_cast<double>(dx * dx + dy * dy + dz * dz);

    } else {
        // ── Integer coordinate path ───────────────────────────────
        // T is MyIntPosType (uint32_t, uint64_t, or uint128_t).
        // All arithmetic stays in integer space — no float conversion.
        // Each axis gap is right-shifted by _DIST_SHIFT before squaring
        // to keep the 3-axis squared sum within uint64_t.

#ifdef PERIODIC_BC
        // ── Periodic boundary (toroidal) ──────────────────────────
        // The domain wraps: coordinate 0 and coordinate 2^B are the same point.
        // Distance from val to the interval [lo, hi] on a torus of period 2^B:
        //   - If val is inside [lo, hi] (on the torus), distance = 0.
        //   - Otherwise, distance = min(|val - lo|_periodic, |val - hi|_periodic).
        auto periodic_axis_dist = [](T val, T lo, T hi) -> uint64_t {
            if constexpr (sizeof(T) >= 8) {
                constexpr T mask = (T(1) << BITS_PER_DIMENSION) - T(1);
                val &= mask;
            }
            if (val >= lo && val <= hi) return 0ULL;
            MyIntPosTypeSigned d_lo   = shortest_periodic_distance(val, lo);
            MyIntPosTypeSigned d_hi   = shortest_periodic_distance(val, hi);
            uint64_t           abs_lo = static_cast<uint64_t>(d_lo < 0 ? -d_lo : d_lo);
            uint64_t           abs_hi = static_cast<uint64_t>(d_hi < 0 ? -d_hi : d_hi);
            uint64_t           gap    = (abs_lo < abs_hi) ? abs_lo : abs_hi;
            return gap >> _DIST_SHIFT;
        };

        uint64_t dx = periodic_axis_dist(px, bmin_x, bmax_x);
        uint64_t dy = periodic_axis_dist(py, bmin_y, bmax_y);
        uint64_t dz = periodic_axis_dist(pz, bmin_z, bmax_z);
        return dx * dx + dy * dy + dz * dz; // uint64_t, no overflow after shift

#else
        // ── Non-periodic integer distance ─────────────────────────
        // Standard half-open-box distance: 0 if inside, else gap to nearest face.
        auto axis_dist = [](T val, T lo, T hi) -> uint64_t {
            if (val < lo) return get_lo_word(lo - val) >> _DIST_SHIFT;
            if (val > hi) return get_lo_word(val - hi) >> _DIST_SHIFT;
            return 0ULL;
        };

        uint64_t dx = axis_dist(px, bmin_x, bmax_x);
        uint64_t dy = axis_dist(py, bmin_y, bmax_y);
        uint64_t dz = axis_dist(pz, bmin_z, bmax_z);
        return dx * dx + dy * dy + dz * dz; // uint64_t, no overflow after shift
#endif
    }
}

template <int _MAX_K_ = 32>
void knn_query_small_k(
    sycl::queue   &q,
    const TreeSoA &tree,
    const coord_t *dev_qx,
    const coord_t *dev_qy,
    const coord_t *dev_qz,
    int            k,
    int            num_queries,
    size_t        *dev_results,
    dist_t        *dev_result_dists
) {
    static_assert(_MAX_K_ <= 32, "knn_query_small_k: use knn_query_large_k for k > 32");

    size_t   n       = tree.num_leaves;
    coord_t *p_min_x = tree.min_x, *p_max_x = tree.max_x;
    coord_t *p_min_y = tree.min_y, *p_max_y = tree.max_y;
    coord_t *p_min_z = tree.min_z, *p_max_z = tree.max_z;
    int     *p_left     = tree.left_child;
    int     *p_right    = tree.right_child;
    int     *p_orig_idx = tree.orig_idx;

    q.parallel_for(sycl::range<1>(num_queries), [=](sycl::id<1> gid) {
         size_t  qi = gid[0];
         coord_t px = dev_qx[qi];
         coord_t py = dev_qy[qi];
         coord_t pz = dev_qz[qi];

         RegisterMaxHeap<heap_dist_t, int, _MAX_K_> heap;

         // ── Traversal stack ──────────────────────────────────
         int stack[MAX_STACK_DEPTH];
         int sp      = 0;
         stack[sp++] = 0; // root

         while (sp > 0) {
             int node = stack[--sp];

             auto d2 = node_distance_sq(
                 px,
                 py,
                 pz,
                 p_min_x[node],
                 p_max_x[node],
                 p_min_y[node],
                 p_max_y[node],
                 p_min_z[node],
                 p_max_z[node]
             );

             // ── Prune: skip node if farther than k-th nearest so far
             if (heap.should_prune(d2, k)) continue;

             if (node >= static_cast<int>(n) - 1 && n > 1) {
                // ── Leaf node
#ifdef RETURN_ORIG_INDICES
                 heap.push(d2, p_orig_idx[node], k);
#else
                    heap.push(d2, node - static_cast<int>(n - 1), k);
#endif
             } else if (n == 1) {
                // ── Single-particle tree
#ifdef RETURN_ORIG_INDICES
                 heap.push(d2, p_orig_idx[0], k);
#else
                    heap.push(d2, 0, k);
#endif
             } else {
                 // ── Internal node: push children
                 int l = p_left[node];
                 int r = p_right[node];

                 auto ld2 = node_distance_sq(
                     px,
                     py,
                     pz,
                     p_min_x[l],
                     p_max_x[l],
                     p_min_y[l],
                     p_max_y[l],
                     p_min_z[l],
                     p_max_z[l]
                 );
                 auto rd2 = node_distance_sq(
                     px,
                     py,
                     pz,
                     p_min_x[r],
                     p_max_x[r],
                     p_min_y[r],
                     p_max_y[r],
                     p_min_z[r],
                     p_max_z[r]
                 );

                 if (sp < MAX_STACK_DEPTH - 2) {
                     // Push farther child first → closer processed first
                     if (ld2 <= rd2) {
                         stack[sp++] = r;
                         stack[sp++] = l;
                     } else {
                         stack[sp++] = l;
                         stack[sp++] = r;
                     }
                 }
             }
         }

         // ── Extract results in ascending distance order ──────
         size_t offset = qi * static_cast<size_t>(k);

         // Temporary buffers for sorted extraction
         heap_dist_t sorted_dist[_MAX_K_];
         int         sorted_idx[_MAX_K_];
         heap.extract_sorted(sorted_dist, sorted_idx, k);

         for (int i = 0; i < k; ++i) {
             if (sorted_idx[i] >= 0) {
                 dev_results[offset + i]      = static_cast<size_t>(sorted_idx[i]);
                 dev_result_dists[offset + i] = static_cast<dist_t>(sorted_dist[i]);
             } else {
                 dev_results[offset + i]      = static_cast<size_t>(-1);
                 dev_result_dists[offset + i] = type_identity_max<dist_t>();
             }
         }
     }).wait();
}

template <int _MAX_K_ = 128>
void knn_query_large_k(
    sycl::queue   &q,
    const TreeSoA &tree,
    const coord_t *dev_qx,
    const coord_t *dev_qy,
    const coord_t *dev_qz,
    int            k,
    int            num_queries,
    size_t        *dev_results,
    dist_t        *dev_result_dists
) {
    static_assert(_MAX_K_ > 32, "knn_query_large_k: use knn_query_small_k for k <= 32");

    constexpr int HEAP_CAP = []() constexpr {
        int cap = 1;
        while (cap < _MAX_K_)
            cap <<= 1;
        return cap;
    }();

    size_t   n       = tree.num_leaves;
    coord_t *p_min_x = tree.min_x, *p_max_x = tree.max_x;
    coord_t *p_min_y = tree.min_y, *p_max_y = tree.max_y;
    coord_t *p_min_z = tree.min_z, *p_max_z = tree.max_z;
    int     *p_left     = tree.left_child;
    int     *p_right    = tree.right_child;
    int     *p_orig_idx = tree.orig_idx;

    size_t lm_heap_dist  = HEAP_CAP * sizeof(heap_dist_t);
    size_t lm_heap_idx   = HEAP_CAP * sizeof(int);
    size_t lm_stage_dist = KNN_WG_SIZE * sizeof(heap_dist_t);
    size_t lm_stage_idx  = KNN_WG_SIZE * sizeof(int);
    size_t lm_control    = 2 * sizeof(int);
    size_t lm_stack      = MAX_STACK_DEPTH * sizeof(int);
    size_t lm_total =
        lm_heap_dist + lm_heap_idx + lm_stage_dist + lm_stage_idx + lm_control + lm_stack;

    size_t max_lm = q.get_device().template get_info<sycl::info::device::local_mem_size>();
    if (lm_total > max_lm) {
        throw std::runtime_error(
            "knn_query_large_k: k too large, "
            "local memory requirement exceeds device limit. "
            "Reduce k or increase KNN_WG_SIZE."
        );
    }

    q.submit([&](sycl::handler &h) {
         sycl::local_accessor<heap_dist_t, 1> sh_dist(HEAP_CAP, h);
         sycl::local_accessor<int, 1>         sh_idx(HEAP_CAP, h);
         sycl::local_accessor<heap_dist_t, 1> sh_stage_dist(KNN_WG_SIZE, h);
         sycl::local_accessor<int, 1>         sh_stage_idx(KNN_WG_SIZE, h);
         sycl::local_accessor<int, 1>         sh_count(1, h);
         sycl::local_accessor<int, 1>         sh_stack(MAX_STACK_DEPTH, h);
         sycl::local_accessor<int, 1>         sh_sp(1, h);

         h.parallel_for(
             sycl::nd_range<1>(
                 num_queries * KNN_WG_SIZE, // global size
                 KNN_WG_SIZE
             ), // local size (work-group)
             [=](sycl::nd_item<1> item) {
                 int    lid = static_cast<int>(item.get_local_id(0));
                 size_t qi  = item.get_group(0);

                 coord_t px = dev_qx[qi];
                 coord_t py = dev_qy[qi];
                 coord_t pz = dev_qz[qi];

                 // (b) init call:
                 SortedMergeHeap<heap_dist_t, int>::init(item, sh_dist, sh_idx, sh_count, HEAP_CAP);

                 if (lid == 0) {
                     sh_sp[0]    = 1;
                     sh_stack[0] = 0; // push root
                 }
                 sycl::group_barrier(item.get_group());

                 while (true) {
                     int node = -1;
                     if (lid == 0) {
                         if (sh_sp[0] > 0) { node = sh_stack[--sh_sp[0]]; }
                     }
                     node = sycl::group_broadcast(item.get_group(), node, 0);
                     if (node < 0) break;

                     auto d2 = node_distance_sq(
                         px,
                         py,
                         pz,
                         p_min_x[node],
                         p_max_x[node],
                         p_min_y[node],
                         p_max_y[node],
                         p_min_z[node],
                         p_max_z[node]
                     );

                     if (SortedMergeHeap<heap_dist_t, int>::should_prune(
                             d2, sh_dist, sh_count, k, HEAP_CAP
                         )) {
                         continue;
                     }

                     if (node >= static_cast<int>(n) - 1 && n > 1) {
                         heap_dist_t my_d = std::numeric_limits<heap_dist_t>::max();
                         int         my_i = -1;

                         if (lid == 0) {
#ifdef RETURN_ORIG_INDICES
                             my_i = p_orig_idx[node];
#else
                             my_i = node - static_cast<int>(n - 1);
#endif
                             my_d = static_cast<heap_dist_t>(d2);
                         }

                         SortedMergeHeap<heap_dist_t, int>::merge_batch(
                             item,
                             sh_dist,
                             sh_idx,
                             sh_count,
                             sh_stage_dist,
                             sh_stage_idx,
                             my_d,
                             my_i,
                             k,
                             HEAP_CAP
                         );

                     } else if (n == 1) {
                         auto my_d = (lid == 0) ? d2 : std::numeric_limits<heap_dist_t>::max();
                         int  my_i = (lid == 0) ? 0 : -1;
                         SortedMergeHeap<heap_dist_t, int>::merge_batch(
                             item,
                             sh_dist,
                             sh_idx,
                             sh_count,
                             sh_stage_dist,
                             sh_stage_idx,
                             my_d,
                             my_i,
                             k,
                             HEAP_CAP
                         );

                     } else {
                         if (lid == 0) {
                             int l = p_left[node];
                             int r = p_right[node];

                             auto ld2 = node_distance_sq(
                                 px,
                                 py,
                                 pz,
                                 p_min_x[l],
                                 p_max_x[l],
                                 p_min_y[l],
                                 p_max_y[l],
                                 p_min_z[l],
                                 p_max_z[l]
                             );
                             auto rd2 = node_distance_sq(
                                 px,
                                 py,
                                 pz,
                                 p_min_x[r],
                                 p_max_x[r],
                                 p_min_y[r],
                                 p_max_y[r],
                                 p_min_z[r],
                                 p_max_z[r]
                             );

                             if (sh_sp[0] < MAX_STACK_DEPTH - 2) {
                                 if (ld2 <= rd2) {
                                     sh_stack[sh_sp[0]++] = r;
                                     sh_stack[sh_sp[0]++] = l;
                                 } else {
                                     sh_stack[sh_sp[0]++] = l;
                                     sh_stack[sh_sp[0]++] = r;
                                 }
                             }
                         }
                         sycl::group_barrier(item.get_group());
                     }
                 }

                 size_t offset = qi * static_cast<size_t>(k);

                 // sh_dist/sh_idx are already sorted ascending, just write out:
                 for (int i = lid; i < k; i += KNN_WG_SIZE) {
                     if (sh_idx[i] >= 0) {
                         dev_results[offset + i]      = static_cast<size_t>(sh_idx[i]);
                         dev_result_dists[offset + i] = static_cast<dist_t>(sh_dist[i]);
                     } else {
                         dev_results[offset + i]      = static_cast<size_t>(-1);
                         dev_result_dists[offset + i] = type_identity_max<dist_t>();
                     }
                 }
             }
         );
     }).wait();
}

/**
 * @brief Unified KNN query dispatch for multiple query points.
 *
 * Dispatches to RegisterMaxHeap for k <= 32 or SharedMaxHeap for k > 32.
 *
 * @param[in] q SYCL queue to run the query kernel.
 * @param[in] tree The built spatial tree.
 * @param[in] qx Pointer to query point x-coordinates.
 * @param[in] qy Pointer to query point y-coordinates.
 * @param[in] qz Pointer to query point z-coordinates.
 * @param[in] k Number of nearest neighbors to find.
 * @param[in] num_queries Total number of query points.
 * @param[out] results Output buffer for nearest neighbor indices. Size: num_queries * k.
 * @param[out] result_dists Output buffer for neighbor squared distances. Size: num_queries * k.
 */
template <int _MAX_K_ = 256>
inline void knn_query(
    sycl::queue   &q,
    const TreeSoA &tree,
    const coord_t *qx,
    const coord_t *qy,
    const coord_t *qz,
    int            k,
    int            num_queries,
    size_t        *results,
    dist_t        *result_dists
) {
    size_t n = tree.num_leaves;
    if (n == 0 || num_queries == 0) return;

    bool qx_alloc = false, qy_alloc = false, qz_alloc = false;
    bool res_alloc = false, dist_alloc = false;

    const coord_t *dev_qx      = ensure_device_readable(q, qx, num_queries, qx_alloc);
    const coord_t *dev_qy      = ensure_device_readable(q, qy, num_queries, qy_alloc);
    const coord_t *dev_qz      = ensure_device_readable(q, qz, num_queries, qz_alloc);
    size_t        *dev_results = ensure_device_writable(
        q, results, static_cast<size_t>(num_queries) * static_cast<size_t>(k), res_alloc
    );
    dist_t *dev_result_dists = ensure_device_writable(
        q, result_dists, static_cast<size_t>(num_queries) * static_cast<size_t>(k), dist_alloc
    );

    if constexpr (_MAX_K_ <= 32) {
        knn_query_small_k<32>(
            q, tree, dev_qx, dev_qy, dev_qz, k, num_queries, dev_results, dev_result_dists
        );
    } else {
        if (k <= 32) {
            knn_query_small_k<32>(
                q, tree, dev_qx, dev_qy, dev_qz, k, num_queries, dev_results, dev_result_dists
            );
        } else {
            if (k > _MAX_K_) {
                throw std::invalid_argument(
                    "k exceeds _MAX_K_ template parameter. Instantiate knn_query with larger "
                    "_MAX_K_."
                );
            }
            knn_query_large_k<_MAX_K_>(
                q, tree, dev_qx, dev_qy, dev_qz, k, num_queries, dev_results, dev_result_dists
            );
        }
    }

    free_device_readable(q, dev_qx, qx_alloc);
    free_device_readable(q, dev_qy, qy_alloc);
    free_device_readable(q, dev_qz, qz_alloc);
    copy_back_and_free(
        q,
        dev_results,
        results,
        static_cast<size_t>(num_queries) * static_cast<size_t>(k),
        res_alloc
    );
    copy_back_and_free(
        q,
        dev_result_dists,
        result_dists,
        static_cast<size_t>(num_queries) * static_cast<size_t>(k),
        dist_alloc
    );
}

// ================================================================
// self_knn_query_grouped_impl, generalized to dispatch on k.
// Traversal/grouping logic (any_of_group prune-skip, stack, SFC
// assumption) is UNCHANGED from the point-1 version -- only the
// heap storage and its init/push/extract call sites differ between
// the two branches.
//
// Note: unlike the RegisterMaxHeap-only version, the k>32 branch
// needs a sycl::handler to declare local_accessors, so the two
// branches are separate kernel submissions (mirrors knn_query_
// small_k / knn_query_large_k being two separate functions, not
// one function with a runtime branch inside a single kernel).
// ================================================================

template <int _MAX_K_ = 32, int _GROUP_WIDTH_ = 32>
void self_knn_query_grouped_small_k(
    sycl::queue   &q,
    const TreeSoA &tree,
    const int     *query_leaf_ids,
    int            num_queries,
    int            k,
    size_t        *dev_results,
    dist_t        *dev_result_dists,
    bool           exclude_self
) {
    static_assert(_MAX_K_ <= 32, "grouped self-kNN (register path) requires k<=32");

    constexpr int WARP = _GROUP_WIDTH_;

    size_t   n       = tree.num_leaves;
    coord_t *p_min_x = tree.min_x, *p_max_x = tree.max_x;
    coord_t *p_min_y = tree.min_y, *p_max_y = tree.max_y;
    coord_t *p_min_z = tree.min_z, *p_max_z = tree.max_z;
    int     *p_left       = tree.left_child;
    int     *p_right      = tree.right_child;
    int     *p_orig_idx   = tree.orig_idx;
    int      num_internal = static_cast<int>(tree.num_internal);

    // Leaf coordinate views (leaf storage — see section 4 for the bbox-splitting note;
    // this code assumes the *current* min==max leaf layout so it drops in as-is).
    const coord_t *leaf_x = tree.min_x + tree.num_internal;
    const coord_t *leaf_y = tree.min_y + tree.num_internal;
    const coord_t *leaf_z = tree.min_z + tree.num_internal;

    int num_groups = (num_queries + WARP - 1) / WARP;

    q.submit([&](sycl::handler &h) {
         h.parallel_for(
             sycl::nd_range<1>(num_groups * WARP, WARP),
             [=](sycl::nd_item<1> item) [[sycl::reqd_sub_group_size(WARP)]] {
                 sycl::sub_group sg     = item.get_sub_group();
                 int             lane   = static_cast<int>(sg.get_local_id()[0]);
                 int             group  = static_cast<int>(item.get_group(0));
                 int             q_slot = group * WARP + lane;

                 bool active = q_slot < num_queries;
                 int  leaf_rank =
                     active ? (query_leaf_ids ? query_leaf_ids[q_slot] : q_slot)
                            : 0; // inactive lanes shadow lane 0's traversal to stay in lockstep

                 coord_t px            = leaf_x[leaf_rank];
                 coord_t py            = leaf_y[leaf_rank];
                 coord_t pz            = leaf_z[leaf_rank];
                 int     self_leaf_idx = leaf_rank + num_internal;

                 RegisterMaxHeap<heap_dist_t, int, _MAX_K_> heap;

                 int stack[MAX_STACK_DEPTH];
                 int sp      = 0;
                 stack[sp++] = 0; // root

                 while (sp > 0) {
                     int node = stack[--sp];

                     auto d2 = node_distance_sq(
                         px,
                         py,
                         pz,
                         p_min_x[node],
                         p_max_x[node],
                         p_min_y[node],
                         p_max_y[node],
                         p_min_z[node],
                         p_max_z[node]
                     );

                     // Per-lane prune decision, but the GROUP decides whether to
                     // bother reading children/leaf data at all: if every active
                     // lane has already pruned this node, skip the work entirely.
                     bool mine_pruned     = active && heap.should_prune(d2, k);
                     bool anyone_needs_it = sycl::any_of_group(sg, active && !mine_pruned);

                     if (!anyone_needs_it) continue; // whole warp skips together — no divergence

                     if (node >= static_cast<int>(n) - 1 && n > 1) {
                         // Leaf: lanes that already pruned just don't push (branch is
                         // uniform in *shape*, only the push predicate differs — cheap).
                         if (active && !mine_pruned) {
                             if (!(exclude_self && node == self_leaf_idx)) {
#ifdef RETURN_ORIG_INDICES
                                 heap.push(d2, p_orig_idx[node], k);
#else
                                 heap.push(d2, node - static_cast<int>(n - 1), k);
#endif
                             }
                         }
                     } else if (n == 1) {
                         if (active && !mine_pruned && !exclude_self) {
#ifdef RETURN_ORIG_INDICES
                             heap.push(d2, p_orig_idx[0], k);
#else
                             heap.push(d2, 0, k);
#endif
                         }
                     } else {
                         // Internal node: every lane in the warp reads the SAME
                         // node's children bounds -> single coalesced/broadcast
                         // read regardless of divergence in prune state.
                         int l = p_left[node];
                         int r = p_right[node];

                         auto ld2 = node_distance_sq(
                             px,
                             py,
                             pz,
                             p_min_x[l],
                             p_max_x[l],
                             p_min_y[l],
                             p_max_y[l],
                             p_min_z[l],
                             p_max_z[l]
                         );
                         auto rd2 = node_distance_sq(
                             px,
                             py,
                             pz,
                             p_min_x[r],
                             p_max_x[r],
                             p_min_y[r],
                             p_max_y[r],
                             p_min_z[r],
                             p_max_z[r]
                         );

                         if (sp < MAX_STACK_DEPTH - 2) {
                             if (ld2 <= rd2) {
                                 stack[sp++] = r;
                                 stack[sp++] = l;
                             } else {
                                 stack[sp++] = l;
                                 stack[sp++] = r;
                             }
                         }
                     }
                 }

                 if (active) {
                     size_t      offset = static_cast<size_t>(q_slot) * static_cast<size_t>(k);
                     heap_dist_t sorted_dist[_MAX_K_];
                     int         sorted_idx[_MAX_K_];
                     heap.extract_sorted(sorted_dist, sorted_idx, k);
                     for (int i = 0; i < k; ++i) {
                         if (sorted_idx[i] >= 0) {
                             dev_results[offset + i]      = static_cast<size_t>(sorted_idx[i]);
                             dev_result_dists[offset + i] = static_cast<dist_t>(sorted_dist[i]);
                         } else {
                             dev_results[offset + i]      = static_cast<size_t>(-1);
                             dev_result_dists[offset + i] = type_identity_max<dist_t>();
                         }
                     }
                 }
             }
         );
     }).wait();
}

template <int _MAX_K_ = 256, int _GROUP_WIDTH_ = 32>
void self_knn_query_grouped_large_k(
    sycl::queue   &q,
    const TreeSoA &tree,
    const int     *query_leaf_ids,
    int            num_queries,
    int            k,
    size_t        *dev_results,
    dist_t        *dev_result_dists,
    bool           exclude_self
) {
    static_assert(_MAX_K_ > 32, "use self_knn_query_grouped_small_k for k<=32");

    constexpr int WARP = _GROUP_WIDTH_;

    size_t   n       = tree.num_leaves;
    coord_t *p_min_x = tree.min_x, *p_max_x = tree.max_x;
    coord_t *p_min_y = tree.min_y, *p_max_y = tree.max_y;
    coord_t *p_min_z = tree.min_z, *p_max_z = tree.max_z;
    int     *p_left       = tree.left_child;
    int     *p_right      = tree.right_child;
    int     *p_orig_idx   = tree.orig_idx;
    int      num_internal = static_cast<int>(tree.num_internal);

    const coord_t *leaf_x = tree.min_x + tree.num_internal;
    const coord_t *leaf_y = tree.min_y + tree.num_internal;
    const coord_t *leaf_z = tree.min_z + tree.num_internal;

    // local memory footprint check -- see the sizing note below the code.
    size_t lm_needed = static_cast<size_t>(WARP) * k * (sizeof(heap_dist_t) + sizeof(int));
    size_t max_lm    = q.get_device().template get_info<sycl::info::device::local_mem_size>();
    if (lm_needed > max_lm) {
        throw std::runtime_error(
            "self_knn_query_grouped_large_k: k too large for local memory. "
            "Reduce k, shrink WARP, or split into more retry tiers."
        );
    }

    int num_groups = (num_queries + WARP - 1) / WARP;

    q.submit([&](sycl::handler &h) {
         sycl::local_accessor<heap_dist_t, 1> sh_dist(WARP * k, h);
         sycl::local_accessor<int, 1>         sh_idx(WARP * k, h);

         h.parallel_for(
             sycl::nd_range<1>(num_groups * WARP, WARP),
             [=](sycl::nd_item<1> item) [[sycl::reqd_sub_group_size(WARP)]] {
                 sycl::sub_group sg     = item.get_sub_group();
                 int             lane   = static_cast<int>(sg.get_local_id()[0]);
                 int             group  = static_cast<int>(item.get_group(0));
                 int             q_slot = group * WARP + lane;

                 bool active    = q_slot < num_queries;
                 int  leaf_rank = active ? (query_leaf_ids ? query_leaf_ids[q_slot] : q_slot) : 0;

                 coord_t px            = leaf_x[leaf_rank];
                 coord_t py            = leaf_y[leaf_rank];
                 coord_t pz            = leaf_z[leaf_rank];
                 int     self_leaf_idx = leaf_rank + num_internal;

                 LocalMaxHeap<heap_dist_t, int>::init(lane, sh_dist, sh_idx, k);
                 int count = 0; // private per-lane, mirrors RegisterMaxHeap::count

                 int stack[MAX_STACK_DEPTH];
                 int sp      = 0;
                 stack[sp++] = 0;

                 while (sp > 0) {
                     int node = stack[--sp];

                     auto d2 = node_distance_sq(
                         px,
                         py,
                         pz,
                         p_min_x[node],
                         p_max_x[node],
                         p_min_y[node],
                         p_max_y[node],
                         p_min_z[node],
                         p_max_z[node]
                     );

                     bool mine_pruned     = active && LocalMaxHeap<heap_dist_t, int>::should_prune(
                                                          lane, d2, sh_dist, count, k, k
                                                      );
                     bool anyone_needs_it = sycl::any_of_group(sg, active && !mine_pruned);
                     if (!anyone_needs_it) continue;

                     if (node >= static_cast<int>(n) - 1 && n > 1) {
                         if (active && !mine_pruned && !(exclude_self && node == self_leaf_idx)) {
#ifdef RETURN_ORIG_INDICES
                             LocalMaxHeap<heap_dist_t, int>::push(
                                 lane, d2, p_orig_idx[node], count, sh_dist, sh_idx, k, k
                             );
#else
                             LocalMaxHeap<heap_dist_t, int>::push(
                                 lane,
                                 d2,
                                 node - static_cast<int>(n - 1),
                                 count,
                                 sh_dist,
                                 sh_idx,
                                 k,
                                 k
                             );
#endif
                         }
                     } else if (n == 1) {
                         if (active && !mine_pruned && !exclude_self) {
                             LocalMaxHeap<heap_dist_t, int>::push(
                                 lane, d2, 0, count, sh_dist, sh_idx, k, k
                             );
                         }
                     } else {
                         int  l   = p_left[node];
                         int  r   = p_right[node];
                         auto ld2 = node_distance_sq(
                             px,
                             py,
                             pz,
                             p_min_x[l],
                             p_max_x[l],
                             p_min_y[l],
                             p_max_y[l],
                             p_min_z[l],
                             p_max_z[l]
                         );
                         auto rd2 = node_distance_sq(
                             px,
                             py,
                             pz,
                             p_min_x[r],
                             p_max_x[r],
                             p_min_y[r],
                             p_max_y[r],
                             p_min_z[r],
                             p_max_z[r]
                         );
                         if (sp < MAX_STACK_DEPTH - 2) {
                             if (ld2 <= rd2) {
                                 stack[sp++] = r;
                                 stack[sp++] = l;
                             } else {
                                 stack[sp++] = l;
                                 stack[sp++] = r;
                             }
                         }
                     }
                 }

                 if (active) {
                     size_t offset = static_cast<size_t>(q_slot) * static_cast<size_t>(k);

                     LocalMaxHeap<heap_dist_t, int>::sort_in_place(lane, count, sh_dist, sh_idx, k);

                     int base = lane * k;
                     for (int i = 0; i < k; ++i) {
                         if (i < count && sh_idx[base + i] >= 0) {
                             dev_results[offset + i]      = static_cast<size_t>(sh_idx[base + i]);
                             dev_result_dists[offset + i] = static_cast<dist_t>(sh_dist[base + i]);
                         } else {
                             dev_results[offset + i]      = static_cast<size_t>(-1);
                             dev_result_dists[offset + i] = type_identity_max<dist_t>();
                         }
                     }
                 }
             }
         );
     }).wait();
}

/**
 * @brief Returns the device's native sub-group size (warp/wavefront/SIMD width).
 *
 * Queries the SYCL device for supported sub_group_sizes and picks the maximum
 * native width (clamped to supported static dispatch widths) to ensure optimal
 * performance and avoid software sub-group emulation.
 */
inline int get_native_sub_group_width(const sycl::queue &q) {
    auto dev = q.get_device();
    if constexpr (std::is_same_v<coord_t, double>) {
        return dev.get_info<sycl::info::device::native_vector_width_double>();
    } else if constexpr (std::is_same_v<coord_t, float>) {
        return dev.get_info<sycl::info::device::native_vector_width_float>();
    } else if constexpr (sizeof(coord_t) == 8) { // uint64_t position rep
        return dev.get_info<sycl::info::device::native_vector_width_long>();
    } else { // 32-bit integer position rep
        return dev.get_info<sycl::info::device::native_vector_width_int>();
    }
}

template <int _MAX_K_ = 32>
inline void dispatch_self_knn_grouped_small_k(
    sycl::queue   &q,
    const TreeSoA &tree,
    const int     *query_leaf_ids,
    int            num_queries,
    int            k,
    size_t        *dev_results,
    dist_t        *dev_result_dists,
    bool           exclude_self
) {
    bool is_cpu = q.get_device().is_cpu();
    int  w      = is_cpu ? get_native_sub_group_width(q) : 32;

    switch (w) {
    case 4:
        self_knn_query_grouped_small_k<_MAX_K_, 4>(
            q, tree, query_leaf_ids, num_queries, k, dev_results, dev_result_dists, exclude_self
        );
        break;
    case 8:
        self_knn_query_grouped_small_k<_MAX_K_, 8>(
            q, tree, query_leaf_ids, num_queries, k, dev_results, dev_result_dists, exclude_self
        );
        break;
    case 16:
        self_knn_query_grouped_small_k<_MAX_K_, 16>(
            q, tree, query_leaf_ids, num_queries, k, dev_results, dev_result_dists, exclude_self
        );
        break;
    case 64:
        self_knn_query_grouped_small_k<_MAX_K_, 64>(
            q, tree, query_leaf_ids, num_queries, k, dev_results, dev_result_dists, exclude_self
        );
        break;
    case 32:
    default:
        self_knn_query_grouped_small_k<_MAX_K_, 32>(
            q, tree, query_leaf_ids, num_queries, k, dev_results, dev_result_dists, exclude_self
        );
        break;
    }
}

template <int _MAX_K_ = 256>
inline void dispatch_self_knn_grouped_large_k(
    sycl::queue   &q,
    const TreeSoA &tree,
    const int     *query_leaf_ids,
    int            num_queries,
    int            k,
    size_t        *dev_results,
    dist_t        *dev_result_dists,
    bool           exclude_self
) {
    bool is_cpu = q.get_device().is_cpu();
    int  w      = is_cpu ? get_native_sub_group_width(q) : 32;

    switch (w) {
    case 4:
        self_knn_query_grouped_large_k<_MAX_K_, 4>(
            q, tree, query_leaf_ids, num_queries, k, dev_results, dev_result_dists, exclude_self
        );
        break;
    case 8:
        self_knn_query_grouped_large_k<_MAX_K_, 8>(
            q, tree, query_leaf_ids, num_queries, k, dev_results, dev_result_dists, exclude_self
        );
        break;
    case 16:
        self_knn_query_grouped_large_k<_MAX_K_, 16>(
            q, tree, query_leaf_ids, num_queries, k, dev_results, dev_result_dists, exclude_self
        );
        break;
    case 64:
        self_knn_query_grouped_large_k<_MAX_K_, 64>(
            q, tree, query_leaf_ids, num_queries, k, dev_results, dev_result_dists, exclude_self
        );
        break;
    case 32:
    default:
        self_knn_query_grouped_large_k<_MAX_K_, 32>(
            q, tree, query_leaf_ids, num_queries, k, dev_results, dev_result_dists, exclude_self
        );
        break;
    }
}

// Dispatch, mirroring knn_query's existing k<=32 / k>32 split:
template <int _MAX_K_ = 256>
inline void self_knn_query(
    sycl::queue   &q,
    const TreeSoA &tree,
    int            k,
    size_t        *results,
    dist_t        *result_dists,
    bool           exclude_self = true
) {
    size_t n = tree.num_leaves;
    if (n == 0) return;
    if (k > _MAX_K_) {
        throw std::invalid_argument(
            "k exceeds _MAX_K_ template parameter. Instantiate self_knn_query with larger _MAX_K_."
        );
    }
    if (k <= 32) {
        dispatch_self_knn_grouped_small_k<32>(
            q, tree, nullptr, static_cast<int>(n), k, results, result_dists, exclude_self
        );
    } else {
        dispatch_self_knn_grouped_large_k<_MAX_K_>(
            q, tree, nullptr, static_cast<int>(n), k, results, result_dists, exclude_self
        );
    }
}

template <int _MAX_K_ = 256>
inline void self_knn_query_subset(
    sycl::queue   &q,
    const TreeSoA &tree,
    const int     *leaf_ids,
    int            num_ids,
    int            k,
    size_t        *results,
    dist_t        *result_dists,
    bool           exclude_self = true
) {
    if (num_ids == 0) return;
    if (k > _MAX_K_) {
        throw std::invalid_argument(
            "k exceeds _MAX_K_ template parameter. Instantiate self_knn_query_subset with larger "
            "_MAX_K_."
        );
    }
    if (k <= 32) {
        dispatch_self_knn_grouped_small_k<32>(
            q, tree, leaf_ids, num_ids, k, results, result_dists, exclude_self
        );
    } else {
        dispatch_self_knn_grouped_large_k<_MAX_K_>(
            q, tree, leaf_ids, num_ids, k, results, result_dists, exclude_self
        );
    }
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
 * @note result_counts[i] records total matching particles found. If result_counts[i] >
 * max_results_per_query, the output results buffer is truncated to max_results_per_query.
 */
inline void range_query(
    sycl::queue   &q,
    const TreeSoA &tree,
    const coord_t *qx,
    const coord_t *qy,
    const coord_t *qz,
    const coord_t *r_min,
    const coord_t *r_max,
    int            num_queries,
    int           *results,
    int           *result_counts,
    int            max_results_per_query
) {
    size_t n = tree.num_leaves;
    if (n == 0 || num_queries == 0) return;

    bool qx_alloc = false, qy_alloc = false, qz_alloc = false;
    bool r_min_alloc = false, r_max_alloc = false;
    bool res_alloc = false, counts_alloc = false;

    const coord_t *dev_qx      = ensure_device_readable(q, qx, num_queries, qx_alloc);
    const coord_t *dev_qy      = ensure_device_readable(q, qy, num_queries, qy_alloc);
    const coord_t *dev_qz      = ensure_device_readable(q, qz, num_queries, qz_alloc);
    const coord_t *dev_r_min   = ensure_device_readable(q, r_min, num_queries, r_min_alloc);
    const coord_t *dev_r_max   = ensure_device_readable(q, r_max, num_queries, r_max_alloc);
    int           *dev_results = ensure_device_writable(
        q,
        results,
        static_cast<size_t>(num_queries) * static_cast<size_t>(max_results_per_query),
        res_alloc
    );
    int *dev_result_counts =
        ensure_device_writable(q, result_counts, static_cast<size_t>(num_queries), counts_alloc);

    coord_t *p_min_x = tree.min_x, *p_max_x = tree.max_x;
    coord_t *p_min_y = tree.min_y, *p_max_y = tree.max_y;
    coord_t *p_min_z = tree.min_z, *p_max_z = tree.max_z;
    int     *p_left_child = tree.left_child, *p_right_child = tree.right_child;
    int     *p_orig_idx = tree.orig_idx;

    q.parallel_for(sycl::range<1>(num_queries), [=](sycl::id<1> idx) {
         size_t  qi = idx[0];
         coord_t px = dev_qx[qi], py = dev_qy[qi], pz = dev_qz[qi];
#if defined(FASTTREE_INTEGER_COORDS)
         // r_min/r_max are integer coordinate differences (same space as coords).
         // Shift before squaring to match _DIST_SHIFT applied inside node_distance_sq.
         uint64_t rm_i = get_lo_word(dev_r_min[qi]) >> _DIST_SHIFT;
         uint64_t RM_i = get_lo_word(dev_r_max[qi]) >> _DIST_SHIFT;
         uint64_t RM2  = RM_i * RM_i;
         uint64_t rm2  = rm_i * rm_i;
#else
     double rm_d = static_cast<double>(dev_r_min[qi]);
     double RM_d = static_cast<double>(dev_r_max[qi]);
     double RM2  = RM_d * RM_d;
     double rm2  = rm_d * rm_d;
#endif

         int stack[MAX_STACK_DEPTH];
         int stack_ptr = 0;

         if (n == 1) {
             stack[stack_ptr++] = 0; // Only one leaf
         } else {
             stack[stack_ptr++] = 0; // Root is internal node 0
         }

         int    count  = 0;
         size_t offset = qi * static_cast<size_t>(max_results_per_query);
         while (stack_ptr > 0) {
             int node_idx = stack[--stack_ptr];

             coord_t bmin_x = p_min_x[node_idx], bmax_x = p_max_x[node_idx];
             coord_t bmin_y = p_min_y[node_idx], bmax_y = p_max_y[node_idx];
             coord_t bmin_z = p_min_z[node_idx], bmax_z = p_max_z[node_idx];

             auto d2 = node_distance_sq(px, py, pz, bmin_x, bmax_x, bmin_y, bmax_y, bmin_z, bmax_z);

             if (d2 <= RM2) {
                 if (node_idx >= (int)n - 1 && n > 1) { // Leaf node
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
                 } else if (n == 1) { // Single leaf case
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
    copy_back_and_free(
        q,
        dev_results,
        results,
        static_cast<size_t>(num_queries) * static_cast<size_t>(max_results_per_query),
        res_alloc
    );
    copy_back_and_free(
        q, dev_result_counts, result_counts, static_cast<size_t>(num_queries), counts_alloc
    );
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
 * @param[in,out] tree The output tree structure to build.
 * @param[in] bbox Optional precomputed bounding box. If nullptr, it will be computed.
 * @param[in] key Optional precomputed space-filling curve keys. If nullptr, it will be
 * computed.
 *
 * @note PERIODIC_BC only affects node_distance_sq (query time).
 * The BVH tree itself is built on the NON-WRAPPED coordinate space.
 * Leaf bounding boxes represent actual particle positions in [0, 2^B).
 * For queries near the boundary (within r_max of the wrap point),
 * the traversal will correctly find the closest image of each bounding box
 * via the toroidal distance metric in node_distance_sq, so no duplicate
 * tree or particle replication is required.
 * Limitation: if a single leaf's extent spans more than half the domain
 * (which cannot happen for point particles), periodic detection breaks.
 */
inline void build_bvh(
    sycl::queue              &q,
    const particles<coord_t> &p,
    TreeSoA                  &tree,
    BoundingBox<coord_t>     *bbox = nullptr,
    sfc_key                  *key  = nullptr
) {
    size_t n = p.pos_x.size();
    if (n == 0) return;

    if (n > static_cast<size_t>(std::numeric_limits<int>::max() / 2)) {
        throw std::runtime_error(
            "Particle count n exceeds maximum supported tree size (INT_MAX / 2)."
        );
    }

    // 1. Single-Pass Host-to-Device Staging Buffer
    bool           x_alloc = false, y_alloc = false, z_alloc = false;
    const coord_t *dev_pos_x = ensure_device_readable(q, p.pos_x.data(), n, x_alloc);
    const coord_t *dev_pos_y = ensure_device_readable(q, p.pos_y.data(), n, y_alloc);
    const coord_t *dev_pos_z = ensure_device_readable(q, p.pos_z.data(), n, z_alloc);

    // Compute Bounding Box using parallel GPU reduction if not provided
    BoundingBox<coord_t> local_bbox(0, 0, 0, 0, 0, 0);
    if (bbox == nullptr) {
        local_bbox = compute_bbox(q, dev_pos_x, dev_pos_y, dev_pos_z, n);
        bbox       = &local_bbox;
    }

    // 2. Allocate and initialize SFC key array on device
    bool     key_owner = (key == nullptr);
    sfc_key *d_smk     = key_owner ? sycl::malloc_shared<sfc_key>(n, q) : key;
    if (key_owner) { sfc_encode(q, dev_pos_x, dev_pos_y, dev_pos_z, n, d_smk, *bbox); }

    // 3. Allocate and initialize index array [0, 1, ..., n-1]
    size_t *d_indices = sycl::malloc_shared<size_t>(n, q);
    q.parallel_for(sycl::range<1>(n), [=](sycl::id<1> idx) { d_indices[idx] = idx[0]; });
    q.wait();

    // 4. Single-Pass Full GPU Sort using oneDPL
#if (3 * BITS_PER_DIMENSION) <= 64
    uint64_t *d_sort_keys = sycl::malloc_shared<uint64_t>(n, q);
    sfc_key  *d_smk_ptr   = d_smk;
    q.parallel_for(sycl::range<1>(n), [=](sycl::id<1> idx) {
         size_t i       = idx[0];
         d_sort_keys[i] = to_sort_key(d_smk_ptr[i]);
     }).wait();

    auto zip2_begin = oneapi::dpl::make_zip_iterator(d_sort_keys, d_smk, d_indices);
    auto zip2_end   = zip2_begin + n;
    oneapi::dpl::sort(
        oneapi::dpl::execution::make_device_policy(q), zip2_begin, zip2_end, [](auto a, auto b) {
            return oneapi::dpl::get<0>(a) < oneapi::dpl::get<0>(b);
        }
    );
    q.wait();
    sycl::free(d_sort_keys, q);
#else
    auto policy    = oneapi::dpl::execution::make_device_policy(q);
    auto zip_begin = oneapi::dpl::make_zip_iterator(d_smk, d_indices);
    auto zip_end   = zip_begin + n;

    oneapi::dpl::sort(policy, zip_begin, zip_end, [](auto a, auto b) {
        return oneapi::dpl::get<0>(a) < oneapi::dpl::get<0>(b);
    });
    q.wait();
#endif

    // Prepare ID and Ghost arrays
    const uint32_t       *p_id = nullptr;
    std::vector<uint32_t> temp_id;
    bool                  id_alloc = false;
    if (p.id.empty()) {
        temp_id.resize(n);
        std::iota(temp_id.begin(), temp_id.end(), 0u);
        p_id = temp_id.data();
    } else {
        p_id = p.id.data();
    }
    const uint32_t *dev_p_id = ensure_device_readable(q, p_id, n, id_alloc);

    const int8_t       *p_ghost = nullptr;
    std::vector<int8_t> temp_ghost;
    bool                ghost_alloc = false;
    if (p.is_ghost.empty()) {
        temp_ghost.resize(n, 0);
        p_ghost = temp_ghost.data();
    } else {
        p_ghost = p.is_ghost.data();
    }
    const int8_t *dev_p_ghost = ensure_device_readable(q, p_ghost, n, ghost_alloc);

    // 5. Direct Coordinate and Attribute Reordering into Tree Leaves on GPU
    coord_t  *tree_min_x = tree.min_x, *tree_max_x = tree.max_x;
    coord_t  *tree_min_y = tree.min_y, *tree_max_y = tree.max_y;
    coord_t  *tree_min_z = tree.min_z, *tree_max_z = tree.max_z;
    uint32_t *tree_id       = tree.id;
    int8_t   *tree_ghost    = tree.is_ghost;
    int      *tree_orig_idx = tree.orig_idx;

    q.parallel_for(sycl::range<1>(n), [=](sycl::id<1> idx) {
         size_t i             = idx[0];
         size_t orig_idx      = d_indices[i];
         int    leaf_idx      = i + n - 1;
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

    if (key_owner) { sycl::free(d_smk, q); }
    sycl::free(d_indices, q);
}

} // namespace fasttree

#endif // SYCL_FASTTREE_HLBVH_HPPREE_HLBVH_HPP