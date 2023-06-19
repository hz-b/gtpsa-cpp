#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <pybind11/complex.h>
#include <pybind11/operators.h>
#include <gtpsa/ctpsa.hpp>
#include <gtpsa/tpsa.hpp>
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

#include "gtpsa_module.h"

namespace py = pybind11;
namespace gpy = gtpsa::python;

static const char tpsa_init_same_doc[] = "Create a new (c)tpsa object with the same properties as this one\n\
Use clone to create a copy of the content too. \n";

static const char tpsa_init_desc_doc[] = "Create a new (c)tpsa object using the passed description object. Default order\
 will be used if none is speficied\n\
Use clone to create a copy of the content too. \n";


template<class Cls, typename T>
static void set_variable(Cls& inst, const T& v, idx_t i, const T& s, const bool check_first)
{
    auto nv = inst.getDescription()->getNv();
    if(check_first) {
	if(i <= 0){
	    std::stringstream strm;
	    strm << "index of variable must be at least 1, but was " << i;
	    throw std::runtime_error(strm.str());
	}
	if(i > nv){
	    std::stringstream strm;
	    strm << "index of derivative must not exceed number of variables (" << nv << ") but was " << i;
	    throw std::runtime_error(strm.str());
	}
    }
    inst.setVariable(v, i, s);
}

template<class Cls, typename T>
static void set_variable(Cls& inst, const T& v, const std::string& var_name, const T& s, const gpy::IndexMapping& im, bool check_first)
{
    const auto index = im.index(var_name);
    /* seems that variables start to count at 1 not at zero */
    set_variable(inst, v, index + 1, s, check_first);
}

static const std::vector<ord_t> convert_size_t_to_ord_t(const std::vector<size_t>& m)
{
    std::vector<ord_t> v(m.size());
    /* should include check ... */
    std::transform(m.begin(), m.end(), v.begin(), [](size_t v){return ord_t(v);});
    return v;
}

template<class Cls>
static auto check_index(const Cls& inst, const std::vector<ord_t>& m)
{
    auto index = inst.index(m);
    if(index < 0){
	throw std::runtime_error("index out of range");
    }
    return index;
}


template<class Cls, typename T>
static void set(Cls& inst, const gpy::index_mapping& powers, const T& a, const T& b, const gpy::IndexMapping& im, const bool check_first)
{
    const auto tmp =  im.order_vector_from_power(powers);
    const auto p = convert_size_t_to_ord_t(tmp);
    if(check_first){
	check_index(inst, p);
    }
    inst.set(p, a, b);
}

template<class Cls, typename T>
static inline T get(const Cls& inst, const gpy::index_mapping& powers, const gpy::IndexMapping& im, const bool check_first)
{
    // std::cerr << "get with mapping" << std::endl;
    const auto tmp =  im.order_vector_from_power(powers);
    // std::cerr << "order {";
    // for(const auto t: tmp) {std::cerr << t << " , ";}
    // std::cerr << "}";
    const auto p = convert_size_t_to_ord_t(tmp);
    if(check_first) {
	check_index(inst, p);
    }
    // std::cerr << "get value";
    return inst.get(p);
}

struct coefficient_entry_double {
    ord_t order;
    double val;
    idx_t index;
};

struct coefficient_entry_complex {
    //std::vector<ord_t> order;
    ord_t order;
    std::complex<double> cval;
    idx_t index;
};


#if 0
//template<class Cls>
static inline void coefficient_arrays(const gtpsa::tpsa& inst)
{
    const auto coeffs = inst.getCoefficients();
    if(coeffs.size() <= 0){
	throw std::runtime_error("get cofficients returned no coefficients");
    }

    const auto& tup = coeffs[0];
    std::vector<ord_t> ord = std::get<0>(tup);
    const auto value = std::get<1>(tup);
    const auto index = std::get<2>(tup);

    py::array_t<ord_t> orders_a(
	{coeffs.size(), tup.size()} // shape
	);

}
#endif

template<class Cls>
struct AddMethods
{
    template<typename BCls, typename T>
    void add_methods_gtpsa_mad(py::class_<BCls> a_cls) {


	a_cls
	    .def("length",          &Cls::length)
	    .def("get_description", &Cls::getDescription)
	    .def("clear",           &Cls::clear)
	    .def("is_null",         &Cls::isNull)
	    //.def("mono",            &Cls::mono)
	    .def("get",             [](const Cls& inst){
					return inst.get();
	    })
	    .def("get",             [](const Cls& inst, const std::vector<ord_t>& m, const bool check_first){
		if(check_first) {
		    check_index(inst, m);
		}
		return inst.get(m);
	    }, "get coefficient at given powers", py::arg("vector of orders"), py::arg("check_index")=true)
	    .def("get",             [](const Cls& inst, const gpy::index_mapping& powers, const bool check_first){
		return get<Cls, T>(inst, powers, gpy::DefaultIndexMapping, check_first);
	    },
		"get coefficient at given powers, specify powers in the dictionary",
		py::arg("dict of no zero order"), py::arg("check_index")=true
		)
	    .def("get_coefficients", &Cls::getCoefficients)
	    .def("set",             [](Cls& inst,      const std::vector<ord_t>& m, const T& a, const T& b){
	      const bool check_first = true;
		if(check_first) {
		    check_index(inst, m);
		}
		inst.set(m, a, b);
	    })
	  /*
	    .def("set",             [](Cls& inst,      const gpy::index_mapping& p, const T& a, const T& b, const bool check_first){
		set(inst, p, a, b, gpy::DefaultIndexMapping, check_first);
	    })
	  */
	    .def("set",             [](Cls& inst,                                   T a, T b){
		inst.set(a, b);
	    })
	    .def("index",          [](const Cls& inst, const std::vector<ord_t>& m){ return inst.index(m);})
	    // make it more pythonic!
	    .def("getv",           [](const Cls& inst, idx_t i){
				       std::vector<T> tmp(inst.length()); inst.getv(i, &tmp);
				       return py::array(py::cast(tmp));
				   })
	    .def("setv",           &Cls::setv)
	    .def("getsm",          &Cls::getsm)
	    .def("get_coefficients", [](const Cls& inst) {})
	    .def("set_variable",  [](Cls& inst, const T& v, idx_t i, const T& s, const bool check_first){
		set_variable(inst, v, i, s, check_first);
	    },
		"set the variable to value and gradient at index of variable to 1. v:= scale * this->v + value",
		py::arg("value"), py::arg("index_of_variable") = 0, py::arg("scale") = 0, py::arg("check_first") = true)
	    .def("set_variable",  [](Cls& inst, const T& v, const std::string& var_name, const T& s, const bool check_first){
		set_variable(inst, v, var_name, s, gpy::DefaultIndexMapping, check_first);
	    },
		"set the variable to value and gradient at index of variable_name to 1. . v:= scale * this->v + value",
		py::arg("value"), py::arg("variable_name"), py::arg("scale") = 0, py::arg("check_first") = true)
	    .def("print", [](const Cls& inst, std::string name, double eps, bool nohdr){
                FILE* f = stdout;
                inst.print(name.c_str(), eps, nohdr, f);
	    },
		"print the cofficients to stdout using c's stdout",
		py::arg("name") = "", py::arg("eps") = 1e-30 , py::arg("nohdr") = false)
	    .def_property("name",  &Cls::name, &Cls::setName)
	    .def_property("uid",   [](Cls& inst){ return inst.uid(0);}, &Cls::uid)
	    .def_property_readonly("order", &Cls::order)
	    .def(py::init<std::shared_ptr<gtpsa::mad::desc>, const ord_t>(), tpsa_init_desc_doc,
		 py::arg("tpsa"), py::arg("order") = int(gtpsa::mad::init::default_)
		)
#if 0
#endif
	    .def(py::init<const BCls&, const ord_t>(), tpsa_init_same_doc,
		 py::arg("tpsa"), py::arg("order") = int(gtpsa::mad::init::same))
	    ;
    }

    template<typename BCls, typename T>
    void add_methods_ops(py::class_<BCls> a_cls) {
	a_cls
	    .def("__repr__", &Cls::repr)
	    .def("__str__" , &Cls::pstr)
	    .def("__copy__", &Cls::clone)
	    .def("copy",     &Cls::clone)
	    .def(py::self += py::self)
	    .def(py::self -= py::self)
	    .def(py::self *= py::self)
	    .def(py::self /= py::self)

	    .def(-py::self )
	    .def(py::self +  py::self)
	    .def(py::self -  py::self)
	    .def(py::self *  py::self)
	    .def(py::self /  py::self)

	    .def(py::self +  T()     )
	    .def(py::self -  T()     )
	    .def(py::self *  T()     )
	    .def(py::self /  T()     )


	    .def(T()      +  py::self)
	    .def(T()      -  py::self)
	    .def(T()      *  py::self)
	    .def(T()      /  py::self)

	    .def("__pow__", [](BCls& inst, const int    n) { return gtpsa::pow(inst, n); })
	    .def("__pow__", [](BCls& inst, const T      v) { return gtpsa::pow(inst, v); })
	    ;
    }

    template<typename BCls, typename T>
    void add_methods(py::class_<BCls> a_cls) {
	add_methods_gtpsa_mad<BCls, T>(a_cls);
	add_methods_ops<BCls, T>(a_cls);
    }
};


void gpy::py_gtpsa_init_tpsa(py::module &m)
{


    PYBIND11_NUMPY_DTYPE(coefficient_entry_double, order, val, index);
    //PYBIND11_NUMPY_DTYPE(coefficient_entry_complex, order, val, index);

    typedef gtpsa::TpsaWithOp<gtpsa::TpsaTypeInfo>   TpsaOp;
    typedef gtpsa::TpsaWithOp<gtpsa::CTpsaTypeInfo> CTpsaOp;

    py::class_<TpsaOp, std::shared_ptr<TpsaOp>> tpsa_with_op  (m, "_TPSAWithOp");
    AddMethods<TpsaOp> tpsa_m_op;
    tpsa_m_op.add_methods<TpsaOp, num_t>(tpsa_with_op);
    py::class_<gtpsa::tpsa, std::shared_ptr<gtpsa::tpsa>>   tpsa  (m, "tpsa",  tpsa_with_op);
    AddMethods<gtpsa::tpsa> tpsa_m;
    tpsa_m.add_methods_ops<gtpsa::tpsa, num_t>(tpsa);
    tpsa
	//.def("set", py::overload_cast<num_t, num_t>( &gtpsa::tpsa::set))
	//.def("set", py::overload_cast<const std::vector<ord_t>&, num_t, num_t>( &gtpsa::tpsa::set))
	.def(py::self += double())
	.def(py::self -= double())
	.def(py::self *= double())
	.def(py::self /= double())
	//
	.def(py::self + double())
	.def(py::self - double())
	.def(py::self * double())
	.def(py::self / double())
	//
	.def(double() + py::self)
	.def(double() - py::self)
	.def(double() * py::self)
	.def(double() / py::self)
	.def(py::init<std::shared_ptr<gtpsa::mad::desc>, const ord_t>(), tpsa_init_desc_doc,
	     py::arg("tpsa"), py::arg("order") = int(gtpsa::mad::init::default_)
	    )
	.def(py::init<const gtpsa::tpsa&, const ord_t>(), tpsa_init_same_doc,
	     py::arg("tpsa"), py::arg("order") = int(gtpsa::mad::init::same))
	;

#define GTPSA_FUNC_ARG1(func)                                                                     \
    m.def(#func       , py::overload_cast<const gtpsa::tpsa&>              (&gtpsa:: func     )); \
    m.def(#func  "_"  , py::overload_cast<const gtpsa::tpsa&, gtpsa::tpsa*>(&gtpsa:: func ## _));
#include <gtpsa/funcs.h>

#undef GTPSA_FUNC_ARG1

    py::class_<CTpsaOp, std::shared_ptr<CTpsaOp>> ctpsa_with_op  (m, "_CTPSAWithOp");
    py::class_<gtpsa::ctpsa , std::shared_ptr<gtpsa::ctpsa>>  ctpsa (m, "ctpsa", ctpsa_with_op);
    AddMethods<gtpsa::ctpsa> ctpsa_m;
    ctpsa_m.add_methods<gtpsa::ctpsa, std::complex<double>>(ctpsa);
    ctpsa
    .def("set0",  [](gtpsa::ctpsa& t, const std::complex<double> a, const std::complex<double> b) {
      t.set(a, b);
    })
    .def("set0",  [](gtpsa::ctpsa& t, const double a, const std::complex<double> b) {
      std::complex<double> tmpa = {a, 0};
      t.set(tmpa, b);
    })
    .def("set",  [](gtpsa::ctpsa& t, const double a, const std::complex<double> b) {
      std::complex<double> tmpa = {a, 0};
      t.set(tmpa, b);
    })
    .def("setv",  [](gtpsa::ctpsa& t, int m, const std::vector<std::complex<double>>& c) {
	t.setv(m, c);
    })
    .def("setm",  &gtpsa::ctpsa::_setm)
    .def("real", [](const gtpsa::ctpsa& t) -> gtpsa::tpsa { return t.real();}, "return real part (newly allocated object)")
    .def("imag", [](const gtpsa::ctpsa& t) -> gtpsa::tpsa { return t.imag();}, "return imaginary part (newly allocated object)")
    .def("real", [](const gtpsa::ctpsa& t, gtpsa::tpsa *re) { t.real(re);}, "place real part in passed object re")
    .def("imag", [](const gtpsa::ctpsa& t, gtpsa::tpsa *im) { t.real(im);}, "place imaginary part in passed object im")
    .def("polar", &gtpsa::ctpsa::polar, "polar variables ")
    .def("unit",  &gtpsa::ctpsa::unit, "z / abs(z)")
    .def("abs", [](const gtpsa::ctpsa& t) -> gtpsa::tpsa { return t.abs();}, "abs(z)")
    .def("arg", [](const gtpsa::ctpsa& t) -> gtpsa::tpsa { return t.abs();}, "arg(z)")

	.def(py::self += std::complex<double>())
	.def(py::self -= std::complex<double>())
	.def(py::self *= std::complex<double>())
	.def(py::self /= std::complex<double>())

	.def(py::self + std::complex<double>())
	.def(py::self - std::complex<double>())
	.def(py::self * std::complex<double>())
	.def(py::self / std::complex<double>())

	.def(std::complex<double>() + py::self)
	.def(std::complex<double>() - py::self)
	.def(std::complex<double>() * py::self)
	.def(std::complex<double>() / py::self)

	.def(py::init<std::shared_ptr<gtpsa::mad::desc>, const ord_t>(), tpsa_init_desc_doc,
	     py::arg("tpsa"), py::arg("order") = int(gtpsa::mad::init::default_)
	    )
	.def(py::init<const gtpsa::tpsa&, const ord_t>(), tpsa_init_same_doc,
	     py::arg("tpsa"), py::arg("order") = int(gtpsa::mad::init::same))
	.def(py::init<const gtpsa::tpsa&, const gtpsa::tpsa&>(), "init from real and imag",
	     py::arg("real"), py::arg("imag"))
	;



#define GTPSA_FUNC_ARG1(func) \
   m.def(#func,       py::overload_cast<const gtpsa::ctpsa&               >(&gtpsa:: func     )); \
   m.def(#func  "_",  py::overload_cast<const gtpsa::ctpsa&, gtpsa::ctpsa*>(&gtpsa:: func ## _));
#include <gtpsa/funcs.h>
#undef GTPSA_FUNC_ARG1


    py::enum_<gtpsa::mad::init>(m, "init")
	.value("default", gtpsa::mad::init::default_)
	.value("same",    gtpsa::mad::init::same);

    /*
    tpsa
    .def("set",            py::overload_cast<cpx_t, cpx_t>( &gtpsa::tpsa::set))
    ;
    */


}
