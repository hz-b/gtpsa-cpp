#ifndef _GTPSA_UTIL_H_
#define _GTPSA_UTIL_H_ 1

namespace gtpsa {
template <class T>
inline double cst(T v){return v.cst(); }
inline double cst(double v){ return v; }
} // namespace gtpa

#endif /* _GTPSA_UTIL_H_ */
