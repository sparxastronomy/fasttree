#ifndef SYCL_FASTTREE_SFC_PEANO_HILBERT_HPP
#define SYCL_FASTTREE_SFC_PEANO_HILBERT_HPP

#include <sycl/sycl.hpp>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <type_traits>

namespace fasttree {

// ================================================================
// SECTION 0: Always define uint128_t emulation struct
// ================================================================

/**
 * @brief Emulated 128-bit unsigned integer structure for SYCL device and host execution.
 *
 * Provides bitwise shifts, arithmetic operators (+, -), comparison operators, and double
 * conversions. Used for 128-bit integer position representation and multi-word space-filling curve
 * keys.
 */
struct uint128_t {
    uint64_t lo = 0; ///< Lower 64 bits
    uint64_t hi = 0; ///< Upper 64 bits

    constexpr uint128_t() noexcept = default;
    constexpr uint128_t(uint64_t h, uint64_t l) noexcept : lo(l), hi(h) {}
    constexpr uint128_t(uint64_t v) noexcept : lo(v), hi(0) {}

    /**
     * @brief Left shift operator with boundary handling (n <= 0, n = 64, n > 64, n >= 128).
     * @note Standard C++ bit shifts >= 64 bits on 64-bit integers produce undefined behavior.
     *       This implementation explicitly handles all shift range cases.
     */
    constexpr uint128_t &operator<<=(int n) noexcept {
        if (n <= 0) {
            // do nothing
        } else if (n >= 128) {
            hi = 0;
            lo = 0;
        } else if (n == 64) {
            hi = lo;
            lo = 0;
        } else if (n > 64) {
            hi = lo << (n - 64);
            lo = 0;
        } else {
            hi = (hi << n) | (lo >> (64 - n));
            lo <<= n;
        }
        return *this;
    }

    constexpr uint128_t operator-(const uint128_t &o) const noexcept {
        uint64_t new_lo = lo - o.lo;
        uint64_t borrow = (lo < o.lo) ? 1u : 0u;
        return uint128_t(hi - o.hi - borrow, new_lo);
    }

    constexpr uint128_t operator+(const uint128_t &o) const noexcept {
        uint64_t new_lo = lo + o.lo;
        uint64_t carry  = (new_lo < lo) ? 1u : 0u;
        return uint128_t(hi + o.hi + carry, new_lo);
    }

    constexpr uint128_t operator+(uint64_t v) const noexcept { return *this + uint128_t(v); }
    constexpr uint128_t operator-(uint64_t v) const noexcept { return *this - uint128_t(v); }

    constexpr uint128_t &operator|=(uint64_t v) noexcept {
        lo |= v;
        return *this;
    }

    constexpr uint64_t top3() const noexcept { return hi >> 61; }

    constexpr uint128_t operator^(const uint128_t &o) const noexcept {
        return uint128_t(hi ^ o.hi, lo ^ o.lo);
    }

    explicit operator double() const noexcept {
        return static_cast<double>(hi) * 1.8446744073709551616e19 + static_cast<double>(lo);
    }

    constexpr bool operator==(const uint128_t &o) const noexcept {
        return hi == o.hi && lo == o.lo;
    }
    constexpr bool operator!=(const uint128_t &o) const noexcept { return !(*this == o); }
    constexpr bool operator<(const uint128_t &o) const noexcept {
        if (hi != o.hi) return hi < o.hi;
        return lo < o.lo;
    }
    constexpr bool operator>(const uint128_t &o) const noexcept { return o < *this; }
    constexpr bool operator<=(const uint128_t &o) const noexcept { return !(o < *this); }
    constexpr bool operator>=(const uint128_t &o) const noexcept { return !(*this < o); }
};

inline std::ostream &operator<<(std::ostream &os, const uint128_t &v) {
    if (v.hi == 0) return os << v.lo;
    return os << "0x" << std::hex << v.hi << std::setfill('0') << std::setw(16) << v.lo << std::dec;
}

/**
 * @brief Helper functions to extract the lowest 64-bit word across integral types and uint128_t.
 */
inline uint64_t get_lo_word(uint32_t w) noexcept {
    return w;
}
inline uint64_t get_lo_word(uint64_t w) noexcept {
    return w;
}
inline uint64_t get_lo_word(const uint128_t &w) noexcept {
    return w.lo;
}

// ================================================================
// SECTION 1: Configuration
// Exactly one of these must be defined
// ================================================================

#if !defined(POSITIONS_IN_32BIT) && !defined(POSITIONS_IN_64BIT) && !defined(POSITIONS_IN_128BIT)
#ifdef FASTTREE_DOUBLE_PRECISION
#define POSITIONS_IN_64BIT
#else
#define POSITIONS_IN_32BIT
#endif
#endif

#if defined(POSITIONS_IN_32BIT) && defined(POSITIONS_IN_64BIT)
#error "Cannot define both POSITIONS_IN_32BIT and POSITIONS_IN_64BIT"
#endif
#if defined(POSITIONS_IN_32BIT) && defined(POSITIONS_IN_128BIT)
#error "Cannot define both POSITIONS_IN_32BIT and POSITIONS_IN_128BIT"
#endif
#if defined(POSITIONS_IN_64BIT) && defined(POSITIONS_IN_128BIT)
#error "Cannot define both POSITIONS_IN_64BIT and POSITIONS_IN_128BIT"
#endif

// ================================================================
// SECTION 2: Type Selection
// ================================================================

#if defined(POSITIONS_IN_32BIT)
using MyIntPosType       = uint32_t;
using MyIntPosTypeSigned = int32_t;
#define BITS_FOR_POSITIONS 32
#define MAX_BITS_PER_DIM 32

#elif defined(POSITIONS_IN_64BIT)
using MyIntPosType       = uint64_t;
using MyIntPosTypeSigned = int64_t;
#define BITS_FOR_POSITIONS 64
#define MAX_BITS_PER_DIM 64

#elif defined(POSITIONS_IN_128BIT)
using MyIntPosType       = uint128_t;
using MyIntPosTypeSigned = int64_t;
#define BITS_FOR_POSITIONS 128
#define MAX_BITS_PER_DIM 128
#endif

// ================================================================
// SECTION 3: BITS_PER_DIMENSION validation
// ================================================================

#ifndef BITS_PER_DIMENSION
#if defined(POSITIONS_IN_32BIT)
#define BITS_PER_DIMENSION 21 // 3x21=63 fits uint64_t sort key
#define SHIFT_BITS 11         // 32-21=11 bits of headroom for integer distance shift
constexpr double NORM = 1.0 / static_cast<double>(1ULL << BITS_PER_DIMENSION);
#elif defined(POSITIONS_IN_64BIT)
#define BITS_PER_DIMENSION 42 // 3x42=126 fits 128-bit key
#define SHIFT_BITS 22         // 64-42=22 bits of headroom for integer distance shift
constexpr double NORM = 1.0 / static_cast<double>(1ULL << BITS_PER_DIMENSION);
#elif defined(POSITIONS_IN_128BIT)
#define BITS_PER_DIMENSION 64 // 3x64=192 needs {hs,is,ls}
#define SHIFT_BITS 64         // 128-64=64 bits of headroom for integer distance shift
// 1ULL << 64 is a no-op
constexpr double NORM = 1.0 / (static_cast<double>(1ULL << 32) * static_cast<double>(1ULL << 32));
#endif
#endif

// Integer distance type and overflow-prevention shift
#ifdef FASTTREE_INTEGER_COORDS
using dist_t              = uint64_t;
constexpr int _DIST_SHIFT = (BITS_PER_DIMENSION > 31) ? (BITS_PER_DIMENSION - 31) : 0;
#else
// In float/double mode dist_t is not used for integer distances
using dist_t              = double;
constexpr int _DIST_SHIFT = 0;
#endif

static_assert(BITS_PER_DIMENSION >= 1, "BITS_PER_DIMENSION must be at least 1");
static_assert(
    BITS_PER_DIMENSION <= MAX_BITS_PER_DIM,
    "BITS_PER_DIMENSION exceeds coordinate type width — "
    "increase position precision (POSITIONS_IN_64BIT or POSITIONS_IN_128BIT)"
);
static_assert(
    BITS_PER_DIMENSION <= 64,
    "`node_distance_sq` integer path assumes coordinate gap fits in uint64_t. "
    "BITS_PER_DIMENSION > 64 with POSITIONS_IN_128BIT is not supported. "
    "42 bits/dim covers exascale galaxy formation; 64 bits covers the next decade."
);

// ================================================================
// SECTION 4: Key type
// Total key bits = 3 x BITS_PER_DIMENSION
// Stored in minimum number of MyIntPosType words
// ================================================================

static constexpr int KEY_TOTAL_BITS = 3 * BITS_PER_DIMENSION;
constexpr int KEY_WORDS_NEEDED = (KEY_TOTAL_BITS + BITS_FOR_POSITIONS - 1) / BITS_FOR_POSITIONS;

static_assert(KEY_WORDS_NEEDED <= 3, "Internal error: key requires more than 3 words");

/**
 * @brief Multi-word Space-Filling Curve (SFC) key container.
 *
 * Holds up to 3 words {hs, is, ls} of type MyIntPosType to represent 3D SFC keys
 * spanning up to 3 x BITS_PER_DIMENSION bits.
 */
struct sfc_key {
    MyIntPosType hs = {}; ///< High word
    MyIntPosType is = {}; ///< Intermediate word
    MyIntPosType ls = {}; ///< Low word

    constexpr bool operator<(const sfc_key &o) const noexcept {
        if (hs != o.hs) return hs < o.hs;
        if (is != o.is) return is < o.is;
        return ls < o.ls;
    }
    constexpr bool operator==(const sfc_key &o) const noexcept {
        return hs == o.hs && is == o.is && ls == o.ls;
    }
    constexpr bool operator!=(const sfc_key &o) const noexcept { return !(*this == o); }
};

#if defined(POSITIONS_IN_128BIT)
inline sfc_key sfc_key_max_impl(uint128_t *) noexcept {
    sfc_key k;
    k.hs = uint128_t(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull);
    k.is = uint128_t(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull);
    k.ls = uint128_t(0xFFFFFFFFFFFFFFFFull, 0xFFFFFFFFFFFFFFFFull);
    return k;
}
#endif

#if defined(POSITIONS_IN_64BIT)
inline sfc_key sfc_key_max_impl(uint64_t *) noexcept {
    sfc_key k;
    k.hs = 0xFFFFFFFFFFFFFFFFull;
    k.is = 0xFFFFFFFFFFFFFFFFull;
    k.ls = 0xFFFFFFFFFFFFFFFFull;
    return k;
}
#endif

#if defined(POSITIONS_IN_32BIT)
inline sfc_key sfc_key_max_impl(uint32_t *) noexcept {
    sfc_key k;
    k.hs = 0xFFFFFFFFu;
    k.is = 0xFFFFFFFFu;
    k.ls = 0xFFFFFFFFu;
    return k;
}
#endif

/**
 * @brief Returns the maximum possible sfc_key value.
 */
inline sfc_key sfc_key_max() noexcept {
    MyIntPosType *tag = nullptr;
    return sfc_key_max_impl(tag);
}

#if (3 * BITS_PER_DIMENSION) <= 64
using sort_key_t = uint64_t;
/**
 * @brief Extracts a 64-bit primitive sort key for high-performance single-pass GPU radix sort.
 * @note Used when KEY_TOTAL_BITS <= 64. For 64-bit integer positions (MyIntPosType = uint64_t),
 *       returns k.ls directly since the 63-bit key resides entirely in k.ls.
 */
inline sort_key_t to_sort_key(const sfc_key &k) noexcept {
    if constexpr (std::is_same_v<MyIntPosType, uint32_t>) {
        return (static_cast<uint64_t>(k.is) << 32) | static_cast<uint64_t>(k.ls);
    } else if constexpr (std::is_same_v<MyIntPosType, uint64_t>) {
        return k.ls;
    } else {
        return get_lo_word(k.ls);
    }
}
#else
using sort_key_t = sfc_key;
inline sort_key_t to_sort_key(const sfc_key &k) noexcept {
    return k;
}
#endif

/**
 * @brief Extracts the top m bits of an sfc_key as a bucket identifier for domain decomposition.
 * @param[in] k Space-filling curve key.
 * @param[in] m Number of top bits to extract.
 * @return 32-bit bucket identifier.
 */
inline uint32_t extract_bucket_id(const sfc_key &k, int m) noexcept {
#if (3 * BITS_PER_DIMENSION) <= 64
    uint64_t sort_k = to_sort_key(k);
    int      shift  = (KEY_TOTAL_BITS > m) ? (KEY_TOTAL_BITS - m) : 0;
    return static_cast<uint32_t>(sort_k >> shift);
#elif defined(POSITIONS_IN_64BIT)
    int shift = (63 > m) ? (63 - m) : 0;
    return static_cast<uint32_t>(k.is >> shift);
#else
    int shift = (64 > m) ? (64 - m) : 0;
    return static_cast<uint32_t>(k.is.lo >> shift);
#endif
}

// ================================================================
// SECTION 5: Coordinate type for SFC encoding
// ================================================================

using sfc1D = MyIntPosType;

// ================================================================
// SECTION 6: AREPO lookup tables (verbatim, machine-verified)
// ================================================================

static constexpr uint8_t rottable3[48][8] = {
    {36, 28, 25, 27, 10, 10, 25, 27}, {29, 11, 24, 24, 37, 11, 26, 26},
    {8, 8, 25, 27, 30, 38, 25, 27},   {9, 39, 24, 24, 9, 31, 26, 26},
    {40, 24, 44, 32, 40, 6, 44, 6},   {25, 7, 33, 7, 41, 41, 45, 45},
    {4, 42, 4, 46, 26, 42, 34, 46},   {43, 43, 47, 47, 5, 27, 5, 35},
    {33, 35, 36, 28, 33, 35, 2, 2},   {32, 32, 29, 3, 34, 34, 37, 3},
    {33, 35, 0, 0, 33, 35, 30, 38},   {32, 32, 1, 39, 34, 34, 1, 31},
    {24, 42, 32, 46, 14, 42, 14, 46}, {43, 43, 47, 47, 25, 15, 33, 15},
    {40, 12, 44, 12, 40, 26, 44, 34}, {13, 27, 13, 35, 41, 41, 45, 45},
    {28, 41, 28, 22, 38, 43, 38, 22}, {42, 40, 23, 23, 29, 39, 29, 39},
    {41, 36, 20, 36, 43, 30, 20, 30}, {37, 31, 37, 31, 42, 40, 21, 21},
    {28, 18, 28, 45, 38, 18, 38, 47}, {19, 19, 46, 44, 29, 39, 29, 39},
    {16, 36, 45, 36, 16, 30, 47, 30}, {37, 31, 37, 31, 17, 17, 46, 44},
    {12, 4, 1, 3, 34, 34, 1, 3},      {5, 35, 0, 0, 13, 35, 2, 2},
    {32, 32, 1, 3, 6, 14, 1, 3},      {33, 15, 0, 0, 33, 7, 2, 2},
    {16, 0, 20, 8, 16, 30, 20, 30},   {1, 31, 9, 31, 17, 17, 21, 21},
    {28, 18, 28, 22, 2, 18, 10, 22},  {19, 19, 23, 23, 29, 3, 29, 11},
    {9, 11, 12, 4, 9, 11, 26, 26},    {8, 8, 5, 27, 10, 10, 13, 27},
    {9, 11, 24, 24, 9, 11, 6, 14},    {8, 8, 25, 15, 10, 10, 25, 7},
    {0, 18, 8, 22, 38, 18, 38, 22},   {19, 19, 23, 23, 1, 39, 9, 39},
    {16, 36, 20, 36, 16, 2, 20, 10},  {37, 3, 37, 11, 17, 17, 21, 21},
    {4, 17, 4, 46, 14, 19, 14, 46},   {18, 16, 47, 47, 5, 15, 5, 15},
    {17, 12, 44, 12, 19, 6, 44, 6},   {13, 7, 13, 7, 18, 16, 45, 45},
    {4, 42, 4, 21, 14, 42, 14, 23},   {43, 43, 22, 20, 5, 15, 5, 15},
    {40, 12, 21, 12, 40, 6, 23, 6},   {13, 7, 13, 7, 41, 41, 22, 20}
};

static constexpr uint8_t subpix3[48][8] = {
    {0, 7, 1, 6, 3, 4, 2, 5}, {7, 4, 6, 5, 0, 3, 1, 2}, {4, 3, 5, 2, 7, 0, 6, 1},
    {3, 0, 2, 1, 4, 7, 5, 6}, {1, 0, 6, 7, 2, 3, 5, 4}, {0, 3, 7, 4, 1, 2, 6, 5},
    {3, 2, 4, 5, 0, 1, 7, 6}, {2, 1, 5, 6, 3, 0, 4, 7}, {6, 1, 7, 0, 5, 2, 4, 3},
    {1, 2, 0, 3, 6, 5, 7, 4}, {2, 5, 3, 4, 1, 6, 0, 7}, {5, 6, 4, 7, 2, 1, 3, 0},
    {7, 6, 0, 1, 4, 5, 3, 2}, {6, 5, 1, 2, 7, 4, 0, 3}, {5, 4, 2, 3, 6, 7, 1, 0},
    {4, 7, 3, 0, 5, 6, 2, 1}, {6, 7, 5, 4, 1, 0, 2, 3}, {7, 0, 4, 3, 6, 1, 5, 2},
    {0, 1, 3, 2, 7, 6, 4, 5}, {1, 6, 2, 5, 0, 7, 3, 4}, {2, 3, 1, 0, 5, 4, 6, 7},
    {3, 4, 0, 7, 2, 5, 1, 6}, {4, 5, 7, 6, 3, 2, 0, 1}, {5, 2, 6, 1, 4, 3, 7, 0},
    {7, 0, 6, 1, 4, 3, 5, 2}, {0, 3, 1, 2, 7, 4, 6, 5}, {3, 4, 2, 5, 0, 7, 1, 6},
    {4, 7, 5, 6, 3, 0, 2, 1}, {6, 7, 1, 0, 5, 4, 2, 3}, {7, 4, 0, 3, 6, 5, 1, 2},
    {4, 5, 3, 2, 7, 6, 0, 1}, {5, 6, 2, 1, 4, 7, 3, 0}, {1, 6, 0, 7, 2, 5, 3, 4},
    {6, 5, 7, 4, 1, 2, 0, 3}, {5, 2, 4, 3, 6, 1, 7, 0}, {2, 1, 3, 0, 5, 6, 4, 7},
    {0, 1, 7, 6, 3, 2, 4, 5}, {1, 2, 6, 5, 0, 3, 7, 4}, {2, 3, 5, 4, 1, 0, 6, 7},
    {3, 0, 4, 7, 2, 1, 5, 6}, {1, 0, 2, 3, 6, 7, 5, 4}, {0, 7, 3, 4, 1, 6, 2, 5},
    {7, 6, 4, 5, 0, 1, 3, 2}, {6, 1, 5, 2, 7, 0, 4, 3}, {5, 4, 6, 7, 2, 3, 1, 0},
    {4, 3, 7, 0, 5, 2, 6, 1}, {3, 2, 0, 1, 4, 5, 7, 6}, {2, 5, 1, 6, 3, 4, 0, 7}
};

// ================================================================
// SECTION 7: Key shift-register accumulation
// ================================================================

inline uint64_t top3_to_bottom(uint32_t w) noexcept {
    return w >> 29;
}
inline uint64_t top3_to_bottom(uint64_t w) noexcept {
    return w >> 61;
}
inline uint64_t top3_to_bottom(const uint128_t &w) noexcept {
    return w.top3();
}

/**
 * @brief Pushes 3 bits of subpixel octant index into the multi-word sfc_key shift-register {hs, is,
 * ls}.
 */
inline void key_push_3bits(sfc_key &key, uint8_t chunk) noexcept {
    key.hs <<= 3;
    key.hs |= top3_to_bottom(key.is);

    key.is <<= 3;
    key.is |= top3_to_bottom(key.ls);

    key.ls <<= 3;
    key.ls |= chunk;
}

// ================================================================
// SECTION 8: Quantization (IEEE 754 Bit Reinterpretation)
// ================================================================

/**
 * @brief Quantizes a normalized floating-point coordinate mapped to [1.0, 2.0) into discrete
 * integer bits.
 * @note Reinterprets IEEE 754 double mantissa bits using sycl::bit_cast to perform fast hardware
 * quantization.
 */
inline uint32_t convert_to_sfc1d_impl(double d, uint32_t * /*tag*/) noexcept {
    const uint64_t     bits  = sycl::bit_cast<uint64_t>(d);
    constexpr int      b_dim = (BITS_PER_DIMENSION < 52) ? BITS_PER_DIMENSION : 52;
    constexpr int      shift = 52 - b_dim;
    constexpr uint32_t mask  = (BITS_PER_DIMENSION < 32)
                                   ? static_cast<uint32_t>((1ULL << BITS_PER_DIMENSION) - 1u)
                                   : 0xFFFFFFFFu;
    return static_cast<uint32_t>(((bits & 0x000FFFFFFFFFFFFFull) >> shift) & mask);
}

inline uint64_t convert_to_sfc1d_impl(double d, uint64_t * /*tag*/) noexcept {
    const uint64_t     bits  = sycl::bit_cast<uint64_t>(d);
    constexpr int      b_dim = (BITS_PER_DIMENSION < 52) ? BITS_PER_DIMENSION : 52;
    constexpr int      shift = 52 - b_dim;
    constexpr uint64_t mask =
        (BITS_PER_DIMENSION < 64) ? ((uint64_t(1) << BITS_PER_DIMENSION) - 1u) : ~uint64_t(0);
    return ((bits & 0x000FFFFFFFFFFFFFull) >> shift) & mask;
}

inline uint128_t convert_to_sfc1d_impl(double d, uint128_t * /*tag*/) noexcept {
    const uint64_t bits      = sycl::bit_cast<uint64_t>(d);
    const uint64_t mantissa  = bits & 0x000FFFFFFFFFFFFFull;
    constexpr int  used_bits = (BITS_PER_DIMENSION < 52) ? BITS_PER_DIMENSION : 52;
    constexpr int  shift     = 52 - used_bits;
    uint64_t       trimmed   = mantissa >> shift;
    uint128_t      result;
    result.lo = trimmed << (64 - used_bits);
    result.hi = 0;
    return result;
}

inline uint32_t convert_to_sfc1d_impl(float f, uint32_t *tag) noexcept {
    return convert_to_sfc1d_impl(static_cast<double>(f), tag);
}
inline uint64_t convert_to_sfc1d_impl(float f, uint64_t *tag) noexcept {
    return convert_to_sfc1d_impl(static_cast<double>(f), tag);
}
inline uint128_t convert_to_sfc1d_impl(float f, uint128_t *tag) noexcept {
    return convert_to_sfc1d_impl(static_cast<double>(f), tag);
}

inline MyIntPosType convert_to_sfc1d(double d) noexcept {
    MyIntPosType *tag = nullptr;
    return convert_to_sfc1d_impl(d, tag);
}

inline MyIntPosType convert_to_sfc1d(float f) noexcept {
    MyIntPosType *tag = nullptr;
    return convert_to_sfc1d_impl(f, tag);
}

/**
 * @brief Maps normalized float in [0.0, 1.0) into the biased IEEE 754 exponent range [1.0, 2.0) and
 * quantizes.
 */
template <typename FloatT> inline MyIntPosType quantize_coord(FloatT normalized) noexcept {
    constexpr double upper  = 0x1.fffffffffffffp+0;
    double           biased = sycl::fmin(1.0 + static_cast<double>(normalized), upper);
    return convert_to_sfc1d(biased);
}

// ================================================================
// SECTION 8.5: Conversion Utilities between Float & Integer Reps
// ================================================================

/**
 * @brief Converts a normalized floating-point coordinate in [0, 1) into an integer representation.
 */
template <typename FloatT = double>
inline MyIntPosType float_to_int_rep(FloatT normalized) noexcept {
    return quantize_coord(normalized);
}

/**
 * @brief Converts a physical floating-point coordinate into an integer representation relative to
 * domain min_val.
 */
template <typename FloatT>
inline MyIntPosType float_to_int_rep(FloatT val, FloatT min_val, FloatT inv_dx) noexcept {
    constexpr double clamp_upper = 0x1.fffffffffffffp+0;
    double           nx          = sycl::clamp(
        (static_cast<double>(val) - static_cast<double>(min_val)) * static_cast<double>(inv_dx),
        0.0,
        clamp_upper
    );
    return quantize_coord(nx);
}

/**
 * @brief Converts an integer coordinate representation back to a normalized float in [0, 1).
 * @note When BITS_PER_DIMENSION > 52 with 64-bit integer coordinates, double conversion is lossy
 *       due to IEEE 754 double 52-bit mantissa precision limits.
 */
inline double int_rep_to_float(MyIntPosType int_val) noexcept {
#if defined(POSITIONS_IN_32BIT)
    uint64_t mantissa = static_cast<uint64_t>(int_val) << (52 - BITS_PER_DIMENSION);
    uint64_t bits     = (1023ULL << 52) | mantissa;
    return sycl::bit_cast<double>(bits) - 1.0;
#elif defined(POSITIONS_IN_64BIT)
    constexpr int b_dim    = (BITS_PER_DIMENSION < 52) ? BITS_PER_DIMENSION : 52;
    uint64_t      mantissa = (int_val & ((1ULL << b_dim) - 1ULL)) << (52 - b_dim);
    uint64_t      bits     = (1023ULL << 52) | mantissa;
    return sycl::bit_cast<double>(bits) - 1.0;
#else
    uint64_t mantissa = int_val.lo >> (64 - 52);
    uint64_t bits     = (1023ULL << 52) | mantissa;
    return sycl::bit_cast<double>(bits) - 1.0;
#endif
}

/**
 * Extract shortest periodic distance between to integer representaion of coordinates.
 * @param[in] a First integer coordinate representation.
 * @param[in] b Second integer coordinate representation.
 */
inline MyIntPosTypeSigned shortest_periodic_distance(MyIntPosType a, MyIntPosType b) noexcept {
    return static_cast<MyIntPosTypeSigned>((a - b) << SHIFT_BITS) >> SHIFT_BITS;
}

/**
 * @brief Computes the shortest physical periodic distance between two integer coordinates, in
 * noramlized [0,1) units.
 * @param[in] a First integer coordinate.
 * @param[in] b Second integer coordinate.
 */
inline double phy_periodic_distance(MyIntPosType a, MyIntPosType b) noexcept {
    MyIntPosTypeSigned dist = shortest_periodic_distance(a, b);
    return static_cast<double>(dist) * NORM;
}

/**
 * @brief Converts an integer coordinate representation back to physical floating-point coordinates.
 */
template <typename FloatT>
inline double int_rep_to_float(MyIntPosType int_val, FloatT min_val, FloatT dx) noexcept {
    double norm = int_rep_to_float(int_val);
    return static_cast<double>(min_val) + norm * static_cast<double>(dx);
}

// ================================================================
// SECTION 9: Core AREPO Peano-Hilbert 3D Encoder
// ================================================================

/**
 * @brief Encodes 3D integer coordinates into a multi-word Peano-Hilbert sfc_key.
 * @note Iteratively reflects and rotates coordinate octants using AREPO rottable3 and subpix3
 * lookup tables.
 */
template <typename PosType>
inline sfc_key sfc_encode3D(PosType px, PosType py, PosType pz) noexcept {
    sfc_key key      = {};
    uint8_t rotation = 0;

#pragma unroll
    for (int bit = BITS_PER_DIMENSION - 1; bit >= 0; bit--) {
        uint8_t bx = 0, by = 0, bz = 0;
        if constexpr (std::is_same_v<PosType, uint128_t>) {
            if (bit >= 64) {
                bx = static_cast<uint8_t>((px.hi >> (bit - 64)) & 1u);
                by = static_cast<uint8_t>((py.hi >> (bit - 64)) & 1u);
                bz = static_cast<uint8_t>((pz.hi >> (bit - 64)) & 1u);
            } else {
                bx = static_cast<uint8_t>((px.lo >> bit) & 1u);
                by = static_cast<uint8_t>((py.lo >> bit) & 1u);
                bz = static_cast<uint8_t>((pz.lo >> bit) & 1u);
            }
        } else {
            bx = static_cast<uint8_t>((px >> bit) & PosType(1));
            by = static_cast<uint8_t>((py >> bit) & PosType(1));
            bz = static_cast<uint8_t>((pz >> bit) & PosType(1));
        }

        const uint8_t pix = (bx << 2) | (by << 1) | bz;

        key_push_3bits(key, subpix3[rotation][pix]);
        rotation = rottable3[rotation][pix];
    }

    return key;
}

// ================================================================
// SECTION 10: Sorting interface
// ================================================================

struct SortSingle {};
struct SortDouble {};
struct SortTriple {};

using SortStrategy = std::conditional_t<
    KEY_TOTAL_BITS <= 64,
    SortSingle,
    std::conditional_t<KEY_TOTAL_BITS <= 128, SortDouble, SortTriple>>;

// ================================================================
// SECTION 11: Summary
// ================================================================

inline void print_sfc_config() {
    const char *pos_type =
#if defined(POSITIONS_IN_32BIT)
        "32-bit";
#elif defined(POSITIONS_IN_64BIT)
        "64-bit";
#else
        "128-bit";
#endif

    const char *sort_strat = KEY_TOTAL_BITS <= 64    ? "single-pass uint64_t radix"
                             : KEY_TOTAL_BITS <= 128 ? "2-pass radix"
                                                     : "3-pass radix";

    printf("[SFC Config]\n");
    printf("  Position type:      %s (%d bits/dim)\n", pos_type, BITS_FOR_POSITIONS);
    printf("  Bits per dimension: %d\n", BITS_PER_DIMENSION);
    printf("  Total key bits:     %d (in %d word(s))\n", KEY_TOTAL_BITS, KEY_WORDS_NEEDED);
    printf(
        "  Grid resolution:    2^%d = ~%.2e cells/axis\n",
        BITS_PER_DIMENSION,
        std::pow(2.0, (double)BITS_PER_DIMENSION)
    );
    printf("  Sort strategy:      %s\n", sort_strat);
}

} // namespace fasttree

#endif // SYCL_FASTTREE_SFC_PEANO_HILBERT_HPP