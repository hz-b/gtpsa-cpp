#include <stdbool.h>
#include <gtpsa/lielib.hpp>


/**
 *
 *    y = exp(v*nabla) * x
 *
 * @param v
 * @param x
 * @param eps
 * @param n_max
 * @return
 *
 * todo:
 *    return a flag
 *
 * Expflo in Forest's F77 LieLib:
 *
 */
#if 0
static
gtpsa::tpsa exp_v_to_tps
(const gtpsa::ss_vect<gtpsa::tpsa> &v, const gtpsa::tpsa &x, const double eps,
 const int n_max)
{
  // Expflo in Forest's F77 LieLib:
  //   y = exp(v*nabla) * x
  double eps1;
  int k = 0;
  //gtpsa::tpsa    y_k, y;

  auto y_k = x.clone();
  auto y = x.clone();
  for (k = 1; k <= n_max; k++) {
    y_k = v_to_tps(v, y_k / k);
    y += y_k;

#warning "is that only inspecting the constant part"
    eps1 = std::abs(y_k.cst());
    if (eps1 < eps) {
      break;
    }
  }
  // why not return above?
  // code fell of the ramp ...
  if (eps1 < eps) {
    return y;
  }

  std::stringstream strm;
  strm << "exp_v_to_tps: did not converge eps1 (term " << k << " )= " << eps1
       << " tolerance (eps) " << eps << " n max " << n_max;
  throw std::runtime_error(strm.str());
}
#endif


/**
 * @brief   Factor map:
 * $[M = M_2 ... * M_n$]
 *
 * Flofac in Forest's F77 LieLib.
 *
 */
static gtpsa::ss_vect<gtpsa::tpsa>
M_to_M_fact(const gtpsa::ss_vect<gtpsa::tpsa> &t_map)
{
  // Flofac in Forest's F77 LieLib.
  // Factor map:
  //   M = M_2 ... * M_n
  auto  map_lin_inv = t_map.allocateLikeMe();
  arma::mat jac = t_map.jacobian(), jac_inv = arma::inv(jac);
  map_lin_inv.setJacobian(jac_inv);
  auto map_res = gtpsa::compose(t_map, map_lin_inv);

  /*
   * iterate over the higher orders
   * work on it order by order
   */
  auto map_fact = t_map.allocateLikeMe();
  auto map_single_order = t_map.allocateLikeMe();
  map_fact.set_zero();
  for(int k = 2; k < t_map.getMaximumOrder(); ++k) {
    map_single_order.rgetOrder(map_res, k);
    map_fact += map_single_order;
    map_fact *= -1.0;
    map_res = gtpsa::exppb(map_fact, map_res);
  }
  return map_fact;
}



/**
 *
 * Integrate monomials:
 *   M -> exp(:h:)
 * E. Forest, M. Berz, J. Irwin "Normal Form Methods for Complicated
 * Periodic Systems: A Complete Solution Using Differential Algebra and Lie
 * Operators" Part. Accel. 24, 91-107 (1989):
 *
 * @param t_map
 * @return
 */
gtpsa::tpsa M_to_h(const gtpsa::ss_vect<gtpsa::tpsa> &t_map)
{
  // Intd in Forest's F77 LieLib.
  // E. Forest, M. Berz, J. Irwin "Normal Form Methods for Complicated
  // Periodic Systems: A Complete Solution Using Differential Algebra and Lie
  // Operators" Part. Accel. 24, 91-107 (1989):
  //   Eqs. (34)-(37).
  // Integrate monomials:
  //   M -> exp(:h:)
  auto max_ord = t_map.getMaximumOrder();
  auto desc = t_map[0].getDescription();
  auto h = gtpsa::tpsa(desc, max_ord);
  h.clear();

  t_map.fld2vec(&h);

  return h;
}


gtpsa::tpsa gtpsa::M_to_h_DF(const gtpsa::ss_vect<gtpsa::tpsa> &t_map)
{
  // Liefact in Forest's F77 LieLib.
  // A. Dragt, J. Finn "Lie Series and Invariant Functions for Analytic
  // Symplectic maps" J. Math. Phys. 17, 2215-2227 (1976).
  // Dragt-Finn factorization:
  //   M ->  M_lin * exp(:h_3:) * exp(:h_4:) ...  * exp(:h_n:)
  return M_to_h(M_to_M_fact(t_map));
}
