add_executable(test_desc_double_allocate test_desc_double_allocate.c)
target_link_libraries(test_desc_double_allocate
        gtpsa
        lapack
        blas
)
add_test(desc_double_allocate test_desc_double_allocate)

add_executable(test_desc_many_pars test_desc_many_pars.c)
target_link_libraries(test_desc_many_pars
        gtpsa
        lapack
        blas
)
add_test(desc_many_pars test_desc_many_pars)
