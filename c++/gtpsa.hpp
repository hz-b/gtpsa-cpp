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
	inline tpsa_mgr(const tpsa_mgr&& o)      { this->ptr = o.ptr;  };
	inline tpsa_mgr(tpsa_t *p) {
	    this->ptr = p;
	}
	inline ~tpsa_mgr(void){
	    mad_tpsa_del(this->ptr);
	    this->ptr = nullptr; // a bit of paranoia
	}
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
    inline void getv(idx_t i, std::vector<num_t> &v) { mad_tpsa_setv(this->getPtr(), i, v.size(), v.data());   }
    inline void setv(idx_t i, std::vector<num_t> *v) { mad_tpsa_setv(this->getPtr(), i, v->size(), v->data()); }

    inline void print(str_t name_, num_t eps_, int nohdr_, FILE *stream_){
	mad_tpsa_print(this->getPtr(), name_, eps_, nohdr_, stream_);
    }

    inline tpsa& operator += (const tpsa& o){ mad_tpsa_add(this->getPtr(), o.getPtr(), this->getPtr()); return *this;}
    // (a_i-b_i)/max(|a_i|,1)
    inline tpsa& operator -= (const tpsa& o){ mad_tpsa_sub(this->getPtr(), o.getPtr(), this->getPtr()); return *this;}
    inline tpsa& operator *= (const tpsa& o){ mad_tpsa_mul(this->getPtr(), o.getPtr(), this->getPtr()); return *this;}
    inline tpsa& operator /= (const tpsa& o){ mad_tpsa_div(this->getPtr(), o.getPtr(), this->getPtr()); return *this;}

private:
    std::unique_ptr<tpsa_mgr>  tm;

    inline const tpsa_t * getPtr(void) const { return this->tm->getPtr(); }
    inline tpsa_t *       getPtr(void)       { return this->tm->getPtr(); }

    friend inline auto norm (const tpsa& a);

    friend inline void add  (const tpsa& a, const tpsa& b, tpsa *r);
    friend inline void sub  (const tpsa& a, const tpsa& b, tpsa *r);
    friend inline void mul  (const tpsa& a, const tpsa& b, tpsa *r);
    friend inline void div  (const tpsa& a, const tpsa& b, tpsa *r);

    friend inline void pow  (const tpsa& a, const tpsa& b, tpsa *r);
    friend inline void pow  (const tpsa& a,         int n, tpsa *r);
    friend inline void pow  (const tpsa& a,       num_t v, tpsa *r);

    friend inline void asin_(const tpsa& t1, tpsa *t2);
    friend inline tpsa asin (const tpsa& t1);
    friend inline void sin_ (const tpsa& t1, tpsa *t2);
    friend inline tpsa sin  (const tpsa& t1);

    friend inline auto ordn(std::vector<tpsa *> objs);

}; // class tpsa

    inline auto ordn(std::vector<tpsa *> objs) {
	std::vector<const tpsa_t *> tv(objs.size());
	auto toPtr = [](tpsa * obj){ return obj->getPtr();};
	std::transform(objs.begin(), objs.end(), tv.begin(), toPtr);
	return mad_tpsa_ordn(tv.size(), tv.data());
    }

    inline auto norm(const tpsa& a)                        { return mad_tpsa_nrm(a.getPtr()); }

    inline void add (const tpsa& a, const tpsa& b, tpsa *r){ mad_tpsa_add (a.getPtr(), b.getPtr(), r->getPtr()); }
    inline void sub (const tpsa& a, const tpsa& b, tpsa *r){ mad_tpsa_sub (a.getPtr(), b.getPtr(), r->getPtr()); }
    inline void mul (const tpsa& a, const tpsa& b, tpsa *r){ mad_tpsa_mul (a.getPtr(), b.getPtr(), r->getPtr()); }
    inline void div (const tpsa& a, const tpsa& b, tpsa *r){ mad_tpsa_div (a.getPtr(), b.getPtr(), r->getPtr()); }

    inline void pow (const tpsa& a, const tpsa& b, tpsa *r){ mad_tpsa_pow (a.getPtr(), b.getPtr(), r->getPtr()); }
    inline void pow (const tpsa& a,         int n, tpsa *r){ mad_tpsa_powi(a.getPtr(), n,          r->getPtr()); }
    inline void pow (const tpsa& a,       num_t v, tpsa *r){ mad_tpsa_pown(a.getPtr(), v,          r->getPtr()); }



    // test the interface ...
    void inline asin_(const tpsa& t1, tpsa *t2){ mad_tpsa_asin(t1.getPtr(), t2->getPtr()); }
    void inline sin_ (const tpsa& t1, tpsa *t2){ mad_tpsa_asin(t1.getPtr(), t2->getPtr()); }
    tpsa inline asin(const tpsa& t) {
	auto ret = tpsa(t, mad_tpsa_same);
	mad_tpsa_asin(t.getPtr(), ret.getPtr());
	return std::move(ret);
    }

    tpsa inline sin(const tpsa& t) {
	auto ret = tpsa(t, mad_tpsa_same);
	mad_tpsa_sin(t.getPtr(), ret.getPtr());
	return std::move(ret);
    }

}; // namespace gtsa
// --- end --------------------------------------------------------------------o



#endif /* _GTPSA__H_ */
