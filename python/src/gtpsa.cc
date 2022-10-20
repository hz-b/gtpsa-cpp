#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <pybind11/operators.h>
#include <gtpsa/desc.hpp>
#include <gtpsa/tpsa.hpp>
#include <gtpsa/ctpsa.hpp>
#include <string>
#include <sstream>

namespace py = pybind11;

template<class Cls>
struct AddMethods
{
    template<typename BCls, typename T>
    void add_methods_gtpsa_mad(py::class_<BCls> a_cls) {
	a_cls
	    .def("length",         &Cls::len)
	    .def("getDescription", &Cls::getDescription)
	    .def("clear",          &Cls::clear)
	    .def("isNull",         &Cls::isNull)
	    .def("mono",           &Cls::mono)
	    .def("index",          py::overload_cast<const std::vector<ord_t>&>( &Cls::idx, py::const_))
	    .def("get",            py::overload_cast<>( &Cls::get, py::const_))
	    .def("get",            py::overload_cast<const std::vector<ord_t>&>( &Cls::get, py::const_))
	    // make it more pythonic!
	    .def("getv",           [](const Cls& inst, idx_t i){ std::vector<T> tmp(inst.len()); inst.getv(i, &tmp); return tmp; })
	    .def("setv",           &Cls::setv)
	    .def("getsm",          &Cls::getsm)
	    // problems with complex values ....
	    // .def("set",            py::overload_cast<const std::vector<ord_t>&, T, T>( &Cls::set))
	    .def("setVariable",    &Cls::setVariable, "set the variable?",
		 py::arg("v"), py::arg("iv") = 0, py::arg("scl") = 0)
	    .def_property("name",  &Cls::name, &Cls::setName)
	    .def_property("uid",   [](Cls& inst){ return inst.uid(0);}, &Cls::uid)

	    .def_property_readonly("order", &Cls::ord)
	    .def(py::init<std::shared_ptr<gtpsa::desc>, const ord_t>())
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

	    .def("__repr__", &Cls::repr)
	    .def("__str__",  &Cls::pstr)
	    ;
    }

    template<typename BCls, typename T>
    void add_methods(py::class_<BCls> a_cls) {
	add_methods_gtpsa_mad<BCls, T>(a_cls);
	add_methods_ops<BCls, T>(a_cls);
    }
};

PYBIND11_MODULE(lib_gtpsa, m) {
    m.doc() = "gtpsa python wrapper";

    const char desc_newv_doc[] = "\
Args:\n\
    nv: number of variables\n\
    mo: maximum order\n\
	";

    const char desc_newvp_doc[] = "\
Args:\n\
    nv: number of variables\n\
    np: number of parameters?\n\
    mo: maximum order (of variables)?\n\
    po: maximum order (of parameters)?\n\
	";

    const char desc_newvpo_doc[] = "\
Args:\n\
    nv: number of variables\n\
    np: number of parameters?\n\
    no: vector of orders?\n\
    po: maximum order (of parameters)?\n\
";
    py::class_<gtpsa::desc  , std::shared_ptr<gtpsa::desc>>   desc  (m, "desc" );
    desc
	.def_property_readonly("number_of_variables", &gtpsa::desc::getNv  )
	.def_property_readonly("maximum_orders",      &gtpsa::desc::maxOrd )
	.def_property_readonly("maximum_length",      &gtpsa::desc::maxLen )
	// rename this mehtod
	.def("ordLen"         , &gtpsa::desc::ordLen    )
	.def("truncate"       , &gtpsa::desc::trunc     )
	//.def("index"        ,    &gtpsa::desc::nxtbyvar )
	.def("nextByVariable" , &gtpsa::desc::nxtbyvar  )
	.def("nextByOrder"    , &gtpsa::desc::nxtbyord  )
	.def("mono"           , &gtpsa::desc::mono      )
	.def("__repr__"       , &gtpsa::desc::repr      )
	.def("isvalidsm"      , &gtpsa::desc::isvalidsm )
	.def("indexsm"        , &gtpsa::desc::idxsm     )
	.def("isvalid"        , py::overload_cast<const std::vector<ord_t>&>( &gtpsa::desc::isvalid, py::const_ ))
	.def("index"          , py::overload_cast<const std::vector<ord_t>&>( &gtpsa::desc::idx, py::const_     ))
	.def(py::init<int, ord_t>(),             desc_newv_doc,
	     py::arg("nv"), py::arg("mo") = 0)
	.def(py::init<int, int, ord_t, ord_t>(), desc_newvp_doc,
	     py::arg("nv"), py::arg("np") = 0, py::arg("mo") = 0, py::arg("po") = 0)
	.def(py::init<int, int, std::vector<ord_t>, ord_t>(), desc_newvpo_doc,
	     py::arg("nv"), py::arg("np") = 0, py::arg("mo") = 0, py::arg("po") = 0);

    py::class_<gtpsa::TPSAWithOp, std::shared_ptr<gtpsa::TPSAWithOp>> tpsa_with_op  (m, "_TPSAWithOp");
    AddMethods<gtpsa::TPSAWithOp> tpsa_m;
    tpsa_m.add_methods<gtpsa::TPSAWithOp, num_t>(tpsa_with_op);
    py::class_<gtpsa::tpsa, std::shared_ptr<gtpsa::tpsa>>   tpsa  (m, "tpsa",  tpsa_with_op);
    tpsa
	.def("set", py::overload_cast<num_t, num_t>( &gtpsa::tpsa::set))
	.def("set", py::overload_cast<const std::vector<ord_t>&, num_t, num_t>( &gtpsa::tpsa::set))
	.def(py::self += double())
	.def(py::self -= double())
	.def(py::self *= double())
	.def(py::self /= double())

	.def(py::self + double())
	.def(py::self - double())
	.def(py::self * double())
	.def(py::self / double())

	.def(double() + py::self)
	.def(double() - py::self)
	.def(double() * py::self)
	.def(double() / py::self)
	.def(py::init<std::shared_ptr<gtpsa::desc>, const ord_t>())
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
