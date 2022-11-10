#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <pybind11/complex.h>
#include <pybind11/operators.h>
#include <gtpsa/tpsa.hpp>
#include <gtpsa/ctpsa.hpp>
#include <string>
#include <sstream>
#include <vector>

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
	    .def("length",          &Cls::length)
	    .def("get_description", &Cls::getDescription)
	    .def("clear",           &Cls::clear)
	    .def("is_null",         &Cls::isNull)
	    .def("mono",            &Cls::mono)
	    .def("get",             [](const Cls& inst){
					return inst.get();
				    })
	    .def("get",             [](const Cls& inst, const std::vector<ord_t>& m){
					return inst.get(m);
				    })
	    .def("set",             [](Cls& inst,      const std::vector<ord_t>& m, T a, T b){
					inst.set(m, a, b);
				    })
	    .def("set",             [](Cls& inst,                                   T a, T b){
					inst.set(a, b);
				    })
	    .def("index",          [](const Cls& inst, const std::vector<ord_t>& m){ return inst.index(m);})
	    // make it more pythonic!
	    .def("getv",           [](const Cls& inst, idx_t i){
				       std::vector<T> tmp(inst.length()); inst.getv(i, &tmp);
				       return py::array(py::cast(tmp));
				   })
	    .def("setv",           &Cls::setv)
	    .def("getsm",          &Cls::getsm)
	    .def("set_variable",    &Cls::setVariable, "set the variable?",
		 py::arg("v"), py::arg("iv") = 0, py::arg("scl") = 0)
	    .def_property("name",  &Cls::name, &Cls::setName)
	    .def_property("uid",   [](Cls& inst){ return inst.uid(0);}, &Cls::uid)

	    .def_property_readonly("order", &Cls::order)
	    .def(py::init<std::shared_ptr<gtpsa::mad::desc>, const ord_t>(), tpsa_init_desc_doc,
		 py::arg("tpsa"), py::arg("order") = int(gtpsa::mad::init::default_)
		)
	    .def(py::init<const BCls&, const ord_t>(), tpsa_init_same_doc,
		 py::arg("tpsa"), py::arg("order") = int(gtpsa::mad::init::same))
	    ;
    }

    template<typename BCls, typename T>
    void add_methods_ops(py::class_<BCls> a_cls) {
	a_cls
	    .def("__repr__", &Cls::repr)
	    .def("__str__" , &Cls::pstr)
	    .def("__copy__", &Cls::clone)
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


    typedef gtpsa::TpsaWithOp<gtpsa::TpsaTypeBridgeInfo>   TpsaOp;
    typedef gtpsa::TpsaWithOp<gtpsa::CTpsaTypeBridgeInfo> CTpsaOp;

    py::class_<TpsaOp, std::shared_ptr<TpsaOp>> tpsa_with_op  (m, "_TPSAWithOp");
    AddMethods<TpsaOp> tpsa_m_op;
    tpsa_m_op.add_methods<TpsaOp, num_t>(tpsa_with_op);
    py::class_<gtpsa::tpsa, std::shared_ptr<gtpsa::tpsa>>   tpsa  (m, "tpsa",  tpsa_with_op);
    AddMethods<gtpsa::tpsa> tpsa_m;
    tpsa_m.add_methods_ops<gtpsa::tpsa, num_t>(tpsa);
    tpsa
	//.def("set", py::overload_cast<num_t, num_t>( &gtpsa::tpsa::set))
	//.def("set", py::overload_cast<const std::vector<ord_t>&, num_t, num_t>( &gtpsa::tpsa::set))
	.def(py::self += double())
	.def(py::self -= double())
	.def(py::self *= double())
	.def(py::self /= double())
	//
	.def(py::self + double())
	.def(py::self - double())
	.def(py::self * double())
	.def(py::self / double())
	//
	.def(double() + py::self)
	.def(double() - py::self)
	.def(double() * py::self)
	.def(double() / py::self)
	.def(py::init<std::shared_ptr<gtpsa::mad::desc>, const ord_t>(), tpsa_init_desc_doc,
	     py::arg("tpsa"), py::arg("order") = int(gtpsa::mad::init::default_)
	    )
	.def(py::init<const gtpsa::tpsa&, const ord_t>(), tpsa_init_same_doc,
	     py::arg("tpsa"), py::arg("order") = int(gtpsa::mad::init::same))
	;

#define GTPSA_FUNC_ARG1(func)                                                                     \
    m.def(#func       , py::overload_cast<const gtpsa::tpsa&>              (&gtpsa:: func     )); \
    m.def(#func  "_"  , py::overload_cast<const gtpsa::tpsa&, gtpsa::tpsa*>(&gtpsa:: func ## _));
#include <gtpsa/funcs.h>

#undef GTPSA_FUNC_ARG1

    py::class_<CTpsaOp, std::shared_ptr<CTpsaOp>> ctpsa_with_op  (m, "_CTPSAWithOp");
    py::class_<gtpsa::ctpsa , std::shared_ptr<gtpsa::ctpsa>>  ctpsa (m, "ctpsa", ctpsa_with_op);
    AddMethods<gtpsa::ctpsa> ctpsa_m;
    ctpsa_m.add_methods<gtpsa::ctpsa, cnum_t>(ctpsa);
    ctpsa
	.def("set0",  &gtpsa::ctpsa::_set0)
	.def("setm",  &gtpsa::ctpsa::_setm)
	.def(py::self += std::complex<double>())
	.def(py::self -= std::complex<double>())
	.def(py::self *= std::complex<double>())
	.def(py::self /= std::complex<double>())

	.def(py::self + std::complex<double>())
	.def(py::self - std::complex<double>())
	.def(py::self * std::complex<double>())
	.def(py::self / std::complex<double>())

	.def(std::complex<double>() + py::self)
	.def(std::complex<double>() - py::self)
	.def(std::complex<double>() * py::self)
	.def(std::complex<double>() / py::self)

	.def(py::init<std::shared_ptr<gtpsa::mad::desc>, const ord_t>(), tpsa_init_desc_doc,
	     py::arg("tpsa"), py::arg("order") = int(gtpsa::mad::init::default_)
	    )
	.def(py::init<const gtpsa::tpsa&, const ord_t>(), tpsa_init_same_doc,
	     py::arg("tpsa"), py::arg("order") = int(gtpsa::mad::init::same))
	;
	;


#define GTPSA_FUNC_ARG1(func) \
   m.def(#func,       py::overload_cast<const gtpsa::ctpsa&               >(&gtpsa:: func     )); \
   m.def(#func  "_",  py::overload_cast<const gtpsa::ctpsa&, gtpsa::ctpsa*>(&gtpsa:: func ## _));
#include <gtpsa/funcs.h>
#undef GTPSA_FUNC_ARG1


    py::enum_<gtpsa::mad::init>(m, "init")
	.value("default", gtpsa::mad::init::default_)
	.value("same",    gtpsa::mad::init::same);

    /*
    tpsa
    .def("set",            py::overload_cast<cnum_t, cnum_t>( &gtpsa::tpsa::set))
    ;
    */


}
