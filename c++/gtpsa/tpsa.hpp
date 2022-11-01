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

extern "C" {
#include <mad_tpsa.h>
}

#ifdef GTPSA_CLASS
#undef GTPSA_CLASS
#endif
#define GTPSA_CLASS(clsname) TPSA ## clsname
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
#include <gtpsa/common.tpp>

namespace gtpsa {
    typedef GTPSA_CLASS(WithOp) TpsaWithOp;
}

#ifndef GTPSA_KEEP_MACROS
#undef GTPSA_CLASS
#undef GTPSA_METH
#undef T
#undef GTPSA_PTR_T
#endif /*  GTPSA_KEEP_MACROS */

namespace gtpsa {

    /*
     * @brief tpsa object
     *
     * Provides the methods as of the original
     */
    class tpsa : public TpsaWithOp {
    public:
	inline tpsa(std::shared_ptr<desc> desc, const ord_t mo)
	    : TpsaWithOp(desc, mo)
	    {}

	inline tpsa(const tpsa&              t, const ord_t mo)
	    : TpsaWithOp(t,   mo)
	    {}

	inline tpsa(      tpsa&&             o) = default;
	inline tpsa(      TpsaWithOp&&       o)  noexcept
	    :  TpsaWithOp(std::move(o))
	    { };

#ifndef GTSPA_ONLY_OPTIMISED_OPS

	// inline tpsa(const tpsa& o)
	//    : TpsaWithOp(o)
	//    {}

	//inline tpsa(const TpsaWithOp& o)
	//    : TpsaWithOp(o)
	//    {}
	inline tpsa(const tpsa&              o) = default;

#else /* GTSPA_ONLY_OPTIMISED_OPS */

	inline tpsa(const tpsa&              o) = delete;

#endif
	tpsa clone(void) const { auto tmp = tpsa(*this, gtpsa::init::same); tmp._copyInPlace(*this); return tmp; }

	inline tpsa& operator =  (const num_t o )       { TpsaWithOp::operator=(o); return *this; }
	inline tpsa& operator =  (const tpsa& o )       { TpsaWithOp::operator=(o); return *this; }
	// inline tpsa& operator =  (const TpsaWithOp& o ) { this->_copyInPlace(o); return *this; }
	// inline tpsa& operator =  (const num_t o )       { this->set(0, o); return *this; }

	inline bool  operator >= (const num_t a ) const { return this->cst() >= a; }
	inline bool  operator <= (const num_t a ) const { return this->cst() <= a; }
	inline bool  operator >  (const num_t a ) const { return this->cst() >  a; }
	inline bool  operator <  (const num_t a ) const { return this->cst() <  a; }

	inline tpsa  operator  - ( void         ) const { return tpsa( TpsaWithOp::operator-(*this) ); }

	inline tpsa& operator += (const tpsa& o ) { TpsaWithOp::operator += (o) ; return *this; }
	inline tpsa& operator -= (const tpsa& o ) { TpsaWithOp::operator -= (o) ; return *this; }
	inline tpsa& operator *= (const tpsa& o ) { TpsaWithOp::operator *= (o) ; return *this; }
	inline tpsa& operator /= (const tpsa& o ) { TpsaWithOp::operator /= (o) ; return *this; }

	inline tpsa& operator += (const num_t o ) { TpsaWithOp::operator += (o) ; return *this; }
	inline tpsa& operator -= (const num_t o ) { TpsaWithOp::operator -= (o) ; return *this; }
	inline tpsa& operator *= (const num_t o ) { TpsaWithOp::operator *= (o) ; return *this; }
	inline tpsa& operator /= (const num_t o ) { TpsaWithOp::operator /= (o) ; return *this; }

	inline tpsa  operator +  ( const tpsa&  o ) const { return tpsa( TpsaWithOp::operator+ (o)  ) ; }
	inline tpsa  operator -  ( const tpsa&  o ) const { return tpsa( TpsaWithOp::operator- (o)  ) ; }
	inline tpsa  operator *  ( const tpsa&  o ) const { return tpsa( TpsaWithOp::operator* (o)  ) ; }
	inline tpsa  operator /  ( const tpsa&  o ) const { return tpsa( TpsaWithOp::operator/ (o)  ) ; }

	inline tpsa  operator +  ( const double o ) const { return tpsa( TpsaWithOp::operator+ (o)  ) ; }
	inline tpsa  operator -  ( const double o ) const { return tpsa( TpsaWithOp::operator- (o)  ) ; }
	inline tpsa  operator *  ( const double o ) const { return tpsa( TpsaWithOp::operator* (o)  ) ; }
	inline tpsa  operator /  ( const double o ) const { return tpsa( TpsaWithOp::operator/ (o)  ) ; }

	/*
	 * helper functions for the different operators
	 * here if no copies are required
	 */
	friend inline auto equ  (const tpsa& a, const tpsa& b, num_t tol);
	friend inline auto norm (const tpsa& a);

	friend inline auto ordn (const std::vector<const tpsa&> objs);

    }; // class tpsa

    inline tpsa operator +  (const num_t a, const tpsa& b)  { return a + TpsaWithOp(b); }
    inline tpsa operator -  (const num_t a, const tpsa& b)  { return a - TpsaWithOp(b); }
    inline tpsa operator *  (const num_t a, const tpsa& b)  { return a * TpsaWithOp(b); }
    inline tpsa operator /  (const num_t a, const tpsa& b)  { return a / TpsaWithOp(b); }

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

    inline auto norm  (const tpsa& a)                           { return mad_tpsa_nrm (a.getPtr()); }
    inline auto equ   (const tpsa& a, const tpsa& b, num_t tol) { return mad_tpsa_equ (a.getPtr(), b.getPtr(), tol); }

} // namespace gtsa




#endif /* _GTPSA_TPSA_H_ */
