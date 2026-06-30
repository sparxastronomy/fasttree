#ifndef SYCL_FASTTREE_DOMAIN_DECOMPOSITION_HPP
#define SYCL_FASTTREE_DOMAIN_DECOMPOSITION_HPP

#include "hlbvh.hpp"
#include <sycl/sycl.hpp>
#include <algorithm>
#include <limits>
#include <numeric>
#include <vector>
#include <mpi.h>

namespace fasttree {

/**
 * @brief Traits helper to map C++ arithmetic types to their corresponding MPI datatype.
 *
 * Provides compile-time mapping from floating-point types to MPI_Datatype,
 * facilitating precision-independent MPI collective operations.
 *
 * @tparam T Coordinate type (float or double).
 */
template <typename T>
struct mpi_type_traits;

template <>
struct mpi_type_traits<float> {
  /// Returns MPI_FLOAT for float coordinates.
  static MPI_Datatype type() { return MPI_FLOAT; }
};

template <>
struct mpi_type_traits<double> {
  /// Returns MPI_DOUBLE for double coordinates.
  static MPI_Datatype type() { return MPI_DOUBLE; }
};

// Phase 1: Global Bounding Box Calculation
/**
 * @brief Calculates the global spatial bounding box spanning all particles across all MPI ranks.
 *
 * Each rank computes the bounding box of its local particles using a parallel reduction on the GPU.
 * Ranks then exchange and combine boundaries using MPI_Allreduce to establish a consistent
 * global bounding box, which is necessary for identical space-filling curve quantization.
 *
 * @tparam FloatT Floating-point coordinate type.
 * @param[in] q SYCL queue.
 * @param[in] p Local particles on this rank.
 * @return BoundingBox<FloatT> The global coordinate boundaries.
 */
template <typename FloatT>
inline BoundingBox<FloatT> get_global_bounding_box(sycl::queue &q, const particles<FloatT> &p) {
  size_t n = p.pos_x.size();

  // 1. Handle empty particle array: return empty box and let MPI allreduce resolve it
  if (n == 0) {
    FloatT min_val = std::numeric_limits<FloatT>::max();
    FloatT max_val = -std::numeric_limits<FloatT>::max();
    FloatT local_min_x = min_val, local_max_x = max_val;
    FloatT local_min_y = min_val, local_max_y = max_val;
    FloatT local_min_z = min_val, local_max_z = max_val;

    FloatT global_min_x, global_max_x;
    FloatT global_min_y, global_max_y;
    FloatT global_min_z, global_max_z;

    MPI_Allreduce(&local_min_x, &global_min_x, 1, mpi_type_traits<FloatT>::type(), MPI_MIN, MPI_COMM_WORLD);
    MPI_Allreduce(&local_max_x, &global_max_x, 1, mpi_type_traits<FloatT>::type(), MPI_MAX, MPI_COMM_WORLD);
    MPI_Allreduce(&local_min_y, &global_min_y, 1, mpi_type_traits<FloatT>::type(), MPI_MIN, MPI_COMM_WORLD);
    MPI_Allreduce(&local_max_y, &global_max_y, 1, mpi_type_traits<FloatT>::type(), MPI_MAX, MPI_COMM_WORLD);
    MPI_Allreduce(&local_min_z, &global_min_z, 1, mpi_type_traits<FloatT>::type(), MPI_MIN, MPI_COMM_WORLD);
    MPI_Allreduce(&local_max_z, &global_max_z, 1, mpi_type_traits<FloatT>::type(), MPI_MAX, MPI_COMM_WORLD);

    return BoundingBox<FloatT>{global_min_x, global_max_x, global_min_y, global_max_y, global_min_z, global_max_z};
  }
  // 2. Compute local bounding box using SYCL
  BoundingBox<FloatT> local_bbox = compute_bbox(q, p, n);

  // 3. Retrieve local reduction results from shared memory
  FloatT local_min_x = local_bbox.min_x;
  FloatT local_max_x = local_bbox.max_x;
  FloatT local_min_y = local_bbox.min_y;
  FloatT local_max_y = local_bbox.max_y;
  FloatT local_min_z = local_bbox.min_z;
  FloatT local_max_z = local_bbox.max_z;

  // 4. Perform global MPI reductions to synchronize bounding box across all ranks
  FloatT global_min_x, global_max_x;
  FloatT global_min_y, global_max_y;
  FloatT global_min_z, global_max_z;

  MPI_Allreduce(&local_min_x, &global_min_x, 1, mpi_type_traits<FloatT>::type(), MPI_MIN, MPI_COMM_WORLD);
  MPI_Allreduce(&local_max_x, &global_max_x, 1, mpi_type_traits<FloatT>::type(), MPI_MAX, MPI_COMM_WORLD);
  MPI_Allreduce(&local_min_y, &global_min_y, 1, mpi_type_traits<FloatT>::type(), MPI_MIN, MPI_COMM_WORLD);
  MPI_Allreduce(&local_max_y, &global_max_y, 1, mpi_type_traits<FloatT>::type(), MPI_MAX, MPI_COMM_WORLD);
  MPI_Allreduce(&local_min_z, &global_min_z, 1, mpi_type_traits<FloatT>::type(), MPI_MIN, MPI_COMM_WORLD);
  MPI_Allreduce(&local_max_z, &global_max_z, 1, mpi_type_traits<FloatT>::type(), MPI_MAX, MPI_COMM_WORLD);

  return BoundingBox<FloatT>{global_min_x, global_max_x, global_min_y, global_max_y, global_min_z, global_max_z};
}

// Phase 2: Coarse-Grid Histogram Calculation
/**
 * @brief Computes a global histogram of particles on a coarse grid defined by Morton/SFC encoding.
 *
 * Quantizes and encodes local particles, updates a local histogram array using GPU atomic additions,
 * and executes an MPI_Allreduce with MPI_SUM to compute the global particle count distribution.
 *
 * @tparam FloatT Floating-point coordinate type.
 * @param[in] q SYCL queue.
 * @param[in] p Local particles on this rank.
 * @param[in] global_bbox Global coordinate bounding box.
 * @param[in] m Number of bits for the coarse grid buckets (default 20, yielding 1 million buckets).
 * @return std::vector<int> Global histogram containing total particle counts per coarse grid bucket.
 */
template <typename FloatT>
inline std::vector<int> get_global_histogram(sycl::queue &q, const particles<FloatT> &p, const BoundingBox<FloatT> &global_bbox, int m = 20) {
  size_t n = p.pos_x.size();
  int num_buckets = 1 << m;

  // 1. Allocate local histogram in USM and initialize it to zero
  int *d_local_hist = sycl::malloc_shared<int>(num_buckets, q);
  q.fill(d_local_hist, 0, num_buckets).wait();

  // 2. Generate Morton/SFC keys and compute bucket indices if particles exist locally
  if (n > 0) {
    uint64_t *d_keys = sycl::malloc_shared<uint64_t>(n, q);
    sfc_encode(q, p, d_keys, global_bbox);
    q.wait();

    // 3. Increment histogram buckets using atomic additions on the GPU
    q.parallel_for(sycl::range<1>(n), [=](sycl::id<1> idx) {
       size_t i = idx[0];
       uint32_t bucket_idx = static_cast<uint32_t>(d_keys[i] >> (63 - m));
       auto atomic_ref = sycl::atomic_ref<int, sycl::memory_order::relaxed, sycl::memory_scope::device, sycl::access::address_space::global_space>(
           d_local_hist[bucket_idx]);
       atomic_ref.fetch_add(1);
     }).wait();

    sycl::free(d_keys, q);
  }

  // 4. Copy local histogram from device/shared memory back to host
  std::vector<int> local_hist(num_buckets);
  q.copy(d_local_hist, local_hist.data(), num_buckets).wait();
  sycl::free(d_local_hist, q);

  // 5. Perform global MPI reduction to sum histograms across all MPI ranks
  std::vector<int> global_hist(num_buckets);
  MPI_Allreduce(local_hist.data(), global_hist.data(), num_buckets, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  return global_hist;
}

// Phase 3: Splitter Generation (Histogram-based)
/**
 * @brief Computes spatial domain splitters from the global coarse-grid histogram.
 *
 * Partitions the coarse SFC buckets into P segments such that each rank is assigned
 * a contiguous SFC segment containing roughly target_load = N_total / P particles.
 * Since the global histogram is identical, every rank executes this computation redundantly
 * to avoid communication overhead.
 *
 * @param[in] global_hist Global histogram containing total particle counts per bucket.
 * @param[in] P Total number of MPI ranks.
 * @param[in] m Number of coarse grid bits (default 20).
 * @return std::vector<uint32_t> Splitter boundaries of size P + 1 (bucket IDs).
 */
inline std::vector<uint32_t> generate_splitters(const std::vector<int> &global_hist, int P, int m = 20) {
  int num_buckets = 1 << m;
  long long total_particles = 0;
  for (int count : global_hist) { total_particles += count; }

  // 1. Calculate target load balance per MPI rank
  long long target_load = total_particles / P;
  std::vector<uint32_t> rank_splitters(P + 1);
  rank_splitters[0] = 0;

  long long current_particles = 0;
  int current_rank = 1;

  // 2. Scan through buckets to assign splitter boundaries when cumulative load targets are reached
  for (uint32_t bucket = 0; bucket < static_cast<uint32_t>(num_buckets); ++bucket) {
    current_particles += global_hist[bucket];
    while (current_rank < P && current_particles >= target_load * current_rank) {
      rank_splitters[current_rank] = bucket;
      current_rank++;
    }
  }

  // 3. Initialize any remaining ranks with the maximum bucket ID (e.g. for empty ranks/voids)
  while (current_rank <= P) {
    rank_splitters[current_rank] = num_buckets;
    current_rank++;
  }

  return rank_splitters;
}

// Helper: Normalize/prepare particle attributes for reordering
/**
 * @brief Normalizes local particle SoA vector sizes and initializes missing attributes.
 *
 * Resizes vectors to match particle count n and populates default IDs (sequential indices) and
 * default is_ghost flags (0 for local) if they are not already initialized.
 *
 * @tparam FloatT Floating-point coordinate type.
 * @param[in,out] p Particle dataset to normalize.
 * @param[in] n Total number of active particles.
 */
template <typename FloatT>
inline void normalize_particles(particles<FloatT> &p, size_t n) {
  // 1. Ensure position vectors are correctly sized
  if (p.pos_x.size() < n) p.pos_x.resize(n, static_cast<FloatT>(0.0));
  if (p.pos_y.size() < n) p.pos_y.resize(n, static_cast<FloatT>(0.0));
  if (p.pos_z.size() < n) p.pos_z.resize(n, static_cast<FloatT>(0.0));

  // 2. Populate mass array with default value if uninitialized
  if (p.mass.size() < n) p.mass.resize(n, static_cast<FloatT>(1.0));

  // 3. Initialize sequence of particle IDs if uninitialized
  if (p.id.size() < n) {
    size_t prev_size = p.id.size();
    p.id.resize(n);
    for (size_t i = prev_size; i < n; ++i) { p.id[i] = static_cast<uint32_t>(i); }
  }

  // 4. Default is_ghost flag to 0 (locally owned)
  if (p.is_ghost.size() < n) p.is_ghost.resize(n, 0);
}

// Phase 3 Alternative: Stride-Based Deterministic Sampling Splitters
/**
 * @brief Computes domain splitters using deterministic stride-based sampling.
 *
 * Each rank sorts its local SFC keys, extracts a stride-spaced sample set (128 samples per rank),
 * and transmits them to Rank 0. Rank 0 aggregates and sorts these samples to determine boundaries for P
 * equal-load segments. These 64-bit key boundaries are then broadcast to all ranks.
 *
 * @tparam FloatT Floating-point coordinate type.
 * @param[in] q SYCL queue.
 * @param[in] p Local particles on this rank.
 * @param[in] global_bbox Global coordinate bounding box.
 * @return std::vector<sfc_key> Broadcasted 64-bit splitter keys of size P + 1.
 */
template <typename FloatT>
inline std::vector<sfc_key> get_deterministic_splitters(sycl::queue &q, const particles<FloatT> &p, const BoundingBox<FloatT> &global_bbox) {
  int rank, P;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &P);

  size_t n = p.pos_x.size();
  const int S = 128;  // Number of samples per rank

  std::vector<sfc_key> local_samples(S, std::numeric_limits<sfc_key>::max());

  if (n > 0) {
    // 1. Allocate and compute local SFC keys
    sfc_key *d_keys = sycl::malloc_shared<sfc_key>(n, q);
    sfc_encode(q, p, d_keys, global_bbox);
    q.wait();

    // 2. Sort the keys locally to find proper percentiles
    auto policy = oneapi::dpl::execution::make_device_policy(q);
    oneapi::dpl::sort(policy, d_keys, d_keys + n);
    q.wait();

    // 3. Stride-based deterministic sampling
    sfc_key *d_samples = sycl::malloc_shared<sfc_key>(S, q);

    q.parallel_for(sycl::range<1>(S), [=](sycl::id<1> idx) {
       int i = idx[0];
       size_t stride = n / S;
       size_t target_idx = i * stride;
       if (target_idx >= n) target_idx = n - 1;  // Safety clamp
       d_samples[i] = d_keys[target_idx];
     }).wait();

    // 4. Copy samples back to host
    q.copy(d_samples, local_samples.data(), S).wait();

    sycl::free(d_keys, q);
    sycl::free(d_samples, q);
  }

  // 5. Gather all samples to Rank 0
  std::vector<sfc_key> global_samples(P * S);
  // TODO: Add a trait to determine the correct MPI datatype for sfc_key (uint64_t) and use it in MPI_Gather
  // For now sfc_key is set to uint64_t, so we can use MPI_UINT64_T
  MPI_Gather(local_samples.data(), S, MPI_UINT64_T, global_samples.data(), S, MPI_UINT64_T, 0, MPI_COMM_WORLD);

  std::vector<sfc_key> splitters(P + 1);
  splitters[0] = 0;                                    // Minimum possible 64-bit key
  splitters[P] = std::numeric_limits<sfc_key>::max();  // Maximum possible 64-bit key

  // 6. Rank 0 calculates the perfect boundaries
  if (rank == 0) {
    // Sort the aggregated samples
    std::sort(global_samples.begin(), global_samples.end());

    // Count valid samples (ignore the max keys from empty ranks)
    int num_valid = 0;
    for (const auto &s : global_samples) {
      if (s != std::numeric_limits<sfc_key>::max()) { num_valid++; }
    }

    // Extract exact percentiles to define domain boundaries
    if (num_valid > 0) {
      for (int i = 1; i < P; ++i) {
        int idx = (i * num_valid) / P;
        splitters[i] = global_samples[idx];
      }
    } else {
      // Fallback if the entire simulation is completely empty
      for (int i = 1; i < P; ++i) { splitters[i] = 0; }
    }
  }

  // 7. Broadcast the final 64-bit splitters to all ranks
  // TODO: Same here, add a trait to determine the correct MPI type
  MPI_Bcast(splitters.data(), P + 1, MPI_UINT64_T, 0, MPI_COMM_WORLD);

  return splitters;
}

// Phase 4: Local Binning & Network Routing
/**
 * @brief Shuffles and routes particles across the network according to the domain splitters.
 *
 * Performs local binning: each particle runs a binary search on the splitters to determine its
 * target rank. Attributes are packed into contiguous send buffers on the GPU, and data counts
 * are shared via MPI_Alltoall. Finally, MPI_Alltoallv distributes particle data, leaving
 * each rank with its local spatial domain segment.
 *
 * @tparam FloatT Floating-point coordinate type.
 * @param[in] q SYCL queue.
 * @param[in,out] p Input local particles.
 * @param[in] rank_splitters Active domain splitters (bucket IDs or 64-bit keys).
 * @param[in] global_bbox Global coordinate bounding box.
 * @param[in] m Coarse grid bucket size (default 20, ignored in sampling mode).
 * @return particles<FloatT> Received particles representing this rank's local segment.
 */
template <typename FloatT>
inline particles<FloatT> redistribute_particles(sycl::queue &q, particles<FloatT> &p,
#if defined(DCOMPOSITION_TYPE_SAMPLING)
                                                const std::vector<sfc_key> &rank_splitters,
#else
                                                const std::vector<uint32_t> &rank_splitters,
#endif
                                                const BoundingBox<FloatT> &global_bbox, int m = 20) {
  int rank, P;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &P);

  size_t n = p.pos_x.size();

  // 1. Ensure all local particle properties are fully normalized/allocated
  normalize_particles(p, n);

  // 2. Allocate USM structures for keys, destination ranks, and indices
  uint64_t *d_keys = sycl::malloc_shared<uint64_t>(n > 0 ? n : 1, q);
  int *d_dest_ranks = sycl::malloc_shared<int>(n > 0 ? n : 1, q);
  size_t *d_indices = sycl::malloc_shared<size_t>(n > 0 ? n : 1, q);

  if (n > 0) {
    // 3. Morton/SFC encode local particles
    sfc_encode(q, p, d_keys, global_bbox);
    q.wait();

#if defined(DCOMPOSITION_TYPE_SAMPLING)
    // 4. Copy splitters to USM shared memory for GPU kernel access
    uint64_t *d_splitters = sycl::malloc_shared<uint64_t>(P + 1, q);
    q.copy(rank_splitters.data(), d_splitters, P + 1).wait();

    // 5. Run binary search to identify target destination rank for each particle using full 64-bit keys
    q.parallel_for(sycl::range<1>(n), [=](sycl::id<1> idx) {
       size_t i = idx[0];
       d_indices[i] = i;
       uint64_t key = d_keys[i];

       int low = 0;
       int high = P - 1;
       int dest_rank = 0;
       while (low <= high) {
         int mid = low + (high - low) / 2;
         if (key >= d_splitters[mid]) {
           dest_rank = mid;
           low = mid + 1;
         } else {
           high = mid - 1;
         }
       }
       d_dest_ranks[i] = dest_rank;
     }).wait();
#else
    // 4. Copy splitters to USM shared memory for GPU kernel access
    uint32_t *d_splitters = sycl::malloc_shared<uint32_t>(P + 1, q);
    q.copy(rank_splitters.data(), d_splitters, P + 1).wait();

    // 5. Run binary search to identify target destination rank for each particle
    q.parallel_for(sycl::range<1>(n), [=](sycl::id<1> idx) {
       size_t i = idx[0];
       d_indices[i] = i;
       uint32_t bucket_id = static_cast<uint32_t>(d_keys[i] >> (63 - m));

       int low = 0;
       int high = P - 1;
       int dest_rank = 0;
       while (low <= high) {
         int mid = low + (high - low) / 2;
         if (bucket_id >= d_splitters[mid]) {
           dest_rank = mid;
           low = mid + 1;
         } else {
           high = mid - 1;
         }
       }
       d_dest_ranks[i] = dest_rank;
     }).wait();
#endif

    sycl::free(d_splitters, q);
  }

  // 6. Build local send count histogram on CPU
  std::vector<int> send_counts(P, 0);
  if (n > 0) {
    std::vector<int> host_dest_ranks(n);
    q.copy(d_dest_ranks, host_dest_ranks.data(), n).wait();
    for (size_t i = 0; i < n; ++i) { send_counts[host_dest_ranks[i]]++; }
  }

  // 7. Sort particle indices by destination rank on GPU (using oneDPL zip_iterator)
  if (n > 0) {
    auto policy = oneapi::dpl::execution::make_device_policy(q);
    auto zip_begin = oneapi::dpl::make_zip_iterator(d_dest_ranks, d_indices);
    auto zip_end = zip_begin + n;
    oneapi::dpl::sort(policy, zip_begin, zip_end, [](auto a, auto b) { return std::get<0>(a) < std::get<0>(b); });
    q.wait();
  }

  // 8. Allocate GPU USM buffers for packed send attributes
  FloatT *send_pos_x = sycl::malloc_shared<FloatT>(n > 0 ? n : 1, q);
  FloatT *send_pos_y = sycl::malloc_shared<FloatT>(n > 0 ? n : 1, q);
  FloatT *send_pos_z = sycl::malloc_shared<FloatT>(n > 0 ? n : 1, q);
  FloatT *send_mass = sycl::malloc_shared<FloatT>(n > 0 ? n : 1, q);
  uint32_t *send_id = sycl::malloc_shared<uint32_t>(n > 0 ? n : 1, q);
  int8_t *send_ghost = sycl::malloc_shared<int8_t>(n > 0 ? n : 1, q);

  // 9. Pack particle attributes into GPU send buffers in sorted destination rank order
  if (n > 0) {
    const FloatT *pos_x = p.pos_x.data();
    const FloatT *pos_y = p.pos_y.data();
    const FloatT *pos_z = p.pos_z.data();
    const FloatT *p_mass = p.mass.data();
    const uint32_t *p_id = p.id.data();
    const int8_t *p_ghost = p.is_ghost.data();

    bool x_alloc = false, y_alloc = false, z_alloc = false;
    bool mass_alloc = false, id_alloc = false, ghost_alloc = false;

    const FloatT *dev_pos_x = ensure_device_readable(q, pos_x, n, x_alloc);
    const FloatT *dev_pos_y = ensure_device_readable(q, pos_y, n, y_alloc);
    const FloatT *dev_pos_z = ensure_device_readable(q, pos_z, n, z_alloc);
    const FloatT *dev_mass = ensure_device_readable(q, p_mass, n, mass_alloc);
    const uint32_t *dev_id = ensure_device_readable(q, p_id, n, id_alloc);
    const int8_t *dev_ghost = ensure_device_readable(q, p_ghost, n, ghost_alloc);

    q.parallel_for(sycl::range<1>(n), [=](sycl::id<1> idx) {
       size_t i = idx[0];
       size_t orig_idx = d_indices[i];
       send_pos_x[i] = dev_pos_x[orig_idx];
       send_pos_y[i] = dev_pos_y[orig_idx];
       send_pos_z[i] = dev_pos_z[orig_idx];
       send_mass[i] = dev_mass[orig_idx];
       send_id[i] = dev_id[orig_idx];
       send_ghost[i] = dev_ghost[orig_idx];
     }).wait();

    free_device_readable(q, dev_pos_x, x_alloc);
    free_device_readable(q, dev_pos_y, y_alloc);
    free_device_readable(q, dev_pos_z, z_alloc);
    free_device_readable(q, dev_mass, mass_alloc);
    free_device_readable(q, dev_id, id_alloc);
    free_device_readable(q, dev_ghost, ghost_alloc);
  }

  // 10. Exchange send counts via MPI_Alltoall to determine receive counts from other ranks
  std::vector<int> recv_counts(P, 0);
  MPI_Alltoall(send_counts.data(), 1, MPI_INT, recv_counts.data(), 1, MPI_INT, MPI_COMM_WORLD);

  // 11. Compute send/receive displacements and total expected received count
  std::vector<int> send_displs(P, 0);
  std::vector<int> recv_displs(P, 0);
  int total_recv = 0;
  for (int i = 0; i < P; ++i) {
    send_displs[i] = (i == 0) ? 0 : send_displs[i - 1] + send_counts[i - 1];
    recv_displs[i] = (i == 0) ? 0 : recv_displs[i - 1] + recv_counts[i - 1];
    total_recv += recv_counts[i];
  }

  // 12. Allocate output host vectors and matching GPU USM buffers for received particles
  particles<FloatT> recv_p;
  recv_p.pos_x.resize(total_recv);
  recv_p.pos_y.resize(total_recv);
  recv_p.pos_z.resize(total_recv);
  recv_p.mass.resize(total_recv);
  recv_p.id.resize(total_recv);
  recv_p.is_ghost.resize(total_recv);

  FloatT *recv_pos_x = sycl::malloc_shared<FloatT>(total_recv > 0 ? total_recv : 1, q);
  FloatT *recv_pos_y = sycl::malloc_shared<FloatT>(total_recv > 0 ? total_recv : 1, q);
  FloatT *recv_pos_z = sycl::malloc_shared<FloatT>(total_recv > 0 ? total_recv : 1, q);
  FloatT *recv_mass = sycl::malloc_shared<FloatT>(total_recv > 0 ? total_recv : 1, q);
  uint32_t *recv_id = sycl::malloc_shared<uint32_t>(total_recv > 0 ? total_recv : 1, q);
  int8_t *recv_ghost = sycl::malloc_shared<int8_t>(total_recv > 0 ? total_recv : 1, q);

  // 13. Perform MPI_Alltoallv to distribute/collect particle attributes across network
  MPI_Alltoallv(send_pos_x, send_counts.data(), send_displs.data(), mpi_type_traits<FloatT>::type(), recv_pos_x, recv_counts.data(),
                recv_displs.data(), mpi_type_traits<FloatT>::type(), MPI_COMM_WORLD);
  MPI_Alltoallv(send_pos_y, send_counts.data(), send_displs.data(), mpi_type_traits<FloatT>::type(), recv_pos_y, recv_counts.data(),
                recv_displs.data(), mpi_type_traits<FloatT>::type(), MPI_COMM_WORLD);
  MPI_Alltoallv(send_pos_z, send_counts.data(), send_displs.data(), mpi_type_traits<FloatT>::type(), recv_pos_z, recv_counts.data(),
                recv_displs.data(), mpi_type_traits<FloatT>::type(), MPI_COMM_WORLD);
  MPI_Alltoallv(send_mass, send_counts.data(), send_displs.data(), mpi_type_traits<FloatT>::type(), recv_mass, recv_counts.data(), recv_displs.data(),
                mpi_type_traits<FloatT>::type(), MPI_COMM_WORLD);
  MPI_Alltoallv(send_id, send_counts.data(), send_displs.data(), MPI_UINT32_T, recv_id, recv_counts.data(), recv_displs.data(), MPI_UINT32_T,
                MPI_COMM_WORLD);
  MPI_Alltoallv(send_ghost, send_counts.data(), send_displs.data(), MPI_INT8_T, recv_ghost, recv_counts.data(), recv_displs.data(), MPI_INT8_T,
                MPI_COMM_WORLD);

  q.wait();

  // 14. Copy received particles from GPU shared buffers back to host vector structures
  if (total_recv > 0) {
    q.copy(recv_pos_x, recv_p.pos_x.data(), total_recv);
    q.copy(recv_pos_y, recv_p.pos_y.data(), total_recv);
    q.copy(recv_pos_z, recv_p.pos_z.data(), total_recv);
    q.copy(recv_mass, recv_p.mass.data(), total_recv);
    q.copy(recv_id, recv_p.id.data(), total_recv);
    q.copy(recv_ghost, recv_p.is_ghost.data(), total_recv);
    q.wait();
  }

  // 15. Free all allocated USM memory
  sycl::free(d_keys, q);
  sycl::free(d_dest_ranks, q);
  sycl::free(d_indices, q);
  sycl::free(send_pos_x, q);
  sycl::free(send_pos_y, q);
  sycl::free(send_pos_z, q);
  sycl::free(send_mass, q);
  sycl::free(send_id, q);
  sycl::free(send_ghost, q);
  sycl::free(recv_pos_x, q);
  sycl::free(recv_pos_y, q);
  sycl::free(recv_pos_z, q);
  sycl::free(recv_mass, q);
  sycl::free(recv_id, q);
  sycl::free(recv_ghost, q);

  return recv_p;
}

// Phase 5: Explicit Halo Exchange (Static Ghosting)
/**
 * @brief Exchanges boundary particles with spatially overlapping neighbors.
 *
 * Broadcasts rank bounding boxes using MPI_Allgather, detects ranks overlapping this rank's bounds
 * within search distance h_max, and runs a GPU filter to pack local boundary particles.
 * Ranks exchange counts via MPI_Alltoall and route attributes via MPI_Alltoallv.
 * Received boundary particles are tagged with is_ghost = 1 and appended to the local particle array.
 *
 * @tparam FloatT Floating-point coordinate type.
 * @param[in] q SYCL queue.
 * @param[in,out] p Redistributed local particles.
 * @param[in] h_max Boundary search radius (smoothing length/ghost width).
 * @return particles<FloatT> Combined SoA containing local particles followed by ghost particles.
 */
template <typename FloatT>
inline particles<FloatT> exchange_halos(sycl::queue &q, particles<FloatT> &p, FloatT h_max) {
  int rank, P;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &P);

  size_t n = p.pos_x.size();

  // 1. Ensure all local particles are properly normalized
  normalize_particles(p, n);

  // 2. Compute the local spatial bounding box for the rank's particles
  BoundingBox<FloatT> my_bbox{static_cast<FloatT>(0.0), static_cast<FloatT>(0.0), static_cast<FloatT>(0.0),
                              static_cast<FloatT>(0.0), static_cast<FloatT>(0.0), static_cast<FloatT>(0.0)};
  if (n == 0) {
    my_bbox = BoundingBox<FloatT>{static_cast<FloatT>(0.0), static_cast<FloatT>(0.0), static_cast<FloatT>(0.0),
                                  static_cast<FloatT>(0.0), static_cast<FloatT>(0.0), static_cast<FloatT>(0.0)};
  } else {
    my_bbox = {p.pos_x[0], p.pos_x[0], p.pos_y[0], p.pos_y[0], p.pos_z[0], p.pos_z[0]};
    for (size_t i = 1; i < n; ++i) {
      my_bbox.min_x = std::min(my_bbox.min_x, p.pos_x[i]);
      my_bbox.max_x = std::max(my_bbox.max_x, p.pos_x[i]);
      my_bbox.min_y = std::min(my_bbox.min_y, p.pos_y[i]);
      my_bbox.max_y = std::max(my_bbox.max_y, p.pos_y[i]);
      my_bbox.min_z = std::min(my_bbox.min_z, p.pos_z[i]);
      my_bbox.max_z = std::max(my_bbox.max_z, p.pos_z[i]);
    }
  }

  // 3. Share local bounding boxes globally across all ranks via MPI_Allgather
  std::vector<FloatT> local_box_floats = {my_bbox.min_x, my_bbox.max_x, my_bbox.min_y, my_bbox.max_y, my_bbox.min_z, my_bbox.max_z};
  std::vector<FloatT> all_box_floats(P * 6);
  MPI_Allgather(local_box_floats.data(), 6, mpi_type_traits<FloatT>::type(), all_box_floats.data(), 6, mpi_type_traits<FloatT>::type(),
                MPI_COMM_WORLD);

  std::vector<BoundingBox<FloatT>> all_bboxes(P, BoundingBox<FloatT>(static_cast<FloatT>(0.0), static_cast<FloatT>(0.0), static_cast<FloatT>(0.0),
                                                                     static_cast<FloatT>(0.0), static_cast<FloatT>(0.0), static_cast<FloatT>(0.0)));
  for (int i = 0; i < P; ++i) {
    all_bboxes[i] = BoundingBox<FloatT>{all_box_floats[i * 6 + 0], all_box_floats[i * 6 + 1], all_box_floats[i * 6 + 2],
                                        all_box_floats[i * 6 + 3], all_box_floats[i * 6 + 4], all_box_floats[i * 6 + 5]};
  }

  // 4. Filter and identify direct spatial neighbors (overlapping within search radius h_max) on CPU
  std::vector<int> neighbor_ranks;
  for (int r = 0; r < P; ++r) {
    if (r == rank) continue;
    if (all_bboxes[r].min_x > all_bboxes[r].max_x) continue;  // empty rank
    if (my_bbox.min_x - h_max <= all_bboxes[r].max_x && my_bbox.max_x + h_max >= all_bboxes[r].min_x &&
        my_bbox.min_y - h_max <= all_bboxes[r].max_y && my_bbox.max_y + h_max >= all_bboxes[r].min_y &&
        my_bbox.min_z - h_max <= all_bboxes[r].max_z && my_bbox.max_z + h_max >= all_bboxes[r].min_z) {
      neighbor_ranks.push_back(r);
    }
  }

  // 5. Declare matched indices and tracking structures
  size_t *d_matched_indices = nullptr;
  std::vector<int> send_counts(P, 0);
  int total_sends = 0;

  // 6. Find local boundary particles overlapping with neighbor domains using a memory-efficient two-pass approach
  if (n > 0 && !neighbor_ranks.empty()) {
    const FloatT *pos_x = p.pos_x.data();
    const FloatT *pos_y = p.pos_y.data();
    const FloatT *pos_z = p.pos_z.data();

    bool x_alloc = false, y_alloc = false, z_alloc = false;
    const FloatT *dev_pos_x = ensure_device_readable(q, pos_x, n, x_alloc);
    const FloatT *dev_pos_y = ensure_device_readable(q, pos_y, n, y_alloc);
    const FloatT *dev_pos_z = ensure_device_readable(q, pos_z, n, z_alloc);

    std::vector<int> neighbor_sends(neighbor_ranks.size(), 0);

    // Pass 1: Count overlapping particles for each neighbor rank
    for (size_t k = 0; k < neighbor_ranks.size(); ++k) {
      int r = neighbor_ranks[k];
      BoundingBox<FloatT> neighbor_box = all_bboxes[r];
      FloatT min_x = neighbor_box.min_x - h_max;
      FloatT max_x = neighbor_box.max_x + h_max;
      FloatT min_y = neighbor_box.min_y - h_max;
      FloatT max_y = neighbor_box.max_y + h_max;
      FloatT min_z = neighbor_box.min_z - h_max;
      FloatT max_z = neighbor_box.max_z + h_max;

      int *d_count = sycl::malloc_shared<int>(1, q);
      d_count[0] = 0;

      q.parallel_for(sycl::range<1>(n), [=](sycl::id<1> idx) {
         size_t i = idx[0];
         if (dev_pos_x[i] >= min_x && dev_pos_x[i] <= max_x && dev_pos_y[i] >= min_y && dev_pos_y[i] <= max_y && dev_pos_z[i] >= min_z &&
             dev_pos_z[i] <= max_z) {
           auto atomic_ref =
               sycl::atomic_ref<int, sycl::memory_order::relaxed, sycl::memory_scope::device, sycl::access::address_space::global_space>(d_count[0]);
           atomic_ref.fetch_add(1);
         }
       }).wait();

      int count = d_count[0];
      neighbor_sends[k] = count;
      send_counts[r] = count;
      total_sends += count;
      sycl::free(d_count, q);
    }

    // Allocate matched indices array with EXACT size
    d_matched_indices = sycl::malloc_shared<size_t>(total_sends > 0 ? total_sends : 1, q);

    // Pass 2: Write matched indices into the allocated array
    int current_offset = 0;
    for (size_t k = 0; k < neighbor_ranks.size(); ++k) {
      int r = neighbor_ranks[k];
      int count = neighbor_sends[k];
      if (count == 0) continue;

      BoundingBox<FloatT> neighbor_box = all_bboxes[r];
      FloatT min_x = neighbor_box.min_x - h_max;
      FloatT max_x = neighbor_box.max_x + h_max;
      FloatT min_y = neighbor_box.min_y - h_max;
      FloatT max_y = neighbor_box.max_y + h_max;
      FloatT min_z = neighbor_box.min_z - h_max;
      FloatT max_z = neighbor_box.max_z + h_max;

      int *d_count = sycl::malloc_shared<int>(1, q);
      d_count[0] = 0;
      size_t *matched_out = d_matched_indices + current_offset;

      q.parallel_for(sycl::range<1>(n), [=](sycl::id<1> idx) {
         size_t i = idx[0];
         if (dev_pos_x[i] >= min_x && dev_pos_x[i] <= max_x && dev_pos_y[i] >= min_y && dev_pos_y[i] <= max_y && dev_pos_z[i] >= min_z &&
             dev_pos_z[i] <= max_z) {
           auto atomic_ref =
               sycl::atomic_ref<int, sycl::memory_order::relaxed, sycl::memory_scope::device, sycl::access::address_space::global_space>(d_count[0]);
           int pos = atomic_ref.fetch_add(1);
           matched_out[pos] = i;
         }
       }).wait();

      sycl::free(d_count, q);
      current_offset += count;
    }

    free_device_readable(q, dev_pos_x, x_alloc);
    free_device_readable(q, dev_pos_y, y_alloc);
    free_device_readable(q, dev_pos_z, z_alloc);
  }

  // 7. Allocate send buffers for ghost particle attributes on GPU
  FloatT *send_pos_x = sycl::malloc_shared<FloatT>(total_sends > 0 ? total_sends : 1, q);
  FloatT *send_pos_y = sycl::malloc_shared<FloatT>(total_sends > 0 ? total_sends : 1, q);
  FloatT *send_pos_z = sycl::malloc_shared<FloatT>(total_sends > 0 ? total_sends : 1, q);
  FloatT *send_mass = sycl::malloc_shared<FloatT>(total_sends > 0 ? total_sends : 1, q);
  uint32_t *send_id = sycl::malloc_shared<uint32_t>(total_sends > 0 ? total_sends : 1, q);
  int8_t *send_ghost = sycl::malloc_shared<int8_t>(total_sends > 0 ? total_sends : 1, q);

  // 8. Pack attributes of particles to send and mark receiver-side is_ghost flag as 1
  if (total_sends > 0) {
    const FloatT *pos_x = p.pos_x.data();
    const FloatT *pos_y = p.pos_y.data();
    const FloatT *pos_z = p.pos_z.data();
    const FloatT *p_mass = p.mass.data();
    const uint32_t *p_id = p.id.data();

    bool x_alloc = false, y_alloc = false, z_alloc = false;
    bool mass_alloc = false, id_alloc = false;
    const FloatT *dev_pos_x = ensure_device_readable(q, pos_x, n, x_alloc);
    const FloatT *dev_pos_y = ensure_device_readable(q, pos_y, n, y_alloc);
    const FloatT *dev_pos_z = ensure_device_readable(q, pos_z, n, z_alloc);
    const FloatT *dev_mass = ensure_device_readable(q, p_mass, n, mass_alloc);
    const uint32_t *dev_id = ensure_device_readable(q, p_id, n, id_alloc);

    q.parallel_for(sycl::range<1>(total_sends), [=](sycl::id<1> idx) {
       size_t i = idx[0];
       size_t orig_idx = d_matched_indices[i];
       send_pos_x[i] = dev_pos_x[orig_idx];
       send_pos_y[i] = dev_pos_y[orig_idx];
       send_pos_z[i] = dev_pos_z[orig_idx];
       send_mass[i] = dev_mass[orig_idx];
       send_id[i] = dev_id[orig_idx];
       send_ghost[i] = 1;  // Receive on other side as ghost particle
     }).wait();

    free_device_readable(q, dev_pos_x, x_alloc);
    free_device_readable(q, dev_pos_y, y_alloc);
    free_device_readable(q, dev_pos_z, z_alloc);
    free_device_readable(q, dev_mass, mass_alloc);
    free_device_readable(q, dev_id, id_alloc);
  }

  // 9. Exchange send counts via MPI_Alltoall to determine receive counts from other ranks
  std::vector<int> recv_counts(P, 0);
  MPI_Alltoall(send_counts.data(), 1, MPI_INT, recv_counts.data(), 1, MPI_INT, MPI_COMM_WORLD);

  // 10. Compute send and receive offsets and count total expected received ghosts
  std::vector<int> send_displs(P, 0);
  std::vector<int> recv_displs(P, 0);
  int total_recv = 0;
  for (int i = 0; i < P; ++i) {
    send_displs[i] = (i == 0) ? 0 : send_displs[i - 1] + send_counts[i - 1];
    recv_displs[i] = (i == 0) ? 0 : recv_displs[i - 1] + recv_counts[i - 1];
    total_recv += recv_counts[i];
  }

  // 11. Allocate GPU USM buffers for received ghost particle attributes
  FloatT *recv_pos_x = sycl::malloc_shared<FloatT>(total_recv > 0 ? total_recv : 1, q);
  FloatT *recv_pos_y = sycl::malloc_shared<FloatT>(total_recv > 0 ? total_recv : 1, q);
  FloatT *recv_pos_z = sycl::malloc_shared<FloatT>(total_recv > 0 ? total_recv : 1, q);
  FloatT *recv_mass = sycl::malloc_shared<FloatT>(total_recv > 0 ? total_recv : 1, q);
  uint32_t *recv_id = sycl::malloc_shared<uint32_t>(total_recv > 0 ? total_recv : 1, q);
  int8_t *recv_ghost = sycl::malloc_shared<int8_t>(total_recv > 0 ? total_recv : 1, q);

  // 12. Exchange ghost particle attributes via MPI_Alltoallv
  MPI_Alltoallv(send_pos_x, send_counts.data(), send_displs.data(), mpi_type_traits<FloatT>::type(), recv_pos_x, recv_counts.data(),
                recv_displs.data(), mpi_type_traits<FloatT>::type(), MPI_COMM_WORLD);
  MPI_Alltoallv(send_pos_y, send_counts.data(), send_displs.data(), mpi_type_traits<FloatT>::type(), recv_pos_y, recv_counts.data(),
                recv_displs.data(), mpi_type_traits<FloatT>::type(), MPI_COMM_WORLD);
  MPI_Alltoallv(send_pos_z, send_counts.data(), send_displs.data(), mpi_type_traits<FloatT>::type(), recv_pos_z, recv_counts.data(),
                recv_displs.data(), mpi_type_traits<FloatT>::type(), MPI_COMM_WORLD);
  MPI_Alltoallv(send_mass, send_counts.data(), send_displs.data(), mpi_type_traits<FloatT>::type(), recv_mass, recv_counts.data(), recv_displs.data(),
                mpi_type_traits<FloatT>::type(), MPI_COMM_WORLD);
  MPI_Alltoallv(send_id, send_counts.data(), send_displs.data(), MPI_UINT32_T, recv_id, recv_counts.data(), recv_displs.data(), MPI_UINT32_T,
                MPI_COMM_WORLD);
  MPI_Alltoallv(send_ghost, send_counts.data(), send_displs.data(), MPI_INT8_T, recv_ghost, recv_counts.data(), recv_displs.data(), MPI_INT8_T,
                MPI_COMM_WORLD);

  q.wait();

  // 13. Create combined particle set of local particles followed by received ghost particles
  particles<FloatT> combined_p;
  size_t total_combined = n + total_recv;
  combined_p.pos_x.resize(total_combined);
  combined_p.pos_y.resize(total_combined);
  combined_p.pos_z.resize(total_combined);
  combined_p.mass.resize(total_combined);
  combined_p.id.resize(total_combined);
  combined_p.is_ghost.resize(total_combined);

  // 14. Copy local particles to the starting segment of the combined vectors
  std::copy(p.pos_x.begin(), p.pos_x.end(), combined_p.pos_x.begin());
  std::copy(p.pos_y.begin(), p.pos_y.end(), combined_p.pos_y.begin());
  std::copy(p.pos_z.begin(), p.pos_z.end(), combined_p.pos_z.begin());
  std::copy(p.mass.begin(), p.mass.end(), combined_p.mass.begin());
  std::copy(p.id.begin(), p.id.end(), combined_p.id.begin());
  std::copy(p.is_ghost.begin(), p.is_ghost.end(), combined_p.is_ghost.begin());

  // 15. Copy received ghost particles from GPU USM buffers to the trailing segment
  if (total_recv > 0) {
    q.copy(recv_pos_x, combined_p.pos_x.data() + n, total_recv);
    q.copy(recv_pos_y, combined_p.pos_y.data() + n, total_recv);
    q.copy(recv_pos_z, combined_p.pos_z.data() + n, total_recv);
    q.copy(recv_mass, combined_p.mass.data() + n, total_recv);
    q.copy(recv_id, combined_p.id.data() + n, total_recv);
    q.copy(recv_ghost, combined_p.is_ghost.data() + n, total_recv);
    q.wait();
  }

  // 16. Free all allocated USM memory
  sycl::free(d_matched_indices, q);
  sycl::free(send_pos_x, q);
  sycl::free(send_pos_y, q);
  sycl::free(send_pos_z, q);
  sycl::free(send_mass, q);
  sycl::free(send_id, q);
  sycl::free(send_ghost, q);
  sycl::free(recv_pos_x, q);
  sycl::free(recv_pos_y, q);
  sycl::free(recv_pos_z, q);
  sycl::free(recv_mass, q);
  sycl::free(recv_id, q);
  sycl::free(recv_ghost, q);

  return combined_p;
}

}  // namespace fasttree

#endif  // SYCL_FASTTREE_DOMAIN_DECOMPOSITION_HPP
