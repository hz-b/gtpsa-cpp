#define BOOST_TEST_MODULE gtpsa_template
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <gtpsa/ctpsa_complex_variant.hpp>
#include <memory>
#include <array>
#include <utility>

BOOST_AUTO_TEST_CASE(test01_variant_ctpsa_I)
{
    gtpsa::CTpsaOrComplex I (0e0, 1e0);
    gtpsa::CTpsaOrComplex I2 (0, 1);

}

BOOST_AUTO_TEST_CASE(test10_variant_ctpsa_alloc)
{
    auto a_desc = std::make_shared<gtpsa::desc>(6, 2);

    auto t = gtpsa::ctpsa(a_desc, 1);
    auto td = gtpsa::CTpsaOrComplex(t);

}

BOOST_AUTO_TEST_CASE(test20_variant_complex_add)
{
    std::complex<double> a = {3, 11}, b = {5, 13};
    auto ta = gtpsa::CTpsaOrComplex(a);
    auto tb = gtpsa::CTpsaOrComplex(b);
    auto tc = ta + tb;

    auto c = a + b;
    auto check = tc.cst();
    BOOST_CHECK_CLOSE(check.real(), c.real(), 1e-12);
    BOOST_CHECK_CLOSE(check.imag(), c.imag(), 1e-12);
}

static std::array<gtpsa::CTpsaOrComplex, 2> create_tab(const double a, const double b)
{
    auto desc = std::make_shared<gtpsa::desc>(6,1);
    auto ta = gtpsa::ctpsa(desc, 1);
    auto tb = gtpsa::ctpsa(desc, 1);
    ta += a; tb += b;
    auto da = gtpsa::CTpsaOrComplex(ta);
    auto db = gtpsa::CTpsaOrComplex(tb);
    BOOST_CHECK_CLOSE(da.real().cst(), a, 1e-12);
    BOOST_CHECK_CLOSE(db.real().cst(), b, 1e-12);

    std::array<gtpsa::CTpsaOrComplex, 2> r ({std::move(da), std::move(db)});
    return r;
}

BOOST_AUTO_TEST_CASE(test30_variant_tpsa_add)
{
    double a = 7, b = -11;
    auto tmp = create_tab(a, b);

    auto dc = tmp[0] + tmp[1];
    BOOST_CHECK_CLOSE(dc.real().cst(), a + b, 1e-12);
}

BOOST_AUTO_TEST_CASE(test30_variant_tpsa_sub)
{
    double a = 7, b = -11;
    auto tmp = create_tab(a, b);

    auto dc = tmp[0] - tmp[1];
    BOOST_CHECK_CLOSE(dc.real().cst(), a - b, 1e-12);
}

BOOST_AUTO_TEST_CASE(test30_variant_tpsa_mul)
{
    double a = 7, b = -11;
    auto tmp = create_tab(a, b);

    auto dc = tmp[0] * tmp[1];
    BOOST_CHECK_CLOSE(dc.real().cst(), a * b, 1e-12);
}

BOOST_AUTO_TEST_CASE(test30_variant_tpsa_div)
{
    double a = 7, b = -11;
    auto tmp = create_tab(a, b);

    auto dc = tmp[0] / tmp[1];
    BOOST_CHECK_CLOSE(dc.real().cst(), a / b, 1e-12);
}

BOOST_AUTO_TEST_CASE(test40_variant_ctpsa_exp)
{
    auto ta = gtpsa::CTpsaOrComplex(0e0);
    auto e = gtpsa::exp(ta);
    BOOST_CHECK_CLOSE(e.real().cst(), 1e0, 1e-12);
}
