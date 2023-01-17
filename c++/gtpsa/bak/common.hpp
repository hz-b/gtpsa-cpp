/* intentionally not sentinel */
#ifndef _GTPSA_COMMON_H_
#define _GTPSA_COMMON_H_ 1

    /**
     * forward declaration
     */
    template<typename P, typename base_t>
    class Base;

    /**
     * @brief Manage mad_ng object pointer lifetime
     */
    template<typename P, typename base_t>
    class TpsaSessionManager {
    public:
	inline TpsaMgr(P *p)
	    : ptr(p)
	    { }
	inline ~TpsaMgr(void) { GTPSA_CLASS(del)(this->ptr);  this->ptr = nullptr; }

    private:
	TpsaMgr            (const TpsaMgr& o) = delete;
	TpsaMgr& operator= (const TpsaMgr& o) = delete;

	inline const P*    getPtr(void) const { return this->ptr; }
	inline       P*    getPtr(void)       { return this->ptr; }

	P* ptr=nullptr;

	friend class  tpsa;
	friend class ctpsa;
	friend class Base<P, base_t>;
    }; // class TpsaMgr


    template<typename P, typename base_t>
    inline Base<P, base_t> process1(const Base<P, base_t>& t, void (*func)(const Base<P, base_t>& t, Base<P, base_t> *r) );

    // forward declaration ... required that operators will use the same functions
    template<typename P, typename base_t>
    inline void  add (const Base<P, base_t>& a, const Base<P, base_t>& b, Base<P, base_t>* r);
    template<typename P, typename base_t>
    inline void  dif (const Base<P, base_t>& a, const Base<P, base_t>& b, Base<P, base_t>* r);
    template<typename P, typename base_t>
    inline void  sub (const Base<P, base_t>& a, const Base<P, base_t>& b, Base<P, base_t>* r);
    template<typename P, typename base_t>
    inline void  mul (const Base<P, base_t>& a, const Base<P, base_t>& b, Base<P, base_t>* r);
    template<typename P, typename base_t>
    inline void  div (const Base<P, base_t>& a, const Base<P, base_t>& b, Base<P, base_t>* r);
    template<typename P, typename base_t>
    inline void  pow (const Base<P, base_t>& a, const Base<P, base_t>& b, Base<P, base_t>* r);

    template<typename P, typename base_t>
    inline Base<P, base_t>  add (const Base<P, base_t>& a, const Base<P, base_t>& b         );
    template<typename P, typename base_t>
    inline Base<P, base_t>  dif (const Base<P, base_t>& a, const Base<P, base_t>& b         );
    template<typename P, typename base_t>
    inline Base<P, base_t>  sub (const Base<P, base_t>& a, const Base<P, base_t>& b         );
    template<typename P, typename base_t>
    inline Base<P, base_t>  mul (const Base<P, base_t>& a, const Base<P, base_t>& b         );
    template<typename P, typename base_t>
    inline Base<P, base_t>  div (const Base<P, base_t>& a, const Base<P, base_t>& b         );

    template<typename P, typename base_t>
    inline void  add_d (const Base<P, base_t>& a, const base_t b, Base<P, base_t>* r);
    template<typename P, typename base_t>
    inline void  sub_d (const Base<P, base_t>& a, const base_t b, Base<P, base_t>* r);
    template<typename P, typename base_t>
    inline void  mul_d (const Base<P, base_t>& a, const base_t b, Base<P, base_t>* r);
    template<typename P, typename base_t>
    inline void  div_d (const Base<P, base_t>& a, const base_t b, Base<P, base_t>* r);
    template<typename P, typename base_t>
    inline void  pow_d (const Base<P, base_t>& a, const base_t b, Base<P, base_t>* r);

    template<typename P, typename base_t>
    inline Base<P, base_t>  add_d (const Base<P, base_t>& a, const base_t b          );
    template<typename P, typename base_t>
    inline Base<P, base_t>  dif_d (const Base<P, base_t>& a, const base_t b          );
    template<typename P, typename base_t>
    inline Base<P, base_t>  sub_d (const Base<P, base_t>& a, const base_t b          );
    template<typename P, typename base_t>
    inline Base<P, base_t>  mul_d (const Base<P, base_t>& a, const base_t b          );
    template<typename P, typename base_t>
    inline Base<P, base_t>  div_d (const Base<P, base_t>& a, const base_t b          );

    template<typename P, typename base_t>
    inline Base<P, base_t>  scl   (const tpsa &a, const base_t v          );

    template<typename P, typename base_t>
    inline void div_d (const P& a, const base_t b, P* r ){ process2_(a, b, r, [](base_t a, base_t b, base_t *r){ *r = a / b; }); }

    template<typename P, typename base_t>
    inline void process1_   (const Base<P, base_t>& a, Base<P, base_t> *r,                 void (*func)(const P* a,P *r) );

    template<typename P, typename base_t>
    inline Base<P, base_t> process2(const Base<P, base_t>& a, const Base<P, base_t>&   b, void (*func)(const Base<P, base_t>& a, const Base<P, base_t>&   b, Base<P, base_t> *r) );

    /**
     * @brief base class for mad_tpsa and mad_ctpsa objects wrappers
     */
    template<typename P, typename base_t>
    class Base {
    public:
    	inline auto getDescription(void)   const { return this->t_desc; }

	/**
	 *
	 * if mo > d_mo, mo = d_mo
	 * @todo: define it for ctpsa &t too
	 */
	inline Base(std::shared_ptr<desc> desc, const ord_t mo)
	    : t_desc(desc)
	    , tm( std::make_unique<TpsaMgr<P, base_t>>(GTPSA_CLASS(newd)(desc->getPtr(), mo)) )
	    {}
	/**
	 *
	 * @todo: define it for ctpsa &t too
	 */
	inline Base(const Base&              t,  const ord_t mo)
	    : t_desc(t.getDescription())
	    , tm( std::make_unique<TpsaMgr<P, base_t>>(GTPSA_CLASS(new)(t.getPtr(), mo)) )
	    { }

	/**
	 * Copy ctor: use clone instead if required ....
	 */
#ifndef GTSPA_ONLY_OPTIMISED_OPS
	inline Base(const Base&              o)
	    : t_desc( o.getDescription())
	    , tm  ( std::make_unique<TpsaMgr<P, base_t>>(GTPSA_CLASS(new)(o.getPtr(), mad_tpsa_same)) )
	    { this->_copyInPlace(o); };
#else /* GTSPA_ONLY_OPTIMISED_OPS */
	inline Base(const Base&              o) = delete;
#endif /* GTSPA_ONLY_OPTIMISED_OPS */

	/**
	 * move ctor
	 */
	inline Base(Base&& o)                   = default; //  noexcept :  tm(std::move(o.tm)), t_desc(std::move(o.t_desc)) { };
	inline Base& operator= (Base && o)      = default;

	/**
	 * copies content of an other tpsa object to this
	 * @todo implement a clone function
	 *
	 */
	inline void  _copyInPlace(const Base &o)      {        GTPSA_CLASS(copy)(o.getPtr(), this->getPtr());                }
	inline Base  clone(void)                const { Base res(*this, mad_tpsa_same); res._copyInPlace(*this); return res; }

	/**
	 * @short set uid if != 0
	 *
	 * @todo review if separate in constant and non constant part ...
	 */
	inline auto uid(int32_t uid_)                 { return GTPSA_CLASS(uid)(this->getPtr(), uid_); }
	// inline auto uid(void)                const { return GTPSA_CLASS(uid)((P*)(this->getPtr()), 0); }
	inline auto len(void)                   const { return GTPSA_CLASS(len)(this->getPtr()); }

	/*
	 * @todo return std::string?
	 */
	inline auto name(void)                  const { return GTPSA_CLASS(nam)(this->getPtr()); }

	/**
	 * @todo: rename to order ?
	 */
	inline auto ord(void)                   const { return GTPSA_CLASS(ord)(this->getPtr()); }

	// initialization
	inline void clear(void)                       {        GTPSA_CLASS(clear)(this->getPtr()); }
	inline auto isNull(void)                const {        GTPSA_CLASS(isnul)(this->getPtr()); }

	inline void setName(std::string name)         {        GTPSA_CLASS(setnam)(this->getPtr(), name.c_str());}
	/**
	 * @brief:
	 */
	void setVariable(base_t v, idx_t iv_= 0, base_t scl_ = 0)
	                                                   {         GTPSA_CLASS(setvar)(this->getPtr(), v, iv_, scl_);	}
	// indexing / monomials (return idx_t = -1 if invalid)
	/**
	 *
	 */
	inline auto mono(std::vector<ord_t> m, idx_t i)     { return GTPSA_CLASS(mono)(this->getPtr(), m.size(), m.data(), i);}

	/**
	 *  @brief string mono "[0-9]*"
	 */
	inline auto idx(std::string s)                     { return GTPSA_CLASS(idxs)(this->getPtr(), s.size(), s.data());}
	inline auto idx(const std::vector<ord_t> m)        { return GTPSA_CLASS(idxm)(this->getPtr(), m.size(), m.data());}

	/**
	 * @brief sparse mono [(i,o)]
	 * @note not using overload ... could be misleading
	 */
	inline auto idxsm(const std::vector<int> m)       { return GTPSA_CLASS(idxsm) (this->getPtr(), m.size(), m.data());}
	inline auto cycle(std::vector<ord_t> m, idx_t i, base_t *v)
	    { return GTPSA_CLASS(cycle)(this->getPtr(), m.size(), m.data(), i, v);}
	/**
	 * @todo: use standard accessor operators ?
	 */
	inline auto get(void)                        const { return GTPSA_CLASS(get0)  (this->getPtr()                     ); }
	inline auto get(const idx_t i)               const { return GTPSA_CLASS(geti)  (this->getPtr(), i                  ); }
	inline auto get(std::string s)               const { return GTPSA_CLASS(gets)  (this->getPtr(), s.size(), s.data() ); }
	inline auto get(const std::vector<ord_t> m)  const { return GTPSA_CLASS(getm)  (this->getPtr(), m.size(), m.data() ); }
	/**
	 * @brief sparse mono "[(i, o)]*"
	 */
	inline auto getsm(const std::vector<int> m)  const { return GTPSA_CLASS(getsm) (this->getPtr(), m.size(), m.data() ); }

	inline auto cst(void)                        const { return this->get(); }

	/**
	 * @brief a*x[0]+b
	 */
	inline void set(                            base_t a, base_t b) { GTPSA_CLASS(set0) (this->getPtr()                    , a, b ); }

	/**
	 * @brief a*x[i]+b
	 */
	inline void set(const idx_t i             , base_t a, base_t b) { GTPSA_CLASS(seti) (this->getPtr(), i                 , a, b ); }
	/**
	 * @brief a*x[m]+b
	 */
	inline void set(std::string s             , base_t a, base_t b) { GTPSA_CLASS(sets) (this->getPtr(), s.size(), s.data(), a, b ); }
	/**
	 * @brief a*x[m]+b
	 */
	inline void set(const std::vector<ord_t> m, base_t a, base_t b) { GTPSA_CLASS(setm) (this->getPtr(), m.size(), m.data(), a, b ); }

	/**
	 * @brief a*x[m]+b, sparse mono [(i,o)]
	 * @todo vector of pairs?
	 */
	inline auto setsm(const std::vector<int> m, base_t a, base_t b) { return GTPSA_CLASS(setsm)  (this->getPtr(), m.size(), m.data(), a, b ); }


	inline void getv(idx_t i,       std::vector<base_t> *v) const { GTPSA_CLASS(getv)(this->getPtr(), i, v->size(), v->data() ); }
	inline void setv(idx_t i, const std::vector<base_t> &v)       { GTPSA_CLASS(setv)(this->getPtr(), i, v.size() , v.data()  ); }


	inline void print(str_t name_, num_t eps_, int nohdr_, FILE *stream_){
	    GTPSA_CLASS(print)(this->getPtr(), name_, eps_, nohdr_, stream_);
	}


#ifndef GTSPA_ONLY_OPTIMISED_OPS
	/**
	 * These operators need to make copys of the actual instance
	 * In the current implementation I assum the compiler can not fully optimise it away
	 * if not required
	 */
	inline Base&  operator =  (const Base& o )       { this->_copyInPlace(o); return *this; }
	/**
	 * @todo do I need to make a copy ....?
	 */
	// negation
	inline Base   operator -  (void)           const { Base r = this->clone(); r *= -1e0; return r;}

	//process2
	inline Base  operator +  (const Base& o ) const { return process2(*this, o, add); }
	inline Base  operator -  (const Base& o ) const { return process2(*this, o, sub); }
	inline Base  operator *  (const Base& o ) const { return process2(*this, o, mul); }
	inline Base  operator /  (const Base& o ) const { return process2(*this, o, div); }


	// inline Base  operator +  (const base_t o) const { return process2(*this, o, add_; }
	// inline Base  operator -  (const base_t o) const { return process2(*this, o, sub_d); }
	// inline Base  operator *  (const base_t o) const { return process2(*this, o, mul_d); }
	// inline Base  operator /  (const base_t o) const { return process2(*this, o, div_d); }

#endif
	// forward declaration ... needs to be implemented in the derived class
	inline  void show(std::ostream& strm, int level) const {
	    strm << "Method show should be implemented in derived class!";
	};

	/* functions  that need to access ptr ... */


    protected:
	// managed access to the underlying pointer
	std::shared_ptr<desc>                t_desc;
	std::unique_ptr<TpsaMgr<P, base_t>>  tm;

	inline const P*    getPtr(void) const { return this->tm->getPtr(); }
	inline       P*    getPtr(void)       { return this->tm->getPtr(); }


    }; // class Base

    // ------------------------------------------------------------------------------
    // helper functions for different functions
    // start with inplace

    /**
     * @brief single argument in single argument out
     * @todo renaming required
     */
    template<typename P, typename base_t>
    inline void process1_   (const Base<P, base_t>& a, Base<P, base_t> *r,                 void (*func)(const P* a,P *r) ) {
	func(a.getPtr(), r->getPtr());
    }

    /**
     * @brief single argument in, two arguments out
     */
    template<typename P, typename base_t>
    inline void process1to2_(const Base<P, base_t>& a, Base<P, base_t>* r1, Base<P, base_t> *r2,      void (*func)(const P* a, P *r1,  P *r2) ) {
	func(a.getPtr(), r1->getPtr(), r2->getPtr());
    }
    /**
     * @brief two arguments in, single argument out
     */
    template<typename P, typename base_t>
    inline void process2_ (const Base<P, base_t>& a, const Base<P, base_t>& b, Base<P, base_t> *r,  void (*func)(const P* a, const P* b, P *r) ) {
	func(a.getPtr(), b.getPtr(), r->getPtr());
    }
    // template<typename P, typename base_t>
    // inline void process2_   (const Base<P, base_t>& a, const num_t b, Base<P, base_t> *r,        void (*func)(const P* a, num_t b, P *r) ) {
    // func(a.getPtr(), b, r->getPtr());
    // }
    template<typename P, typename base_t>
    inline void process2_  (const Base<P, base_t>& a, const base_t b, Base<P, base_t> *r, void (*func)(const base_t a, const base_t b, base_t *r) ) {
	base_t rv;
	func(GTPSA_CLASS(get0)(a.getPtr()), b, &rv);
	GTPSA_CLASS(set0)(r->getPtr(), 0.0, rv);
    }




    /**
     * @brief single argument in, single argument out, return argument allocated
     */
    template<typename P, typename base_t>
    Base<P, base_t> inline process1(const Base<P, base_t>& t, void (*func)(const Base<P, base_t>& t, Base<P, base_t> *r) )                                 { auto ret = Base<P, base_t>(t, mad_tpsa_same); func(t, &ret);    return ret; }
    /**
     * @brief two arguments in, single argument out, return argument allocated
     */
    template<typename P, typename base_t>
    inline Base<P, base_t>  process2(const Base<P, base_t>& a, const Base<P, base_t>&  b, void (*func)(const Base<P, base_t>& a, const Base<P, base_t>&  b, Base<P, base_t> *r) ) { auto ret = Base<P, base_t>(a, mad_tpsa_same); func(a, b, &ret); return ret; }

    // template<typename P, typename base_t>
    // Base<P, base_t> inline process2(const Base<P, base_t>& a, const Base<P, base_t>&  b, void (*func)(const Base<P, base_t>& a, const Base<P, base_t>&  b, Base<P, base_t> *r) ) { auto ret = Base<P, base_t>(a, mad_tpsa_same); func(a, b, &ret); return ret; }
    // template<typename P, typename base_t>
    // Base<P, base_t> inline process2(const Base<P, base_t>& a, const base_t b, void (*func)(const Base<P, base_t>& a, const base_t b, Base<P, base_t> *r) ) { auto ret = Base<P, base_t>(a, mad_tpsa_same); func(a, b, &ret); return ret; }
    // template<typename P, typename base_t>
    // Base<P, base_t> inline process2(const Base<P, base_t>& a, const base_t b, void (*func)(const Base<P, base_t>& a, const Base<P, base_t>&  b, Base<P, base_t> *r) ) { auto ret = Base<P, base_t>(a, mad_tpsa_same); ret.set(0, b); func(a, ret, &ret); return ret; }

    // ------------------------------------------------------------------------------
    // helper functions for different functions
    // start with inplace


    /**
     * @brief single argument in, single argument out, return argument allocated
     */
    /**
     * @brief c += v*a, aliasing OK
     */
    template<typename P, typename base_t>
    inline void acc     (const Base<P, base_t> &a, base_t v, Base<P, base_t> *c) { process2_(a, v, c, GTPSA_CLASS(acc)    ); }
    /**
     * @brief c = v*a, aliasing OK ?
     */
    template<typename P, typename base_t>
    inline void scl     (const Base<P, base_t> &a, base_t v, Base<P, base_t> *c) { process2_(a, v, c, GTPSA_CLASS(scl)    ); }
    /**
     * @brief c = v/a, aliasing OK ?
     */
    template<typename P, typename base_t>
    inline void inv     (const Base<P, base_t> &a, base_t v, Base<P, base_t> *c) { process2_(a, v, c, GTPSA_CLASS(inv)    ); }
    /**
     * @brief c = v/sqrt(a), aliasing OK ?
     */
    template<typename P, typename base_t>
    inline void invsqrt (const Base<P, base_t> &a, base_t v, Base<P, base_t> *c) { process2_(a, v, c, GTPSA_CLASS(invsqrt) ); }

    template<typename P, typename base_t>
    inline Base<P, base_t> acc     (const Base<P, base_t> &a, base_t v         ) { return process2 (a, v,      acc    ); }
    template<typename P, typename base_t>
    inline Base<P, base_t> scl     (const Base<P, base_t> &a, const base_t v   ) { return process2 (a, v,      scl    ); }
    template<typename P, typename base_t>
    inline Base<P, base_t> inv     (const Base<P, base_t> &a, base_t v         ) { return process2 (a, v,      inv    ); }
    template<typename P, typename base_t>
    inline Base<P, base_t> invsqrt (const Base<P, base_t> &a, base_t v         ) { return process2 (a, v,      invsqrt); }

    template<typename P, typename base_t>
    inline void add   (const Base<P, base_t>& a, const Base<P, base_t>& b,  Base<P, base_t>* r ){ process2_(a, b, r, GTPSA_CLASS(add)); }
    /**
     * @brief  (a_i-b_i)/max(|a_i|,1)
     */
    template<typename P, typename base_t>
    inline void dif   (const Base<P, base_t>& a, const Base<P, base_t>& b,  Base<P, base_t>* r ){ process2_(a, b, r, GTPSA_CLASS(dif)); }
    template<typename P, typename base_t>
    inline void sub   (const Base<P, base_t>& a, const Base<P, base_t>& b,  Base<P, base_t>* r ){ process2_(a, b, r, GTPSA_CLASS(sub)); }
    template<typename P, typename base_t>
    inline void mul   (const Base<P, base_t>& a, const Base<P, base_t>& b,  Base<P, base_t>* r ){ process2_(a, b, r, GTPSA_CLASS(mul)); }
    template<typename P, typename base_t>
    inline void div   (const Base<P, base_t>& a, const Base<P, base_t>& b,  Base<P, base_t>* r ){ process2_(a, b, r, GTPSA_CLASS(div)); }
    template<typename P, typename base_t>
    inline void pow   (const Base<P, base_t>& a, const Base<P, base_t>& b,  Base<P, base_t>* r ){ process2_(a, b, r, GTPSA_CLASS(pow)); }

    template<typename P, typename base_t>
    inline void pow   (const Base<P, base_t>& a,         int n,  Base<P, base_t>* r ){ GTPSA_CLASS(powi) (a.getPtr(), n, r->getPtr()); }
    template<typename P, typename base_t>
    inline void pow   (const Base<P, base_t>& a,       base_t v,  Base<P, base_t>* r ){ GTPSA_CLASS(pown) (a.getPtr(), v, r->getPtr()); }

#if 0
    template<typename P, typename base_t>
    inline void add_d (const Base<P, base_t>& a, const base_t b, Base<P, base_t>* r ){ process2_(a, b, r, [](base_t a, base_t b, base_t *r){ *r = a + b; }); }
    template<typename P, typename base_t>
    inline void sub_d (const Base<P, base_t>& a, const base_t b, Base<P, base_t>* r ){ process2_(a, b, r, [](base_t a, base_t b, base_t *r){ *r = a - b; }); }
    template<typename P, typename base_t>
    inline void mul_d (const Base<P, base_t>& a, const base_t b, Base<P, base_t>* r ){ process2_(a, b, r, [](base_t a, base_t b, base_t *r){ *r = a * b; }); }
    template<typename P, typename base_t>
    inline void div_d (const Base<P, base_t>& a, const base_t b, Base<P, base_t>* r ){ process2_(a, b, r, [](base_t a, base_t b, base_t *r){ *r = a / b; }); }
#endif

    template<typename P, typename base_t>
    inline Base<P, base_t> add   (const Base<P, base_t>& a, const Base<P, base_t>& b           ){ return process2(a, b, add); }
    template<typename P, typename base_t>
    inline Base<P, base_t> sub   (const Base<P, base_t>& a, const Base<P, base_t>& b           ){ return process2(a, b, sub); }
    template<typename P, typename base_t>
    inline Base<P, base_t> mul   (const Base<P, base_t>& a, const Base<P, base_t>& b           ){ return process2(a, b, mul); }
    template<typename P, typename base_t>
    inline Base<P, base_t> div   (const Base<P, base_t>& a, const Base<P, base_t>& b           ){ return process2(a, b, div); }


    // inline tpsa add_d (const tpsa& a, const base_t b          ){ return process2(a, b, add); }
    // inline tpsa sub_d (const tpsa& a, const base_t b          ){ return process2(a, b, sub); }
    // inline tpsa mul_d (const tpsa& a, const base_t b          ){ return process2(a, b, mul); }
    // inline tpsa div_d (const tpsa& a, const base_t b          ){ return process2(a, b, div); }

    /* standard mathematical functions ... trigonometic etc ... taking one argument returning one */


    #ifndef GTSPA_ONLY_OPTIMISED_OPS
    template<typename P, typename base_t>
    inline Base<P, base_t>  operator + (const base_t a, const Base<P, base_t>&  b) { auto r = Base<P, base_t>(b, mad_tpsa_same); r.set(0, a    ); r += b; return r; }
    template<typename P, typename base_t>
    inline Base<P, base_t>  operator + (const Base<P, base_t>&  a, const base_t b) { return b + a;                                                       }
    // avoid double copy of b: a - b => -(b-a) => -b + a
    template<typename P, typename base_t>
    inline Base<P, base_t>  operator - (const base_t a, const Base<P, base_t>&  b) { auto r = Base<P, base_t>(b, mad_tpsa_same); r.set(0, a    ); r -= b; return r; }
    template<typename P, typename base_t>
    inline Base<P, base_t>  operator - (const Base<P, base_t>&  a, const base_t b) { return a + (-b);                                                    }
    template<typename P, typename base_t>
    inline Base<P, base_t>  operator * (const base_t a, const Base<P, base_t>&  b) { auto r = Base<P, base_t>(b, mad_tpsa_same); r.set(0, a    ); r *= b; return r; }
    template<typename P, typename base_t>
    inline Base<P, base_t>  operator * (const Base<P, base_t>&  a, const base_t b) { return b * a;                                                       }
    template<typename P, typename base_t>
    inline Base<P, base_t>  operator / (const Base<P, base_t>&  a, const base_t b) { auto r = Base<P, base_t>(a, mad_tpsa_same); r.set(0, 1e0/b); r *= a; return r; }
    template<typename P, typename base_t>
    inline Base<P, base_t>  operator / (const base_t a, const Base<P, base_t>&  b) { auto r = Base<P, base_t>(b, mad_tpsa_same); r.set(0,     a); r /= b; return r; }
    // inline tpsa  operator / (const double a, const tpsa&  b) { return inv(b, a); }
#endif

#ifdef FUNC_ARG1
#undef FUNC_ARG1
#endif
#define FUNC_ARG1_WITH_RET_ARG(fname)                                                                                 \
    template<typename P, typename base_t>                                                                             \
    inline void fname ## _ (const Base<P, base_t>& t, Base<P, base_t>* r){ process1_(t, r, GTPSA_CLASS(fname) ); }

#define FUNC_ARG1_NO_RET_ARG(fname)                                                                                   \
    template<typename P, typename base_t>				                                              \
    inline Base<P, base_t> fname (const Base<P, base_t>& t){ return process1(t, fname ## _); }
#define FUNC_ARG1(fname) FUNC_ARG1_WITH_RET_ARG(fname) FUNC_ARG1_NO_RET_ARG(fname)
#include <gtpsa/funcs.h>

    template<typename P, typename base_t>                                                                             \
    inline void sincos_ (const Base<P, base_t>& t, Base<P, base_t>* r1, Base<P, base_t>* r2) { process1to2_(t, r1, r2, GTPSA_CLASS(sincos)  ) ; }
    template<typename P, typename base_t>                                                                             \
    inline void sincosh_(const Base<P, base_t>& t, Base<P, base_t>* r1, Base<P, base_t>* r2) { process1to2_(t, r1, r2, GTPSA_CLASS(sincosh) ) ; }


    template<typename P, typename base_t>                                                                             \
    inline std::ostream& operator<<(std::ostream& strm, const Base<P, base_t>& a)
    {
	a.show(strm, 0);
	return strm;
    }



} // namespace gtsa

#endif /* _GTPSA_COMMON_H_ */
