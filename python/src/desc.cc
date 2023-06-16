#include "gtpsa_module.h"
#include <pybind11/stl.h>
#include <gtpsa/desc.hpp>

namespace py = pybind11;
namespace gpy = gtpsa::python;

void gpy::py_gtpsa_init_desc(py::module &m)
{
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
    py::class_<gtpsa::mad::desc  , std::shared_ptr<gtpsa::mad::desc>>   desc  (m, "desc" );
    desc
	.def_property_readonly("number_of_variables", &gtpsa::mad::desc::getNv  )
	.def_property_readonly("maximum_orders",      &gtpsa::mad::desc::maxOrd )
	.def_property_readonly("maximum_length",      &gtpsa::mad::desc::maxLen )
	// rename this mehtod
	.def("ordLen"         , &gtpsa::mad::desc::ordLen    )
	.def("truncate"       , &gtpsa::mad::desc::trunc     )
	//.def("index"        ,    &gtpsa::mad::desc::nxtbyvar )
	.def("nextByVariable" , &gtpsa::mad::desc::nxtbyvar  )
	.def("nextByOrder"    , &gtpsa::mad::desc::nxtbyord  )
	.def("mono"           , &gtpsa::mad::desc::mono      )
	.def("__repr__"       , &gtpsa::mad::desc::repr      )
	.def("isvalidsm"      , &gtpsa::mad::desc::isvalidsm )
	.def("indexsm"        , &gtpsa::mad::desc::idxsm     )
	.def("isvalid"        , py::overload_cast<const std::vector<ord_t>&>( &gtpsa::mad::desc::isvalid, py::const_ ))
	.def("index"          , py::overload_cast<const std::vector<ord_t>&>( &gtpsa::mad::desc::idx,     py::const_ ))
	.def(py::init<int, ord_t>(),             desc_newv_doc,
	     py::arg("nv"), py::arg("mo") = 0)
	.def(py::init<int, int, ord_t, ord_t>(), desc_newvp_doc,
	     py::arg("nv"), py::arg("np") = 0, py::arg("mo") = 0, py::arg("po") = 0)
	.def(py::init<int, int, std::vector<ord_t>, ord_t>(), desc_newvpo_doc,
	     py::arg("nv"), py::arg("np") = 0, py::arg("mo") = 0, py::arg("po") = 0);

}
