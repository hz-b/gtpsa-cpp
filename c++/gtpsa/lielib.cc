#include <stdbool.h>
#include <gtpsa/lielib.hpp>
#include <assert.h>


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
 * @brief Factor map:
 *
 * Flofac in Forest's F77 LieLib.
 * Factor map:
 *   M = M_2 ... * M_n
  */
static gtpsa::ss_vect<gtpsa::tpsa>
M_to_M_fact(const gtpsa::ss_vect<gtpsa::tpsa> &t_map)
{
  auto map_lin     = t_map.allocateLikeMe();
  auto map_lin_inv = t_map.allocateLikeMe();

  map_lin.rgetOrder(t_map, 1);
  map_lin_inv = gtpsa::minv(map_lin);

  auto map_res          = gtpsa::compose(t_map, map_lin_inv);
  auto map_fact         = t_map.allocateLikeMe();
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
 * Intd in Forest's F77 LieLib.
 * E. Forest, M. Berz, J. Irwin 𝑁𝑜𝑟𝑚𝑎𝑙 𝐹𝑜𝑟𝑚 𝑀𝑒𝑡ℎ𝑜𝑑𝑠 𝑓𝑜𝑟 𝐶𝑜𝑚𝑝𝑙𝑖𝑐𝑎𝑡𝑒𝑑 𝑃𝑒𝑟𝑖𝑜𝑑𝑖𝑐 𝑆𝑦𝑠𝑡𝑒𝑚𝑠:
 * 𝐴 𝐶𝑜𝑚𝑝𝑙𝑒𝑡𝑒 𝑆𝑜𝑙𝑢𝑡𝑖𝑜𝑛 𝑈𝑠𝑖𝑛𝑔 𝐷𝑖𝑓𝑓𝑒𝑟𝑒𝑛𝑡𝑖𝑎𝑙 𝐴𝑙𝑔𝑒𝑏𝑟𝑎 𝑎𝑛𝑑 𝐿𝑖𝑒 𝑂𝑝𝑒𝑟𝑎𝑡𝑜𝑟𝑠 Part. Accel. 24,
 * 91-107 (1989):
 *   Eqs. (34)-(37).
 * Integrate monomials:
 *   M -> exp(:h:)
 *
 * @param t_map
 * @return
 */
gtpsa::tpsa M_to_h(const gtpsa::ss_vect<gtpsa::tpsa> &t_map)
{
  auto max_ord = t_map.getMaximumOrder();
  auto desc = t_map[0].getDescription();
  auto h = gtpsa::tpsa(desc, max_ord);
  h.clear();
  // In ../gtpsa/mad-ng/src/mad_tpsa_mops.c.
  t_map.fld2vec(&h);
  return h;
}

/**
 * Liefact in Forest's F77 LieLib.
 * A. Dragt, J. Finn "Lie Series and Invariant Functions for Analytic
 * Symplectic maps" J. Math. Phys. 17, 2215-2227 (1976).
 * Dragt-Finn factorization:
 *   M ->  M_lin * exp(:h_3:) * exp(:h_4:) ...  * exp(:h_n:)
 */
gtpsa::tpsa gtpsa::M_to_h_DF(const gtpsa::ss_vect<gtpsa::tpsa> &t_map)
{
  return M_to_h(M_to_M_fact(t_map));
}
