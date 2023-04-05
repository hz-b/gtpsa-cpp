#ifndef _GTPSA_PYTHON_WRAPPER_NAMED_INDEX_
#define _GTPSA_PYTHON_WRAPPER_NAMED_INDEX_ 1
#include <pybind11/pybind11.h>

namespace gtpsa::python {

  /**
   * @brief allow accessing elements by name assuming that a mapping can be accessed
   *        by getMapping
   */
  template<class WrappedClass, class P_MGR, typename T>
    static void add_methods_named_index(pybind11::class_<WrappedClass, P_MGR>& inst)
  {
    inst
      .def("__dir__",      [](const WrappedClass& self){
	  return self.getMapping()->pdir();
	})
      .def("__getattr__",  [](const WrappedClass& self, const std::string& key){
	  return self.at(self.getMapping()->index(key));
	})
      .def("__setattr__",  [](      WrappedClass& self, const std::string& key, T& v){
	  self.at(self.getMapping()->index(key)) = v;
	})
      ;
  }

} // namespace gtpsa::python
#endif /* _GTPSA_PYTHON_WRAPPER_NAMED_INDEX_ */
