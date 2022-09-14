#ifndef _GTPSA__H_
#define _GTPSA__H_ 1
/**
 * @brief wrapper around the  Truncated Power Series Algebra module interface
 *
 * As distributed as part of the MAD package see
 * https://cern.ch/mad for details
 *
 */
#include <gtpsa_desc.hpp>
#include <algorithm>
#include <memory>
#include <vector>
#include <iostream>

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

    // forward declaration ... required that operators will use the same functions
    inline void add (const tpsa& a, const tpsa& b, tpsa* r);
    inline void sub (const tpsa& a, const tpsa& b, tpsa* r);
    inline void mul (const tpsa& a, const tpsa& b, tpsa* r);
    inline void div (const tpsa& a, const tpsa& b, tpsa* r);
    inline void pow (const tpsa& a, const tpsa& b, tpsa* r);

class tpsa{
public:
    /**
     * if mo > d_mo, mo = d_mo
     */
    /**
     *
     * @todo: define it for ctpsa &t too
     */

    inline tpsa(const desc &desc, const ord_t mo) { this->tm = std::make_unique<tpsa_mgr>(mad_tpsa_newd(desc.getPtr(), mo)); }
     /**
      *
      * @todo: define it for ctpsa &t too
      */
    inline tpsa(const tpsa &t, const ord_t mo)    { this->tm = std::make_unique<tpsa_mgr>(mad_tpsa_new(t.getPtr(), mo));     } // ok with t=(tpsa_t*)ctpsa;
    //inline ~tpsa(void){  }

    /**
     * move ctor
     */
    inline tpsa(tpsa&& o)  :  tm(std::move(o.tm)) { };

    /**
     * Copy ctor: use clone instead if required ....
     */
    inline tpsa(const tpsa& o) = delete;
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
    inline auto uid(void)                   const { return mad_tpsa_uid((tpsa_t*) this->getPtr(), 0); }
    inline auto len(void)                   const { return mad_tpsa_len(this->getPtr());}
    /*
     * @todo return std::string?
     */
    inline auto name(void)                  const { return mad_tpsa_nam(this->getPtr()); }
    inline auto ord(void)                   const { return mad_tpsa_ord(this->getPtr()); }
#if 0
    inline auto ordv(...)                   const { return mad_tpsa_ord(this->getPtr()); }
#endif

    // initialization

    inline void clear(void)                       { mad_tpsa_clear(this->getPtr()); }
    inline auto isNull(void)                const { mad_tpsa_isnul(this->getPtr()); }

    /*
     *
     * @todo replace str_t with std::string ...
     */
    inline void  setName(std::string name)        { mad_tpsa_setnam(this->getPtr(), name.c_str());}

    /*
     * @todo: use standard accessor operators ?
     */
    inline void getv(idx_t i, std::vector<num_t> &v) { mad_tpsa_getv(this->getPtr(), i, v.size(), v.data());   }
    inline void setv(idx_t i, std::vector<num_t> *v) { mad_tpsa_setv(this->getPtr(), i, v->size(), v->data()); }

    inline void print(str_t name_, num_t eps_, int nohdr_, FILE *stream_){
	mad_tpsa_print(this->getPtr(), name_, eps_, nohdr_, stream_);
    }

    inline tpsa& operator += (const tpsa& o){ add(*this, o, this); return *this;}
    // (a_i-b_i)/max(|a_i|,1)
    inline tpsa& operator -= (const tpsa& o){ sub(*this, o, this); return *this;}
    inline tpsa& operator *= (const tpsa& o){ mul(*this, o, this); return *this;}
    inline tpsa& operator /= (const tpsa& o){ div(*this, o, this); return *this;}

private:
    std::unique_ptr<tpsa_mgr>  tm;

    inline const tpsa_t * getPtr(void) const { return this->tm->getPtr(); }
    inline tpsa_t *       getPtr(void)       { return this->tm->getPtr(); }

    friend inline void process2_   (const tpsa& a, const tpsa& b, tpsa *r,  void (*func)(const tpsa_t* a, const tpsa_t* b, tpsa_t *r)  );
    friend inline void process1_   (const tpsa& a,                tpsa *r,  void (*func)(const tpsa_t* a,                  tpsa_t *r)  );
    friend inline void process1to2_(const tpsa& a,      tpsa* r1, tpsa *r2, void (*func)(const tpsa_t* a,       tpsa_t *r, tpsa_t *r2) );
    //friend void inline process_op2_(const tpsa& a, const tpsa& b, tpsa *r, void (*func)(const tpsa& a, const tpsa& b, tpsa *r) );

    friend inline auto norm (const tpsa& a);

    friend inline void pow  (const tpsa& a,         int n, tpsa *r);
    friend inline void pow  (const tpsa& a,       num_t v, tpsa *r);
    friend inline auto ordn(std::vector<tpsa *> objs);

}; // class tpsa

    inline auto ordn(std::vector<tpsa *> objs) {
	std::vector<const tpsa_t *> tv(objs.size());
	auto toPtr = [](tpsa * obj){ return obj->getPtr();};
	std::transform(objs.begin(), objs.end(), tv.begin(), toPtr);
	return mad_tpsa_ordn(tv.size(), tv.data());
    }

    inline void process1_   (const tpsa& a, tpsa *r,
			     void (*func)(const tpsa_t* a,tpsa_t *r) ) {
	func(a.getPtr(), r->getPtr());
    }
    inline void process1to2_(const tpsa& a, tpsa* r1, tpsa *r2,
			     void (*func)(const tpsa_t* a, tpsa_t *r1,  tpsa_t *r2) ) {
	func(a.getPtr(), r1->getPtr(), r2->getPtr());
    }
    inline void process2_(const tpsa& a, const tpsa& b, tpsa *r,
			  void (*func)(const tpsa_t* a, const tpsa_t* b, tpsa_t *r) ) {
	func(a.getPtr(), b.getPtr(), r->getPtr());
    }

    tpsa inline process1(const tpsa& t, void (*func)(const tpsa& t, tpsa *r) ) {auto ret = tpsa(t, mad_tpsa_same); func(t, &ret); return std::move(ret);}

    inline auto norm(const tpsa& a)                        { return mad_tpsa_nrm(a.getPtr()); }
    inline void add (const tpsa& a, const tpsa& b, tpsa* r){ process2_(a, b, r, mad_tpsa_add); }
    inline void sub (const tpsa& a, const tpsa& b, tpsa* r){ process2_(a, b, r, mad_tpsa_sub); }
    inline void mul (const tpsa& a, const tpsa& b, tpsa* r){ process2_(a, b, r, mad_tpsa_mul); }
    inline void div (const tpsa& a, const tpsa& b, tpsa* r){ process2_(a, b, r, mad_tpsa_div); }
    inline void pow (const tpsa& a, const tpsa& b, tpsa* r){ process2_(a, b, r, mad_tpsa_pow); }

    inline void pow (const tpsa& a,         int n, tpsa* r){ mad_tpsa_powi(a.getPtr(), n, r->getPtr()); }
    inline void pow (const tpsa& a,       num_t v, tpsa* r){ mad_tpsa_pown(a.getPtr(), v, r->getPtr()); }


    inline void abs_    (const tpsa& t, tpsa* r)            { process1_(t, r, mad_tpsa_abs   ); }
    inline void sqrt_   (const tpsa& t, tpsa* r)            { process1_(t, r, mad_tpsa_sqrt  ); }
    inline void exp_    (const tpsa& t, tpsa* r)            { process1_(t, r, mad_tpsa_exp   ); }
    inline void log_    (const tpsa& t, tpsa* r)            { process1_(t, r, mad_tpsa_log   ); }
    inline void sin_    (const tpsa& t, tpsa* r)            { process1_(t, r, mad_tpsa_sin   ); }
    inline void cos_    (const tpsa& t, tpsa* r)            { process1_(t, r, mad_tpsa_cos   ); }
    inline void tan_    (const tpsa& t, tpsa* r)            { process1_(t, r, mad_tpsa_tan   ); }
    inline void cot_    (const tpsa& t, tpsa* r)            { process1_(t, r, mad_tpsa_cot   ); }
    inline void sinc_   (const tpsa& t, tpsa* r)            { process1_(t, r, mad_tpsa_sinc  ); }
    inline void sinh_   (const tpsa& t, tpsa* r)            { process1_(t, r, mad_tpsa_sinh  ); }
    inline void cosh_   (const tpsa& t, tpsa* r)            { process1_(t, r, mad_tpsa_cosh  ); }
    inline void tanh_   (const tpsa& t, tpsa* r)            { process1_(t, r, mad_tpsa_tanh  ); }
    inline void coth_   (const tpsa& t, tpsa* r)            { process1_(t, r, mad_tpsa_coth  ); }
    inline void sinhc_  (const tpsa& t, tpsa* r)            { process1_(t, r, mad_tpsa_sinhc ); }
    inline void asin_   (const tpsa& t, tpsa* r)            { process1_(t, r, mad_tpsa_asin  ); }
    inline void acos_   (const tpsa& t, tpsa* r)            { process1_(t, r, mad_tpsa_acos  ); }
    inline void atan_   (const tpsa& t, tpsa* r)            { process1_(t, r, mad_tpsa_atan  ); }
    inline void acot_   (const tpsa& t, tpsa* r)            { process1_(t, r, mad_tpsa_acot  ); }
    inline void asinc_  (const tpsa& t, tpsa* r)            { process1_(t, r, mad_tpsa_asinc ); }
    inline void asinh_  (const tpsa& t, tpsa* r)            { process1_(t, r, mad_tpsa_asinh ); }
    inline void acosh_  (const tpsa& t, tpsa* r)            { process1_(t, r, mad_tpsa_acosh ); }
    inline void atanh_  (const tpsa& t, tpsa* r)            { process1_(t, r, mad_tpsa_atanh ); }
    inline void acoth_  (const tpsa& t, tpsa* r)            { process1_(t, r, mad_tpsa_acoth ); }
    inline void asinhc_ (const tpsa& t, tpsa* r)            { process1_(t, r, mad_tpsa_asinhc); }
    inline void erf_    (const tpsa& t, tpsa* r)            { process1_(t, r, mad_tpsa_erf   ); }
    inline void erfc_   (const tpsa& t, tpsa* r)            { process1_(t, r, mad_tpsa_erfc  ); }

    inline void sincos_ (const tpsa& t, tpsa* r1, tpsa* r2) { process1to2_(t, r1, r2, mad_tpsa_sincos);  }
    inline void sincosh_(const tpsa& t, tpsa* r1, tpsa* r2) { process1to2_(t, r1, r2, mad_tpsa_sincosh); }

    inline tpsa asin(const tpsa& t) { return process1(t, asin_); }
    inline tpsa sin (const tpsa& t) { return process1(t, sin_);  }

}; // namespace gtsa
// --- end --------------------------------------------------------------------o



#endif /* _GTPSA__H_ */
