#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/../.." && pwd)"
CMAKE_BIN="/mpcdf/soft/SLE_15/packages/x86_64/cmake/4.2/bin/cmake"

OUTPUT_DIR="$ROOT_DIR/docs/benchmark_results"
mkdir -p "$OUTPUT_DIR"

BENCHMARK_EXES=(
    "sfc_encoding_scaling.exe"
    "gpu_sort_scaling.exe"
    "tree_build_scaling.exe"
    "rebuild_scaling.exe"
    "range_query_scaling.exe"
    "knn_query_scaling.exe"
    "self_knn_query_scaling.exe"
)

# Configuration tuples for GPU runs: (BUILD_DIR, MODE_NAME, EXTRA_CMAKE_FLAGS)
CONFIGS=(
    "build_gpu|gpu_double|COORDS_REPRESENTATION=FLOAT|COORDS_TYPE=DOUBLE|POSITIONS_PRECISION=32"
    "build_int32|gpu_int32|COORDS_REPRESENTATION=INTEGER|COORDS_TYPE=FLOAT|POSITIONS_PRECISION=32"
    "build_int64|gpu_int64|COORDS_REPRESENTATION=INTEGER|COORDS_TYPE=FLOAT|POSITIONS_PRECISION=64"
)

PERIODIC_MODES=("OFF" "ON")

GPU_CXX="${GPU_CXX:-/vera/ptmp/gc/bipra/softwares/illvm/build/install/bin/clang++}"
if [ ! -x "$GPU_CXX" ]; then
    GPU_CXX="clang++"
fi

for config in "${CONFIGS[@]}"; do
    IFS="|" read -r build_dir mode_name coords_rep coords_type pos_prec <<< "$config"

    for periodic in "${PERIODIC_MODES[@]}"; do
        echo "=========================================================================="
        echo "Running GPU Benchmark Config: Mode=$mode_name, Periodic=$periodic"
        echo "Build Dir: $ROOT_DIR/$build_dir"
        echo "Compiler: $GPU_CXX"
        echo "=========================================================================="

        mkdir -p "$ROOT_DIR/$build_dir"
        cd "$ROOT_DIR/$build_dir"

        "$CMAKE_BIN" -DCMAKE_BUILD_TYPE=Release \
                     -DCMAKE_CXX_COMPILER="$GPU_CXX" \
                     -DMPI_CXX_COMPILER=mpiicpx \
                     -DTARGET_GPU=nvidia \
                     -DSFC_TYPE=PEANO_HILBERT \
                     -D"$coords_rep" \
                     -D"$coords_type" \
                     -D"$pos_prec" \
                     -DPERIODIC_BC="$periodic" \
                     -DBENCHMARK=ON \
                     "$ROOT_DIR"

        make -j16 "${BENCHMARK_EXES[@]}"

        # Copy config.txt to binary directory if needed
        cp "$SCRIPT_DIR/config.txt" ./config.txt

        RESULT_FILE="$OUTPUT_DIR/scaling_${mode_name}_periodic_${periodic,,}.md"
        echo "# Scaling Benchmark Results: $mode_name (Periodic BC: $periodic)" > "$RESULT_FILE"
        echo "Date: $(date)" >> "$RESULT_FILE"
        echo "Device: GPU" >> "$RESULT_FILE"
        echo "Target GPU: nvidia" >> "$RESULT_FILE"
        echo "SFC Curve: Peano-Hilbert" >> "$RESULT_FILE"
        echo "Periodic BC: $periodic" >> "$RESULT_FILE"
        echo "Build Directory: $build_dir" >> "$RESULT_FILE"
        echo '```' >> "$RESULT_FILE"

        for exe in "${BENCHMARK_EXES[@]}"; do
            if [ -f "./$exe" ]; then
                echo "--- Executing ./$exe ---" >> "$RESULT_FILE"
                ./"$exe" >> "$RESULT_FILE" 2>&1 || true
                echo "" >> "$RESULT_FILE"
            fi
        done

        echo '```' >> "$RESULT_FILE"
        echo "Completed $mode_name (Periodic: $periodic). Saved to $RESULT_FILE"
    done
done

echo "=========================================================================="
echo "ALL GPU SCALING BENCHMARKS COMPLETED!"
echo "Results saved in $OUTPUT_DIR"
echo "=========================================================================="
