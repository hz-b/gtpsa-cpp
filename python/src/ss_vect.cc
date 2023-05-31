#include <armadillo>
#include <gtpsa/ctpsa.hpp>
//#include <gtpsa/python/name_index.h>
#include <gtpsa/ss_vect.h>
#include "gtpsa_module.h"
#include "gtpsa_wrapper.h"
#include <gtpsa/python/objects_with_named_index.h>
#include <pybind11/complex.h>
#include <pybind11/operators.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

namespace py = pybind11;
namespace gpy = gtpsa::python;


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



namespace gtpsa::python{

    // support loc
    template<typename T>
    class ss_vect_list_access_loc : public ss_vect_list_access<T> {
        /* just for pybind type system */
        using capsule_ptr = typename ss_vect_list_access<T>::capsule_ptr;
    public:
        ss_vect_list_access_loc(capsule_ptr p_inst)
	    : ss_vect_list_access<T>(p_inst)
            {}
    };

    // support iloc
    template<typename T>
    class ss_vect_list_access_iloc : public ss_vect_list_access<T> {
        /* just for pybind type system */
        using capsule_ptr = typename ss_vect_list_access<T>::capsule_ptr;
    public:
        ss_vect_list_access_iloc(capsule_ptr p_inst)
	 : ss_vect_list_access<T>(p_inst)
	{}
    };



    /*
     * Support a .loc["x"] access to the elements
      */
    template<class WrappedClass, class P_MGR, typename T>
    static void add_methods_list_access_loc_getitem(pybind11::class_<WrappedClass, P_MGR>& inst)
    {
        inst
	    .def("__getitem__",  [](const WrappedClass &self, const std::string name){
		auto idx = self.getVectorPtr()->getMapping()->index(name);
		return self.getVectorPtr()->at(idx);
	        }, py::keep_alive<0, 1>()
		)
            ;
    }

    /*
     * review if __len__ should not be part of the getitem
     */
    template<class WrappedClass, class P_MGR, typename T>
    static void add_methods_list_access_loc(pybind11::class_<WrappedClass, P_MGR>& inst)
    {
        inst
                .def("__len__",      [](const WrappedClass &self){
                    // std::cerr <<  "template attribute access, self = " << &self << "  getting length ....";
                    const auto p_vec = self.getVectorPtr();
                    // std::cerr <<  "p_vec = " << p_vec << ", len(p_vec) = ";
                    const auto tmp =  p_vec->size();
                    // std::cerr << " =  "  << tmp << std::endl;
                    return tmp;
                })
                .def("__setitem__",  [](const WrappedClass &self, const std::string name, const T& v){
                    auto idx = self.getVectorPtr()->getMapping()->index(name);
                    self.getVectorPtr()->at(idx) = v;
                })
                ;
    }

    template<class WrappedClass, class P_MGR, typename T>
    static void add_methods_list_access_setitem(pybind11::class_<WrappedClass, P_MGR>& inst)
    {
        inst
            .def("__setitem__",  [](WrappedClass &self, const long int idx, const T& v){
                                    self.getVectorPtr()->at(idx) = v;
                                 })
            ;
    }
    /*
     * Support a .iloc["x"] access to the elements
    */
    template<class WrappedClass, class P_MGR, typename T>
    static void add_methods_list_access(pybind11::class_<WrappedClass, P_MGR>& inst)
    {
        inst
            .def("__len__",      [](const WrappedClass &self){
                // std::cerr <<  "template list self = " << &self << "  getting length ....";
                const auto p_vec = self.getVectorPtr();
                // std::cerr <<  "p_vec = " << p_vec << ", len(p_vec) = ";
                const auto tmp = p_vec->size();
                // std::cerr << " =  "  << tmp << std::endl;
                return tmp;
            });
	    add_methods_list_access_setitem<WrappedClass, P_MGR, T>(inst);
    }

    template<class WrappedClass, class P_MGR, typename T>
    static void add_methods_list_access_getitem(pybind11::class_<WrappedClass, P_MGR>& inst)
    {
        inst
            .def("__getitem__",  [](const WrappedClass &self, const long int idx){
                // std::cerr <<  "template list self = " << &self << "  getting item " << idx << " ";
                // std::cerr.flush();
                const auto p_vec = self.getVectorPtr();
                // std::cerr <<  "p_vec = " << p_vec << ", idx = " << idx;
                auto t =  p_vec->at(idx);
                // std::cerr << "p_vec[" << idx <<"] = " << t;
                // std::cerr << ", c++ side done" << std::endl;
		return t;
            },  py::keep_alive<1, 0>()
                )
	    ;
    }

}; // namespace gtpsa::python


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
/*
 * do not activate before proper check ... the methods are added to classes
 * without able to manage their index mapping
           .def("__dir__",      [](const WrappedClass& self){
                                        return self.getMapping().pdir();
                                 })
        .def("__getattr__",  [](const WrappedClass& self, const std::string& key){
                                    return self.at(gpy::mapping_index(gpy::DefaultIndexMapping, key));
                                 })
            .def("__setattr__",  [](      WrappedClass& self, const std::string& key, T& v){
                                    self.at(gpy::mapping_index(gpy::DefaultIndexMapping, key)) = v;
                                 })
*/
            .def("cst_as_array", [](const WrappedClass& self) {
                                    return py::array(py::cast(self.cst()));
                                 })
            .def("cst",          [](const WrappedClass& self) {
		return gtpsa::ss_vect<double>(self.cst());
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

        .def("rcompose", &WrappedClass::rcompose)
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
                arma::mat res = self.jacobian();
                return py::array(
                    py::cast( res )
                    );
            })
            .def("set_jacobian", [](WrappedClass& self, py::array_t<double, py::array::c_style|py::array::forcecast>& buffer){
                arma::mat mat = from_np_array(buffer);
                self.setJacobian(mat);
            })
            .def("hessian",     [](const WrappedClass& self) {
                return py::array(
                        py::cast( self.hessian() )
                );
            })
        ;
    }
};


namespace gtpsa::python {
} // gtpsa:: python

void gpy::py_gtpsa_init_ss_vect(py::module &m)
{
    typedef gtpsa::ss_vect<double> ss_vect_dbl_t;
    typedef gtpsa::ss_vect<gtpsa::tpsa> ss_vect_tpsa_t;

    typedef gtpsa::TpsaWithOp<gtpsa::TpsaTypeInfo>   TpsaOp;
    typedef gtpsa::TpsaWithOp<gtpsa::CTpsaTypeInfo> CTpsaOp;

    /* access using named index */
    typedef gtpsa::python::StateSpaceWithNamedIndex<double> ss_vect_dbl_py_t;
    typedef gtpsa::python::StateSpaceWithNamedIndex<gtpsa::tpsa> ss_vect_tpsa_py_t;

    /* .loc like access ... typing */
    typedef gtpsa::python::ss_vect_list_access_loc<double>	ss_vect_dbl_list_access_loc_t;
    typedef gtpsa::python::ss_vect_list_access_loc<gtpsa::tpsa> ss_vect_tpsa_list_access_loc_t;

    /* .iloc, would then autmatically use python buffer protocol using array method*/
    typedef gtpsa::python::ss_vect_list_access_iloc<double>	 ss_vect_dbl_list_access_iloc_t;
    typedef gtpsa::python::ss_vect_list_access_iloc<gtpsa::tpsa> ss_vect_tpsa_list_access_iloc_t;

    /* access to the underlying elements */
    typedef gtpsa::python::ss_vect_list_access_loc<double>	ss_vect_dbl_list_access_loc_t;
    typedef gtpsa::python::ss_vect_list_access_loc<gtpsa::tpsa> ss_vect_tpsa_list_access_loc_t;

    /* pandas .iloc like access */
    py::class_<ss_vect_dbl_list_access_iloc_t, std::shared_ptr<ss_vect_dbl_list_access_iloc_t>> ss_vect_double_list_access_iloc (m, "_ss_vect_double_list_access_iloc");
    add_methods_list_access<ss_vect_dbl_list_access_iloc_t, std::shared_ptr<ss_vect_dbl_list_access_iloc_t>, double>(ss_vect_double_list_access_iloc);
    add_methods_list_access_getitem<ss_vect_dbl_list_access_iloc_t, std::shared_ptr<ss_vect_dbl_list_access_iloc_t>, double>(ss_vect_double_list_access_iloc);
    /* pandas .iloc like access */
    py::class_<ss_vect_tpsa_list_access_iloc_t, std::shared_ptr<ss_vect_tpsa_list_access_iloc_t>> ss_vect_tpsa_list_access_iloc (m, "_ss_vect_tpsa_list_access_iloc");
    add_methods_list_access<ss_vect_tpsa_list_access_iloc_t, std::shared_ptr<ss_vect_tpsa_list_access_iloc_t>, gtpsa::tpsa>(ss_vect_tpsa_list_access_iloc);
   //add_methods_list_access_setitem<ss_vect_tpsa_list_access_iloc_t, std::shared_ptr<ss_vect_tpsa_list_access_iloc_t>, ss_vect_tpsa_elem_access_t>(ss_vect_tpsa_list_access_iloc);
    ss_vect_tpsa_list_access_iloc
	.def("__getitem__",  [](ss_vect_tpsa_list_access_iloc_t &self, const size_t idx){
	    std::cerr << "returning ss_vect tpsa access with index " << idx << std::endl;
	    return ss_vect_tpsa_elem_access_t(self.getVectorPtr(), idx);
	})
	.def("__setitem__",  [](ss_vect_tpsa_list_access_iloc_t &self, const size_t idx, const ss_vect_tpsa_elem_access_t& o){
	    std::cerr << "adding ss_vect tpsa access with index " << idx << std::endl;
	    self.getVectorPtr()->at(idx) = o.getTpsaObjectAsReference();
	})
	;
    /* pandas .loc like access */
    py::class_<ss_vect_dbl_list_access_loc_t, std::shared_ptr<ss_vect_dbl_list_access_loc_t>> ss_vect_double_list_access_loc (m, "_ss_vect_double_list_access_loc");
    add_methods_list_access_loc<ss_vect_dbl_list_access_loc_t, std::shared_ptr<ss_vect_dbl_list_access_loc_t>, double>(ss_vect_double_list_access_loc);
    add_methods_list_access_loc_getitem<ss_vect_dbl_list_access_loc_t, std::shared_ptr<ss_vect_dbl_list_access_loc_t>, double>(ss_vect_double_list_access_loc);

    /* pandas .loc like access */
    py::class_<ss_vect_tpsa_list_access_loc_t, std::shared_ptr<ss_vect_tpsa_list_access_loc_t>> ss_vect_tpsa_list_access_loc (m, "_ss_vect_tpsa_list_access_loc");
    add_methods_list_access_loc<ss_vect_tpsa_list_access_loc_t, std::shared_ptr<ss_vect_tpsa_list_access_loc_t>, gpy::TpsaWithNamedIndex>(ss_vect_tpsa_list_access_loc);
    ss_vect_tpsa_list_access_loc
	.def("__getitem__",  [](const ss_vect_tpsa_list_access_loc_t &self, const std::string name){
		    // std::cerr << "__getitem__ returning TpsaWithNamedIndex " << std::endl;
		    auto idx = self.getVectorPtr()->getMapping()->index(name);
		    return ss_vect_tpsa_elem_access_t(self.getVectorPtr(), idx);
	})
	;

    /**
     */
    py::class_<ss_vect_dbl_t, std::shared_ptr<ss_vect_dbl_t>> ss_vect_double_intern (m, "_ss_vect_double");
    AddMethods<ss_vect_dbl_t, std::shared_ptr<ss_vect_dbl_t>> double_cls;
    double_cls.add_methods<double>(ss_vect_double_intern);


	py::class_<ss_vect_tpsa_t, std::shared_ptr<ss_vect_tpsa_t>>  ss_vect_tpsa_intern (m, "_ss_vect_tpsa");
	AddMethods<ss_vect_tpsa_t, std::shared_ptr<ss_vect_tpsa_t>> tpsa_cls;
	tpsa_cls.add_methods<gtpsa::tpsa>(ss_vect_tpsa_intern);
	tpsa_cls.add_methods_tpsa<gtpsa::tpsa>(ss_vect_tpsa_intern);
	ss_vect_tpsa_intern
            .def(py::self += gtpsa::ss_vect<double>(0e0))
            .def(py::self -= gtpsa::ss_vect<double>(0e0))
            .def(py::self +  gtpsa::ss_vect<double>(0e0))
            .def(py::self -  gtpsa::ss_vect<double>(0e0))
            ;


	py::class_<ss_vect_dbl_py_t, std::shared_ptr<ss_vect_dbl_py_t>> ss_vect_double (m, "ss_vect_double", ss_vect_double_intern);
	// add_methods_named_index<ss_vect_dbl_py_t, std::shared_ptr<ss_vect_dbl_py_t>, double>(ss_vect_double);
	ss_vect_double
	.def(py::init<const double&, const size_t, std::shared_ptr<gpy::IndexMapping>>(),
                 "init state space",
                 py::arg("place_holder"), py::arg("state_space_size") = gtpsa::ss_vect_n_dim,
                 py::arg("index_mapping") =  gpy::default_index_mapping_ptr
                   )

        .def(py::init<const std::vector<double>&, std::shared_ptr<gpy::IndexMapping>>(),
                  "init state space",
                  py::arg("place_holder"),
                  py::arg("index_mapping") =  gpy::default_index_mapping_ptr
            )
               .def_property_readonly("loc",          [](ss_vect_dbl_py_t &self) {
		   return ss_vect_dbl_list_access_loc_t(self.getPtr());
               }  , py::keep_alive<1, 0>() )
               .def_property_readonly("iloc",         [](ss_vect_dbl_py_t &self) {
		   return ss_vect_dbl_list_access_iloc_t(self.getPtr());
               } , py::keep_alive<0, 1>()
		   )
            // .def("cst", &ss_vect_dbl_py_t::cst,  py::keep_alive<1, 0>())
            /*
               .def("__dir__",      [](const ss_vect_dbl_py_t& self){
                                               return self.getMapping()->pdir();
                                        })
            */
               .def("__getattr__",  [](const ss_vect_dbl_py_t&  self, const std::string& key){
                       auto t = self.at(self.getMapping()->index(key));
                       // misses index mapping
                       return t;
               }, py::keep_alive<0, 1>())
               .def("__setattr__",  [](ss_vect_dbl_py_t&  self, const std::string& key, const double val){
                       self.at(self.getMapping()->index(key)) = val;
               })
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self += py::self)
        .def(py::self -= py::self)
        .def(py::self += double())
        .def(py::self -= double())
               .def("__copy__",  &ss_vect_dbl_py_t::clone)
               .def("copy",      &ss_vect_dbl_py_t::clone)

       ;

        py::class_<ss_vect_tpsa_py_t, std::shared_ptr<ss_vect_tpsa_py_t>> ss_vect_tpsa (m, "ss_vect_tpsa", ss_vect_tpsa_intern);
        // these functions have to be here before the ones defined by the classes
        ss_vect_tpsa
            .def(py::init<const std::shared_ptr<gtpsa::mad::desc>, const ord_t, const size_t, std::shared_ptr<gpy::IndexMapping>>(),
                 "init state space",
                 py::arg("desc"), py::arg("maximum_order"), py::arg("state_space_size") = gtpsa::ss_vect_n_dim,
                 py::arg("index_mapping") =  gpy::default_index_mapping_ptr
                )
            .def(py::init<const gtpsa::tpsa&, const size_t, std::shared_ptr<gpy::IndexMapping>>(),
                 "init state space",
                 py::arg("truncated power series object (as reference to description)"),
                 py::arg("state_space_size") = gtpsa::ss_vect_n_dim,
                 py::arg("index_mapping") =  gpy::default_index_mapping_ptr
             )
/*
            .def("__getitem__",  [](const ss_vect_tpsa_py_t& self, const long int idx){
                auto t =  self.at(idx);
                // misses index mapping
                std::cerr << "Getting tpsa itme of index " << idx << std::endl;
                return gpy::TpsaWithNamedIndex(t, self.getMapping());
            }, py::keep_alive<0, 1>())
*/
            .def("__dir__",      [](const ss_vect_tpsa_py_t& self){
                                        return self.getMapping()->pdir();
                                 })
            .def("__getattr__",  [](ss_vect_tpsa_py_t&  self, const std::string& key){
		std::cerr << "tpsa object" << std::endl;
		const auto idx =  self.getMapping()->index(key);
                // misses index mapping
		return ss_vect_tpsa_elem_access_t(self.getPtr(), idx);
            }, py::keep_alive<0, 1>())
            .def("__setattr__",  [](ss_vect_tpsa_py_t&  self, const std::string& key, const gtpsa::tpsa& val){
                self.at(self.getMapping()->index(key)) = val;
            })
            // not taking reference properly yet
            .def_property_readonly("iloc",          [](ss_vect_tpsa_py_t &self) {
                // std::cerr << "providing iloc access for " << &self;
                auto tmp =  ss_vect_tpsa_list_access_iloc_t(self.getPtr());
                // std::cerr << " returning " << &tmp;
                auto vec = tmp.getVectorPtr();
                // std::cerr<< " getVector " <<  vec <<  " keepalive <0, 1> " <<std::endl;
                return tmp;
            }, py::keep_alive<0, 1>() )
            .def_property_readonly("loc",          [](ss_vect_tpsa_py_t &self) {
                return ss_vect_tpsa_list_access_loc_t(self.getPtr());
            } , py::keep_alive<0, 1>())
            .def("cst", &ss_vect_tpsa_py_t::cst, py::return_value_policy::move)
            .def(py::self + py::self)
            .def(py::self - py::self)
            .def(py::self += py::self)
            .def(py::self -= py::self)
            .def(py::self += ss_vect_dbl_py_t(0e0))
            .def(py::self -= ss_vect_dbl_py_t(0e0))
            .def(py::self +  ss_vect_dbl_py_t(0e0))
            .def(py::self -  ss_vect_dbl_py_t(0e0))
	    .def("__copy__",  &ss_vect_tpsa_py_t::clone)
	    .def("copy",      &ss_vect_tpsa_py_t::clone)
	    ;

        // add_methods_named_index<ss_vect_tpsa_py_t, std::shared_ptr<ss_vect_tpsa_py_t>, gtpsa::tpsa>(ss_vect_tpsa);
        // adding functions
        m.def("compose", &gtpsa::compose);

        /*
          py::class_<gtpsa::ss_vect<gtpsa::ctpsa>> ss_vect_ctpsa  (m, "ss_vect_ctpsa");
          AddMethods<gtpsa::ss_vect<gtpsa::ctpsa>> ctpsa_cls;
          ctpsa_cls.add_methods<gtpsa::ctpsa>(ss_vect_ctpsa);
          ctpsa_cls.add_methods_tpsa<gtpsa::ctpsa>(ss_vect_ctpsa);
        */

}
