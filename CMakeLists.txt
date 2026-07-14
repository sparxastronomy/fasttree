cmake_minimum_required(VERSION 4.0)
project(fasttree)
enable_language(CXX)

#
# Set default build mode to Release
#
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build (e.g. Debug, Release)" FORCE)
endif(NOT CMAKE_BUILD_TYPE)


#
# Detect the compiler
# If CMAKE_<CXX/C>_COMPILER, MPI_<CXX/C>_COMPILER is set then use them
# else try finding the compiler in the following order: icpx, acpp
#
if((NOT CMAKE_CXX_COMPILER) OR (NOT MPI_CXX_COMPILER))
    find_program(CMAKE_CXX_COMPILER NAMES icpx acpp g++)
    find_program(MPI_CXX_COMPILER NAMES mpicxx mpiicpc)
endif()


#
# Store library paths in executables
#
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Put executables directly in build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

message(STATUS "C++ compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ compiler version: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C++ compiler path: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMake generator: ${CMAKE_GENERATOR}")

#
# Force clang to use the newer GCC 15 toolchain instead of the default system GCC 7
# Check which path is available on your system. For now this is set to the path either on Vera or my Mac
#
set(GCC_TOOLCHAIN_PATH "")
if(EXISTS "/mpcdf/soft/SLE_15/packages/x86_64/gcc/15.1.0")
    set(GCC_TOOLCHAIN_PATH "/mpcdf/soft/SLE_15/packages/x86_64/gcc/15.1.0")
    message(STATUS "Found GCC 15 toolchain at ${GCC_TOOLCHAIN_PATH}")
elseif(EXISTS "/Library/Developer/CommandLineTools/usr/bin")
    set(GCC_TOOLCHAIN_PATH "/Library/Developer/CommandLineTools/usr/bin")
    message(STATUS "Found Apple Clang toolchain at ${GCC_TOOLCHAIN_PATH}")
else()
    message(FATAL_ERROR "GCC 15 toolchain not found. Please install it or set the correct path.")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --gcc-toolchain=${GCC_TOOLCHAIN_PATH}")

# set C++ standard to C++ 23, find SYCL, MPI
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 
# Set compile time flags for SFC type and coordinate type
#
set(SFC_TYPE "MORTON" CACHE STRING "SFC type: MORTON or PEANO_HILBERT")
set(COORDS_TYPE "FLOAT" CACHE STRING "Coordinate type: FLOAT or DOUBLE")

if(SFC_TYPE STREQUAL "PEANO_HILBERT")
    add_compile_definitions(SFC_TYPE_PEANO_HILBERT)
    message(STATUS "Using Peano-Hilbert space-filling-curve")
else()
    add_compile_definitions(SFC_TYPE_MORTON)
    message(STATUS "Using Morton space-filing-curve")
endif()

if(COORDS_TYPE STREQUAL "DOUBLE")
    add_compile_definitions(COORDS_TYPE_DOUBLE)
    message(STATUS "Using double precision coordinates")
else()
    add_compile_definitions(COORDS_TYPE_FLOAT)
    message(STATUS "Using single precision coordinates")
endif()

set(DCOMPOSITION_TYPE "HISTOGRAM" CACHE STRING "Decomposition type: HISTOGRAM or SAMPLING")

if(DCOMPOSITION_TYPE STREQUAL "SAMPLING")
    add_compile_definitions(DCOMPOSITION_TYPE_SAMPLING)
    message(STATUS "Using sampling-based domain decomposition")
else()
    add_compile_definitions(DCOMPOSITION_TYPE_HISTOGRAM)
    message(STATUS "Using histogram-based domain decomposition")
endif()

#
# Find MPI libraries
#
find_package(MPI REQUIRED COMPONENTS C CXX)
include_directories(${MPI_C_INCLUDE_DIRS} ${MPI_CXX_INCLUDE_DIRS})

# Include oneDPL headers since the custom DPC++ compiler might not bundle them
include_directories(/u/bipra/ptmp/library/oneDPL/include)

#
# Find HDF5 libraries
#
# find_package(HDF5 COMPONENTS C HL REQUIRED)
# include_directories(${HDF5_C_INCLUDE_DIRS})

#
# Find SYCL libraries (AdaptiveCpp / DPC++)
#
find_package(IntelSYCL QUIET)
if(NOT COMMAND add_sycl_to_target)
    macro(add_sycl_to_target)
        # We manually add -fsycl flags below, so this can be a no-op if the legacy macro is absent
    endmacro()
endif()

# Add source files
set(FASTTREE_SRC
    test/main.cpp
)

# Create executable
add_executable(fasttree.exe ${FASTTREE_SRC})
target_compile_features(fasttree.exe PRIVATE cxx_std_23)

target_include_directories(fasttree.exe PRIVATE src)
target_compile_options(fasttree.exe PRIVATE -O3)

# Target GPU Option
set(TARGET_GPU "" CACHE STRING "Target GPU architecture: nvidia, amd, or empty for CPU")
set(GPU_SYCL_FLAGS "")
set(USER_FLAG "" CACHE STRING "User-defined flags for the compiler")
if(TARGET_GPU STREQUAL "nvidia")
    set(GPU_SYCL_FLAGS "-fsycl;-fsycl-targets=nvptx64-nvidia-cuda,native_cpu;-Xsycl-target-backend=nvptx64-nvidia-cuda;--cuda-gpu-arch=sm_80")
elseif(TARGET_GPU STREQUAL "amd")
    set(GPU_SYCL_FLAGS "-fsycl;-fsycl-targets=amdgcn-amd-amdhsa")
else()
    # Official Intel oneAPI Compiler (icpx) uses production-grade CPU runtime (OpenCL + TBB) via default -fsycl
    if(CMAKE_CXX_COMPILER_ID STREQUAL "IntelLLVM")
        set(GPU_SYCL_FLAGS "-fsycl")

    # AdaptiveCpp is SYCL-compatible by default; do not add -fsycl
    # any extra flags must be passed by the user in USER_FLAG
    elseif(CMAKE_CXX_COMPILER MATCHES "(^|.*/)?acpp$")
        set(GPU_SYCL_FLAGS "")
    
    # Custom LLVM compiler uses native_cpu for CPU runs
    else()
        set(GPU_SYCL_FLAGS "-fsycl;-fsycl-targets=native_cpu")
    endif()
endif()

# add user-defined flags to GPU_SYCL_FLAGS
if(USER_FLAG)
    list(APPEND GPU_SYCL_FLAGS ${USER_FLAG})
endif()

# find oneDPL library path and add its directory to GPU_SYCL_FLAGS
find_library(ONEDPL_LIB NAMES onedpl HINTS /u/bipra/ptmp/library/onedpl/lib NO_DEFAULT_PATH)
if(NOT ONEDPL_LIB)
    find_library(ONEDPL_LIB NAMES onedpl HINTS /opt/homebrew/Cellar/onedpl/2022.13.0/lib NO_DEFAULT_PATH)
endif()

if(ONEDPL_LIB)
    get_filename_component(ONEDPL_LIB_DIR ${ONEDPL_LIB} DIRECTORY)
    set(ONEDPL_INC_DIR ${ONEDPL_LIB_DIR}/../include)
    message(STATUS "Found oneDPL library at ${ONEDPL_LIB}")
    list(APPEND GPU_SYCL_FLAGS "-L${ONEDPL_LIB_DIR}")
elseif(EXISTS "/u/bipra/ptmp/library/oneDPL/include")
    set(ONEDPL_INC_DIR "/u/bipra/ptmp/library/oneDPL/include")
    message(STATUS "Found oneDPL headers at ${ONEDPL_INC_DIR}")
elseif(EXISTS "/opt/homebrew/include")
    set(ONEDPL_INC_DIR "/opt/homebrew/include")
    message(STATUS "Using oneDPL headers from ${ONEDPL_INC_DIR}")
else()
    message(WARNING "oneDPL library not found; continuing without explicit oneDPL link path")
endif()

if(ONEDPL_INC_DIR)
    include_directories(${ONEDPL_INC_DIR})
endif()


# We apply these flags to our targets manually to prevent breaking third-party dependencies like benchmark
add_sycl_to_target(TARGET fasttree.exe SOURCES ${FASTTREE_SRC})

target_link_libraries(fasttree.exe PRIVATE ${MPI_CXX_LIBRARIES} ${HDF5_C_LIBRARIES} ${HDF5_HL_LIBRARIES})
if(GPU_SYCL_FLAGS)
    target_compile_options(fasttree.exe PRIVATE ${GPU_SYCL_FLAGS})
    target_link_options(fasttree.exe PRIVATE ${GPU_SYCL_FLAGS})
endif()

# MPI Test Executable
add_executable(test_mpi.exe test/test_mpi.cpp)
target_compile_features(test_mpi.exe PRIVATE cxx_std_23)
target_include_directories(test_mpi.exe PRIVATE src)
target_compile_options(test_mpi.exe PRIVATE -O3)
add_sycl_to_target(TARGET test_mpi.exe SOURCES test/test_mpi.cpp)
target_link_libraries(test_mpi.exe PRIVATE ${MPI_CXX_LIBRARIES} ${HDF5_C_LIBRARIES} ${HDF5_HL_LIBRARIES})
if(GPU_SYCL_FLAGS)
    target_compile_options(test_mpi.exe PRIVATE ${GPU_SYCL_FLAGS})
    target_link_options(test_mpi.exe PRIVATE ${GPU_SYCL_FLAGS})
endif()

# Advanced MPI Halo-Exchange Test Executable
add_executable(advance_mpi.exe test/advance_mpi.cpp)
target_compile_features(advance_mpi.exe PRIVATE cxx_std_23)
target_include_directories(advance_mpi.exe PRIVATE src)
target_compile_options(advance_mpi.exe PRIVATE -O3)
add_sycl_to_target(TARGET advance_mpi.exe SOURCES test/advance_mpi.cpp)
target_link_libraries(advance_mpi.exe PRIVATE ${MPI_CXX_LIBRARIES} ${HDF5_C_LIBRARIES} ${HDF5_HL_LIBRARIES})
if(GPU_SYCL_FLAGS)
    target_compile_options(advance_mpi.exe PRIVATE ${GPU_SYCL_FLAGS})
    target_link_options(advance_mpi.exe PRIVATE ${GPU_SYCL_FLAGS})
endif()

# Distributed Domain Decomposition Test Executable
add_executable(test_domain_decomposition.exe test/test_domain_decomposition.cpp)
target_compile_features(test_domain_decomposition.exe PRIVATE cxx_std_23)
target_include_directories(test_domain_decomposition.exe PRIVATE src)
target_compile_options(test_domain_decomposition.exe PRIVATE -O3)
add_sycl_to_target(TARGET test_domain_decomposition.exe SOURCES test/test_domain_decomposition.cpp)
target_link_libraries(test_domain_decomposition.exe PRIVATE ${MPI_CXX_LIBRARIES} ${HDF5_C_LIBRARIES} ${HDF5_HL_LIBRARIES})
if(GPU_SYCL_FLAGS)
    target_compile_options(test_domain_decomposition.exe PRIVATE ${GPU_SYCL_FLAGS})
    target_link_options(test_domain_decomposition.exe PRIVATE ${GPU_SYCL_FLAGS})
endif()

option(BENCHMARK "Enable benchmarks" OFF)
if(BENCHMARK)
    add_subdirectory(test/benchmark)
endif()