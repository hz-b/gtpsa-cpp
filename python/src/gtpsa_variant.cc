
/**
 * see  https://github.com/pybind/pybind11/issues/4108
 * for implementation
 *
 */
#include "gtpsa_module.h"
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include <gtpsa/gtpsa_base_variant.hpp>
#include <gtpsa/tpsa_double_variant.hpp>
#include <gtpsa/ctpsa_complex_variant.hpp>
#include <variant>

namespace py=pybind11;

// helper type for std::visit
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

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
    void add_methods(py::class_<Cls> a_cls) {
    }
};

template<typename C, typename T>
static auto to_tpsa_or_base(const typename C::variant_type tmp)
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
    py::object m_obj;

public:
    virtual void visit(const gtpsa::GTpsaOrBase<C>& o) override final {
	py::object obj;

	auto arg = this->getArg(o);
	std::visit(overloaded {
		[&obj](const typename C::base_type& arg) { obj = py::cast(arg); },
		[&obj](const typename C::tpsa_type& arg) { obj = py::cast(arg); }
	}, arg);
	m_obj = obj;
    }

    auto getObject(void) const {
	return this->m_obj;
    }

};

template<typename C, typename T>
auto to_pyobject(T& inst)
{
    PythonVisitor<C> visitor;
    inst.accept(visitor);
    return visitor.getObject();
}


template<typename Types, typename Class>
void add_methods(py::class_<Class> t_mapper)
{
    t_mapper
	.def("__repr__", &Class::pstr)
	.def("to_object", [](Class& inst){ return to_pyobject<Types, Class>(inst); })
	.def(py::self += py::self)
	.def(py::self -= py::self)
	.def(py::self *= py::self)
	.def(py::self /= py::self)
	.def(py::self +  py::self)
	.def(py::self -  py::self)
	.def(py::self *  py::self)
	.def(py::self /  py::self)
	// .def("__pow__", [](Class& inst, const int    n ) { return gtpsa::pow(inst, n); })
	// .def("__pow__", [](Class& inst, const double v ) { return gtpsa::pow(inst, v); })
	.def(py::init<>(
		 [](const typename Types::base_type& v) {
		     // class provides ctor's for both types
		     return Class(v);
		 }),
	     "initialise gtpsa or base with a base value", py::arg("base") = 0e0)
	.def(py::init<>(
		 [](const typename Types::tpsa_type& t) {
		     // class provides ctor's for both types
		     return Class(t);
		 }),
	     /* no default here .. due to desc obj */
	     "initialise tpsa or double with a double value", py::arg("tpsa object"))
	.def(py::init<>(
		 /* is this used at all ? */
		 [](typename Types::variant_type& v) {
		     return to_tpsa_or_base<Types, Class>(v);
		 }),
	     "initialise tpsa or double", py::arg("tpsa or double"))
	;
}

void py_gtpsa_init_variant(pybind11::module &m)
{

    py::class_<gtpsa::TpsaOrDouble> t_d(m, "TpsaOrDouble");
    add_methods<gtpsa::TpsaVariantDoubleTypes, gtpsa::TpsaOrDouble>(t_d);

    py::class_<gtpsa::CTpsaOrComplex> t_c(m, "CTpsaOrComplex");
    add_methods<gtpsa::TpsaVariantComplexTypes, gtpsa::CTpsaOrComplex>(t_c);

}
