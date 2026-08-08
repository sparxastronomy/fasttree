#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/../.." && pwd)"
CMAKE_BIN="/mpcdf/soft/SLE_15/packages/x86_64/cmake/4.2/bin/cmake"

DEVICE_MODE="${1:-cpu}" # Options: cpu or gpu

OUTPUT_DIR="$ROOT_DIR/docs/benchmark_results"
mkdir -p "$OUTPUT_DIR"

BENCHMARK_EXES=(
    "self_knn_query_scaling.exe"
)

if [ "$DEVICE_MODE" = "gpu" ]; then
    unset ONEAPI_DEVICE_SELECTOR || true
    unset SYCL_DEVICE_FILTER || true
    GPU_CXX="${GPU_CXX:-/vera/ptmp/gc/bipra/softwares/illvm/build/install/bin/clang++}"
    if [ ! -x "$GPU_CXX" ]; then
        GPU_CXX="clang++"
    fi
    CONFIGS=(
        "build_gpu|gpu_double|COORDS_REPRESENTATION=FLOAT|COORDS_TYPE=DOUBLE|POSITIONS_PRECISION=32"
        "build_int32|gpu_int32|COORDS_REPRESENTATION=INTEGER|COORDS_TYPE=FLOAT|POSITIONS_PRECISION=32"
        "build_int64|gpu_int64|COORDS_REPRESENTATION=INTEGER|COORDS_TYPE=FLOAT|POSITIONS_PRECISION=64"
    )
else
    unset ONEAPI_DEVICE_SELECTOR || true
    unset SYCL_DEVICE_FILTER || true
    export OMP_NUM_THREADS=72
    CONFIGS=(
        "build_cpu|cpu_double|COORDS_REPRESENTATION=FLOAT|COORDS_TYPE=DOUBLE|POSITIONS_PRECISION=32"
        "build_int32|cpu_int32|COORDS_REPRESENTATION=INTEGER|COORDS_TYPE=FLOAT|POSITIONS_PRECISION=32"
        "build_int64|cpu_int64|COORDS_REPRESENTATION=INTEGER|COORDS_TYPE=FLOAT|POSITIONS_PRECISION=64"
    )
fi

PERIODIC_MODES=("OFF" "ON")

for config in "${CONFIGS[@]}"; do
    IFS="|" read -r build_dir mode_name coords_rep coords_type pos_prec <<< "$config"

    for periodic in "${PERIODIC_MODES[@]}"; do
        echo "=========================================================================="
        echo "Running Self-kNN Benchmark Config ($DEVICE_MODE): Mode=$mode_name, Periodic=$periodic"
        echo "Build Dir: $ROOT_DIR/$build_dir"
        echo "=========================================================================="

        mkdir -p "$ROOT_DIR/$build_dir"
        cd "$ROOT_DIR/$build_dir"

        rm -rf CMakeFiles CMakeCache.txt cmake_install.cmake

        if [ "$DEVICE_MODE" = "gpu" ]; then
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
        else
            "$CMAKE_BIN" -DCMAKE_BUILD_TYPE=Release \
                         -DCMAKE_CXX_COMPILER=icpx \
                         -DMPI_CXX_COMPILER=mpiicpx \
                         -DTARGET_GPU= \
                         -DSFC_TYPE=PEANO_HILBERT \
                         -D"$coords_rep" \
                         -D"$coords_type" \
                         -D"$pos_prec" \
                         -DPERIODIC_BC="$periodic" \
                         -DBENCHMARK=ON \
                         "$ROOT_DIR"
        fi

        make -j16 "${BENCHMARK_EXES[@]}"

        cp "$SCRIPT_DIR/config.txt" ./config.txt

        RESULT_FILE="$OUTPUT_DIR/scaling_${mode_name}_self_knn_periodic_${periodic,,}.md"
        echo "# Scaling Benchmark Results: $mode_name Self-kNN (Periodic BC: $periodic)" > "$RESULT_FILE"
        echo "Date: $(date)" >> "$RESULT_FILE"
        echo "Device: ${DEVICE_MODE^^}" >> "$RESULT_FILE"
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
        echo "Completed $mode_name Self-kNN (Periodic: $periodic). Saved to $RESULT_FILE"
    done
done

echo "=========================================================================="
echo "ALL SELF-KNN SCALING BENCHMARKS COMPLETED!"
echo "Results saved in $OUTPUT_DIR"
echo "=========================================================================="
