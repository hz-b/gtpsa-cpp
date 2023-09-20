#ifndef _GTPSA_PYTHON_UTILS_H_
#define _GTPSA_PYTHON_UTILS_H_ 1

#include <pybind11/pybind11.h>
#include <gtpsa/gtpsa_base_variant.hpp>
#include <gtpsa/tpsa_double_variant.hpp>
#include <gtpsa/ctpsa_complex_variant.hpp>

namespace gtpsa::python {

    namespace intern {

// helper type for std::visit
	template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
	template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;


        template<typename C, typename T>
            static auto to_tpsa_base_variant(const typename C::variant_type tmp)
        {
            T res = std::visit(overloaded {
                    [](const typename C::base_type& arg) { return T(arg); },
                    [](const typename C::tpsa_type& arg) { return T(arg); }
                }, tmp);
            return res;
        }


        template<class C>
            class PythonVisitor : public gtpsa::GTpsaOrBaseVisitorImplementation<C>
        {
            pybind11::object m_obj;

        public:
            virtual void visit(const gtpsa::GTpsaOrBase<C>& o) override final {
                pybind11::object obj;

                auto arg = this->getArg(o);
                std::visit(overloaded {
                        [&obj](const typename C::base_type& arg) { obj = pybind11::cast(arg); },
                        [&obj](const typename C::tpsa_type& arg) { obj = pybind11::cast(arg); }
                    }, arg);
                m_obj = obj;
            }

            inline auto getObject(void) const {
                return this->m_obj;
            }

        };

        template<typename C, typename T>
            static auto to_pyobject(const T& inst)
        {
            PythonVisitor<C> visitor;
            inst.accept(visitor);
            return visitor.getObject();
        }

    } //  namespace gtpsa::python::intern

    // support conversion to python object within c++ wrapper
    // i.e. do not leak C++ variant type
    template<typename T>
        static auto to_pyobject(const T& arg) { return pybind11::cast(arg); }
    static auto to_pyobject(const gtpsa::TpsaOrDouble&   arg){
        return gtpsa::python::intern::to_pyobject<gtpsa::TpsaVariantDoubleTypes, gtpsa::TpsaOrDouble>(arg);
    }
    static auto to_pyobject(const gtpsa::CTpsaOrComplex& arg) {
        return gtpsa::python::intern::to_pyobject<gtpsa::TpsaVariantComplexTypes, gtpsa::CTpsaOrComplex>(arg);
    }


} // namespace gtpsa::python

#endif /* _GTPSA_PYTHON_UTILS_H_ */
