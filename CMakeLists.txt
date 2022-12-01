cmake_minimum_required(VERSION 3.0.0)

project("gtpsa" LANGUAGES C CXX)
set(GTPSA_VERSION 0.0.1)

OPTION (USE_OpenMP "Use OpenMP" ON)


find_package(BLAS)
find_package(LAPACK)
find_package(Armadillo REQUIRED)

IF(USE_OpenMP)
    FIND_PACKAGE(OpenMP)
    IF(OPENMP_FOUND)
        message(STATUS "Open MP enabled using flags CFLAGS=${OpenMP_C_FLAGS} CXXFLAGS=${OpenMP_CXX_FLAGS}")
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS} ")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} ")
    ENDIF()
ENDIF()

set(mad_ng_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/mad-ng/src/)

include_directories(
  ${mad_ng_SRC_DIR}/libgtpsa/
  ${mad_ng_SRC_DIR}
)

# specify the C standard
set(CMAKE_C_STANDARD 11)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(CMAKE_COMPILER_IS_GNUCXX)
   add_definitions(
     -O3 -ftree-vectorize -fPIC -flto -march=native
     -fno-math-errno -fno-builtin-cproj -ffast-math
     -fno-cx-limited-range
     -fassociative-math -freciprocal-math -ffinite-math-only
     -fexpensive-optimizations
     )
 endif()
add_subdirectory(c)
# Required info in the directories below
# need to build proper package support
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/c++)
add_subdirectory(c++)
add_subdirectory(python)
add_subdirectory(examples)
add_subdirectory(tests)
