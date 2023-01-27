//
// Created by mfp on 24.01.23.
//

#include "gtpsa_module.h"
#include <pybind11/pybind11.h>
#include <gtpsa/gtpsa_base_variant.hpp>
#include <gtpsa/tpsa_double_variant.hpp>
#include <gtpsa/ctpsa_complex_variant.hpp>
namespace PYBIND11_NAMESPACE {
    namespace detail {
        template <> struct type_caster<gtpsa::TpsaOrDouble>{
        public:
            PYBIND11_TYPE_CASTER(gtpsa::TpsaOrDouble, const_name("TpsaOrDouble"));
        };
    } // namespace PYBIND11_NAMESPACE::detail
} // namespace PYBIND11_NAMESPACE

void py_gtpsa_init_variant(pybind11::module &m)
{

}