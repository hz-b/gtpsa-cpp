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
