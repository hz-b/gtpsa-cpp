
/**
 * see  https://github.com/pybind/pybind11/issues/4108
 * for implementation
 *
 */
#include "gtpsa_module.h"
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/complex.h>

#include <gtpsa/gtpsa_base_variant.hpp>
#include <gtpsa/tpsa_double_variant.hpp>
#include <gtpsa/ctpsa_complex_variant.hpp>
#include <gtpsa/python/utils.h>

namespace py = pybind11;
namespace gpy = gtpsa::python;

static const char to_object_doc[] = "return the stored variant as a python object.";
template<typename Types, typename Class>
void add_methods(py::class_<Class> t_mapper)
{
    t_mapper
	.def("__repr__", &Class::pstr)
	.def("to_object", [](Class& inst){ return gpy::intern::to_pyobject<Types, Class>(inst); }, to_object_doc)
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
		     return gpy::intern::to_tpsa_base_variant<Types, Class>(v);
		 }),
	     "initialise tpsa or double", py::arg("tpsa or double"))
	;
}

void gpy::py_gtpsa_init_variant(pybind11::module &m)
{

    py::class_<gtpsa::TpsaOrDouble> t_d(m, "TpsaOrDouble");
    add_methods<gtpsa::TpsaVariantDoubleTypes, gtpsa::TpsaOrDouble>(t_d);
    t_d
	.def(py::self += double())
	.def(py::self -= double())
	.def(py::self *= double())
	.def(py::self /= double())

	.def(py::self + double())
	.def(py::self - double())
	.def(py::self * double())
	.def(py::self / double())

	;

    py::class_<gtpsa::CTpsaOrComplex> t_c(m, "CTpsaOrComplex");
    add_methods<gtpsa::TpsaVariantComplexTypes, gtpsa::CTpsaOrComplex>(t_c);
    t_c
	.def("real", [](const gtpsa::CTpsaOrComplex& inst){ return inst.real();})
	.def("imag", [](const gtpsa::CTpsaOrComplex& inst){ return inst.imag();})

	.def(py::self += std::complex<double>())
	.def(py::self -= std::complex<double>())
	.def(py::self *= std::complex<double>())
	.def(py::self /= std::complex<double>())

	.def(py::self + std::complex<double>())
	.def(py::self - std::complex<double>())
	.def(py::self * std::complex<double>())
	.def(py::self / std::complex<double>())
	;

    m.def("cos", [](const gtpsa::TpsaOrDouble& v){ return gtpsa::cos( v ); });
    m.def("sin", [](const gtpsa::TpsaOrDouble& v){ return gtpsa::sin( v ); });
}
