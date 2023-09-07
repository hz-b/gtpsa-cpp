# a dedicated preprocessor flag for gtpsa
# currently used for some writing some warnings
# at compile time and enabling some
# debug message prints
#
# current use desc.hpp
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Dgtpsa_DEBUG")


set(gtpsa_c++_mad_HEADERS
  gtpsa/mad/forward_decl.hpp
  gtpsa/mad/wrapper.tpp
  gtpsa/mad/utils.hpp
  gtpsa/mad/tpsa_wrapper.hpp
  gtpsa/mad/ctpsa_wrapper.hpp
  gtpsa/mad/container_wrapper.tpp
  )

set(gtpsa_c++_bridge_HEADERS
  gtpsa/bridge/bridge.hpp
  gtpsa/bridge/container.hpp
)

set(gtpsa_c++_intern_HEADERS
  gtpsa/intern/complex_utils.hpp
  gtpsa/intern/gtpsa_container.hpp
  gtpsa/intern/templated_funcs.hpp
  gtpsa/intern/with_operators_funcs.hpp
  gtpsa/intern/with_operators.hpp
  )

set(gtpsa_c++_python_HEADERS
  gtpsa/python/name_index.h
  gtpsa/python/objects_with_named_index.h
  )

set(gtpsa_c++_HEADERS
  gtpsa/funcs.h

  gtpsa/forward_decl.hpp
  gtpsa/utils.hpp
  gtpsa/desc.hpp
  gtpsa/tpsa.hpp
  gtpsa/ctpsa.hpp

  gtpsa/variant_forward_decl.hpp
  gtpsa/gtpsa_base_variant.hpp
  gtpsa/tpsa_double_variant.hpp
  gtpsa/ctpsa_complex_variant.hpp

  gtpsa/ss_vect.h

  gtpsa/utils_tps.hpp

  gtpsa/lielib.hpp
  )

install(FILES ${gtpsa_c++_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gtpsa
  )

install(FILES ${gtpsa_c++_mad_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gtpsa/mad/
  )
install(FILES ${gtpsa_c++_bridge_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gtpsa/bridge/
  )
install(FILES ${gtpsa_c++_intern_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gtpsa/intern/
  )
install(FILES ${gtpsa_c++_python_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gtpsa/python/
  )


set(gtpsa_c++_FILES
  gtpsa/mad/tpsa_wrapper.cc
  gtpsa/mad/ctpsa_wrapper.cc

  gtpsa/python/name_index.cc

  gtpsa/desc.cc
  gtpsa/tpsa.cc
  gtpsa/ctpsa.cc
  gtpsa/ss_vect.cc

  gtpsa/lielib.cc
  )

add_library(gtpsa-c++ SHARED
  ${gtpsa_c++_FILES}
  ${gtpsa_c++_HEADERS}
  ${gtpsa_c++_mad_HEADERS}
  ${gtpsa_c++_bridge_HEADERS}
  ${gtpsa_c++_intern_HEADERS}
  ${gtpsa_c++_python_HEADERS}
  )
add_library(gtpsa::gtpsacpp ALIAS gtpsa-c++)

target_link_libraries(gtpsa-c++ gtpsa lapack blas ${OpenMP_C_LIBRARIES} ${ARMADILLO_LIBRARIES})

target_include_directories(gtpsa-c++ PUBLIC
  #$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
  $<INSTALL_INTERFACE:include/>  # <prefix>/include/mylib
  ${ARMADILLO_INCLUDE_DIRS}
  )


install(TARGETS gtpsa-c++
        EXPORT gtpsacppTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    EXPORT gtpsacppTargets
    FILE gtpsacppTargets.cmake
    DESTINATION ${gtpsa_CMAKE_LIB_DIR}
    NAMESPACE gtpsa::
)
