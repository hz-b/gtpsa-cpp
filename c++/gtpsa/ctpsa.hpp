#ifndef _GTPSA_CTPSA_H_
#define _GTPSA_CTPSA_H_ 1
/**
 * @brief wrapper around the  Truncated Power Series Algebra module interface
 *
 * As distributed as part of the MAD package see
 * https://cern.ch/mad for details
 *
 * This wrapper provides "nearly direct access" to the original C functions
 * and provides operator access to these functions too.
 *
 * @todo: add wrapper for more advanced functions
 */
#include <gtpsa/desc.hpp>
#include <algorithm>
#include <cassert>
#include <iomanip>
#include <memory>
#include <ostream>
#include <vector>
#include <complex>

extern "C" {
#include <mad_ctpsa.h>
}

#include <gtpsa/tpsa.hpp>
#include <gtpsa/complex_utils.hpp>
#ifdef GTPSA_CLASS
#undef GTPSA_CLASS
#endif
#define GTPSA_CLASS(clsname) CTPSA ## clsname
#ifdef GTPSA_METH
#undef GTPSA_METH
#endif
#define GTPSA_METH(func) mad_ctpsa_ ## func
#ifdef GTPSA_BASE_T
#undef GTPSA_BASE_T
#endif
#define GTPSA_BASE_T cnum_t
#ifdef GTPSA_PTR_T
#undef GTPSA_PTR_T
#endif
#define GTPSA_PTR_T ctpsa_t
#include <gtpsa/common.tpp>

#include <complex>
namespace gtpsa {
    typedef GTPSA_CLASS(WithOp) CTpsaWithOp;
}


#ifndef GTPSA_KEEP_MACROS
#undef GTPSA_CLASS
#undef GTPSA_METH
#undef T
#undef GTPSA_PTR_T
#endif /*  GTPSA_KEEP_MACROS */


namespace gtpsa {

    class ctpsa : public CTpsaWithOp {
    public:
	inline ctpsa(std::shared_ptr<desc> desc, const ord_t mo)
	    : CTpsaWithOp(desc, mo)
	    {}

	inline ctpsa(const ctpsa&             t, const ord_t mo)
	    : CTpsaWithOp(t.getDescription(),   mo)
	    {}

	inline ctpsa(const tpsa&              t, const ord_t mo)
	    : CTpsaWithOp(t.getDescription(),   mo)
	    {}

	inline ctpsa(const CTpsaWithOp&& o)
	    : CTpsaWithOp(std::move(o))
	    {}

	/**
	 * @brief a*x[0]+b
	 */
	inline void set(                            num_t a_re, num_t a_im, num_t b_re, num_t b_im) { mad_ctpsa_set0_r (this->getPtr()                    , a_re, a_im,  b_re, b_im  ); }

	/**
	 * @brief a*x[i]+b
	 */
	inline void set(const idx_t i             , num_t a_re, num_t a_im, num_t b_re, num_t b_im) { mad_ctpsa_seti_r (this->getPtr(), i                 , a_re, a_im,  b_re, b_im ); }
	/**
	 * @brief a*x[m]+b
	 */
	inline void set(std::string s             , num_t a_re, num_t a_im, num_t b_re, num_t b_im) { mad_ctpsa_sets_r (this->getPtr(), s.size(), s.data(), a_re, a_im,  b_re, b_im ); }
	/**
	 * @brief a*x[m]+b
	 */
	inline void set(const std::vector<ord_t> m, num_t a_re, num_t a_im, num_t b_re, num_t b_im) { mad_ctpsa_setm_r (this->getPtr(), m.size(), m.data(), a_re, a_im,  b_re, b_im ); }

	/**
	 * @brief a*x[m]+b, sparse mono [(i,o)]
	 * @todo vector of pairs?
	 */
	inline auto setsm(const std::vector<int> m, num_t a_re, num_t a_im, num_t b_re, num_t b_im) { return mad_ctpsa_setsm_r  (this->getPtr(), m.size(), m.data(), a_re, a_im,  b_re, b_im ); }

	inline void getv(idx_t i,       std::vector<cnum_t> *v) const { mad_ctpsa_getv(this->getPtr(), i, v->size(), v->data() ); }
	inline void setv(idx_t i, const std::vector<cnum_t> &v)       { mad_ctpsa_setv(this->getPtr(), i, v.size() , v.data()  ); }

	inline void print(str_t name_, num_t eps_, int nohdr_, FILE *stream_){
	    mad_ctpsa_print(this->getPtr(), name_, eps_, nohdr_, stream_);
	}

	/* compatible to complex part */
	inline void set(                             const std::complex<double> a, const std::complex<double> b) {
	    CTpsaWithOp::set(std_complex_double_to_cnum_t(a), std_complex_double_to_cnum_t(b));
	}
	inline void set(const idx_t i,               const std::complex<double> a, const std::complex<double> b) {
	    CTpsaWithOp::set(i, std_complex_double_to_cnum_t(a), std_complex_double_to_cnum_t(b));
	}

	inline void set(const std::string& s,        const std::complex<double> a, const std::complex<double> b) {
	    CTpsaWithOp::set(s, std_complex_double_to_cnum_t(a), std_complex_double_to_cnum_t(b));
	}

	inline void set(const std::vector<ord_t>& m, const std::complex<double> a, const std::complex<double> b) {
	    CTpsaWithOp::set(m, std_complex_double_to_cnum_t(a), std_complex_double_to_cnum_t(b));
	}

	inline void setsm(const std::vector<int>& m, const std::complex<double> a, const std::complex<double> b) {
	    CTpsaWithOp::setsm(m, std_complex_double_to_cnum_t(a), std_complex_double_to_cnum_t(b));
	}

	inline ctpsa& operator += (const ctpsa& o ) { CTpsaWithOp::operator += (o) ; return *this; }
	inline ctpsa& operator -= (const ctpsa& o ) { CTpsaWithOp::operator -= (o) ; return *this; }
	inline ctpsa& operator *= (const ctpsa& o ) { CTpsaWithOp::operator *= (o) ; return *this; }
	inline ctpsa& operator /= (const ctpsa& o ) { CTpsaWithOp::operator /= (o) ; return *this; }


	inline ctpsa& operator += (const cnum_t o ) { CTpsaWithOp::operator += (o) ; return *this; }
	inline ctpsa& operator -= (const cnum_t o ) { CTpsaWithOp::operator -= (o) ; return *this; }
	inline ctpsa& operator *= (const cnum_t o ) { CTpsaWithOp::operator *= (o) ; return *this; }
	inline ctpsa& operator /= (const cnum_t o ) { CTpsaWithOp::operator /= (o) ; return *this; }


	inline ctpsa  operator  - ( void         ) const { return ctpsa( CTpsaWithOp::operator-(*this) ); }

	/*
	 * add operators that accept std::complex<double> ... needs to be converted to cnum_t
	 */
	inline ctpsa& operator += ( const std::complex<double>& o )       { return operator += (std_complex_double_to_cnum_t(o)) ; }
	inline ctpsa& operator -= ( const std::complex<double>& o )       { return operator -= (std_complex_double_to_cnum_t(o)) ; }
	inline ctpsa& operator *= ( const std::complex<double>& o )       { return operator *= (std_complex_double_to_cnum_t(o)) ; }
	inline ctpsa& operator /= ( const std::complex<double>& o )       { return operator /= (std_complex_double_to_cnum_t(o)) ; }

	inline ctpsa  operator +  ( const ctpsa&  o ) const { return ctpsa( std::move( CTpsaWithOp::operator+ (o) ) ) ; }
	inline ctpsa  operator -  ( const ctpsa&  o ) const { return ctpsa( std::move( CTpsaWithOp::operator- (o) ) ) ; }
	inline ctpsa  operator *  ( const ctpsa&  o ) const { return ctpsa( std::move( CTpsaWithOp::operator* (o) ) ) ; }
	inline ctpsa  operator /  ( const ctpsa&  o ) const { return ctpsa( std::move( CTpsaWithOp::operator/ (o) ) ) ; }

	inline ctpsa  operator +  ( const cnum_t o ) const { return ctpsa( CTpsaWithOp::operator+ (std_complex_double_to_cnum_t(o)) ) ; }
	inline ctpsa  operator -  ( const cnum_t o ) const { return ctpsa( CTpsaWithOp::operator- (std_complex_double_to_cnum_t(o)) ) ; }
	inline ctpsa  operator *  ( const cnum_t o ) const { return ctpsa( CTpsaWithOp::operator* (std_complex_double_to_cnum_t(o)) ) ; }
	inline ctpsa  operator /  ( const cnum_t o ) const { return ctpsa( CTpsaWithOp::operator/ (std_complex_double_to_cnum_t(o)) ) ; }

	inline ctpsa  operator +  ( const std::complex<double>& o ) const { return ctpsa( this->operator+ (std_complex_double_to_cnum_t(o)) ) ; }
	inline ctpsa  operator -  ( const std::complex<double>& o ) const { return ctpsa( this->operator- (std_complex_double_to_cnum_t(o)) ) ; }
	inline ctpsa  operator *  ( const std::complex<double>& o ) const { return ctpsa( this->operator* (std_complex_double_to_cnum_t(o)) ) ; }
	inline ctpsa  operator /  ( const std::complex<double>& o ) const { return ctpsa( this->operator/ (std_complex_double_to_cnum_t(o)) ) ; }


}; // class ctpsa


    inline ctpsa operator +  (const cnum_t a, const ctpsa& b)  { return a + CTpsaWithOp(b); }
    inline ctpsa operator -  (const cnum_t a, const ctpsa& b)  { return a - CTpsaWithOp(b); }
    inline ctpsa operator *  (const cnum_t a, const ctpsa& b)  { return a * CTpsaWithOp(b); }
    inline ctpsa operator /  (const cnum_t a, const ctpsa& b)  { return a / CTpsaWithOp(b); }

    inline ctpsa operator +  (const std::complex<double> a, const ctpsa& b)  { return std_complex_double_to_cnum_t(a) + CTpsaWithOp(b); }
    inline ctpsa operator -  (const std::complex<double> a, const ctpsa& b)  { return std_complex_double_to_cnum_t(a) - CTpsaWithOp(b); }
    inline ctpsa operator *  (const std::complex<double> a, const ctpsa& b)  { return std_complex_double_to_cnum_t(a) * CTpsaWithOp(b); }
    inline ctpsa operator /  (const std::complex<double> a, const ctpsa& b)  { return std_complex_double_to_cnum_t(a) / CTpsaWithOp(b); }



} // namespace gtsa




#endif /* _GTPSA_CTPSA_H_ */
