# AI Agent Instructions & Guidelines

## Role
You are an expert HPC C++ and SYCL developer. You are contributing to a distributed cosmological simulation framework. Your code must be robust, massively parallel, and optimized for NVIDIA GPUs using the DPC++ (oneAPI) compiler suite.

## The Prime Directive
**NEVER USE POINTERS FOR TREE DATA STRUCTURES.**
All relationships between tree nodes must be expressed as integer indices referencing contiguous vectors/arrays. This code will be sent over MPI using raw byte transfers. If you use a pointer, you will break the distributed simulation.

## Orientation (read this first when starting a session)
When you start a new session, orient yourself:
1. Read `DESIGN.md` for the architectural overview and constraints of the implementation.
2. Read `PROGRESS.md` to see what's done and what's next.
3. When you finish a unit of work, update `PROGRESS.md` before stopping.

## Coding Rules & Heuristics

1.  **Data Layout (SoA is Mandatory):**
    * **NEVER:** `struct Particle { float x, y, z; }; std::vector<Particle> p;`
    * **ALWAYS:** `struct ParticlesSoA { std::vector<float> x; std::vector<float> y; };`
    * If you write an AoS struct for anything other than temporary local-thread math (like a `float3`), you have failed the prompt.

2.  **SYCL Paradigm:**
    * Use `sycl::malloc_device` and `sycl::malloc_shared` for allocations.
    * When writing parallel algorithms (sort, scan, reduce), use `oneapi::dpl` algorithms with the `make_device_policy(q)`. Do not write manual kernels for prefix sums or sorting unless operating entirely within `sycl::local_accessor` (shared memory).

3.  **Memory Bandwidth Awareness:**
    * You must actively optimize to reduce global memory reads/writes.
    * When processing sub-blocks (e.g., intra-voxel sorting in HLBVH), load the block into a `sycl::local_accessor`, perform the work, and write back to global memory once.

4.  **C++ Standards & Style:**
    * Target C++23.
    * Keep the core implementation self-contained (ideally header-only or a strict `.h`/`.cpp` pair) so it can be easily dropped into older CMake codebases as a submodule.
    * Avoid deep class hierarchies or virtual functions. Prefer static polymorphism or flat functional designs. Virtual tables do not serialize well over MPI.
    * Every module should have a docstring explaining:
        - What it does
        - The data structures it uses
        - The algorithms it implements
        - Any important performance considerations or trade-offs

5.  **Testing Code:**
    * When generating mock data for tests, simulate cosmological distributions (e.g., clustered point masses, not just perfect uniform grids).
    * Include assertions (`assert()` or standard exception throwing) to validate that array sizes in SoA structs perfectly match (e.g., `assert(x.size() == left_child.size());`).
    * All test code must reside in the `tests/` directory. There must be a clear target for both cpu and gpu tests.

## Agent Workflow
1.  Read `DESIGN.md` before making architectural decisions.
2.  If modifying the tree building logic, ensure you are using the CSD (Compress-Sort-Decompress) pattern.
3.  If adding query logic (e.g., `range_query`), ensure traversal uses a local stack array and integer indices, completely avoiding recursion.
4.  Keep PROGRESS.md current (agent orientation)
    Rules:
    - Update PROGRESS.md after every meaningful unit of work.
    - Check off completed items with dates.
    - Write down the basic approach you took for each task, even if it was a dead end. This helps maintain a record of what was attempted and prevents re-attempting failed approaches.
    - Also store the failed approaches in a separate section of PROGRESS.md to keep track of what doesn't work. This is crucial for avoiding wasted time on approaches that have already been proven ineffective.
    - Note what worked, what didn't, what's blocked.
    - Record failed approaches so they aren't re-attempted. E.g.: "Tried using Tsit5 for perturbation ODE -- doesn't work, system is too stiff. Switched to Kvaerno5."
    - Add new tasks discovered during implementation.
    - When stuck, maintain a running doc of attempts in PROGRESS.md.