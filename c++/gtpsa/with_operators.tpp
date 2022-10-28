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

    inline void  add_d (const GTPSA_CLASS(WithOp)& a, const T b, GTPSA_CLASS(WithOp)* r);
    inline void  sub_d (const GTPSA_CLASS(WithOp)& a, const T b, GTPSA_CLASS(WithOp)* r);
    inline void  mul_d (const GTPSA_CLASS(WithOp)& a, const T b, GTPSA_CLASS(WithOp)* r);
    inline void  div_d (const GTPSA_CLASS(WithOp)& a, const T b, GTPSA_CLASS(WithOp)* r);
    inline void  pow_d (const GTPSA_CLASS(WithOp)& a, const T b, GTPSA_CLASS(WithOp)* r);

    inline GTPSA_CLASS(WithOp)  add_d (const GTPSA_CLASS(WithOp)& a, const T b          );
    inline GTPSA_CLASS(WithOp)  dif_d (const GTPSA_CLASS(WithOp)& a, const T b          );
    inline GTPSA_CLASS(WithOp)  sub_d (const GTPSA_CLASS(WithOp)& a, const T b          );
    inline GTPSA_CLASS(WithOp)  mul_d (const GTPSA_CLASS(WithOp)& a, const T b          );
    inline GTPSA_CLASS(WithOp)  div_d (const GTPSA_CLASS(WithOp)& a, const T b          );

    inline GTPSA_CLASS(WithOp)  scl   (const GTPSA_CLASS(WithOp)& a, const T v          );


    inline void process1_   (const GTPSA_CLASS(WithOp)& a, GTPSA_CLASS(WithOp) *r,                 void (*func)(const P* a,P *r) );

    inline GTPSA_CLASS(WithOp) process2(const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)&   b,
					void (*func)(const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)&   b, GTPSA_CLASS(WithOp) *r) );


    class GTPSA_CLASS(WithOp) : public GTPSA_CLASS(Wrapper)
    {

    public:
	inline GTPSA_CLASS(WithOp)(std::shared_ptr<desc> desc, const ord_t mo)
	    :  GTPSA_CLASS(Wrapper) (desc, mo)
	{}

	inline GTPSA_CLASS(WithOp)(const GTPSA_CLASS(Wrapper)&              t,  const ord_t mo)
	    :  GTPSA_CLASS(Wrapper) (t, mo)
	    { }

#ifndef GTSPA_ONLY_OPTIMISED_OPS
	inline GTPSA_CLASS(WithOp)(const GTPSA_CLASS(WithOp)&              o)
	    : GTPSA_CLASS(Wrapper)(o)
	    { };
#else /* GTSPA_ONLY_OPTIMISED_OPS */
	inline GTPSA_CLASS(WithOp)(const GTPSA_CLASS(WithOp)&              o) = delete;
#endif /* GTSPA_ONLY_OPTIMISED_OPS */

	/**
	 * move ctor
	 */
	inline GTPSA_CLASS(WithOp)(GTPSA_CLASS(WithOp)&& o)                   = default; //  noexcept :  ltm(std::move(o.tm)), t_desc(std::move(o.t_desc)) { };
	inline GTPSA_CLASS(WithOp)& operator= (GTPSA_CLASS(WithOp) && o)      = default;



	inline GTPSA_CLASS(WithOp) clone(void)  const { GTPSA_CLASS(WithOp) res(*this, mad_tpsa_same); res._copyInPlace(*this); return res; }

	inline auto cst(void)                        const { return this->get(); }

	inline GTPSA_CLASS(WithOp)& operator += (const GTPSA_CLASS(WithOp)& o  ){ add(*this, o, this); return *this; }
	// (a_i-b_i)/max(|a_i|,1)
	inline GTPSA_CLASS(WithOp)& operator -= (const GTPSA_CLASS(WithOp)& o  ){ sub(*this, o, this); return *this; }
	inline GTPSA_CLASS(WithOp)& operator *= (const GTPSA_CLASS(WithOp)& o  ){ mul(*this, o, this); return *this; }
	inline GTPSA_CLASS(WithOp)& operator /= (const GTPSA_CLASS(WithOp)& o  ){ div(*this, o, this); return *this; }


#ifndef GTSPA_ONLY_OPTIMISED_OPS
	/**
	 * These operators need to make copys of the actual instance
	 * In the current implementation I assum the compiler can not fully optimise it away
	 * if not required
	 */
	inline GTPSA_CLASS(WithOp)&  operator =  (const GTPSA_CLASS(WithOp)& o )       { this->_copyInPlace(o); return *this; }
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


	// inline GTPSA_CLASS(WithOp)  operator +  (const T o) const { return process2(*this, o, add_d; }
	// inline GTPSA_CLASS(WithOp)  operator -  (const T o) const { return process2(*this, o, sub_d); }
	// inline GTPSA_CLASS(WithOp)  operator *  (const T o) const { return process2(*this, o, mul_d); }
	// inline GTPSA_CLASS(WithOp)  operator /  (const T o) const { return process2(*this, o, div_d); }

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

	friend inline GTPSA_CLASS(WithOp) process2(const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)& b,
						void (*func)(const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)& b, GTPSA_CLASS(WithOp) *r) );

	friend inline void process1_ (const GTPSA_CLASS(WithOp)& a, GTPSA_CLASS(WithOp) *r,                 void (*func)(const P* a, P *r) );

	friend inline void process2_ (const GTPSA_CLASS(WithOp)& a, const GTPSA_CLASS(WithOp)& b, GTPSA_CLASS(WithOp) *r,  void (*func)(const P* a, const P* b, P *r) );
	friend inline void process2_ (const GTPSA_CLASS(WithOp)& a, const T b, GTPSA_CLASS(WithOp) *r, void (*func)(const T  a, const T b, T *r) );
	friend inline void process2_ (const GTPSA_CLASS(WithOp)& a, const T b, GTPSA_CLASS(WithOp) *r, void (*func)(const P* a, const T b, P *r) );
	friend inline void process1to2_(const GTPSA_CLASS(WithOp)& a, GTPSA_CLASS(WithOp)* r1, GTPSA_CLASS(WithOp) *r2,      void (*func)(const P* a, P *r1,  P *r2) ) ;

	friend inline void pow (const GTPSA_CLASS(WithOp)& a, int n, GTPSA_CLASS(WithOp)* r );
	friend inline void pow (const GTPSA_CLASS(WithOp)& a, T   n, GTPSA_CLASS(WithOp)* r );

	friend inline void taylor_(const GTPSA_CLASS(WithOp)& a, std::vector<T> coeff, GTPSA_CLASS(WithOp)* c);
    }; /* class GTPSA_CLASS(WithOp) */


    inline std::ostream& operator<<(std::ostream& strm, const GTPSA_CLASS(WithOp)& a)
    {
	a.show(strm, 0);
	return strm;
    }


} //namespace gtpsa

// EOF
