# current hack .. until the library is properly defined

add_executable(gtpsa_ex0 gtpsa_ex0.cc)
add_executable(gtpsa_ex1 gtpsa_ex1.cc)
add_executable(gtpsa_ex2 gtpsa_ex2.cc)
add_executable(gtpsa_ex3 gtpsa_ex3.cc)
add_executable(gtpsa_ex4 gtpsa_ex4.cc)

target_link_libraries(gtpsa_ex0 gtpsa lapack blas)
target_link_libraries(gtpsa_ex1 gtpsa lapack blas)
target_link_libraries(gtpsa_ex2 gtpsa lapack blas)
target_link_libraries(gtpsa_ex3 gtpsa lapack blas)
target_link_libraries(gtpsa_ex4 gtpsa lapack blas)

add_executable(space_state space_state.cc)
target_link_libraries(space_state gtpsa lapack blas)

add_executable(set_identity set_identity.cc)
target_link_libraries(set_identity gtpsa lapack blas)
