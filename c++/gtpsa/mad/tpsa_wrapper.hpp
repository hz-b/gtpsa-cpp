#ifndef _GTPSA_MAD_TPSA_WRAPPER_H_
#define _GTPSA_MAD_TPSA_WRAPPER_H_ 1
/**
 * @brief wrapper around the Truncated Power Series Algebra object
 *
 * This class wraps the real (double) part
 *
 * As distributed as part of the MAD package see
 * https://cern.ch/mad for details
 *
 * most functionality is generated using "mad_gtpsa_wrapper.tpp" and the
 * preprocessor. Have a look to the comments given in this file
 */

#include <gtpsa/forward_decl.hpp>

extern "C" {
#include <mad_tpsa.h>
}

#ifdef GTPSA_CLASS
#undef GTPSA_CLASS
#endif
#define GTPSA_CLASS(clsname) _Tpsa ## clsname
#ifdef GTPSA_METH
#undef GTPSA_METH
#endif
#define GTPSA_METH(func) mad_tpsa_ ## func
#ifdef GTPSA_BASE_T
#undef GTPSA_BASE_T
#endif
#define GTPSA_BASE_T num_t
#ifdef GTPSA_PTR_T
#undef GTPSA_PTR_T
#endif
#define GTPSA_PTR_T tpsa_t

#include <gtpsa/mad/wrapper.tpp>
#include <gtpsa/mad/container_wrapper.tpp>
#include <gtpsa/utils.hpp>

#ifndef GTPSA_KEEP_MACROS
#undef GTPSA_CLASS
#undef GTPSA_METH
#undef GTPSA_BASE_T
#undef GTPSA_PTR_T
#endif /*  GTPSA_KEEP_MACROS */



namespace gtpsa::mad {
    class TpsaWrapper: public _TpsaWrapper {
    public:
	inline TpsaWrapper(std::shared_ptr<desc> desc, const ord_t mo)
	    :  _TpsaWrapper(desc, mo)
	    {}

	inline TpsaWrapper(const TpsaWrapper& t, const ord_t mo)
	    :  _TpsaWrapper(t,  mo)
	    {}

#ifndef GTSPA_ONLY_OPTIMISED_OPS
	inline TpsaWrapper(const TpsaWrapper& t)
	    :  _TpsaWrapper(t)
	    {}

#endif  //GTSPA_ONLY_OPTIMISED_OPS
	friend inline auto norm  (const TpsaWrapper& a);
	friend inline auto equ   (const TpsaWrapper& a, const TpsaWrapper& b, num_t tol);
    };

    /*o---------------------------------------------------------------------o
      |                                                                     |
      | Functions below are given here as these only exist for the normal   |
      | "tpsa" part                                                         |
     */

     /*
     *
     */
    inline auto norm (const TpsaWrapper& a) {
	return mad_tpsa_nrm (a.getPtr());
    }

    inline auto equ (const TpsaWrapper& a, const TpsaWrapper& b, num_t tol) {
	return mad_tpsa_equ (a.getPtr(), b.getPtr(), tol);
    }

    /*|                                                                     |
      o---------------------------------------------------------------------o */

    /*o---------------------------------------------------------------------o
      |                                                                     |
      | Functions below implement the "operator functions" that return an   |
      | object, requires casting                                            |
      | are these really requried                                           |
     */
    inline void add  (const TpsaWrapper& a, const TpsaWrapper& b,  TpsaWrapper* r ){
	add_(
	    static_cast<const _TpsaWrapper &>(a),
	    static_cast<const _TpsaWrapper &>(b),
	    static_cast<_TpsaWrapper*>(r)
	    );
    }
    /**
     * @brief  (a_i-b_i)/max(|a_i|,1)
     */
    inline void dif   (const TpsaWrapper& a, const TpsaWrapper& b,  TpsaWrapper* r ){
	dif_(
	    static_cast<const _TpsaWrapper &>(a),
	    static_cast<const _TpsaWrapper &>(b),
	    static_cast<_TpsaWrapper*>(r)
	    );
    }
    inline void sub   (const TpsaWrapper& a, const TpsaWrapper& b,  TpsaWrapper* r ){
	sub_(
	    static_cast<const _TpsaWrapper &>(a),
	    static_cast<const _TpsaWrapper &>(b),
	    static_cast<_TpsaWrapper*>(r)
	    );
    }
    inline void mul   (const TpsaWrapper& a, const TpsaWrapper& b,  TpsaWrapper* r ){
	mul_(
	    static_cast<const _TpsaWrapper &>(a),
	    static_cast<const _TpsaWrapper &>(b),
	    static_cast<_TpsaWrapper*>(r))
	    ;
    }
    inline void div   (const TpsaWrapper& a, const TpsaWrapper& b,  TpsaWrapper* r ){
	div_(
	    static_cast<const _TpsaWrapper &>(a),
	    static_cast<const _TpsaWrapper &>(b),
	    static_cast<_TpsaWrapper*>(r)
	    );
    }
    inline void acc  (const TpsaWrapper& a, const num_t & b,  TpsaWrapper* r ){
	acc_(static_cast<const _TpsaWrapper &>(a), b, static_cast<_TpsaWrapper*>(r) );
    }
    inline void scl  (const TpsaWrapper& a, const num_t & b,  TpsaWrapper* r ){
	scl_(static_cast<const _TpsaWrapper &>(a), b, static_cast<_TpsaWrapper*>(r) );
    }
    inline void inv  (const TpsaWrapper& a, const num_t & b,  TpsaWrapper* r ){
	inv_(static_cast<const _TpsaWrapper &>(a), b, static_cast<_TpsaWrapper*>(r) );
    }
    inline void invsqrt (const TpsaWrapper& a, const num_t & b,  TpsaWrapper* r ){
	invsqrt_(static_cast<const _TpsaWrapper &>(a), b, static_cast<_TpsaWrapper*>(r) );
    }

    inline void pow   (const TpsaWrapper& a, const TpsaWrapper& b,  TpsaWrapper* r ){
	pow_(
	    static_cast<const _TpsaWrapper &>(a),
	    static_cast<const _TpsaWrapper &>(b),
	    static_cast<_TpsaWrapper*>(r)
	    );
    }
    inline void pow   (const TpsaWrapper& a, const num_t & b,  TpsaWrapper* r ){
	pow_(static_cast<const _TpsaWrapper &>(a), b, static_cast<_TpsaWrapper*>(r) );
    }
    inline void pow   (const TpsaWrapper& a, const  int  & i,  TpsaWrapper* r ){
	pow_(static_cast<const _TpsaWrapper &>(a), i, static_cast<_TpsaWrapper*>(r) );
    }

    // inline void pow   (const TpsaWrapper& a, const int i ,  TpsaWrapper* r ){ div_(static_cast<const _TpsaWrapper &>(a), i, static_cast<_TpsaWrapper*>(r)); }

#ifdef GTPSA_FUNC_ARG1
#undef GTPSA_FUNC_ARG1
#endif
#ifdef GTPSA_FUNC_ARG1_WITH_RET_ARG
#undef GTPSA_FUNC_ARG1_WITH_RET_ARG
#endif
#define GTPSA_FUNC_ARG1_WITH_RET_ARG(fname)				\
    inline void fname (const TpsaWrapper& t, TpsaWrapper* r){ fname (static_cast<const _TpsaWrapper&>(t), static_cast<_TpsaWrapper*>(r)); }
#define GTPSA_FUNC_ARG1(fname) GTPSA_FUNC_ARG1_WITH_RET_ARG(fname)
#include <gtpsa/funcs.h>
#undef GTPSA_FUNC_ARG1_WITH_RET_ARG
#undef GTPSA_FUNC_ARG1



} // namespace gtpsa::mad





#endif /* _GTPSA_MAD_TPSA_WRAPPER_H_ */
