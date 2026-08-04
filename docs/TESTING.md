# Testing Documentation

The project includes an extensive test suite verifying the functional correctness, structural topology, and distributed MPI capabilities of the HLBVH implementation. All tests are located in the `test/` directory.

## 1. Core Integration Test (`main.cpp`)
**Target:** `fasttree.exe`
**Purpose:** Verifies the complete end-to-end functionality of the library locally across floating-point and integer coordinate representations.
**Execution:**
```bash
# Floating-point build
cd build_cpu && ./fasttree.exe

# 32-bit, 64-bit, and 128-bit integer coordinate builds
cd build_int32 && ./fasttree.exe
cd build_int64 && ./fasttree.exe
cd build_int128 && ./fasttree.exe
```
**Test Workflow:**
1. Generates a deterministic $10 \times 10 \times 10 = 1,000$ particle 3D grid across $[0.0, 100.0]^3$.
2. In integer modes (`FASTTREE_INTEGER_COORDS`), converts positions to discrete integer representations using `float_to_int_rep()`.
3. Executes `build_bvh` to compute SFC keys (Morton/Peano-Hilbert), sort points, and build the `TreeSoA` structure.
4. Validates strict SFC key monotonicity across all sorted particles.
5. Issues a `range_query` for particles within a fixed radius.
6. Uses CPU-based brute-force distance calculation to compute the expected particle count and validates the GPU tree results against it.
7. Executes `knn_query` for both small $k=5$ ($k \le 32$, register-resident `RegisterMaxHeap`) and large $k=48$ ($k > 32$, shared-memory bitonic sort `SharedMaxHeap`), printing physical floating-point coordinates and squared distances, and verifying identical nearest particle IDs and ascending squared distances across floating-point and integer coordinate representation builds.

---

## 2. Basic MPI Serialization Test (`test_mpi.cpp`)
**Target:** `test_mpi`
**Purpose:** Validates that the pointer-free `TreeSoA` layout can be serialized, transmitted across distributed processes, and successfully queried on the receiving end.
**Execution:**
```bash
mpirun -np 2 ./test_mpi
```
**Test Workflow:**
1. **Rank 0:** Generates 1,000 particles, builds a `TreeSoA`, and serializes the raw underlying arrays (min_x, max_x, parent, etc.). It then transmits these flat arrays to Rank 1 via `MPI_Send`.
2. **Rank 1:** Pre-allocates a `TreeSoA` using the received dimensional metadata, receives the raw arrays into its USM pointers via `MPI_Recv`, and executes a `range_query` directly against the transferred tree structure. It expects exactly 20 particles; an abort is triggered if the count mismatches.

---

## 3. Advanced MPI Halo-Exchange (`advance_mpi.cpp`)
**Target:** `advance_mpi`
**Purpose:** Simulates a realistic domain decomposition scenario for cosmological simulations, benchmarking both the performance of local sub-tree extraction and MPI transmission costs.
**Execution:**
```bash
mpirun -np 2 ./advance_mpi
```
**Test Workflow:**
1. **Domain Setup:** Rank 0 owns domain `[0, 100]`, and Rank 1 owns domain `[100, 200]`. Each generates 100,000 localized particles.
2. **Halo Extraction:** Rank 0 identifies particles close to the boundary (`[90, 100]`) and Rank 1 identifies particles in `[100, 110]`.
3. **Halo Tree Construction:** Both ranks build specialized boundary `TreeSoA` objects for only their halo particles.
4. **Exchange:** Ranks perform an `MPI_Sendrecv` to simultaneously swap their serialized halo trees with their neighbor.
5. **Verification:** Each rank queries the newly received Locally Essential Tree (LET) to ensure integrity.
6. **Reporting:** The test outputs detailed high-precision timings (in milliseconds) for Local Build, Halo Build, MPI Transmission, and Tree Verification.