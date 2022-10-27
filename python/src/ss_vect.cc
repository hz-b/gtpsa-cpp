#include <gtpsa/ss_vect.h>
#include <gtpsa/ctpsa.hpp>
#include "gtpsa_module.h"
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <pybind11/complex.h>

namespace py=pybind11;

static const char init_ss_vect_doc [] = \
    "Initialise the space state vector";

static const char init_ss_vect_desc_doc [] = \
    "Initialise the space state vector with a description";

template<class WrappedClass>
struct AddMethods
{


    template<typename T>
    void add_methods(py::class_<WrappedClass> &a_cls){
	a_cls
	    .def("setZero",   &WrappedClass::set_zero)
	    .def("__str__",   &WrappedClass::pstr)
	    .def("__repr__",  &WrappedClass::repr)
	    .def("__len__",     &WrappedClass::size)
	    .def("cst",         [](const WrappedClass& self) {
				    return py::array(py::cast(self.cst()));
				})
	    .def("__getitem__", [](WrappedClass &self, const long int idx){
				    return self.at(idx);
				})
	    .def("__setitem__", [](WrappedClass &self, const long int idx, const T& v){
				    self.at(idx) = v;
				})
	    .def("__copy__",  [](gtpsa::ss_vect<T> &self) {
				  throw std::runtime_error("copy not (yet) implemented");
				      //return gtpsa::ss_vect<T>(self);
			      })

	    .def(py::self += py::self)
	    .def(py::self -= py::self)
	    .def(py::self + py::self)
	    .def(py::self - py::self)
	    .def(py::init<const T&, const size_t> (), init_ss_vect_doc, py::arg("T"),
		 py::arg("dim") = gtpsa::ss_vect_n_dim)
	    .def(py::init<const std::shared_ptr<gtpsa::desc>, const ord_t, const size_t> (),
		 init_ss_vect_desc_doc,
		 py::arg("desc"), py::arg("ord"), py::arg("dim") = gtpsa::ss_vect_n_dim
		)
	    .def(py::self  += double())
	    .def(py::self  -= double())
	    .def(py::self  *= double())
	    .def(py::self  /= double())

	    .def(py::self  + double())
	    .def(py::self  - double())
	    .def(py::self  * double())
	    .def(py::self  / double())

	    //.def(double()  + py::self)
	    //.def(double()  - py::self)
	    //.def(double()  * py::self)
	    //.def(double()  / py::self)
	    // .def( gtpsa::ss_vect<double>() - py::self)
	    ;
    }
    template<typename T>
    void add_methods_tpsa(py::class_<WrappedClass> &a_cls){
	a_cls
	    .def("setIdentity",   &WrappedClass::set_identity)
	    .def("jacobian",      &WrappedClass::jacobian)
	;
    }
};


void py_gtpsa_init_ss_vect(py::module &m)
{
	py::class_<gtpsa::ss_vect<double>>       ss_vect_double (m, "ss_vect_double");
	AddMethods<gtpsa::ss_vect<double>> double_cls;
	double_cls.add_methods<double>(ss_vect_double);

        py::class_<gtpsa::ss_vect<gtpsa::tpsa>>  ss_vect_tpsa   (m, "ss_vect_tpsa");
	AddMethods<gtpsa::ss_vect<gtpsa::tpsa>> tpsa_cls;
	tpsa_cls.add_methods<gtpsa::tpsa>(ss_vect_tpsa);
	tpsa_cls.add_methods_tpsa<gtpsa::tpsa>(ss_vect_tpsa);

        py::class_<gtpsa::ss_vect<gtpsa::ctpsa>> ss_vect_ctpsa  (m, "ss_vect_ctpsa");
	// py::class_<gtpsa::ss_vect<gtpsa::tpsa>> ss_vect_tpsa(m, "ss_vect_double");
	//py::class_<gtpsa::ss_vect<gtpsa::ctpsa> ss_vect_ctpsa(m, "ss_vect_double");

}
