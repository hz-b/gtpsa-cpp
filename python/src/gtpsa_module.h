#ifndef _GTPSA_MODULE_H_
#define _GTPSA_MODULE_H_ 1

#include <pybind11/pybind11.h>
#include <gtpsa/python/name_index.h>

namespace gtpsa::python {
    void py_gtpsa_init_desc(pybind11::module &m);
    void py_gtpsa_init_tpsa(pybind11::module &m);
    void py_gtpsa_init_variant(pybind11::module &m);
    void py_gtpsa_init_ss_vect(pybind11::module &m);
    void py_gtpsa_init_arma(pybind11::module &m);

    /**
     *@brief  translate exception to pybind11 exception if received
     */
    const size_t mapping_index(const gtpsa::python::IndexMapping& im, const std::string key);

} // namespace gtpsa::python

#endif /* _GTPSA_MODULE_H_ */
