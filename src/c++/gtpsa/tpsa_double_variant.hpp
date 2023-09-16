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
#include <gtpsa/utils.hpp>

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

    inline void neg_helper(const intern::tpsa_or_double_t& a, intern::tpsa_or_double_t* c)
    {
        std::visit( overloaded {
                [&c](const double& ta) { c->emplace<double>(-ta); },
                [&c](const auto&   ta) { c->emplace<tpsa> (-ta); },
        }, a);
    }

    inline void rapply_helper(const intern::tpsa_or_double_t& a, intern::tpsa_or_double_t* c,
			     double (*fd)(const double), gtpsa::tpsa (*ft)(const gtpsa::tpsa&))
    {
        std::visit( overloaded {
                [&c, &fd](const double& ta) { c->emplace<double>( fd(ta) ); },
		[&c, &ft](const auto&   ta) { c->emplace<tpsa>  ( ft(ta) ); },
	    }, a);
    }

    //inline void
} // gtpsa

namespace gtpsa {


    /**
     * @brief a variant allowing tpsa or double
     */
    typedef TpsaVariantTypes<double, tpsa, intern::tpsa_or_double_t> TpsaVariantDoubleTypes;

    class TpsaOrDouble;

    class TpsaOrDouble : public GTpsaOrBase<TpsaVariantDoubleTypes> {
        using base = GTpsaOrBase<TpsaVariantDoubleTypes>;
        friend class CTpsaOrComplex;
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

        inline TpsaOrDouble operator - ( void                  ) const { return TpsaOrDouble( std::move( base::operator- (*this) ) ) ; }

        inline TpsaOrDouble operator + ( const TpsaOrDouble& o ) const { return TpsaOrDouble( std::move( base::operator+ (o) ) ) ; }
        inline TpsaOrDouble operator - ( const TpsaOrDouble& o ) const { return TpsaOrDouble( std::move( base::operator- (o) ) ) ; }
        inline TpsaOrDouble operator * ( const TpsaOrDouble& o ) const { return TpsaOrDouble( std::move( base::operator* (o) ) ) ; }
        inline TpsaOrDouble operator / ( const TpsaOrDouble& o ) const { return TpsaOrDouble( std::move( base::operator/ (o) ) ) ; }

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
            bool flagm
            std::visit(overloaded{
                    [&flag] (const base_type& t, const base_type& o){ flag = (t == o); },
                    [&flag] (const auto& t, const auto& o){ flag = false; /* (t == o); */ },
            }, this->m_arg, o.m_arg);
            return flag;
        }
#endif
        inline void rsin(const TpsaOrDouble& o) { rapply_helper( o.m_arg, &this->m_arg, std::sin, gtpsa::sin); }
        inline void rcos(const TpsaOrDouble& o) { rapply_helper( o.m_arg, &this->m_arg, std::cos, gtpsa::cos); }

	inline void atan2(const TpsaOrDouble& y, const TpsaOrDouble& x) {
	    intern::tpsa_or_double_t* r = &this->m_arg;
	    std::visit( overloaded {
		    [&r](const double& ty, const double& tx) { r->emplace<double>( std::atan2(ty, tx)   ); },
		    [&r](const auto&   ty, const auto&   tx) { r->emplace<tpsa>  ( gtpsa::atan2(ty, tx) ); },
		}, y.m_arg, x.m_arg);
	}

	// required for operator +=
	inline double& ladd(double& a) const {
            std::visit(overloaded{
                    [&a] (const double& t){ a += t;       },
                    [&a] (const tpsa&   t){ a += t.cst(); },
            }, m_arg);
	    return a;
	}
	// required for operator -=
	inline double& lsub(double& a) const {
            std::visit(overloaded{
                    [&a] (const double& t){ a -= t;       },
                    [&a] (const tpsa&   t){ a -= t.cst(); },
            }, m_arg);
	    return a;
	}
	inline double& lmul(double& a) const {
            std::visit(overloaded{
                    [&a] (const double& t){ a *= t;       },
                    [&a] (const tpsa&   t){ a *= t.cst(); },
            }, m_arg);
	    return a;
	}
	// required for operator /=
	inline double& ldiv(double& a) const {
            std::visit(overloaded{
                    [&a] (const double& t){ a /= t;       },
                    [&a] (const tpsa&   t){ a /= t.cst(); },
            }, m_arg);
	    return a;
	}


    };



#if 0
    inline bool operator== (const double a, const TpsaOrDouble& b){ return b.operator==(a); }
#endif

#if 1
    /* just experimenting if conversion could be made  */
    inline double& operator+= (double& a, const TpsaOrDouble& b) { return b.ladd(a); }
    inline double& operator-= (double& a, const TpsaOrDouble& b) { return b.lsub(a); }
    inline double& operator*= (double& a, const TpsaOrDouble& b) { return b.lmul(a); }
    inline double& operator/= (double& a, const TpsaOrDouble& b) { return b.ldiv(a); }

    inline TpsaOrDouble sin(const TpsaOrDouble& o){ TpsaOrDouble n(o.clone()); n.rsin(o); return n; }
    inline TpsaOrDouble cos(const TpsaOrDouble& o){ TpsaOrDouble n(o.clone()); n.rcos(o); return n; }

    inline TpsaOrDouble atan2(const TpsaOrDouble& y, const TpsaOrDouble& x){ TpsaOrDouble n = y; n.atan2(y, x); return n; }
#endif

} // namespace gtpsa
#endif /*   _GTPSA_TPSA_DOUBLE_VARIANT_HPP_ */
