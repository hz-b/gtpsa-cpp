#ifndef _GTPSA_BASE_VARIANT_HPP_
#define _GTPSA_BASE_VARIANT_HPP_ 1

#include <memory>
#include <variant>
#include <gtpsa/desc.hpp>
#include <gtpsa/variant_forward_decl.hpp>

namespace gtpsa {
    /*
     * types that are contained within the variant next to
     * the variant itself
     */
    template<typename B, typename T, typename V>
    struct TpsaVariantTypes {
	// base type: e.g. double
	using base_type =  B;
	// tpsa type: tpsa for base type double
	using tpsa_type = T;
	// variant type tpsa_or_double_t for base of double
	using variant_type = V;
    };


     /* forward declaration */
    template<class C, typename = typename C::base_type, typename = typename C::tpsa_type,  typename = typename C::variant_type>
    class GTpsaOrBase;

    template<class C>
    class GTpsaOrBaseVisitor {
    public:
	virtual ~GTpsaOrBaseVisitor(void) {}
	virtual void visit(const GTpsaOrBase<C>& o) = 0;
    };

    template<class C>
    class GTpsaOrBaseVisitorImplementation;


    /**
     * @brief a class that either contains a double or a tpsa object
     *
     * motivated by engineering studies
     *
     * @todo: consider adding power function, further functions required ?
     */
    template<class C, typename, typename, typename>
    class GTpsaOrBase
    {

	friend class GTpsaOrBaseVisitor<C>;
	friend class GTpsaOrBaseVisitorImplementation<C>;

    protected:
	    using base_type = typename C::base_type;
	    using tpsa_type = typename C::tpsa_type;
	    using variant_type = typename C::variant_type;

	    variant_type m_arg;

    public:
	virtual ~GTpsaOrBase(void)
	    {}

        inline GTpsaOrBase(const base_type d)
	    : m_arg(d)
	    {}

        inline GTpsaOrBase(const tpsa_type t)
                : m_arg(t)
        {}
        inline GTpsaOrBase(const std::shared_ptr<gtpsa::desc> d, int nd)
                : m_arg(tpsa_type(d, nd))
        {}

        inline GTpsaOrBase(GTpsaOrBase&& o) = default;

        // assignment operators required ... move one
        inline GTpsaOrBase& operator= (GTpsaOrBase&& o) = default;

        /* seems that a public copy constructor is required */
        // private copy constructor ... a public required?
        inline GTpsaOrBase(const GTpsaOrBase& o) = default;

        // assignment operators required
        inline GTpsaOrBase& operator= (const GTpsaOrBase& o) = default;

        inline GTpsaOrBase clone(void) const { GTpsaOrBase n = *this; return n; }

	std::string pstr(void) const {
	    std::stringstream strm;
            std::visit(overloaded{
                    [&strm] (const base_type& t){  strm << t;},
                    [&strm] (const auto& t){ strm << t;}
            }, this->m_arg);
	    return strm.str();
	}

	virtual  void accept(GTpsaOrBaseVisitor<C>& visitor) {
	    visitor.visit(*this);
	}

        // provided to implement operators taking double as first argument
        inline GTpsaOrBase& operator+= (const base_type    b) { add_helper(this->m_arg, b, &this->m_arg); return *this; }
        inline GTpsaOrBase& operator-= (const base_type    b) { sub_helper(this->m_arg, b, &this->m_arg); return *this; }
        inline GTpsaOrBase& operator*= (const base_type    b) { mul_helper(this->m_arg, b, &this->m_arg); return *this; }
        inline GTpsaOrBase& operator/= (const base_type    b) { div_helper(this->m_arg, b, &this->m_arg); return *this; }

        inline GTpsaOrBase& operator+= (const GTpsaOrBase& b) { add_helper(this->m_arg, b.m_arg, &this->m_arg); return *this; }
        inline GTpsaOrBase& operator-= (const GTpsaOrBase& b) { sub_helper(this->m_arg, b.m_arg, &this->m_arg); return *this; }
        inline GTpsaOrBase& operator*= (const GTpsaOrBase& b) { mul_helper(this->m_arg, b.m_arg, &this->m_arg); return *this; }
        inline GTpsaOrBase& operator/= (const GTpsaOrBase& b) { div_helper(this->m_arg, b.m_arg, &this->m_arg); return *this; }

        inline GTpsaOrBase  operator-  (void)                 const { GTpsaOrBase n = *this; neg_helper(this->m_arg,    &n.m_arg); return n; }
        inline GTpsaOrBase  operator+  (const base_type    b) const { GTpsaOrBase n = *this; add_helper(this->m_arg, b, &n.m_arg); return n; }
        inline GTpsaOrBase  operator-  (const base_type    b) const { GTpsaOrBase n = *this; sub_helper(this->m_arg, b, &n.m_arg); return n; }
        inline GTpsaOrBase  operator*  (const base_type    b) const { GTpsaOrBase n = *this; mul_helper(this->m_arg, b, &n.m_arg); return n; }
        inline GTpsaOrBase  operator/  (const base_type    b) const { GTpsaOrBase n = *this; div_helper(this->m_arg, b, &n.m_arg); return n; }

        inline GTpsaOrBase  operator+  (const tpsa_type    b) const { GTpsaOrBase n = *this; add_helper(this->m_arg, b, &n.m_arg); return n; }
        inline GTpsaOrBase  operator-  (const tpsa_type    b) const { GTpsaOrBase n = *this; sub_helper(this->m_arg, b, &n.m_arg); return n; }
        inline GTpsaOrBase  operator*  (const tpsa_type    b) const { GTpsaOrBase n = *this; mul_helper(this->m_arg, b, &n.m_arg); return n; }
        inline GTpsaOrBase  operator/  (const tpsa_type    b) const { GTpsaOrBase n = *this; div_helper(this->m_arg, b, &n.m_arg); return n; }

        inline GTpsaOrBase  operator+  (const GTpsaOrBase& b) const { GTpsaOrBase n = *this; add_helper(this->m_arg, b.m_arg, &n.m_arg); return n; }
        inline GTpsaOrBase  operator-  (const GTpsaOrBase& b) const { GTpsaOrBase n = *this; sub_helper(this->m_arg, b.m_arg, &n.m_arg); return n; }
        inline GTpsaOrBase  operator*  (const GTpsaOrBase& b) const { GTpsaOrBase n = *this; mul_helper(this->m_arg, b.m_arg, &n.m_arg); return n; }
        inline GTpsaOrBase  operator/  (const GTpsaOrBase& b) const { GTpsaOrBase n = *this; div_helper(this->m_arg, b.m_arg, &n.m_arg); return n; }

	inline GTpsaOrBase  dadd       (const base_type    a) const { GTpsaOrBase n = *this; add_helper(a, this->m_arg, &n.m_arg); return n; }
        inline GTpsaOrBase  dsub       (const base_type    a) const { GTpsaOrBase n = *this; sub_helper(a, this->m_arg, &n.m_arg); return n; }
        inline GTpsaOrBase  dmul       (const base_type    a) const { GTpsaOrBase n = *this; mul_helper(a, this->m_arg, &n.m_arg); return n; }
        inline GTpsaOrBase  ddiv       (const base_type    a) const { GTpsaOrBase n = *this; div_helper(a, this->m_arg, &n.m_arg); return n; }

        inline GTpsaOrBase  dadd       (const tpsa_type    a) const { GTpsaOrBase n = *this; add_helper(a, this->m_arg, &n.m_arg); return n; }
        inline GTpsaOrBase  dsub       (const tpsa_type    a) const { GTpsaOrBase n = *this; sub_helper(a, this->m_arg, &n.m_arg); return n; }
        inline GTpsaOrBase  dmul       (const tpsa_type    a) const { GTpsaOrBase n = *this; mul_helper(a, this->m_arg, &n.m_arg); return n; }
        inline GTpsaOrBase  ddiv       (const tpsa_type    a) const { GTpsaOrBase n = *this; div_helper(a, this->m_arg, &n.m_arg); return n; }

        /**
         *
         * @return argument as tpsa or cptsa object
         *
	 * ref needed to see which desc etc ...
         * @warning fails if not reached
         */
        inline tpsa_type toTpsaType(const tpsa_type& ref) const noexcept {
	    if(std::holds_alternative<tpsa_type>(this->m_arg)){
		return this->tpsaType();
	    }

	    // assuming only two types need to be supported .. so now make
	    // a tpsa object and restun
            tpsa_type res = ref.newFromThis();
	    auto tmp =  std::get<base_type>(this->m_arg);
	    res.set(0, tmp);
	    return res;
        }

	inline tpsa_type tpsaType(void) const noexcept {
            return std::get<tpsa_type>(this->m_arg);
        }

        inline  base_type cst(void) const noexcept {
            base_type res;
            std::visit(overloaded{
                    /* needs to be fixed for clang */
                    [&res] (const tpsa_type& arg){ res = arg.cst(); },
                    [&res] (const base_type& arg){ res = arg; }
            }, this->m_arg);
            return res;
        }
        void show (std::ostream& strm) const noexcept {
            std::visit(overloaded{
                    [&strm] (const tpsa_type& arg){ strm << arg; },
                    [&strm] (const base_type& arg){ strm << arg; }
            }, this->m_arg);
        }

    };


    template<class C>
    inline GTpsaOrBase<C> operator+ (const typename C::base_type a, const GTpsaOrBase<C>& b){ return b.dadd(a); }
    template<class C>
    inline GTpsaOrBase<C> operator- (const typename C::base_type a, const GTpsaOrBase<C>& b){ return b.dsub(a); }
    template<class C>
    inline GTpsaOrBase<C> operator* (const typename C::base_type a, const GTpsaOrBase<C>& b){ return b.dmul(a); }
    template<class C>
    inline GTpsaOrBase<C> operator/ (const typename C::base_type a, const GTpsaOrBase<C>& b){ return b.ddiv(a); }

    template<class C>
    inline GTpsaOrBase<C> operator+ (const typename C::tpsa_type a, const GTpsaOrBase<C>& b){ return b.dadd(a); }
    template<class C>
    inline GTpsaOrBase<C> operator- (const typename C::tpsa_type a, const GTpsaOrBase<C>& b){ return b.dsub(a); }
    template<class C>
    inline GTpsaOrBase<C> operator* (const typename C::tpsa_type a, const GTpsaOrBase<C>& b){ return b.dmul(a); }
    template<class C>
    inline GTpsaOrBase<C> operator/ (const typename C::tpsa_type a, const GTpsaOrBase<C>& b){ return b.ddiv(a); }

    /*
    template<class C>
    inline typename C::base_type operator+ (const typename C::base_type a, const GTpsaOrBase<C>& b){ return a + b.cst(); }
    template<class C>
    inline typename C::base_type operator- (const typename C::base_type a, const GTpsaOrBase<C>& b){ return a - b.cst(); }
    template<class C>
    inline typename C::base_type operator* (const typename C::base_type a, const GTpsaOrBase<C>& b){ return a * b.cst(); }
    template<class C>
    inline typename C::base_type operator/ (const typename C::base_type a, const GTpsaOrBase<C>& b){ return a / b.cst(); }

    template<class C>
    inline typename C::tpsa_type operator+ (const typename C::tpsa_type a, const GTpsaOrBase<C>& b){ return a + b.toTpsaType(a); }
    template<class C>
    inline typename C::tpsa_type operator- (const typename C::tpsa_type a, const GTpsaOrBase<C>& b){ return a - b.toTpsaType(a); }
    template<class C>
    inline typename C::tpsa_type operator* (const typename C::tpsa_type a, const GTpsaOrBase<C>& b){ return a * b.toTpsaType(a); }
    template<class C>
    inline typename C::tpsa_typeo operator/ (const typename C::tpsa_type a, const GTpsaOrBase<C>& b){ return a / b.toTpsaType(a); }
    */


    template<class C>
    inline
    std::ostream& operator<<(std::ostream& strm, const GTpsaOrBase<C>& td)
    {
	td.show(strm);
	return strm;
    }


    template<class C>
    class GTpsaOrBaseVisitorImplementation : public GTpsaOrBaseVisitor<C> {
	// provide access to this internal detail .. a hack...
    public:
	virtual ~GTpsaOrBaseVisitorImplementation() {}
    protected:
	auto getArg(const GTpsaOrBase<C>& o) {
	    return o.m_arg;
	}
    };

} // namespace gtpsa


/*
typedef TpsaVariantTypes<std::complex<double>, gtpsa::ctpsa, tpsa_typeor_double_t> TpsaVariantComplexTypes;
typedef GTpsaOrBase<TpsaVariantComplexTypes> CTpsaOrComplex;
*/
#endif /* _GTPSA_BASE_VARIANT_HPP_ */
