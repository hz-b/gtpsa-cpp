#ifndef _GTPSA_LIELIB_H_
#define _GTPSA_LIELIB_H_
#include <gtpsa/tpsa.hpp>
#include <gtpsa/ss_vect.h>

namespace gtpsa {
  /**
   * @brief Compute Dragt-Finn Lie generator from a (symplectic) map
   *
   *
   * Liefact in Forest's F77 LieLib.
   * A. Dragt, J. Finn "Lie Series and Invariant Functions for Analytic
   *  Symplectic maps" J. Math. Phys. 17, 2215-2227 (1976).
   *  Dragt-Finn factorization:
   *   M ->  M_lin * exp(:h_3:) * exp(:h_4:) ...  * exp(:h_n:)
   */
  tpsa M_to_h_DF(const ss_vect<tpsa> &t_map);


} // namespace gtpsa

#endif //_GTPSA_LIELIB_H_
