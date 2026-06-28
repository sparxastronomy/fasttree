#!/usr/bin/env bash
# profile_gpu.sh
# Script to run GPU profiling for FastTree benchmarks using NVIDIA Nsight Systems and Nsight Compute.

set -euo pipefail

# Print usage instructions
usage() {
    echo "Usage: $0 [options] <executable>"
    echo "Options:"
    echo "  -n, --ranks <num>     Number of MPI ranks (default: 1)"
    echo "  -t, --type <type>     Profiling type: nsys | ncu (default: nsys)"
    echo "  -k, --kernel <name>   NVIDIA Nsight Compute kernel filter pattern (default: profile all)"
    echo "  -f, --filter <pat>    Filter dataset/benchmark (e.g. 10M) (default: run all)"
    echo "  -c, --config <file>   Path to config file (default: config.txt)"
    echo "  -h, --help            Show this help message"
    echo ""
    echo "Supported executables (look in build_gpu/):"
    echo "  domain_decomposition_scaling.exe"
    echo "  tree_build_scaling.exe"
    echo "  range_query_scaling.exe"
    echo "  knn_query_scaling.exe"
    echo "  sfc_encoding_scaling.exe"
    echo ""
    echo "Example:"
    echo "  $0 -n 8 -t nsys -f 10M domain_decomposition_scaling.exe"
    echo "  $0 -t ncu -k \"sfc_encode\" -f 10M tree_build_scaling.exe"
    exit 1
}

RANKS=1
TYPE="nsys"
KERNEL_FILTER=""
CONFIG_FILE="config.txt"
FILTER=""
EXE=""

# Parse command line options
while [[ $# -gt 0 ]]; do
    case "$1" in
        -n|--ranks)
            RANKS="$2"
            shift 2
            ;;
        -t|--type)
            TYPE="$2"
            shift 2
            ;;
        -k|--kernel)
            KERNEL_FILTER="$2"
            shift 2
            ;;
        -f|--filter)
            FILTER="$2"
            shift 2
            ;;
        -c|--config)
            CONFIG_FILE="$2"
            shift 2
            ;;
        -h|--help)
            usage
            ;;
        -*)
            echo "Error: Unknown option $1"
            usage
            ;;
        *)
            EXE="$1"
            shift
            ;;
    esac
done

if [ -z "$EXE" ]; then
    echo "Error: Missing executable name."
    usage
fi

# Locate directories
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/../.." && pwd)"
BUILD_DIR="$ROOT_DIR/build_gpu"
PROFILE_DIR="$ROOT_DIR/profile_data"

# Resolve CONFIG_FILE absolute path
if [ -f "$CONFIG_FILE" ]; then
    CONFIG_FILE="$(realpath "$CONFIG_FILE")"
elif [ -f "$SCRIPT_DIR/$CONFIG_FILE" ]; then
    CONFIG_FILE="$(realpath "$SCRIPT_DIR/$CONFIG_FILE")"
elif [ -f "$ROOT_DIR/$CONFIG_FILE" ]; then
    CONFIG_FILE="$(realpath "$ROOT_DIR/$CONFIG_FILE")"
else
    echo "Error: Config file '$CONFIG_FILE' not found."
    exit 1
fi

# Handle temporary config filtering to isolate target datasets (e.g. 10M)
CLEANUP_CONFIG=false
cleanup() {
    if [ "$CLEANUP_CONFIG" = true ]; then
        if [ -f "${CONFIG_FILE}.bak" ]; then
            mv "${CONFIG_FILE}.bak" "$CONFIG_FILE"
            echo "Restored original $CONFIG_FILE."
        fi
    fi
}
trap cleanup EXIT

if [ -n "$FILTER" ]; then
    FILTERED_LINES=$(grep -i "$FILTER" "$CONFIG_FILE" || true)
    if [ -z "$FILTERED_LINES" ]; then
        echo "Error: No lines matching filter '$FILTER' found in $CONFIG_FILE."
        exit 1
    fi
    mv "$CONFIG_FILE" "${CONFIG_FILE}.bak"
    echo "$FILTERED_LINES" > "$CONFIG_FILE"
    CLEANUP_CONFIG=true
    echo "Temporarily filtered $CONFIG_FILE to contain only matching '$FILTER' datasets."
fi

# Additional args for Google Benchmark executables
EXTRA_ARGS=""
if [ "$EXE" != "domain_decomposition_scaling.exe" ] && [ -n "$FILTER" ]; then
    EXTRA_ARGS="--benchmark_filter=$FILTER"
fi

# Verify executable existence
EXE_PATH="$BUILD_DIR/$EXE"
if [ ! -f "$EXE_PATH" ]; then
    # Fallback to current dir if run directly inside build_gpu
    if [ -f "./$EXE" ]; then
        EXE_PATH="./$EXE"
    else
        echo "Error: Executable $EXE not found in $BUILD_DIR."
        echo "Please build the project first in build_gpu directory."
        exit 1
    fi
fi

mkdir -p "$PROFILE_DIR"

# Check dependencies
check_tool() {
    if ! command -v "$1" &> /dev/null; then
        echo "Warning: Command '$1' could not be found."
        echo "Please make sure to load the required module/CUDA toolkit. For example:"
        echo "  module load cuda"
        echo "  module load nsight-systems"
        echo "  module load nsight-compute"
        return 1
    fi
    return 0
}

# Determine rank environment variable for file names under MPI
# Intel MPI uses PMI_RANK, OpenMPI uses OMPI_COMM_WORLD_RANK, Slurm uses SLURM_PROCID
RANK_VAR="OMPI_COMM_WORLD_RANK"
if [ -n "${SLURM_PROCID:-}" ]; then
    RANK_VAR="SLURM_PROCID"
elif [ -n "${PMI_RANK:-}" ]; then
    RANK_VAR="PMI_RANK"
fi

TIMESTAMP=$(date +%Y%m%d_%H%M%S)

# Perform profiling based on the type
case "$TYPE" in
    nsys)
        check_tool "nsys" || exit 1
        echo "=== Running NVIDIA Nsight Systems (nsys) ==="
        echo "Ranks:        $RANKS"
        echo "Executable:   $EXE_PATH"
        
        # We trace CUDA APIs, GPU kernels, NVTX annotations, OS runtime APIs, and MPI
        TRACE_FLAGS="cuda,nvtx,osrt"
        if [ "$RANKS" -gt 1 ]; then
            TRACE_FLAGS="mpi,cuda,nvtx,osrt"
        fi
        
        RESULT_BASE="$PROFILE_DIR/nsys_${EXE}_${RANKS}ranks_${TIMESTAMP}"
        
        # Ensure we target the NVIDIA GPU device
        export ONEAPI_DEVICE_SELECTOR="cuda:gpu"
        
        if [ "$RANKS" -gt 1 ]; then
            # Under MPI, we save separate trace files per rank using the %q environment substitution
            # nsys replaces %q{VAR} at runtime with the value of the environment variable VAR
            nsys_cmd="nsys profile -t $TRACE_FLAGS -o ${RESULT_BASE}_rank_%q{${RANK_VAR}} --force-overwrite true"
            mpirun -n "$RANKS" $nsys_cmd "$EXE_PATH" "$CONFIG_FILE" $EXTRA_ARGS
            echo ""
            echo "Nsight Systems MPI profiling finished. Reports generated in:"
            echo "  ${RESULT_BASE}_rank_0.nsys-rep"
            echo "  ..."
        else
            nsys profile -t $TRACE_FLAGS -o "$RESULT_BASE" --stats=true "$EXE_PATH" "$CONFIG_FILE" $EXTRA_ARGS
            echo ""
            echo "Nsight Systems profiling finished. Report saved at:"
            echo "  ${RESULT_BASE}.nsys-rep"
            echo "To open in UI, launch Nsight Systems GUI and import the report."
        fi
        ;;

    ncu)
        check_tool "ncu" || exit 1
        echo "=== Running NVIDIA Nsight Compute (ncu) ==="
        echo "Ranks:        $RANKS"
        echo "Executable:   $EXE_PATH"
        
        RESULT_BASE="$PROFILE_DIR/ncu_${EXE}_${RANKS}ranks_${TIMESTAMP}"
        
        # Detailed hardware metric selection: SpeedOfLight, MemoryWorkloadAnalysis, Occupancy
        METRIC_SET="--set detailed"
        
        FILTER_ARGS=""
        if [ -n "$KERNEL_FILTER" ]; then
            # Profile only specific kernels matching the name filter (reduces profiling overhead drastically)
            FILTER_ARGS="-k $KERNEL_FILTER"
            echo "Kernel filter: $KERNEL_FILTER"
        fi

        export ONEAPI_DEVICE_SELECTOR="cuda:gpu"
        
        if [ "$RANKS" -gt 1 ]; then
            # Profile Rank 0 specifically to avoid NCU interleaving overhead on all ranks,
            # or save separate reports. NCU uses %i index for process isolation.
            ncu_cmd="ncu $METRIC_SET $FILTER_ARGS -o ${RESULT_BASE}_rank_%i"
            mpirun -n "$RANKS" $ncu_cmd "$EXE_PATH" "$CONFIG_FILE" $EXTRA_ARGS
            echo ""
            echo "Nsight Compute MPI profiling finished. Reports generated in:"
            echo "  ${RESULT_BASE}_rank_0.ncu-rep"
            echo "  ..."
        else
            ncu $METRIC_SET $FILTER_ARGS -o "$RESULT_BASE" "$EXE_PATH" "$CONFIG_FILE" $EXTRA_ARGS
            echo ""
            echo "Nsight Compute profiling finished. Report saved at:"
            echo "  ${RESULT_BASE}.ncu-rep"
            echo "To open in UI, launch Nsight Compute GUI and open the report."
        fi
        ;;
    *)
        echo "Error: Unknown profiling type '$TYPE'."
        usage
        ;;
esac
