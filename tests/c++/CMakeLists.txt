add_definitions(
  -Weffc++
  )

find_package(Boost 1.52.0 REQUIRED COMPONENTS
  prg_exec_monitor
  unit_test_framework
  program_options
  )


add_executable(test_consistency test_consistency.cc)
target_link_libraries(test_consistency
        gtpsa-c++
        gtpsa
        lapack
        blas
        ${Boost_PRG_EXEC_MONITOR_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        )
add_test(consistency test_consistency)

add_executable(test_arithmetic test_arithmetic.cc)
target_link_libraries(test_arithmetic
        gtpsa-c++
        gtpsa
        lapack
        blas
        ${Boost_PRG_EXEC_MONITOR_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        )
add_test(arithmetic test_arithmetic)

add_executable(test_complex_arithmetic test_complex_arithmetic.cc)
target_link_libraries(test_complex_arithmetic
        gtpsa-c++
        gtpsa
        lapack
        blas
        ${Boost_PRG_EXEC_MONITOR_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        )
add_test(complex_arithmetic test_complex_arithmetic)

add_executable(test_mad_wrapper_container test_mad_wrapper_container.cpp )
target_link_libraries(test_mad_wrapper_container
        gtpsa-c++
        gtpsa
        lapack
        blas
        ${Boost_PRG_EXEC_MONITOR_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        )
add_test(mad_wrapper_container test_mad_wrapper_container)

add_executable(test_bridge_container test_bridge_container.cpp )
target_link_libraries(test_bridge_container
        gtpsa-c++
        gtpsa
        lapack
        blas
        ${Boost_PRG_EXEC_MONITOR_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        )
add_test(bridge_container test_bridge_container)

# test_arithmetic_container.cc
add_executable(test_ss_vect test_ss_vect.cc)
target_link_libraries(test_ss_vect
  gtpsa-c++
  gtpsa
  lapack
  blas
  ${Boost_PRG_EXEC_MONITOR_LIBRARY}
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
add_test(test_ss_vect test_ss_vect)

add_executable(ss_vect_compose ss_vect_compose.cpp)
target_link_libraries(ss_vect_compose
        gtpsa-c++
        gtpsa
        lapack
        blas
        ${Boost_PRG_EXEC_MONITOR_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        )


add_executable(test_template test_template.cc)
target_link_libraries(test_template
  gtpsa-c++
  gtpsa
  lapack
  blas
  ${Boost_PRG_EXEC_MONITOR_LIBRARY}
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
add_test(test_template test_template)

add_executable(test_desc test_desc.cc)
target_link_libraries(test_desc
  gtpsa-c++
  gtpsa
  lapack
  blas
  ${Boost_PRG_EXEC_MONITOR_LIBRARY}
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
add_test(test_desc test_desc.cc)
