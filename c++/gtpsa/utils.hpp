#ifndef _GTPSA_UTIL_H_
#define _GTPSA_UTIL_H_ 1

#include <gtpsa/desc.hpp>
#include <gtpsa/utils.hpp>

#include <algorithm>

namespace gtpsa {
    template<class T>
    inline double cst(T &v) { return v.cst(); }

    inline double cst(double v) { return v; }

    template<class T>
    inline T clone(T &v) { return v.clone(); }

    inline double clone(double v) { return v; }

    template<class T>
    inline T same_as_instance(const T &v) { return T(v, init::same); }

    inline double same_as_instance(const double &v) { return double(0e0); }


} // namespace gtpa

#endif /* _GTPSA_UTIL_H_ */
