#ifndef _GTPSA_UTIL_H_
#define _GTPSA_UTIL_H_ 1

#include <gtpsa/desc.hpp>
#include <gtpsa/utils.hpp>

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
    inline T same_as_instance(const T &v) { return T(v, init::same); }

    inline auto same_as_instance(const double &v) { return double(0e0); }
    inline auto same_as_instance(const std::complex<double> &v) { return double(0e0); }


} // namespace gtpa

#endif /* _GTPSA_UTIL_H_ */
