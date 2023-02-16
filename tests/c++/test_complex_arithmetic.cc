#define BOOST_TEST_MODULE gtpsa_cplx_arithmetic
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <gtpsa/ctpsa.hpp>
#include <cmath>
#include <complex>
/* required for creal, cimag */
#include <complex.h>

#ifdef __STDC_NO_COMPLEX__
#error "No complex"
#endif

BOOST_AUTO_TEST_CASE(test00_cast_cnum_t_to_complex)
{
    const double a = 42, c = 2;
    const std::complex<double> ac=a;
    const cpx_t cc= c;

    std::complex<double> c2 = c; // = std::complex<double>(cc);
    auto t = ac + c2;
    BOOST_CHECK_CLOSE(t.real(), a+c, 1e-12 );
    BOOST_CHECK_SMALL(t.imag(),      1e-12 );
}

BOOST_AUTO_TEST_CASE(test10_print)
{
    auto a_desc = std::make_shared<gtpsa::desc>(6, 4);
    auto t1 = gtpsa::ctpsa(a_desc, mad_tpsa_default);

    t1.setv(1, {11, 13, 17, 19, 23, 29});
    t1.setv(1 + 6, {31, 37, 39, 41, 43, 47, 51});

    {
        // check that it works for constant objects
        const auto t = t1.clone();
        t.print("nn", 0, false, 0);
    }

}
BOOST_AUTO_TEST_CASE(test00_cast_complex_to_cpx_t)
{
    const double a = 42, c = 2;
    const std::complex<double> ac=a;
    const cpx_t cc= c;

    cpx_t ac2 = std_complex_double_to_cpx_t(ac);
    auto t = ac2 + cc;
    // BOOST_CHECK_CLOSE(creal(t), a+c, 1e-12 );
    // BOOST_CHECK_SMALL(cimag(t),      1e-12 );
}


#if 1
BOOST_AUTO_TEST_CASE(test100_set)
{
    const std::complex<double> a=0e0, b=42e0;

    auto a_desc = std::make_shared<gtpsa::desc>(1, 1);
    auto t1 = gtpsa::ctpsa(a_desc, mad_tpsa_default);
    t1.set(a, b);

}
#endif
BOOST_AUTO_TEST_CASE(test110_combine_re_im)
{

    const double a = 0, re_v = 2, im_v = 3;

    auto a_desc = std::make_shared<gtpsa::desc>(1, 1);
    auto re = gtpsa::tpsa(a_desc, mad_tpsa_default);
    auto im = gtpsa::tpsa(a_desc, mad_tpsa_default);

    re.set(a, re_v);
    im.set(a, im_v);

    auto cplx = gtpsa::ctpsa(re, im);

    auto check = cplx.get();
    BOOST_CHECK_CLOSE(check.real(), re_v, 1e-12);
    BOOST_CHECK_CLOSE(check.imag(), im_v, 1e-12);
}

// check that it works, does not crash
BOOST_AUTO_TEST_CASE(test111_cplx_to_re_im)
{
    auto a_desc = std::make_shared<gtpsa::desc>(6, 3);
    auto t_cplx = gtpsa::ctpsa(a_desc, 2);
    t_cplx.setName("cplx");

    auto t_re = t_cplx.real();
    auto t_im = t_cplx.imag();

}

// check that it works, does not crash
BOOST_AUTO_TEST_CASE(test112_cplx_to_polar)
{

    const double angle = 45e0/180e0 * M_PI;
    const double r = 1.0/std::sqrt(2e0), a = r * std::cos(angle), b = r * std::sin(angle);
    std::complex<double> c = {a,b};

    auto a_desc = std::make_shared<gtpsa::desc>(6, 2);
    auto t_cplx = gtpsa::ctpsa(a_desc, 1);
    t_cplx.setName("cplx");

    t_cplx.set(0, c);

    auto t_polar = t_cplx.polar();
    auto val = std::complex(t_polar.cst());
    BOOST_CHECK_CLOSE(val.real(), r, 1e-12);
    BOOST_CHECK_CLOSE(val.imag(), angle, 1e-12);
}

// check that it works, does not crash
BOOST_AUTO_TEST_CASE(test113_cplx_to_polar_to_real_imag)
{

    const double angle = 60e0/180e0 * M_PI;
    const double r = 1.0, a = r * std::cos(angle), b = r * std::sin(angle);
    std::complex<double> c = {a,b};

    auto a_desc = std::make_shared<gtpsa::desc>(1, 1);
    auto t_cplx = gtpsa::ctpsa(a_desc, 1);
    t_cplx.setName("cplx");

    t_cplx.set(0, c);
    auto z = t_cplx.cst();

    BOOST_CHECK_CLOSE(z.real(), 1/2.0, 1e-12);
    BOOST_CHECK_CLOSE(z.imag(), std::sqrt(3)/2.0, 1e-12);


    auto t_polar = t_cplx.polar();
    auto val = std::complex(t_polar.cst());
    BOOST_CHECK_CLOSE(val.real(), r, 1e-12);
    BOOST_CHECK_CLOSE(val.imag(), angle, 1e-12);
}

// check that it works, does not crash
BOOST_AUTO_TEST_CASE(test114_cplx_to_unit)
{
    const double angle = 45e0/180e0 * M_PI;
    const double scale = 2.0;
    const double r = scale  * std::sqrt(2e0), a = r * std::cos(angle), b = r * std::sin(angle);
    std::complex<double> c = {a,b};

    auto a_desc = std::make_shared<gtpsa::desc>(1, 1);
    auto t_cplx = gtpsa::ctpsa(a_desc, 1);
    t_cplx.setName("cplx");

    t_cplx.set(0, c);

    auto t_unit = t_cplx.unit();
    auto val = std::complex(t_unit.cst());
    BOOST_CHECK_CLOSE(val.real(), a/r, 1e-12);
    BOOST_CHECK_CLOSE(val.imag(), b /r, 1e-12);
}
