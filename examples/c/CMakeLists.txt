# current hack .. until the library is properly defined
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../c++)

set(GTPSA_EX_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../gtpsa/external_repositories/mad-ng/src/libgtpsa/)

add_executable(gtpsa_ex0_c ${GTPSA_EX_DIR}/gtpsa_ex0.c)
add_executable(gtpsa_ex1_c ${GTPSA_EX_DIR}/gtpsa_ex1.c)
add_executable(gtpsa_ex2_c ${GTPSA_EX_DIR}/gtpsa_ex2.c)
add_executable(gtpsa_ex3_c ${GTPSA_EX_DIR}/gtpsa_ex3.c)
add_executable(gtpsa_ex4_c ${GTPSA_EX_DIR}/gtpsa_ex4.c)
add_executable(gtpsa_ex5_c ${GTPSA_EX_DIR}/gtpsa_ex5.c)
add_executable(gtpsa_ex6_c ${GTPSA_EX_DIR}/gtpsa_ex6.c)

target_link_libraries(gtpsa_ex0_c gtpsa lapack blas)
target_link_libraries(gtpsa_ex1_c gtpsa lapack blas)
target_link_libraries(gtpsa_ex2_c gtpsa lapack blas)
target_link_libraries(gtpsa_ex3_c gtpsa lapack blas)
target_link_libraries(gtpsa_ex4_c gtpsa lapack blas)
target_link_libraries(gtpsa_ex5_c gtpsa lapack blas)
target_link_libraries(gtpsa_ex6_c gtpsa lapack blas)
