#ifndef _GTPSA_TPSA_DOUBLE_VARIANT_HPP_
#define _GTPSA_TPSA_DOUBLE_VARIANT_HPP_ 1
/**
 * @brief tpsa or double object in one variant type
 *
 * Beam dynamics calculations require to study the impact of
 * engineering tolerances, which are often represented using
 * truncated power series. On the other hand, one typically
 * want's only a limited set to be of the truncated power
 * series type.
 *
 * Here a type based on std::variant is provided.
 */

//#include <memory>
#include <gtpsa/gtpsa_base_variant.hpp>
#include <gtpsa/tpsa.hpp>

namespace gtpsa {
    namespace intern  {
        typedef std::variant<tpsa, double> tpsa_or_double_t;
    }

    inline void add_helper(const intern::tpsa_or_double_t& a, const intern::tpsa_or_double_t& b, intern::tpsa_or_double_t* c)
    {
	std::visit( overloaded {
		[&c](const double& ta, const double& tb) { c->emplace<double>(ta + tb); },
		// don't forget to include a default ...
		[&c](const auto&   ta, const auto&   tb) { c->emplace<tpsa> (ta + tb); },
	    }, a, b);
    }

    inline void sub_helper(const intern::tpsa_or_double_t& a, const intern::tpsa_or_double_t& b, intern::tpsa_or_double_t* c)
    {
	std::visit( overloaded {
		[&c](const double& ta, const double& tb) { c->emplace<double>(ta - tb); },
		[&c](const auto&   ta, const auto&   tb) { c->emplace<tpsa> (ta - tb); },
	    }, a, b);
    }

    inline void mul_helper(const intern::tpsa_or_double_t& a, const intern::tpsa_or_double_t& b, intern::tpsa_or_double_t* c)
    {
	std::visit( overloaded {
		[&c](const double& ta, const double& tb) { c->emplace<double>(ta * tb); },
		[&c](const auto&   ta, const auto&   tb) { c->emplace<tpsa> (ta * tb); },
	    }, a, b);
    }

    inline void div_helper(const intern::tpsa_or_double_t& a, const intern::tpsa_or_double_t& b, intern::tpsa_or_double_t* c)
    {
	std::visit( overloaded {
		[&c](const double& ta, const double& tb) { c->emplace<double>(ta / tb); },
		[&c](const auto&   ta, const auto&   tb) { c->emplace<tpsa> (ta / tb); },
	    }, a, b);
    }

    //inline void
} // gtpsa

namespace gtpsa {
    /**
     * @brief a variant allowing tpsa or double
     */
    typedef TpsaVariantTypes<double, tpsa, intern::tpsa_or_double_t> TpsaVariantDoubleTypes;

    class TpsaOrDouble;

    class TpsaOrDoubleVisitor {
    public:
	virtual ~TpsaOrDoubleVisitor(void) {}
	virtual void visit(const TpsaOrDouble& o) = 0;
    };

    class TODVImpl;

    class TpsaOrDouble : public GTpsaOrBase<TpsaVariantDoubleTypes> {
        using base = GTpsaOrBase<TpsaVariantDoubleTypes>;
        friend class CTpsaOrComplex;
	friend class TpsaOrDoubleVisitor;
	friend class TODVImpl;

	int t_access;
    public:
	virtual ~TpsaOrDouble() {}

        TpsaOrDouble(const double d)
                : base(d)
        {}

        TpsaOrDouble(const tpsa t)
                : base(t)
        {}
        TpsaOrDouble(const std::shared_ptr<gtpsa::desc> d, int nd)
        : base(d, nd)
        {}
        TpsaOrDouble(const base b)
        : base(b)
        {}
        TpsaOrDouble clone(void) const {return TpsaOrDouble(base::clone()) ;}

	virtual  void accept(TpsaOrDoubleVisitor& visitor) {
	    visitor.visit(*this);
	}
#if 0
// bool operators currently not working using a hack on user side
        inline bool operator== (const base_type o) const {
            bool flag;
            std::visit(overloaded{
                    [&flag] (const base_type& t, const base_type& o){ flag = false /* (t == o) */; },
                    [&flag] (const auto& t, const auto& o){ flag = false; /* flag = (t == o);  */},
            }, this->m_arg, o);
            return flag;
        }

        inline bool operator== (const TpsaOrDouble& o) const {
            bool flag;
            std::visit(overloaded{
                    [&flag] (const base_type& t, const base_type& o){ flag = (t == o); },
                    [&flag] (const auto& t, const auto& o){ flag = false; /* (t == o); */ },
            }, this->m_arg, o.m_arg);
            return flag;
        }
#endif
    };


#if 0
    inline bool operator== (const double a, const TpsaOrDouble& b){ return b.operator==(a); }
#endif

    class TODVImpl : public TpsaOrDoubleVisitor {
	// provide access to this internal detail .. a hack...
    public:
	virtual ~TODVImpl() {}
    protected:
	auto getArg(const TpsaOrDouble& o) {
	    return o.m_arg;
	}
    };


} // namespace gtpsa
#endif /*   _GTPSATODVImpl_TPSA_DOUBLE_VARIANT_HPP_ */
