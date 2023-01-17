#ifndef _GTPSA_CTPSA_COMPLEX_VARIANT_HPP_
#define _GTPSA_CTPSA_COMPLEX_VARIANT_HPP_ 1
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
#include <gtpsa/ctpsa.hpp>
#include <gtpsa/variant_forward_decl.hpp>
#include <variant>
#include <complex>

namespace gtpsa {
    namespace intern {
        typedef std::complex<double> cplx_;
        typedef std::variant<gtpsa::ctpsa, cplx_> ctpsa_or_cplx_t;
    }
    inline void add_helper(const intern::ctpsa_or_cplx_t& a, const intern::ctpsa_or_cplx_t& b, intern::ctpsa_or_cplx_t* c)
    {
        std::visit( gtpsa::overloaded {
                [&c](const intern::cplx_& ta, const intern::cplx_& tb) { c->emplace<intern::cplx_>(ta + tb);  },
                // don't forget to include a default ...
                [&c](const auto&  ta, const auto&  tb) { c->emplace<ctpsa>(ta + tb); },
        }, a, b);
    }

    inline void sub_helper(const intern::ctpsa_or_cplx_t& a, const intern::ctpsa_or_cplx_t& b, intern::ctpsa_or_cplx_t* c)
    {
        std::visit( gtpsa::overloaded {
                [&c](const intern::cplx_& ta, const intern::cplx_& tb) { c->emplace<intern::cplx_> (ta - tb); },
                [&c](const auto&  ta, const auto&  tb) { c->emplace<ctpsa>(ta - tb); },
        }, a, b);
    }

    inline void mul_helper(const intern::ctpsa_or_cplx_t& a, const intern::ctpsa_or_cplx_t& b, intern::ctpsa_or_cplx_t* c)
    {
        std::visit( gtpsa::overloaded {
                [&c](const intern::cplx_& ta, const intern::cplx_& tb) { c->emplace<intern::cplx_> (ta * tb); },
                [&c](const auto&  ta, const auto&  tb) { c->emplace<ctpsa>(ta * tb); },
        }, a, b);
    }

    inline void div_helper(const intern::ctpsa_or_cplx_t& a, const intern::ctpsa_or_cplx_t& b, intern::ctpsa_or_cplx_t* c) {
        std::visit(gtpsa::overloaded{
                [&c](const intern::cplx_ &ta, const intern::cplx_ &tb) { c->emplace<intern::cplx_>(ta / tb); },
                [&c](const auto &ta, const auto &tb) { c->emplace<ctpsa>(ta / tb); },
        }, a, b);
    }

    inline void neg_helper(const intern::ctpsa_or_cplx_t& a, intern::ctpsa_or_cplx_t* c)
    {
        std::visit( overloaded {
                [&c](const intern::cplx_& ta) { c->emplace<intern::cplx_>(-ta); },
                [&c](const ctpsa&         ta) { c->emplace<ctpsa>        (-ta); },
        }, a);
    }
    inline void apply_helper(const intern::ctpsa_or_cplx_t& a, intern::ctpsa_or_cplx_t* r,
                             intern::cplx_(fc)(const intern::cplx_&), ctpsa(ft)(const ctpsa&)){
        std::visit( gtpsa::overloaded{
                [&r, &fc](const intern::cplx_ &a) {r->emplace<intern::cplx_>( fc(a) );},
                [&r, &ft](const ctpsa         &a) {r->emplace<ctpsa>        ( ft(a) );}
        }, a);
    }
} // gtpsa namespace

#include <gtpsa/gtpsa_base_variant.hpp>
// for real and imaginary
#include <gtpsa/tpsa_double_variant.hpp>


namespace gtpsa {
    /**
     * @brief a variant allowing tpsa or double
     */
    typedef TpsaVariantTypes<intern::cplx_, ctpsa, intern::ctpsa_or_cplx_t> TpsaVariantComplexTypes;

    class CTpsaOrComplex : public GTpsaOrBase<TpsaVariantComplexTypes>
    {
        using base = GTpsaOrBase<TpsaVariantComplexTypes>;
    public:
        inline CTpsaOrComplex(const base_type c)
                : base(c)
        {}
        inline CTpsaOrComplex(const double c)
                : base(c)
        {}
        inline CTpsaOrComplex(const tpsa_type t)
                : base(t)
        {}
        inline CTpsaOrComplex(const base& o)
                : base(o)
        {}
        inline CTpsaOrComplex(const double re, const double im)
                : base(base_type (re, im))
	    {}
        inline CTpsaOrComplex(const tpsa re, const tpsa im)
                : base(tpsa_type (re, im))
        {}
        inline CTpsaOrComplex(const TpsaOrDouble& re, const TpsaOrDouble& im)
		: base( base_type (0e0, 0e0))
        {
                intern::ctpsa_or_cplx_t res(0e0);
                std::visit(
			overloaded{
				[&res](const double &a, const double &b) { res.emplace<intern::cplx_>(a, b); },
				[&res](const tpsa   &a, const tpsa   &b) { res.emplace<ctpsa        >(a, b); },
				[&res](const tpsa   &a, const double &b) { gtpsa::tpsa tmp(a.getDescription(), 0); tmp.set(0, b); res.emplace<ctpsa>(a, tmp); },
				[&res](const double &a, const tpsa   &b) { gtpsa::tpsa tmp(b.getDescription(), 0); tmp.set(0, a); res.emplace<ctpsa>(tmp, b); }
			}, re.m_arg, im.m_arg);
                this->m_arg = res;
        }

        /*
        inline CTpsaOrComplex(const CTpsaOrComplex& o)
                : base(o)
        {}
        */


        inline CTpsaOrComplex clone(void) const {  return GTpsaOrBase(base::clone()); }

        inline TpsaOrDouble real(void) const {
            TpsaOrDouble res(0e0);
            std::visit(overloaded{
                    [&res](const base_type& arg){ res.m_arg.emplace<double>(arg.real()); },
                    [&res](const tpsa_type& arg){ res.m_arg.emplace<tpsa>(arg.real()); },
            }, this->m_arg);
            return res;
        }
        inline const TpsaOrDouble imag(void) const {
            TpsaOrDouble res(0e0);
            std::visit(overloaded{
                    [&res](const base_type& arg){ res.m_arg.emplace<double>(arg.imag()); },
                    [&res](const tpsa_type& arg){ res.m_arg.emplace<tpsa>(arg.imag()); },
            }, this->m_arg);
            return res;
        }
        //only implemented

        inline void real(gtpsa::tpsa* t) const {
            std::get<tpsa_type>(this->m_arg).real(t);
        }
        inline void imag(gtpsa::tpsa* t) const {
            std::get<tpsa_type>(this->m_arg).imag(t);
        }
        inline CTpsaOrComplex operator+ (const CTpsaOrComplex& o) const {return CTpsaOrComplex(base::operator+(o)); }
        inline CTpsaOrComplex operator- (const CTpsaOrComplex& o) const {return CTpsaOrComplex(base::operator-(o)); }
        inline CTpsaOrComplex operator* (const CTpsaOrComplex& o) const {return CTpsaOrComplex(base::operator*(o)); }
        inline CTpsaOrComplex operator/ (const CTpsaOrComplex& o) const {return CTpsaOrComplex(base::operator/(o)); }

        inline CTpsaOrComplex& operator+= (const CTpsaOrComplex& o)  { CTpsaOrComplex(base::operator+=(o)); return *this; }
        inline CTpsaOrComplex& operator-= (const CTpsaOrComplex& o)  { CTpsaOrComplex(base::operator-=(o)); return *this; }
        inline CTpsaOrComplex& operator*= (const CTpsaOrComplex& o)  { CTpsaOrComplex(base::operator*=(o)); return *this; }
        inline CTpsaOrComplex& operator/= (const CTpsaOrComplex& o)  { CTpsaOrComplex(base::operator/=(o)); return *this; }

        inline CTpsaOrComplex& operator+= (const std::complex<double>& o) { CTpsaOrComplex(base::operator+=(std_complex_double_to_cpx_t(o))); return *this; }
        inline CTpsaOrComplex& operator-= (const std::complex<double>& o) { CTpsaOrComplex(base::operator-=(std_complex_double_to_cpx_t(o))); return *this; }
        inline CTpsaOrComplex& operator*= (const std::complex<double>& o) { CTpsaOrComplex(base::operator*=(std_complex_double_to_cpx_t(o))); return *this; }
        inline CTpsaOrComplex& operator/= (const std::complex<double>& o) { CTpsaOrComplex(base::operator/=(std_complex_double_to_cpx_t(o))); return *this; }

        inline void rexp(const CTpsaOrComplex& o) { apply_helper(o.m_arg, &this->m_arg, std::exp, gtpsa::exp); }
    };

    /*
    inline std::complex<double> operator+ (const std::complex<double> a, const CTpsaOrComplex& b){ return std_complex_double_to_cpx_t(a) + b.cst(); }
    inline std::complex<double> operator- (const std::complex<double> a, const CTpsaOrComplex& b){ return std_complex_double_to_cpx_t(a) - b.cst(); }
    inline std::complex<double> operator* (const std::complex<double> a, const CTpsaOrComplex& b){ return std_complex_double_to_cpx_t(a) * b.cst(); }
    inline std::complex<double> operator/ (const std::complex<double> a, const CTpsaOrComplex& b){ return std_complex_double_to_cpx_t(a) / b.cst(); }

    inline ctpsa operator+ (const ctpsa a, const CTpsaOrComplex& b){ return a + static_cast<GTpsaOrBase<TpsaVariantComplexTypes>>(b); }
    inline ctpsa operator- (const ctpsa a, const CTpsaOrComplex& b){ return a - static_cast<GTpsaOrBase<TpsaVariantComplexTypes>>(b); }
    inline ctpsa operator* (const ctpsa a, const CTpsaOrComplex& b){ return a * static_cast<GTpsaOrBase<TpsaVariantComplexTypes>>(b); }
    inline ctpsa operator/ (const ctpsa a, const CTpsaOrComplex& b){ return a / static_cast<GTpsaOrBase<TpsaVariantComplexTypes>>(b); }
    */
    inline CTpsaOrComplex exp(const CTpsaOrComplex& a) { auto n = a.clone(); n.rexp(a); return n;}


} // namespace gtpsa
#endif /* _GTPSA_CTPSA_COMPLEX_VARIANT_HPP_ */
