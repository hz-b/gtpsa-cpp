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
        inline CTpsaOrComplex(const tpsa_type t)
                : base(t)
        {}

        /*
        inline CTpsaOrComplex(const CTpsaOrComplex& o)
                : base(o)
        {}
        */

        inline CTpsaOrComplex(const base& o)
                : base(o)
        {}

        inline CTpsaOrComplex clone(void) const {  return GTpsaOrBase(base::clone()); }

        inline TpsaOrDouble real(void) const {
            TpsaOrDouble res(0e0);
            std::visit(overloaded{
                    [&res](const base_type& arg){ res.m_arg.emplace<double>(arg.real()); },
                    [&res](const tpsa_type& arg){ res.m_arg.emplace<tpsa>(arg.real()); },
            }, this->m_arg);
            return res;
        }
        const TpsaOrDouble imag(void) const {
            TpsaOrDouble res(0e0);
            std::visit(overloaded{
                    [&res](const base_type& arg){ res.m_arg.emplace<double>(arg.imag()); },
                    [&res](const tpsa_type& arg){ res.m_arg.emplace<tpsa>(arg.imag()); },
            }, this->m_arg);
            return res;
        }
        //only implemented
        void real(gtpsa::tpsa* t) const {
            std::get<tpsa_type>(this->m_arg).real(t);
        }
        void imag(gtpsa::tpsa* t) const {
            std::get<tpsa_type>(this->m_arg).imag(t);
        }
        CTpsaOrComplex operator+ (const CTpsaOrComplex& o) const {return CTpsaOrComplex(base::operator+(o)); }
        CTpsaOrComplex operator- (const CTpsaOrComplex& o) const {return CTpsaOrComplex(base::operator-(o)); }
        CTpsaOrComplex operator* (const CTpsaOrComplex& o) const {return CTpsaOrComplex(base::operator*(o)); }
        CTpsaOrComplex operator/ (const CTpsaOrComplex& o) const {return CTpsaOrComplex(base::operator/(o)); }
    };


} // namespace gtpsa
#endif /* _GTPSA_CTPSA_COMPLEX_VARIANT_HPP_ */
