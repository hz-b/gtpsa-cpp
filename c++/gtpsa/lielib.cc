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
gtpsa::tpsa exp_v_to_tps(const gtpsa::ss_vect<gtpsa::tpsa> &v, const gtpsa::tpsa &x,
			 const double eps, const int n_max)
{
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
  strm << "exp_v_to_tps: did not converge eps1 (term " << k << " )= " << eps1 << " tolerance (eps) " << eps << " n max "
       << n_max;
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

    // factor off the linear part
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
    for(int k = 2; k < t_map.getMaximumOrder(); ++k){
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
 * would be order 1 good enough
 * E. Forest, M. Berz, J. Irwin "Normal Form Methods for Complicated
 * Periodic Systems: A Complete Solution Using Differential Algebra and Lie
 * Operators" Part. Accel. 24, 91-107 (1989):
 *
 * @param t_map
 * @return
 */
gtpsa::tpsa M_to_h(const gtpsa::ss_vect<gtpsa::tpsa> &t_map)
{
  auto max_ord =  t_map.getMaximumOrder();
  auto desc = t_map[0].getDescription();
  gtpsa::ss_vect<gtpsa::tpsa> Id(desc, max_ord);

  Id.set_identity();

  auto h = gtpsa::tpsa(desc, max_ord);
  h.clear();
  // h.reset();

#warning "fix number of dimensions"
   auto f = gtpsa::tpsa(desc, max_ord);
   auto f_p = gtpsa::tpsa(desc, max_ord);

   auto n_dim = t_map.size() / 2;
   for (size_t k = 0; k < n_dim; ++k) {
    // Integrate monomials.
      f.clear();
      f.rinteg(t_map[2*k + 1], 2 *k) ;
      // just increase the coefficient order for all

      f_p.clear();
      f_p.rinteg(t_map[2*k   ], 2 * k+1) ;
      h += f - f_p;
  }
  return h;
}

gtpsa::tpsa M_to_h_DF(const gtpsa::ss_vect<gtpsa::tpsa> &t_map)
{
    auto tmp = M_to_M_fact(t_map);
    // fld2vec: Intd in Forest's F77 LieLib.
    auto res =  M_to_h(tmp);
    return res;
}
