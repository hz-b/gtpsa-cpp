#define BOOST_TEST_MODULE tpsa
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <gtpsa/utils.hpp>
#include <gtpsa/tpsa.hpp>

BOOST_AUTO_TEST_CASE(test10_variant_ctpsa_alloc)
{
    auto a_desc = std::make_shared<gtpsa::desc>(6, 2);

    // allocation should work for both
    gtpsa::ctpsa t0(a_desc, 0), t1(a_desc, 1);
}

BOOST_AUTO_TEST_CASE(test10_variant_ctpsa_set_var)
{
    auto a_desc = std::make_shared<gtpsa::desc>(6, 2);

    // allocation should work for both
    gtpsa::ctpsa t0(a_desc, 0), t1(a_desc, 1);
}
