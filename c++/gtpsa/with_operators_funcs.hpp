/* -*- c++ -*- */
#ifndef _GTPSA_TPSA_WITH_OPERATORS_FUNCS_H_
#define _GTPSA_TPSA_WITH_OPERATORS_FUNCS_H_ 1
#include <algorithm>
#include <vector>


namespace gtpsa {
    // ------------------------------------------------------------------------------
    // helper functions for different operators and mathematical functions
    // ------------------------------------------------------------------------------
    // inplace

    /**
     * @brief single argument in single argument out
     * @todo renaming required
     */




    // ------------------------------------------------------------------------------
    // End of helper functions
    // ------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------
    // mathematical functions using helper functions
    // inplace versions



    template<typename T, typename = typename T::ptr_type, typename = typename T::base_type>
    inline TpsaWithOp<T> acc     (const TpsaWithOp<T> &a, const typename T::base_type v ) { return process2 (a, v,      acc    ); }
    template<typename T, typename = typename T::ptr_type, typename = typename T::base_type>
    inline TpsaWithOp<T> scl     (const TpsaWithOp<T> &a, const typename T::base_type v ) { return TpsaProcessorHelpers<T>::process2 (a, v,      scl    ); }
    template<typename T, typename = typename T::ptr_type, typename = typename T::base_type>
    inline TpsaWithOp<T> inv     (const TpsaWithOp<T> &a, const typename T::base_type v ) { return process2 (a, v,      inv    ); }
    template<typename T, typename = typename T::ptr_type, typename = typename T::base_type>
    inline TpsaWithOp<T> invsqrt (const TpsaWithOp<T> &a, const typename T::base_type v ) { return process2 (a, v,      invsqrt); }

/*
    template<typename T, typename = typename T::ptr_type, typename = typename T::base_type>
    inline void add_d (const TpsaWithOp<T>& a, const typename T::base_type b, TpsaWithOp<T>* r ){ process2_(a, b, r, [](typename T::base_type a, typename T::base_type b, typename T::base_type *r){ *r = a + b; }); }
    template<typename T, typename = typename T::ptr_type, typename = typename T::base_type>
    inline void sub_d (const TpsaWithOp<T>& a, const typename T::base_type b, TpsaWithOp<T>* r ){ process2_(a, b, r, [](typename T::base_type a, typename T::base_type b, typename T::base_type *r){ *r = a - b; }); }
    template<typename T, typename = typename T::ptr_type, typename = typename T::base_type>
    inline void mul_d (const TpsaWithOp<T>& a, const typename T::base_type b, TpsaWithOp<T>* r ){ process2_(a, b, r, [](typename T::base_type a, typename T::base_type b, typename T::base_type *r){ *r = a * b; }); }
    template<typename T, typename = typename T::ptr_type, typename = typename T::base_type>
    inline void div_d (const TpsaWithOp<T>& a, const typename T::base_type b, TpsaWithOp<T>* r ){ process2_(a, b, r, [](typename T::base_type a, typename T::base_type b, typename T::base_type *r){ *r = a / b; }); }
*/


    // inline tpsa add_d (const tpsa& a, const typename T::base_type b          ){ return process2(a, b, add); }
    // inline tpsa sub_d (const tpsa& a, const typename T::base_type b          ){ return process2(a, b, sub); }
    // inline tpsa mul_d (const tpsa& a, const typename T::base_type b          ){ return process2(a, b, mul); }
    // inline tpsa div_d (const tpsa& a, const typename T::base_type b          ){ return process2(a, b, div); }

    /* standard mathematical functions ... trigonometic etc ... taking one argument returning one */




    template<typename T, typename = typename T::bridge_type, typename = typename T::base_type, typename = typename T::ptr_type>
    inline TpsaWithOp<T> taylor(const TpsaWithOp<T>& a, std::vector<typename T::base_type> coeff) {
	    auto ret = a.newFromThis(); //TpsaWithOp<T>(a, gtpsa::init::same);
	    mad::taylor(a, coeff, &ret);
	    return ret;
    }

    template<typename T, typename = typename T::bridge_type, typename = typename T::base_type, typename = typename T::ptr_type>
    inline TpsaWithOp<T> pow (const TpsaWithOp<T>& a, int n         ){ auto r = a.newFromThis(); pow(a, n, &r); return r; }
    template<typename T, typename = typename T::bridge_type, typename = typename T::base_type, typename = typename T::ptr_type>
    inline TpsaWithOp<T> pow (const TpsaWithOp<T>& a, typename T::base_type v){ auto r = a.newFromThis(); pow(a, v, &r); return r; }

} // namespace gtpsa

#endif /* _GTPSA_TPSA_WITH_OPERATORS_FUNCS_H_ */
