cmake_minimum_required(VERSION 3.0.0)

project("gtpsa" LANGUAGES C CXX)

find_package(BLAS)
find_package(LAPACK)
find_package(OpenMP)

IF(OPENMP_FOUND)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
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


add_subdirectory(c)
# add_subdirectory(c++)
# Required info in the directories below
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/c++)
add_subdirectory(examples)
add_subdirectory(tests)
