# current hack .. until the library is properly defined
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../c++)


add_executable(gtpsa_ex0_c
  gtpsa_ex0.c
)

target_link_libraries(gtpsa_ex0_c
  gtpsa
  lapack
  blas
)
