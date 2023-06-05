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
    typedef GTpsaTypeInfo<tpsa_t, num_t, tpsa, mad::TpsaWrapper, mad::_TpsaWrapper, mad::_TpsaContainerWrapper>  TpsaTypeInfo;

    /*
     * @brief tpsa object
     *
     * Bridges over to the original object
     */

    // namespace intern {
	class tpsa_bridge :  public TpsaBridge<TpsaTypeInfo> {
	protected:
	    using base = TpsaBridge<TpsaTypeInfo>;

	public:
	    inline tpsa_bridge(std::shared_ptr<mad::desc> desc, const ord_t mo)
		: base(desc, mo)
		{}

	    inline tpsa_bridge(tpsa_bridge&& o) = default;
	    inline tpsa_bridge(base&&        o)  noexcept
		:  base(std::move(o))
		{ };

	    inline void atan2 (const tpsa_bridge& y, const tpsa_bridge& x) { this->m_impl.atan2(y.m_impl, x.m_impl); }

	    inline void pow   (const tpsa_bridge& a, const tpsa_bridge& b) { ( (base*)(this) )->pow(a, b); }
	    inline void pow   (const tpsa_bridge& a, const int          i) { ( (base*)(this) )->pow(a, i); }
	    inline void pow   (const tpsa_bridge& a, const num_t        v) { ( (base*)(this) )->pow(a, v); }
	    	};
    // } // intern

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
    protected:
	using base = tpsa_with_op;
    public:

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

	inline void atan2(const tpsa&  y, const tpsa&   x) {
	    this->m_impl.atan2(y.m_impl, x.m_impl);
	}
	// for variant
	inline void atan2(const tpsa&  y, const double& x) {
	    auto tmp = y.newFromThis();
	    tmp.set(0, x);
	    this->atan2(y, tmp);
	}
	inline void atan2(const double& y,  const tpsa&   x) {
	    auto tmp = x.newFromThis();
	    tmp.set(0, y);
	    this->atan2(tmp, x);
	}

	inline tpsa& operator =  (const num_t o )       { base::operator=(o); return *this; }
	inline tpsa& operator =  (const tpsa& o )       { base::operator=(o); return *this; }
	// inline tpsa& operator =  (const base& o ) { this->_copyInPlace(o); return *this; }
	// inline tpsa& operator =  (const num_t o )       { this->set(0, o); return *this; }
	inline bool  operator >= (const num_t a ) const { return this->cst() >= a; }
	inline bool  operator <= (const num_t a ) const { return this->cst() <= a; }
	inline bool  operator >  (const num_t a ) const { return this->cst() >  a; }
	inline bool  operator <  (const num_t a ) const { return this->cst() <  a; }
	inline bool  operator == (const num_t a ) const { return this->cst() ==  a; }
	inline bool  operator == (const tpsa& a ) const { return this->cst() ==  a.cst(); }

	inline tpsa  operator  - ( void         ) const { return tpsa( base::operator-() ); }
#if 0
	inline tpsa& operator += (const tpsa& o ) { base::operator += (o) ; return *this; }
	inline tpsa& operator -= (const tpsa& o ) { base::operator -= (o) ; return *this; }
	inline tpsa& operator *= (const tpsa& o ) { base::operator *= (o) ; return *this; }
	inline tpsa& operator /= (const tpsa& o ) { base::operator /= (o) ; return *this; }

	inline tpsa& operator += (const num_t o ) { base::operator += (o) ; return *this; }
	inline tpsa& operator -= (const num_t o ) { base::operator -= (o) ; return *this; }
	inline tpsa& operator *= (const num_t o ) { base::operator *= (o) ; return *this; }
	inline tpsa& operator /= (const num_t o ) { base::operator /= (o) ; return *this; }
#endif
	inline tpsa  operator +  ( const double o ) const { return tpsa( base::operator+ (o)  ) ; }
	inline tpsa  operator -  ( const double o ) const { return tpsa( base::operator- (o)  ) ; }
	inline tpsa  operator *  ( const double o ) const { return tpsa( base::operator* (o)  ) ; }
	inline tpsa  operator /  ( const double o ) const { return tpsa( base::operator/ (o)  ) ; }

	inline tpsa  operator +  ( const tpsa&  o ) const { return tpsa( static_cast<const tpsa::base&>(*this) + static_cast<const tpsa::base&>(o) ) ; }
	inline tpsa  operator -  ( const tpsa&  o ) const { return tpsa( static_cast<const tpsa::base&>(*this) - static_cast<const tpsa::base&>(o) ) ; }
	inline tpsa  operator *  ( const tpsa&  o ) const { return tpsa( static_cast<const tpsa::base&>(*this) * static_cast<const tpsa::base&>(o) ) ; }
	inline tpsa  operator /  ( const tpsa&  o ) const { return tpsa( static_cast<const tpsa::base&>(*this) / static_cast<const tpsa::base&>(o) ) ; }

	/*
	 * helper functions for the different operators
	 * here if no copies are required
	 */
	friend inline auto equ  (const tpsa& a, const tpsa& b, num_t tol);
	friend inline auto norm (const tpsa& a);

	friend inline auto ordn (const std::vector<const tpsa&> objs);


	friend inline tpsa operator +  (const num_t a, const tpsa& b);
	friend inline tpsa operator -  (const num_t a, const tpsa& b);
	friend inline tpsa operator *  (const num_t a, const tpsa& b);
	friend inline tpsa operator /  (const num_t a, const tpsa& b);

#if 0
	friend inline tpsa  operator + ( const tpsa& a, const tpsa& b );
	friend inline tpsa  operator - ( const tpsa& a, const tpsa& b );
	friend inline tpsa  operator * ( const tpsa& a, const tpsa& b );
	friend inline tpsa  operator / ( const tpsa& a, const tpsa& b );
#endif
	friend inline tpsa deriv (const tpsa& a,  const int v);

	// required to implement real, imag etc..
	friend class ctpsa;
    }; // class tpsa

#if 0
    inline tpsa operator + ( const tpsa& a, const tpsa& b ) { return tpsa( static_cast<const tpsa::base&>(a) + static_cast<const tpsa::base&>(b) ) ; }
    inline tpsa operator - ( const tpsa& a, const tpsa& b ) { return tpsa( static_cast<const tpsa::base&>(a) - static_cast<const tpsa::base&>(b) ) ; }
    inline tpsa operator * ( const tpsa& a, const tpsa& b ) { return tpsa( static_cast<const tpsa::base&>(a) * static_cast<const tpsa::base&>(b) ) ; }
    inline tpsa operator / ( const tpsa& a, const tpsa& b ) { return tpsa( static_cast<const tpsa::base&>(a) / static_cast<const tpsa::base&>(b) ) ; }
#endif

    inline tpsa operator +  ( const num_t a, const tpsa& b ) { return a + static_cast<const tpsa::base&>(b); }
    inline tpsa operator -  ( const num_t a, const tpsa& b ) { return a - static_cast<const tpsa::base&>(b); }
    inline tpsa operator *  ( const num_t a, const tpsa& b ) { return a * static_cast<const tpsa::base&>(b); }
    inline tpsa operator /  ( const num_t a, const tpsa& b ) { return a / static_cast<const tpsa::base&>(b); }


    inline bool operator == (const num_t a, const tpsa& b) { return (b == a);}
    inline tpsa pow (const tpsa& a,  const tpsa& b){ tpsa n = a.newFromThis(); n.pow(a, b); return n; }
    inline tpsa pow (const tpsa& a,  const int   i){ tpsa n = a.newFromThis(); n.pow(a, i); return n; }
    inline tpsa pow (const tpsa& a,  const num_t v){ tpsa n = a.newFromThis(); n.pow(a, v); return n; }



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

    inline tpsa deriv (const tpsa& a,  const int v){
	return tpsa( deriv(static_cast<const tpsa::base&>(a), v  ));
    }

    inline tpsa atan2(const tpsa&   y, const tpsa&   x) { tpsa r = y.newFromThis(); r.atan2(y, x); return r; }
    inline tpsa atan2(const double& y, const tpsa&   x) { tpsa r = x.newFromThis(); r.atan2(y, x); return r; }
    inline tpsa atan2(const tpsa&   y, const double& x) { tpsa r = y.newFromThis(); r.atan2(y, x); return r; }

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
