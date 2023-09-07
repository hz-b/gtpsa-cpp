#ifndef _GTPSA_MAD_UTIL_H_
#define _GTPSA_MAD_UTIL_H_ 1

#include <complex>

namespace gtpsa {

    template<class T>
    inline T same_as_instance(const T &v) { return T(v, init::same); }

    inline auto same_as_instance(const double &v) { return double(0e0); }
    inline auto same_as_instance(const std::complex<double> &v) { return double(0e0); }

};
#endif /* _GTPSA_MAD_UTIL_H_*/
