// sfc.maxheap.hpp
#ifndef FASTTREE_MAXHEAP_HPP
#define FASTTREE_MAXHEAP_HPP

#include <sycl/sycl.hpp>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace fasttree {

// ================================================================
// SECTION 1: Register MaxHeap — unchanged, no local_accessor used
//
// DistT: distance squared type — typically float or double
//        float:  saves 2x registers, sufficient for most sims
//        double: needed for zoom sims where distances span many
//                orders of magnitude
//
// IdxT:  index type — int for n < 2^31, int64_t for exascale
// ================================================================

/**
 * @brief Register-resident MaxHeap for small k queries (k <= 32).
 *
 * All state is stored in thread local register arrays. Provides O(1) worst-distance
 * lookup for BVH pruning, O(log k) insertion, and O(k log k) in-place extraction.
 *
 * @tparam DistT Floating-point type for squared distances (float or double).
 * @tparam IdxT  Integer type for particle indices (int or int64_t).
 * @tparam MAX_K Maximum capacity of the register heap (must be in [1, 32]).
 */
template <typename DistT, typename IdxT, int MAX_K>
struct RegisterMaxHeap {
  static_assert(MAX_K > 0 && MAX_K <= 32, "RegisterMaxHeap: MAX_K must be in [1,32].");
  static_assert(std::is_floating_point_v<DistT> || std::is_integral_v<DistT>,
                "RegisterMaxHeap: DistT must be float, double, or an unsigned integer type.");
  static_assert(std::is_integral_v<IdxT>, "RegisterMaxHeap: IdxT must be an integer type.");

  DistT dist[MAX_K];  ///< Squared distances array (max-heap ordered).
  IdxT idx[MAX_K];    ///< Particle indices array corresponding to distances.
  int count;          ///< Current heap occupancy.

  static constexpr DistT DIST_MAX = std::numeric_limits<DistT>::max();
  static constexpr IdxT IDX_NONE = static_cast<IdxT>(-1);

  /**
   * @brief Constructs an empty RegisterMaxHeap with sentinels.
   */
  RegisterMaxHeap() noexcept : count(0) {
#pragma unroll
    for (int i = 0; i < MAX_K; ++i) {
      dist[i] = DIST_MAX;
      idx[i] = IDX_NONE;
    }
  }

  /**
   * @brief Prune check to decide if a BVH node should be skipped.
   * @param node_min_d2 Minimum possible squared distance to BVH node bounding box.
   * @param k Requested number of nearest neighbors.
   * @return True if heap is full (count >= k) and node_min_d2 >= current worst distance.
   */
  bool should_prune(DistT node_min_d2, int k) const noexcept { return count >= k && node_min_d2 >= dist[0]; }

  /**
   * @brief Inserts a candidate particle if it belongs in the k nearest.
   * @param d2 Candidate squared distance.
   * @param i Candidate particle index.
   * @param k Requested number of nearest neighbors.
   */
  void push(DistT d2, IdxT i, int k) noexcept {
    if (count < k) {
      dist[count] = d2;
      idx[count] = i;
      sift_up(count);
      count++;
    } else if (d2 < dist[0]) {
      dist[0] = d2;
      idx[0] = i;
      sift_down(0, k);
    }
  }

  /**
   * @brief Extracts sorted results in ascending squared distance order (closest-first).
   * @param[out] out_d2 Output array for squared distances (size >= k).
   * @param[out] out_idx Output array for particle indices (size >= k).
   * @param[in]  k Requested number of neighbors.
   */
  void extract_sorted(DistT *out_d2, IdxT *out_idx, int k) noexcept {
    int original_count = count;
    int heap_size = count;
    while (heap_size > 1) {
      swap_entries(0, heap_size - 1);
      heap_size--;
      sift_down(0, heap_size);
    }
    for (int i = 0; i < k; ++i) {
      if (i < original_count) {
        out_d2[i] = dist[i];
        out_idx[i] = idx[i];
      } else {
        out_d2[i] = DIST_MAX;
        out_idx[i] = IDX_NONE;
      }
    }
  }

 private:
  void sift_up(int i) noexcept {
    while (i > 0) {
      int p = (i - 1) >> 1;
      if (dist[i] <= dist[p]) break;
      swap_entries(i, p);
      i = p;
    }
  }

  void sift_down(int i, int size) noexcept {
    while (true) {
      int largest = i;
      int l = (i << 1) + 1;
      int r = (i << 1) + 2;
      if (l < size && dist[l] > dist[largest]) largest = l;
      if (r < size && dist[r] > dist[largest]) largest = r;
      if (largest == i) break;
      swap_entries(i, largest);
      i = largest;
    }
  }

  void swap_entries(int a, int b) noexcept {
    DistT td = dist[a];
    dist[a] = dist[b];
    dist[b] = td;
    IdxT ti = idx[a];
    idx[a] = idx[b];
    idx[b] = ti;
  }
};

// ================================================================
// SECTION 2: Shared Memory Heap (k > 32)
//
// Key SYCL 2020 design decision:
//   Pass local_accessor BY CONST REFERENCE into helper functions.
//   Use operator[] for all element access.
//   Never call get_pointer() or extract local_ptr.
//   Never use sycl::access::target::local.
// ================================================================

static constexpr int KNN_WG_SIZE = 32;

// ----------------------------------------------------------------
// Helper: swap two elements in a local_accessor array
// Templated on the accessor type to avoid any pointer extraction
// ----------------------------------------------------------------
/**
 * @brief Swaps two distance and index elements in SYCL local accessors.
 */
template <typename DistT, typename IdxT>
inline void local_swap(const sycl::local_accessor<DistT, 1> &sh_dist, const sycl::local_accessor<IdxT, 1> &sh_idx, int a, int b) noexcept {
  DistT td = sh_dist[a];
  sh_dist[a] = sh_dist[b];
  sh_dist[b] = td;
  IdxT ti = sh_idx[a];
  sh_idx[a] = sh_idx[b];
  sh_idx[b] = ti;
}

// ----------------------------------------------------------------
// Sequential sift_up — called by lane 0 only
// Uses local_accessor directly with operator[]
// ----------------------------------------------------------------
/**
 * @brief Sifts an element up in a local_accessor max-heap.
 */
template <typename DistT, typename IdxT>
inline void local_heap_sift_up(const sycl::local_accessor<DistT, 1> &sh_dist, const sycl::local_accessor<IdxT, 1> &sh_idx, int i) noexcept {
  while (i > 0) {
    int p = (i - 1) >> 1;
    if (sh_dist[i] <= sh_dist[p]) break;
    local_swap<DistT, IdxT>(sh_dist, sh_idx, i, p);
    i = p;
  }
}

// ----------------------------------------------------------------
// Sequential sift_down — called by lane 0 only
// ----------------------------------------------------------------
/**
 * @brief Sifts an element down in a local_accessor max-heap.
 */
template <typename DistT, typename IdxT>
inline void local_heap_sift_down(const sycl::local_accessor<DistT, 1> &sh_dist, const sycl::local_accessor<IdxT, 1> &sh_idx, int i,
                                 int size) noexcept {
  while (true) {
    int largest = i;
    int l = (i << 1) + 1;
    int r = (i << 1) + 2;
    if (l < size && sh_dist[l] > sh_dist[largest]) largest = l;
    if (r < size && sh_dist[r] > sh_dist[largest]) largest = r;
    if (largest == i) break;
    local_swap<DistT, IdxT>(sh_dist, sh_idx, i, largest);
    i = largest;
  }
}

// ----------------------------------------------------------------
// Parallel bitonic sort over local_accessor arrays
// Sorts ascending by sh_dist, carries sh_idx along.
// n must be a power of 2.
// All work-items must call this — barriers included.
// ----------------------------------------------------------------
/**
 * @brief Performs parallel bitonic sort over local_accessor arrays in ascending order.
 * @param item SYCL nd_item context.
 * @param sh_dist Shared distance local_accessor array.
 * @param sh_idx Shared index local_accessor array.
 * @param n Power-of-two size of the array to sort.
 */
template <typename DistT, typename IdxT>
inline void local_bitonic_sort(sycl::nd_item<1> &item, const sycl::local_accessor<DistT, 1> &sh_dist, const sycl::local_accessor<IdxT, 1> &sh_idx,
                               int n) noexcept {
  int lid = static_cast<int>(item.get_local_id(0));

  for (int k = 2; k <= n; k <<= 1) {
    for (int j = k >> 1; j > 0; j >>= 1) {
      for (int i = lid; i < n; i += KNN_WG_SIZE) {
        int l = i ^ j;
        bool ascending = ((i & k) == 0);
        if (l > i && l < n) {
          bool do_swap = (sh_dist[i] > sh_dist[l]) == ascending;
          if (do_swap) { local_swap<DistT, IdxT>(sh_dist, sh_idx, i, l); }
        }
      }
      sycl::group_barrier(item.get_group());
    }
  }
}

// ----------------------------------------------------------------
// SharedMaxHeap
//
// All methods take local_accessor BY CONST REFERENCE.
// No pointers extracted. No deprecated APIs used.
// ----------------------------------------------------------------
/**
 * @brief Work-group shared memory MaxHeap for large k queries (k > 32).
 *
 * Storage lives in caller-allocated SYCL local_accessor instances passed by reference.
 */
template <typename DistT, typename IdxT>
struct SharedMaxHeap {
  static_assert(std::is_floating_point_v<DistT> || std::is_integral_v<DistT>,
                "SharedMaxHeap: DistT must be float, double, or an unsigned integer type.");
  static_assert(std::is_integral_v<IdxT>, "SharedMaxHeap: IdxT must be an integer type.");

  static constexpr DistT DIST_MAX = std::numeric_limits<DistT>::max();
  static constexpr IdxT IDX_NONE = static_cast<IdxT>(-1);

  // ── Initialize ───────────────────────────────────────────────
  // All work-items call this. Barriers included.
  /**
   * @brief Initializes shared memory heap structures with sentinels.
   */
  static void init(sycl::nd_item<1> &item, const sycl::local_accessor<DistT, 1> &sh_dist, const sycl::local_accessor<IdxT, 1> &sh_idx,
                   const sycl::local_accessor<int, 1> &sh_count,    // size 1
                   const sycl::local_accessor<DistT, 1> &sh_worst,  // size 1
                   int heap_cap) noexcept {
    int lid = static_cast<int>(item.get_local_id(0));

    for (int i = lid; i < heap_cap; i += KNN_WG_SIZE) {
      sh_dist[i] = DIST_MAX;
      sh_idx[i] = IDX_NONE;
    }
    if (lid == 0) {
      sh_count[0] = 0;
      sh_worst[0] = DIST_MAX;
    }
    sycl::group_barrier(item.get_group());
  }

  // ── Prune check — O(1), no barrier ───────────────────────────
  // All lanes call independently.
  /**
   * @brief O(1) prune check evaluating node bounding box distance against shared worst distance.
   */
  static bool should_prune(DistT node_min_d2, const sycl::local_accessor<DistT, 1> &sh_worst, const sycl::local_accessor<int, 1> &sh_count,
                           int k) noexcept {
    return sh_count[0] >= k && node_min_d2 >= sh_worst[0];
  }

  // ── Batch insert ─────────────────────────────────────────────
  // Each lane contributes one candidate (IDX_NONE = no candidate).
  // Barriers included — must not be called in divergent flow.
  /**
   * @brief Inserts staged candidates contributed by all lanes into the shared max-heap.
   */
  static void batch_insert(sycl::nd_item<1> &item, const sycl::local_accessor<DistT, 1> &sh_dist, const sycl::local_accessor<IdxT, 1> &sh_idx,
                           const sycl::local_accessor<int, 1> &sh_count, const sycl::local_accessor<DistT, 1> &sh_worst,
                           const sycl::local_accessor<DistT, 1> &sh_stage_dist,  // size KNN_WG_SIZE
                           const sycl::local_accessor<IdxT, 1> &sh_stage_idx,    // size KNN_WG_SIZE
                           DistT my_d2, IdxT my_i, int k, int heap_cap) noexcept {
    int lid = static_cast<int>(item.get_local_id(0));

    // Stage candidates
    sh_stage_dist[lid] = my_d2;
    sh_stage_idx[lid] = my_i;
    sycl::group_barrier(item.get_group());

    // Lane 0 inserts all staged candidates
    if (lid == 0) {
      for (int s = 0; s < KNN_WG_SIZE; ++s) {
        if (sh_stage_idx[s] == IDX_NONE) continue;

        DistT d2 = sh_stage_dist[s];
        IdxT i = sh_stage_idx[s];

        if (sh_count[0] < heap_cap) {
          int pos = sh_count[0]++;
          sh_dist[pos] = d2;
          sh_idx[pos] = i;
          local_heap_sift_up<DistT, IdxT>(sh_dist, sh_idx, pos);
        } else if (d2 < sh_dist[0]) {
          sh_dist[0] = d2;
          sh_idx[0] = i;
          local_heap_sift_down<DistT, IdxT>(sh_dist, sh_idx, 0, heap_cap);
        }
      }
      sh_worst[0] = (sh_count[0] > 0) ? sh_dist[0] : DIST_MAX;
    }
    sycl::group_barrier(item.get_group());
  }

  // ── Extract sorted results ────────────────────────────────────
  // Parallel bitonic sort then write k d2 values ascending.
  // No sqrt — caller decides.
  // All work-items participate. Barriers included.
  /**
   * @brief Performs parallel bitonic sort over shared heap and writes top-k sorted results ascending.
   */
  static void extract_sorted_k(sycl::nd_item<1> &item, const sycl::local_accessor<DistT, 1> &sh_dist, const sycl::local_accessor<IdxT, 1> &sh_idx,
                               const sycl::local_accessor<int, 1> &sh_count,
                               DistT *out_d2,  // device memory
                               IdxT *out_idx, int k, int heap_cap) noexcept {
    int lid = static_cast<int>(item.get_local_id(0));
    int count = sh_count[0];

    // Pad unused slots with sentinels
    for (int i = count + lid; i < heap_cap; i += KNN_WG_SIZE) {
      sh_dist[i] = DIST_MAX;
      sh_idx[i] = IDX_NONE;
    }
    sycl::group_barrier(item.get_group());

    // Parallel sort: ascending by d2
    local_bitonic_sort<DistT, IdxT>(item, sh_dist, sh_idx, heap_cap);

    // Write results if pointers are provided
    if (out_d2 && out_idx) {
      for (int i = lid; i < k; i += KNN_WG_SIZE) {
        out_d2[i] = (i < count) ? sh_dist[i] : DIST_MAX;
        out_idx[i] = (i < count) ? sh_idx[i] : IDX_NONE;
      }
    }
  }
};

// ================================================================
// SECTION 3: Type aliases
//
// In integer coordinate mode, distances are computed and stored as
// uint64_t (squared integer distances, pre-shifted).
// In float/double coordinate mode, distances are double.
// ================================================================

#ifdef FASTTREE_INTEGER_COORDS
using heap_dist_t = uint64_t;
#else
using heap_dist_t = double;
#endif

template <int MAX_K>
using SmallKHeap = RegisterMaxHeap<heap_dist_t, int, MAX_K>;

using LargeKHeap = SharedMaxHeap<heap_dist_t, int>;

}  // namespace fasttree

#endif  // FASTTREE_MAXHEAP_HPP
