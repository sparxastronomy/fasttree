# Release: v.1.0.0-beta2 (Distributed Domain Decomposition)

Here is a tentative, step-by-step  plan for implementing the **Distributed Domain Decomposition** using MPI and SYCL.

This plan leverages the coarse bits of  64-bit Morton keys to achieve perfect load balancing across thousands of GPUs, resolving the extreme density variations (halos vs. voids) found in cosmological simulations like AREPO.


## Objective

Distribute $N$ total particles across $P$ MPI ranks such that:

1. Every rank ends up with approximately $N / P$ particles (Perfect Memory & Compute Load Balancing).
2. Every rank owns a single, contiguous segment of the global 3D Morton curve (Perfect Spatial Locality for the Local Essential Tree).

## Phase 1: Global Bounding Box & Local Encoding

Before generating Morton keys, all MPI ranks must agree on the exact same global coordinate space. If they don't, their Morton keys will not align.

1. **Local Reduction:** Every rank uses `sycl::reduce_over_group` to find its local `min_x`, `max_x`, etc.
2. **Global Reduction:** Call `MPI_Allreduce` with `MPI_MIN` and `MPI_MAX` so every rank obtains the exact same global `BoundingBox`.
3. **Morton Encoding:** Every rank calls your existing `morton_encode(q, local_particles, local_keys, global_bbox)` kernel.

## Phase 2: Coarse-Grid Histogram (The "Virtual" Voxels)

Instead of sorting billions of particles globally, we sort the *virtual voxels* they fall into. We will use the top $m$ bits of the 64-bit key to define these buckets.

* *Recommendation:* 
    - Use $m = 20$ bits. Since `hlbvh.hpp` uses `BITS_PER_DIM = 21`, the Morton key is 63 bits total (bit 63 is always 0). To extract exactly $m=20$ bits, we must shift the keys by `63 - 20 = 43` bits. This creates $2^{20} \approx 1.04$ million active buckets.
    - Or use $m$ such that the global buckets is equal to the number of MPI-ranks. This is the most straightforward approach.

1. **Allocate Local Histogram:** Allocate an array `int local_hist[1048576]` initialized to zero.
2. **Populate Local Histogram (SYCL):** Launch a kernel over your local particles.
    ```cpp
    uint32_t bucket_idx = morton_keys[i] >> (63 - 20); // Extract top 20 active bits (shift by 43)
    sycl::atomic_ref<int, ...>(local_hist[bucket_idx]).fetch_add(1);
    ```

3. **Global Histogram (MPI):**
    ```cpp
    int global_hist[1048576];
    MPI_Allreduce(local_hist, global_hist, 1048576, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    ```
    Now we know exactly how many particles fall into each of the 1.04 million buckets across the entire nodes.

*Result:* Every rank now knows the exact spatial distribution of every particle in the entire supercomputer.

## Phase 3: Curve Partitioning (Splitter Generation)

We must divide the 1.04 million buckets into $P$ chunks so that each chunk contains approximately `target_load = Total_Particles / P` particles.
Because `global_hist` is identical on all nodes, every rank can compute this redundantly without further communication.

1. **Prefix Sum & Splitting:**
    ```cpp
    std::vector<uint32_t> rank_splitters(P + 1);
    rank_splitters[0] = 0;

    long long current_particles = 0;
    int current_rank = 1;

    for(uint32_t bucket = 0; bucket < 1048576; ++bucket) {
        current_particles += global_hist[bucket];

        // Safely catch any bucket jumps caused by extreme clustering
        while (current_rank < P && current_particles >= target_load * current_rank) {
            rank_splitters[current_rank] = bucket;
            current_rank++;
        }
    }
    // Fill remaining splitters if current_rank did not reach P
    while (current_rank <= P) {
        rank_splitters[current_rank] = 1048576;
        current_rank++;
    }
    ```

*Note on Load Balancing:* If a single coarse bucket contains $N_{clustered}$ particles where $N_{clustered} > \text{target\_load}$ (due to dense halos), that bucket cannot be split. The rank assigned to it will receive all of them. Therefore, while partitioning is designed to balance the load, we do not have a hard guarantee that no rank will receive more than `target_load`. We must allocate receive buffers dynamically.

*Result:* `rank_splitters` now tells us exactly which coarse Morton bits belong to which MPI rank.

## Phase 4: Local Binning & Network Routing

Now every rank must prepare its data to be sent across the network.

1. **Bucket Particles (SYCL):**
Launch a kernel that reads a particle's top 20 bits, checks `rank_splitters` via **binary search** to determine its destination rank (0 to $P-1$) in $O(\log P)$ time.
    ```cpp
    // Inside SYCL kernel for a single local particle
    uint32_t bucket_id = local_morton_keys[i] >> (63 - 20); // Extract bucket ID

    int low = 0;
    int high = P - 1;
    int dest_rank = 0;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (bucket_id >= rank_splitters[mid]) {
            dest_rank = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    ```

2. **Local Reordering & Binning:**
To group particles by `dest_rank`:
- Compute a histogram of `dest_rank` (size $P$) on the GPU to determine `send_counts`.
- Sort particle indices by their `dest_rank` on the GPU using `oneapi::dpl::sort`.
- Use the sorted indices to pack particle properties (`pos_x`, `pos_y`, `pos_z`, `mass`, `id`, etc.) into contiguous send buffers.

3. **The Grand Shuffle (MPI):**
- First, call `MPI_Alltoall` to exchange `send_counts` and obtain `recv_counts` from all ranks.
- Allocate receive buffers and compute receive displacements `rdispls`.
- Execute `MPI_Alltoallv` to shuffle particle data across ranks.

## Phase 5: Explicit Halo Exchange (Static Ghosting)
We do an explicit Halo-exchange after the shuffle to ensure that every rank has the necessary neighboring particles to build a correct local tree and the SYCL kernels can run without divergence.
Because `dpl::sort` would mix the particles we need to identify the halo particles. The easiest way to do this is modify the `ParticlesSoA`
```cpp
struct ParticlesSoA {
  std::vector<float> pos_x, pos_y, pos_z;
  std::vector<float> mass;
  std::vector<uint32_t> id;
  std::vector<int8_t> is_ghost; // 0 = Local, 1 = Ghost
};
```
Right after `MPI_Alltoallv` domain decomposition finishes, every rank holds exactly $N_{local}$ particles. Before building the tree, we execute the halo exchange.
1. Broadcast Domain Bounds: Every rank computes the spatial bounding box of its $N_{local}$ particles. You execute an MPI_Allgather so every rank knows the exact bounding box of every other rank.
2. Identify Neighbors: Rank $i$ expands its bounding box by $h_{max}$ (the maximum smoothing length/search radius in the simulation). It checks which other ranks' bounding boxes intersect this expanded box.
3. Exchange Ghosts: Rank $i$ sends copies of its particles that fall near the boundary to the intersecting ranks, and receives ghost particles in return.
4. Append & Tag: 
    ```cpp
    // When Rank i receives a batch of ghost particles:
    for (auto& p : received_ghosts) {
    particles.pos_x.push_back(p.x);
    // ... push y, z, mass, id ...
    particles.is_ghost.push_back(1); // Tag as ghost!
    }
    ```


## Phase 6: Finalization & Local Tree Build

At this point, the global domain decomposition is complete. You now have $N_{total} = N_{local} + N_{ghost}$ particles.

1. Every rank has received its slice of the universe. 
2. Every rank has an equal number of particles.
3. Every rank has particles that are strictly bounded within its assigned segment of the Morton curve.
4. **Execute `build_bvh`:** You now call your one-shot `oneapi::dpl::sort` and Karras builder exactly as we designed it for a single node.

### NOTE: Masked Physics Evaluation (for future full hydro / N-body sims)
Because the particles are now mixed, we simply use the `is_ghost` mask to dictate which threads do the actual physics updates. When we launch  SPH density/force kernel, we launch it over all $N_{total}$ particles in the sorted tree, but mask out the ghosts from doing active computations:
```cpp
q.parallel_for(sycl::range<1>(n_total), [=](sycl::id<1> idx) {
    int i = idx[0];
    // ALL particles exist in the tree, but ghosts don't compute their own forces.
    // They only exist to be found by the local particles during the tree search.
    if (is_ghost[i]) return; 
    float px = sorted_x[i];
    float py = sorted_y[i];
    float pz = sorted_z[i];
    // 1. Perform range_query / knn_query
    // 2. Compute hydrodynamic forces / gravity
    // 3. Write resulting forces to a separate SoA array
});
```

When it is time to do the Kick-Drift-Kick integration to step the simulation forward in time, we only integrate the local particles. Once the timestep is complete:      
1. Discard all particles where is_ghost == 1.
2. You rebuild the global_hist for the next domain decomposition (if re-balancing is needed), or just repeat the Halo Exchange with the newly updated local positions.

By building the system this way, the logic is entirely contained within the MPI setup phases. The actual GPU execution—which runs thousands of times per second should then remains a pure, uninterrupted, cache-optimized pipeline.

## Code Layout
We keep the domain decomposition code in a separte file like `domain_decomposition.hpp` and the needed MPI functions in `mpi_utils.hpp`. This way, the main tree-building and physics kernels can remain clean and focus on their specific tasks, while the complex logic of distributed load balancing is neatly encapsulated.


## Unit tests
All the tests for the distributed domain are in the sub-folder `tests/domain_decomposition`. We design the following tests:

Here are the plan for Domain Decomposition pipeline, broken down by the phases we established.

### 1. The Splitter Generation Test (Pure CPU / Math)

This is the most critical test. Verify that the load-balancing logic works perfectly when fed extreme cosmological data distributions. Because this step relies purely on a `global_hist` array, we can test it on a single CPU without MPI.

* **Test Case A: Uniform Distribution**
    * **Input:** A `global_hist` of 1,000 buckets, where every bucket has exactly 10 particles. ($N = 10,000$, $P = 4$).
    * **Assert:** `rank_splitters` should perfectly divide the buckets at indices `0, 250, 500, 750, 1000`.


* **Test Case B: Extreme Clustering (The Cosmology Edge Case)**
    * **Input:** A `global_hist` of 1,000 buckets. Bucket 500 contains 9,900 particles. Buckets 0-499 and 501-999 contain a total of 100 particles. ($P = 4$).
    * **Assert:** The logic does not crash or assign fractional buckets. One rank should be forced to take Bucket 500, even if it temporarily violates the perfect $N/P$ target, while the other ranks take the empty voids.
    * **Assert:** `rank_splitters[0] == 0` and `rank_splitters[P] == num_buckets`.

```cpp
// Example GoogleTest / Catch2 snippet for Splitter Logic
TEST_CASE("Splitter Generation handles extreme clustering") {
    int num_buckets = 1000;
    int P = 4;
    std::vector<int> global_hist(num_buckets, 0);
    
    // Simulate a massive galaxy cluster at bucket 500
    global_hist[500] = 9900;
    for(int i=0; i<100; i++) global_hist[i] = 1; // Sparse background

    std::vector<int> splitters = generate_splitters(global_hist, P);

    REQUIRE(splitters.size() == P + 1);
    REQUIRE(splitters[0] == 0);
    REQUIRE(splitters[P] == num_buckets);
    
    // Verify monotonically increasing
    for(int i=0; i<P; i++) {
        REQUIRE(splitters[i] <= splitters[i+1]);
    }
}

```

### 2. The Local Binning & Routing Test (SYCL)

We need to guarantee that the SYCL kernel maps a particle's 64-bit Morton key to the correct MPI destination rank using the splitters.

* **Test Case A: Key Extraction**
    * **Input:** A known set of mock 64-bit Morton keys.
    * **Assert:** The bit-shift operation `key >> (64 - m)` extracts the correct top $m$ bits (the bucket ID). Test the absolute minimum key (`0x000...`) and maximum key (`0xFFF...`).


* **Test Case B: Destination Mapping**
    * **Input:** A mock `rank_splitters` array (e.g., `[0, 10, 50, 100]`) and a particle with bucket ID `25`.
    * **Assert:** The SYCL kernel assigns `dest_rank = 1`.



### 3. Particle Conservation Test (MPI Integration)

This test requires running `mpirun -np 4` or similar. It verifies that the local prefix sums (for packing send buffers) and `MPI_Alltoallv` parameters (send counts, displacements) are flawless.

* **Test Case: The Grand Shuffle**
    * **Setup:** Rank 0 spawns 10,000 particles. Rank 1, 2, and 3 spawn 0 particles. (This simulates massive load imbalance at startup).
    * **Action:** Run the full Domain Decomposition pipeline (Allreduce bounding box -> Histogram -> Splitters -> Alltoallv).
    * **Assert 1 (Global Conservation):** After the shuffle, execute an `MPI_Allreduce` with `MPI_SUM` on the local particle counts. The total *must* be exactly 10,000. If it is 9,999 or 10,001, you have a buffer offset bug.
    * **Assert 2 (Load Balance):** Every rank should report `local_particle_count == 2500` (assuming uniform spatial distribution for this test).



### 4. Spatial Consistency Test (MPI Integration)

It is not enough that particles survive the network transfer; they must arrive at the *correct* rank.

* **Test Case: Boundary Verification**
    * **Action:** After the `MPI_Alltoallv` shuffle, every rank iterates over its newly received local particles.
    * **Assert:** For every particle $i$ on Rank $R$, extract its bucket ID.
    * **Check:** `REQUIRE(bucket_id >= rank_splitters[R] && bucket_id < rank_splitters[R+1])`.
    * *Why this matters:* If this test passes, you have mathematically proven that the Morton domains are strictly isolated, which guarantees your local tree building and kNN queries won't accidentally cross un-ghosted boundaries.



### 5. The Ghost Buffer / Halo Exchange Test (MPI Integration)

If you implemented the explicit halo exchange (Approach 2), you must test that the expanded bounding boxes actually catch overlapping data.

* **Test Case: Ghost Overlap**
    * **Setup:** Create two particles exactly on the boundary line between Rank 0 and Rank 1.
    * **Action:** Run the Halo Exchange with a search radius $h$ large enough to cover the distance between the particles.
    * **Assert:** Rank 0's particle array now contains a copy of Rank 1's particle (with `is_ghost == 1`), and Rank 1 contains a copy of Rank 0's particle.
    * **Assert:** The total number of `is_ghost == 0` (real) particles across all ranks has not changed.

## Profiling & Benchmark
To understand the performance implications of the distributed domain decomposition, we will implement a profiling suite that measures the following:

1. Time taken for each phase of the domain decomposition (bounding box reduction, histogram generation, splitter generation, local binning, MPI communication (Alltoallv))
    - All of this for a range of particle counts (1K, 10K, 100K, 1M, 10M, 100M, 1B) (see DESIGN.md) and MPI ranks (2, 4, 8, 16).
    - The MPI ranks should be distributed across different nodes as SYCL handles the node-local parallel execution. 
    - For tests with more than 1M particles we can't  read the data into a single node, so the read must be done across different node.
    - A bash script will be provided to automate the execution of these benchmarks across different cluster configurations and aggregate the results into a markdown tables.


---
