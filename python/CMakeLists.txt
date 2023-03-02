find_package(pybind11 2.9 REQUIRED)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

cmake_policy(SET CMP0063 NEW)

add_library(_gtpsa MODULE
  src/gtpsa_module.cc
  src/desc.cc
  src/named_index.cc
  src/gtpsa.cc
  src/ss_vect.cc
  src/arma.cc
  src/gtpsa_variant.cc
  )

target_link_libraries(_gtpsa
  pybind11::module
  pybind11::lto
  pybind11::windows_extras
  gtpsa-c++
  gtpsa
  #
  )

pybind11_extension(_gtpsa)
pybind11_strip(_gtpsa)
set_target_properties(_gtpsa
  PROPERTIES CXX_VISIBILITY_PRESET "hidden"
  CUDA_VISIBILITY_PRESET "hidden"
  )

add_library(_gtpsa_variant_test MODULE
  src/gtpsa_variant_test_module.cc
  src/gtpsa_variant.cc
  )

target_link_libraries(_gtpsa_variant_test
  pybind11::module
  pybind11::lto
  pybind11::windows_extras
  gtpsa-c++
  gtpsa
  #
  )

pybind11_extension(_gtpsa_variant_test)
pybind11_strip(_gtpsa_variant_test)
set_target_properties(_gtpsa_variant_test
  PROPERTIES CXX_VISIBILITY_PRESET "hidden"
  CUDA_VISIBILITY_PRESET "hidden"
  )


# For development ... let's copy the built shared objects to thor_scsi
# For final installation: use setup.py
if(PYTHON_MODULE_DEVELOPMENT_INSTALL)
  message(STATUS "copying python dynamic built libraries to source dir ${CMAKE_CURRENT_SOURCE_DIR}")
  install(TARGETS _gtpsa DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/gtpsa)
  install(TARGETS _gtpsa_variant_test DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/gtpsa)
endif()

# detect virtualenv and set Pip args accordingly
if(DEFINED ENV{VIRTUAL_ENV} OR DEFINED ENV{CONDA_PREFIX})
  set(_pip_args)
else()
  set(_pip_args "--user")
endif()


if(PYTHON_MODULE_INSTALL)
    # not proper code (nor tested)
    # execute_process(COMMAND ${Python3_EXECUTABLE} -m pip install ${_pip_args} ${_pypkg_name})
endif()
if(PYTHON_MODULE_DEVELOPMENT_INSTALL)
    # not proper code (nor tested)
    # install Python package locally in development mode
    #  execute_process(COMMAND ${Python3_EXECUTABLE} -m pip install ${_pip_args} -e )
endif()
