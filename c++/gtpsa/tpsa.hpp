#ifndef _GTPSA_TPSA_H_
#define _GTPSA_TPSA_H_ 1
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
#include <ostream>

#include <gtpsa/mad/tpsa_wrapper.hpp>
#include <gtpsa/intern/templated_funcs.hpp>
#include <gtpsa/bridge/bridge.hpp>
#include <gtpsa/intern/with_operators.hpp>


namespace gtpsa {

    /*
     * Implementation split up in three parts:
     *
     * 1. bridge: defined as template
     * 2. operator functions using the bridge
     * 3. class using c++ operators defined in a template
     * 4. class providing full functionality derived from the template
     *
     * This splits the functionality in different parts. Hopefully that
     * makes the code a little more maintainable
     */

    // Type information for the bridge template
    typedef GTpsaTypeInfo<tpsa_t, num_t, mad::TpsaWrapper, mad::_TpsaWrapper, mad::_TpsaContainerWrapper>  TpsaTypeInfo;

    /*
     * @brief tpsa object
     *
     * Bridges over to the original object
     */
    typedef TpsaBridge<TpsaTypeInfo> tpsa_bridge;


    /* c++ style functionality of the operator functions. */
    inline void radd (const tpsa_bridge& a, const tpsa_bridge& b, tpsa_bridge* r)  { r->apply2_with_return_object(a, b, mad::add ); }
    inline void rsub (const tpsa_bridge& a, const tpsa_bridge& b, tpsa_bridge* r)  { r->apply2_with_return_object(a, b, mad::sub ); }
    inline void rmul (const tpsa_bridge& a, const tpsa_bridge& b, tpsa_bridge* r)  { r->apply2_with_return_object(a, b, mad::mul ); }
    inline void rdiv (const tpsa_bridge& a, const tpsa_bridge& b, tpsa_bridge* r)  { r->apply2_with_return_object(a, b, mad::div ); }

    inline void racc     (const tpsa_bridge& a, const num_t v, tpsa_bridge* r) { r->apply2_base_with_return_object(a, v, mad::acc ); }
    inline void rscl     (const tpsa_bridge& a, const num_t v, tpsa_bridge* r) { r->apply2_base_with_return_object(a, v, mad::scl ); }
    inline void rinv     (const tpsa_bridge& a, const num_t v, tpsa_bridge* r) { r->apply2_base_with_return_object(a, v, mad::inv ); }
    inline void rinvsqrt (const tpsa_bridge& a, const num_t v, tpsa_bridge* r) { r->apply2_base_with_return_object(a, v, mad::invsqrt ); }

    inline void rpow (const tpsa_bridge& a, const tpsa_bridge& b, tpsa_bridge *r) { r->pow(a, b); }
    inline void rpow (const tpsa_bridge& a, const int          i, tpsa_bridge *r) { r->pow(a, i); }
    inline void rpow (const tpsa_bridge& a, const num_t        v, tpsa_bridge *r) { r->pow(a, v); }

    inline tpsa_bridge pow (const tpsa_bridge& a, const tpsa_bridge& b) { tpsa_bridge r = a.newFromThis(); rpow(a, b, &r); return r;}
    inline tpsa_bridge pow (const tpsa_bridge& a, const int          i) { tpsa_bridge r = a.newFromThis(); rpow(a, i, &r); return r;}
    inline tpsa_bridge pow (const tpsa_bridge& a, const num_t        v) { tpsa_bridge r = a.newFromThis(); rpow(a, v, &r); return r;}


    inline tpsa_bridge add (const tpsa_bridge& a, const tpsa_bridge& b ) { return apply2<tpsa_bridge>(a, b, radd); }
    inline tpsa_bridge sub (const tpsa_bridge& a, const tpsa_bridge& b ) { return apply2<tpsa_bridge>(a, b, rsub); }
    inline tpsa_bridge mul (const tpsa_bridge& a, const tpsa_bridge& b ) { return apply2<tpsa_bridge>(a, b, rmul); }
    inline tpsa_bridge div (const tpsa_bridge& a, const tpsa_bridge& b ) { return apply2<tpsa_bridge>(a, b, rdiv); }

    inline tpsa_bridge acc     (const tpsa_bridge& a, const num_t b)  { return apply2_base<tpsa_bridge, num_t>(a, b, racc); }
    inline tpsa_bridge scl     (const tpsa_bridge& a, const num_t b)  { return apply2_base<tpsa_bridge, num_t>(a, b, rscl); }
    inline tpsa_bridge inv     (const tpsa_bridge& a, const num_t b)  { return apply2_base<tpsa_bridge, num_t>(a, b, rinv); }
    inline tpsa_bridge invsqrt (const tpsa_bridge& a, const num_t b)  { return apply2_base<tpsa_bridge, num_t>(a, b, rinvsqrt); }

#if 0
    struct TpsaTypeBridgeInfo {
	using base_type = num_t;
	using bridge = tpsa_bridge;
    };
#endif
    /*
     * @brief truncated power series
     *
     * @todo add missing functionality
     */
    typedef TpsaWithOp<TpsaTypeInfo> tpsa_with_op ;

    class tpsa : public tpsa_with_op {

    public:
	using base = tpsa_with_op;

	inline tpsa(std::shared_ptr<mad::desc> desc, const ord_t mo)
	    : base(desc, mo)
	    {}

	inline tpsa(const tpsa&              t, const ord_t mo)
	    : base(t,   mo)
	    {}

	inline tpsa(      tpsa&&             o) = default;
	inline tpsa(      base&&       o)  noexcept
	    :  base(std::move(o))
	    { };

#ifndef GTSPA_ONLY_OPTIMISED_OPS

	// inline tpsa(const tpsa& o)
	//    : base(o)
	//    {}

	inline tpsa(const base& o)
	    : base(o)
	    {}

	inline tpsa(const tpsa&              o) = default;

#else /* GTSPA_ONLY_OPTIMISED_OPS */

	inline tpsa(const tpsa&              o) = delete;

#endif
	tpsa clone(void)       const { auto tmp = tpsa(*this, mad::init::same); tmp._copyInPlace(*this); return tmp; }
	/**
	 *
	 * @todo: rename to allocateLikeMe?
	 */
	tpsa newFromThis(void) const { return tpsa(base::newFromThis()); }

	inline tpsa& operator =  (const num_t o )       { base::operator=(o); return *this; }
	inline tpsa& operator =  (const tpsa& o )       { base::operator=(o); return *this; }
	// inline tpsa& operator =  (const base& o ) { this->_copyInPlace(o); return *this; }
	// inline tpsa& operator =  (const num_t o )       { this->set(0, o); return *this; }
	inline bool  operator >= (const num_t a ) const { return this->cst() >= a; }
	inline bool  operator <= (const num_t a ) const { return this->cst() <= a; }
	inline bool  operator >  (const num_t a ) const { return this->cst() >  a; }
	inline bool  operator <  (const num_t a ) const { return this->cst() <  a; }

	inline tpsa  operator  - ( void         ) const { return tpsa( base::operator-(*this) ); }

	inline tpsa& operator += (const tpsa& o ) { base::operator += (o) ; return *this; }
	inline tpsa& operator -= (const tpsa& o ) { base::operator -= (o) ; return *this; }
	inline tpsa& operator *= (const tpsa& o ) { base::operator *= (o) ; return *this; }
	inline tpsa& operator /= (const tpsa& o ) { base::operator /= (o) ; return *this; }

	inline tpsa& operator += (const num_t o ) { base::operator += (o) ; return *this; }
	inline tpsa& operator -= (const num_t o ) { base::operator -= (o) ; return *this; }
	inline tpsa& operator *= (const num_t o ) { base::operator *= (o) ; return *this; }
	inline tpsa& operator /= (const num_t o ) { base::operator /= (o) ; return *this; }


	inline tpsa  operator +  ( const tpsa&  o ) const { return tpsa( base::operator+ (o)  ) ; }
	inline tpsa  operator -  ( const tpsa&  o ) const { return tpsa( base::operator- (o)  ) ; }
	inline tpsa  operator *  ( const tpsa&  o ) const { return tpsa( base::operator* (o)  ) ; }
	inline tpsa  operator /  ( const tpsa&  o ) const { return tpsa( base::operator/ (o)  ) ; }

	inline tpsa  operator +  ( const double o ) const { return tpsa( base::operator+ (o)  ) ; }
	inline tpsa  operator -  ( const double o ) const { return tpsa( base::operator- (o)  ) ; }
	inline tpsa  operator *  ( const double o ) const { return tpsa( base::operator* (o)  ) ; }
	inline tpsa  operator /  ( const double o ) const { return tpsa( base::operator/ (o)  ) ; }
	/*
	 * helper functions for the different operators
	 * here if no copies are required
	 */
	friend inline auto equ  (const tpsa& a, const tpsa& b, num_t tol);
	friend inline auto norm (const tpsa& a);

	friend inline auto ordn (const std::vector<const tpsa&> objs);

    friend class ctpsa;
    }; // class tpsa


    inline tpsa operator +  (const num_t a, const tpsa& b) { return a + static_cast<const tpsa::base&>(b); }
    inline tpsa operator -  (const num_t a, const tpsa& b) { return a - static_cast<const tpsa::base&>(b); }
    inline tpsa operator *  (const num_t a, const tpsa& b) { return a * static_cast<const tpsa::base&>(b); }
    inline tpsa operator /  (const num_t a, const tpsa& b) { return a / static_cast<const tpsa::base&>(b); }

    inline tpsa pow (const tpsa& a,  const tpsa& b){ return tpsa( pow(static_cast<const tpsa::base&>(a), static_cast<const tpsa::base&>(b) ) ); }
    inline tpsa pow (const tpsa& a,  const int   i){ return tpsa( pow(static_cast<const tpsa::base&>(a), i) ); }
    inline tpsa pow (const tpsa& a,  const num_t v){ return tpsa( pow(static_cast<const tpsa::base&>(a), v  )); }


    /**
     * @brief get maximum number of all
     */
    /*
    inline auto ordn(const std::vector<const tpsa&> objs) {
	std::vector<const tpsa_t *> tv;
	tv.reserve(objs.size());
	auto toPtr = [](const tpsa& obj){ return obj.getPtr();};
	std::transform(objs.begin(), objs.end(), std::back_inserter(tv.begin()), toPtr);
	return mad_tpsa_ordn(tv.size(), tv.data());
    }
    */

    inline std::ostream& operator<<(std::ostream& strm, const tpsa& a)
    {
	a.show(strm, 0);
	return strm;
    }

/*
 * Trigonometric and similar functions ...
 */
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
    inline void fname ## _ (const tpsa& t, tpsa* r){ r->apply_with_return_object(t, mad::fname);  }
#define GTPSA_FUNC_ARG1_NO_RET_ARG(fname)				\
    inline tpsa fname (const tpsa& t){ return apply<tpsa>(t, fname ## _);  }
#define GTPSA_FUNC_ARG1(fname) GTPSA_FUNC_ARG1_WITH_RET_ARG(fname) GTPSA_FUNC_ARG1_NO_RET_ARG(fname)
#include <gtpsa/funcs.h>
#undef GTPSA_FUNC_ARG1_NO_RET_ARG
#undef GTPSA_FUNC_ARG1_WITH_RET_ARG
#undef GTPSA_FUNC_ARG1

} // namespace gtsa




#endif /* _GTPSA_TPSA_H_ */
