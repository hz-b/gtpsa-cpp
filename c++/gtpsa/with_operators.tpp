/* -*- c++ -*- */

namespace gtpsa {
    // forward declaration of class ...

    class GTPSA_CLASS(WithOp);
    // forward declaration ... required that operators will use the same functions
    inline void  add (const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)& b, GTPSA_CLASS(WithOp)* r);
    inline void  dif (const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)& b, GTPSA_CLASS(WithOp)* r);
    inline void  sub (const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)& b, GTPSA_CLASS(WithOp)* r);
    inline void  mul (const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)& b, GTPSA_CLASS(WithOp)* r);
    inline void  div (const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)& b, GTPSA_CLASS(WithOp)* r);
    inline void  pow (const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)& b, GTPSA_CLASS(WithOp)* r);

    inline GTPSA_CLASS(WithOp)  add (const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)& b         );
    inline GTPSA_CLASS(WithOp)  dif (const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)& b         );
    inline GTPSA_CLASS(WithOp)  sub (const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)& b         );
    inline GTPSA_CLASS(WithOp)  mul (const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)& b         );
    inline GTPSA_CLASS(WithOp)  div (const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)& b         );

    inline void  add_d (const GTPSA_CLASS(WithOp)& a, const GTPSA_BASE_T b, GTPSA_CLASS(WithOp)* r);
    inline void  sub_d (const GTPSA_CLASS(WithOp)& a, const GTPSA_BASE_T b, GTPSA_CLASS(WithOp)* r);
    inline void  mul_d (const GTPSA_CLASS(WithOp)& a, const GTPSA_BASE_T b, GTPSA_CLASS(WithOp)* r);
    inline void  div_d (const GTPSA_CLASS(WithOp)& a, const GTPSA_BASE_T b, GTPSA_CLASS(WithOp)* r);
    inline void  pow_d (const GTPSA_CLASS(WithOp)& a, const GTPSA_BASE_T b, GTPSA_CLASS(WithOp)* r);

    inline GTPSA_CLASS(WithOp)  add_d (const GTPSA_CLASS(WithOp)& a, const GTPSA_BASE_T b          );
    inline GTPSA_CLASS(WithOp)  dif_d (const GTPSA_CLASS(WithOp)& a, const GTPSA_BASE_T b          );
    inline GTPSA_CLASS(WithOp)  sub_d (const GTPSA_CLASS(WithOp)& a, const GTPSA_BASE_T b          );
    inline GTPSA_CLASS(WithOp)  mul_d (const GTPSA_CLASS(WithOp)& a, const GTPSA_BASE_T b          );
    inline GTPSA_CLASS(WithOp)  div_d (const GTPSA_CLASS(WithOp)& a, const GTPSA_BASE_T b          );

    inline GTPSA_CLASS(WithOp)  scl   (const GTPSA_CLASS(WithOp)& a, const GTPSA_BASE_T v          );


    inline void process1_   (const GTPSA_CLASS(WithOp)& a, GTPSA_CLASS(WithOp) *r,                 void (*func)(const GTPSA_PTR_T* a,GTPSA_PTR_T *r) );

    inline GTPSA_CLASS(WithOp) process2(const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)&   b,
					void (*func)(const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)&   b, GTPSA_CLASS(WithOp) *r) );


    /*
     * bridge class
     */
    class GTPSA_CLASS(WithOp) : protected GTPSA_CLASS(Wrapper)
    {

    public:
	inline GTPSA_CLASS(WithOp)(std::shared_ptr<desc> desc, const ord_t mo)
	    : GTPSA_CLASS(Wrapper) (desc, mo)
	{ }

	inline GTPSA_CLASS(WithOp)(const GTPSA_CLASS(Wrapper)&              t,  const ord_t mo)
	    :  GTPSA_CLASS(Wrapper) (t, mo)
	{ }
	inline GTPSA_CLASS(WithOp)(const GTPSA_CLASS(WithOp)&              t,  const ord_t mo)
	    :  GTPSA_CLASS(Wrapper) (t, mo)
	{ }

#ifndef GTSPA_ONLY_OPTIMISED_OPS
	inline GTPSA_CLASS(WithOp)(const GTPSA_CLASS(WithOp)&              o)
	    : GTPSA_CLASS(Wrapper)(o)
	    { }
#else /* GTSPA_ONLY_OPTIMISED_OPS */
	inline GTPSA_CLASS(WithOp)(const GTPSA_CLASS(WithOp)&              o) = delete;
#endif /* GTSPA_ONLY_OPTIMISED_OPS */

	/**
	 * move ctor
	 */
	inline GTPSA_CLASS(WithOp)(GTPSA_CLASS(WithOp)&& o)                   = default; //  noexcept :  ltm(std::move(o.tm)), t_desc(std::move(o.t_desc)) { };
	inline GTPSA_CLASS(WithOp)& operator= (GTPSA_CLASS(WithOp) && o)      = default;



	inline GTPSA_CLASS(WithOp) clone(void)  const { GTPSA_CLASS(WithOp) res(*this, mad_tpsa_same); res._copyInPlace(*this); return res; }

	/* O----------------------------------------------------------------O
	   | methods required for the private bridge                        |
	   |                                                                |
	   | @todo review if the vector to buffer conversion should not be  |
	   |       part of the functions on this side                       |
	   |                                                                |
	*/
	/*
	 * @brief create a new instance with similar order / desc property as this
	 *
	 * required as the constructor is now only be used within this class or
	 * base class. Avoids that many functions like process and so on need to
	 * be declared as friend classes
	 *
	 * not needed as this class adds functionality to the base class. Still kept as
	 * it seems to make further code easier to understand....
	 */
	inline GTPSA_CLASS(WithOp) newFromThis(void) const {
	    return GTPSA_CLASS(WithOp)(*this, gtpsa::init::same);
	}

	inline auto getDescription(void) const { return GTPSA_CLASS(Wrapper)::getDescription(); }

	inline auto uid(int32_t uid)               { return GTPSA_CLASS(Wrapper)::uid(uid); }
	inline auto length()                 const { return this->len();     }
	inline auto order()                  const { return this->ord();     }
	inline auto name()                   const { return this->nam();     }
	inline auto setName(std::string s)         {        this->setnam(s); }

	inline void clear()  { GTPSA_CLASS(Wrapper)::clear(); }
	inline void isNull() { GTPSA_CLASS(Wrapper)::isNull(); }

	/**
	 * @brief indexing / monomials (return idx_t = -1 if invalid)
	 */
	inline auto mono(std::vector<ord_t>& m, idx_t i)        { return GTPSA_CLASS(Wrapper)::mono(m, i); }

	/**
	 *  @brief string mono "[0-9]*"
	 */
	inline auto idx(std::string s)                    const { return this->idxs(s);}
	inline auto idx(const std::vector<ord_t>& m)      const { return this->idxm(m);}

	/**
	 * @brief sparse mono [(i,o)]
	 * @note not using overload ... could be misleading
	 */
	inline auto idxsm(const std::vector<int> m)             { return  GTPSA_CLASS(Wrapper)::idxsm(m); }

	inline auto get(void)                        const { return this->get0(); }
	inline auto get(const idx_t i)               const { return this->geti(i); }
	inline auto get(std::string s)               const { return this->gets(s); }
	inline auto get(const std::vector<ord_t>& m) const { return this->getm(m); }
	/**
	 * @brief sparse mono "[(i, o)]*"
	 */
	inline auto getsm(const std::vector<int>& m) const { return GTPSA_CLASS(Wrapper)::getsm(m); }

	/*
	 * for pybind 11
	*/
	inline void _set0(                             GTPSA_BASE_T a, GTPSA_BASE_T b) { this->set(a, b);}
	inline void _setm(const std::vector<ord_t>& m, GTPSA_BASE_T a, GTPSA_BASE_T b) { this->set(m, a, b);}

	/**
	 * @brief a*x[0]+b
	 */
	inline void set(                              GTPSA_BASE_T a, GTPSA_BASE_T b) { this->set0(   a, b ); }

	/**
	 * @brief a*x[i]+b
	 */
	inline void set(const idx_t i               , GTPSA_BASE_T a, GTPSA_BASE_T b) { this->seti(i, a, b ); }
	/**
	 * @brief a*x[m]+b
	 */
	inline void set(const std::string& s        , GTPSA_BASE_T a, GTPSA_BASE_T b) { this->sets(s, a, b ); }
	/**
	 * @brief a*x[m]+b
	 */
	inline void set(const std::vector<ord_t>& m , GTPSA_BASE_T a, GTPSA_BASE_T b) { this->setm(m, a, b ); }

	/**
	 * @brief a*x[m]+b, sparse mono [(i,o)]
	 * @todo vector of pairs?
	 */
	inline auto setsm(const std::vector<int> m, GTPSA_BASE_T a, GTPSA_BASE_T b) { GTPSA_CLASS(Wrapper)::setsm(m, a, b ); }

	inline void getv(idx_t i,       std::vector<GTPSA_BASE_T> *v) const { GTPSA_CLASS(Wrapper)::getv(i, v); }
	inline void setv(idx_t i, const std::vector<GTPSA_BASE_T> &v)       { GTPSA_CLASS(Wrapper)::setv(i, v); }

	inline void setVariable(GTPSA_BASE_T v, idx_t iv= 0, GTPSA_BASE_T scale = 0) {	    this->setvar(v, iv, scale);	}

	/**
	 *
	 * @todo interface needs to be reviewed
	 */
	inline void print(str_t name = nullptr, num_t eps = 0, int nohdr = 0, FILE *stream = nullptr){
	    GTPSA_CLASS(Wrapper)::print(name, eps, nohdr, stream);
	}
	/* |                                                                |
	   O----------------------------------------------------------------O */

	inline auto cst(void)                        const { return this->get(); }

	inline GTPSA_CLASS(WithOp)& operator += (const GTPSA_CLASS(WithOp)& o  ){ add(*this, o, this); return *this; }
	// (a_i-b_i)/max(|a_i|,1)
	inline GTPSA_CLASS(WithOp)& operator -= (const GTPSA_CLASS(WithOp)& o  ){ sub(*this, o, this); return *this; }
	inline GTPSA_CLASS(WithOp)& operator *= (const GTPSA_CLASS(WithOp)& o  ){ mul(*this, o, this); return *this; }
	inline GTPSA_CLASS(WithOp)& operator /= (const GTPSA_CLASS(WithOp)& o  ){ div(*this, o, this); return *this; }

	inline GTPSA_CLASS(WithOp)& operator += (const GTPSA_BASE_T& o ){ this->set(   1e0,    o); return *this; }
	inline GTPSA_CLASS(WithOp)& operator -= (const GTPSA_BASE_T& o ){ this->set(   1e0,   -o); return *this; }
	inline GTPSA_CLASS(WithOp)& operator *= (const GTPSA_BASE_T& o ){ this->set(     o,  0e0); return *this; }
	inline GTPSA_CLASS(WithOp)& operator /= (const GTPSA_BASE_T& o ){ this->set( 1e0/o,  0e0); return *this; }


#ifndef GTSPA_ONLY_OPTIMISED_OPS
	/**
	 * These operators need to make copys of the actual instance
	 * In the current implementation I assum the compiler can not fully optimise it away
	 * if not required
	 */
	inline GTPSA_CLASS(WithOp)&  operator =  (const GTPSA_CLASS(WithOp)& o )       { this->_copyInPlace(o); return *this; }
	/**
	 * brief: assign a c
	 *
	 * @todo check if not the whold object should be zeroed ...
	 *       same as in Johan's tps
	 */
	inline GTPSA_CLASS(WithOp)&  operator =  (const GTPSA_BASE_T& o )              { this->set(0, o);       return *this; }
	/**
	 * @todo do I need to make a copy ....?
	 */
	// negation
	inline GTPSA_CLASS(WithOp)   operator -  (void) const { return scl(*this, -1); }

	//process2
	inline GTPSA_CLASS(WithOp)  operator +  (const GTPSA_CLASS(WithOp)& o ) const { return process2(*this, o, add); }
	inline GTPSA_CLASS(WithOp)  operator -  (const GTPSA_CLASS(WithOp)& o ) const { return process2(*this, o, sub); }
	inline GTPSA_CLASS(WithOp)  operator *  (const GTPSA_CLASS(WithOp)& o ) const { return process2(*this, o, mul); }
	inline GTPSA_CLASS(WithOp)  operator /  (const GTPSA_CLASS(WithOp)& o ) const { return process2(*this, o, div); }

	/*
	  inline GTPSA_CLASS(WithOp)  operator +  (const GTPSA_BASE_T o) const { return process2(*this, o, add_d); }
	  inline GTPSA_CLASS(WithOp)  operator -  (const GTPSA_BASE_T o) const { return process2(*this, o, sub_d); }
	  inline GTPSA_CLASS(WithOp)  operator *  (const GTPSA_BASE_T o) const { return process2(*this, o, mul_d); }
	  inline GTPSA_CLASS(WithOp)  operator /  (const GTPSA_BASE_T o) const { return process2(*this, o, div_d); }
	*/

	inline GTPSA_CLASS(WithOp)  operator +  (const GTPSA_BASE_T o) const { auto r = this->newFromThis(); r.set(0, o    ); r += *this;     return r; }
	inline GTPSA_CLASS(WithOp)  operator -  (const GTPSA_BASE_T o) const { auto r = this->newFromThis(); r.set(0, o    ); return *this - r;         }
	inline GTPSA_CLASS(WithOp)  operator *  (const GTPSA_BASE_T o) const { auto r = this->newFromThis(); r.set(0, o    ); r *= *this;     return r; }
	inline GTPSA_CLASS(WithOp)  operator /  (const GTPSA_BASE_T o) const { auto r = this->newFromThis(); r.set(0, o    ); return *this / r;         }


#endif
	// forward declaration ... needs to be implemented in the derived class
	void show(std::ostream& strm, int level) const;

	/**
	 * @brief support python representation of this object
	 */
	std::string repr(void) const;
	/**
	 * @brief support python __str__ of this object
	 */
	std::string pstr(void) const;

    private:

	friend inline void process1_ (const GTPSA_CLASS(WithOp)& a, GTPSA_CLASS(WithOp) *r,                 void (*func)(const GTPSA_PTR_T* a, GTPSA_PTR_T *r) );

	friend inline void process2_ (const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)& b, GTPSA_CLASS(WithOp) *r,  void (*func)(const GTPSA_PTR_T* a, const GTPSA_PTR_T* b, GTPSA_PTR_T *r) );
	friend inline void process2_ (const GTPSA_CLASS(WithOp)& a, const GTPSA_BASE_T b, GTPSA_CLASS(WithOp) *r, void (*func)(const GTPSA_BASE_T a, const GTPSA_BASE_T b, GTPSA_BASE_T *r) );
	friend inline void process2_ (const GTPSA_CLASS(WithOp)& a, const GTPSA_BASE_T b, GTPSA_CLASS(WithOp) *r, void (*func)(const GTPSA_PTR_T* a, const GTPSA_BASE_T b, GTPSA_PTR_T *r) );
	friend inline void process1to2_(const GTPSA_CLASS(WithOp)& a, GTPSA_CLASS(WithOp)* r1, GTPSA_CLASS(WithOp) *r2,      void (*func)(const GTPSA_PTR_T* a, GTPSA_PTR_T *r1,  GTPSA_PTR_T *r2) ) ;


	/*
	 * @todo review is this should not be part of the wrapper class
	 */
	friend inline void taylor_(const GTPSA_CLASS(WithOp)& a, std::vector<GTPSA_BASE_T> coeff, GTPSA_CLASS(WithOp)* c);

	/*
	 * @todo review is this should not be part of the wrapper class
	 */
	friend inline void pow (const GTPSA_CLASS(WithOp)& a, int n, GTPSA_CLASS(WithOp)* r );

	/*
	 * @todo review is this should not be part of the wrapper class
	 */
	friend inline void pow (const GTPSA_CLASS(WithOp)& a, GTPSA_BASE_T   n, GTPSA_CLASS(WithOp)* r );

	// as the base is private, these functions now need to be fried functions too ...
	// friend inline GTPSA_CLASS(WithOp) process1(const GTPSA_CLASS(WithOp)& t, void (*func)(const GTPSA_CLASS(WithOp)& t, GTPSA_CLASS(WithOp) *r));
	// friend inline GTPSA_CLASS(WithOp) process2(const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)&  b,
	// 					   void (*func)(const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)&  b, GTPSA_CLASS(WithOp) *r) );

    }; /* class GTPSA_CLASS(WithOp) */


    inline std::ostream& operator<<(std::ostream& strm, const GTPSA_CLASS(WithOp)& a)
    {
	a.show(strm, 0);
	return strm;
    }


#ifndef GTSPA_ONLY_OPTIMISED_OPS
    inline GTPSA_CLASS(WithOp)  operator +  (const GTPSA_BASE_T a, const GTPSA_CLASS(WithOp)& b)  { return b + a; }
    inline GTPSA_CLASS(WithOp)  operator -  (const GTPSA_BASE_T a, const GTPSA_CLASS(WithOp)& b)  { auto t = b.newFromThis(); t.set(0, a    ); return t - b; }
    inline GTPSA_CLASS(WithOp)  operator *  (const GTPSA_BASE_T a, const GTPSA_CLASS(WithOp)& b)  { return b * a; }
    inline GTPSA_CLASS(WithOp)  operator /  (const GTPSA_BASE_T a, const GTPSA_CLASS(WithOp)& b)  { auto t = b.newFromThis(); t.set(0, a    ); return t / b; }
#endif

} //namespace gtpsa

// EOF
