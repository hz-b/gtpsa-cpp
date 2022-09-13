# current hack .. until the library is properly defined
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../c++)

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
