#define BOOST_TEST_MODULE gtpsa_cplx_arithmetic
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <gtpsa/ctpsa.hpp>
#include <cmath>
#include <complex>
/* required for creal, cimag */
#include <complex.h>

#ifdef __STDC_NO_COMPLEX__
#errpr "No complex"
#endif

BOOST_AUTO_TEST_CASE(test00_cast_cnum_t_to_complex)
{
    const double a = 42, c = 2;
    const std::complex<double> ac=a;
    const cnum_t cc= c;

    std::complex<double> c2 = c; // = std::complex<double>(cc);
    auto t = ac + c2;
    BOOST_CHECK_CLOSE(t.real(), a+c, 1e-12 );
    BOOST_CHECK_SMALL(t.imag(),      1e-12 );
}


BOOST_AUTO_TEST_CASE(test00_cast_complex_to_cnum_t)
{
    const double a = 42, c = 2;
    const std::complex<double> ac=a;
    const cnum_t cc= c;

    cnum_t ac2 = std_complex_double_to_cnum_t(ac);
    auto t = ac2 + cc;
    // BOOST_CHECK_CLOSE(creal(t), a+c, 1e-12 );
    // BOOST_CHECK_SMALL(cimag(t),      1e-12 );
}


#if 1
BOOST_AUTO_TEST_CASE(test100_set)
{
    const std::complex<double> a=0e0, b=42e0;

    auto a_desc = std::make_shared<gtpsa::desc>(1, 0);
    auto t1 = gtpsa::ctpsa(a_desc, mad_tpsa_default);
    t1.set(a, b);

}
#endif
