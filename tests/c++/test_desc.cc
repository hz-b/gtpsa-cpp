#define BOOST_TEST_MODULE gtpsa_template
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <gtpsa/desc.hpp>

// Simple check for some methods

BOOST_AUTO_TEST_CASE(test20_tpsa_isvaild_str)
{
    const int nv = 5, no =7;
    auto a_desc = gtpsa::desc(nv, no);

    // BOOST_CHECK_EQUAL(a_desc.maxLen(), nv * no);
    BOOST_CHECK_EQUAL(a_desc.maxOrd(), no);

    BOOST_CHECK_EQUAL(a_desc.isvalid("00"), true );
    BOOST_CHECK_EQUAL(a_desc.isvalid("01"), true );
    BOOST_CHECK_EQUAL(a_desc.isvalid("02"), true );
    BOOST_CHECK_EQUAL(a_desc.isvalid("03"), true );
    BOOST_CHECK_EQUAL(a_desc.isvalid("04"), true );
    BOOST_CHECK_EQUAL(a_desc.isvalid("05"), true );
    BOOST_CHECK_EQUAL(a_desc.isvalid("06"), true );
    BOOST_CHECK_EQUAL(a_desc.isvalid("07"), true );
    // invalid
    BOOST_CHECK_EQUAL(a_desc.isvalid("08"), false);

    BOOST_CHECK_EQUAL(a_desc.isvalid("10"), true );
    BOOST_CHECK_EQUAL(a_desc.isvalid("11"), true );
    BOOST_CHECK_EQUAL(a_desc.isvalid("12"), true );
    BOOST_CHECK_EQUAL(a_desc.isvalid("13"), true );
    BOOST_CHECK_EQUAL(a_desc.isvalid("14"), true );
    BOOST_CHECK_EQUAL(a_desc.isvalid("15"), true );
    BOOST_CHECK_EQUAL(a_desc.isvalid("16"), true );
    // invalid
    BOOST_CHECK_EQUAL(a_desc.isvalid("17"), false);


    BOOST_CHECK_EQUAL(a_desc.isvalid("20"), true );
    BOOST_CHECK_EQUAL(a_desc.isvalid("21"), true );
    BOOST_CHECK_EQUAL(a_desc.isvalid("22"), true );
    BOOST_CHECK_EQUAL(a_desc.isvalid("23"), true );
    BOOST_CHECK_EQUAL(a_desc.isvalid("24"), true );
    BOOST_CHECK_EQUAL(a_desc.isvalid("25"), true );
    // invalid
    BOOST_CHECK_EQUAL(a_desc.isvalid("26"), false);
    BOOST_CHECK_EQUAL(a_desc.isvalid("27"), false);


    BOOST_CHECK_EQUAL(a_desc.isvalid("30"), true );
    BOOST_CHECK_EQUAL(a_desc.isvalid("31"), true );
    BOOST_CHECK_EQUAL(a_desc.isvalid("32"), true );
    BOOST_CHECK_EQUAL(a_desc.isvalid("33"), true );
    BOOST_CHECK_EQUAL(a_desc.isvalid("34"), true );
    // invalid
    BOOST_CHECK_EQUAL(a_desc.isvalid("35"), false);
    BOOST_CHECK_EQUAL(a_desc.isvalid("36"), false);
    BOOST_CHECK_EQUAL(a_desc.isvalid("37"), false);


    BOOST_CHECK_EQUAL(a_desc.isvalid("40"), true );
    BOOST_CHECK_EQUAL(a_desc.isvalid("41"), true );
    BOOST_CHECK_EQUAL(a_desc.isvalid("42"), true );
    BOOST_CHECK_EQUAL(a_desc.isvalid("43"), true );
    // invalid
    BOOST_CHECK_EQUAL(a_desc.isvalid("44"), false);
    BOOST_CHECK_EQUAL(a_desc.isvalid("45"), false);
    BOOST_CHECK_EQUAL(a_desc.isvalid("46"), false);
    BOOST_CHECK_EQUAL(a_desc.isvalid("47"), false);


    BOOST_CHECK_EQUAL(a_desc.isvalid("50"), true );
    BOOST_CHECK_EQUAL(a_desc.isvalid("51"), true );
    BOOST_CHECK_EQUAL(a_desc.isvalid("52"), true );
    //invalid
    BOOST_CHECK_EQUAL(a_desc.isvalid("53"), false);
    BOOST_CHECK_EQUAL(a_desc.isvalid("54"), false);
    BOOST_CHECK_EQUAL(a_desc.isvalid("55"), false);
    BOOST_CHECK_EQUAL(a_desc.isvalid("56"), false);
    BOOST_CHECK_EQUAL(a_desc.isvalid("57"), false);
    BOOST_CHECK_EQUAL(a_desc.isvalid("58"), false);


    BOOST_CHECK_EQUAL(a_desc.isvalid("60"), true );
    BOOST_CHECK_EQUAL(a_desc.isvalid("61"), true );
    //invalid
    BOOST_CHECK_EQUAL(a_desc.isvalid("62"), false);
    BOOST_CHECK_EQUAL(a_desc.isvalid("63"), false);
    BOOST_CHECK_EQUAL(a_desc.isvalid("64"), false);
    BOOST_CHECK_EQUAL(a_desc.isvalid("65"), false);


    BOOST_CHECK_EQUAL(a_desc.isvalid("70"), true );
    // invalid
    BOOST_CHECK_EQUAL(a_desc.isvalid("71"), false);
    BOOST_CHECK_EQUAL(a_desc.isvalid("72"), false);


}


BOOST_AUTO_TEST_CASE(test20_tpsa_isvaildsm)
{
    const int nv = 5, no =7;
    auto a_desc = gtpsa::desc(nv, no);

    {
    std::vector<idx_t> v={0, 1};
    BOOST_CHECK_EQUAL(a_desc.isvalidsm(v), true);
    }
    {
	std::vector<idx_t> v={1, 0};
	BOOST_CHECK_EQUAL(a_desc.isvalidsm(v), true);
    }
    BOOST_CHECK_EQUAL(a_desc.isvalidsm(std::vector<idx_t> {4, 6}), true);

    BOOST_CHECK_EQUAL(a_desc.isvalidsm(std::vector<idx_t> {4, 6}), true);
    BOOST_CHECK_EQUAL(a_desc.isvalidsm(std::vector<idx_t> {4, 6}), true);
    BOOST_CHECK_EQUAL(a_desc.isvalidsm(std::vector<idx_t> {4, 6}), true);

}
