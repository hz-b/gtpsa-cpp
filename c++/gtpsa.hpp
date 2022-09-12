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
#include <vector>

extern "C" {
#include <mad_tpsa.h>
}

namespace gtpsa {
class tpsa{
public:
    /**
     * if mo > d_mo, mo = d_mo
     */
    inline const tpsa_t * getPtr(void) const { return this->t_tpsa; }
    inline tpsa_t *       getPtr(void)  { return this->t_tpsa; }
    inline tpsa(const desc &desc, const ord_t mo) { this->t_tpsa = mad_tpsa_newd(desc.getPtr(), mo);  }
    /**
     *
     * @todo: define it for ctpsa &t too
     */
    inline tpsa(const tpsa &t, const ord_t mo) {this->t_tpsa = mad_tpsa_new(t.getPtr(), mo); } // ok with t=(tpsa_t*)ctpsa;
    inline ~tpsa(void){ mad_tpsa_del(this->t_tpsa); }

    /**
     * move ctor
     */
    inline tpsa(const tpsa&& o) : t_tpsa(std::move(o.t_tpsa)) {}

    /**
     * Copy ctor ....
     */
    // inline tpsa(const tpsa& o){ this->t_tpsa = mad_tpsa_new(o.getPtr(), mad_tpsa_same); };
    inline tpsa(const tpsa& o) = delete; //{ this->t_tpsa = mad_tpsa_new(o.getPtr(), mad_tpsa_same); };
    /**
     * copies content of an other tpsa object to this
     * @todo implement a clone function
     *
     */
    inline void _copy(const tpsa &o){ mad_tpsa_copy(o.getPtr(), this->t_tpsa); }

// introspection
    /*
     * @todo: memory handling shared ptrs?
     */
    inline const desc getDesc(void) const { return gtpsa::desc(mad_tpsa_desc(this->t_tpsa)); }

    /**
     * @short set uid if != 0
     *
     * @todo review if separate in constant and non constant part ...
     */
    inline auto uid(int32_t uid_)         { return mad_tpsa_uid(this->t_tpsa, uid_); }
    inline auto uid(void)           const { return mad_tpsa_uid(this->t_tpsa, 0); }
    inline auto len(void)           const { return mad_tpsa_len(this->t_tpsa);}
    /*
     * @todo return std::string?
     */
    inline auto name(void)          const { return mad_tpsa_nam(this->t_tpsa); }
    inline auto ord(void)           const { return mad_tpsa_ord(this->t_tpsa); }
#if 0
    inline auto ordv(...)           const { return mad_tpsa_ord(this->t_tpsa); }
#endif

    // initialization

    inline void clear(void)               { mad_tpsa_clear(this->t_tpsa); }
    inline auto isNull(void)        const { mad_tpsa_isnul(this->t_tpsa); }

    /*
     *
     * @todo replace str_t with std::string ...
     */
    inline void  setName(str_t nam)       { mad_tpsa_setnam(this->t_tpsa, nam);}

    /*
     * @todo: use standard accessor operators ?
     */
    inline void getv(idx_t i, std::vector<num_t> &v) { mad_tpsa_setv(this->t_tpsa, i, v.size(), v.data());   }
    inline void setv(idx_t i, std::vector<num_t> *v) { mad_tpsa_setv(this->t_tpsa, i, v->size(), v->data()); }

    inline void print(str_t name_, num_t eps_, int nohdr_, FILE *stream_){
	mad_tpsa_print(this->t_tpsa, name_, eps_, nohdr_, stream_);
    }

private:
    tpsa_t * t_tpsa;
}; // class tpsa

    inline auto ordn(std::vector<tpsa *> objs) {
	std::vector<const tpsa_t *> tv(objs.size());
    auto toPtr = [](tpsa * obj){ return obj->getPtr();};
    std::transform(objs.begin(), objs.end(), tv.begin(), toPtr);
    return mad_tpsa_ordn(tv.size(), tv.data());
}


    // test the interface ...
    void inline asin_(const tpsa& t1, tpsa *t2){ mad_tpsa_asin(t1.getPtr(), t2->getPtr()); }
    tpsa inline asin(const tpsa& t) {
	tpsa ret = std::move(t);
	 mad_tpsa_asin(t.getPtr(), ret.getPtr());
	return ret;
    }
}; // namespace gtsa
// --- end --------------------------------------------------------------------o



#endif /* _GTPSA__H_ */
