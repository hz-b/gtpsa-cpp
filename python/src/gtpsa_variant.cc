
/**
 * see  https://github.com/pybind/pybind11/issues/4108
 * for implementation
 *
 */
#include "gtpsa_module.h"
#include <pybind11/pybind11.h>
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

static gtpsa::TpsaOrDouble to_tpsa_or_double(const gtpsa::intern::tpsa_or_double_t tmp)
{

    gtpsa::TpsaOrDouble res = std::visit(overloaded {
	    [](const double&        arg) { return gtpsa::TpsaOrDouble(arg); },
	    [](const gtpsa::tpsa&   arg) { return gtpsa::TpsaOrDouble(arg); }
	}, tmp);
    return res;

}

struct PythonVisitor
{

};

void py_gtpsa_init_variant(pybind11::module &m)
{

    py::class_<gtpsa::TpsaOrDouble>(m, "TpsaOrDouble")
	.def("__repr__", &gtpsa::TpsaOrDouble::pstr)
	.def(py::init<>(
		 /* is this used at all ? */
		 [](gtpsa::intern::tpsa_or_double_t& v) {
		     return to_tpsa_or_double(v);
		 }),
	     "initialise tpsa or double", py::arg("tpsa or double"))
	.def(py::init<>(
		 [](const double& v) {
		     return gtpsa::TpsaOrDouble(v);
		 }),
	     "initialise tpsa or double with a double value", py::arg("double") = 0e0)
	.def(py::init<>(
		 [](const gtpsa::tpsa& t) {
		     return gtpsa::TpsaOrDouble(t);
		 }),
	     /* no default here .. due to desc obj */
	     "initialise tpsa or double with a double value", py::arg("tpsa object"))
	.def("to_object", [](Cls& inst){ return inst})
	;

}
