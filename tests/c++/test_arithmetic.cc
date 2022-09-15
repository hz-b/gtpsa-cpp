#define BOOST_TEST_MODULE gtpsa_arithmetic
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <gtpsa.hpp>

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

BOOST_AUTO_TEST_CASE(test11_add)
{
    auto a_desc = gtpsa::desc(1, 0);

    auto t1 = gtpsa::tpsa(a_desc, mad_tpsa_default);
    auto t2 = gtpsa::tpsa(t1, mad_tpsa_default);

    const double a1=1.0, b1=2.0, b2=b1 * 3, r1=b1+b2;
    t1.set(a1, b1);
    t2.set(0e0, b2);

    BOOST_CHECK_CLOSE(t1.get(), b1, 1e-12);
    BOOST_CHECK_CLOSE(t1.cst(), b1, 1e-12);
    BOOST_CHECK_CLOSE(t2.get(), b2, 1e-12);
    BOOST_CHECK_CLOSE(t2.cst(), b2, 1e-12);

    /* also check that constantness is correct */
    const auto t3 = t1.clone();
    const auto t4 = t2.clone();
    t1 += t4;
    BOOST_CHECK_CLOSE(t2.get(), b2, 1e-12);
    BOOST_CHECK_CLOSE(t2.cst(), b2, 1e-12);
    BOOST_CHECK_CLOSE(t1.get(), r1, 1e-12);
    BOOST_CHECK_CLOSE(t1.cst(), r1, 1e-12);

    const auto tc = t3 + t4;
    BOOST_CHECK_CLOSE(tc.get(), r1, 1e-12);
    BOOST_CHECK_CLOSE(tc.cst(), r1, 1e-12);
}


BOOST_AUTO_TEST_CASE(test12_sub)
{
    auto a_desc = gtpsa::desc(1, 0);

    auto t1 = gtpsa::tpsa(a_desc, mad_tpsa_default);
    auto t2 = gtpsa::tpsa(t1, mad_tpsa_same);

    const double a1=1.0, b1=2.0, b2=b1 * 3, r1=b1-b2;
    t1.set(a1, b1);
    t2.set(0e0, b2);

    t1 -= t2;

    BOOST_CHECK_CLOSE(t2.get(), b2, 1e-12);
    BOOST_CHECK_CLOSE(t2.cst(), b2, 1e-12);
    BOOST_CHECK_CLOSE(t1.get(), r1, 1e-12);
    BOOST_CHECK_CLOSE(t1.cst(), r1, 1e-12);

}


BOOST_AUTO_TEST_CASE(test13_mul)
{
    auto a_desc = gtpsa::desc(1, 0);

    auto t1 = gtpsa::tpsa(a_desc, mad_tpsa_default);
    auto t2 = gtpsa::tpsa(t1, mad_tpsa_same);

    const double a1=1.0, b1=2.0, b2=b1 * 3, r1=b1*b2;
    t1.set(a1, b1);
    t2.set(0e0, b2);

    t1 *= t2;

    BOOST_CHECK_CLOSE(t2.get(), b2, 1e-12);
    BOOST_CHECK_CLOSE(t2.cst(), b2, 1e-12);
    BOOST_CHECK_CLOSE(t1.get(), r1, 1e-12);
    BOOST_CHECK_CLOSE(t1.cst(), r1, 1e-12);

}

BOOST_AUTO_TEST_CASE(test14_mul)
{
    auto a_desc = gtpsa::desc(1, 0);

    auto t1 = gtpsa::tpsa(a_desc, mad_tpsa_default);
    auto t2 = gtpsa::tpsa(t1, mad_tpsa_same);

    const double a1=1.0, b1=2.0, b2=b1 * 3, r1=b1/b2;
    t1.set(a1, b1);
    t2.set(0e0, b2);

    t1 /= t2;

    BOOST_CHECK_CLOSE(t2.get(), b2, 1e-12);
    BOOST_CHECK_CLOSE(t2.cst(), b2, 1e-12);
    BOOST_CHECK_CLOSE(t1.get(), r1, 1e-12);
    BOOST_CHECK_CLOSE(t1.cst(), r1, 1e-12);

}
