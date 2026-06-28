#!/usr/bin/env bash
# profile_cpu.sh
# Script to run CPU profiling for FastTree benchmarks using Intel VTune and Intel APS.

set -euo pipefail

# Print usage instructions
usage() {
    echo "Usage: $0 [options] <executable>"
    echo "Options:"
    echo "  -n, --ranks <num>     Number of MPI ranks (default: 1)"
    echo "  -t, --type <type>     Profiling type: hotspots | memory | threading | aps (default: hotspots)"
    echo "  -f, --filter <pat>    Filter dataset/benchmark (e.g. 10M) (default: run all)"
    echo "  -c, --config <file>   Path to config file (default: config.txt)"
    echo "  -h, --help            Show this help message"
    echo ""
    echo "Supported executables (look in build_cpu/):"
    echo "  domain_decomposition_scaling.exe"
    echo "  tree_build_scaling.exe"
    echo "  range_query_scaling.exe"
    echo "  knn_query_scaling.exe"
    echo "  sfc_encoding_scaling.exe"
    echo ""
    echo "Example:"
    echo "  $0 -n 8 -t aps -f 10M domain_decomposition_scaling.exe"
    echo "  $0 -t memory -f 10M tree_build_scaling.exe"
    exit 1
}

RANKS=1
TYPE="hotspots"
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
BUILD_DIR="$ROOT_DIR/build_cpu"
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
    # Fallback to current dir if run directly inside build_cpu
    if [ -f "./$EXE" ]; then
        EXE_PATH="./$EXE"
    else
        echo "Error: Executable $EXE not found in $BUILD_DIR."
        echo "Please build the project first in build_cpu directory."
        exit 1
    fi
fi

mkdir -p "$PROFILE_DIR"

# Check dependencies
check_tool() {
    if ! command -v "$1" &> /dev/null; then
        echo "Warning: Command '$1' could not be found."
        echo "Please make sure to load the required module. For example:"
        echo "  module load intel-oneapi"
        echo "  module load vtune"
        echo "  module load intel-aps"
        return 1
    fi
    return 0
}

# Set thread affinity and environment variables
export OMP_PLACES=cores
export OMP_PROC_BIND=close

if [ "$RANKS" -gt 1 ]; then
    export OMP_NUM_THREADS=36
    export I_MPI_PIN_DOMAIN=socket
else
    export OMP_NUM_THREADS=72
fi

TIMESTAMP=$(date +%Y%m%d_%H%M%S)

# Perform profiling based on the type
case "$TYPE" in
    aps)
        check_tool "aps" || exit 1
        echo "=== Running Intel Application Performance Snapshot (APS) ==="
        echo "Ranks:        $RANKS"
        echo "Executable:   $EXE_PATH"
        
        RESULT_DIR="$PROFILE_DIR/aps_result_${EXE}_${RANKS}ranks_${TIMESTAMP}"
        
        if [ "$RANKS" -gt 1 ]; then
            # APS in MPI mode
            aps mpirun -n "$RANKS" "$EXE_PATH" "$CONFIG_FILE" $EXTRA_ARGS
        else
            # Single node APS
            aps "$EXE_PATH" "$CONFIG_FILE" $EXTRA_ARGS
        fi
        
        echo ""
        echo "APS run completed. Generating report..."
        # Find the latest aps_result directory if it generated a default named one
        LATEST_DIR=$(ls -td aps_result_* 2>/dev/null | head -n 1 || true)
        if [ -n "$LATEST_DIR" ]; then
            mv "$LATEST_DIR" "$RESULT_DIR"
        else
            RESULT_DIR=$(ls -td "$PROFILE_DIR"/aps_result_* 2>/dev/null | head -n 1 || echo "$RESULT_DIR")
        fi
        
        aps-report -h "$RESULT_DIR"
        echo "==========================================================="
        echo "Report generated at: ${RESULT_DIR}/aps_report.html"
        echo "==========================================================="
        ;;

    hotspots|memory|threading)
        check_tool "vtune" || exit 1
        echo "=== Running Intel VTune Profiler ($TYPE) ==="
        echo "Ranks:        $RANKS"
        echo "Executable:   $EXE_PATH"
        
        RESULT_DIR="$PROFILE_DIR/vtune_${TYPE}_${EXE}_${RANKS}ranks_${TIMESTAMP}"
        
        COLLECT_FLAG=""
        KNOBS=""
        case "$TYPE" in
            hotspots)
                COLLECT_FLAG="hotspots"
                KNOBS="-knob sampling-mode=hw"
                ;;
            memory)
                COLLECT_FLAG="memory-access"
                KNOBS="-knob analyze-mem-objects=true"
                ;;
            threading)
                COLLECT_FLAG="threading"
                ;;
        esac

        if [ "$RANKS" -gt 1 ]; then
            # For MPI runs, VTune result directory template %r gets replaced by MPI rank ID
            MPI_RESULT_DIR="${RESULT_DIR}_rank_%r"
            mpirun -n "$RANKS" vtune -collect "$COLLECT_FLAG" $KNOBS -result-dir="$MPI_RESULT_DIR" -- "$EXE_PATH" "$CONFIG_FILE" $EXTRA_ARGS
            
            echo ""
            echo "VTune MPI runs completed. Result directories created:"
            echo "  ${RESULT_DIR}_rank_0"
            echo "  ..."
        else
            # Single rank/node run
            vtune -collect "$COLLECT_FLAG" $KNOBS -result-dir="$RESULT_DIR" -- "$EXE_PATH" "$CONFIG_FILE" $EXTRA_ARGS
            echo ""
            echo "VTune run completed. Results saved in: $RESULT_DIR"
            echo "To open VTune GUI, run:"
            echo "  vtune-gui $RESULT_DIR &"
        fi
        ;;
    *)
        echo "Error: Unknown profiling type '$TYPE'."
        usage
        ;;
esac
