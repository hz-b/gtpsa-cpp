set(gtpsa_c++_HEADERS
  gtpsa/common.tpp
  gtpsa/forward_decl.tpp
  gtpsa/mad_gtpsa_wrapper.tpp
  gtpsa/with_operators_funcs.tpp
  gtpsa/with_operators.tpp

  gtpsa/forward_decl.hpp
  gtpsa/desc.hpp
  gtpsa/tpsa.hpp
  gtpsa/ctpsa.hpp
  gtpsa/utils.hpp
  gtpsa/utils_tps.hpp

  gtpsa/funcs.h
  gtpsa/ss_vect.h

)
install(FILES ${gtpsa_c++_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gtpsa/)
#install(TARGETS gtpsa-c++
#        EXPORT gtpsaTargets
#        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#)

set(gtpsa_c++_FILES
  gtpsa/tpsa.cc
  gtpsa/ctpsa.cc
  gtpsa/ss_vect.cc
  )

add_library(gtpsa-c++ SHARED
  ${gtpsa_c++_FILES}
  ${gtpsa_c++_FILES}
  ${gtpsa_c++_HEADERS}
)
target_link_libraries(gtpsa-c++ gtpsa lapack blas ${OpenMP_C_LIBRARIES})

set(GTPSA_C++_CMAKE_LIB_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/")
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

install(FILES ${gtpsa_c++_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
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

install(FILES
         "${CMAKE_CURRENT_BINARY_DIR}/gtpsa-c++Config.cmake"
         "${CMAKE_CURRENT_BINARY_DIR}/gtpsa-c++ConfigVersion.cmake"
	 DESTINATION ${GTPSA_C++_CMAKE_LIB_DIR}
)
