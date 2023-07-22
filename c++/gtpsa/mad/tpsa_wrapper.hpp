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

#include <gtpsa/mad/forward_decl.hpp>
#include <gtpsa/mad/wrapper.tpp>
#include <gtpsa/mad/container_wrapper.tpp>
#include <gtpsa/utils.hpp>
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
        friend class CTpsaWrapper;

        inline void atan2(const TpsaWrapper& y, const TpsaWrapper& x) {
	    GTPSA_METH(atan2)(y.getPtr(), x.getPtr(), this->getPtr());
	}

	inline auto norm (void) {
	    return mad_tpsa_nrm (this->getPtr());
	}

	inline auto equ (const TpsaWrapper& o, num_t tol) {
	    return mad_tpsa_equ (this->getPtr(), o.getPtr(), tol);
    }
	friend inline auto norm(const TpsaWrapper& a);
	friend inline auto
	equ(const TpsaWrapper& a, const TpsaWrapper& b, num_t tol);
    };
};
#ifndef GTPSA_KEEP_MACROS
#undef GTPSA_CLASS
#undef GTPSA_METH
#undef GTPSA_BASE_T
#undef GTPSA_PTR_T
#endif /*  GTPSA_KEEP_MACROS */



namespace gtpsa::mad {

    /*o---------------------------------------------------------------------o
      |                                                                     |
      | Functions below are given here as these only exist for the normal   |
      | "tpsa" part                                                         |
     */

     /*
     *
     */

#ifdef GTPSA_FUNC_ARG1
#undef GTPSA_FUNC_ARG1
#endif
#ifdef GTPSA_FUNC_ARG1_WITH_RET_ARG
#undef GTPSA_FUNC_ARG1_WITH_RET_ARG
#endif
#define GTPSA_FUNC_ARG1_WITH_RET_ARG(fname)				\
    inline void fname (const TpsaWrapper& t, TpsaWrapper* r) \
    { fname (static_cast<const _TpsaWrapper&>(t), \
	     static_cast<_TpsaWrapper*>(r)); }
#define GTPSA_FUNC_ARG1(fname) GTPSA_FUNC_ARG1_WITH_RET_ARG(fname)
#include <gtpsa/funcs.h>
#undef GTPSA_FUNC_ARG1_WITH_RET_ARG
#undef GTPSA_FUNC_ARG1



} // namespace gtpsa::mad





#endif /* _GTPSA_MAD_TPSA_WRAPPER_H_ */
