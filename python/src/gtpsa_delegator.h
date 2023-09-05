#ifndef _GTPSA_PYTHON_GTPSA_DELEGATOR_H_
#define _GTPSA_PYTHON_GTPSA_DELEGATOR_H_ 1

#include <gtpsa/python/objects_with_named_index.h>

namespace gtpsa::python {
    /**
     * @brief: shared ptr capsule for ss_vect.support for  .loc and .iloc
     */
    template<typename T>
    class ss_vect_list_access {
    protected:
	using capsule_ptr = std::shared_ptr<gtpsa::python::StateSpaceWithNamedIndex<T>>;
    private:
	    capsule_ptr p_ss_vect;

    public:
        ss_vect_list_access(capsule_ptr p_inst)
	    : p_ss_vect(p_inst)
	    {}
	inline void checkVectorPtr() const {
	    if(!this->p_ss_vect){
		throw std::runtime_error("No shared pointer to ss_vect!");
	    }
	}
        inline capsule_ptr getVectorPtr(void) {
	    this->checkVectorPtr();
	    return p_ss_vect;
	}
        inline const capsule_ptr getVectorPtr(void) const {
	    this->checkVectorPtr();
	    return p_ss_vect;
	}
    };

    /**
     * @brief allows accessing a tpsa object which is part of the state space vector
     *
     * handled as part of gtpsa as it needs to export the same functionality as a
     * gtpsa object
     */
    template<class T, typename = typename T::tpsa_type, typename = typename T::base_type>
    class ss_vect_element_access : public ss_vect_list_access<typename T::tpsa_type> {
        using base_type = typename T::base_type;
        using tpsa_type = typename T::tpsa_type;
        using capsule_ptr = typename ss_vect_list_access<tpsa_type>::capsule_ptr;
	size_t m_index; ///< the index of the tpsa object

	/**
	 * @brief if a temporary reference is required in the C++ code
	 */
	inline tpsa_type& getTpsaObjectIntern(void) {
	    return this->getVectorPtr()->at(this->m_index);
	}

	inline const tpsa_type& getTpsaObjectIntern(void) const {
	    return this->getVectorPtr()->at(this->m_index);
	}

    public:

	/**
	 * @brief convert to tpsa object that is expected to be returned to Python
	 */
	inline auto getTpsaObject(void) {
	    return TpsaWithNamedIndex(this->getTpsaObjectIntern(), this->getVectorPtr()->getMapping());
	}

	/**
	 * convert to tpsa object with index
	 */
	inline const auto getTpsaObject(void) const {
	    return TpsaWithNamedIndex(this->getTpsaObjectIntern(), this->getVectorPtr()->getMapping());
	}

	ss_vect_element_access(capsule_ptr p_inst, size_t index)
	    : ss_vect_list_access<tpsa_type>(p_inst)
	    , m_index(index)
	    {
		size_t n = this->getVectorPtr()->size();
		if (index < 0){
		    throw std::out_of_range("index < 0");
		}
		if (index >= n){
		    throw std::out_of_range("index >= n");
		}
	    }

	/**
	 * @warning only use if you know what you are doing
	 * @todo need to find a better interface
	 */
	inline const tpsa_type& getTpsaObjectAsReference(void) const {
	    return this->getTpsaObjectIntern();
	}
	inline auto getMapping(void) const {
	    return this->getVectorPtr()->getMapping();
	}

	inline void setMapping(std::shared_ptr<gtpsa::python::IndexMapping> mapping) const {
	    this->getVectorPtr()->setMapping(mapping);
	}

	auto uid             ( int32_t uid )  { return this->getTpsaObjectIntern().uid(uid)         ; }
	auto setName         ( std::string s) { return this->getTpsaObjectIntern().setName(s)       ; }
	auto clear           ( void )         { return this->getTpsaObjectIntern().clear()          ; }
	auto name            ( void ) const   { return this->getTpsaObjectIntern().name()           ; }
	auto order           ( void ) const   { return this->getTpsaObjectIntern().order ()         ; }
	auto length          ( void ) const   { return this->getTpsaObjectIntern().length()         ; }
	auto getDescription  ( void ) const   { return this->getTpsaObjectIntern().getDescription() ; }
	auto isNull          ( void ) const   { return this->getTpsaObjectIntern().isNull()         ; }
	auto pstr            ( void ) const   { return this->getTpsaObjectIntern().pstr()           ; }
	auto repr            ( void ) const   { return this->getTpsaObjectIntern().repr()           ; }
	auto getCoefficients ( void ) const   { return this->getTpsaObjectIntern().getCoefficients(); }


	auto newFromThis     ( void ) const   { return this->getTpsaObject().newFromThis()          ; }
	auto clone           ( void ) const   { return this->getTpsaObject().clone      ()          ; }
	auto cst             ( void ) const   { return this->getTpsaObject().cst()                  ; }

        auto get ( void )                        const { return this->getTpsaObjectIntern().get()   ; }
	auto get ( const std::vector<ord_t>& m ) const { return this->getTpsaObjectIntern().get(m)  ; }

        void set ( const base_type a, const base_type b) {
	    tpsa_type& tmp = this->getTpsaObjectIntern();
	    tmp.set(a, b) ;
	    std::cerr << "Set a " <<  a << " b " << b
		      << " to " << tmp
		      << " to object " << this->getTpsaObjectIntern()
		      << std::endl;
	}
	void set ( const std::vector<ord_t>& m, const base_type a, const base_type b) {
	    this->getTpsaObjectIntern().set(m, a, b)   ;
	}

	inline void getv(idx_t i,       std::vector<typename T::base_type> *v) const { this->getTpsaObjectIntern().getv(i, v); }
	inline void setv(idx_t i, const std::vector<typename T::base_type> &v)       { this->getTpsaObjectIntern().setv(i, v); }

	inline auto getsm(const std::vector<int>& m)   const { return this->getTpsaObjectIntern().getsm(m);         }

	void setVariable(const typename T::base_type v, const idx_t iv= 0, const typename T::base_type scale = 0) {
	    this->getTpsaObjectIntern().setVariable(v, iv, scale);
	}
	inline auto index(std::string s)               const { return this->getTpsaObjectIntern().index(s)        ; }
	inline auto index(const std::vector<ord_t>& m) const { return this->getTpsaObjectIntern().index(m)        ; }

        auto to_tpsa(void) const {return this->getTpsaObject().clone(); }

	inline void print(str_t name = nullptr, num_t eps = 0, int nohdr = 0, FILE *stream = nullptr) const {
	    this->getTpsaObjectIntern().print(name, eps, nohdr, stream);
	}

	// need to play a trick here
	// would be my favourite implementation
	// auto& operator -  (void) const { auto n = this->clone(); return operator- (this->getTpsaObject()); }
        // auto& operator -  (void) const { return operator- (this->getTpsaObject()); }

        auto& operator += (const base_type  o ) { this->getTpsaObjectIntern() += o; return *this; }
        auto& operator -= (const base_type  o ) { this->getTpsaObjectIntern() -= o; return *this; }
        auto& operator *= (const base_type  o ) { this->getTpsaObjectIntern() *= o; return *this; }
        auto& operator /= (const base_type  o ) { this->getTpsaObjectIntern() /= o; return *this; }

        auto& operator += (const tpsa_type& o ) { this->getTpsaObjectIntern() += o;  return *this; }
        auto& operator -= (const tpsa_type& o ) { this->getTpsaObjectIntern() -= o;  return *this; }
        auto& operator *= (const tpsa_type& o ) { this->getTpsaObjectIntern() *= o;  return *this; }
        auto& operator /= (const tpsa_type& o ) { this->getTpsaObjectIntern() /= o;  return *this; }

	auto& operator += (const ss_vect_element_access& o ) { this->getTpsaObjectIntern() += o.getTpsaObjectIntern(); return *this; }
        auto& operator -= (const ss_vect_element_access& o ) { this->getTpsaObjectIntern() -= o.getTpsaObjectIntern(); return *this; }
        auto& operator *= (const ss_vect_element_access& o ) { this->getTpsaObjectIntern() *= o.getTpsaObjectIntern(); return *this; }
        auto& operator /= (const ss_vect_element_access& o ) { this->getTpsaObjectIntern() /= o.getTpsaObjectIntern(); return *this; }

	auto  operator +  (const base_type  o ) const { return this->getTpsaObject() + (o); }
        auto  operator -  (const base_type  o ) const { return this->getTpsaObject() -  o; }
        auto  operator *  (const base_type  o ) const { return this->getTpsaObject() *  o; }
        auto  operator /  (const base_type  o ) const { return this->getTpsaObject() /  o; }

	auto  operator +  (const tpsa_type  o ) const { return this->getTpsaObject().operator+(o); }
        auto  operator -  (const tpsa_type  o ) const { return this->getTpsaObject().operator-(o); }
        auto  operator *  (const tpsa_type  o ) const { return this->getTpsaObject().operator*(o); }
        auto  operator /  (const tpsa_type  o ) const { return this->getTpsaObject().operator/(o); }

	auto  radd  (const base_type o ) const { return  o  + this->getTpsaObject(); }
        auto  rsub  (const base_type o ) const { return  o  - this->getTpsaObject(); }
        auto  rmul  (const base_type o ) const { return  o  * this->getTpsaObject(); }
        auto  rdiv  (const base_type o ) const { return  o  / this->getTpsaObject(); }

	auto  radd  (const tpsa_type o ) const { return  o  + this->getTpsaObject(); }
        auto  rsub  (const tpsa_type o ) const { return  o  - this->getTpsaObject(); }
        auto  rmul  (const tpsa_type o ) const { return  o  * this->getTpsaObject(); }
        auto  rdiv  (const tpsa_type o ) const { return  o  / this->getTpsaObject(); }

	tpsa_type pow(const int n) const { return gtpsa::pow(this->getTpsaObject(), n); }

	/* O----------------------------------------------------------------O
	   | methods supporting functions that require access to the        |
	   | wrapped object                                                 |
	*/
	inline void apply_with_return_object(gtpsa::tpsa * o,
					     void(f)(const tpsa_type& a, tpsa_type* r)) const {
	    f(this->getTpsaObjectIntern(), o);
	}
	inline void rapply_object(const tpsa_type& o,
				  void(f)(const tpsa_type& a, tpsa_type* r)) {
	    f(o, &this->getTpsaObjectIntern());
	}
	inline void rapply_ss_vect_access(const ss_vect_element_access<T>& o,
					  void(f)(const tpsa_type& a, tpsa_type* r)) {
	    f(o.getTpsaObjectIntern(), &this->getTpsaObjectIntern());
	}

	inline tpsa_type apply(tpsa_type (f)(const tpsa_type& a)) const {
	    return f(this->getTpsaObject());
	}

	inline auto apply_with_int(const int iv, tpsa_type (f)(const tpsa_type& a, const int iv)) const {
	       return TpsaWithNamedIndex(this->getTpsaObjectIntern(), this->getMapping());
	}
    };

    typedef ss_vect_element_access<gtpsa::TpsaTypeInfo> ss_vect_tpsa_elem_access_t;

    // overloading functions so that templated wrappers can be reused
    template<class C>
    inline auto deriv(const gtpsa::python::ss_vect_element_access<C>&o, int iv){ return o.apply_with_int(iv, gtpsa::deriv); }

    // operators for right side
    template<class C>
    auto operator+  (const typename C::tpsa_type& a,  const gtpsa::python::ss_vect_element_access<C>& b)  { return b.radd(a); }
    template<class C>
    auto operator-  (const typename C::tpsa_type& a,  const gtpsa::python::ss_vect_element_access<C>& b)  { return b.rsub(a); }
    template<class C>
    auto operator*  (const typename C::tpsa_type& a,  const gtpsa::python::ss_vect_element_access<C>& b)  { return b.rmul(a); }
    template<class C>
    auto operator/  (const typename C::tpsa_type& a,  const gtpsa::python::ss_vect_element_access<C>& b)  { return b.rdiv(a); }

    #if 0
    template<class C>
    auto  operator +  (const ss_vect_element_access<C>& a, const gtpsa::tpsa& b ) { return a.getTpsaObject() +  b; }
    template<class C>
    auto  operator -  (const ss_vect_element_access<C>& a, const gtpsa::tpsa& b ) { return a.getTpsaObject() -  b; }
    template<class C>
    auto  operator *  (const ss_vect_element_access<C>& a, const gtpsa::tpsa& b ) { return a.getTpsaObject() *  b; }
    template<class C>
    auto  operator /  (const ss_vect_element_access<C>& a, const gtpsa::tpsa& b ) { return a.getTpsaObject() /  b; }
    #endif

    template<class C>
    auto  operator +  (const ss_vect_element_access<C>& a, const ss_vect_element_access<C>& b ) { return a.getTpsaObject() + b.getTpsaObject(); }
    template<class C>
    auto  operator -  (const ss_vect_element_access<C>& a, const ss_vect_element_access<C>& b ) { return a.getTpsaObject() - b.getTpsaObject(); }
    template<class C>
    auto  operator *  (const ss_vect_element_access<C>& a, const ss_vect_element_access<C>& b ) { return a.getTpsaObject() * b.getTpsaObject(); }
    template<class C>
    auto  operator /  (const ss_vect_element_access<C>& a, const ss_vect_element_access<C>& b ) { return a.getTpsaObject() / b.getTpsaObject(); }


    template<class C>
    auto operator+ (const typename C::base_type a, const ss_vect_element_access<C>& b) {return b.radd(a); }
    template<class C>
    auto operator- (const typename C::base_type a, const ss_vect_element_access<C>& b) {return b.rsub(a); }
    template<class C>
    auto operator* (const typename C::base_type a, const ss_vect_element_access<C>& b) {return b.rmul(a); }
    template<class C>
    auto operator/ (const typename C::base_type a, const ss_vect_element_access<C>& b) {return b.rdiv(a); }

    template<class C>
    typename C::tpsa_type pow(ss_vect_element_access<C> &inst, const int n) { return inst.pow(n); }


    // auto deriv =  gtpsa::deriv;
} // namespace gtpsa::python


#endif /* _GTPSA_PYTHON_GTPSA_DELEGATOR_H_ */
