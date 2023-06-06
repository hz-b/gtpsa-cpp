cmake_minimum_required(VERSION 3.0.0)

project("gtpsa" LANGUAGES C CXX)
set(GTPSA_VERSION 0.0.1)

OPTION (USE_OpenMP "Use OpenMP" ON)
# Normally used during development.
# For release and installing the python module it is
# better to use the provided python setup files to
# achieve it
OPTION (USE_pybind11 "Use cmake to build the pybind11 extension" ON)
# Install the python package to the found python interpreter
# convienient for debug builds. Only useful if the pybind11 build is
# also used
option(PYTHON_MODULE_DEVELOPMENT_INSTALL "Install python binary module to source directory" ON)

find_package(BLAS)
find_package(LAPACK)
find_package(Armadillo REQUIRED)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RELEASE) # default to release build
  message(STATUS "Default to Release build")
endif()


IF(USE_OpenMP)
    FIND_PACKAGE(OpenMP)
    IF(OPENMP_FOUND)
        message(STATUS "Open MP enabled using flags CFLAGS=${OpenMP_C_FLAGS} CXXFLAGS=${OpenMP_CXX_FLAGS}")
        SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS} ")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} ")
    ENDIF()
ENDIF()

# specify the C standard
set(CMAKE_C_STANDARD 11)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()

if(CMAKE_COMPILER_IS_GNUCXX)
#   add_definitions(
#     -O3 -ftree-vectorize -fPIC -flto -march=native
#     -fno-math-errno -fno-builtin-cproj -ffast-math
#     -fno-cx-limited-range
#     -fassociative-math -freciprocal-math -ffinite-math-only
#     -fexpensive-optimizations
#     )
# add_definitions(
 endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

add_subdirectory(c)

# Required info in the directories below
# need to build proper package support
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/c++)
add_subdirectory(c++)
IF(USE_pybind11)
  # used when building the pybind11 module
  # in the tree
  if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(gtpsa_DEBUG_BUILD ON)
  else()
    set(gtpsa_DEBUG_BUILD OFF)
  endif()
  add_subdirectory(python)
endif()
add_subdirectory(examples)
add_subdirectory(tests)
