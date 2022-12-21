set(gtpsa_c++_mad_HEADERS
  gtpsa/mad/wrapper.tpp
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

set(gtpsa_c++_HEADERS
  gtpsa/funcs.h

  gtpsa/forward_decl.hpp
  gtpsa/utils.hpp
  gtpsa/desc.hpp
  gtpsa/tpsa.hpp
  gtpsa/ctpsa.hpp

  gtpsa/ss_vect.h

  gtpsa/utils_tps.hpp
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


set(gtpsa_c++_FILES
  gtpsa/mad/tpsa_wrapper.cc
  gtpsa/mad/ctpsa_wrapper.cc

  gtpsa/tpsa.cc
  gtpsa/ctpsa.cc
  gtpsa/ss_vect.cc
  )

add_library(gtpsa-c++ SHARED
  ${gtpsa_c++_FILES}
  ${gtpsa_c++_HEADERS}
  ${gtpsa_c++_mad_HEADERS}
  ${gtpsa_c++_bridge_HEADERS}
  ${gtpsa_c++_intern_HEADERS}
  )

target_link_libraries(gtpsa-c++ gtpsa lapack blas ${OpenMP_C_LIBRARIES} ${ARMADILLO_LIBRARIES})

target_include_directories(gtpsa-c++ PUBLIC
  #$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
  $<INSTALL_INTERFACE:include/>  # <prefix>/include/mylib
  ${ARMADILLO_INCLUDE_DIRS}
  )

set(GTPSA_C++_CMAKE_LIB_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/")

install(TARGETS gtpsa-c++
        EXPORT gtpsa-c++Targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    EXPORT gtpsa-c++Targets
    FILE gtpsa-c++Targets.cmake
    DESTINATION ${GTPSA_C++_CMAKE_LIB_DIR}
    NAMESPACE gtpsa::
)

configure_package_config_file(
		${CMAKE_CURRENT_SOURCE_DIR}/cmake/gtpsa-c++Config.cmake.in
		"${CMAKE_CURRENT_BINARY_DIR}/gtpsa-c++Config.cmake"
		INSTALL_DESTINATION ${GTPSA_C++_CMAKE_LIB_DIR}
)

write_basic_package_version_file(
		${CMAKE_CURRENT_BINARY_DIR}/gtpsa-c++ConfigVersion.cmake
		VERSION "${GTPSA_VERSION}"
		COMPATIBILITY SameMajorVersion
)

install(FILES
		"${CMAKE_CURRENT_BINARY_DIR}/gtpsa-c++Config.cmake"
		"${CMAKE_CURRENT_BINARY_DIR}/gtpsa-c++ConfigVersion.cmake"
		DESTINATION ${GTPSA_C++_CMAKE_LIB_DIR}
		)

