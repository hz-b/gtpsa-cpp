#ifndef _GTPSA_LIELIB_H_
#define _GTPSA_LIELIB_H_
#include <gtpsa/tpsa.hpp>
#include <gtpsa/ss_vect.h>
// #include <pybind11/stl.h>

void print_map(const std::string &str, const gtpsa::ss_vect<gtpsa::tpsa> &M);

void print_vec(std::vector<num_t> &v);

gtpsa::ss_vect<gtpsa::tpsa> param_to_ss_vect
(const int nm, const gtpsa::ss_vect<gtpsa::tpsa> &A,
 gtpsa::ss_vect<gtpsa::tpsa> &B);

gtpsa::ss_vect<gtpsa::tpsa> ss_vect_to_param
(const int nm, const gtpsa::ss_vect<gtpsa::tpsa> &A,
 gtpsa::ss_vect<gtpsa::tpsa> &B);
  
namespace gtpsa {
  /**
   * @brief Compute Dragt-Finn Lie generator from a (symplectic) map
   *
   */

  tpsa M_to_h_DF(const ss_vect<tpsa> &t_map);

} // namespace gtpsa

#endif //_GTPSA_LIELIB_H_
