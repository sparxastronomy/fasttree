#ifndef SYCL_FASTTREE_SFC_MORTON_HPP
#define SYCL_FASTTREE_SFC_MORTON_HPP

#include <sycl/sycl.hpp>
#include <algorithm>
#include <cstdint>
#include <vector>

namespace fasttree {

#ifndef BITS_PER_DIMENSION
#define BITS_PER_DIMENSION 21
#endif

// Define types for 1D and 3D space-filling curve keys
using sfc1D = std::uint32_t;
using sfc_key = std::uint64_t;

#ifdef FASTTREE_INTEGER_COORDS
using dist_t = std::uint64_t;
#else
using dist_t = double;
#endif

// Utilities for Morton keys
static constexpr std::uint64_t MASK_21 = 0x1FFFFFULL;
static constexpr std::uint64_t M1 = 0x1F00000000FFFFULL;
static constexpr std::uint64_t M2 = 0x1F0000FF0000FFULL;
static constexpr std::uint64_t M3 = 0x100F00F00F00F00FULL;
static constexpr std::uint64_t M4 = 0x10C30C30C30C30C3ULL;
static constexpr std::uint64_t M5 = 0x1249249249249249ULL;

inline sfc_key spread3_u64(sfc1D x) {
  sfc_key w = x & MASK_21;
  w = (w | (w << 32)) & M1;
  w = (w | (w << 16)) & M2;
  w = (w | (w << 8)) & M3;
  w = (w | (w << 4)) & M4;
  w = (w | (w << 2)) & M5;
  return w;
}

inline sfc_key compact3_u64(sfc_key x) {
  x &= M5;
  x = (x | (x >> 2)) & M4;
  x = (x | (x >> 4)) & M3;
  x = (x | (x >> 8)) & M2;
  x = (x | (x >> 16)) & M1;
  x = (x | (x >> 32)) & MASK_21;
  return x;
}

// template <typename FloatT>
// inline void sfc_decode(sycl::queue &q, const std::vector<sfc_key> &keys, const BoundingBox<FloatT> &bbox, particles<FloatT> &parts) {
//   size_t num_particles = keys.size();
//   parts.pos_x.resize(num_particles);
//   parts.pos_y.resize(num_particles);
//   parts.pos_z.resize(num_particles);

//   const sfc_key *d_keys = keys.data();
//   FloatT *pos_x = parts.pos_x.data();
//   FloatT *pos_y = parts.pos_y.data();
//   FloatT *pos_z = parts.pos_z.data();

//   FloatT dx = bbox.max_x - bbox.min_x;
//   FloatT dy = bbox.max_y - bbox.min_y;
//   FloatT dz = bbox.max_z - bbox.min_z;
//   FloatT scale = static_cast<FloatT>(1.0) / ((1ULL << BITS_PER_DIMENSION) - 1);

//   q.parallel_for(sycl::range<1>(num_particles), [=](sycl::id<1> idx) {
//     size_t i = idx[0];
//     sfc_key morton_key = d_keys[i];

//     std::uint64_t ix = compact3_u64(morton_key);
//     std::uint64_t iy = compact3_u64(morton_key >> 1);
//     std::uint64_t iz = compact3_u64(morton_key >> 2);

//     pos_x[i] = bbox.min_x + static_cast<FloatT>(ix) * scale * dx;
//     pos_y[i] = bbox.min_y + static_cast<FloatT>(iy) * scale * dy;
//     pos_z[i] = bbox.min_z + static_cast<FloatT>(iz) * scale * dz;
//   });
// }

}  // namespace fasttree

#endif  // SYCL_FASTTREE_SFC_MORTON_HPP
