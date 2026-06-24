#ifndef SYCL_FASTTREE_SFC_PEANO_HILBERT_HPP
#define SYCL_FASTTREE_SFC_PEANO_HILBERT_HPP

#include <sycl/sycl.hpp>
#include <algorithm>
#include <cstdint>
#include <vector>

namespace fasttree {

#ifndef BITS_PER_DIMENSION
#define BITS_PER_DIMENSION 21
#endif

using sfc1D = uint32_t;
using sfc_key = uint64_t;

inline sfc1D convert_to_sfc1d(double d) noexcept {
  const uint64_t bits = sycl::bit_cast<uint64_t>(d);
  constexpr uint64_t mantissa_mask = 0x000FFFFFFFFFFFFFLLU;
  constexpr int shift = 52 - BITS_PER_DIMENSION;
  return static_cast<sfc1D>((bits & mantissa_mask) >> shift);
}

inline sfc_key sfc_encode3D(sfc1D px, sfc1D py, sfc1D pz) noexcept {
  sfc_key key = 0;
  int Nlev = BITS_PER_DIMENSION;
  const uint32_t valid_mask = (Nlev < 32) ? ((1u << Nlev) - 1u) : 0xFFFFFFFFu;
  px &= valid_mask;
  py &= valid_mask;
  pz &= valid_mask;

  for (int jj = Nlev - 1; jj >= 0; jj--) {
    const uint32_t xi = (px >> jj) & 1u;
    const uint32_t yi = (py >> jj) & 1u;
    const uint32_t zi = (pz >> jj) & 1u;

    const uint32_t not_yi = yi ^ 1u;
    const uint32_t not_zi = zi ^ 1u;

    px ^= -(xi & (not_yi | zi));
    px &= valid_mask;

    py ^= -((xi & (yi | zi)) | (yi & not_zi));
    py &= valid_mask;

    pz ^= -((xi & not_yi & not_zi) | (yi & not_zi));
    pz &= valid_mask;

    const sfc_key chunk = ((sfc_key)xi << 2) | ((sfc_key)(xi ^ yi) << 1) | (sfc_key)(xi ^ zi ^ yi);
    key |= (chunk << (3 * jj));

    const uint32_t mask1 = -(zi);
    const uint32_t mask2 = -(not_zi & not_yi);
    const uint32_t mask0 = ~(mask1 | mask2);

    const uint32_t old_px = px;
    const uint32_t new_px = (px & mask0) | (py & mask1) | (pz & mask2);
    const uint32_t new_py = (py & mask0) | (pz & mask1) | (py & mask2);
    const uint32_t new_pz = (pz & mask0) | (old_px & ~mask0);

    px = new_px;
    py = new_py;
    pz = new_pz;
  }
  return key;
}

inline void decodePeano3D(int Nlev, sfc_key key, uint32_t &px, uint32_t &py, uint32_t &pz) noexcept {
  px = 0;
  py = 0;
  pz = 0;

  uint32_t xi[32], yi[32], zi[32];
  for (int jj = 0; jj < Nlev; jj++) {
    sfc_key chunk = (key >> (3 * jj)) & 7u;
    xi[jj] = (chunk >> 2) & 1u;
    yi[jj] = xi[jj] ^ ((chunk >> 1) & 1u);
    zi[jj] = xi[jj] ^ yi[jj] ^ (chunk & 1u);
  }

  const uint32_t valid_mask = (Nlev < 32) ? ((1u << Nlev) - 1u) : 0xFFFFFFFFu;

  for (int jj = 0; jj < Nlev; jj++) {
    const uint32_t x = xi[jj];
    const uint32_t y = yi[jj];
    const uint32_t z = zi[jj];
    const uint32_t not_y = y ^ 1u;
    const uint32_t not_z = z ^ 1u;

    const uint32_t mask1 = -(z);
    const uint32_t mask2 = -(not_z & not_y);
    const uint32_t mask0 = ~(mask1 | mask2);

    const uint32_t old_px = px;
    const uint32_t old_py = py;

    const uint32_t new_px = (px & mask0) | (pz & mask1) | (pz & mask2);
    const uint32_t new_py = (py & mask0) | (old_px & mask1) | (py & mask2);
    const uint32_t new_pz = (pz & mask0) | (old_py & mask1) | (old_px & mask2);

    px = new_px;
    py = new_py;
    pz = new_pz;

    px ^= -(x & (not_y | z));
    px &= valid_mask;

    py ^= -((x & (y | z)) | (y & not_z));
    py &= valid_mask;

    pz ^= -((x & not_y & not_z) | (y & not_z));
    pz &= valid_mask;

    px |= (x << jj);
    py |= (y << jj);
    pz |= (z << jj);
  }
}

// ! NOTE: not needed for now
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
//     sfc_key ph_key = d_keys[i];

//     uint32_t ix = 0;
//     uint32_t iy = 0;
//     uint32_t iz = 0;
//     decodePeano3D(BITS_PER_DIMENSION, ph_key, ix, iy, iz);

//     pos_x[i] = bbox.min_x + static_cast<FloatT>(ix) * scale * dx;
//     pos_y[i] = bbox.min_y + static_cast<FloatT>(iy) * scale * dy;
//     pos_z[i] = bbox.min_z + static_cast<FloatT>(iz) * scale * dz;
//   });
// }

}  // namespace fasttree

#endif  // SYCL_FASTTREE_SFC_PEANO_HILBERT_HPP