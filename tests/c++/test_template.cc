#define BOOST_TEST_MODULE gtpsa_template
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <gtpsa/tpsa.hpp>
#include <gtpsa/utils.hpp>

BOOST_AUTO_TEST_CASE(test20_tpsa_cst)
{
    const double a=0e0, b=42e0;

    auto a_desc = gtpsa::desc(1, 0);
    auto t1 = gtpsa::tpsa(a_desc, mad_tpsa_default);
    t1.set(a, b);

    BOOST_CHECK_CLOSE(t1.cst(), b, 1e-12);
    BOOST_CHECK_CLOSE(gtpsa::cst(t1), b, 1e-12);
    BOOST_CHECK_CLOSE(gtpsa::cst(b), b, 1e-12);
}

// a bit paranoic
BOOST_AUTO_TEST_CASE(test20_double_cst)
{
    const double b=42e0;

    BOOST_CHECK_CLOSE(gtpsa::cst(b), b, 1e-12);
}
