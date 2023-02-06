#ifndef _GTPSA_COMPLEX_UTIL_H_
#define _GTPSA_COMPLEX_UTIL_H_ 1

#include <complex>
#include <cstring>
#include <mad_def.h>

/**
 * @brief convert a C++ complex to cnum_t aka C complex
 *
 * use array copy to avoid the import of complex.h and multiplications
 * uses feature that standard defines
 *
 * I guess there should be a library function, did not find one
 *
 * Could be templated if required
 */
static inline cpx_t std_complex_double_to_cpx_t(const std::complex<double> a_cplx)
{
    double _Complex res;
    /*
     * Both C and C complex standard define that complex numbers have to
     * be arrays of length 2.
     */
    memcpy(&res, reinterpret_cast<const double(&)[2]>(a_cplx), sizeof(res));
    /*
     * cnum_t is defined as double _Complex ... at least when I checked
     * mad_def.h.
     * Let the compiler complain if it changed.
     */
    return res;
}
static inline std::complex<double> cpx_t_to_std_complex_double(cpx_t a_cplx)
{
    std::complex<double> res;
    /*
     * Both C and C complex standard define that complex numbers have to
     * be arrays of length 2.
     */
    memcpy(&res, reinterpret_cast<const double(&)[2]>(a_cplx), sizeof(res));
    /*
     * cnum_t is defined as double _Complex ... at least when I checked
     * mad_def.h.
     * Let the compiler complain if it changed.
     */
    return res;
}

#endif /* _GTPSA_COMPLEX_UTIL_H_ */
