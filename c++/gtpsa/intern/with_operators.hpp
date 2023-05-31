/* -*- c++ -*- */
#ifndef _GTPSA_TPSA_WITH_OPERATORS_H_
#define _GTPSA_TPSA_WITH_OPERATORS_H_ 1
#include <gtpsa/bridge/bridge.hpp>
namespace gtpsa {

    /**
     * @brief extend bridge definition with the associated opp
     */
    //template<typename T, typename = typename T::base_type, typename = typename T::bridge>
    //class TpsaWithOp : public T::bridge
    template<class T, typename = typename T::bridge_type, typename = typename T::base_type, typename = typename T::ptr_type>
    class TpsaWithOp : public TpsaBridge<T>
    {
    public:
	using bridge = TpsaBridge<T>;
    using base_type = typename T::base_type;

    public:
	inline TpsaWithOp(std::shared_ptr<mad::desc> desc, const ord_t mo)
	    : bridge(desc, mo)
	    {}

	inline TpsaWithOp(const TpsaWithOp&              t, const ord_t mo)
	    : bridge(t,   mo)
	    {}

	inline TpsaWithOp(      TpsaWithOp&&       o) = default;

#ifndef GTSPA_ONLY_OPTIMISED_OPS

	inline TpsaWithOp(const bridge& o)
	    : bridge(o)
	    {}

	inline TpsaWithOp(const TpsaWithOp&              o) = default;

#else /* GTSPA_ONLY_OPTIMISED_OPS */

	inline TpsaWithOp(const TpsaWithOp&              o) = delete;

        // inline TpsaWithOP getOrder(int order) {return bridge::getOrder(); }
#endif

	inline TpsaWithOp newFromThis(void) const { return bridge::newFromThis(); }

        inline void rgetOrder(const TpsaWithOp& o, int order) { return bridge::rgetOrder(o, order); }

        inline auto toBridgePtr(void) const { return static_cast<bridge &>(*this); }


	/**
	 * These operators need to make copys of the actual instance
	 * In the current implementation I assume the compiler can not fully optimise it away
	 * if not required
	 *
	 * review if the copy constructor should be private and called from here ...
	 * seems that it is more efficient than the clone
	 */
	inline TpsaWithOp<T>&  operator =  (const TpsaWithOp<T>& o )       { this->_copyInPlace(o); return *this; }

	/**
	 * brief: assign a c
	 *
	 * @todo check if not the whole object should be zeroed ...
	 *       same as in Johan's tps
	 */
	inline TpsaWithOp<T>&  operator =  (const typename T::base_type& o ) { this->set(0, o);       return *this; }

	/*
	 * operators: use already defined functions
	 */
	inline TpsaWithOp& operator += (const TpsaWithOp& o ) { radd(*this, o, this); return *this; }
	// (a_i-b_i)/max(|a_i|,1)
	inline TpsaWithOp& operator -= (const TpsaWithOp& o ) { rsub(*this, o, this); return *this; }
	inline TpsaWithOp& operator *= (const TpsaWithOp& o ) { rmul(*this, o, this); return *this; }
	inline TpsaWithOp& operator /= (const TpsaWithOp& o ) { rdiv(*this, o, this); return *this; }

	/*
	 * operators with base type as one argument
	 */
	inline TpsaWithOp& operator += (const typename T::base_type& o ){ this->set(   1e0,    o); return *this; }
	inline TpsaWithOp& operator -= (const typename T::base_type& o ){ this->set(   1e0,   -o); return *this; }
	inline TpsaWithOp& operator *= (const typename T::base_type& o ){ this->set(     o,  0e0); return *this; }
	inline TpsaWithOp& operator /= (const typename T::base_type& o ){ this->set( 1e0/o,  0e0); return *this; }

#ifndef GTSPA_ONLY_OPTIMISED_OPS
	/**
	 * @brief negation
	 * @todo do I need to make a copy ....?
	 */
	inline TpsaWithOp   operator -  (void) const { return scl(*this, -1); }

	inline TpsaWithOp  operator +  (const TpsaWithOp<T>& o ) const { return TpsaWithOp( add(*this, o) ); }
	inline TpsaWithOp  operator -  (const TpsaWithOp<T>& o ) const { return TpsaWithOp( sub(*this, o) ); }
	inline TpsaWithOp  operator *  (const TpsaWithOp<T>& o ) const { return TpsaWithOp( mul(*this, o) ); }
	inline TpsaWithOp  operator /  (const TpsaWithOp<T>& o ) const { return TpsaWithOp( div(*this, o) ); }

	inline TpsaWithOp  operator +  (const typename T::base_type o) const { auto r = this->newFromThis(); r.set(0, o); r += *this;     return r; }
	inline TpsaWithOp  operator -  (const typename T::base_type o) const { auto r = this->newFromThis(); r.set(0, o); return *this - r;         }
	inline TpsaWithOp  operator *  (const typename T::base_type o) const { auto r = this->newFromThis(); r.set(0, o); r *= *this;     return r; }
	inline TpsaWithOp  operator /  (const typename T::base_type o) const { auto r = this->newFromThis(); r.set(0, o); return *this / r;         }
#endif // GTSPA_ONLY_OPTIMISED_OPS

	/*
	 * Some minimal support for input output
	 */
	// forward declaration ... needs to be implemented in the derived class
	void show(std::ostream& strm, int level) const
	{

	    strm << "gtpsa  cst:\n\t" << this->cst();
	    if(this->order()){
		// at least first order ...
		const auto& info = this->getDescription()->getInfo();
		const int nn = info.getNumberOfVariables() + info.getNumberOfParameters();
		std::vector<typename T::base_type> v(nn);
		this->getv(1, &v);

		strm  << "\ngtpsa linear :\n"
		      << std::scientific << std::setw(20);
		for(auto& e: v) strm <<  std::scientific << std::setw(20) << e << " ";
	    }
	    strm << "\n";
	}

	/**
	 *
	 * @todo: how to find the total number of coefficients?
	 */
	inline auto getCoefficients(void) const {
	    const auto n_max = this->getDescription()->getInfo().getTotalNumber();
	    std::vector< std::tuple<std::vector<ord_t>, base_type, idx_t> > coefficients;
	    int i = -1;
	    while(true){
		std::vector<ord_t> t_orders(n_max);
		const auto r = this->cycle(i, &t_orders);
		i = r.first;
		if(i == -1){
		    break;
		}
		coefficients.push_back(std::tie(t_orders, r.second, i));
	    };
	    return coefficients;
	}

	/**
	 * @brief support python representation of this object
	 */
	inline std::string repr(void) const {
	    std::stringstream strm;
	    this->show(strm, 10);
	    return strm.str();
	}
	/**
	 * @brief support python __str__ of this object
	 */
	std::string pstr(void) const {
	    std::stringstream strm;
	    this->show(strm, 0);
	    return strm.str();
	}
    /*
    void convolve() {
        mad::compose(ma, mb, mc);
    }
     */
    private:

    }; /* class TpsaWithOp */


    template<class T, typename, typename, typename>
    inline std::ostream& operator<<(std::ostream& strm, const TpsaWithOp<T>& a)
    {
	a.show(strm, 0);
	return strm;
    }


#ifndef GTSPA_ONLY_OPTIMISED_OPS
    template<class T, typename = typename T::base_type>
    inline TpsaWithOp<T>  operator +  (const typename T::base_type a, const TpsaWithOp<T>& b)  { return b + a; }
    template<class T, typename = typename T::base_type>
    inline TpsaWithOp<T>  operator -  (const typename T::base_type a, const TpsaWithOp<T>& b)  { auto t = b.newFromThis(); t.set(0, a    ); return t - b; }
    template<class T, typename = typename T::base_type>
    inline TpsaWithOp<T>  operator *  (const typename T::base_type a, const TpsaWithOp<T>& b)  { return b * a; }
    template<class T, typename = typename T::base_type>
    inline TpsaWithOp<T>  operator /  (const typename T::base_type a, const TpsaWithOp<T>& b)  { auto t = b.newFromThis(); t.set(0, a    ); return t / b; }
    template<class T, typename = typename T::base_type>
    inline TpsaWithOp<T>  deriv  (const TpsaWithOp<T>& o, const int iv)  { auto t = o.newFromThis(); t.rderiv(o, iv); return t; }
#endif

} //namespace gtpsa
#endif /* _GTPSA_TPSA_WITH_OPERATORS_H_ */
