/* -*- c++ -*- */
/* sentinel macros intentionally ommitted */
/**
 * @brief wrapper around the  Truncated Power Series Algebra module interface
 *
 * As distributed as part of the MAD package see
 * https://cern.ch/mad for details
 *
 * This wrapper provides "nearly direct access" to the original C functions.
 * It makes the functions methods, keeps their signature except for the
 * arrays: for these standard containers are used instead
 *
 * The preprocessor is used to create the instance for the required type.
 * The generated class is typically not directly used but TpsaWrapper or
 * CTpsaWrapper, which can be seen as adaptor classes.
 *
 * Extended functionallity is provided by the tpsa and gtpsa classes.
 * These uses the adapaters generated here as end of their bridge.
 *
 *
 * @todo: add wrapper for more advanced functions
 */
#include <memory>
#include <vector>
#include <string>
#include <cstdbool>

#ifndef GTPSA_CLASS
#error "GTPSA CLASS has to be defined (either MadTpsa_ or MadCTpsa_)"
#endif /* GTPSA_CLASS */

#ifndef GTPSA_METH
#error "GTPSA METH has to be defined (either mad_tpsa_ or mad_ctpsa_)"
#endif /* GTPSA_METH */

#include <gtpsa/desc.hpp>

namespace gtpsa::mad {
    // forward declaration

    class GTPSA_CLASS(Wrapper);

    /**
     * @brief manages lifetime of underlying C object
     *
     * Expected to be handled by std::unique_ptr
     */
    class GTPSA_CLASS(LifeTimeManager) {
    public:
	inline GTPSA_CLASS(LifeTimeManager)(GTPSA_PTR_T *p)
	    : ptr(p)
	    { }
	inline ~GTPSA_CLASS(LifeTimeManager)(void) { GTPSA_METH(del)(this->ptr);  this->ptr = nullptr; }

    private:
	GTPSA_CLASS(LifeTimeManager)            (const GTPSA_CLASS(LifeTimeManager)& o) = delete;
	GTPSA_CLASS(LifeTimeManager)& operator= (const GTPSA_CLASS(LifeTimeManager)& o) = delete;

	inline const GTPSA_PTR_T*    getPtr(void) const { return this->ptr; }
	inline       GTPSA_PTR_T*    getPtr(void)       { return this->ptr; }

	GTPSA_PTR_T* ptr=nullptr;

	friend class GTPSA_CLASS(Wrapper);
    }; /* class GTPSA_CLASS(LifeTimeManager) */


    /**
     * @brief wraps mad_tpsa or mad_ctpsa object
     *
     * Wraps the C functions / methods as C++ class methods.
     *
     * The pointer to the C object is managed by a LifeTimeManger using a
     * std::unique_ptr. Furthermore keeps a shared ptr to the description
     * object.
     */
    class GTPSA_CLASS(Wrapper) {
    private:
	// managed access to the underlying pointer
	std::shared_ptr<desc>                          t_desc;
	std::unique_ptr<GTPSA_CLASS(LifeTimeManager)>  ltm;


    public:
	// for the adaptor/ bridge pattern ?
	// friend class GTPSA_CLASS(WithOp);

	inline auto getDescription(void) const {
	    return this->t_desc;
	}

	virtual ~GTPSA_CLASS(Wrapper)() = default;

	/**
	 *
	 * if mo > d_mo, mo = d_mo
	 * @todo: define it for ctpsa &t too
	 */
	inline GTPSA_CLASS(Wrapper)(std::shared_ptr<desc> desc, const ord_t mo)
	    : t_desc(desc)
	    , ltm( std::make_unique<GTPSA_CLASS(LifeTimeManager)>(GTPSA_METH(newd)(desc->getPtr(), mo)) )
	{}

	/**
	 *
	 * @todo: define it for ctpsa &t too
	 */
	inline GTPSA_CLASS(Wrapper)(const GTPSA_CLASS(Wrapper)&              t,  const ord_t mo)
	    : t_desc(t.getDescription())
	    , ltm( std::make_unique<GTPSA_CLASS(LifeTimeManager)>(GTPSA_METH(new)(t.getPtr(), mo)) )
	    { }


	/**
	 * Copy ctor: use clone instead if required ....
	 */
#ifndef GTSPA_ONLY_OPTIMISED_OPS
	inline GTPSA_CLASS(Wrapper)(const GTPSA_CLASS(Wrapper)&              o)
	    : t_desc( o.getDescription())
	    , ltm  ( std::make_unique<GTPSA_CLASS(LifeTimeManager)>(GTPSA_METH(new)(o.getPtr(), init::same)) )
	    { this->_copyInPlace(o); };
#else /* GTSPA_ONLY_OPTIMISED_OPS */
	inline GTPSA_CLASS(Wrapper)(const GTPSA_CLASS(Wrapper)&              o) = delete;
#endif /* GTSPA_ONLY_OPTIMISED_OPS */

	/**
	 * move ctor
	 */
	inline GTPSA_CLASS(Wrapper)(GTPSA_CLASS(Wrapper)&& o)                   = default; //  noexcept :  ltm(std::move(o.tm)), t_desc(std::move(o.t_desc)) { };
	inline GTPSA_CLASS(Wrapper)& operator= (GTPSA_CLASS(Wrapper) && o)      = default;


	/**
	 * copies content of an other tpsa object to this
	 * @todo implement a clone function
	 *
	 */
	inline void  _copyInPlace(const GTPSA_CLASS(Wrapper) &o)      {        GTPSA_METH(copy)(o.getPtr(), this->getPtr());                }
	inline GTPSA_CLASS(Wrapper)  clone(void)                const { GTPSA_CLASS(Wrapper) res(*this, init::same); res._copyInPlace(*this); return res; }
	inline GTPSA_CLASS(Wrapper)  newFromThis(void)          const { return GTPSA_CLASS(Wrapper)(*this, init::same); }

	/**
	 * @short set uid if != 0
	 *
	 * @todo review if separate in constant and non constant part ...
	 */
	inline auto uid(int32_t uid_)                 { return GTPSA_METH(uid)(this->getPtr(), uid_); }
	// inline auto uid(void)                const { return GTPSA_METH(uid)((P*)(this->getPtr()), 0); }
	inline auto len(void)                   const { return GTPSA_METH(len)(this->getPtr()); }

	/*
	 * @todo return std::string?
	 */
	inline auto nam(void)                   const { return GTPSA_METH(nam)(this->getPtr()); }

	/**
	 * @todo: rename to order ?
	 */
	inline auto ord(void)                   const { return GTPSA_METH(ord)(this->getPtr()); }

	// initialization
	inline void clear(void)                       {        GTPSA_METH(clear)(this->getPtr()); }
	inline auto isNull(void)                const {        GTPSA_METH(isnul)(this->getPtr()); }

	inline void setnam(std::string name)          {        GTPSA_METH(setnam)(this->getPtr(), name.c_str());}
	/**
	 * @brief:
	 */
	void setvar(GTPSA_BASE_T v, idx_t iv_= 0, GTPSA_BASE_T scl_ = 0){
	    GTPSA_METH(setvar)(this->getPtr(), v, iv_, scl_);
	}

	/**
	 * @brief indexing / monomials (return idx_t = -1 if invalid)
	 */
	inline auto mono(std::vector<ord_t>& m, idx_t i)  const { return GTPSA_METH(mono)(this->getPtr(), m.size(), m.data(), i);}

	/**
	 *  @brief string mono "[0-9]*"
	 */
	inline auto idxs(std::string s)                   const { return GTPSA_METH(idxs)(this->getPtr(), s.size(), s.data());}
	inline auto idxm(const std::vector<ord_t>& m)     const { return GTPSA_METH(idxm)(this->getPtr(), m.size(), m.data());}

	/**
	 * @brief sparse mono [(i,o)]
	 * @note not using overload ... could be misleading
	 */
	inline auto idxsm(const std::vector<int> m)       {
	    return GTPSA_METH(idxsm) (this->getPtr(), m.size(), m.data());
	}

	inline auto cycle(std::vector<ord_t> m, idx_t i, GTPSA_BASE_T *v) {
	    return GTPSA_METH(cycle)(this->getPtr(), m.size(), m.data(), i, v);
	}
	/**
	 * @todo: use standard accessor operators ?
	 */
	inline auto get0(void)                        const { return GTPSA_METH(get0) (this->getPtr()                     ); }
	inline auto geti(const idx_t i)               const { return GTPSA_METH(geti) (this->getPtr(), i                  ); }
	inline auto gets(std::string s)               const { return GTPSA_METH(gets) (this->getPtr(), s.size(), s.data() ); }
	inline auto getm(const std::vector<ord_t>& m) const { return GTPSA_METH(getm) (this->getPtr(), m.size(), m.data() ); }

	/**
	 * @brief sparse mono "[(i, o)]*"
	 */
	inline auto getsm(const std::vector<int>& m) const { return GTPSA_METH(getsm) (this->getPtr(), m.size(), m.data() ); }


	/**
	 * @brief a*x[0]+b
	 */
	inline void set0(                               GTPSA_BASE_T a, GTPSA_BASE_T b) { GTPSA_METH(set0) (this->getPtr()                    , a, b ); }

	/**
	 * @brief a*x[i]+b
	 */
	inline void seti(const idx_t i                , GTPSA_BASE_T a, GTPSA_BASE_T b) { GTPSA_METH(seti) (this->getPtr(), i                 , a, b ); }
	/**
	 * @brief a*x[m]+b
	 */
	inline void sets(const std::string& s          , GTPSA_BASE_T a, GTPSA_BASE_T b) { GTPSA_METH(sets) (this->getPtr(), s.size(), s.data(), a, b ); }
	/**
	 * @brief a*x[m]+b
	 */
	inline void setm(const std::vector<ord_t>& m  , GTPSA_BASE_T a, GTPSA_BASE_T b) { GTPSA_METH(setm) (this->getPtr(), m.size(), m.data(), a, b ); }

	/**
	 * @brief a*x[m]+b, sparse mono [(i,o)]
	 * @todo vector of pairs?
	 */
	inline auto setsm(const std::vector<int> m, GTPSA_BASE_T a, GTPSA_BASE_T b) { return GTPSA_METH(setsm)  (this->getPtr(), m.size(), m.data(), a, b ); }


	inline void getv(idx_t i,       std::vector<GTPSA_BASE_T> *v) const { GTPSA_METH(getv)(this->getPtr(), i, v->size(), v->data() ); }
	inline void setv(idx_t i, const std::vector<GTPSA_BASE_T> &v)       { GTPSA_METH(setv)(this->getPtr(), i, v.size() , v.data()  ); }


	inline void print(str_t name = nullptr, num_t eps = 0, int nohdr = 0, FILE *stream = nullptr){
	    GTPSA_METH(print)(this->getPtr(), name, eps, nohdr, stream);
	}

	inline void pow (const GTPSA_CLASS(Wrapper)& a, const GTPSA_CLASS(Wrapper)& b){ process2w_(a, b, this, GTPSA_METH(pow)); }
	inline void pow (const GTPSA_CLASS(Wrapper)& a, const int n                  ){ GTPSA_METH(powi) (a.getPtr(), n, this->getPtr()); }
	inline void pow (const GTPSA_CLASS(Wrapper)& a, const GTPSA_BASE_T v         ){ process2w_(a, v, this, GTPSA_METH(pown)); }

    protected:
	inline const GTPSA_PTR_T*    getPtr(void) const { return this->ltm->getPtr(); }
	inline       GTPSA_PTR_T*    getPtr(void)       { return this->ltm->getPtr(); }

	/*
	 * processing functions with a single argument in, single out
	 */
	friend inline void process1w_ (const GTPSA_CLASS(Wrapper)& a, GTPSA_CLASS(Wrapper) *r ,
					void (*func)(const GTPSA_PTR_T* a, GTPSA_PTR_T *r) );
	/*
	 * processing functions with a two arguments  in, single out
	 */
	friend inline void process2w_ (const GTPSA_CLASS(Wrapper)& a, const GTPSA_CLASS(Wrapper)& b, GTPSA_CLASS(Wrapper) *r,
			   void (*func)(const GTPSA_PTR_T* a, const GTPSA_PTR_T* b, GTPSA_PTR_T *r) );

	friend inline void process2w_  (const GTPSA_CLASS(Wrapper)& a, const GTPSA_BASE_T b, GTPSA_CLASS(Wrapper) *r,
				void (*func)(const GTPSA_PTR_T* a, const GTPSA_BASE_T b, GTPSA_PTR_T *r) );

	/*
	 * processing functions with a single input two output arguments
	 */
	friend inline void process1to2w_(const GTPSA_CLASS(Wrapper)& a, GTPSA_CLASS(Wrapper)* r1, GTPSA_CLASS(Wrapper) *r2,
					void (*func)(const GTPSA_PTR_T* a, GTPSA_PTR_T *r1,  GTPSA_PTR_T *r2) );

	/*
	 * functions with a single call pattern
	 */
	friend inline void taylor     (const GTPSA_CLASS(Wrapper)& a, std::vector<GTPSA_BASE_T> coeff, GTPSA_CLASS(Wrapper)* c);
    }; /* class GTPSA_CLASS(Wrapper) */

/*
 * different mathematical functions processing the class processing one input argument
 */
#ifdef GTPSA_FUNC_ARG1
#undef GTPSA_FUNC_ARG1
#endif
#ifdef GTPSA_FUNC_ARG1_WITH_RET_ARG
#undef GTPSA_FUNC_ARG1_WITH_RET_ARG
#endif

#define GTPSA_FUNC_ARG1_WITH_RET_ARG(fname)				\
    inline void fname (const GTPSA_CLASS(Wrapper)& t, GTPSA_CLASS(Wrapper)* r){ process1w_(t, r, GTPSA_METH(fname) ); }

#define GTPSA_FUNC_ARG1(fname) GTPSA_FUNC_ARG1_WITH_RET_ARG(fname)
#include <gtpsa/funcs.h>
#undef GTPSA_FUNC_ARG1_WITH_RET_ARG
#undef GTPSA_FUNC_ARG1


    /*
     * @brief processing functions with a single argument in, single out, all gtpsa object pointers
     */
    inline void process1w_  (const GTPSA_CLASS(Wrapper)& a, GTPSA_CLASS(Wrapper) *r,
			    void (*func)(const GTPSA_PTR_T* a, GTPSA_PTR_T*r) ) {
	func(a.getPtr(), r->getPtr());
    }

    /*
     * @brief processing functions with two arguments in, single out, all gtpsa object pointers
     */
    inline void process2w_ (const GTPSA_CLASS(Wrapper)& a, const GTPSA_CLASS(Wrapper)& b, GTPSA_CLASS(Wrapper) *r,
			   void (*func)(const GTPSA_PTR_T* a, const GTPSA_PTR_T* b, GTPSA_PTR_T *r) ) {
	func(a.getPtr(), b.getPtr(), r->getPtr());
    }

    /*
     * @brief processing functions with two arguments in, single out
     * @param a first argument gtpsa object pointer
     * @param b second argument gtpsa base object (double or complex double)
     * @param c return argument
     */
    inline void process2w_  (const GTPSA_CLASS(Wrapper)& a, const GTPSA_BASE_T b, GTPSA_CLASS(Wrapper) *r,
			    void (*func)(const GTPSA_PTR_T* a, const GTPSA_BASE_T b, GTPSA_PTR_T *r) ) {
	func(a.getPtr(), b, r->getPtr());
    }

    /*
    inline GTPSA_CLASS(Wrapper) process2w(const GTPSA_CLASS(Wrapper)& a, const GTPSA_CLASS(Wrapper)&  b,
					void (*func)(const GTPSA_CLASS(Wrapper)& a, const GTPSA_CLASS(Wrapper)& b, GTPSA_CLASS(Wrapper)* r) ) {
	auto ret = a.newFromThis(); //TpsaWithOp<T>(a, gtpsa::init::same);
	func(a, b, &ret);
	return ret;
    }
    */

    /**
     * @brief single argument in, two arguments out, all gtpsa object pointers
     */
    inline void process1to2w_(const GTPSA_CLASS(Wrapper)& a, GTPSA_CLASS(Wrapper)* r1, GTPSA_CLASS(Wrapper) *r2,
			     void (*func)(const GTPSA_PTR_T* a, GTPSA_PTR_T *r1,  GTPSA_PTR_T *r2) ) {
	func(a.getPtr(), r1->getPtr(), r2->getPtr());
    }

    inline void add_  (const GTPSA_CLASS(Wrapper)& a, const GTPSA_CLASS(Wrapper)& b,  GTPSA_CLASS(Wrapper)* r ){ process2w_(a, b, r, GTPSA_METH(add)); }
    /**
     * @brief  (a_i-b_i)/max(|a_i|,1)
     */
    inline void dif_   (const GTPSA_CLASS(Wrapper)& a, const GTPSA_CLASS(Wrapper)& b,  GTPSA_CLASS(Wrapper)* r ){ process2w_(a, b, r, GTPSA_METH(dif)); }
    inline void sub_   (const GTPSA_CLASS(Wrapper)& a, const GTPSA_CLASS(Wrapper)& b,  GTPSA_CLASS(Wrapper)* r ){ process2w_(a, b, r, GTPSA_METH(sub)); }
    inline void mul_   (const GTPSA_CLASS(Wrapper)& a, const GTPSA_CLASS(Wrapper)& b,  GTPSA_CLASS(Wrapper)* r ){ process2w_(a, b, r, GTPSA_METH(mul)); }
    inline void div_   (const GTPSA_CLASS(Wrapper)& a, const GTPSA_CLASS(Wrapper)& b,  GTPSA_CLASS(Wrapper)* r ){ process2w_(a, b, r, GTPSA_METH(div)); }

    inline void pow_   (const GTPSA_CLASS(Wrapper)& a, const GTPSA_CLASS(Wrapper)& b,  GTPSA_CLASS(Wrapper)* r ){ r->pow(a, b); }
    inline void pow_   (const GTPSA_CLASS(Wrapper)& a, const int n                  ,  GTPSA_CLASS(Wrapper)* r ){ r->pow(a, n); }
    inline void pow_   (const GTPSA_CLASS(Wrapper)& a, const GTPSA_BASE_T v         ,  GTPSA_CLASS(Wrapper)* r ){ r->pow(a, v); }

    /**
     * @brief single argument in, single argument out, return argument allocated
     */
    /**
     * @brief c += v*a, aliasing OK
     */
    inline void acc_   (const GTPSA_CLASS(Wrapper) &a, const GTPSA_BASE_T& v, GTPSA_CLASS(Wrapper) *c) { process2w_(a, v, c, &GTPSA_METH(acc)    ); }
    /**
     * @brief c = v*a, aliasing OK ?
     */
    inline void scl_  (const GTPSA_CLASS(Wrapper) &a, const GTPSA_BASE_T& v, GTPSA_CLASS(Wrapper) *c) { process2w_(a, v, c, &GTPSA_METH(scl)    ); }
    /**
     * @brief c = v/a, aliasing OK ?
     */
    inline void inv_ (const GTPSA_CLASS(Wrapper) &a, const GTPSA_BASE_T& v, GTPSA_CLASS(Wrapper) *c) { process2w_(a, v, c, &GTPSA_METH(inv)    ); }
    /**
     * @brief c = v/sqrt(a), aliasing OK ?
     */
    inline void invsqrt_ (const GTPSA_CLASS(Wrapper) &a, const GTPSA_BASE_T& v, GTPSA_CLASS(Wrapper) *c) { process2w_(a, v, c, &GTPSA_METH(invsqrt) ); }


    inline void sincos (const GTPSA_CLASS(Wrapper)& t, GTPSA_CLASS(Wrapper)* r1, GTPSA_CLASS(Wrapper)* r2) { process1to2w_(t, r1, r2, GTPSA_METH(sincos)  ) ; }
    inline void sincosh(const GTPSA_CLASS(Wrapper)& t, GTPSA_CLASS(Wrapper)* r1, GTPSA_CLASS(Wrapper)* r2) { process1to2w_(t, r1, r2, GTPSA_METH(sincosh) ) ; }
    inline void taylor (const GTPSA_CLASS(Wrapper)& a, std::vector<GTPSA_BASE_T> coeff, GTPSA_CLASS(Wrapper)* c){ GTPSA_METH(taylor)(a.getPtr(), coeff.size(), coeff.data(), c->getPtr()); }

    /*
      inline GTPSA_CLASS(Wrapper) add (const GTPSA_CLASS(Wrapper)& a, const GTPSA_CLASS(Wrapper)& b){ return process2w(a, b, add_wr); }
      inline GTPSA_CLASS(Wrapper) dif (const GTPSA_CLASS(Wrapper)& a, const GTPSA_CLASS(Wrapper)& b){ return process2w(a, b, dif_wr); }
      inline GTPSA_CLASS(Wrapper) sub (const GTPSA_CLASS(Wrapper)& a, const GTPSA_CLASS(Wrapper)& b){ return process2w(a, b, sub_wr); }
      inline GTPSA_CLASS(Wrapper) mul (const GTPSA_CLASS(Wrapper)& a, const GTPSA_CLASS(Wrapper)& b){ return process2w(a, b, mul_wr); }
      inline GTPSA_CLASS(Wrapper) div (const GTPSA_CLASS(Wrapper)& a, const GTPSA_CLASS(Wrapper)& b){ return process2w(a, b, div_wr); }
    */

} // namespace gtpsa::mad
