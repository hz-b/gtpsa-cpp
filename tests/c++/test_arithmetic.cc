#define BOOST_TEST_MODULE gtpsa_arithmetic
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <gtpsa/tpsa.hpp>

BOOST_AUTO_TEST_CASE(test00_set)
{
    const double a=0e0, b=42e0;

    auto a_desc = gtpsa::desc(1, 0);
    auto t1 = gtpsa::tpsa(a_desc, mad_tpsa_default);

    t1.set(a, b);
    BOOST_CHECK_CLOSE(t1.get(), b, 1e-12);
    t1.set(a, b);
    BOOST_CHECK_CLOSE(t1.get(), b, 1e-12);

}

BOOST_AUTO_TEST_CASE(test01_clone)
{
    const double a=355e0, b=113e0;

    auto a_desc = gtpsa::desc(1, 0);
    auto t1 = gtpsa::tpsa(a_desc, mad_tpsa_default);
    t1.set(a, b);

    const auto t2 = t1.clone();

    BOOST_CHECK_CLOSE(t1.get(), b, 1e-12);
    BOOST_CHECK_CLOSE(t2.get(), b, 1e-12);

}

BOOST_AUTO_TEST_CASE(test02_get_set)
{
    const double a=355e0, b=113e0;

    auto a_desc = gtpsa::desc(1, 0);
    auto t1 = gtpsa::tpsa(a_desc, mad_tpsa_default);
    t1.set(a, b);

}

BOOST_AUTO_TEST_CASE(test10_neg)
{
    auto a_desc = gtpsa::desc(1, 0);

    auto t1 = gtpsa::tpsa(a_desc, mad_tpsa_default);

    const double b1=2.0, r1 = -b1;

    t1.set(0e0, b1);

    auto t2 = - t1;
    BOOST_CHECK_CLOSE(t1.get(), b1, 1e-12);
    BOOST_CHECK_CLOSE(t2.get(), r1, 1e-12);

}

BOOST_AUTO_TEST_CASE(test11_add)
{
    auto a_desc = gtpsa::desc(1, 0);

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
	gtpsa::add_d(t3, b2, &tr);
	BOOST_CHECK_CLOSE(tr.get(), r1, 1e-12);


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
    auto a_desc = gtpsa::desc(1, 0);

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
    auto a_desc = gtpsa::desc(1, 0);

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
    auto a_desc = gtpsa::desc(1, 0);

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
