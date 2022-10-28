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

extern "C" {
#include <mad_ctpsa.h>
}

#include <gtpsa/tpsa.hpp>
#ifdef GTPSA_CLASS
#undef GTPSA_CLASS
#endif
#define GTPSA_CLASS(clsname) CTPSA ## clsname
#ifdef GTPSA_METH
#undef GTPSA_METH
#endif
#define GTPSA_METH(func) mad_ctpsa_ ## func
#ifdef T
#undef T
#endif
#define T cnum_t
#ifdef P
#undef P
#endif
#define P ctpsa_t
#include <gtpsa/common.tpp>

namespace gtpsa {
    typedef GTPSA_CLASS(WithOp) CTpsaWithOp;
}
/*
  #undef GTPSA_CLASS
  #undef GTPSA_METH
  #undef T
  #undef P
*/

namespace gtpsa {

    class ctpsa : public CTpsaWithOp {
    public:
	inline ctpsa(std::shared_ptr<desc> desc, const ord_t mo)
	    : CTpsaWithOp(desc, mo)
	    {}

	inline ctpsa(const tpsa&              t, const ord_t mo)
	    : CTpsaWithOp(t.getDescription(),   mo)
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

	/*
	 *  @warning: no compatible version in mad_ctpsa ...
	 *  @todo: review method
	 */
	// inline void show(std::ostream& strm, int level) const override final {
	//
	//     strm << "gctpsa  cst:\n\t" << this->cst();
	//     if(this->ord()){
	// 	// at least first order ...
	// 	auto nv = this->getDescription()->getNv(0, 0, 0);
	// 	std::vector<cnum_t> v(nv);
	// 	this->getv(1, &v);
	//
	// 	strm  << "\ng ctpsa linear :\n"
	// 	      << std::scientific << std::setw(20);
	// 	for(auto& e: v) strm <<  std::scientific << std::setw(20) << e << " ";
	//     }
	//     strm << "\n";
	// }


}; // class ctpsa


} // namespace gtsa




#endif /* _GTPSA_CTPSA_H_ */
