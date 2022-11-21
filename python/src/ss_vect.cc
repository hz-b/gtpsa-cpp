#include <gtpsa/ss_vect.h>
#include <gtpsa/ctpsa.hpp>
#include "gtpsa_module.h"
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <pybind11/complex.h>
#include <armadillo>

namespace py=pybind11;


/**
 * @brief convert a numpy matrix to a armadillo matrix
 *
 * using the buffer protocol
 *
 * It is not intended to leak armadillo arrays to python
 */
static arma::mat from_np_array(py::array_t<double, py::array::c_style|py::array::forcecast>& buffer) {
	/* Request a buffer descriptor from Python */
	py::buffer_info info = buffer.request();

	/* Some sanity checks ... */
	if (info.format != py::format_descriptor<double>::format())
		throw std::runtime_error("Incompatible format: expected a double array!");

	if (info.ndim != 2){
		std::stringstream strm;
		strm << "Incompatible buffer: expected 2 but got "
		     << info.ndim << "dimensions!";
		throw std::runtime_error(strm.str());
	}

	bool need_transpose = false;
	if(info.strides[0] != sizeof(double)){
		need_transpose = true;
	}
	/*
	  std::cerr << "Strides [" << info.strides[0] << ", " << info.strides[1] << "]"
	  << ": need_transpose " << std::boolalpha << need_transpose << std::endl;
	*/
	auto mat = arma::mat(static_cast<const double *>(info.ptr),
			     info.shape[0], info.shape[1]);
	if(need_transpose){
		arma::inplace_trans(mat);
	}
	return mat;
}

static py::array_t<double> from_arma_mat(arma::mat& mat)
{

    /* thought I could avoid the double copy */
    // auto mat_ptr = std::make_unique<arma::mat>(mat);

    auto result = py::array_t<double>(
	/* dimensions */
	{mat.n_rows, mat.n_cols},
	/*  strides */
	{
	    /* Strides (in bytes) for each index */
	    static_cast<py::ssize_t>(sizeof(double)),
	    static_cast<py::ssize_t>(sizeof(double) * mat.n_cols)
	},
	mat.memptr()
	);
    return result;
}



static const char init_ss_vect_doc [] = \
    "Initialise the space state vector using one vector argument as reference and its size";

static const char init_ss_vect_from_vec_doc [] = \
    "Initialise the space state vector providing a standard vector as argument";

static const char init_ss_vect_desc_doc [] = \
    "Initialise the space state vector with a description";

template<class WrappedClass, class P_MGR>
struct AddMethods
{


    template<typename T>
    void add_methods(py::class_<WrappedClass, P_MGR> &a_cls){
	a_cls
	    .def("set_zero",     &WrappedClass::set_zero)
	    .def("__str__",      &WrappedClass::pstr)
	    .def("__repr__",     &WrappedClass::repr)
	    .def("__len__",      &WrappedClass::size)
	    .def("cst_as_array", [](const WrappedClass& self) {
				    return py::array(py::cast(self.cst()));
				 })
	    .def("cst",          [](const WrappedClass& self) {
				     return gtpsa::ss_vect<double>(self.cst());
				 })
	    .def("__getitem__",  [](WrappedClass &self, const long int idx){
				    return self.at(idx);
				 })
	    .def("__setitem__",  [](WrappedClass &self, const long int idx, const T& v){
				     self.at(idx) = v;
				 })
	    .def("__copy__",     [](gtpsa::ss_vect<T> &self) {
				  return self.clone();
				 })
	    .def("copy",         [](gtpsa::ss_vect<T> &self) {
				  return self.clone();
				 })
	    .def("set_constant",  &WrappedClass::setConstant)
	    .def_property("name", &WrappedClass::name, &WrappedClass::setName)
	    .def(py::self += py::self)
	    .def(py::self -= py::self)
	    .def(py::self + py::self)
	    .def(py::self - py::self)
	    .def(py::init<const T&, const size_t> (), init_ss_vect_doc, py::arg("T"),
		 py::arg("dim") = gtpsa::ss_vect_n_dim)
	    .def(py::init<const std::vector<T>&> (), init_ss_vect_from_vec_doc)
	    .def(py::init<const std::shared_ptr<gtpsa::mad::desc>, const ord_t, const size_t> (),
		 init_ss_vect_desc_doc,
		 py::arg("desc"), py::arg("ord"), py::arg("dim") = gtpsa::ss_vect_n_dim
		)
	    .def(py::self += double())
	    .def(py::self -= double())
	    .def(py::self *= double())
	    .def(py::self /= double())

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
    void add_methods_tpsa(py::class_<WrappedClass, P_MGR> &a_cls){
	a_cls
	    .def("set_identity", &WrappedClass::set_identity)
	    .def("jacobian",     [](const WrappedClass& self) {
				     return py::array(
					 py::cast( self.jacobian() )
					);
				 })
	    .def("set_jacobian", [](WrappedClass& self, py::array_t<double, py::array::c_style|py::array::forcecast>& buffer){
				     arma::mat mat = from_np_array(buffer);
				     self.setJacobian(mat);
				 })
	;
    }
};


void py_gtpsa_init_ss_vect(py::module &m)
{
	typedef gtpsa::ss_vect<double> ss_vect_dbl_t;
	typedef gtpsa::ss_vect<gtpsa::tpsa> ss_vect_tpsa_t;


        py::class_<ss_vect_dbl_t, std::shared_ptr<ss_vect_dbl_t>> ss_vect_double (m, "ss_vect_double");
	AddMethods<ss_vect_dbl_t, std::shared_ptr<ss_vect_dbl_t>> double_cls;
	double_cls.add_methods<double>(ss_vect_double);

	/*
	typedef std::complex<double> dcplx;
	py::class_<gtpsa::ss_vect<dcplx>>  ss_vect_dcplx (m, "ss_vect_complex_double");
	AddMethods<gtpsa::ss_vect<dcplx>> dcplx_cls;
	dcplx_cls.add_methods<dcplx>(ss_vect_dcplx);
	*/

        py::class_<ss_vect_tpsa_t, std::shared_ptr<ss_vect_tpsa_t>>  ss_vect_tpsa   (m, "ss_vect_tpsa");
	AddMethods<ss_vect_tpsa_t, std::shared_ptr<ss_vect_tpsa_t>> tpsa_cls;
	tpsa_cls.add_methods<gtpsa::tpsa>(ss_vect_tpsa);
	tpsa_cls.add_methods_tpsa<gtpsa::tpsa>(ss_vect_tpsa);
	ss_vect_tpsa
	   .def(py::self += gtpsa::ss_vect<double>(0e0))
	   .def(py::self -= gtpsa::ss_vect<double>(0e0))
	   .def(py::self +  gtpsa::ss_vect<double>(0e0))
	   .def(py::self -  gtpsa::ss_vect<double>(0e0))
	   ;
	/*
        py::class_<gtpsa::ss_vect<gtpsa::ctpsa>> ss_vect_ctpsa  (m, "ss_vect_ctpsa");
	AddMethods<gtpsa::ss_vect<gtpsa::ctpsa>> ctpsa_cls;
	ctpsa_cls.add_methods<gtpsa::ctpsa>(ss_vect_ctpsa);
	ctpsa_cls.add_methods_tpsa<gtpsa::ctpsa>(ss_vect_ctpsa);
	*/
}
