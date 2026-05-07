cmake_minimum_required(VERSION 4.0)
project(fasttree)
enable_language(C)
enable_language(CXX)

#
# Set default build mode to Release
#
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build (e.g. Debug, Release)" FORCE)
endif(NOT CMAKE_BUILD_TYPE)


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

# set C++ standard to C++ 23, find SYCL, MPI
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#
# Find MPI libraries
#
find_package(MPI REQUIRED COMPONENTS C CXX)
include_directories(${MPI_C_INCLUDE_DIRS} ${MPI_CXX_INCLUDE_DIRS})

#
# Find HDF5 libraries
#
find_package(HDF5 COMPONENTS C HL REQUIRED)
include_directories(${HDF5_C_INCLUDE_DIRS})

#
# Find SYCL libraries
#
find_package(IntelSYCL REQUIRED)

# Add source files
# header files should be detected automatically
set(FASTTREE_SRC
    src/main.cpp
)

# set compiler flags for SYCL
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${INTELSYCL_CXX_FLAGS}")

# Create executable
add_executable(fasttree ${FASTTREE_SRC})
target_compile_features(fasttree PRIVATE cxx_std_23)

target_include_directories(fasttree PRIVATE src/include src src/domain)
target_compile_options(fasttree PRIVATE -O3 -march=skylake -mavx2)

if(CMAKE_CXX_COMPILER_ID MATCHES "Intel" OR CMAKE_CXX_COMPILER_ID MATCHES "IntelLLVM")
    target_link_options(fasttree PRIVATE -static-intel)
    message(STATUS "Intel compiler detected: using -static-intel for Intel runtime linkage")
else()
    message(STATUS "Non-Intel compiler detected: skipping -static-intel, compiled binary might be slow due to missing non-intel runtime optimizations")
endif()

target_link_libraries(fasttree ${MPI_CXX_LIBRARIES} ${HDF5_C_LIBRARIES} ${HDF5_HL_LIBRARIES} ${INTELSYCL_LIBRARIES})
