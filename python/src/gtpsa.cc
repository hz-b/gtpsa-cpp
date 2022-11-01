#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <pybind11/complex.h>
#include <pybind11/operators.h>
#include <gtpsa/tpsa.hpp>
#include <gtpsa/ctpsa.hpp>
#include <string>
#include <sstream>

#include "gtpsa_module.h"

namespace py = pybind11;

static const char tpsa_init_same_doc[] = "Create a new (c)tpsa object with the same properties as this one\n\
Use clone to create a copy of the content too. \n";

static const char tpsa_init_desc_doc[] = "Create a new (c)tpsa object using the passed description object. Default order\
 will be used if none is speficied\n\
Use clone to create a copy of the content too. \n";


template<class Cls>
struct AddMethods
{
    template<typename BCls, typename T>
    void add_methods_gtpsa_mad(py::class_<BCls> a_cls) {


	a_cls
	    .def("length",         &Cls::length)
	    .def("getDescription", &Cls::getDescription)
	    .def("clear",          &Cls::clear)
	    .def("isNull",         &Cls::isNull)
	    .def("mono",           &Cls::mono)
	    .def("index",          py::overload_cast<const std::vector<ord_t>&>( &Cls::idx, py::const_))
	    .def("get",            py::overload_cast<>( &Cls::get, py::const_))
	    .def("get",            py::overload_cast<const std::vector<ord_t>&>( &Cls::get, py::const_))
	    // make it more pythonic!
	    .def("getv",           [](const Cls& inst, idx_t i){
				       std::vector<T> tmp(inst.length()); inst.getv(i, &tmp);
				       return py::array(py::cast(tmp));
				   })
	    .def("setv",           &Cls::setv)
	    .def("getsm",          &Cls::getsm)
	    // problems with complex values ....
	    // .def("set",            py::overload_cast<const std::vector<ord_t>&, T, T>( &Cls::set))
	    .def("setVariable",    &Cls::setVariable, "set the variable?",
		 py::arg("v"), py::arg("iv") = 0, py::arg("scl") = 0)
	    .def_property("name",  &Cls::name, &Cls::setName)
	    .def_property("uid",   [](Cls& inst){ return inst.uid(0);}, &Cls::uid)

	    .def_property_readonly("order", &Cls::order)
	    .def(py::init<std::shared_ptr<gtpsa::desc>, const ord_t>(), tpsa_init_desc_doc,
		 py::arg("tpsa"), py::arg("order") = int(gtpsa::init::default_)
		)
	    .def(py::init<const BCls&, const ord_t>(), tpsa_init_same_doc,
		 py::arg("tpsa"), py::arg("order") = int(gtpsa::init::same))
	    ;
    }

    template<typename BCls, typename T>
    void add_methods_ops(py::class_<BCls> a_cls) {
	a_cls
	    .def(py::self += py::self)
	    .def(py::self -= py::self)
	    .def(py::self *= py::self)
	    .def(py::self /= py::self)

	    .def(-py::self )
	    .def(py::self +  py::self)
	    .def(py::self -  py::self)
	    .def(py::self *  py::self)
	    .def(py::self /  py::self)

	    .def(py::self +  T()     )
	    .def(py::self -  T()     )
	    .def(py::self *  T()     )
	    .def(py::self /  T()     )


	    .def(T()      +  py::self)
	    .def(T()      -  py::self)
	    .def(T()      *  py::self)
	    .def(T()      /  py::self)

	    .def("__repr__", &Cls::repr)
	    .def("__str__" , &Cls::pstr)
	    .def("__copy__", &Cls::clone)
	    ;
    }

    template<typename BCls, typename T>
    void add_methods(py::class_<BCls> a_cls) {
	add_methods_gtpsa_mad<BCls, T>(a_cls);
	add_methods_ops<BCls, T>(a_cls);
    }
};


void py_gtpsa_init_tpsa(py::module &m)
{


    py::class_<gtpsa::TPSAWithOp, std::shared_ptr<gtpsa::TPSAWithOp>> tpsa_with_op  (m, "_TPSAWithOp");
    AddMethods<gtpsa::TPSAWithOp> tpsa_m;
    tpsa_m.add_methods<gtpsa::TPSAWithOp, num_t>(tpsa_with_op);
    py::class_<gtpsa::tpsa, std::shared_ptr<gtpsa::tpsa>>   tpsa  (m, "tpsa",  tpsa_with_op);
    tpsa
	.def("set", py::overload_cast<num_t, num_t>( &gtpsa::tpsa::set))
	.def("set", py::overload_cast<const std::vector<ord_t>&, num_t, num_t>( &gtpsa::tpsa::set))
	//.def(py::self += double())
	//.def(py::self -= double())
	//.def(py::self *= double())
	//.def(py::self /= double())
	//
	//.def(py::self + double())
	//.def(py::self - double())
	//.def(py::self * double())
	//.def(py::self / double())
	//
	//.def(double() + py::self)
	//.def(double() - py::self)
	//.def(double() * py::self)
	//.def(double() / py::self)
	.def(py::init<std::shared_ptr<gtpsa::desc>, const ord_t>(), tpsa_init_desc_doc,
	     py::arg("tpsa"), py::arg("order") = int(gtpsa::init::default_)
	    )
	.def(py::init<const gtpsa::tpsa&, const ord_t>(), tpsa_init_same_doc,
	     py::arg("tpsa"), py::arg("order") = int(gtpsa::init::same))
	;

#define GTPSA_FUNC_ARG1(func) \
    m.def(#func,       py::overload_cast<const gtpsa::TPSAWithOp&>(&gtpsa:: func)); \
    m.def(#func  "_",  py::overload_cast<const gtpsa::TPSAWithOp&, gtpsa::TPSAWithOp*>(&gtpsa:: func ## _));
#include <gtpsa/funcs.h>

#undef GTPSA_FUNC_ARG1


    py::class_<gtpsa::CTPSAWithOp, std::shared_ptr<gtpsa::CTPSAWithOp>> ctpsa_with_op  (m, "_CTPSAWithOp");
    py::class_<gtpsa::ctpsa , std::shared_ptr<gtpsa::ctpsa>>  ctpsa (m, "ctpsa", ctpsa_with_op);
    AddMethods<gtpsa::ctpsa> ctpsa_m;
    ctpsa_m.add_methods<gtpsa::ctpsa, cnum_t>(ctpsa);
    ctpsa
	.def("set0",  &gtpsa::ctpsa::_set0)
	.def("setm",  &gtpsa::ctpsa::_setm)
	//.def(py::self += std::complex<double>())
	//.def(py::self -= std::complex<double>())
	//.def(py::self *= std::complex<double>())
	//.def(py::self /= std::complex<double>())
	//
	//.def(py::self + std::complex<double>())
	//.def(py::self - std::complex<double>())
	//.def(py::self * std::complex<double>())
	//.def(py::self / std::complex<double>())

	//.def(std::complex<double>() + py::self)
	//.def(std::complex<double>() - py::self)
	//.def(std::complex<double>() * py::self)
	//.def(std::complex<double>() / py::self)

	.def(py::init<std::shared_ptr<gtpsa::desc>, const ord_t>(), tpsa_init_desc_doc,
	     py::arg("tpsa"), py::arg("order") = int(gtpsa::init::default_)
	    )
	.def(py::init<const gtpsa::tpsa&, const ord_t>(), tpsa_init_same_doc,
	     py::arg("tpsa"), py::arg("order") = int(gtpsa::init::same))
	;
	;

#define GTPSA_FUNC_ARG1(func) \
   m.def(#func,       py::overload_cast<const gtpsa::CTPSAWithOp&>(&gtpsa:: func)); \
   m.def(#func  "_",  py::overload_cast<const gtpsa::CTPSAWithOp&, gtpsa::CTPSAWithOp*>(&gtpsa:: func ## _));

#include <gtpsa/funcs.h>
#undef GTPSA_FUNC_ARG1


    py::enum_<gtpsa::init>(m, "init")
	.value("default", gtpsa::init::default_)
	.value("same",    gtpsa::init::same);

    /*
    tpsa
    .def("set",            py::overload_cast<cnum_t, cnum_t>( &gtpsa::tpsa::set))
    ;
    */


}
