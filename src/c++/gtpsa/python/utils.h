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

#if 0
// inspired by https://stackoverflow.com/a/52393977
// this struct is not meant to be instantiated, but to statically provide ::type
template <typename T, typename... Args> struct variant_prepender;

// Create a variant type from another variant type by prepending its list of
// alternative types with std::monostate
// this struct is not meant to be instantiated, but to statically provide ::type
template <typename Variant> struct monostated : public variant_prepender<Variant, std::monostate>
{};


template <typename... Args0, typename... Args1>
struct variant_prepender<std::variant<Args0...>, Args1...> {
    using type = std::variant<Args1..., Args0...>;
};



// Implementation of variant_cast_no_monostate()
// inspired by https://stackoverflow.com/a/47204507
template <class... Args>
struct variant_cast_no_monostate_impl
{
    std::variant<Args...> v;

    template <class... ToArgs>
    operator std::variant<ToArgs...>() const
    {
        return std::visit(overloaded {
		[](const std::monostate&) -> std::variant<ToArgs...>
		{
		    throw std::runtime_error("variant_cast_no_monostate received a value of type std::monostate, cannot cast it");
		    return 0.0;
		},
		[](double&& arg) -> std::variant<ToArgs...>
		{
		    return arg ;
		},
		[](gtpsa::tpsa&& arg) -> std::variant<ToArgs...>
		{
		    return arg ;
		},
	    }, v);
    }
};

// Will convert one variant value into a value of another variant type that
// needs not to have a std::monostate in his alternative types. So the value
// converted must not be of type std::monostate else an exception is thrown.
template <class... Args>
auto variant_cast_no_monostate(const std::variant<Args...>& v) -> variant_cast_no_monostate_impl<Args...>
{
    return {v};
}


template<class Cls>
struct AddMethods {
    template<typename T>
    void add_methods(pybind11::class_<Cls> a_cls) {
    }
};

#endif

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
