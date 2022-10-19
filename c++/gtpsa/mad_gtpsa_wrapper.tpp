/* -*- c++ -*- */
#include <memory>
#include <vector>
#include <string>

#ifndef GTPSA_CLASS
#error "GTPSA CLASS has to be defined (either MadTpsa_ or MadCTpsa_)"
#endif /* GTPSA_CLASS */

#ifndef GTPSA_METH
#error "GTPSA METH has to be defined (either mad_tpsa_ or mad_ctpsa_)"
#endif /* GTPSA_METH */

#include <gtpsa/desc.hpp>

namespace gtpsa {
    class GTPSA_CLASS(Wrapper);

    class GTPSA_CLASS(LifeTimeManager) {
    public:
	inline GTPSA_CLASS(LifeTimeManager)(P *p)
	    : ptr(p)
	    { }
	inline ~GTPSA_CLASS(LifeTimeManager)(void) { GTPSA_METH(del)(this->ptr);  this->ptr = nullptr; }

    private:
	GTPSA_CLASS(LifeTimeManager)            (const GTPSA_CLASS(LifeTimeManager)& o) = delete;
	GTPSA_CLASS(LifeTimeManager)& operator= (const GTPSA_CLASS(LifeTimeManager)& o) = delete;

	inline const P*    getPtr(void) const { return this->ptr; }
	inline       P*    getPtr(void)       { return this->ptr; }

	P* ptr=nullptr;

	friend class GTPSA_CLASS(Wrapper);
    }; /* class GTPSA_CLASS(LifeTimeManager) */


    class GTPSA_CLASS(Wrapper) {
    private:
	// managed access to the underlying pointer
	std::shared_ptr<desc>                          t_desc;
	std::unique_ptr<GTPSA_CLASS(LifeTimeManager)>  ltm;

    public:
	inline auto getDescription(void) const {
	    return this->t_desc;
	}

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
	    , ltm  ( std::make_unique<GTPSA_CLASS(LifeTimeManager)>(GTPSA_METH(new)(o.getPtr(), mad_tpsa_same)) )
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
	inline GTPSA_CLASS(Wrapper)  clone(void)                const { GTPSA_CLASS(Wrapper) res(*this, mad_tpsa_same); res._copyInPlace(*this); return res; }

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
	inline auto name(void)                  const { return GTPSA_METH(nam)(this->getPtr()); }

	/**
	 * @todo: rename to order ?
	 */
	inline auto ord(void)                   const { return GTPSA_METH(ord)(this->getPtr()); }

	// initialization
	inline void clear(void)                       {        GTPSA_METH(clear)(this->getPtr()); }
	inline auto isNull(void)                const {        GTPSA_METH(isnul)(this->getPtr()); }

	inline void setName(std::string name)         {        GTPSA_METH(setnam)(this->getPtr(), name.c_str());}
	/**
	 * @brief:
	 */
	void setVariable(T v, idx_t iv_= 0, T scl_ = 0)
	                                                   {         GTPSA_METH(setvar)(this->getPtr(), v, iv_, scl_);	}
	// indexing / monomials (return idx_t = -1 if invalid)
	/**
	 *
	 */
	inline auto mono(std::vector<ord_t> m, idx_t i)     { return GTPSA_METH(mono)(this->getPtr(), m.size(), m.data(), i);}

	/**
	 *  @brief string mono "[0-9]*"
	 */
	inline auto idx(std::string s)                     { return GTPSA_METH(idxs)(this->getPtr(), s.size(), s.data());}
	inline auto idx(const std::vector<ord_t> m)        { return GTPSA_METH(idxm)(this->getPtr(), m.size(), m.data());}

	/**
	 * @brief sparse mono [(i,o)]
	 * @note not using overload ... could be misleading
	 */
	inline auto idxsm(const std::vector<int> m)       { return GTPSA_METH(idxsm) (this->getPtr(), m.size(), m.data());}
	inline auto cycle(std::vector<ord_t> m, idx_t i, T *v)
	    { return GTPSA_METH(cycle)(this->getPtr(), m.size(), m.data(), i, v);}
	/**
	 * @todo: use standard accessor operators ?
	 */
	inline auto get(void)                        const { return GTPSA_METH(get0) (this->getPtr()                     ); }
	inline auto get(const idx_t i)               const { return GTPSA_METH(geti) (this->getPtr(), i                  ); }
	inline auto get(std::string s)               const { return GTPSA_METH(gets) (this->getPtr(), s.size(), s.data() ); }
	inline auto get(const std::vector<ord_t> m)  const { return GTPSA_METH(getm) (this->getPtr(), m.size(), m.data() ); }
	/**
	 * @brief sparse mono "[(i, o)]*"
	 */
	inline auto getsm(const std::vector<int> m)  const { return GTPSA_METH(getsm) (this->getPtr(), m.size(), m.data() ); }


	/**
	 * @brief a*x[0]+b
	 */
	inline void set(                            T a, T b) { GTPSA_METH(set0) (this->getPtr()                    , a, b ); }

	/**
	 * @brief a*x[i]+b
	 */
	inline void set(const idx_t i             , T a, T b) { GTPSA_METH(seti) (this->getPtr(), i                 , a, b ); }
	/**
	 * @brief a*x[m]+b
	 */
	inline void set(std::string s             , T a, T b) { GTPSA_METH(sets) (this->getPtr(), s.size(), s.data(), a, b ); }
	/**
	 * @brief a*x[m]+b
	 */
	inline void set(const std::vector<ord_t> m, T a, T b) { GTPSA_METH(setm) (this->getPtr(), m.size(), m.data(), a, b ); }

	/**
	 * @brief a*x[m]+b, sparse mono [(i,o)]
	 * @todo vector of pairs?
	 */
	inline auto setsm(const std::vector<int> m, T a, T b) { return GTPSA_METH(setsm)  (this->getPtr(), m.size(), m.data(), a, b ); }


	inline void getv(idx_t i,       std::vector<T> *v) const { GTPSA_METH(getv)(this->getPtr(), i, v->size(), v->data() ); }
	inline void setv(idx_t i, const std::vector<T> &v)       { GTPSA_METH(setv)(this->getPtr(), i, v.size() , v.data()  ); }


	inline void print(str_t name_, num_t eps_, int nohdr_, FILE *stream_){
	    GTPSA_METH(print)(this->getPtr(), name_, eps_, nohdr_, stream_);
	}

    protected:

	inline const P*    getPtr(void) const { return this->ltm->getPtr(); }
	inline       P*    getPtr(void)       { return this->ltm->getPtr(); }

    }; /* class GTPSA_CLASS(Wrapper) */


} // namespace gtpsa
