cmake_minimum_required(VERSION 4.0)
project(fasttree_benchmarks)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Google Benchmark Integration ---
set(HAVE_STD_REGEX ON CACHE BOOL "Force HAVE_STD_REGEX to true to bypass icpx try_compile bug" FORCE)
set(HAVE_POSIX_REGEX OFF CACHE BOOL "" FORCE)
set(HAVE_GNU_POSIX_REGEX OFF CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(
  benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG        v1.9.1
)
set(BENCHMARK_ENABLE_TESTS OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
set(BENCHMARK_DOWNLOAD_DEPENDENCIES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(benchmark)
# ------------------------------------

find_package(IntelSYCL QUIET)
if(NOT COMMAND add_sycl_to_target)
    macro(add_sycl_to_target)
    endmacro()
endif()
find_package(MPI REQUIRED)
find_package(HDF5 COMPONENTS C HL REQUIRED)

include_directories(../../src)
include_directories(${HDF5_INCLUDE_DIRS})
include_directories(${MPI_CXX_INCLUDE_DIRS})

set(BENCHMARK_APPS
    sfc_encoding_scaling
    gpu_sort_scaling
    tree_build_scaling
    rebuild_scaling
    range_query_scaling
    knn_query_scaling
    domain_decomposition_scaling
)

foreach(app ${BENCHMARK_APPS})
    set(exe_name ${app}.exe)
    add_executable(${exe_name} ${app}.cpp)
    target_include_directories(${exe_name} PRIVATE ../../src .)
    target_compile_options(${exe_name} PRIVATE -O3)
    add_sycl_to_target(TARGET ${exe_name} SOURCES ${app}.cpp)
    target_link_libraries(${exe_name} PRIVATE 
        benchmark::benchmark 
        ${HDF5_C_LIBRARIES} 
        ${HDF5_HL_LIBRARIES} 
        ${MPI_CXX_LIBRARIES}
    )
    if(GPU_SYCL_FLAGS)
        target_compile_options(${exe_name} PRIVATE ${GPU_SYCL_FLAGS})
        target_link_options(${exe_name} PRIVATE ${GPU_SYCL_FLAGS})
    endif()
endforeach()
