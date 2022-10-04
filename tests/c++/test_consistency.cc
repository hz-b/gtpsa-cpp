#define BOOST_TEST_MODULE gtpsa_consistency
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <gtpsa/desc.hpp>

BOOST_AUTO_TEST_CASE(test10_enum)
{
    int default_= int(gtpsa::init::default_), same = int(gtpsa::init::same);


    /* copied manually ... better check these are identical */
    BOOST_CHECK_EQUAL(default_, mad_tpsa_default);
    BOOST_CHECK_EQUAL(same,     mad_tpsa_same);
}
