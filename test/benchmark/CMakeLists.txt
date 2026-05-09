cmake_minimum_required(VERSION 4.0)
project(fasttree_benchmarks)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(IntelSYCL REQUIRED)
find_package(MPI REQUIRED)
# HDF5 is already found in the parent CMakeLists.txt or we can find it here too
find_package(HDF5 COMPONENTS C HL REQUIRED)

include_directories(../../src)
include_directories(${HDF5_INCLUDE_DIRS})
include_directories(${MPI_CXX_INCLUDE_DIRS})

set(BENCHMARK_APPS
    morton_scaling
    radix_sort_scaling
    intra_voxel_sort_scaling
    tree_build_scaling
    range_query_scaling
    knn_query_scaling
)

foreach(app ${BENCHMARK_APPS})
    set(exe_name ${app}.exe)
    add_executable(${exe_name} ${app}.cpp)
    target_include_directories(${exe_name} PRIVATE ../../src .)
    target_compile_options(${exe_name} PRIVATE -O3)
    add_sycl_to_target(TARGET ${exe_name} SOURCES ${app}.cpp)
    target_link_libraries(${exe_name} PRIVATE ${HDF5_C_LIBRARIES} ${HDF5_HL_LIBRARIES} ${MPI_CXX_LIBRARIES})
endforeach()
