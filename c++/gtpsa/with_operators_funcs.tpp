/* -*- c++ -*- */
namespace gtpsa {
    // ------------------------------------------------------------------------------
    // helper functions for different operators and mathematical functions
    // ------------------------------------------------------------------------------
    // inplace

    /**
     * @brief single argument in single argument out
     * @todo renaming required
     */
    inline void process1_  (const GTPSA_CLASS(WithOp)& a, GTPSA_CLASS(WithOp) *r,
			    void (*func)(const GTPSA_PTR_T* a, GTPSA_PTR_T *r) ) {
	func(a.getPtr(), r->getPtr());
    }

    /**
     * @brief single argument in, two arguments out
     */
    inline void process1to2_(const GTPSA_CLASS(WithOp)& a, GTPSA_CLASS(WithOp)* r1, GTPSA_CLASS(WithOp) *r2,
			     void (*func)(const GTPSA_PTR_T* a, GTPSA_PTR_T *r1,  GTPSA_PTR_T *r2) ) {
	func(a.getPtr(), r1->getPtr(), r2->getPtr());
    }
    /**
     * @brief two arguments in, single argument out
     */
    inline void process2_ (const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)& b, GTPSA_CLASS(WithOp) *r,
			   void (*func)(const GTPSA_PTR_T* a, const GTPSA_PTR_T* b, GTPSA_PTR_T *r) ) {
	func(a.getPtr(), b.getPtr(), r->getPtr());
    }

    inline void process2_  (const GTPSA_CLASS(WithOp)& a, const GTPSA_BASE_T b, GTPSA_CLASS(WithOp) *r,
			    void (*func)(const GTPSA_BASE_T  a, const GTPSA_BASE_T b, GTPSA_BASE_T *r) ) {
	GTPSA_BASE_T rv;
	func(GTPSA_METH(get0)(a.getPtr()), b, &rv);
	GTPSA_METH(set0)(r->getPtr(), 0.0, rv);
    }

    inline void process2_  (const GTPSA_CLASS(WithOp)& a, const GTPSA_BASE_T b, GTPSA_CLASS(WithOp) *r,
			    void (*func)(const GTPSA_PTR_T*  a, const GTPSA_BASE_T b, GTPSA_PTR_T *r) ) {
	func(a.getPtr(), b, r->getPtr());
    }

    // ------------------------------------------------------------------------------
    // return new instance
    /**
     * @brief single argument in, single argument out, return argument allocated
     */
    inline GTPSA_CLASS(WithOp) process1(const GTPSA_CLASS(WithOp)& t,
					void (*func)(const GTPSA_CLASS(WithOp)& t, GTPSA_CLASS(WithOp) *r) ){
	auto ret = t.newFromThis(); //GTPSA_CLASS(WithOp)(t, gtpsa::init::same);
	func(t, &ret);
	return ret;
    }

    /**
     * @brief two arguments in, single argument out, return argument allocated
     */
    inline GTPSA_CLASS(WithOp) process2(const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)&  b,
					void (*func)(const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)&  b, GTPSA_CLASS(WithOp) *r) ) {
	auto ret = a.newFromThis(); //GTPSA_CLASS(WithOp)(a, gtpsa::init::same);
	func(a, b, &ret);
	return ret;
    }

    inline GTPSA_CLASS(WithOp) process2(const GTPSA_CLASS(WithOp)& a, const GTPSA_BASE_T b,
					void (*func)(const GTPSA_CLASS(WithOp)& a, const GTPSA_BASE_T b, GTPSA_CLASS(WithOp) *r) ) {
	auto ret = a.newFromThis(); //GTPSA_CLASS(WithOp)(a, gtpsa::init::same);
	func(a, b, &ret);
	return ret;
    }
    // GTPSA_CLASS(WithOp) inline process2(const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)&  b, void (*func)(const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)&  b, GTPSA_CLASS(WithOp) *r) ) { auto ret = GTPSA_CLASS(WithOp)(a, gtpsa::init::same); func(a, b, &ret); return ret; }
    //
    // GTPSA_CLASS(WithOp) inline process2(const GTPSA_CLASS(WithOp)& a, const GTPSA_BASE_T b, void (*func)(const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)&  b, GTPSA_CLASS(WithOp) *r) ) { auto ret = GTPSA_CLASS(WithOp)(a, gtpsa::init::same); ret.set(0, b); func(a, ret, &ret); return ret; }

    // ------------------------------------------------------------------------------
    // End of helper functiosn
    // ------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------
    // mathematical functions using helper functions
    // inplace versions


    /**
     * @brief single argument in, single argument out, return argument allocated
     */
    /**
     * @brief c += v*a, aliasing OK
     */
    inline void acc     (const GTPSA_CLASS(WithOp) &a, const GTPSA_BASE_T v, GTPSA_CLASS(WithOp) *c) { process2_(a, v, c, &GTPSA_METH(acc)    ); }
    /**
     * @brief c = v*a, aliasing OK ?
     */
    inline void scl     (const GTPSA_CLASS(WithOp) &a, const GTPSA_BASE_T v, GTPSA_CLASS(WithOp) *c) { process2_(a, v, c, &GTPSA_METH(scl)    ); }
    /**
     * @brief c = v/a, aliasing OK ?
     */
    inline void inv     (const GTPSA_CLASS(WithOp) &a, const GTPSA_BASE_T v, GTPSA_CLASS(WithOp) *c) { process2_(a, v, c, &GTPSA_METH(inv)    ); }
    /**
     * @brief c = v/sqrt(a), aliasing OK ?
     */
    inline void invsqrt (const GTPSA_CLASS(WithOp) &a, const GTPSA_BASE_T v, GTPSA_CLASS(WithOp) *c) { process2_(a, v, c, &GTPSA_METH(invsqrt) ); }

    inline GTPSA_CLASS(WithOp) acc     (const GTPSA_CLASS(WithOp) &a, const GTPSA_BASE_T v ) { return process2 (a, v,      acc    ); }
    inline GTPSA_CLASS(WithOp) scl     (const GTPSA_CLASS(WithOp) &a, const GTPSA_BASE_T v ) { return process2 (a, v,      scl    ); }
    inline GTPSA_CLASS(WithOp) inv     (const GTPSA_CLASS(WithOp) &a, const GTPSA_BASE_T v ) { return process2 (a, v,      inv    ); }
    inline GTPSA_CLASS(WithOp) invsqrt (const GTPSA_CLASS(WithOp) &a, const GTPSA_BASE_T v ) { return process2 (a, v,      invsqrt); }

    inline void add   (const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)& b,  GTPSA_CLASS(WithOp)* r ){ process2_(a, b, r, GTPSA_METH(add)); }
    /**
     * @brief  (a_i-b_i)/max(|a_i|,1)
     */
    inline void dif   (const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)& b,  GTPSA_CLASS(WithOp)* r ){ process2_(a, b, r, GTPSA_METH(dif)); }
    inline void sub   (const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)& b,  GTPSA_CLASS(WithOp)* r ){ process2_(a, b, r, GTPSA_METH(sub)); }
    inline void mul   (const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)& b,  GTPSA_CLASS(WithOp)* r ){ process2_(a, b, r, GTPSA_METH(mul)); }
    inline void div   (const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)& b,  GTPSA_CLASS(WithOp)* r ){ process2_(a, b, r, GTPSA_METH(div)); }
    inline void pow   (const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)& b,  GTPSA_CLASS(WithOp)* r ){ process2_(a, b, r, GTPSA_METH(pow)); }

    inline void pow   (const GTPSA_CLASS(WithOp)& a,                        int n,  GTPSA_CLASS(WithOp)* r ){ GTPSA_METH(powi) (a.getPtr(), n, r->getPtr()); }
    inline void pow   (const GTPSA_CLASS(WithOp)& a,                          GTPSA_BASE_T v,  GTPSA_CLASS(WithOp)* r ){ GTPSA_METH(pown) (a.getPtr(), v, r->getPtr()); }

    inline void add_d (const GTPSA_CLASS(WithOp)& a, const GTPSA_BASE_T b, GTPSA_CLASS(WithOp)* r ){ process2_(a, b, r, [](GTPSA_BASE_T a, GTPSA_BASE_T b, GTPSA_BASE_T *r){ *r = a + b; }); }
    inline void sub_d (const GTPSA_CLASS(WithOp)& a, const GTPSA_BASE_T b, GTPSA_CLASS(WithOp)* r ){ process2_(a, b, r, [](GTPSA_BASE_T a, GTPSA_BASE_T b, GTPSA_BASE_T *r){ *r = a - b; }); }
    inline void mul_d (const GTPSA_CLASS(WithOp)& a, const GTPSA_BASE_T b, GTPSA_CLASS(WithOp)* r ){ process2_(a, b, r, [](GTPSA_BASE_T a, GTPSA_BASE_T b, GTPSA_BASE_T *r){ *r = a * b; }); }
    inline void div_d (const GTPSA_CLASS(WithOp)& a, const GTPSA_BASE_T b, GTPSA_CLASS(WithOp)* r ){ process2_(a, b, r, [](GTPSA_BASE_T a, GTPSA_BASE_T b, GTPSA_BASE_T *r){ *r = a / b; }); }

    inline GTPSA_CLASS(WithOp) add   (const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)& b           ){ return process2(a, b, add); }
    inline GTPSA_CLASS(WithOp) sub   (const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)& b           ){ return process2(a, b, sub); }
    inline GTPSA_CLASS(WithOp) mul   (const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)& b           ){ return process2(a, b, mul); }
    inline GTPSA_CLASS(WithOp) div   (const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)& b           ){ return process2(a, b, div); }


    // inline tpsa add_d (const tpsa& a, const GTPSA_BASE_T b          ){ return process2(a, b, add); }
    // inline tpsa sub_d (const tpsa& a, const GTPSA_BASE_T b          ){ return process2(a, b, sub); }
    // inline tpsa mul_d (const tpsa& a, const GTPSA_BASE_T b          ){ return process2(a, b, mul); }
    // inline tpsa div_d (const tpsa& a, const GTPSA_BASE_T b          ){ return process2(a, b, div); }

    /* standard mathematical functions ... trigonometic etc ... taking one argument returning one */



#ifdef GTPSA_FUNC_ARG1
#undef GTPSA_FUNC_ARG1
#endif
#ifdef GTPSA_FUNC_ARG1_WITH_RET_ARG
#undef GTPSA_FUNC_ARG1_WITH_RET_ARG
#endif
#ifdef GTPSA_FUNC_ARG1_NO_RET_ARG
#undef GTPSA_FUNC_ARG1_NO_RET_ARG
#endif
#define GTPSA_FUNC_ARG1_WITH_RET_ARG(fname)				\
    inline void fname ## _ (const GTPSA_CLASS(WithOp)& t, GTPSA_CLASS(WithOp)* r){ process1_(t, r, GTPSA_METH(fname) ); }

#define GTPSA_FUNC_ARG1_NO_RET_ARG(fname)				\
    inline GTPSA_CLASS(WithOp) fname (const GTPSA_CLASS(WithOp)& t){ return process1(t, fname ## _); }
#define GTPSA_FUNC_ARG1(fname) GTPSA_FUNC_ARG1_WITH_RET_ARG(fname) GTPSA_FUNC_ARG1_NO_RET_ARG(fname)
#include <gtpsa/funcs.h>
#undef GTPSA_FUNC_ARG1_NO_RET_ARG
#undef GTPSA_FUNC_ARG1_WITH_RET_ARG
#undef GTPSA_FUNC_ARG1

    inline void sincos_ (const GTPSA_CLASS(WithOp)& t, GTPSA_CLASS(WithOp)* r1, GTPSA_CLASS(WithOp)* r2) { process1to2_(t, r1, r2, GTPSA_METH(sincos)  ) ; }
    inline void sincosh_(const GTPSA_CLASS(WithOp)& t, GTPSA_CLASS(WithOp)* r1, GTPSA_CLASS(WithOp)* r2) { process1to2_(t, r1, r2, GTPSA_METH(sincosh) ) ; }

    inline void taylor_(const GTPSA_CLASS(WithOp)& a, std::vector<GTPSA_BASE_T> coeff, GTPSA_CLASS(WithOp)* c){ GTPSA_METH(taylor)(a.getPtr(), coeff.size(), coeff.data(), c->getPtr()); }
    inline GTPSA_CLASS(WithOp) taylor(const GTPSA_CLASS(WithOp)& a, std::vector<GTPSA_BASE_T> coeff) {
	auto ret = a.newFromThis(); //GTPSA_CLASS(WithOp)(a, gtpsa::init::same);
	taylor_(a, coeff, &ret);
	return ret;
    }

    inline GTPSA_CLASS(WithOp) pow (const GTPSA_CLASS(WithOp)& a, int n         ){ auto r = a.newFromThis(); pow(a, n, &r); return r; }
    inline GTPSA_CLASS(WithOp) pow (const GTPSA_CLASS(WithOp)& a, GTPSA_BASE_T v){ auto r = a.newFromThis(); pow(a, v, &r); return r; }


} // namespace gtpsa
