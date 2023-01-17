/* -*- C++ -*- */
namespace gtpsa {
    template<typename B, typename T, typename V>
    struct TpsaVariantTypes {
	// base type: e.g. double
	using base_type =  B;
	// tpsa type: tpsa for base type double
	using tpsa_type = T;
	// variant type tpsa_or_double_t for base of double
	using variant_type = V;
    };


/**
 * @brief a class that either contains a double or a tpsa object
 *
 * motivated by engineering studies
 *
 * @todo: consider adding power function, further functions required ?
 */
    template<class C, typename = typename C::base_type, typename = typename C::tpsa_type,  typename = typename C::variant_type>
    class TpsaOrDoubleTemplate
    {
	using base_type = typename C::base_type;
	using tpsa_type = typename C::tpsa_type;
	using variant_type = typename C::variant_type;

	tpsa_or_double_t  arg;

        // private copy constructor ... a public required?
        TpsaOrDoubleTemplate(const TpsaOrDoubleTemplate& o) = default;

    public:
        inline TpsaOrDoubleTemplate(const double d)
	    : arg(d)
	    {}

        inline TpsaOrDoubleTemplate(const gtpsa::tpsa t)
	    : arg(t)
	    {}

        inline TpsaOrDoubleTemplate(TpsaOrDoubleTemplate&& o) = default;

        inline TpsaOrDoubleTemplate clone(const TpsaOrDoubleTemplate& o) { TpsaOrDoubleTemplate n = *this; return n; }

        // provided to implement operators taking double as first argument
        inline TpsaOrDoubleTemplate  dadd       (const double        a) const { TpsaOrDoubleTemplate n = *this; add_helper(a, this->arg, &n.arg); return n; }
        inline TpsaOrDoubleTemplate  dsub       (const double        a) const { TpsaOrDoubleTemplate n = *this; sub_helper(a, this->arg, &n.arg); return n; }
        inline TpsaOrDoubleTemplate  dmul       (const double        a) const { TpsaOrDoubleTemplate n = *this; mul_helper(a, this->arg, &n.arg); return n; }
        inline TpsaOrDoubleTemplate  ddiv       (const double        a) const { TpsaOrDoubleTemplate n = *this; div_helper(a, this->arg, &n.arg); return n; }

        inline TpsaOrDoubleTemplate& operator+= (const double        b) { add_helper(this->arg, b,     &this->arg); return *this; }
        inline TpsaOrDoubleTemplate& operator-= (const double        b) { sub_helper(this->arg, b,     &this->arg); return *this; }
        inline TpsaOrDoubleTemplate& operator*= (const double        b) { mul_helper(this->arg, b,     &this->arg); return *this; }
        inline TpsaOrDoubleTemplate& operator/= (const double        b) { div_helper(this->arg, b,     &this->arg); return *this; }

        inline TpsaOrDoubleTemplate& operator+= (const TpsaOrDoubleTemplate& b) { add_helper(this->arg, b.arg, &this->arg); return *this; }
        inline TpsaOrDoubleTemplate& operator-= (const TpsaOrDoubleTemplate& b) { sub_helper(this->arg, b.arg, &this->arg); return *this; }
        inline TpsaOrDoubleTemplate& operator*= (const TpsaOrDoubleTemplate& b) { mul_helper(this->arg, b.arg, &this->arg); return *this; }
        inline TpsaOrDoubleTemplate& operator/= (const TpsaOrDoubleTemplate& b) { div_helper(this->arg, b.arg, &this->arg); return *this; }

        inline TpsaOrDoubleTemplate  operator+  (const double        b) const { TpsaOrDoubleTemplate n = *this; add_helper(this->arg, b,     &n.arg); return n; }
        inline TpsaOrDoubleTemplate  operator-  (const double        b) const { TpsaOrDoubleTemplate n = *this; sub_helper(this->arg, b,     &n.arg); return n; }
        inline TpsaOrDoubleTemplate  operator*  (const double        b) const { TpsaOrDoubleTemplate n = *this; mul_helper(this->arg, b,     &n.arg); return n; }
        inline TpsaOrDoubleTemplate  operator/  (const double        b) const { TpsaOrDoubleTemplate n = *this; div_helper(this->arg, b,     &n.arg); return n; }

        inline TpsaOrDoubleTemplate  operator+  (const TpsaOrDoubleTemplate& b) const { TpsaOrDoubleTemplate n = *this; add_helper(this->arg, b.arg, &n.arg); return n; }
        inline TpsaOrDoubleTemplate  operator-  (const TpsaOrDoubleTemplate& b) const { TpsaOrDoubleTemplate n = *this; sub_helper(this->arg, b.arg, &n.arg); return n; }
        inline TpsaOrDoubleTemplate  operator*  (const TpsaOrDoubleTemplate& b) const { TpsaOrDoubleTemplate n = *this; mul_helper(this->arg, b.arg, &n.arg); return n; }
        inline TpsaOrDoubleTemplate  operator/  (const TpsaOrDoubleTemplate& b) const { TpsaOrDoubleTemplate n = *this; div_helper(this->arg, b.arg, &n.arg); return n; }

        void show (std::ostream& strm) const {
	    std::visit(overloaded{
		    [&strm] (const tpsa_&  arg){ strm << arg; },
		    [&strm] (const double& arg){ strm << arg; }
		}, this->arg);
        }
    };

    template<class C>
    inline TpsaOrDoubleTemplate<C> operator+ (const double a, const TpsaOrDoubleTemplate<C>& b){ return b.dadd(a); }
    template<class C>
    inline TpsaOrDoubleTemplate<C> operator- (const double a, const TpsaOrDoubleTemplate<C>& b){ return b.dsub(a); }
    template<class C>
    inline TpsaOrDoubleTemplate<C> operator* (const double a, const TpsaOrDoubleTemplate<C>& b){ return b.dmul(a); }
    template<class C>
    inline TpsaOrDoubleTemplate<C> operator/ (const double a, const TpsaOrDoubleTemplate<C>& b){ return b.ddiv(a); }

    template<class C>
    inline
    std::ostream& operator<<(std::ostream& strm, const TpsaOrDoubleTemplate<C>& td)
    {
	td.show(strm);
	return strm;
    }
} // namespace gtpsa

typedef TpsaVariantTypes<double, gtpsa::tpsa, tpsa_or_double_t> TpsaVariantDoubleTypes;
typedef TpsaOrDoubleTemplate<TpsaVariantDoubleTypes> TpsaOrDouble;

typedef TpsaVariantTypes<std::complex<double>, gtpsa::ctpsa, ctpsa_or_double_t> TpsaVariantComplexTypes;
typedef TpsaOrDoubleTemplate<TpsaVariantComplexTypes> CTpsaOrComplex;
