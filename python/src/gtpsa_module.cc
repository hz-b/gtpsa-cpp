#include <pybind11/pybind11.h>
#include "gtpsa_module.h"


PYBIND11_MODULE(_gtpsa, m) {
    m.doc() = "gtpsa python wrapper";

    py_gtpsa_init_desc(m);
    py_gtpsa_init_tpsa(m);
    py_gtpsa_init_ss_vect(m);
    py_gtpsa_init_arma(m);
}
