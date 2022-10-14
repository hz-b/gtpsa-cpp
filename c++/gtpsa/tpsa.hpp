#ifndef _GTPSA__H_
#define _GTPSA__H_ 1
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
#include <iostream>
#include <iomanip>
#include <memory>
#include <ostream>
#include <vector>

extern "C" {
#include <mad_tpsa.h>
}

namespace gtpsa {

    class tpsa;

    class tpsa_mgr {
    public:
	inline ~tpsa_mgr(void){
	    mad_tpsa_del(this->ptr);
	    this->ptr = nullptr; // a bit of paranoia
	}
	inline tpsa_mgr(tpsa_t *p) {
	    this->ptr = p;
	}
    private:
	inline tpsa_mgr(const tpsa_mgr&& o)      { this->ptr = o.ptr;  };
	tpsa_mgr(const tpsa_mgr& o) = delete;

    private:
	tpsa_t * ptr;
	inline const tpsa_t * getPtr(void) const { return this->ptr; }
	inline tpsa_t *       getPtr(void)       { return this->ptr; }

	friend class tpsa;
    };

    class tpsa{
    public:
	/**
	 * if mo > d_mo, mo = d_mo
	 */
	/**
	 *
	 * @todo: define it for ctpsa &t too
	 */
	inline auto getDescription(void)   const { return this->t_desc; }

	inline tpsa(std::shared_ptr<desc> desc, const ord_t mo) {
	    this->t_desc = desc;
	    this->tm = std::make_unique<tpsa_mgr>(mad_tpsa_newd(desc->getPtr(), mo));
	}
	/**
	 *
	 * @todo: define it for ctpsa &t too
	 */
	inline tpsa(const tpsa              &t,  const ord_t mo) {
	    this->t_desc = t.getDescription();
	    this->tm = std::make_unique<tpsa_mgr>(mad_tpsa_new(t.getPtr(), mo)); // ok with t=(tpsa_t*)ctpsa;
	    this->clear();
	}
	//inline ~tpsa(void){  }

	/**
	 * move ctor
	 */
	inline tpsa(tpsa&& o) = default; //  noexcept :  tm(std::move(o.tm)), t_desc(std::move(o.t_desc)) { };
	inline tpsa& operator= (tpsa && o) = default;

#if 0
	    noexcept {
	    if (this != &o) {
		this->tm = std::move(o.tm);
		this->t_desc = std::move(o.t_desc);
	    }
	    return *this;
	}
#endif
	inline tpsa& operator= (const double && o) {
	    // this->clear();
	    this->set(0, o);
	    return *this;
	}
#if 0
	/**
	 * ctor using double ...
	 *
	 * how to handle appropriate desc!
	 */
	inline tpsa(const double& o) {
	    /* optimise for constant term ... no higher orders */
	    // int nv = 1;
	    /* following could be a static singelton .... */
	    /* ... but not that usable ... */
	    this->t_desc = std::make_shared<gtpsa::desc>(1, 0);
	    this->tm = std::make_unique<tpsa_mgr>(mad_tpsa_newd(this->t_desc->getPtr(), 0));
	    // this->clear();
	    this->set(0, o);
	}
#endif
#ifndef GTSPA_ONLY_OPTIMISED_OPS
	/**
	 * Copy ctor: use clone instead if required ....
	 */
	inline tpsa(const tpsa& o) {
	    this->t_desc = o.getDescription();
	    this->tm = std::make_unique<tpsa_mgr>(mad_tpsa_new(o.getPtr(), mad_tpsa_same));
	    this->clear();
	    this->_copyInPlace(o);
	};
#else /* GTSPA_ONLY_OPTIMISED_OPS */
//#error "Currently supporting copy"
	inline tpsa(const tpsa& o) = delete;
#endif /* GTSPA_ONLY_OPTIMISED_OPS */
	/**
	 * copies content of an other tpsa object to this
	 * @todo implement a clone function
	 *
	 */
	inline void _copyInPlace(const tpsa &o)       { mad_tpsa_copy(o.getPtr(), this->getPtr()); }
	inline tpsa clone(void)                 const { tpsa res(*this, mad_tpsa_same); res._copyInPlace(*this); return res; }

#if 0
// introspection
	/*
	 * @todo: memory handling shared ptrs?
	 */
	inline const desc getDesc(void)         const { return gtpsa::desc(mad_tpsa_desc(this->t_tpsa)); }
#endif

	/**
	 * @short set uid if != 0
	 *
	 * @todo review if separate in constant and non constant part ...
	 */
	inline auto uid(int32_t uid_)                 { return mad_tpsa_uid(this->getPtr(), uid_); }
	// inline auto uid(void)                   const { return mad_tpsa_uid((t->getPtr()), 0); }
	inline auto len(void)                   const { return mad_tpsa_len(this->getPtr());}
	/*
	 * @todo return std::string?
	 */
	inline auto name(void)                  const { return mad_tpsa_nam(this->getPtr()); }

	/**
	 * @todo: rename to order ?
	 */
	inline auto ord(void)                   const { return mad_tpsa_ord(this->getPtr()); }
	// inline auot ord(=
#if 0
	inline auto ordv(...)                   const { return mad_tpsa_ord(this->getPtr()); }
#endif

	// initialization

	inline void clear(void)                       { mad_tpsa_clear(this->getPtr()); }
	inline auto isNull(void)                const { mad_tpsa_isnul(this->getPtr()); }

	/**
	 *
	 * @todo replace str_t with std::string ...
	 */
	inline void setName(std::string name)         { mad_tpsa_setnam(this->getPtr(), name.c_str());}
	/**
	 * @brief:
	 */
	void setVariable(num_t v, idx_t iv_= 0, num_t scl_ = 0){ mad_tpsa_setvar(this->getPtr(), v, iv_, scl_);	}
	// indexing / monomials (return idx_t = -1 if invalid)
	/**
	 *
	 */
	inline auto mono(std::vector<ord_t> m, idx_t i) { return mad_tpsa_mono(this->getPtr(), m.size(), m.data(), i);}

	/**
	 *  @brief string mono "[0-9]*"
	 */
	inline auto idx(std::string s)                  { return mad_tpsa_idxs(this->getPtr(), s.size(), s.data());}
	inline auto idx(const std::vector<ord_t> m)     { return mad_tpsa_idxm(this->getPtr(), m.size(), m.data());}

	/**
	 * @brief sparse mono [(i,o)]
	 * @note not using overload ... could be misleading
	 */
	inline auto idxsm(const std::vector<int> m)     { return mad_tpsa_idxsm(this->getPtr(), m.size(), m.data());}
	inline auto cycle(std::vector<ord_t> m, idx_t i, num_t *v)     { return mad_tpsa_cycle(this->getPtr(), m.size(), m.data(), i, v);}
	/**
	 * @todo: use standard accessor operators ?
	 */
	inline auto get(void)                        const { return mad_tpsa_get0  (this->getPtr()                     ); }
	inline auto get(const idx_t i)               const { return mad_tpsa_geti  (this->getPtr(), i                  ); }
	inline auto get(std::string s)               const { return mad_tpsa_gets  (this->getPtr(), s.size(), s.data() ); }
	inline auto get(const std::vector<ord_t> m)  const { return mad_tpsa_getm  (this->getPtr(), m.size(), m.data() ); }
	/**
	 * @brief sparse mono "[(i, o)]*"
	 */
	inline auto getsm(const std::vector<int> m)  const { return mad_tpsa_getsm (this->getPtr(), m.size(), m.data() ); }

	inline auto cst(void)                        const { return this->get(); }

	/**
	 * @brief a*x[0]+b
	 */
	inline void set(                                 num_t a, num_t b) { mad_tpsa_set0 (this->getPtr()                    , a, b ); }

	/**
	 * @brief a*x[i]+b
	 */
	inline void set(const idx_t i                  , num_t a, num_t b) { mad_tpsa_seti (this->getPtr(), i                 , a, b ); }
	/**
	 * @brief a*x[m]+b
	 */
	inline void set(std::string s                  , num_t a, num_t b) { mad_tpsa_sets (this->getPtr(), s.size(), s.data(), a, b ); }
	/**
	 * @brief a*x[m]+b
	 */
	inline void set(const std::vector<ord_t> m, num_t a, num_t b) { mad_tpsa_setm (this->getPtr(), m.size(), m.data(), a, b ); }

	/**
	 * @brief a*x[m]+b, sparse mono [(i,o)]
	 * @todo vector of pairs?
	 */
	inline auto setsm(const std::vector<int> m, num_t a, num_t b) { return mad_tpsa_setsm  (this->getPtr(), m.size(), m.data(), a, b ); }


	inline void getv(idx_t i,       std::vector<num_t> *v) const { mad_tpsa_getv(this->getPtr(), i, v->size(), v->data() ); }
	inline void setv(idx_t i, const std::vector<num_t> &v)       { mad_tpsa_setv(this->getPtr(), i, v.size() , v.data()  ); }

	inline void print(str_t name_, num_t eps_, int nohdr_, FILE *stream_){
	    mad_tpsa_print(this->getPtr(), name_, eps_, nohdr_, stream_);
	}

	inline tpsa& operator += (const tpsa& o  ){ add(*this, o, this); return *this; }
	// (a_i-b_i)/max(|a_i|,1)
	inline tpsa& operator -= (const tpsa& o  ){ sub(*this, o, this); return *this; }
	inline tpsa& operator *= (const tpsa& o  ){ mul(*this, o, this); return *this; }
	inline tpsa& operator /= (const tpsa& o  ){ div(*this, o, this); return *this; }

	// the following ones can profit from an implementation using mad_set(..., a=1e0, b=o)
	inline tpsa& operator += (const double o ){ mad_tpsa_set0(this->getPtr(),    1e0,    o); return *this; }
	inline tpsa& operator -= (const double o ){ mad_tpsa_set0(this->getPtr(),    1e0,   -o); return *this; }
	inline tpsa& operator *= (const double o ){ mad_tpsa_set0(this->getPtr(),      o,  0e0); return *this; }
	inline tpsa& operator /= (const double o ){ mad_tpsa_set0(this->getPtr(),  1e0/o,  0e0); return *this; }

	inline bool operator>=(const double a) { return this->cst() >= a; }
	inline bool operator<=(const double a) { return this->cst() <= a; }
	inline bool operator> (const double a) { return this->cst() > a;  }
	inline bool operator< (const double a) { return this->cst() < a;  }

	inline tpsa & operator =  (const double & o)     { this->set(0, o); return *this; }

#ifndef GTSPA_ONLY_OPTIMISED_OPS
	/**
	 * These operators need to make copys of the actual instance
	 * In the current implementation I assum the compiler can not fully optimise it away
	 * if not required
	 */
	inline tpsa  operator =  (const tpsa& o )       { this->_copyInPlace(o); return *this; }
	/**
	 * @todo do I need to make a copy ....?
	 */
	// negation
	inline tpsa  operator -  (void)           const { auto r = this->clone(); r *= -1e0; return r;}

	//process2
	inline tpsa  operator +  (const tpsa& o ) const { return process2(*this, o, add); }
	inline tpsa  operator -  (const tpsa& o ) const { return process2(*this, o, sub); }
	inline tpsa  operator *  (const tpsa& o ) const { return process2(*this, o, mul); }
	inline tpsa  operator /  (const tpsa& o ) const { return process2(*this, o, div); }


	// inline tpsa  operator +  (const double o) const { return process2(*this, o, add_d); }
	// inline tpsa  operator -  (const double o) const { return process2(*this, o, sub_d); }
	// inline tpsa  operator *  (const double o) const { return process2(*this, o, mul_d); }
	// inline tpsa  operator /  (const double o) const { return process2(*this, o, div_d); }

#endif
	friend std::ostream& operator<<(std::ostream &, const tpsa &);

	/*
	 *  @warning: no compatible version in mad_tpsa ...
	 *  @todo: review method
	 */
	inline void show(std::ostream& strm, int level) const {

	    strm << "gtpsa  cst:\n\t" << this->cst();
	    if(this->ord()){
		// at least first order ...
		auto nv = this->getDescription()->getNv(0, 0, 0);
		std::vector<num_t> v(nv);
		this->getv(1, &v);

		strm  << "\ngtpsa linear :\n"
		      << std::scientific << std::setw(20);
		for(auto& e: v) strm <<  std::scientific << std::setw(20) << e << " ";
	    }
	    strm << "\n";
	}

    private:
	// managed access to the underlying pointer
	std::unique_ptr<tpsa_mgr>  tm;
	std::shared_ptr<desc>      t_desc;


	inline const tpsa_t * getPtr(void) const { return this->tm->getPtr(); }
	inline tpsa_t *       getPtr(void)       { return this->tm->getPtr(); }

	/*
	 * helper functions for the different operators
	 * here if no copies are required
	 */
	friend inline void process2_   (const tpsa& a,  const tpsa& b, tpsa *r,  void (*func)(const tpsa_t* a, const tpsa_t* b, tpsa_t *r)  );
	friend inline void process1_   (const tpsa& a,                 tpsa *r,  void (*func)(const tpsa_t* a,                  tpsa_t *r)  );
	friend inline void process1to2_(const tpsa& a,       tpsa* r1, tpsa *r2, void (*func)(const tpsa_t* a,       tpsa_t *r, tpsa_t *r2) );
	//friend void inline process_op2_(const tpsa& a, const tpsa& b, tpsa *r, void (*func)(const tpsa& a, const tpsa& b, tpsa *r) );

	friend inline void process2_   (const tpsa& a, const double b, tpsa *r,  void (*func)(const tpsa_t* a, const double b, tpsa_t *r)  );
	friend inline void process2_   (const tpsa& a, const double b, tpsa *r,  void (*func)(const double a,  const double b, double *r)  );
	friend inline auto equ (const tpsa& a, const tpsa& b, num_t tol);
	friend inline auto norm (const tpsa& a);

	friend inline void pow  (const tpsa& a,         int n, tpsa *r);
	friend inline void pow  (const tpsa& a,       num_t v, tpsa *r);
	friend inline auto ordn (std::vector<tpsa *> objs);

}; // class tpsa

    inline auto ordn(std::vector<tpsa *> objs) {
	std::vector<const tpsa_t *> tv(objs.size());
	auto toPtr = [](tpsa * obj){ return obj->getPtr();};
	std::transform(objs.begin(), objs.end(), tv.begin(), toPtr);
	return mad_tpsa_ordn(tv.size(), tv.data());
    }

    // ------------------------------------------------------------------------------
    // helper fuunctions for different functions
    // start with inplance
    /**
     * @brief single argument in single argument out
     * @todo renaming required
     */
    inline void process1_   (const tpsa& a, tpsa *r,                 void (*func)(const tpsa_t* a,tpsa_t *r) ) {
	func(a.getPtr(), r->getPtr());
    }

    /**
     * @brief single argument in, two arguments out
     */
    inline void process1to2_(const tpsa& a, tpsa* r1, tpsa *r2,      void (*func)(const tpsa_t* a, tpsa_t *r1,  tpsa_t *r2) ) {
	func(a.getPtr(), r1->getPtr(), r2->getPtr());
    }
    /**
     * @brief two arguments in, single argument out
     */
    inline void process2_   (const tpsa& a, const tpsa& b, tpsa *r,  void (*func)(const tpsa_t* a, const tpsa_t* b, tpsa_t *r) ) {
	func(a.getPtr(), b.getPtr(), r->getPtr());
    }
    inline void process2_   (const tpsa& a, num_t b, tpsa *r,        void (*func)(const tpsa_t* a, num_t b, tpsa_t *r) ) {
	func(a.getPtr(), b, r->getPtr());
    }
    inline void process2_  (const tpsa& a, const double b, tpsa *r, void (*func)(const double a, const double b, double *r) ) {
	num_t rv;
	func(mad_tpsa_get0(a.getPtr()), b, &rv);
	mad_tpsa_set0(r->getPtr(), 0.0, rv);
    }

    /**
     * @brief single argument in, single argument out, return argument allocated
     */
    tpsa inline process1(const tpsa& t, void (*func)(const tpsa& t, tpsa *r) )                                 { auto ret = tpsa(t, mad_tpsa_same); func(t, &ret);    return ret; }
    /**
     * @brief two arguments in, single argument out, return argument allocated
     */
    tpsa inline process2(const tpsa& a, const tpsa&  b, void (*func)(const tpsa& a, const tpsa&  b, tpsa *r) ) { auto ret = tpsa(a, mad_tpsa_same); func(a, b, &ret); return ret; }
    tpsa inline process2(const tpsa& a, const double b, void (*func)(const tpsa& a, const double b, tpsa *r) ) { auto ret = tpsa(a, mad_tpsa_same); func(a, b, &ret); return ret; }
    tpsa inline process2(const tpsa& a, const double b, void (*func)(const tpsa& a, const tpsa&  b, tpsa *r) ) { auto ret = tpsa(a, mad_tpsa_same); ret.set(0, b); func(a, ret, &ret); return ret; }

    inline auto norm  (const tpsa& a)                          { return mad_tpsa_nrm(a.getPtr()); }
    inline auto equ   (const tpsa& a, const tpsa& b, num_t tol){ return mad_tpsa_equ(a.getPtr(), b.getPtr(), tol); }

    inline void add   (const tpsa& a, const tpsa& b,  tpsa* r ){ process2_(a, b, r, mad_tpsa_add); }
    /**
     * @brief  (a_i-b_i)/max(|a_i|,1)
     */
    inline void dif   (const tpsa& a, const tpsa& b,  tpsa* r ){ process2_(a, b, r, mad_tpsa_dif); }
    inline void sub   (const tpsa& a, const tpsa& b,  tpsa* r ){ process2_(a, b, r, mad_tpsa_sub); }
    inline void mul   (const tpsa& a, const tpsa& b,  tpsa* r ){ process2_(a, b, r, mad_tpsa_mul); }
    inline void div   (const tpsa& a, const tpsa& b,  tpsa* r ){ process2_(a, b, r, mad_tpsa_div); }
    inline void pow   (const tpsa& a, const tpsa& b,  tpsa* r ){ process2_(a, b, r, mad_tpsa_pow); }

    inline void pow   (const tpsa& a,         int n,  tpsa* r ){ mad_tpsa_powi(a.getPtr(), n, r->getPtr()); }
    inline void pow   (const tpsa& a,       num_t v,  tpsa* r ){ mad_tpsa_pown(a.getPtr(), v, r->getPtr()); }
    inline void add_d (const tpsa& a, const double b, tpsa* r ){ process2_(a, b, r, [](double a, double b, double *r){ *r = a + b; }); }
    inline void sub_d (const tpsa& a, const double b, tpsa* r ){ process2_(a, b, r, [](double a, double b, double *r){ *r = a - b; }); }
    inline void mul_d (const tpsa& a, const double b, tpsa* r ){ process2_(a, b, r, [](double a, double b, double *r){ *r = a * b; }); }
    inline void div_d (const tpsa& a, const double b, tpsa* r ){ process2_(a, b, r, [](double a, double b, double *r){ *r = a / b; }); }


    inline tpsa add   (const tpsa& a, const tpsa& b           ){ return process2(a, b, add); }
    inline tpsa sub   (const tpsa& a, const tpsa& b           ){ return process2(a, b, sub); }
    inline tpsa mul   (const tpsa& a, const tpsa& b           ){ return process2(a, b, mul); }
    inline tpsa div   (const tpsa& a, const tpsa& b           ){ return process2(a, b, div); }

    // inline tpsa add_d (const tpsa& a, const double b          ){ return process2(a, b, add); }
    // inline tpsa sub_d (const tpsa& a, const double b          ){ return process2(a, b, sub); }
    // inline tpsa mul_d (const tpsa& a, const double b          ){ return process2(a, b, mul); }
    // inline tpsa div_d (const tpsa& a, const double b          ){ return process2(a, b, div); }

    /* standard mathematical functions ... trigonometic etc ... taking one argument returning one */
#ifdef FUNC_ARG1
#undef FUNC_ARG1
#endif
#define FUNC_ARG1_WITH_RET_ARG(fname) inline void fname ## _ (const tpsa& t, tpsa* r){ process1_(t, r, mad_tpsa_ ## fname); }
#define FUNC_ARG1_NO_RET_ARG(fname)   inline tpsa fname (const tpsa& t){ return process1(t, fname ## _); }
#define FUNC_ARG1(fname) FUNC_ARG1_WITH_RET_ARG(fname) FUNC_ARG1_NO_RET_ARG(fname)
#include <gtpsa/funcs.h>

    inline void sincos_ (const tpsa& t, tpsa* r1, tpsa* r2) { process1to2_(t, r1, r2, mad_tpsa_sincos);  }
    inline void sincosh_(const tpsa& t, tpsa* r1, tpsa* r2) { process1to2_(t, r1, r2, mad_tpsa_sincosh); }

    /**
     * @brief c += v*a, aliasing OK
     */
    inline void acc     (const tpsa &a, num_t v, tpsa *c) { process2_(a, v, c, mad_tpsa_acc    ); }
    /**
     * @brief c = v*a, aliasing OK ?
     */
    inline void scl     (const tpsa &a, num_t v, tpsa *c) { process2_(a, v, c, mad_tpsa_scl    ); }
    /**
     * @brief c = v/a, aliasing OK ?
     */
    inline void inv     (const tpsa &a, num_t v, tpsa *c) { process2_(a, v, c, mad_tpsa_inv    ); }
    /**
     * @brief c = v/sqrt(a), aliasing OK ?
     */
    inline void invsqrt (const tpsa &a, num_t v, tpsa *c) { process2_(a, v, c, mad_tpsa_invsqrt); }

    inline tpsa acc     (const tpsa &a, num_t v         ) { return process2 (a, v,      acc    ); }
    inline tpsa scl     (const tpsa &a, const double v  ) { return process2 (a, v,      scl    ); }
    inline tpsa inv     (const tpsa &a, num_t v         ) { return process2 (a, v,      inv    ); }
    inline tpsa invsqrt (const tpsa &a, num_t v         ) { return process2 (a, v,      invsqrt); }

#ifndef GTSPA_ONLY_OPTIMISED_OPS
    inline tpsa  operator + (const double a, const tpsa&  b) { auto r = tpsa(b, mad_tpsa_same); r.set(0, a    ); r += b; return r; }
    inline tpsa  operator + (const tpsa&  a, const double b) { return b + a;                                                       }
    // avoid double copy of b: a - b => -(b-a) => -b + a
    inline tpsa  operator - (const double a, const tpsa&  b) { auto r = tpsa(b, mad_tpsa_same); r.set(0, a    ); r -= b; return r; }
    inline tpsa  operator - (const tpsa&  a, const double b) { return a + (-b);                                                    }
    inline tpsa  operator * (const double a, const tpsa&  b) { auto r = tpsa(b, mad_tpsa_same); r.set(0, a    ); r *= b; return r; }
    inline tpsa  operator * (const tpsa&  a, const double b) { return b * a;                                                       }
    inline tpsa  operator / (const tpsa&  a, const double b) { auto r = tpsa(a, mad_tpsa_same); r.set(0, 1e0/b); r *= a; return r; }
    inline tpsa  operator / (const double a, const tpsa&  b) { auto r = tpsa(b, mad_tpsa_same); r.set(0,     a); r /= b; return r; }
    // inline tpsa  operator / (const double a, const tpsa&  b) { return inv(b, a); }
#endif

    inline
    std::ostream& operator<<(std::ostream& strm, const tpsa& a)
    {
	a.show(strm, 0);
	return strm;
    }

    /**
     * @brief get maximum number of all
     */

    //inline auto ordn(std::vector<const tpsa&>)

} // namespace gtsa




#endif /* _GTPSA__H_ */
