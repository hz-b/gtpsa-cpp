# current hack .. until the library is properly defined
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../c++)


add_executable(gtpsa_ex0_c gtpsa_ex0.c)
add_executable(gtpsa_ex1_c gtpsa_ex1.c)
add_executable(gtpsa_ex2_c gtpsa_ex2.c)
add_executable(gtpsa_ex3_c gtpsa_ex3.c)
add_executable(gtpsa_ex4_c gtpsa_ex4.c)

target_link_libraries(gtpsa_ex0_c gtpsa lapack blas)
target_link_libraries(gtpsa_ex1_c gtpsa lapack blas)
target_link_libraries(gtpsa_ex2_c gtpsa lapack blas)
target_link_libraries(gtpsa_ex3_c gtpsa lapack blas)
target_link_libraries(gtpsa_ex4_c gtpsa lapack blas)
