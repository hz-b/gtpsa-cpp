#define BOOST_TEST_MODULE gtpsa_arithmetic
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <gtpsa/tpsa.hpp>
#include <gtpsa/ss_vect.h>
#include <iostream>
#include <sstream>

// mainly test that header compiles
BOOST_AUTO_TEST_CASE(test10_sv_tpsa)
{
    auto a_desc = std::make_shared<gtpsa::desc>(1, 0);
    auto t1 = gtpsa::tpsa(a_desc, mad_tpsa_default);
    gtpsa::ss_vect<gtpsa::tpsa> v1(t1);

}


// mainly test that header compiles
BOOST_AUTO_TEST_CASE(test11_sv_dbl)
{
    const double a=0e0;
    gtpsa::ss_vect<double> v1(a);
}


#define print_indices(i, j) \
    do{std::cout << "i " << i  << " j " << j << std::endl; }while(0)

#define test_identity(v)                                   \
    do{							   \
	for(size_t i = 0; i <v.size(); ++i){		   \
	    for(size_t j = 0; j <v.size(); ++j){	   \
	    auto tmp = v[i].get(j);                        \
		if(i == j - 1){				   \
		    if(tmp < 0.9) { print_indices(i, j); } \
		    BOOST_CHECK_CLOSE(tmp, 1.0, 1e-12);    \
		} else {				   \
		    if(tmp > 0.1) { print_indices(i, j); } \
		    BOOST_CHECK_SMALL(tmp,      1e-12);	   \
		}					   \
	    }						   \
	}						   \
    } while(0)

BOOST_AUTO_TEST_CASE(test12_sv_tpsa_identity)
{

    const int nv = 6;
    auto a_desc = std::make_shared<gtpsa::desc>(nv, 1);
    auto t1 = gtpsa::tpsa(a_desc, mad_tpsa_default);
    gtpsa::ss_vect<gtpsa::tpsa> v1(t1);

    BOOST_CHECK_EQUAL(v1.size(), nv);
    /*
    for(size_t i = 0; i <6; ++i){
	std::ostringstream strm;
	strm << "v1," << i;
	v1[i].print(strm.str().c_str(), 0,0,0);
    }
    */
    v1.set_identity();
    BOOST_CHECK_CLOSE(v1[0].get(1), 1.0, 1e-12);
    test_identity(v1);
    v1.size();
    test_identity(v1);
    /*
    for(size_t i = 0; i <6; ++i){
	std::ostringstream strm;
	strm << "v1," << i;
	v1[i].print(strm.str().c_str(), 0,0,0);
    }
    */
    // std::cout << v1 << std::endl;
}

/*
 * This functionallity is not that easily reimplemented in
 * python
 *
 * Here only reference to internal tpsa object
 */
BOOST_AUTO_TEST_CASE(test15_sv_tpsa_component_get_set)
{

    auto a_desc = std::make_shared<gtpsa::desc>(6, 1);
    gtpsa::ss_vect<gtpsa::tpsa> v1(a_desc, 1), v3(a_desc, 1);

    v1.set_zero();
    v3.set_zero();

    double val = 3e0;
    /* here same object, pybind11 currently seems to make an internal copy */
    v1[2].set(0, val);
    v3.at(2).set(0, val);

    // check that operator and at both set the value
    BOOST_CHECK_CLOSE(v1[2].get(), val, 1e-12);
    BOOST_CHECK_CLOSE(v3[2].get(), val, 1e-12);

    // check that value is also retrievable by at
    BOOST_CHECK_CLOSE(v1.at(2).get(), val, 1e-12);
    BOOST_CHECK_CLOSE(v3.at(2).get(), val, 1e-12);

    gtpsa::ss_vect<gtpsa::tpsa> v2(a_desc, 1);
    v2.set_zero();

    auto& t = v2[2];
    t.set(0, 115e0);

    BOOST_CHECK_CLOSE(t.get(),     115, 1e-12);
    BOOST_CHECK_CLOSE(v2[2].get(), 115, 1e-12);

}

/*
 * check that assignment of a new variable works ...
 */
BOOST_AUTO_TEST_CASE(test16_sv_tpsa_component_set_from_tpsa)
{
    auto a_desc = std::make_shared<gtpsa::desc>(6, 1);
    gtpsa::ss_vect<gtpsa::tpsa> v1(a_desc, 1);
    double val = 355e0;

    // defined start
    v1.set_zero();

    gtpsa::tpsa t1(a_desc, 1);
    t1.set(0e0, val);

    BOOST_CHECK_SMALL(v1.at(2).get(), 1e-12);
    v1.at(2) = t1;

    BOOST_CHECK_CLOSE(v1.at(2).get(), val, 1e-12);

}

BOOST_AUTO_TEST_CASE(test20_sv_tpsa_clone)
{
    const double a = 0e0, b1=113e0, b2=355e0;
    auto a_desc = std::make_shared<gtpsa::desc>(6, 1);
    auto t1 = gtpsa::tpsa(a_desc, 1);

    gtpsa::ss_vect<gtpsa::tpsa> v1(t1);
    v1[0].set(a, b1);
    auto v2 = v1.clone();
    auto& tt1 = v1[0];
    auto& tt2 = v2[0];

    BOOST_CHECK_CLOSE(tt1.get(), b1, 1e-12);
    BOOST_CHECK_CLOSE(tt2.get(), b1, 1e-12);

    v2[0].set(a, b2);

    BOOST_CHECK_CLOSE(tt1.get(), b1, 1e-12);
    BOOST_CHECK_CLOSE(tt2.get(), b2, 1e-12);
}


BOOST_AUTO_TEST_CASE(test21_sv_double_clone)
{
    auto t1 = 0e0;
    const double b1=113e0;

    gtpsa::ss_vect<double> v1(t1);

    v1[0] = b1;
    auto v2 = v1.clone();
    // double tt1 = v1[0];
    // double tt2 = v2[0];

    BOOST_CHECK_CLOSE(v1[0], b1, 1e-12);
    BOOST_CHECK_CLOSE(v2[0], b1, 1e-12);
    return;

    // v2[0] = b2;
    // auto tt4 = v2[0];

    // BOOST_CHECK_CLOSE(tt1, b1, 1e-12);
    // BOOST_CHECK_CLOSE(tt4, b2, 1e-12);

}

BOOST_AUTO_TEST_CASE(test22_sv_double_show)
{
    auto t1 = 0e0;
    gtpsa::ss_vect<double> v1(t1);
    const double b1=113e0;
    v1[0] = b1;

    std::cout << v1 << std::endl;
}

BOOST_AUTO_TEST_CASE(test23_sv_tpsa_show)
{
    const double b1=113e0;
    auto a_desc = std::make_shared<gtpsa::desc>(6, 1);
    auto t1 = gtpsa::tpsa(a_desc, mad_tpsa_default);

    gtpsa::ss_vect<gtpsa::tpsa> v1(t1);
    v1[0] = b1;
    std::cout << v1 << std::endl;
}

BOOST_AUTO_TEST_CASE(test30_sv_tpsa_identity_mul)
{
    const int nv = 6;
    auto a_desc = std::make_shared<gtpsa::desc>(nv, 1);
    auto t1 = gtpsa::tpsa(a_desc, mad_tpsa_default);
    gtpsa::ss_vect<gtpsa::tpsa> v1(t1);
    gtpsa::ss_vect<gtpsa::tpsa> v2(t1);

    /*
    for(size_t i = 0; i <6; ++i){
	std::ostringstream strm;
	strm << "v1," << i;
	v1[i].print(strm.str().c_str(), 0,0,0);
    }
    */
    v1.set_identity();
    test_identity(v1);

    v2.set_identity();
    test_identity(v2);
#if 0
    auto v3 = v2 * v1;

    // v3 should be all zeros now ...
    for(size_t i = 0; i <6; ++i){
	std::ostringstream strm;
	strm << "v3," << i;
	v3[i].print(strm.str().c_str(), 0,0,0);
    }
#endif
}

BOOST_AUTO_TEST_CASE(test40_ss_vect_double_dbl_assignment)
{
    const double x = 355;
    const gtpsa::ss_vect<double> ps_orig = {x, 0, 0, 0, 0, 0};
    // the code below should either run or fail during compilation
    // compiler warns on that if optimisation is on
    // the variable on the right should be ps_orig

    // gtpsa::ss_vect<double> ps = ps.clone();

}

BOOST_AUTO_TEST_CASE(test40_ss_vect_radd_dbl_vec)
{
    const double a = 1/2., b = 2/3., c = 3/4.,  d = 4/5., e = 5/6., f = 6/7.;

    const auto desc = std::make_shared<gtpsa::desc> (6, 2);
    gtpsa::ss_vect<gtpsa::tpsa> ss1(desc, 1);

    std::vector<double> v1_vals {a, b, c, d, e, f};
    const gtpsa::ss_vect<double> v2(v1_vals);

    ss1 += v2;

    BOOST_CHECK_CLOSE(ss1[0].cst(), a, 1e-12);
    BOOST_CHECK_CLOSE(ss1[1].cst(), b, 1e-12);
    BOOST_CHECK_CLOSE(ss1[2].cst(), c, 1e-12);
    BOOST_CHECK_CLOSE(ss1[3].cst(), d, 1e-12);
    BOOST_CHECK_CLOSE(ss1[4].cst(), e, 1e-12);
    BOOST_CHECK_CLOSE(ss1[5].cst(), f, 1e-12);

}


BOOST_AUTO_TEST_CASE(test41_ss_vect_add_dbl_vec)
{
    const double a = 1/2., b = 2/3., c = 3/4.,  d = 4/5., e = 5/6., f = 6/7.;

    const auto desc = std::make_shared<gtpsa::desc> (6, 2);
    const gtpsa::ss_vect<gtpsa::tpsa> ss1(desc, 1);

    std::vector<double> v1_vals {a, b, c, d, e, f};
    const gtpsa::ss_vect<double> v2(v1_vals);

    auto ss2 = ss1 + v2;

    BOOST_CHECK_CLOSE(ss2[0].cst(), a, 1e-12);
    BOOST_CHECK_CLOSE(ss2[1].cst(), b, 1e-12);
    BOOST_CHECK_CLOSE(ss2[2].cst(), c, 1e-12);
    BOOST_CHECK_CLOSE(ss2[3].cst(), d, 1e-12);
    BOOST_CHECK_CLOSE(ss2[4].cst(), e, 1e-12);
    BOOST_CHECK_CLOSE(ss2[5].cst(), f, 1e-12);
}

BOOST_AUTO_TEST_CASE(test50_set_jacobian_identity)
{
    const auto desc = std::make_shared<gtpsa::desc> (6, 2);
    gtpsa::ss_vect<gtpsa::tpsa> ss1(desc, 1);

    arma::mat jac(6, 6, arma::fill::eye);

    ss1.setJacobian(jac);

    test_identity(ss1);

}


BOOST_AUTO_TEST_CASE(test51_set_jacobian_primes)
{
    const auto desc = std::make_shared<gtpsa::desc> (6, 2);
    gtpsa::ss_vect<gtpsa::tpsa> ss1(desc, 1);
    std::string name = "ss1";
    ss1.setName(name);

    arma::mat jac(6, 6, arma::fill::zeros);

    jac(0, 1) =  2;
    jac(1, 0) =  3;
    jac(2, 3) =  5;
    jac(3, 2) =  7;
    jac(4, 5) = 11;
    jac(5, 4) = 13;

    ss1.setJacobian(jac);

    BOOST_CHECK_CLOSE( 2, ss1[0].get(1 + 1), 1e-14);
    BOOST_CHECK_CLOSE( 3, ss1[1].get(1 + 0), 1e-14);

    BOOST_CHECK_CLOSE( 5, ss1[2].get(1 + 3), 1e-14);
    BOOST_CHECK_CLOSE( 7, ss1[3].get(1 + 2), 1e-14);

    BOOST_CHECK_CLOSE(11, ss1[4].get(1 + 5), 1e-14);
    BOOST_CHECK_CLOSE(13, ss1[5].get(1 + 4), 1e-14);


    BOOST_CHECK_SMALL(ss1[0].get(1 + 0), 1e-14);
    // BOOST_CHECK_SMALL(ss1[0].get(1 + 1), 1e-14);
    BOOST_CHECK_SMALL(ss1[0].get(1 + 2), 1e-14);
    BOOST_CHECK_SMALL(ss1[0].get(1 + 3), 1e-14);
    BOOST_CHECK_SMALL(ss1[0].get(1 + 4), 1e-14);
    BOOST_CHECK_SMALL(ss1[0].get(1 + 5), 1e-14);

    // BOOST_CHECK_SMALL(ss1[1].get(1 + 0), 1e-14);
    BOOST_CHECK_SMALL(ss1[1].get(1 + 1), 1e-14);
    BOOST_CHECK_SMALL(ss1[1].get(1 + 2), 1e-14);
    BOOST_CHECK_SMALL(ss1[1].get(1 + 3), 1e-14);
    BOOST_CHECK_SMALL(ss1[1].get(1 + 4), 1e-14);
    BOOST_CHECK_SMALL(ss1[1].get(1 + 5), 1e-14);

    BOOST_CHECK_SMALL(ss1[2].get(1 + 0), 1e-14);
    BOOST_CHECK_SMALL(ss1[2].get(1 + 1), 1e-14);
    BOOST_CHECK_SMALL(ss1[2].get(1 + 2), 1e-14);
    // BOOST_CHECK_SMALL(ss1[2].get(1 + 3), 1e-14);
    BOOST_CHECK_SMALL(ss1[2].get(1 + 4), 1e-14);
    BOOST_CHECK_SMALL(ss1[2].get(1 + 5), 1e-14);

    BOOST_CHECK_SMALL(ss1[3].get(1 + 0), 1e-14);
    BOOST_CHECK_SMALL(ss1[3].get(1 + 1), 1e-14);
    //BOOST_CHECK_SMALL(ss1[3].get(1 + 2), 1e-14);
    BOOST_CHECK_SMALL(ss1[3].get(1 + 3), 1e-14);
    BOOST_CHECK_SMALL(ss1[3].get(1 + 4), 1e-14);
    BOOST_CHECK_SMALL(ss1[3].get(1 + 5), 1e-14);

    BOOST_CHECK_SMALL(ss1[4].get(1 + 0), 1e-14);
    BOOST_CHECK_SMALL(ss1[4].get(1 + 1), 1e-14);
    BOOST_CHECK_SMALL(ss1[4].get(1 + 2), 1e-14);
    BOOST_CHECK_SMALL(ss1[4].get(1 + 3), 1e-14);
    BOOST_CHECK_SMALL(ss1[4].get(1 + 4), 1e-14);
    //BOOST_CHECK_SMALL(ss1[4].get(1 + 5), 1e-14);

    BOOST_CHECK_SMALL(ss1[5].get(1 + 0), 1e-14);
    BOOST_CHECK_SMALL(ss1[5].get(1 + 1), 1e-14);
    BOOST_CHECK_SMALL(ss1[5].get(1 + 2), 1e-14);
    BOOST_CHECK_SMALL(ss1[5].get(1 + 3), 1e-14);
    //BOOST_CHECK_SMALL(ss1[5].get(1 + 4), 1e-14);
    BOOST_CHECK_SMALL(ss1[5].get(1 + 5), 1e-14);


}

BOOST_AUTO_TEST_CASE(test60_compose_identity)
{
    auto desc = std::make_shared<gtpsa::desc>(6, 4);

    gtpsa::ss_vect<gtpsa::tpsa> vec(desc, 2);
    arma::mat jac = arma::mat(6, 6, arma::fill::eye);
    vec.setJacobian(jac);
    test_identity(vec);
 // used for tracing variables in debugger
    vec[0].setName("vec_0");
    vec[1].setName("vec_1");
    vec[2].setName("vec_2");
    vec[3].setName("vec_3");
    vec[4].setName("vec_4");
    vec[5].setName("vec_5");
    gtpsa::ss_vect<gtpsa::tpsa> v1 = vec.clone();//, v2 = vec.clone();
    gtpsa::ss_vect<gtpsa::tpsa> v2 = vec.clone();
    v1[0].setName("v1_0");
    v1[1].setName("v1_1");
    v1[2].setName("v1_2");
    v1[3].setName("v1_3");
    v1[4].setName("v1_4");
    v1[5].setName("v1_5");
    v2[0].setName("v2_0");
    v2[1].setName("v2_1");
    v2[2].setName("v2_2");
    v2[3].setName("v2_3");
    v2[4].setName("v2_4");
    v2[5].setName("v2_5");
    //test_identity(vec);
    //test_identity(v1);
    //test_identity(v2);

    arma::mat fill(6, 6, arma::fill::zeros);
    fill.fill( NAN);
    vec.setJacobian(fill);
    vec.rcompose(v1, v2);
    std::cout << "composed identity: " << vec << std::endl;
    test_identity(vec);
}


BOOST_AUTO_TEST_CASE(test62_compose_omega_matrix)
{

    auto desc = std::make_shared<gtpsa::desc>(6, 2);
    gtpsa::ss_vect<gtpsa::tpsa> vec(desc, 1);
    arma::mat omega(6, 6, arma::fill::zeros);
    omega(0, 1) = 1;
    omega(1, 0) = 1;
    omega(2, 3) = 1;
    omega(3, 2) = 1;
    omega(5, 4) = 1;
    omega(4, 5) = 1;

    std::cout << "omega (ct delta swappwd)" << std::endl;

}