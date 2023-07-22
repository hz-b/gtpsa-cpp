#ifndef _GTPSA_LIELIB_H_
#define _GTPSA_LIELIB_H_
#include <gtpsa/tpsa.hpp>
#include <gtpsa/ss_vect.h>
// #include <pybind11/stl.h>

namespace gtpsa {
  /**
   * @brief Compute Dragt-Finn Lie generator from a (symplectic) map
   *
   */

  tpsa M_to_h_DF(const ss_vect<tpsa> &t_map);

} // namespace gtpsa

#endif //_GTPSA_LIELIB_H_
