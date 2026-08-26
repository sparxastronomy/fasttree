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
template <typename DistT, typename IdxT, int MAX_K> struct RegisterMaxHeap {
    static_assert(MAX_K > 0 && MAX_K <= 32, "RegisterMaxHeap: MAX_K must be in [1,32].");
    static_assert(
        std::is_floating_point_v<DistT> || std::is_integral_v<DistT>,
        "RegisterMaxHeap: DistT must be float, double, or an unsigned integer type."
    );
    static_assert(std::is_integral_v<IdxT>, "RegisterMaxHeap: IdxT must be an integer type.");

    DistT dist[MAX_K]; ///< Squared distances array (max-heap ordered).
    IdxT  idx[MAX_K];  ///< Particle indices array corresponding to distances.
    int   count;       ///< Current heap occupancy.

    static constexpr DistT DIST_MAX = std::numeric_limits<DistT>::max();
    static constexpr IdxT  IDX_NONE = static_cast<IdxT>(-1);

    /**
     * @brief Constructs an empty RegisterMaxHeap with sentinels.
     */
    RegisterMaxHeap() noexcept : count(0) {
#pragma unroll
        for (int i = 0; i < MAX_K; ++i) {
            dist[i] = DIST_MAX;
            idx[i]  = IDX_NONE;
        }
    }

    /**
     * @brief Prune check to decide if a BVH node should be skipped.
     * @param node_min_d2 Minimum possible squared distance to BVH node bounding box.
     * @param k Requested number of nearest neighbors.
     * @return True if heap is full (count >= k) and node_min_d2 >= current worst distance.
     */
    bool should_prune(DistT node_min_d2, int k) const noexcept {
        return count >= k && node_min_d2 >= dist[0];
    }

    /**
     * @brief Inserts a candidate particle if it belongs in the k nearest.
     * @param d2 Candidate squared distance.
     * @param i Candidate particle index.
     * @param k Requested number of nearest neighbors.
     */
    void push(DistT d2, IdxT i, int k) noexcept {
        if (count < k) {
            dist[count] = d2;
            idx[count]  = i;
            sift_up(count);
            count++;
        } else if (d2 < dist[0]) {
            dist[0] = d2;
            idx[0]  = i;
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
        int heap_size      = count;
        while (heap_size > 1) {
            swap_entries(0, heap_size - 1);
            heap_size--;
            sift_down(0, heap_size);
        }
        for (int i = 0; i < k; ++i) {
            if (i < original_count) {
                out_d2[i]  = dist[i];
                out_idx[i] = idx[i];
            } else {
                out_d2[i]  = DIST_MAX;
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
            int l       = (i << 1) + 1;
            int r       = (i << 1) + 2;
            if (l < size && dist[l] > dist[largest]) largest = l;
            if (r < size && dist[r] > dist[largest]) largest = r;
            if (largest == i) break;
            swap_entries(i, largest);
            i = largest;
        }
    }

    void swap_entries(int a, int b) noexcept {
        DistT td = dist[a];
        dist[a]  = dist[b];
        dist[b]  = td;
        IdxT ti  = idx[a];
        idx[a]   = idx[b];
        idx[b]   = ti;
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
inline void local_swap(
    const sycl::local_accessor<DistT, 1> &sh_dist,
    const sycl::local_accessor<IdxT, 1>  &sh_idx,
    int                                   a,
    int                                   b
) noexcept {
    DistT td   = sh_dist[a];
    sh_dist[a] = sh_dist[b];
    sh_dist[b] = td;
    IdxT ti    = sh_idx[a];
    sh_idx[a]  = sh_idx[b];
    sh_idx[b]  = ti;
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
inline void local_bitonic_sort(
    sycl::nd_item<1>                     &item,
    const sycl::local_accessor<DistT, 1> &sh_dist,
    const sycl::local_accessor<IdxT, 1>  &sh_idx,
    int                                   n
) noexcept {
    int lid = static_cast<int>(item.get_local_id(0));

    for (int k = 2; k <= n; k <<= 1) {
        for (int j = k >> 1; j > 0; j >>= 1) {
            for (int i = lid; i < n; i += KNN_WG_SIZE) {
                int  l         = i ^ j;
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

// ================================================================
// Warp-parallel sorted top-k merge, replacing the serial lane-0
// batch_insert. Requires WARP == device sub-group size (assert/query
// this — see note below on sub_group_sizes).
//
// Invariant maintained: sh_dist[0..count) is ascending-sorted at all
// times (this replaces the max-heap invariant). sh_worst == sh_dist[count-1]
// (or DIST_MAX if count < k), read in O(1) with no separate root bookkeeping.
// ================================================================
template <typename DistT, typename IdxT> struct SortedMergeHeap {
    static constexpr DistT DIST_MAX = std::numeric_limits<DistT>::max();
    static constexpr IdxT  IDX_NONE = static_cast<IdxT>(-1);
    static constexpr int   WARP     = 32;

    static void init(
        sycl::nd_item<1>                     &item,
        const sycl::local_accessor<DistT, 1> &sh_dist,
        const sycl::local_accessor<IdxT, 1>  &sh_idx,
        const sycl::local_accessor<int, 1>   &sh_count,
        int                                   cap
    ) noexcept {
        int lid = static_cast<int>(item.get_local_id(0));
        for (int i = lid; i < cap; i += WARP) {
            sh_dist[i] = DIST_MAX;
            sh_idx[i]  = IDX_NONE;
        }
        if (lid == 0) sh_count[0] = 0;
        sycl::group_barrier(item.get_group());
    }

    static bool should_prune(
        DistT                                 node_min_d2,
        const sycl::local_accessor<DistT, 1> &sh_dist,
        const sycl::local_accessor<int, 1>   &sh_count,
        int                                   k,
        int                                   cap
    ) noexcept {
        // worst-of-top-k, not worst-of-cap: once count>=k, the k-th smallest
        // (index k-1) is the live pruning bound, even if cap > k.
        return sh_count[0] >= k && node_min_d2 >= sh_dist[k - 1];
    }

    // One "batch" = up to WARP candidates, one per lane (IDX_NONE = none this round).
    // Sorts the incoming batch, bitonic-merges it against the existing sorted
    // list, truncates to k. All WARP lanes participate throughout — no lane-0
    // serialization.
    static void merge_batch(
        sycl::nd_item<1>                     &item,
        const sycl::local_accessor<DistT, 1> &sh_dist, // size cap (>= k, multiple of WARP)
        const sycl::local_accessor<IdxT, 1>  &sh_idx,
        const sycl::local_accessor<int, 1>   &sh_count,
        const sycl::local_accessor<DistT, 1> &sh_stage_dist, // size WARP
        const sycl::local_accessor<IdxT, 1>  &sh_stage_idx,  // size WARP
        DistT                                 my_d2,
        IdxT                                  my_i,
        int                                   k,
        int                                   cap
    ) noexcept {
        int lid = static_cast<int>(item.get_local_id(0));

        sh_stage_dist[lid] = my_d2;
        sh_stage_idx[lid]  = my_i;
        sycl::group_barrier(item.get_group());

        // 1. Sort the WARP-wide staging buffer (bitonic, in-place, all lanes).
        local_bitonic_sort<DistT, IdxT>(item, sh_stage_dist, sh_stage_idx, WARP);

        // 2. Bitonic merge staged-sorted-run into the existing sorted list.
        //    Classic sorted-merge-via-bitonic: concatenate [sh_dist | reversed
        //    stage] conceptually, run a bitonic merge network sized `cap`.
        //    Implemented here via repeated compare-exchange across the
        //    boundary at stride cap/2, cap/4, ... down to 1 (Batcher's
        //    odd-even merge over the two already-sorted cap-halves).
        //
        //    To keep this in local memory without a second `cap`-sized
        //    scratch buffer, append the staged run past sh_count and run one
        //    bitonic-sort pass over [0, count+WARP) each round -- simpler to
        //    reason about than a true O(log) merge network and still fully
        //    parallel; at cap in the low hundreds the extra log-factor cost
        //    is negligible next to eliminating the single-lane serialization.
        for (int i = lid; i < WARP; i += WARP) {
            int dst = sh_count[0] + i; // lane 0 hasn't updated sh_count yet this round
            if (dst < cap) {
                sh_dist[dst] = sh_stage_dist[i];
                sh_idx[dst]  = sh_stage_idx[i];
            }
        }
        sycl::group_barrier(item.get_group());

        int new_count = sycl::min(sh_count[0] + WARP, cap);
        // round up to next pow2 for bitonic_sort's stride math, sentinel-padded
        int pow2 = 1;
        while (pow2 < new_count)
            pow2 <<= 1;
        for (int i = new_count + lid; i < pow2 && i < cap; i += WARP) {
            sh_dist[i] = DIST_MAX;
            sh_idx[i]  = IDX_NONE;
        }
        sycl::group_barrier(item.get_group());

        local_bitonic_sort<DistT, IdxT>(item, sh_dist, sh_idx, pow2);

        if (lid == 0) sh_count[0] = sycl::min(new_count, k > cap ? cap : cap); // clamp to k below
        sycl::group_barrier(item.get_group());
        if (lid == 0) sh_count[0] = sycl::min(sh_count[0], k);
        sycl::group_barrier(item.get_group());
    }
};

// ================================================================
// LocalMaxHeap — per-lane private max-heap backed by local memory.
//
// Algorithmically IDENTICAL to RegisterMaxHeap (same push/sift_up/
// sift_down logic). The only difference is storage: instead of a
// private register array `dist[MAX_K]` per thread, each lane owns
// one row of a shared local_accessor sized [WARP * cap]. Lane `l`
// only ever reads/writes row `l` — there is no cross-lane
// cooperation or synchronization inside push/should_prune, so this
// composes directly with the SFC-grouped traversal from point 1
// (each lane still runs its own independent query; only the k>32
// register-pressure problem is solved by relocating storage).
// ================================================================

template <typename DistT, typename IdxT> struct LocalMaxHeap {
    static_assert(
        std::is_floating_point_v<DistT> || std::is_integral_v<DistT>,
        "LocalMaxHeap: DistT must be float, double, or an unsigned integer type."
    );
    static_assert(std::is_integral_v<IdxT>, "LocalMaxHeap: IdxT must be an integer type.");

    static constexpr DistT DIST_MAX = std::numeric_limits<DistT>::max();
    static constexpr IdxT  IDX_NONE = static_cast<IdxT>(-1);

    // ── Initialize this lane's row with sentinels. Called by every lane
    //    independently — no barrier needed since rows don't overlap.
    static void init(
        int                                   lane,
        const sycl::local_accessor<DistT, 1> &dist, // size WARP * cap
        const sycl::local_accessor<IdxT, 1>  &idx,  // size WARP * cap
        int                                   cap
    ) noexcept {
        int base = lane * cap;
#pragma unroll
        for (int i = 0; i < cap; ++i) {
            dist[base + i] = DIST_MAX;
            idx[base + i]  = IDX_NONE;
        }
    }

    // ── O(1) prune check against this lane's own heap root.
    static bool should_prune(
        int lane, DistT d2, const sycl::local_accessor<DistT, 1> &dist, int count, int k, int cap
    ) noexcept {
        return count >= k && d2 >= dist[lane * cap];
    }

    // ── Insert a candidate into this lane's row. `count` is passed by
    //    reference and lives in the caller's private (register) state —
    //    it does NOT need to be in local memory, since only this lane
    //    ever reads or writes it (unlike SortedMergeHeap's sh_count,
    //    which is genuinely shared).
    static void push(
        int                                   lane,
        DistT                                 d2,
        IdxT                                  i,
        int                                  &count,
        const sycl::local_accessor<DistT, 1> &dist,
        const sycl::local_accessor<IdxT, 1>  &idx,
        int                                   k,
        int                                   cap
    ) noexcept {
        int base = lane * cap;
        if (count < k) {
            int slot          = count;
            dist[base + slot] = d2;
            idx[base + slot]  = i;
            sift_up(lane, slot, dist, idx, cap);
            count++;
        } else if (d2 < dist[base]) {
            dist[base] = d2;
            idx[base]  = i;
            sift_down(lane, 0, count, dist, idx, cap);
        }
    }

    // ── In-place heapsort of this lane's row into ascending order.
    //    Deliberately does NOT write to output buffers itself (unlike
    //    RegisterMaxHeap::extract_sorted) because the caller's output
    //    index type (e.g. size_t for dev_results) generally differs from
    //    IdxT (int) — sorting and type-converting-on-write are kept as
    //    separate steps so this heap stays reusable regardless of the
    //    caller's result-buffer types. After this call, dist[base..base+
    //    count) / idx[base..base+count) are ascending-sorted and the
    //    caller reads them directly (see call site below).
    static void sort_in_place(
        int                                   lane,
        int                                   count,
        const sycl::local_accessor<DistT, 1> &dist,
        const sycl::local_accessor<IdxT, 1>  &idx,
        int                                   cap
    ) noexcept {
        int base      = lane * cap;
        int heap_size = count;
        while (heap_size > 1) {
            swap_entries(base, base + heap_size - 1, dist, idx);
            heap_size--;
            sift_down(lane, 0, heap_size, dist, idx, cap);
        }
    }

  private:
    static void sift_up(
        int                                   lane,
        int                                   i,
        const sycl::local_accessor<DistT, 1> &dist,
        const sycl::local_accessor<IdxT, 1>  &idx,
        int                                   cap
    ) noexcept {
        int base = lane * cap;
        while (i > 0) {
            int p = (i - 1) >> 1;
            if (dist[base + i] <= dist[base + p]) break;
            swap_entries(base + i, base + p, dist, idx);
            i = p;
        }
    }

    static void sift_down(
        int                                   lane,
        int                                   i,
        int                                   size,
        const sycl::local_accessor<DistT, 1> &dist,
        const sycl::local_accessor<IdxT, 1>  &idx,
        int                                   cap
    ) noexcept {
        int base = lane * cap;
        while (true) {
            int largest = i;
            int l       = (i << 1) + 1;
            int r       = (i << 1) + 2;
            if (l < size && dist[base + l] > dist[base + largest]) largest = l;
            if (r < size && dist[base + r] > dist[base + largest]) largest = r;
            if (largest == i) break;
            swap_entries(base + i, base + largest, dist, idx);
            i = largest;
        }
    }

    static void swap_entries(
        int                                   a,
        int                                   b,
        const sycl::local_accessor<DistT, 1> &dist,
        const sycl::local_accessor<IdxT, 1>  &idx
    ) noexcept {
        DistT td = dist[a];
        dist[a]  = dist[b];
        dist[b]  = td;
        IdxT ti  = idx[a];
        idx[a]   = idx[b];
        idx[b]   = ti;
    }
};

// ================================================================
// SECTION 4: Type aliases
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

template <int MAX_K> using SmallKHeap = RegisterMaxHeap<heap_dist_t, int, MAX_K>;

using LargeKHeap = SortedMergeHeap<heap_dist_t, int>;
using LocalKHeap = LocalMaxHeap<heap_dist_t, int>;

} // namespace fasttree

#endif // FASTTREE_MAXHEAP_HPP
