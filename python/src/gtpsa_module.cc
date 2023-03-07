#include <pybind11/pybind11.h>
#include "gtpsa_module.h"


namespace gpy = gtpsa::python;
namespace py = pybind11;

/*
 * translate exception to pybind11 exception if received
 */
const size_t gpy::mapping_index(const gpy::IndexMapping& im, const std::string key)
{
	try {
		return im.index(key);
	}
	catch(gpy::key_error& ke) {
		throw py::key_error(ke.what());
	}

}

PYBIND11_MODULE(_gtpsa, m) {
    m.doc() = "gtpsa python wrapper";

    gpy::py_gtpsa_init_desc(m);
    gpy::py_gtpsa_init_tpsa(m);
    gpy::py_gtpsa_init_variant(m);
    gpy::py_gtpsa_init_ss_vect(m);
    gpy::py_gtpsa_init_arma(m);
}
