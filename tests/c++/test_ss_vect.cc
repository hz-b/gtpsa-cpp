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
    const double b1=113e0, b2=355e0;

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
    gtpsa::ss_vect<double> ps = ps.clone();

}
