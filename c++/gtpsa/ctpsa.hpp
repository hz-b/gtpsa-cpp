#ifndef _GTPSA_CTPSA_H_
#define _GTPSA_CTPSA_H_ 1
/**
 * @brief Complex truncated power series modules
 *
 * As distributed as part of the MAD package see
 * https://cern.ch/mad for details
 *
 * This wrapper provides "nearly direct access" to the original C functions
 * and provides operator access to these functions too.
 *
 * @todo: add wrapper for more advanced functions
 */
#include <algorithm>
#include <cassert>
#include <iomanip>
#include <memory>
#include <ostream>
#include <vector>
#include <complex>

#include <gtpsa/desc.hpp>
#include <gtpsa/mad/tpsa_wrapper.hpp>
#include <gtpsa/mad/ctpsa_wrapper.hpp>
#include <gtpsa/tpsa.hpp>

#include <gtpsa/bridge/bridge.hpp>
#include <gtpsa/intern/complex_utils.hpp>
#include <gtpsa/intern/templated_funcs.hpp>
#include <gtpsa/intern/with_operators.hpp>


namespace gtpsa {

class CTpsaTypeInfo : public GTpsaTypeInfo<ctpsa_t, cpx_t, ctpsa, mad::CTpsaWrapper, mad::_CTpsaWrapper, mad::_CTpsaContainerWrapper>  {};

    /* Bridge to the mad gtpsa c++ wrapper */
    typedef TpsaBridge<CTpsaTypeInfo> ctpsa_bridge;

    /*
     * c++ style functionality of the operator functions.
     * with return object thus name started with an r
     */

    inline void radd (const ctpsa_bridge& a, const ctpsa_bridge& b, ctpsa_bridge* r)  { r->apply2_with_return_object(a, b, mad::add ); }
    inline void rsub (const ctpsa_bridge& a, const ctpsa_bridge& b, ctpsa_bridge* r)  { r->apply2_with_return_object(a, b, mad::sub ); }
    inline void rmul (const ctpsa_bridge& a, const ctpsa_bridge& b, ctpsa_bridge* r)  { r->apply2_with_return_object(a, b, mad::mul ); }
    inline void rdiv (const ctpsa_bridge& a, const ctpsa_bridge& b, ctpsa_bridge* r)  { r->apply2_with_return_object(a, b, mad::div ); }

    inline void racc     (const ctpsa_bridge& a, const cpx_t v, ctpsa_bridge* r) { r->apply2_base_with_return_object(a, v, mad::acc ); }
    inline void rscl     (const ctpsa_bridge& a, const cpx_t v, ctpsa_bridge* r) { r->apply2_base_with_return_object(a, v, mad::scl ); }
    inline void rinv     (const ctpsa_bridge& a, const cpx_t v, ctpsa_bridge* r) { r->apply2_base_with_return_object(a, v, mad::inv ); }
    inline void rinvsqrt (const ctpsa_bridge& a, const cpx_t v, ctpsa_bridge* r) { r->apply2_base_with_return_object(a, v, mad::invsqrt ); }

    inline void rpow (const ctpsa_bridge& a, const ctpsa_bridge& b, ctpsa_bridge *r) { r->pow(a, b);  }
    inline void rpow (const ctpsa_bridge& a, const int           i, ctpsa_bridge *r) { r->pow(a, i);  }
    inline void rpow (const ctpsa_bridge& a, const cpx_t        v, ctpsa_bridge *r) { r->pow(a, v);  }

    /* return newly allocated object */
    inline ctpsa_bridge add ( const ctpsa_bridge& a, const ctpsa_bridge& b ) { return apply2<ctpsa_bridge>(a, b, radd ); }
    inline ctpsa_bridge sub ( const ctpsa_bridge& a, const ctpsa_bridge& b ) { return apply2<ctpsa_bridge>(a, b, rsub ); }
    inline ctpsa_bridge mul ( const ctpsa_bridge& a, const ctpsa_bridge& b ) { return apply2<ctpsa_bridge>(a, b, rmul ); }
    inline ctpsa_bridge div ( const ctpsa_bridge& a, const ctpsa_bridge& b ) { return apply2<ctpsa_bridge>(a, b, rdiv ); }

    inline ctpsa_bridge pow ( const ctpsa_bridge& a, const ctpsa_bridge& b ) { ctpsa_bridge r = a.newFromThis(); rpow(a, b, &r); return r; }
    inline ctpsa_bridge pow ( const ctpsa_bridge& a, const int           i ) { ctpsa_bridge r = a.newFromThis(); rpow(a, i, &r); return r; }
    inline ctpsa_bridge pow ( const ctpsa_bridge& a, const cpx_t        v ) { ctpsa_bridge r = a.newFromThis(); rpow(a, v, &r); return r; }

    //inline tpsa_bridge acc     (const ctpsa_bridge& a, const cpx_t b)  { return apply2_base<ctpsa_bridge, cpx_t>(a, b, racc); }
    //inline tpsa_bridge scl     (const ctpsa_bridge& a, const cpx_t b)  { return apply2_base<ctpsa_bridge, cpx_t>(a, b, rscl); }
    //inline tpsa_bridge inv     (const ctpsa_bridge& a, const cpx_t b)  { return apply2_base<ctpsa_bridge, cpx_t>(a, b, rinv); }
    //inline tpsa_bridge invsqrt (const ctpsa_bridge& a, const cpx_t b)  { return apply2_base<ctpsa_bridge, cpx_t>(a, b, rinvsqrt); }


    /* required for the template adding operators */
    typedef TpsaWithOp<CTpsaTypeInfo> ctpsa_with_op ;
    /*
     * @brief Complex truncated power series
     *
     * @todo add missing functionality
     */
    class ctpsa : public ctpsa_with_op {

    public:
	using base = ctpsa_with_op;

	/*
	 * @brief ctor using description and mo
	 */
	inline ctpsa(std::shared_ptr<mad::desc> desc, const ord_t mo)
	    : base(desc, mo)
	    {}

	inline ctpsa(const ctpsa&             t, const ord_t mo)
	    : base(t.getDescription(),   mo)
	    {}

	inline ctpsa(const tpsa&              t, const ord_t mo)
	    : base(t.getDescription(),   mo)
	    {}

        inline ctpsa(const tpsa& re, const tpsa& im)
            : base(re.getDescription(), std::max({re.order(), im.order()}))
            {
               this->m_impl.rcplx(re.m_impl, im.m_impl);
            }


	inline ctpsa(const base&& o)
	    : base(std::move(o))
	    {}

   // Why required here?
   inline ctpsa(const ctpsa_bridge&& o)
        : base(o)
        {}


#ifndef GTSPA_ONLY_OPTIMISED_OPS

	// inline ctpsa(const ctpsa& o)
	//    : CTpsaWithOp(o)
	//    {}

	inline ctpsa(const base& o)
	    : base(o)
	    {}


	inline ctpsa(const ctpsa&              o) = default;

#else /* GTSPA_ONLY_OPTIMISED_OPS */

	inline ctpsa(const ctpsa&              o) = delete;

#endif
    /**
     * @brief method get return cnum_t, which is incompatible with std::complex<double>
     */
	inline auto get(void)                        const { return std::complex<double> ( base::get()  ); }
	inline auto get(const idx_t i)               const { return std::complex<double> ( base::get(i) ); }
	inline auto get(std::string s)               const { return std::complex<double> ( base::get(s) ); }
	inline auto get(const std::vector<ord_t>& m) const { return std::complex<double> ( base::get(m) ); }

	/**
	 * @brief a*x[0]+b
	 */
	inline void set(                            num_t a_re, num_t a_im, num_t b_re, num_t b_im) { this->m_impl.set0_r(   a_re, a_im,  b_re, b_im  ); }

	/**
	 * @brief a*x[i]+b
	 */
	inline void set(const idx_t i             , num_t a_re, num_t a_im, num_t b_re, num_t b_im) { this->m_impl.seti_r(i, a_re, a_im,  b_re, b_im ); }
	/**
	 * @brief a*x[m]+b
	 */
	inline void set(std::string s             , num_t a_re, num_t a_im, num_t b_re, num_t b_im) { this->m_impl.sets_r(s, a_re, a_im,  b_re, b_im ); }
	/**
	 * @brief a*x[m]+b
	 */
	inline void set(const std::vector<ord_t> m, num_t a_re, num_t a_im, num_t b_re, num_t b_im) { this->m_impl.setm_r(m, a_re, a_im,  b_re, b_im ); }

	/**
	 * @brief a*x[m]+b, sparse mono [(i,o)]
	 * @todo vector of pairs?
	 */
	inline auto setsm(const std::vector<int> m, num_t a_re, num_t a_im, num_t b_re, num_t b_im) { return this->m_impl.setsm_r  (m, a_re, a_im,  b_re, b_im ); }

	/* compatible to complex part */
	inline void set(                             const std::complex<double> a, const std::complex<double> b) {
	    base::set(std_complex_double_to_cpx_t(a), std_complex_double_to_cpx_t(b));
	}
	inline void set(                             const cpx_t a, const cpx_t b) {
	    base::set(a, b);
	}
	inline void set(const idx_t i,               const std::complex<double> a, const std::complex<double> b) {
	    base::set(i, std_complex_double_to_cpx_t(a), std_complex_double_to_cpx_t(b));
	}

	inline void set(const std::string& s,        const std::complex<double> a, const std::complex<double> b) {
	    base::set(s, std_complex_double_to_cpx_t(a), std_complex_double_to_cpx_t(b));
	}

	inline void set(const std::vector<ord_t>& m, const std::complex<double> a, const std::complex<double> b) {
	    base::set(m, std_complex_double_to_cpx_t(a), std_complex_double_to_cpx_t(b));
	}
	inline void set(const std::vector<ord_t>& m, const cpx_t a, const cpx_t b) {
	    base::set(m, a, b);
	}
	inline void setv(idx_t i, const std::vector<std::complex<double>> &v) {
	    std::vector<cpx_t> tmp;
	    std::transform(v.begin(), v.end(), std::back_inserter(tmp),
			   [] (std::complex<double> val) { return std_complex_double_to_cpx_t(val); }
		);
	    base::setv(i, tmp);
	}

	inline void setsm(const std::vector<int>& m, const std::complex<double> a, const std::complex<double> b) {
	    base::setsm(m, std_complex_double_to_cpx_t(a), std_complex_double_to_cpx_t(b));
	}

	/* compatible to c++ and thus python */
	inline void real(tpsa* re) const {
            this->m_impl.real(&re->m_impl);
        }

        inline void imag(tpsa * re) const {
            this->m_impl.imag(&re->m_impl);
        }

	inline void abs(tpsa * abs) const {
            this->m_impl.abs(&abs->m_impl);
        }

	inline void arg(tpsa * arg) const {
            this->m_impl.arg(&arg->m_impl);
        }

        inline void runit(const ctpsa& c) {
            this->m_impl.runit(c.m_impl);
        }

        inline void rpolar(const ctpsa& c)  {
            this->m_impl.rpolar(c.m_impl);
        }

        inline void rrect(const ctpsa& c)  {
            this->m_impl.rrect(c.m_impl);
        }


#ifndef GTSPA_ONLY_OPTIMISED_OPS
        inline tpsa real() const {
            tpsa re = tpsa(this->getDescription(), this->order());
            this->real(&re);
            return re;
        }

        inline tpsa imag() const {
            tpsa im = tpsa(this->getDescription(), this->order());
            this->imag(&im);
            return im;
        }

        inline tpsa abs() const {
            tpsa t = tpsa(this->getDescription(), this->order());
            this->abs(&t);
            return t;
        }

        inline tpsa arg() const {
            tpsa t = tpsa(this->getDescription(), this->order());
            this->arg(&t);
            return t;
        }

        inline ctpsa polar() const {
            ctpsa r = this->newFromThis();
            r.rpolar(*this);
            return r;
        }

        inline ctpsa unit() const {
            ctpsa r = this->newFromThis();
            r.runit(*this);
            return r;
        }
#endif

	/**
	 * @brief method get return cpx_t, which is incompatible with std::complex<double>
	 */
<<<<<<< HEAD
        inline auto get_complex(void) {
	    return cpx_t_to_std_complex_double(this->get());
        }
=======
>>>>>>> f695661 (visitor added for exporing internal object to python object)

        inline auto cst(void) const { return cpx_t_to_std_complex_double(base::cst());}


        inline ctpsa& operator += (const ctpsa& o ) { base::operator += (o) ; return *this; }
        inline ctpsa& operator -= (const ctpsa& o ) { base::operator -= (o) ; return *this; }
        inline ctpsa& operator *= (const ctpsa& o ) { base::operator *= (o) ; return *this; }
        inline ctpsa& operator /= (const ctpsa& o ) { base::operator /= (o) ; return *this; }

        inline ctpsa& operator += (const cpx_t o ) { base::operator += (o) ; return *this; }
        inline ctpsa& operator -= (const cpx_t o ) { base::operator -= (o) ; return *this; }
        inline ctpsa& operator *= (const cpx_t o ) { base::operator *= (o) ; return *this; }
        inline ctpsa& operator /= (const cpx_t o ) { base::operator /= (o) ; return *this; }

        inline ctpsa  operator  - ( void         ) const { return ctpsa( base::operator-(*this) ); }

        /*
         * add operators that accept std::complex<double> ... needs to be converted to cpx_t
         */
        inline ctpsa& operator += ( const std::complex<double>& o )       { return operator += (std_complex_double_to_cpx_t(o)) ; }
        inline ctpsa& operator -= ( const std::complex<double>& o )       { return operator -= (std_complex_double_to_cpx_t(o)) ; }
        inline ctpsa& operator *= ( const std::complex<double>& o )       { return operator *= (std_complex_double_to_cpx_t(o)) ; }
        inline ctpsa& operator /= ( const std::complex<double>& o )       { return operator /= (std_complex_double_to_cpx_t(o)) ; }

        inline ctpsa  operator +  ( const ctpsa&  o ) const { return ctpsa( std::move( base::operator+ (o) ) ) ; }
        inline ctpsa  operator -  ( const ctpsa&  o ) const { return ctpsa( std::move( base::operator- (o) ) ) ; }
        inline ctpsa  operator *  ( const ctpsa&  o ) const { return ctpsa( std::move( base::operator* (o) ) ) ; }
        inline ctpsa  operator /  ( const ctpsa&  o ) const { return ctpsa( std::move( base::operator/ (o) ) ) ; }

        inline ctpsa  operator +  ( const cpx_t o ) const { return ctpsa( base::operator+ (o) ) ; }
        inline ctpsa  operator -  ( const cpx_t o ) const { return ctpsa( base::operator- (o) ) ; }
        inline ctpsa  operator *  ( const cpx_t o ) const { return ctpsa( base::operator* (o) ) ; }
        inline ctpsa  operator /  ( const cpx_t o ) const { return ctpsa( base::operator/ (o) ) ; }

        inline ctpsa  operator +  ( const std::complex<double>& o ) const { return ctpsa( this->operator+ (std_complex_double_to_cpx_t(o)) ) ; }
        inline ctpsa  operator -  ( const std::complex<double>& o ) const { return ctpsa( this->operator- (std_complex_double_to_cpx_t(o)) ) ; }
        inline ctpsa  operator *  ( const std::complex<double>& o ) const { return ctpsa( this->operator* (std_complex_double_to_cpx_t(o)) ) ; }
        inline ctpsa  operator /  ( const std::complex<double>& o ) const { return ctpsa( this->operator/ (std_complex_double_to_cpx_t(o)) ) ; }

    }; // class ctpsa

    inline ctpsa operator +  (const cpx_t a, const ctpsa& b)  { return a + ctpsa::base(b); }
    inline ctpsa operator -  (const cpx_t a, const ctpsa& b)  { return a - ctpsa::base(b); }
    inline ctpsa operator *  (const cpx_t a, const ctpsa& b)  { return a * ctpsa::base(b); }
    inline ctpsa operator /  (const cpx_t a, const ctpsa& b)  { return a / ctpsa::base(b); }

    inline ctpsa operator +  (const std::complex<double> a, const ctpsa& b)  { return std_complex_double_to_cpx_t(a) + ctpsa::base(b); }
    inline ctpsa operator -  (const std::complex<double> a, const ctpsa& b)  { return std_complex_double_to_cpx_t(a) - ctpsa::base(b); }
    inline ctpsa operator *  (const std::complex<double> a, const ctpsa& b)  { return std_complex_double_to_cpx_t(a) * ctpsa::base(b); }
    inline ctpsa operator /  (const std::complex<double> a, const ctpsa& b)  { return std_complex_double_to_cpx_t(a) / ctpsa::base(b); }

    inline ctpsa pow (const ctpsa& a,  const ctpsa& b){ return ctpsa( pow(static_cast<const ctpsa::base&>(a), static_cast<const ctpsa::base&>(b) ) ); }
    inline ctpsa pow (const ctpsa& a,  const int    i){ return ctpsa( pow(static_cast<const ctpsa::base&>(a), i) ); }
    inline ctpsa pow (const ctpsa& a,  const cpx_t v){ return ctpsa( pow(static_cast<const ctpsa::base&>(a), v  )); }

    inline std::ostream& operator<<(std::ostream& strm, const ctpsa& a)
    {
        a.show(strm, 0);
        return strm;
    }

    /* add the different functions e.g. trigonometric functions or similar ones */
#ifdef GTPSA_FUNC_ARG1
#undef GTPSA_FUNC_ARG1
#endif
#ifdef GTPSA_FUNC_ARG1_NO_RET_ARG
#undef GTPSA_FUNC_ARG1_NO_RET_ARG
#endif
#ifdef GTPSA_FUNC_ARG1_WITH_RET_ARG
#undef GTPSA_FUNC_ARG1_WITH_RET_ARG
#endif
#define GTPSA_FUNC_ARG1_WITH_RET_ARG(fname) \
    inline void fname ## _ (const ctpsa& t, ctpsa* r){ return r->apply_with_return_object(t, mad::fname);  }
#define GTPSA_FUNC_ARG1_NO_RET_ARG(fname)                                \
    inline ctpsa fname (const ctpsa& t){ return apply<ctpsa>(t, fname ## _);  }
#define GTPSA_FUNC_ARG1(fname) GTPSA_FUNC_ARG1_WITH_RET_ARG(fname) GTPSA_FUNC_ARG1_NO_RET_ARG(fname)
#include <gtpsa/funcs.h>
#undef GTPSA_FUNC_ARG1_NO_RET_ARG
#undef GTPSA_FUNC_ARG1_WITH_RET_ARG
#undef GTPSA_FUNC_ARG1

} // namespace gtsa


namespace gtpsa {

}


#endif /* _GTPSA_CTPSA_H_ */
