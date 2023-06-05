#define BOOST_TEST_MODULE gtpsa_arithmetic
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <gtpsa/tpsa.hpp>
#include <array>
#include <cmath>
#include <iostream>

BOOST_AUTO_TEST_CASE(test00_set)
{
    const double a=0e0, b=42e0;

    auto a_desc = std::make_shared<gtpsa::desc>(1, 1);
    auto t1 = gtpsa::tpsa(a_desc, mad_tpsa_default);

    t1.set(a, b);
    BOOST_CHECK_CLOSE(t1.get(), b, 1e-12);
    t1.set(a, b);
    BOOST_CHECK_CLOSE(t1.get(), b, 1e-12);

}

BOOST_AUTO_TEST_CASE(test01_clone)
{
    const double a=355e0, b=113e0;

    auto a_desc = std::make_shared<gtpsa::desc>(1, 1);
    auto t1 = gtpsa::tpsa(a_desc, mad_tpsa_default);
    t1.set(a, b);

    const auto t2 = t1.clone();

    BOOST_CHECK_CLOSE(t1.get(), b, 1e-12);
    BOOST_CHECK_CLOSE(t2.get(), b, 1e-12);

}


BOOST_AUTO_TEST_CASE(test02_clone_2_objects)
{
    const double a=355e0, b=113e0;

    auto a_desc = std::make_shared<gtpsa::desc>(1, 1);
    auto t1 = gtpsa::tpsa(a_desc, mad_tpsa_default);
    t1.set(a, b);

    auto t2 = gtpsa::tpsa(a_desc, mad_tpsa_default);
    t2.set(-b, a);

    const auto t3 = t1.clone();
    const auto t4 = t2.clone();

    BOOST_CHECK_CLOSE(t1.get(), b, 1e-12);
    BOOST_CHECK_CLOSE(t2.get(), a, 1e-12);
    BOOST_CHECK_CLOSE(t3.get(), b, 1e-12);
    BOOST_CHECK_CLOSE(t4.get(), a, 1e-12);

}

BOOST_AUTO_TEST_CASE(test02_get_set)
{
    const double a=355e0, b=113e0;

    auto a_desc = std::make_shared<gtpsa::desc>(1, 1);
    auto t1 = gtpsa::tpsa(a_desc, mad_tpsa_default);
    t1.set(0, b);

    BOOST_CHECK_CLOSE(t1.get(), b, 1e-12);
    t1.set(0, a);
    BOOST_CHECK_CLOSE(t1.get(), a, 1e-12);

}

BOOST_AUTO_TEST_CASE(test4_getorder)
{
    auto a_desc = std::make_shared<gtpsa::desc>(6, 4);
    auto t1 = gtpsa::tpsa(a_desc, mad_tpsa_default);

    t1.setv(1, {11, 13, 17, 19, 23, 29});
    t1.setv(1 + 6, {31, 37, 39, 41, 43, 47, 51});

    {
        // check that it works for constant objects
        const auto t = t1.clone();
        // t.print("nn", 0, false, 0);
    }

    auto t2 = t1.newFromThis();
    t2.rgetOrder(t1, 1);
    // t2.print("ord1", 0, false, 0);

    auto t3 = t1.newFromThis();
    t3.rgetOrder(t1, 2);
    // t3.print("ord1", 0, false, 0);



}

BOOST_AUTO_TEST_CASE(test5_set_variable)
{
    auto a_desc = std::make_shared<gtpsa::desc>(6, 1);
    const auto t1 = gtpsa::tpsa(a_desc, mad_tpsa_default);

    {
	const double a = 42e0;
	auto t2 = t1.clone();

	const int  iv = 2;
	t2.setVariable(a, iv, 0e0);

	std::vector<double> derivs(6);
	BOOST_CHECK_CLOSE(t2.get(), a, 1e-12);

	t2.getv(1, &derivs);
	// mad ng starts to count with 1 internally
	BOOST_CHECK_CLOSE(derivs[iv - 1], 1, 1e-12);



	// mad ng starts to count with 1 internally
	BOOST_CHECK_CLOSE(derivs[iv - 1], 1, 1e-12);


	std::vector<ord_t> power(6);
	power[iv-1] = 1;
	BOOST_CHECK_CLOSE(t2.get(power), 1, 1e-12);

    }

    {
	const double a = 355e0/113e0;
	auto t2 = t1.clone();

	const int  iv = 4;
	t2.setVariable(a, iv, 0e0);

	std::vector<double> derivs(6);
	BOOST_CHECK_CLOSE(t2.get(), a, 1e-12);

	t2.getv(1, &derivs);
	// mad ng starts to count with 1 internally
	BOOST_CHECK_CLOSE(derivs[iv - 1], 1, 1e-12);
    }


}


#if 0
BOOST_AUTO_TEST_CASE(test10_neg)
{
    auto a_desc = std::make_shared<gtpsa::desc>(1, 1);

    auto t1 = gtpsa::tpsa(a_desc, mad_tpsa_default);

    const double b1=2.0, r1 = -b1;

    t1.set(0e0, b1);

    auto t2 = - t1;
    BOOST_CHECK_CLOSE(t1.get(), b1, 1e-12);
    BOOST_CHECK_CLOSE(t2.get(), r1, 1e-12);

}
#endif

BOOST_AUTO_TEST_CASE(test11_add)
{
    auto a_desc = std::make_shared<gtpsa::desc>(1, 1);

    auto t1 = gtpsa::tpsa(a_desc, mad_tpsa_default);
    auto t2 = gtpsa::tpsa(t1, mad_tpsa_default);

    const double b1=2.0, b2=b1 * 3, r1 = b1 + b2;

    t1.set(0e0, b1);
    t2.set(0e0, b2);

    BOOST_CHECK_CLOSE(t1.get(), b1, 1e-12);
    BOOST_CHECK_CLOSE(t1.cst(), b1, 1e-12);
    BOOST_CHECK_CLOSE(t2.get(), b2, 1e-12);
    BOOST_CHECK_CLOSE(t2.cst(), b2, 1e-12);

    /* also check that constantness is correct */
    auto t3 = t1.clone();
    BOOST_CHECK_CLOSE(t3.get(), b1, 1e-12);
    const auto t4 = t2.clone();
    BOOST_CHECK_CLOSE(t4.get(), b2, 1e-12);
    t3 += t4;
    BOOST_CHECK_CLOSE(t4.get(), b2, 1e-12);
    BOOST_CHECK_CLOSE(t3.get(), r1, 1e-12);

    const auto t5 = t1.clone();
    const auto tc = t5 + t4;
    BOOST_CHECK_CLOSE(tc.get(), r1, 1e-12);


    {
	const auto t3 = t1.clone();
	auto tr =  gtpsa::tpsa(t3, mad_tpsa_default);
	// gtpsa::add(t3, b2, &tr);
	// BOOST_CHECK_CLOSE(tr.get(), r1, 1e-12);

    }

    // had problems with that ... test of low level function before
    auto t6 = t1.clone();
    BOOST_CHECK_CLOSE(t6.get(), b1, 1e-12);
    t6 += b2;
    BOOST_CHECK_CLOSE(t1.get(), b1, 1e-12);
    BOOST_CHECK_CLOSE(t6.get(), r1, 1e-12);

    auto t7 = t1 + b2;
    BOOST_CHECK_CLOSE(t1.get(), b1, 1e-12);
    BOOST_CHECK_CLOSE(t7.get(), r1, 1e-12);

    auto t8 = b2 + t1;
    BOOST_CHECK_CLOSE(t1.get(), b1, 1e-12);
    BOOST_CHECK_CLOSE(t7.get(), r1, 1e-12);

}



BOOST_AUTO_TEST_CASE(test12_sub)
{
    auto a_desc = std::make_shared<gtpsa::desc>(1, 1);

    auto t1 = gtpsa::tpsa(a_desc, mad_tpsa_default);
    auto t2 = gtpsa::tpsa(t1, mad_tpsa_same);

    const double b1=2.0, b2=b1 * 3, r1=b1-b2;
    t1.set(0e0, b1);
    t2.set(0e0, b2);

    {
	auto t3 = t1.clone();
	t3 -= t2;

	BOOST_CHECK_CLOSE(t2.get(), b2, 1e-12);
	BOOST_CHECK_CLOSE(t2.cst(), b2, 1e-12);
	BOOST_CHECK_CLOSE(t3.get(), r1, 1e-12);
	BOOST_CHECK_CLOSE(t3.cst(), r1, 1e-12);
    }
    {
	auto t3 = t1.clone();
	t3 -= b2;

	BOOST_CHECK_CLOSE(t3.get(), r1, 1e-12);
	BOOST_CHECK_CLOSE(t3.cst(), r1, 1e-12);
    }

    {
	auto t3 = t1 - b2;
	BOOST_CHECK_CLOSE(t3.get(), r1, 1e-12);
	BOOST_CHECK_CLOSE(t3.cst(), r1, 1e-12);
    }
    {
	auto t4 = t1.clone();
	auto t3 = b2 - t1;
	auto r2 = b2 - b1;
	BOOST_CHECK_CLOSE(t1.get(), b1, 1e-12);
	BOOST_CHECK_CLOSE(t4.get(), b1, 1e-12);
	BOOST_CHECK_CLOSE(t3.get(), r2, 1e-12);
	BOOST_CHECK_CLOSE(t3.cst(), r2, 1e-12);
    }

}


BOOST_AUTO_TEST_CASE(test13_mul)
{
    auto a_desc = std::make_shared<gtpsa::desc>(1, 1);

    auto t1 = gtpsa::tpsa(a_desc, mad_tpsa_default);
    auto t2 = gtpsa::tpsa(t1, mad_tpsa_same);

    const double b1=2.0, b2=b1 * 3, r1=b1*b2;
    t1.set(0e0, b1);
    t2.set(0e0, b2);

    {
	auto t3 = t1.clone();
	t3 *= t2;

	BOOST_CHECK_CLOSE(t2.get(), b2, 1e-12);
	BOOST_CHECK_CLOSE(t3.get(), r1, 1e-12);
    }

    {
	auto t3 = t1.clone();
	t3 *= b2;
	BOOST_CHECK_CLOSE(t3.get(), r1, 1e-12);
    }
    {
	auto t3 = t1 * b2;
	BOOST_CHECK_CLOSE(t3.get(), r1, 1e-12);
    }
    {
	auto t3 = b2 * t1;
	BOOST_CHECK_CLOSE(t3.get(), r1, 1e-12);
    }
}

BOOST_AUTO_TEST_CASE(test14_div)
{
    auto a_desc = std::make_shared<gtpsa::desc>(1, 1);

    auto t1 = gtpsa::tpsa(a_desc, mad_tpsa_default);
    auto t2 = gtpsa::tpsa(t1, mad_tpsa_same);

    const double b2=2.0, b1=b2 * 3, r1=b1/b2;
    t1.set(0e0, b1);
    t2.set(0e0, b2);

    {
	auto t3 = t1.clone();
	t3 /= t2;

	BOOST_CHECK_CLOSE(t2.get(), b2, 1e-12);
	BOOST_CHECK_CLOSE(t3.get(), r1, 1e-12);
    }

    {
	auto t3 = t1 / t2;
	BOOST_CHECK_CLOSE(t2.get(), b2, 1e-12);
	BOOST_CHECK_CLOSE(t3.get(), r1, 1e-12);
    }
    {
	auto t3 = t1.clone();
	t3 /=  b2;

	BOOST_CHECK_CLOSE(t2.get(), b2, 1e-12);
	BOOST_CHECK_CLOSE(t3.get(), r1, 1e-12);
    }

    {
	auto t3 = t1 / b2;
	auto r1 = b1 / b2;
	BOOST_CHECK_CLOSE(t2.get(), b2, 1e-12);
	BOOST_CHECK_CLOSE(t3.get(), r1, 1e-12);
    }

    {
	auto t3 = b2 / t1;
	auto r2 = b2 / b1;
	BOOST_CHECK_CLOSE(t2.get(), b2, 1e-12);
	BOOST_CHECK_CLOSE(t3.get(), r2, 1e-12);
    }

}

BOOST_AUTO_TEST_CASE(test20_assign_double)
{
    const double b=42e0;

    auto a_desc = std::make_shared<gtpsa::desc>(1, 1);
    auto t = gtpsa::tpsa(a_desc, mad_tpsa_default);
    t = b;
    BOOST_CHECK_CLOSE(t.get(), b, 1e-12);

}


static std::array<gtpsa::tpsa, 2>
create_t1_t2(void)
{
    auto a_desc = std::make_shared<gtpsa::desc>(6, 6);

    auto t1 = gtpsa::tpsa(a_desc, 2);
    auto t2 = gtpsa::tpsa(t1, mad_tpsa_same);

    // following  x and y

    std::vector<double> v(6);
    for(auto& e : v) e = 0e0;
    v[0] = 1;
    t1.setv(1, v);

    for(auto& e : v) e = 0e0;
    v[2] = 1;
    t2.setv(1, v);

    std::array<gtpsa::tpsa, 2> arr{t1, t2};
    return arr;
}

BOOST_AUTO_TEST_CASE(test30_add_first_order)
{
    auto arr = create_t1_t2();
    auto& t1 = arr[0];
    auto& t2 = arr[1];

    auto t3 = t1 + t2;

    auto nv = t1.getDescription()->getNv(0,0,0);
    std::vector<num_t> v(nv);
    t3.getv(1, &v);

    BOOST_CHECK( nv >= 6);
    BOOST_CHECK_CLOSE(v[0],  1, 1e-12);
    BOOST_CHECK_SMALL(v[1],     1e-12);
    BOOST_CHECK_CLOSE(v[2],  1, 1e-12);
    BOOST_CHECK_SMALL(v[3],     1e-12);
    BOOST_CHECK_SMALL(v[4],     1e-12);
    BOOST_CHECK_SMALL(v[5],     1e-12);
}


BOOST_AUTO_TEST_CASE(test31_sub_first_order)
{
    auto arr = create_t1_t2();
    auto& t1 = arr[0];
    auto& t2 = arr[1];

    auto t3 = t1 - t2;

    auto nv = t1.getDescription()->getNv(0,0,0);
    std::vector<num_t> v(nv);
    t3.getv(1, &v);

    BOOST_CHECK( nv >= 6);
    BOOST_CHECK_CLOSE(v[0],  1, 1e-12);
    BOOST_CHECK_SMALL(v[1],     1e-12);
    BOOST_CHECK_CLOSE(v[2], -1, 1e-12);
    BOOST_CHECK_SMALL(v[3],     1e-12);
    BOOST_CHECK_SMALL(v[4],     1e-12);
    BOOST_CHECK_SMALL(v[5],     1e-12);

}

BOOST_AUTO_TEST_CASE(test32_mul_first_order)
{
    auto arr = create_t1_t2();
    auto& t1 = arr[0];
    auto& t2 = arr[1];

    auto t3 = t1 * t2;

    auto nv = t1.getDescription()->getNv(0,0,0);
    std::vector<num_t> v(nv);
    t3.getv(1, &v);

    BOOST_CHECK( nv >= 6);
    BOOST_CHECK_SMALL(v[0],     1e-12);
    BOOST_CHECK_SMALL(v[1],     1e-12);
    BOOST_CHECK_SMALL(v[2],     1e-12);
    BOOST_CHECK_SMALL(v[3],     1e-12);
    BOOST_CHECK_SMALL(v[4],     1e-12);
    BOOST_CHECK_SMALL(v[5],     1e-12);

}

BOOST_AUTO_TEST_CASE(test33_div_first_order)
{
    auto arr = create_t1_t2();
    auto& t1 = arr[0];
    auto& t2 = arr[1];

    return;

    auto t3 = t1 / t2;

    t3.print("t1 / t2", 0, 0, 0);

}

BOOST_AUTO_TEST_CASE(test40_radd_first_order_dbl)
{
    auto arr = create_t1_t2();
    auto& t1 = arr[0];
    auto nv = t1.getDescription()->getNv(0,0,0);
    std::vector<num_t> v(nv);

    t1.getv(1, &v);

    BOOST_CHECK( nv >= 6);
    BOOST_CHECK_CLOSE(v[0],       1, 1e-12);
    BOOST_CHECK_SMALL(v[1],          1e-12);
    BOOST_CHECK_SMALL(v[2],          1e-12);
    BOOST_CHECK_SMALL(v[3],          1e-12);
    BOOST_CHECK_SMALL(v[4],          1e-12);
    BOOST_CHECK_SMALL(v[5],          1e-12);

    const double val = 40;

    t1 += val;

    t1.getv(1, &v);

    BOOST_CHECK_CLOSE(t1.cst(), val, 1e-12);

    BOOST_CHECK( nv >= 6);
    BOOST_CHECK_CLOSE(v[0],       1, 1e-12);
    BOOST_CHECK_SMALL(v[1],          1e-12);
    BOOST_CHECK_SMALL(v[2],          1e-12);
    BOOST_CHECK_SMALL(v[3],          1e-12);
    BOOST_CHECK_SMALL(v[4],          1e-12);
    BOOST_CHECK_SMALL(v[5],          1e-12);

}

BOOST_AUTO_TEST_CASE(test41_add_first_order_dbl)
{
    auto arr = create_t1_t2();
    auto& t1 = arr[0];

    const double val = 40;

    auto t3 = val + t1;

    auto nv = t1.getDescription()->getNv(0,0,0);
    std::vector<num_t> v(nv);
    t3.getv(1, &v);


    BOOST_CHECK_CLOSE(t3.cst(), val, 1e-12);

    BOOST_CHECK( nv >= 6);
    BOOST_CHECK_CLOSE(v[0],      1,  1e-12);
    BOOST_CHECK_SMALL(v[1],          1e-12);
    BOOST_CHECK_SMALL(v[2],          1e-12);
    BOOST_CHECK_SMALL(v[3],          1e-12);
    BOOST_CHECK_SMALL(v[4],          1e-12);
    BOOST_CHECK_SMALL(v[5],          1e-12);

}


BOOST_AUTO_TEST_CASE(test42_add_first_order_dbl)
{
    auto arr = create_t1_t2();
    auto& t1 = arr[0];
    auto nv = t1.getDescription()->getNv(0,0,0);
    std::vector<num_t> v(nv);

    t1.getv(1, &v);

    BOOST_CHECK( nv >= 6);
    BOOST_CHECK_CLOSE(v[0],       1, 1e-12);
    BOOST_CHECK_SMALL(v[1],          1e-12);
    BOOST_CHECK_SMALL(v[2],          1e-12);
    BOOST_CHECK_SMALL(v[3],          1e-12);
    BOOST_CHECK_SMALL(v[4],          1e-12);
    BOOST_CHECK_SMALL(v[5],          1e-12);

    const double val = 40;

    auto t3 = t1 + val;

    t3.getv(1, &v);

    BOOST_CHECK_CLOSE(t3.cst(), val, 1e-12);

    BOOST_CHECK( nv >= 6);
    BOOST_CHECK_CLOSE(v[0],       1, 1e-12);
    BOOST_CHECK_SMALL(v[1],          1e-12);
    BOOST_CHECK_SMALL(v[2],          1e-12);
    BOOST_CHECK_SMALL(v[3],          1e-12);
    BOOST_CHECK_SMALL(v[4],          1e-12);
    BOOST_CHECK_SMALL(v[5],          1e-12);

}

BOOST_AUTO_TEST_CASE(test45_sub_first_order_dbl)
{
    const double val = 42, cst = 355;

    auto arr = create_t1_t2();
    auto& t1 = arr[0];
    t1.set(0, cst);


    auto t3 = val - t1;

    auto nv = t1.getDescription()->getNv(0,0,0);
    std::vector<num_t> v(nv);
    t3.getv(1, &v);

    BOOST_CHECK_CLOSE(t3.cst(),  val - cst, 1e-12);
    BOOST_CHECK_CLOSE(v[0],           -1,   1e-12);
    BOOST_CHECK_SMALL(v[1],                 1e-12);
    BOOST_CHECK_SMALL(v[2],                 1e-12);
    BOOST_CHECK_SMALL(v[3],                 1e-12);
    BOOST_CHECK_SMALL(v[4],                 1e-12);
    BOOST_CHECK_SMALL(v[5],                 1e-12);

}


BOOST_AUTO_TEST_CASE(test46_sub_first_order_dbl)
{
    const double val = 42, cst=355;

    auto arr = create_t1_t2();
    auto& t1 = arr[0];

    t1.set(0, cst);

    auto t3 = t1 - val;

    auto nv = t1.getDescription()->getNv(0,0,0);
    std::vector<num_t> v(nv);
    t3.getv(1, &v);

    BOOST_CHECK_CLOSE(t3.cst(), cst -val, 1e-12);
    BOOST_CHECK_CLOSE(v[0],            1, 1e-12);
    BOOST_CHECK_SMALL(v[1],               1e-12);
    BOOST_CHECK_SMALL(v[2],               1e-12);
    BOOST_CHECK_SMALL(v[3],               1e-12);
    BOOST_CHECK_SMALL(v[4],               1e-12);
    BOOST_CHECK_SMALL(v[5],               1e-12);

}

BOOST_AUTO_TEST_CASE(test50_mul_first_order_dbl)
{
    const double val = 1.0/113.0, cst=355;

    auto arr = create_t1_t2();
    auto& t1 = arr[0];

    t1.set(0, cst);

    auto t3 = t1 * val;

    auto nv = t1.getDescription()->getNv(0,0,0);
    std::vector<num_t> v(nv);
    t3.getv(1, &v);

    BOOST_CHECK_CLOSE(t3.cst(), cst * val, 1e-12);
    BOOST_CHECK_CLOSE(t3.cst(), M_PI,      1e-5 );
    BOOST_CHECK_CLOSE(v[0],     val,       1e-12);
    BOOST_CHECK_SMALL(v[1],                1e-12);
    BOOST_CHECK_SMALL(v[2],                1e-12);
    BOOST_CHECK_SMALL(v[3],                1e-12);
    BOOST_CHECK_SMALL(v[4],                1e-12);
    BOOST_CHECK_SMALL(v[5],                1e-12);

}

BOOST_AUTO_TEST_CASE(test60_deriv)
{
    auto d = std::make_shared<gtpsa::desc>(6, 3);
    auto t1 = gtpsa::tpsa(d, 2);
    auto t2 = t1.newFromThis();

    t1.rderiv(t1, 1);
    // t1.print();

    BOOST_CHECK(0 ==0);
}

BOOST_AUTO_TEST_CASE(test70_integ)
{
    auto d = std::make_shared<gtpsa::desc>(6, 3);
    auto t1 = gtpsa::tpsa(d, 2);
    auto t2 = t1.newFromThis();

    t1.set(0, 20);
    t1.setv(0, {-1, 1, 2, 3, 4, 5, 6});
    t1.setv(1 + 6, {7, 11, 13, 17, 19, 23, 29, 31, 37, 41});
    // t1.print("start");

    t1.rinteg(t1, 3);
    // t1.print("integrated");

}

BOOST_AUTO_TEST_CASE(test60_compare){
    auto a_desc = std::make_shared<gtpsa::desc>(1, 1);
    auto t1 = gtpsa::tpsa(a_desc, gtpsa::mad::default_);

    // mainly compile checks
    BOOST_CHECK(t1 == t1);
    BOOST_CHECK(t1 == 0);
    BOOST_CHECK(0 == t1);

}

BOOST_AUTO_TEST_CASE(test80_compare){
    auto a_desc = std::make_shared<gtpsa::desc>(6, 3, 6, 2);
    auto t1 = gtpsa::tpsa(a_desc, gtpsa::mad::default_);
    auto t3 = gtpsa::tpsa(a_desc, gtpsa::mad::default_);
    t1.setVariable(42, 3);
    t1.setv(1, {2, 3, 5, 7, 11, 13, 0, 0, 0, 17, 19, 23});
    t3.setVariable(1./42 * 1./42., 5);

    const auto t2 = t1 * t1 * t3;
    t2.print("t3", 1e-12);
    fflush(stdout);
    std::cout << std::endl;
    t2.getCoefficients();
}

BOOST_AUTO_TEST_CASE(test90_atan2){
    auto a_desc = std::make_shared<gtpsa::desc>(6, 1);
    gtpsa::tpsa x(a_desc, gtpsa::mad::default_), y(a_desc, gtpsa::mad::default_);

    x.set(0, -1);
    y.set(0, -1);

    auto angle = gtpsa::atan2(x, y);
    BOOST_CHECK_CLOSE(angle.cst(), -135e0/180e0*M_PI, 1e-12);
}

BOOST_AUTO_TEST_CASE(test100_pow){
    auto a_desc = std::make_shared<gtpsa::desc>(6, 1);
    gtpsa::tpsa x(a_desc, 1);

    x.set(0, 2);
    BOOST_CHECK_CLOSE(x.cst(), 2, 1e-12);
    {
	gtpsa::tpsa p(a_desc, 1);
	p.set(0, 2);
	auto y =  gtpsa::pow(x, p);
	BOOST_CHECK_CLOSE(y.cst(), 4, 1e-12);
	BOOST_CHECK_CLOSE(x.cst(), 2, 1e-12);
    }
    {
	auto y =  gtpsa::pow(x, 2);
	BOOST_CHECK_CLOSE(y.cst(), 4, 1e-12);
	BOOST_CHECK_CLOSE(x.cst(), 2, 1e-12);
    }
    {
	auto y =  gtpsa::pow(x, 2e0);
	BOOST_CHECK_CLOSE(y.cst(), 4, 1e-12);
	BOOST_CHECK_CLOSE(x.cst(), 2, 1e-12);
    }

}
