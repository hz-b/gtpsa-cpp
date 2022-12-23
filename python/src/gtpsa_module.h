#ifndef _GTPSA_MODULE_H_
#define _GTPSA_MODULE_H_ 1

#include <pybind11/pybind11.h>

void py_gtpsa_init_desc(pybind11::module &m);
void py_gtpsa_init_tpsa(pybind11::module &m);
void py_gtpsa_init_ss_vect(pybind11::module &m);
void py_gtpsa_init_arma(pybind11::module &m);

#endif /* _GTPSA_MODULE_H_ */
