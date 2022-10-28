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
#ifdef T
#undef T
#endif
#define T num_t
#ifdef P
#undef P
#endif
#define P tpsa_t
#include <gtpsa/common.tpp>

namespace gtpsa {
    typedef GTPSA_CLASS(WithOp) TpsaWithOp;
}
/*
  #undef GTPSA_CLASS
  #undef GTPSA_METH
  #undef T
  #undef P
*/

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
	inline tpsa(const tpsa&              t,  const ord_t mo)
	    : TpsaWithOp(t,   mo)
	    {}
	/*
	inline tpsa& operator= (const num_t && o) {
	    // this->clear();
	    this->set(0, o);
	    return *this;
	}
	*/

#ifndef GTSPA_ONLY_OPTIMISED_OPS
	inline tpsa(const tpsa& o)
	    : TpsaWithOp(o)
	    {}
	inline tpsa(const TpsaWithOp& o)
	    : TpsaWithOp(o)
	    {}
#else /* GTSPA_ONLY_OPTIMISED_OPS */
	inline tpsa(const tpsa&              o) = delete;
#endif
	tpsa clone(void) const { return static_cast<tpsa>( TpsaWithOp::clone());  }

	inline tpsa& operator =  (const TpsaWithOp& o ) { this->_copyInPlace(o); return *this; }
	inline tpsa& operator =  (const num_t o )       { this->set(0, o); return *this; }
	inline tpsa& operator =  (const tpsa& o )       { this->_copyInPlace(o); return *this; }

	inline bool  operator >= (const num_t a ) const { return this->cst() >= a; }
	inline bool  operator <= (const num_t a ) const { return this->cst() <= a; }
	inline bool  operator >  (const num_t a ) const { return this->cst() >  a; }
	inline bool  operator <  (const num_t a ) const { return this->cst() <  a; }


	inline tpsa& operator += (const tpsa& o ) { add(*this, o, this); return *this; }
	inline tpsa& operator -= (const tpsa& o ) { sub(*this, o, this); return *this; }
	inline tpsa& operator *= (const tpsa& o ) { mul(*this, o, this); return *this; }
	inline tpsa& operator /= (const tpsa& o ) { div(*this, o, this); return *this; }

	/*
	 * do I need to repeat it here ?
	 */
	/*
	 * investigate if the same ones exist for complex type
	 */
	inline tpsa& operator += (const double o ){ mad_tpsa_set0(this->getPtr(),    1e0,    o); return *this; }
	inline tpsa& operator -= (const double o ){ mad_tpsa_set0(this->getPtr(),    1e0,   -o); return *this; }
	inline tpsa& operator *= (const double o ){ mad_tpsa_set0(this->getPtr(),      o,  0e0); return *this; }
	inline tpsa& operator /= (const double o ){ mad_tpsa_set0(this->getPtr(),  1e0/o,  0e0); return *this; }

	/*
	 *  @warning: no compatible version in mad_tpsa ...
	 *  @todo: review method
	 */
	// void show(std::ostream& strm, int level) const override final {
	//
	//     strm << "gtpsa  cst:\n\t" << this->cst();
	//     if(this->ord()){
	// 	// at least first order ...
	// 	auto nv = this->getDescription()->getNv(0, 0, 0);
	// 	std::vector<num_t> v(nv);
	// 	this->getv(1, &v);
	//
	// 	strm  << "\ngtpsa linear :\n"
	// 	      << std::scientific << std::setw(20);
	// 	for(auto& e: v) strm <<  std::scientific << std::setw(20) << e << " ";
	//     }
	//     strm << "\n";
	// }


	/*
	 * helper functions for the different operators
	 * here if no copies are required
	 */
	friend inline auto equ  (const tpsa& a, const tpsa& b, num_t tol);
	friend inline auto norm (const tpsa& a);

	friend inline auto ordn (const std::vector<const tpsa&> objs);

    }; // class tpsa


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
