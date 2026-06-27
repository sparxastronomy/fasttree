#!/usr/bin/env bash
set -euo pipefail

# Print usage instructions
usage() {
    echo "Usage: $0 <cpu|gpu> <true|false>"
    echo "  Second argument (true/false) specifies whether to run MPI benchmarks."
    exit 1
}

# Verify arguments
if [ "$#" -ne 2 ]; then
    usage
fi

DEVICE="$1"
MPI_ENABLED="$2"

if [ "$DEVICE" = "cpu" ]; then
    BUILD_DIR="build_cpu"
    TARGET_GPU=""
elif [ "$DEVICE" = "gpu" ]; then
    BUILD_DIR="build_gpu"
    TARGET_GPU="nvidia"
else
    echo "Error: Invalid device '$DEVICE'. Must be 'cpu' or 'gpu'."
    usage
fi

if [ "$MPI_ENABLED" != "true" ] && [ "$MPI_ENABLED" != "false" ]; then
    echo "Error: Invalid mpi flag '$MPI_ENABLED'. Must be 'true' or 'false'."
    usage
fi

# Locate project root directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/../.." && pwd)"

echo "=== FastTree Scaling Benchmark Automation ==="
echo "Target Device: $DEVICE"
echo "MPI Enabled:   $MPI_ENABLED"
echo "Project Root:  $ROOT_DIR"
echo "Build Dir:     $ROOT_DIR/$BUILD_DIR"
echo "============================================="

# Ensure build directory exists
mkdir -p "$ROOT_DIR/$BUILD_DIR"
cd "$ROOT_DIR/$BUILD_DIR"

if [ "$MPI_ENABLED" = "true" ]; then
    OUTPUT_FILE="$ROOT_DIR/docs/mpi_benchmark_results_${DEVICE}_all_variations.md"
else
    OUTPUT_FILE="$ROOT_DIR/docs/benchmark_results_${DEVICE}_all_variations.md"
fi
mkdir -p "$(dirname "$OUTPUT_FILE")"

# Initialize markdown report
echo "# Benchmark Run: $DEVICE" > "$OUTPUT_FILE"
echo "Date: $(date)" >> "$OUTPUT_FILE"
echo "Device: $DEVICE" >> "$OUTPUT_FILE"
echo "=============================================" >> "$OUTPUT_FILE"
echo "" >> "$OUTPUT_FILE"

# Define configurations
SFC_TYPES=("MORTON" "PEANO_HILBERT")
DECOMP_TYPES=("HISTOGRAM" "SAMPLING")
COORDS_TYPES=("FLOAT" "DOUBLE")

# Loop through permutations
for sfc in "${SFC_TYPES[@]}"; do
    for decomp in "${DECOMP_TYPES[@]}"; do
        for coords in "${COORDS_TYPES[@]}"; do
            echo "--------------------------------------------------"
            echo "Running Configuration:"
            echo "  SFC Curve:    $sfc"
            echo "  Decomp Type:  $decomp"
            echo "  Coords Type:  $coords"
            echo "--------------------------------------------------"
            
            # Configure CMake
            cmake -DCMAKE_BUILD_TYPE=Release \
                  -DSFC_TYPE="$sfc" \
                  -DDCOMPOSITION_TYPE="$decomp" \
                  -DCOORDS_TYPE="$coords" \
                  -DTARGET_GPU="$TARGET_GPU" \
                  -DBENCHMARK=ON \
                  -DCMAKE_C_COMPILER=clang \
                  -DCMAKE_CXX_COMPILER=clang++ \
                  -DMPI_C_COMPILER=mpiicx \
                  -DMPI_CXX_COMPILER=mpiicpx \
                  "$ROOT_DIR"
            
            # Build the binaries
            make -j32

            # ensure OMP_NUM_THREADS is set to a reasonable default for single-node runs
            export OMP_NUM_THREADS=72

            # Record configuration header in the markdown report
            echo "## Configuration: SFC=$sfc, Decomp=$decomp, Coords=$coords" >> "$OUTPUT_FILE"
            echo '```' >> "$OUTPUT_FILE"

            if [ "$MPI_ENABLED" = "false" ]; then
                # 1. Run single-node benchmarks (OMP_NUM_THREADS = 72)
                export OMP_NUM_THREADS=72
                echo "Running Single-Node Benchmarks..."
                
                echo "--- SFC Encoding Scaling ---" >> "$OUTPUT_FILE"
                if [ -f "./sfc_encoding_scaling.exe" ]; then
                    ./sfc_encoding_scaling.exe >> "$OUTPUT_FILE" 2>&1 || true
                fi

                echo "--- GPU/Radix Sort Scaling ---" >> "$OUTPUT_FILE"
                if [ -f "./gpu_sort_scaling.exe" ]; then
                    ./gpu_sort_scaling.exe >> "$OUTPUT_FILE" 2>&1 || true
                fi

                echo "--- Tree Build Scaling ---" >> "$OUTPUT_FILE"
                if [ -f "./tree_build_scaling.exe" ]; then
                    ./tree_build_scaling.exe >> "$OUTPUT_FILE" 2>&1 || true
                fi

                echo "--- Rebuild Scaling ---" >> "$OUTPUT_FILE"
                if [ -f "./rebuild_scaling.exe" ]; then
                    ./rebuild_scaling.exe >> "$OUTPUT_FILE" 2>&1 || true
                fi

                echo "--- Range Query Scaling ---" >> "$OUTPUT_FILE"
                if [ -f "./range_query_scaling.exe" ]; then
                    ./range_query_scaling.exe >> "$OUTPUT_FILE" 2>&1 || true
                fi

                echo "--- kNN Query Scaling ---" >> "$OUTPUT_FILE"
                if [ -f "./knn_query_scaling.exe" ]; then
                    ./knn_query_scaling.exe >> "$OUTPUT_FILE" 2>&1 || true
                fi
            else
                # 2. Run MPI benchmarks (OMP_NUM_THREADS = 36)
                export OMP_NUM_THREADS=36
                export OMP_PLACES=cores
                export OMP_PROC_BIND=close
                export I_MPI_PIN_DOMAIN=socket
                echo "Running MPI Scaling Benchmarks (1 rank / NUMA domain, 36 threads)..."
                
                echo "--- Distributed Domain Decomposition Scaling ---" >> "$OUTPUT_FILE"
                if [ -f "./domain_decomposition_scaling.exe" ]; then
                    for ranks in 2 4 8 16; do
                        echo "MPI Ranks: $ranks" >> "$OUTPUT_FILE"
                        mpirun -n "$ranks" ./domain_decomposition_scaling.exe >> "$OUTPUT_FILE" 2>&1 || true
                    done
                fi
            fi

            echo '```' >> "$OUTPUT_FILE"
            echo "" >> "$OUTPUT_FILE"
        done
    done
done

echo "=== All configurations completed! Results written to $OUTPUT_FILE ==="
