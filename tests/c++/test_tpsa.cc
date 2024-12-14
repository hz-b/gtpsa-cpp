#define BOOST_TEST_MODULE tpsa
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <gtpsa/utils.hpp>
#include <gtpsa/tpsa.hpp>


BOOST_AUTO_TEST_CASE(test10_variant_tpsa_alloc)
{
    auto a_desc = std::make_shared<gtpsa::desc>(6, 2);

    gtpsa::tpsa t0(a_desc, 0), t1(a_desc, 1);
    // t.setVariable(355 / 113, 2, 0);

}

///> @todo make ensure raise c++ exceptions ...
BOOST_AUTO_TEST_CASE(test10_variant_tpsa_set_var)
{
    auto a_desc = std::make_shared<gtpsa::desc>(6, 2);

    gtpsa::tpsa t1(a_desc, 1);
    // iv default was set 0, but needs to be 1
    t1.setVariable(355 / 113, 1);
    // iv default was set 0, but needs to be 1
    t1.setVariable(355 / 113);

    // the following will fail, but mad_tpsa library will call ensure ...
    // this does not end up here yet ...
    // t1.setVariable(355 / 113, 2, 0);
    // BOOST_CHECK_THROW(t0.setVariable(355 / 113, 0, 2), std::runtime_error);
}
