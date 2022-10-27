find_package(pybind11 2.9 REQUIRED)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

cmake_policy(SET CMP0063 NEW)
add_library(lib_gtpsa MODULE
  src/gtpsa_module.cc
  src/desc.cc
  src/gtpsa.cc
  src/ss_vect.cc
  )

target_link_libraries(lib_gtpsa
  pybind11::module
  pybind11::lto
  pybind11::windows_extras
  gtpsa
  #
  )

pybind11_extension(lib_gtpsa)
pybind11_strip(lib_gtpsa)
set_target_properties(lib_gtpsa
  PROPERTIES CXX_VISIBILITY_PRESET "hidden"
  CUDA_VISIBILITY_PRESET "hidden"
  )
