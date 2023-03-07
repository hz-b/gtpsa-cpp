#define BOOST_TEST_MODULE gtpsa_template
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <gtpsa/tpsa_double_variant.hpp>
#include <memory>
#include <array>
#include <utility>

BOOST_AUTO_TEST_CASE(test10_variant_tpsa_alloc)
{
    auto a_desc = std::make_shared<gtpsa::desc>(6, 2);

    auto t = gtpsa::tpsa(a_desc, 1);
    auto td = gtpsa::TpsaOrDouble(t);

}

BOOST_AUTO_TEST_CASE(test20_variant_double_add)
{
    double a = 3, b = 5;
    auto ta = gtpsa::TpsaOrDouble(a);
    auto tb = gtpsa::TpsaOrDouble(b);
    auto tc = ta + tb;

    BOOST_CHECK_CLOSE(tc.cst(), a + b, 1e-12);
}

BOOST_AUTO_TEST_CASE(test21_variant_double_sub)
{
    double a = 3, b = 5;
    auto ta = gtpsa::TpsaOrDouble(a);
    auto tb = gtpsa::TpsaOrDouble(b);
    auto tc = ta - tb;

    BOOST_CHECK_CLOSE(tc.cst(), a - b, 1e-12);
}

BOOST_AUTO_TEST_CASE(test11_variant_double_mul)
{
    double a = 3, b = 5;
    auto ta = gtpsa::TpsaOrDouble(a);
    auto tb = gtpsa::TpsaOrDouble(b);
    auto tc = ta * tb;

    BOOST_CHECK_CLOSE(tc.cst(), a * b, 1e-12);
}

BOOST_AUTO_TEST_CASE(test11_variant_double_div)
{
    double a = 3, b = 5;
    auto ta = gtpsa::TpsaOrDouble(a);
    auto tb = gtpsa::TpsaOrDouble(b);
    auto tc = ta / tb;

    BOOST_CHECK_CLOSE(tc.cst(), a / b, 1e-12);
}
static std::array<gtpsa::TpsaOrDouble, 2> create_tab(const double a, const double b)
{
    auto desc = std::make_shared<gtpsa::desc>(6,1);
    auto ta = gtpsa::tpsa(desc, 1);
    auto tb = gtpsa::tpsa(desc, 1);
    ta += a; tb += b;
    auto da = gtpsa::TpsaOrDouble(ta);
    auto db = gtpsa::TpsaOrDouble(tb);
    BOOST_CHECK_CLOSE(da.cst(), a, 1e-12);
    BOOST_CHECK_CLOSE(db.cst(), b, 1e-12);

    std::array<gtpsa::TpsaOrDouble, 2> r ({std::move(da), std::move(db)});
    return r;
}

BOOST_AUTO_TEST_CASE(test30_variant_tpsa_add)
{
    double a = 7, b = -11;
    auto tmp = create_tab(a, b);

    auto dc = tmp[0] + tmp[1];
    BOOST_CHECK_CLOSE(dc.cst(), a + b, 1e-12);
}

BOOST_AUTO_TEST_CASE(test30_variant_tpsa_sub)
{
    double a = 7, b = -11;
    auto tmp = create_tab(a, b);

    auto dc = tmp[0] - tmp[1];
    BOOST_CHECK_CLOSE(dc.cst(), a - b, 1e-12);
}

BOOST_AUTO_TEST_CASE(test30_variant_tpsa_mul)
{
    double a = 7, b = -11;
    auto tmp = create_tab(a, b);

    auto dc = tmp[0] * tmp[1];
    BOOST_CHECK_CLOSE(dc.cst(), a * b, 1e-12);
}

#if 0
BOOST_AUTO_TEST_CASE(test40_variant_tpsa_cmp_double)
{
    double a = 7, b = -11;
    auto tmp = create_tab(a, b);

    auto flag = (tmp[0] == a);
    BOOST_CHECK(flag == false);
}


BOOST_AUTO_TEST_CASE(test41_variant_double_cmp_tpsa)
{
    double a = 7, b = -11;
    auto tmp = create_tab(a, b);

    auto flag = (b == tmp[1]);
    BOOST_CHECK(flag == False);
}

BOOST_AUTO_TEST_CASE(test42_variant_tpsa_cmp)
{
    double a = 7, b = -11;
    auto tmp = create_tab(a, b);

    auto flag = tmp[0] == tmp[1];
    BOOST_CHECK(flag == False);
}
#endif


/* required e.g. for transform ... TPSA object bud double used for ss_vect */
BOOST_AUTO_TEST_CASE(test11_double_radd)
{
    const double a = 3, b =5;
    const auto ta = gtpsa::TpsaOrDouble(a);


    double r = b;
    r += ta;

    BOOST_CHECK_CLOSE(r, a + b, 1e-12);

}
