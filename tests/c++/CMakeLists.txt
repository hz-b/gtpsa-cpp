
find_package(Boost 1.52.0 REQUIRED COMPONENTS
  prg_exec_monitor
  unit_test_framework
  program_options
  )

add_executable(test_arithmetic test_arithmetic.cc)
target_link_libraries(test_arithmetic
  gtpsa
  lapack
  blas
  ${Boost_PRG_EXEC_MONITOR_LIBRARY}
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
add_test(arithmetic test_arithmetic)


add_executable(test_ss_vect test_ss_vect.cc)
target_link_libraries(test_ss_vect
  gtpsa
  lapack
  blas
  ${Boost_PRG_EXEC_MONITOR_LIBRARY}
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
add_test(test_ss_vect test_ss_vect)

add_executable(test_template test_template.cc)
target_link_libraries(test_template
  gtpsa
  lapack
  blas
  ${Boost_PRG_EXEC_MONITOR_LIBRARY}
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
add_test(test_template test_template)
