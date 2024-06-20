#define BOOST_TEST_MODULE gtpsa_arithmetic
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>

#include <gtpsa/tpsa.hpp>
#include <gtpsa/tpsa_double_variant.hpp>
#include <gtpsa/ss_vect.h>
#include <iostream>
#include <sstream>
#include <iomanip>


const int nv = 7;


// mainly test that header compiles
BOOST_AUTO_TEST_CASE(test10_sv_tpsa)
{
  auto a_desc = std::make_shared<gtpsa::desc>(nv, 2);
  auto t1 = gtpsa::tpsa(a_desc, 1);
  gtpsa::ss_vect<gtpsa::tpsa> v1(t1);

  boost::test_tools::output_test_stream output;
  output << v1;
  BOOST_CHECK( !output.is_empty( false ) );

}


// mainly test that header compiles
BOOST_AUTO_TEST_CASE(test11_sv_dbl)
{
  const double a=0e0;
  gtpsa::ss_vect<double> v1(a);

  boost::test_tools::output_test_stream output;
  output << v1;
  BOOST_CHECK( !output.is_empty( false ) );

}


#define print_indices(msg, i, j, val)					\
  do{std::cout << msg << " i " << i  << " j " << j << " : "<< val << std::endl; }while(0)

#define test_identity(v)						\
  do{									\
    for(size_t i = 0; i <v.size(); ++i){				\
      for(size_t j = 0; j <v.size(); ++j){				\
	auto tmp = v[i].get(j);						\
	if(i == j - 1){							\
	  if(tmp < 0.9) { print_indices("on diag", i, j, tmp); }	\
	  BOOST_CHECK_CLOSE(tmp, 1.0, 1e-12);				\
	} else {							\
	  if(tmp > 0.1) { print_indices("off diag", i, j, tmp); }	\
	  BOOST_CHECK_SMALL(tmp,      1e-12);				\
	}								\
      }									\
    }									\
  } while(0)

BOOST_AUTO_TEST_CASE(test12_sv_tpsa_identity)
{
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

BOOST_AUTO_TEST_CASE(test13_sv_tpsa_identity_jac_desc_with_knobs)
{
  const int nv = 9, nk = 3;
  ord_t mo=3, o=2;
  auto a_desc = std::make_shared<gtpsa::desc>(nv, mo, nk, 2);
  // auto t1 = gtpsa::tpsa(a_desc, mad_tpsa_default);
  gtpsa::ss_vect<gtpsa::tpsa> v1(a_desc, o, 6);

  v1.set_identity();
  arma::mat jac = v1.jacobian();
  arma::mat id(jac.n_rows, jac.n_cols, arma::fill::eye);
  arma::mat diff = id - jac;
  arma::mat test = arma::abs(diff);
  arma::mat test_val = arma::sum(arma::sum(test, 0), 1);

  // std::cout << "jac\n" << jac << "Id\n" << id << "diff\n" << diff << std::endl;
  BOOST_CHECK_SMALL(test_val(0,0), 1e-12);

}

// check that the description object has sufficient variable
BOOST_AUTO_TEST_CASE(test14_sv_eng_sufficient_variables)
{

  auto a_desc = std::make_shared<gtpsa::desc>(9, 3);
  // this allocation should fail
  gtpsa::ss_vect<gtpsa::tpsa> *v1 = nullptr;
  BOOST_CHECK_THROW(v1 = new gtpsa::ss_vect<gtpsa::tpsa>(a_desc, 2, 10), std::runtime_error);

  // allocation should have failed
  BOOST_CHECK(v1 == nullptr);
  if(v1){
    // allocation has not failed
    // does setting the variable (and its derivative work)
    v1->set_identity();
    delete v1;
  }
}


// check that the description object has sufficient variable
// should fail before v1->set_identity
BOOST_AUTO_TEST_CASE(test14_sv_eng_sufficient_variables_tpsa)
{
  auto a_desc = std::make_shared<gtpsa::desc>(9, 3);
  gtpsa::tpsa t1(a_desc, 1);
  gtpsa::ss_vect<gtpsa::tpsa> *v1 = nullptr;
  BOOST_CHECK_THROW(v1 = new gtpsa::ss_vect<gtpsa::tpsa>(t1, 10), std::runtime_error);

  // allocation should have failed
  BOOST_CHECK(v1 == nullptr);
  if(v1){
    // should not end up here
    // this test triggered a failure in gtpsa library
    v1->set_identity();
    delete v1;
  }
}

/*
 * This functionallity is not that easily reimplemented in
 * python
 *
 * Here only reference to internal tpsa object
 */
BOOST_AUTO_TEST_CASE(test15_sv_tpsa_component_get_set)
{

  auto a_desc = std::make_shared<gtpsa::desc>(nv, 1);
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
  auto a_desc = std::make_shared<gtpsa::desc>(nv, 1);
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
  auto a_desc = std::make_shared<gtpsa::desc>(nv, 1);
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

  // std::cout << v1 << std::endl;
}

BOOST_AUTO_TEST_CASE(test23_sv_tpsa_show)
{
  const double b1=113e0;
  auto a_desc = std::make_shared<gtpsa::desc>(nv, 1);
  auto t1 = gtpsa::tpsa(a_desc, mad_tpsa_default);

  gtpsa::ss_vect<gtpsa::tpsa> v1(t1);
  v1[0] = b1;
  // std::cout << v1 << std::endl;
}

BOOST_AUTO_TEST_CASE(test30_sv_tpsa_identity_mul)
{
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

  const auto desc = std::make_shared<gtpsa::desc>(nv, 2);
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

  const auto desc = std::make_shared<gtpsa::desc>(nv, 2);
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
  const auto desc = std::make_shared<gtpsa::desc>(nv, 2);
  gtpsa::ss_vect<gtpsa::tpsa> ss1(desc, 1);

  arma::mat jac(nv, nv, arma::fill::eye);

  ss1.setJacobian(jac);
  test_identity(ss1);

}


BOOST_AUTO_TEST_CASE(test51_set_jacobian_primes)
{
  const auto desc = std::make_shared<gtpsa::desc>(nv, 2);
  gtpsa::ss_vect<gtpsa::tpsa> ss1(desc, 1);
  std::string name = "ss1";
  ss1.setName(name);

  arma::mat jac(nv, nv, arma::fill::zeros);

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

BOOST_AUTO_TEST_CASE(test52_test_identity_desc_knobs)
{
  const int nv = 9, nk=4;

  auto desc = std::make_shared<gtpsa::desc>(nv, 2, nk, 1);
  gtpsa::ss_vect<gtpsa::tpsa> vec(desc, 1, nv);
  vec.set_identity();
  arma::mat jac = vec.jacobian();
  BOOST_CHECK_EQUAL(jac.n_rows, nv);
  BOOST_CHECK_EQUAL(jac.n_cols, nv);

  arma::mat id (nv, nv, arma::fill::eye);
  arma::mat diff = jac - id;
  /*
    std::cout << "jac for knobbed vec\n" << jac << std::endl
    << "diff\n" << diff <<std::endl;
  */
  arma::mat test_val = arma::sum(arma::sum(arma::abs(diff), 0), 1);
  BOOST_CHECK_SMALL(test_val(0, 0), 1e-12);

}

BOOST_AUTO_TEST_CASE(test53_test_identity_ss_vect_desc_nv_mismatch)
{
  const int n = 6, nv = 9, nk=4;

  auto desc = std::make_shared<gtpsa::desc>(nv, 2, nk, 1);
  gtpsa::ss_vect<gtpsa::tpsa> vec(desc, 1, n);
  vec.set_identity();
  arma::mat jac = vec.jacobian();
  BOOST_CHECK_EQUAL(jac.n_cols, nv);
  BOOST_CHECK_EQUAL(jac.n_rows, n);

  arma::mat id (n, nv, arma::fill::eye);
  arma::mat diff = jac - id;

  // std::cout << "jac for knobbed vec\n" << jac << std::endl
  // << "diff\n" << diff <<std::endl;

  arma::mat test_val = arma::sum(arma::sum(arma::abs(diff), 0), 1);
  BOOST_CHECK_SMALL(test_val(0, 0), 1e-12);

}


BOOST_AUTO_TEST_CASE(test60_compose_identity)
{
  auto desc = std::make_shared<gtpsa::desc>(nv, 4);

  gtpsa::ss_vect<gtpsa::tpsa> vec(desc, 2);
  arma::mat jac = arma::mat(nv, nv, arma::fill::eye);
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

  arma::mat fill(nv, nv, arma::fill::zeros);
  fill.fill( NAN);
  vec.setJacobian(fill);
  vec.rcompose(v1, v2);
  //std::cout << "composed identity: " << vec << std::endl;
  test_identity(vec);
}

// don't know if it even runs ...
BOOST_AUTO_TEST_CASE(test63_exppb_compiles)
{
  auto desc = std::make_shared<gtpsa::desc>(nv, 4);

  gtpsa::ss_vect<gtpsa::tpsa> vec(desc, 2);
  arma::mat jac = arma::mat(nv, nv, arma::fill::eye);
  vec.setJacobian(jac);

  gtpsa::ss_vect<gtpsa::tpsa> v1 = vec.clone();
  gtpsa::ss_vect<gtpsa::tpsa> v2 = vec.clone();

  v2.rexppb(vec, v1);
}

#if 0
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
#endif


BOOST_AUTO_TEST_CASE(test70_estimate_order)
{
  BOOST_CHECK(gtpsa::estimate_start_order(0, 6) == 0);
  BOOST_CHECK(gtpsa::estimate_start_order(1, 6) == 1);
  BOOST_CHECK(gtpsa::estimate_start_order(2, 6) == 7);
}

BOOST_AUTO_TEST_CASE(test71_hessian)
{
  const double eps = 1e-12;

  auto desc = std::make_shared<gtpsa::desc>(nv, 3);

  gtpsa::ss_vect<gtpsa::tpsa> vec1(desc, 3);

  const double a = 1, b = 2, c = 3, d = 5, e = 7, f = 11;
  const std::vector<double> tmp = {a, b, c, d, e, f};

  gtpsa::ss_vect<double> vec_cst(tmp);
  // std::cout << "vec_cst " << vec_cst << std::endl;

  vec1.set_identity();
  vec1[6].setv(1, {0e0, 0e0, 0e0, 0e0, 0e0, 0e0, 0e0});
  {
    // expects constant term to be zero...
    test_identity(vec1);
  }
  vec1 += vec_cst;
  {
    //std::cout << "hessian\n" << vec1.hessian() << std::endl;

    BOOST_CHECK_CLOSE(vec1[0].cst(), a, eps);
    BOOST_CHECK_CLOSE(vec1[1].cst(), b, eps);
    BOOST_CHECK_CLOSE(vec1[2].cst(), c, eps);
    BOOST_CHECK_CLOSE(vec1[3].cst(), d, eps);
    BOOST_CHECK_CLOSE(vec1[4].cst(), e, eps);
    BOOST_CHECK_CLOSE(vec1[5].cst(), f, eps);

    arma::cube hes = vec1.hessian();
    arma::cube test = arma::abs(hes);

    arma::cube test_val = arma::sum(arma::sum(arma::sum(test, 0), 1), 2);
    // std::cout <<"test\n"<< test_val << std::endl;
    const double tval = test_val(0,0,0);
    BOOST_CHECK_SMALL(tval, eps);
  }

  auto vec2 = vec1.clone();
  auto vec3 = vec1.clone();
  auto vec = vec1.clone();

  // std::cout << "jac vec1\n" << vec1.jacobian() << std::endl;
  const double scale = 1e0/2e0;
  vec[0] = scale *   1 * vec1[0] * vec2[0];// * vec3[0];
  vec[1] = scale *   2 * vec1[1] * vec2[1];// * vec3[1];
  vec[2] = scale *   3 * vec1[2] * vec2[2];// * vec3[2];
  vec[3] = scale *   5 * vec1[3] * vec2[3];// * vec3[3];
  vec[4] = scale *   7 * vec1[4] * vec2[4];// * vec3[4];
  vec[5] = scale *  11 * vec1[5] * vec2[5];// * vec3[5];

  {
    arma::mat jac = vec.jacobian();
    arma::vec vs = {a*a, b*b, c*c, d*d, e*e, f*f};

    // check diagonal
    BOOST_CHECK_CLOSE(vs[0], jac(0, 0), eps);
    jac(0, 0) = 0;
    BOOST_CHECK_CLOSE(vs[1], jac(1, 1), eps);
    jac(1, 1) = 0;
    BOOST_CHECK_CLOSE(vs[2], jac(2, 2), eps);
    jac(2, 2) = 0;
    BOOST_CHECK_CLOSE(vs[3], jac(3, 3), eps);
    jac(3, 3) = 0;
    BOOST_CHECK_CLOSE(vs[4], jac(4, 4), eps);
    jac(4, 4) = 0;
    BOOST_CHECK_CLOSE(vs[5], jac(5, 5), eps);
    jac(5, 5) = 0;

    const arma::mat tmp = arma::sum(arma::sum(arma::abs(jac), 0), 1);
    const double chk = tmp(0, 0);
    BOOST_CHECK_SMALL(chk, eps);

    arma::cube hessian = vec.hessian();
    // std::cout << hessian << std::endl;

    BOOST_CHECK_CLOSE(hessian(0, 0, 0), 1.0/2.0, eps);
    hessian(0,0,0) = 0;
    BOOST_CHECK_CLOSE(hessian(1, 1, 1), 2.0/2.0, eps);
    hessian(1,1,1) = 0;
    BOOST_CHECK_CLOSE(hessian(2, 2, 2), 3.0/2.0, eps);
    hessian(2,2,2) = 0;
    BOOST_CHECK_CLOSE(hessian(3, 3, 3), 5.0/2.0, eps);
    hessian(3,3,3) = 0;
    BOOST_CHECK_CLOSE(hessian(4, 4, 4), 7.0/2.0, eps);
    hessian(4,4,4) = 0;
    BOOST_CHECK_CLOSE(hessian(5, 5, 5), 11.0/2.0, eps);
    hessian(5,5,5) = 0;

    const arma::cube
      tmp2 = arma::sum(arma::sum(arma::sum(arma::abs(hessian), 0), 1), 2);
    const double chk2 = tmp2(0,0,0);
    BOOST_CHECK_SMALL(chk2, eps);
  }

  return;

#if 0
  for(int i=0; i<6; ++i){
    char n = 'a' + i;
    std::string str = "   ";
    str[0] = n;
    vec[i].setName(str);
    vec[i].print();
  }
#endif
  {
    auto nv = vec[0].getDescription()->getNv();
    std::vector<double> read_all (
				  // constant
				  1
				  // first derivative
				  +  nv
				  // second derivative
				  + nv * nv
				  // third derivative
				  //+ nv * nv * nv
				  );
    std::vector<double> hessian (

				 + nv * nv
				 // third derivative
				 //+ nv * nv * nv
				 );
    return;
    for(int i=0; i<6; ++i){
      for(auto& e : hessian) e=-1.0;
      vec[i].getv(nv, &hessian);
      std::cout << "all [" << i << "]";
      int cnt =0;
      for(auto& e : hessian){
	if( cnt % 6 == 0 && cnt != 0){
	  std::cout <<": ";
	}
	std::cout << std::setw(5) << e * 2 << ", "; cnt++;
      }
      std::cout << std::endl;
    }
  }
}

BOOST_AUTO_TEST_CASE(test71_max_order)
{
  const int ord = 3;
  auto desc = std::make_shared<gtpsa::desc>(nv, 3);

  gtpsa::ss_vect<gtpsa::tpsa> vec1(desc, ord);
  auto val = vec1.getMaximumOrder();

  BOOST_CHECK(val == ord);
}

BOOST_AUTO_TEST_CASE(tes75_size_different_from_6)
{
  int nv = 8;

  auto a_desc = std::make_shared<gtpsa::desc>(nv, 3);
  gtpsa::ss_vect<gtpsa::tpsa> ss_vect(a_desc, 2, nv);

  ss_vect.set_identity();
  ss_vect.jacobian();
  ss_vect.hessian();

  auto sv2 = ss_vect.clone();

}

// just compile test
BOOST_AUTO_TEST_CASE(test80_liebra)
{
  auto desc = std::make_shared<gtpsa::desc>(nv, 2);

  gtpsa::ss_vect<gtpsa::tpsa> vec(desc, 1);
  vec.set_identity();

  auto vec2 = vec.clone();
  auto res = vec.allocateLikeMe();
  res.rliebra(vec, vec2);
}

BOOST_AUTO_TEST_CASE(test81_exppb)
{
  auto desc = std::make_shared<gtpsa::desc>(nv, 2);

  gtpsa::ss_vect<gtpsa::tpsa> vec(desc, 1);
  vec.set_identity();

  auto vec2 = vec.allocateLikeMe();
  vec2.set_identity();
  const double value = -1;
  for(size_t k = 0; k < vec2.size(); ++k){ vec2[k] += value; }
  auto res = gtpsa::exppb(vec, vec2);

  BOOST_CHECK_CLOSE(res[0].cst(), value, 1e-12);
  BOOST_CHECK_CLOSE(res[1].cst(), value, 1e-12);
  BOOST_CHECK_CLOSE(res[2].cst(), value, 1e-12);
  BOOST_CHECK_CLOSE(res[3].cst(), value, 1e-12);
  BOOST_CHECK_CLOSE(res[4].cst(), value, 1e-12);
  BOOST_CHECK_CLOSE(res[5].cst(), value, 1e-12);

  arma::mat jac = res.jacobian();
  arma::mat check = arma::mat(nv, nv, arma::fill::eye) * std::exp(1.0);
  arma::mat adiff = arma::abs(jac - check);
  arma::vec tmp = arma::sum(adiff, 1);
  BOOST_CHECK_SMALL(arma::sum(tmp), 1e-12);

}

#if 0
// not good input data
BOOST_AUTO_TEST_CASE(test82_logpb)
{
  arma::mat jacf(6, 6, arma::fill::zeros);
  jacf(0, 1) = -1.0;
  jacf(1, 0) = 1.0;
  jacf(2, 3) = -1.0;
  jacf(3, 2) = 1.0;
  jacf(4, 5) = -1.0;
  jacf(5, 4) = 1.0;
  //jacf *= -std::exp(1.0);
  std::cout << "jacf \n" << jacf << std::endl;
  auto desc = std::make_shared<gtpsa::desc>(6, 2);

  gtpsa::ss_vect<gtpsa::tpsa> vec(desc, 1);
  vec.setJacobian(jacf);
  const double val = 10;
  for(size_t k = 0; k < vec.size(); ++k){ vec[k] += val; }

  auto vec2 = vec.allocateLikeMe();
  arma::mat ones =arma::mat(6, 6, arma::fill::ones);
  vec2.setJacobian(ones);
  const double val2 = 1.0;
  for(size_t k = 0; k < vec2.size(); ++k){ vec2[k] += val2; }

  auto res = gtpsa::logpb(vec, vec2);

#if 0
  for(size_t k = 0; k < vec2.size(); ++k){
    char cname[10] = "log  \0\0\0";
    cname[5] = 'a' + k;
    std::string name(cname);
    res[k].print(name.c_str());
  }
#endif

  BOOST_CHECK_CLOSE(res[0].cst(), val, 1e-12);
  BOOST_CHECK_CLOSE(res[1].cst(), val, 1e-12);
  BOOST_CHECK_CLOSE(res[2].cst(), val, 1e-12);
  BOOST_CHECK_CLOSE(res[3].cst(), val, 1e-12);
  BOOST_CHECK_CLOSE(res[4].cst(), val, 1e-12);
  BOOST_CHECK_CLOSE(res[5].cst(), val, 1e-12);

  arma::mat jac = res.jacobian();
  arma::mat check = arma::mat(6, 6, arma::fill::eye) * std::exp(1.0);
  arma::mat adiff = arma::abs(jac - check);
  arma::vec tmp = arma::sum(adiff, 1);
  BOOST_CHECK_SMALL(arma::sum(tmp), 1e-12);
}
#endif

// check that it works, does not crash
BOOST_AUTO_TEST_CASE(test120_vec2fld)
{
  auto desc = std::make_shared<gtpsa::desc>(nv, 3);
  gtpsa::ss_vect<gtpsa::tpsa> vec(desc, 3);

  gtpsa::tpsa t1(desc, 3);
  t1.set(0, 42);
  t1.setv(1, {2, -3, 5, -7, 11, -13, 17, -19, 23, -25, 29, -31, 37, -41, 43, -47});

  const gtpsa::tpsa t = t1.clone();
#if 0
  t.print("t");
#endif

  vec.rvec2fld(t1);
  for(size_t k = 0; k < vec.size(); ++k){
    char cname[] = "vec2fld \0\0";
    cname[8] = 'a' + k;
    std::string name(cname);
    // vec[k].print(name.c_str());
  }

  // let test deliberately fail: to see output
  // BOOST_CHECK(1 == 0);
}

// check that it works, does not crash
BOOST_AUTO_TEST_CASE(test121_fld2vec)
{
  auto desc = std::make_shared<gtpsa::desc>(nv, 3);
  gtpsa::ss_vect<gtpsa::tpsa> vec(desc, 3);
  gtpsa::ss_vect<double> d(std::vector<double>({-3, 5, -7, 11, -13, 17}));

  arma::mat jac(nv, nv, arma::fill::zeros);
  // encode offset from digonal next to row and col
  for(int col = 0; col<6; ++ col){
    for(int row = 0; row<6; ++ row){
      int d = row - col;
      double tmp =( 10 * row  + col)/1000e0;
      double val = d > 0 ? d + tmp : d - tmp ;
      jac(row, col) = val;
    }
  }
  // std::cout << "jac\n" << jac << std::endl;

  vec.setJacobian(jac);
  vec += d;
  const gtpsa::ss_vect<gtpsa::tpsa> v = vec.clone();
  for(size_t k = 0; k < vec.size(); ++k){
    char cname[] = "v   \0\0";
    cname[3] = 'a' + k;
    std::string name(cname);
    // vec[k].print(name.c_str());
  }

  gtpsa::tpsa t(desc, 3);

  v.fld2vec(&t);

  BOOST_CHECK_CLOSE(t.get(), 0, 1e-12);
  // t.print("t");

  // let test deliberately fail: to see output
  // BOOST_CHECK(1 == 0);
}


// check that it works, does not crash
BOOST_AUTO_TEST_CASE(test122_vec2fld_and_back)
{
  auto desc = std::make_shared<gtpsa::desc>(nv, 3);
  gtpsa::ss_vect<gtpsa::tpsa> vec(desc, 3);
  double val = 42e0;

  gtpsa::tpsa t1(desc, 3);
  t1.set(0, val);
  std::vector<double> v_init = {2, -3, 5, -7, 11, -13, 17, -19, 23, -25, 29, -31, 37, -41, 43, -47};
  t1.setv(1, v_init);

  const gtpsa::tpsa t = t1.clone();
  BOOST_CHECK_CLOSE(t.get(), val, 1e-12);
  //t.print("t");

  vec.rvec2fld(t1);
  const auto v = vec.clone();

  gtpsa::tpsa tc = t1.newFromThis();

  v.fld2vec(&tc);

  std::vector<double> v_check(v_init.size());
  t1.getv(1, &v_check);

  BOOST_CHECK_SMALL(tc.get(), 1e-12);
  for(size_t cnt = 0; cnt < v_init.size(); cnt++){
    BOOST_CHECK_CLOSE(v_init[cnt], v_check[cnt], 1e-12);
  }
  //tc.print("tc");

  // let test deliberately fail: to see output
  // BOOST_CHECK(1 == 0);
}

// check that it works, does not crash
BOOST_AUTO_TEST_CASE(test130_compute_norm)
{
  auto desc = std::make_shared<gtpsa::desc>(nv, 3);
  gtpsa::ss_vect<gtpsa::tpsa> vec(desc, 3);

  vec.set_identity();
  const auto v = vec.clone();

  auto nrm = vec.computeNorm();
  BOOST_CHECK_CLOSE(nrm, 7e0, 1e-12);
  // let test deliberately fail: to see output
  BOOST_CHECK(0 == 0);
}

BOOST_AUTO_TEST_CASE(test140_inverse)
{
  auto desc = std::make_shared<gtpsa::desc>(nv, 3);
  gtpsa::ss_vect<gtpsa::tpsa> vec(desc, 3);
  vec.set_identity();
  const auto v = vec.clone();

  auto res = v.allocateLikeMe();
  res.rminv(v);

#if 0
  for(size_t k = 0; k < vec.size(); ++k){
    char cname[] = "inv    \0\0";
    cname[5] = 'a' + k;
    std::string name(cname);
    res[k].print(name.c_str());
  }
  // BOOST_CHECK(1 == 0);

#endif
  {
    const auto resc = res.clone();
    arma::mat jac = resc.jacobian();
    arma::mat id(nv, nv, arma::fill::eye);

    arma::mat chk = arma::abs(jac - id);
    arma::vec vec = arma::sum(chk, 1);
    BOOST_CHECK_SMALL(arma::sum(vec), 1e-12);
  }

  arma::mat jac = {
    {101,   2,   3,   4,   5,   6, 0},
    { -7, 102,   8,   9,  10,  11, 0},
    {-13, -12, 103,  14,  15,  16, 0},
    {-17, -18, -19, 104,  20,  21, 0},
    {-22, -23, -24, -25, 105,  26, 0},
    {-27, -28, -29, -30, -31, 106, 0},
    {0,     0,   0,   0,   0,   0, 1}
  };
  jac /= 100e0;
  std::cout << jac << std::endl;

  vec.set_zero();
  vec.setJacobian(jac);


  {
    arma::mat jac_inv = arma::inv(jac);

    auto res = gtpsa::minv(vec);
    for(size_t k = 0; k < vec.size(); ++k){
      char cname[] = "inv any   \0\0";
      cname[8] = 'a' + k;
      std::string name(cname);
      // res[k].print(name.c_str());
    }

    // std::cout << "arma:  jac inv\n" << jac_inv << std::endl;

    arma::mat chk = arma::abs(jac_inv - res.jacobian());
    arma::vec vec = arma::sum(chk, 1);
    BOOST_CHECK_SMALL(arma::sum(vec), 1e-12);
  }

}


// as required by standard observer
BOOST_AUTO_TEST_CASE(test160_copy_inplace)
{
  auto desc = std::make_shared<gtpsa::desc>(nv, 2);
  gtpsa::ss_vect<gtpsa::tpsa> vec(desc, 1);
  vec.set_identity();
  vec[2].set(0, 2);
  BOOST_CHECK_CLOSE(vec[2].get(), 2, 1e-12);

  gtpsa::ss_vect<gtpsa::tpsa> vec2(desc, 1);
  vec2._copyInPlace(vec);

  arma::mat eye(nv, nv, arma::fill::eye);
  {
    arma::mat chk = arma::abs(eye - vec2.jacobian());
    arma::vec vchk = arma::sum(chk, 1);
    BOOST_CHECK_SMALL(arma::sum(vchk), 1e-12);

    // constant part copied ?
    BOOST_CHECK_CLOSE(vec2[2].get(), 2, 1e-12);
  }

  // and if I modify the original vector
  vec.set_zero();
  vec[2].set(0, 0);
  vec[4].set(0, 4);

  {
    arma::mat chk = arma::abs(eye - vec2.jacobian());
    arma::vec vchk = arma::sum(chk, 1);
    BOOST_CHECK_SMALL(arma::sum(vchk), 1e-12);

    // constant part copied ?
    BOOST_CHECK_CLOSE(vec2[2].get(), 2, 1e-12);
  }
}
