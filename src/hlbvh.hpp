#ifndef SYCL_FASTTREE_HLBVH_HPP
#define SYCL_FASTTREE_HLBVH_HPP

#include <oneapi/dpl/algorithm>
#include <oneapi/dpl/execution>
#include <sycl/sycl.hpp>
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

uint64_t float_to_int(float f) {
  union {
    float f;
    uint32_t u;  // Use a 32-bit int to read the 32-bit float
  } u;

  u.f = f;

  // 0x7FFFFF masks the 23-bit mantissa of a 32-bit float
  return (uint64_t)((u.u & 0x7FFFFFu) >> (23 - BITS_PER_DIM));
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

// Return indices that would sort the Morton keys
inline std::vector<std::uint64_t> sort_morton_keys(std::vector<std::uint64_t> &morton_keys) {
  std::vector<size_t> indices(morton_keys.size());

  // Parallel iota
  auto count_it = oneapi::dpl::counting_iterator<size_t>(0);
  oneapi::dpl::copy(count_it, count_it + morton_keys.size(), indices.begin());

  // Parallel sort by Morton keys
  oneapi::dpl::sort(oneapi::dpl::execution::par_unseq, indices.begin(), indices.end(),
                    [&morton_keys](size_t i1, size_t i2) { return morton_keys[i1] < morton_keys[i2]; });
  return indices;
}

inline void morton_encode(sycl::queue &q,                           // SYCL queue for offloading
                          const particles<float> &particles,        // Input particle data
                          std::vector<std::uint64_t> &morton_keys,  // USM compatible vector for output
                          const BoundingBox &bbox                   // Bounding box for normalization
) {
  size_t num_particles = particles.pos_x.size();

  q.parallel_for(sycl::range<1>(num_particles), [&](sycl::id<1> idx) {
    size_t i = idx[0];

    std::uint64_t ix = float_to_int(1 + (particles.pos_x[i] - bbox.min_x) / (bbox.max_x - bbox.min_x));
    std::uint64_t iy = float_to_int(1 + (particles.pos_y[i] - bbox.min_y) / (bbox.max_y - bbox.min_y));
    std::uint64_t iz = float_to_int(1 + (particles.pos_z[i] - bbox.min_z) / (bbox.max_z - bbox.min_z));

    // Compute Morton key
    morton_keys[i] = spread3_u64(ix) | (spread3_u64(iy) << 1) | (spread3_u64(iz) << 2);
  });
}

inline void morton_decode(sycl::queue &q, const std::vector<std::uint64_t> &morton_keys, const BoundingBox &bbox, particles<float> &particles) {
  size_t num_particles = morton_keys.size();
  // Make sure the output particle arrays are resized to hold the decoded positions
  particles.pos_x.resize(num_particles);
  particles.pos_y.resize(num_particles);
  particles.pos_z.resize(num_particles);

  q.parallel_for(sycl::range<1>(num_particles), [&](sycl::id<1> idx) {
    size_t i = idx[0];
    std::uint64_t morton_key = morton_keys[i];

    std::uint64_t ix = compact3_u64(morton_key);
    std::uint64_t iy = compact3_u64(morton_key >> 1);
    std::uint64_t iz = compact3_u64(morton_key >> 2);

    particles.pos_x[i] = bbox.min_x + (static_cast<float>(ix) / (1 << BITS_PER_DIM)) * (bbox.max_x - bbox.min_x);
    particles.pos_y[i] = bbox.min_y + (static_cast<float>(iy) / (1 << BITS_PER_DIM)) * (bbox.max_y - bbox.min_y);
    particles.pos_z[i] = bbox.min_z + (static_cast<float>(iz) / (1 << BITS_PER_DIM)) * (bbox.max_z - bbox.min_z);
  });
}

// std::vector<uint64_t> encode_morton3D(particles<float> &particles)

}  // namespace fasttree

#endif  // SYCL_FASTTREE_HLBVH_HPP