#define BOOST_TEST_MODULE arithmetic
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <gtpsa.hpp>

BOOST_AUTO_TEST_CASE(test10_add)
{
    auto a_desc = gtpsa::desc(1, 0);

    auto t1 = gtpsa::tpsa(a_desc, mad_tpsa_default);
    auto t2 = gtpsa::tpsa(t1, mad_tpsa_default);

    t2 += t1;
}
