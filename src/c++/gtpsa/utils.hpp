#ifndef _GTPSA_UTIL_H_
#define _GTPSA_UTIL_H_ 1

#include <gtpsa/desc.hpp>
#include <gtpsa/tpsa.hpp>
#include <gtpsa/ctpsa.hpp>
#include <gtpsa/mad/utils.hpp>

#include <algorithm> // to be removed?
#include <complex>

namespace gtpsa {
    template<class T>
    inline auto cst(T &v) { return v.cst(); }
    //inline std::complex<double> cst(gtpsa::ctpsa &v) { return v.get_complex(); }
    inline double cst(double v) { return v; }
    inline std::complex<double> cst(std::complex<double> v) { return v; }

    template<class T>
    inline T clone(const T &v) { return v.clone(); }

    inline auto clone(const double v) { return v; }
    inline auto clone(const std::complex<double> v) { return v; }


    template<class T>
    inline T     sqr  (const T &v) { return v * v; }

    inline tpsa  sqr (const gtpsa::tpsa&  v) { return gtpsa::pow(v, 2); }
    inline ctpsa sqr (const gtpsa::ctpsa& v) { return gtpsa::pow(v, 2); }

    inline auto sqrt(const double  v) { return std::sqrt(v); }
} // namespace gtpa

#endif /* _GTPSA_UTIL_H_ */
