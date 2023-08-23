#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <gtpsa/python/name_index.h>
#include "gtpsa_module.h"


namespace py = pybind11;
namespace gpy = gtpsa::python;

static const char named_index_init_doc[] =                              \
        "create a named index from a dictonary. contains indices for variables and parameters\n\
info will be used to augment the info text of raised exceptions";

void gpy::py_gtpsa_init_named_index(py::module &m)
{

        py::class_<gpy::IndexMapping, std::shared_ptr<IndexMapping>> index_mapping(m, "IndexMapping");
        index_mapping
                .def("index"     ,  &gpy::IndexMapping::index)
                .def("__dir__"    , &gpy::IndexMapping::pdir)
                .def("__getattr__", &gpy::IndexMapping::index)
                .def(py::init<const gpy::index_mapping_t&, const std::string, const bool>(),
                     named_index_init_doc,
                     py::arg("indices_dict") = gpy::default_mapping, py::arg("info") = "",
                     py::arg("check_default_keys") = true
                    )
                ;
}
