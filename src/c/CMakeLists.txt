
add_compile_definitions(GTPSA_AS_LIB)
include(CMakePackageConfigHelpers)

set(mad_ng_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../external_repositories/mad-ng/src/)

set(gtpsa_HEADERS
  ${mad_ng_SRC_DIR}/mad_bit.h
  ${mad_ng_SRC_DIR}/mad_cst.h
  ${mad_ng_SRC_DIR}/mad_ctpsa.h
  # ${mad_ng_SRC_DIR}/mad_ctpsa_impl.h
  ${mad_ng_SRC_DIR}/mad_def.h
  ${mad_ng_SRC_DIR}/mad_desc.h
  # ${mad_ng_SRC_DIR}/mad_desc_impl.h
  ${mad_ng_SRC_DIR}/mad_erfw.h
  ${mad_ng_SRC_DIR}/mad_log.h
  ${mad_ng_SRC_DIR}/mad_mat.h
  ${mad_ng_SRC_DIR}/mad_mem.h
  ${mad_ng_SRC_DIR}/mad_mono.h
  ${mad_ng_SRC_DIR}/mad_num.h
  ${mad_ng_SRC_DIR}/mad_poly.h
  ${mad_ng_SRC_DIR}/mad_rad.h
  ${mad_ng_SRC_DIR}/mad_str.h
  ${mad_ng_SRC_DIR}/mad_tpsa.h
  # ${mad_ng_SRC_DIR}/mad_tpsa_impl.h
  ${mad_ng_SRC_DIR}/mad_vec.h
  ${mad_ng_SRC_DIR}/mad_ver.h
)

set(gtpsa_FILES
  ${mad_ng_SRC_DIR}/libgtpsa/mad_log.c
  ${mad_ng_SRC_DIR}/mad_bit.c
  ${mad_ng_SRC_DIR}/mad_cst.c
  ${mad_ng_SRC_DIR}/mad_ctpsa.c
  ${mad_ng_SRC_DIR}/mad_ctpsa_comp.c
  ${mad_ng_SRC_DIR}/mad_ctpsa_conv.c
  ${mad_ng_SRC_DIR}/mad_ctpsa_fun.c
  ${mad_ng_SRC_DIR}/mad_ctpsa_io.c
  ${mad_ng_SRC_DIR}/mad_ctpsa_minv.c
  ${mad_ng_SRC_DIR}/mad_ctpsa_mops.c
  ${mad_ng_SRC_DIR}/mad_ctpsa_ops.c
  ${mad_ng_SRC_DIR}/mad_desc.c
  ${mad_ng_SRC_DIR}/mad_erfw.c
  ${mad_ng_SRC_DIR}/mad_mat.c
  ${mad_ng_SRC_DIR}/mad_mem.c
  ${mad_ng_SRC_DIR}/mad_mono.c
  ${mad_ng_SRC_DIR}/mad_num.c
  ${mad_ng_SRC_DIR}/mad_poly.c
  ${mad_ng_SRC_DIR}/mad_rad.c
  ${mad_ng_SRC_DIR}/mad_str.c
  ${mad_ng_SRC_DIR}/mad_tpsa.c
  ${mad_ng_SRC_DIR}/mad_tpsa_comp.c
  ${mad_ng_SRC_DIR}/mad_tpsa_fun.c
  ${mad_ng_SRC_DIR}/mad_tpsa_io.c
  ${mad_ng_SRC_DIR}/mad_tpsa_minv.c
  ${mad_ng_SRC_DIR}/mad_tpsa_mops.c
  ${mad_ng_SRC_DIR}/mad_tpsa_ops.c
  ${mad_ng_SRC_DIR}/mad_vec.c

  # Fortran File implementing former MAD
  # micado version
  ${mad_ng_SRC_DIR}/madx_micado.f90
  )


add_library(gtpsa SHARED
  ${gtpsa_FILES}
  ${gtpsa_HEADERS}
)
add_library(gtpsa::c ALIAS gtpsa)
target_link_libraries(gtpsa lapack blas ${OpenMP_C_LIBRARIES})
set_target_properties(gtpsa
  PROPERTIES
  SOVERSION 1.0
  LIBRARY_OUTPUT_NAME gtpsa
)


#-- .cmake ? --
# should that not be in the .cmake file?

set(mad_ng_build_SRC_DIR ${mad_ng_SRC_DIR})
# need to check if required ...
# mad include dirs installed in include/
set(mad_ng_install_SRC_DIR ${CMAKE_INSTALL_INCLUDEDIR}../external_repositiories/mad-ng/src/)
#-- .cmake ? --

message(STATUS "mad_ng_build_SRC_DIR ${mad_ng_build_SRC_DIR}")


target_include_directories(gtpsa
                           PUBLIC
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
                           "$<BUILD_INTERFACE:${mad_ng_build_SRC_DIR}>"
                           "$<BUILD_INTERFACE:${mad_ng_build_SRC_DIR/gtpsa}>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)



install(TARGETS gtpsa
        EXPORT gtpsaTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	)

install(
    EXPORT gtpsaTargets
    FILE gtpsaTargets.cmake
    DESTINATION ${gtpsa_CMAKE_LIB_DIR}
    NAMESPACE gtpsa::
)


message(STATUS "cmake  ${CMAKE_INSTALL_INCLUDEDIR}")
install(FILES ${gtpsa_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
