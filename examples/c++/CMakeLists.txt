# current hack .. until the library is properly defined

add_executable(gtpsa_ex0 gtpsa_ex0.cc)
add_executable(gtpsa_ex1 gtpsa_ex1.cc)
add_executable(gtpsa_ex2 gtpsa_ex2.cc)
add_executable(gtpsa_ex3 gtpsa_ex3.cc)
add_executable(gtpsa_ex4 gtpsa_ex4.cc)
add_executable(gtpsa_ex5 gtpsa_ex5.cc)
add_executable(gtpsa_ex6 gtpsa_ex6.cc)

target_link_libraries(gtpsa_ex0 gtpsa-c++ gtpsa lapack blas)
target_link_libraries(gtpsa_ex1 gtpsa-c++ gtpsa lapack blas)
target_link_libraries(gtpsa_ex2 gtpsa-c++ gtpsa lapack blas)
target_link_libraries(gtpsa_ex3 gtpsa-c++ gtpsa lapack blas)
target_link_libraries(gtpsa_ex4 gtpsa-c++ gtpsa lapack blas)
target_link_libraries(gtpsa_ex5 gtpsa-c++ gtpsa lapack blas)
target_link_libraries(gtpsa_ex6 gtpsa-c++ gtpsa lapack blas)

add_executable(space_state space_state.cc)
target_link_libraries(space_state gtpsa-c++ gtpsa lapack blas)

add_executable(set_identity set_identity.cc)
target_link_libraries(set_identity gtpsa-c++ gtpsa lapack blas)

add_executable(a_mul a_mul.cc)
target_link_libraries(a_mul gtpsa-c++ gtpsa lapack blas)
