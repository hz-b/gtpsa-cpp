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

    /*
     *  @todo:  check again standard library if not available
     */
    template<typename T, typename B>
    std::vector<B *> vector_type_to_base_ptr(std::vector<T> vec) {
        std::vector<B *> r;
        r.reserve(vec.size());
        std::transform(vec.begin(), vec.end(), r.begin(), [](T &o) { return static_cast<B *>(&o); });
        return r;
    }

    template <typename T, typename B>
    std::vector<const B*> vector_type_to_const_base_ptr(const std::vector<T> vec)
    {
        std::vector<const B*> r;
        r.reserve(vec.size());
        std::transform(vec.begin(), vec.end(), r.begin(), [](const T& o){ return static_cast<const B*>(&o); });
        return r;
    }
    /*
    template<typename T, typename B>
    std::vector<const B *> vector_type_to_base_ptr(const std::vector<T> &vec)
    {
        return vector_type_to_const_base_ptr<T, B>(vec);
    }
    */
    template <typename T, typename B>
    std::vector<B*> vector_ptr_to_base_ptr(std::vector<T*>& vec)
    {
        std::vector<B*> r;
        r.reserve(vec.size());
        std::transform(vec.begin(), vec.end(), r.begin(), [](T* o){ return static_cast<B*>(o); });
        return r;
    }

    template <typename T, typename B>
    std::vector<const B*> vector_ptr_to_const_base_ptr(const std::vector<const T*>& vec)
    {
        std::vector<const B*> r;
        r.reserve(vec.size());
        std::transform(vec.begin(), vec.end(), r.begin(), [](const T* o){ return static_cast<const B*>(o); });
        return r;
    }
    /*
    template <typename T, typename B>
    std::vector<const B*> vector_ptr_to_base_ptr(const std::vector<const T*>& vec) {
        return vector_ptr_to_const_base_ptr<T, B>(vec);
    }
     */
} // namespace gtpa

#endif /* _GTPSA_UTIL_H_ */
