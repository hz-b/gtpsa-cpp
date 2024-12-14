#ifndef _GTPSA_MAD_CTPSA_WRAPPER_H_
#define _GTPSA_MAD_CTPSA_WRAPPER_H_ 1
/**
 * @brief wrapper around the Complex Truncated Power Series Algebra object
 *
 * This class wraps the complex part
 *
 * As distributed as part of the MAD package see
 * https://cern.ch/mad for details
 *
 * most functionality is generated using "mad_gtpsa_wrapper.tpp" and the
 * preprocessor. Have a look to the comments given in this file
 */

#include <gtpsa/forward_decl.hpp>

extern "C" {
#include <mad_ctpsa.h>
}

#ifdef GTPSA_CLASS
#undef GTPSA_CLASS
#endif
#define GTPSA_CLASS(clsname) _CTpsa ## clsname
#ifdef GTPSA_METH
#undef GTPSA_METH
#endif
#define GTPSA_METH(func) mad_ctpsa_ ## func
#ifdef GTPSA_BASE_T
#undef GTPSA_BASE_T
#endif
#define GTPSA_BASE_T cpx_t
#ifdef GTPSA_PTR_T
#undef GTPSA_PTR_T
#endif
#define GTPSA_PTR_T ctpsa_t

#include <gtpsa/mad/forward_decl.hpp>
#include <gtpsa/mad/wrapper.tpp>
#include <gtpsa/mad/container_wrapper.tpp>
#include <gtpsa/mad/tpsa_wrapper.hpp>

#ifndef GTPSA_KEEP_MACROS
#undef GTPSA_CLASS
#undef GTPSA_METH
#undef GTPSA_BASE_T
#undef GTPSA_PTR_T
#endif /*  GTPSA_KEEP_MACROS */

namespace gtpsa::mad {
#if 0
    class CTpsaWrapper;
    inline void add  (const CTpsaWrapper& a, const CTpsaWrapper& b,  CTpsaWrapper* r );
/**
 * @brief  (a_i-b_i)/max(|a_i|,1)
 */
    inline void dif   (const CTpsaWrapper& a, const CTpsaWrapper& b,  CTpsaWrapper* r );
    inline void sub   (const CTpsaWrapper& a, const CTpsaWrapper& b,  CTpsaWrapper* r );
    inline void mul   (const CTpsaWrapper& a, const CTpsaWrapper& b,  CTpsaWrapper* r );
    inline void div   (const CTpsaWrapper& a, const CTpsaWrapper& b,  CTpsaWrapper* r );
#endif

    class CTpsaWrapper: public _CTpsaWrapper {
    public:
	inline CTpsaWrapper(std::shared_ptr<desc> desc, const ord_t mo)
	    :  _CTpsaWrapper(desc, mo)
	    {}

	inline CTpsaWrapper(const CTpsaWrapper& t, const ord_t mo)
	    :  _CTpsaWrapper(t,  mo)
	    {}
    /*
     * not used ...
     */
    /*
    inline CTpsaWrapper(const TpsaWrapper& re, const TpsaWrapper& im)
                :  _CTpsaWrapper(re.getDescription(), maxord(std::vector<const _TpsaWrapper*>({&re, &im})))
        {
        }
    */

#ifndef GTSPA_ONLY_OPTIMISED_OPS
	inline CTpsaWrapper(const CTpsaWrapper& t)
	    :  _CTpsaWrapper(t)
	    {}
#endif
	/**
	 * @brief a*x[0]+b
	 */
	// inline void set0_r(                            num_t a_re, num_t a_im, num_t b_re, num_t b_im) { mad_ctpsa_set0_r (this->getPtr()                    , a_re, a_im,  b_re, b_im  ); }

	/**
	 * @brief a*x[i]+b
	 */
	inline void seti_r(const idx_t i             , num_t a_re, num_t a_im, num_t b_re, num_t b_im) { mad_ctpsa_seti_r (this->getPtr(), i                 , a_re, a_im,  b_re, b_im ); }
	/**
	 * @brief a*x[m]+b
	 */
	inline void sets_r(std::string s             , num_t a_re, num_t a_im, num_t b_re, num_t b_im) { mad_ctpsa_sets_r (this->getPtr(), s.size(), s.data(), a_re, a_im,  b_re, b_im ); }
	/**
	 * @brief a*x[m]+b
	 */
	inline void setm_r(const std::vector<ord_t> m, num_t a_re, num_t a_im, num_t b_re, num_t b_im) { mad_ctpsa_setm_r (this->getPtr(), m.size(), m.data(), a_re, a_im,  b_re, b_im ); }

	/**
	 * @brief a*x[m]+b, sparse mono [(i,o)]
	 * @todo vector of pairs?
	 */
	inline auto setsm_r(const std::vector<int> m, num_t a_re, num_t a_im, num_t b_re, num_t b_im) { return mad_ctpsa_setsm_r  (this->getPtr(), m.size(), m.data(), a_re, a_im,  b_re, b_im ); }

	inline void getv(idx_t i,       std::vector<cpx_t> *v) const { mad_ctpsa_getv(this->getPtr(), i, v->size(), v->data() ); }
	inline void setv(idx_t i, const std::vector<cpx_t> &v)       { mad_ctpsa_setv(this->getPtr(), i, v.size() , v.data()  ); }

	inline void real(TpsaWrapper* re) const {
	    mad_ctpsa_real(this->getPtr(), re->getPtr());
	}
        inline void imag(TpsaWrapper* im) const {
            mad_ctpsa_imag(this->getPtr(), im->getPtr());
        }
        inline void abs(TpsaWrapper* im) const {
            mad_ctpsa_cabs(this->getPtr(), im->getPtr());
        }
        inline void arg(TpsaWrapper* im) const {
            mad_ctpsa_cabs(this->getPtr(), im->getPtr());
        }

	inline void rcplx(const TpsaWrapper &re, const TpsaWrapper &im) {
	    mad_ctpsa_cplx(re.getPtr(), im.getPtr(), this->getPtr());
	}
        inline void runit(const CTpsaWrapper &o) {
            mad_ctpsa_unit(o.getPtr(),  this->getPtr());
        }
        inline void rpolar(const CTpsaWrapper &o) {
            mad_ctpsa_polar(o.getPtr(),  this->getPtr());
        }
        inline void rrect(const CTpsaWrapper &o) {
            mad_ctpsa_rect(o.getPtr(),  this->getPtr());
        }

    };


#ifdef GTPSA_FUNC_ARG1
#undef GTPSA_FUNC_ARG1
#endif
#ifdef GTPSA_FUNC_ARG1_WITH_RET_ARG
#undef GTPSA_FUNC_ARG1_WITH_RET_ARG
#endif
#define GTPSA_FUNC_ARG1_WITH_RET_ARG(fname)				\
    inline void fname (const CTpsaWrapper& t, CTpsaWrapper* r){ fname (static_cast<const _CTpsaWrapper&>(t), static_cast<_CTpsaWrapper*>(r)); }
#define GTPSA_FUNC_ARG1(fname) GTPSA_FUNC_ARG1_WITH_RET_ARG(fname)
#include <gtpsa/funcs.h>
#undef GTPSA_FUNC_ARG1_WITH_RET_ARG
#undef GTPSA_FUNC_ARG1


}// namespace gtpsa::mad

#endif /* _GTPSA_MAD_CTPSA_WRAPPER_H_ */
