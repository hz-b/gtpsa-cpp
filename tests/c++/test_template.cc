#define BOOST_TEST_MODULE gtpsa_template
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <gtpsa/tpsa.hpp>
#include <gtpsa/utils.hpp>
#include <vector>

BOOST_AUTO_TEST_CASE(test20_tpsa_cst)
{
    const double a=0e0, b=42e0;

    auto a_desc = std::make_shared<gtpsa::desc>(1, 0);
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

BOOST_AUTO_TEST_CASE(test30_tpsa_get)
{
    const double a=0e0, b=42e0;
    const int nv = 6, no = 1;

    auto a_desc = std::make_shared<gtpsa::desc>(nv, no);
    auto t1 = gtpsa::tpsa(a_desc, mad_tpsa_default);
    t1.set(a, b);

    BOOST_CHECK_CLOSE(t1.cst(), b, 1e-12);
    BOOST_CHECK_CLOSE(gtpsa::cst(t1), b, 1e-12);
    BOOST_CHECK_CLOSE(gtpsa::cst(b), b, 1e-12);

    BOOST_CHECK_CLOSE(t1.get(0), b, 1e-12);

}

// test how a std::vector can be constructed
BOOST_AUTO_TEST_CASE(test40_ss_vect_tpsa)
{
    const int nv = 6, no = 1;
    const double a = 0e0, b = 42e0;

    auto a_desc = std::make_shared<gtpsa::desc>(nv, no);
    auto t = gtpsa::tpsa(a_desc, mad_tpsa_default);

    {
        std::vector<gtpsa::tpsa> vec(nv, t);

        vec[0].set(a, b);
        BOOST_CHECK_CLOSE(vec[0].get(), b, 1e-12);

        // vector constructor copied t1...
        BOOST_CHECK_SMALL(vec[1].get(),    1e-12);
        BOOST_CHECK_SMALL(vec[2].get(),    1e-12);
        BOOST_CHECK_SMALL(vec[3].get(),    1e-12);
    }

    {
        auto& tref = t;
        std::vector<gtpsa::tpsa> vec(nv, tref);

        vec[0].set(a, b);
        BOOST_CHECK_CLOSE(vec[0].get(), b, 1e-12);

        // vector constructor copied t1...
        BOOST_CHECK_SMALL(vec[1].get(),    1e-12);
        BOOST_CHECK_SMALL(vec[2].get(),    1e-12);
        BOOST_CHECK_SMALL(vec[3].get(),    1e-12);

    }

}
