#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "gtpsa_module.h"


PYBIND11_MODULE(_gtpsa_variant_test, m)
{
    m.doc() = "gtpsa variant test module python wrapper";

    py_gtpsa_init_variant(m);
}
