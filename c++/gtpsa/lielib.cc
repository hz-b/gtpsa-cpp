//
// Created by mfp on 25.01.23.
//
#include <utility>
#include <gtpsa/tpsa.hpp>
#include <gtpsa/ss_vect.h>
#include <armadillo>
#include <gtpsa/tpsa.hpp>
#include <gtpsa/ss_vect.h>

const double eps_tps = 1e-20;


#if 0
tps tps_fun
(const tps &a, std::function<double (const long int [])> fun)
{
  // Dacfu in Forest's F77 LieLib.
  // Multiplies mononials I_vec with function f(I_vec).
  char     name[name_len_for+1];
  int      k, n;
  long int ibuf1[bufsize], ibuf2[bufsize], jj[nv_tps];
  double   rbuf[bufsize];
  tps      b;

  a.exprt(rbuf, ibuf1, ibuf2, name);
  n = rbuf[0];
  for (k = 1; k <= n; k++) {
    dehash_(no_tps, nv_tps, ibuf1[k-1], ibuf2[k-1], jj);
    rbuf[k] *= fun(jj);
  }
  b.imprt(n, rbuf, ibuf1, ibuf2);
  return b;
}
#endif

#if 0
double f_int_mon(const long int jj[])
{
  // Integrate monomials:
  //   scl = 1/(|I_vec|+1)
  int    k;
  double scl;

  scl = 0e0;
  for (k = 0; k < 2*nd_tps; k++)
    scl += jj[k];
  scl += 1e0;
  scl = 1e0/scl;
  return scl;
}
#endif



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

#if 1
gtpsa::ss_vect<gtpsa::tpsa> h_to_v(const gtpsa::tpsa &h)
{
  // Difd in Forest's F77 LieLib:
  // Compute vector flow operator from Lie operator :h:
  //   v = Omega * [del_x H, del_px H]^T
  int          k;
  gtpsa::ss_vect<gtpsa::tpsa> v(h);

  auto n_dim = v.size() / 2;
  for (k = 0; k < n_dim; k++) {
    v[2*k + 1].rderiv(h, 2*k+1);
    v[2*k    ].rderiv(h, 2*k+2);
    v[2*k    ] *= -1;
  }
  return v;
}
#endif

/**
 *   y = v * nabla * x
 *
 * @param v
 * @param x
 * @return
 *
 * *  Daflo in Forest's F77 LieLib.
 */
gtpsa::tpsa v_to_tps(const gtpsa::ss_vect<gtpsa::tpsa> &v, const gtpsa::tpsa &x)
{

  // need to take order of x into account
    // int max_order = std::max(v.getMaximumOrder(), x.getMaximumOrder());
    int max_order = v.getMaximumOrder();
    auto y = gtpsa::tpsa(x.getDescription(), max_order);
    auto this_deriv = gtpsa::tpsa(x.getDescription(), max_order);
    for (int k = 0; k < 2 * v.size(); k++) {
        this_deriv.clear();
        this_deriv.rderiv(x, k + 1);
        y += v[k] * this_deriv;
    }
  return y;
}


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
#if 1

gtpsa::tpsa exp_v_to_tps(const gtpsa::ss_vect<gtpsa::tpsa> &v, const gtpsa::tpsa &x, const double eps,
                         const int n_max)
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



gtpsa::tpsa exp_v_to_tps(const gtpsa::ss_vect<gtpsa::tpsa> &v, const gtpsa::tpsa &x, const int k1,
	      const int k2, const double scl, const bool reverse)
{
  // Facflo in Forest's F77 LieLib.
  // not reverse:
  //   y = exp(D_k1) * exp(D_k1+1) ...  * exp(D_k2) * x
  // reverse:
  //   y = exp(D_k2) * exp(D_k2-1) ... * exp(D_k1) * x
  int          k;

  const int n_max = 100;

  auto y = x.clone();
  auto t_order = v.allocateLikeMe();

  if (!reverse) {
      for (k = k1; k <= k2; k++) {
          t_order.set_zero();
          t_order.rgetOrder(v, k);
          t_order *= scl;
          y = exp_v_to_tps(t_order, y, eps_tps, n_max);
      }
    } else {
      for (k = k2; k >= k1; k--) {
          t_order.set_zero();
          t_order.rgetOrder(v, k);
          t_order *= scl;
          y = exp_v_to_tps(t_order, y, eps_tps, n_max);
    }
  }
  return y;
}
#endif

#if 1
gtpsa::ss_vect<gtpsa::tpsa>M_to_M_fact(const gtpsa::ss_vect<gtpsa::tpsa> &t_map)
{
  // Flofac in Forest's F77 LieLib.
  // Factor map:
  //   M = M_2 ... * M_n

    // factor off the linear part
    auto  map_lin_inv = t_map.allocateLikeMe();
    arma::mat jac = t_map.jacobian(), jac_inv = arma::inv(jac);
    map_lin_inv.setJacobian(jac_inv);
    auto map_res = gtpsa::compose(t_map, map_lin_inv);

    auto map_fact = t_map.allocateLikeMe();
    auto map_single_order = t_map.allocateLikeMe();
    map_fact.set_zero();
    for(int k = 2; k < t_map.getMaximumOrder(); ++k){
        map_single_order.rgetOrder(map_res, k);
        map_fact += map_single_order;
# warning "using number of dimensions, should that not be a config parameter",
        for(size_t j = 0; j < 2 * t_map.size(); ++j){
            map_res[j] = exp_v_to_tps(map_fact, map_res[j], k, k, -1e0, false);
        }
    }
  return map_fact;
}
#endif

#if 1
gtpsa::tpsa exp_h_to_tps(const gtpsa::tpsa &h, const gtpsa::tpsa &x, const double eps,
		   const int n_max)
{
  // Exp1d in Forest's F77 LieLib.
  //   y = exp(:h:) x
  return exp_v_to_tps(h_to_v(h), x, eps, n_max);
}
#endif

#if 1
gtpsa::ss_vect<gtpsa::tpsa> exp_h_to_M(const gtpsa::tpsa &h, const gtpsa::ss_vect<gtpsa::tpsa> &x, const double eps,
		       const int n_max)
{
  // Expnd2 in Forest's F77 LieLib.
  //   Y = exp(:h:) X
  auto y = x.clone();
  // warning: fix number of dimensions
  auto n_dim = x.size() / 2;
  for (int k = 0; k < 2 * n_dim; k++)
    y[k] = exp_h_to_tps(h, y[k], eps, n_max);
  return y;
}
#endif

/**
 * @brief Taylor map to  Dragt-Finn factorised Lie generator
 *
 * // Liefact in Forest's F77 LieLib.
  // A. Dragt, J. Finn "Lie Series and Invariant Functions for Analytic
  // Symplectic maps" J. Math. Phys. 17, 2215-2227 (1976).
  // Dragt-Finn factorization:
  //   M ->  M_lin * exp(:h_3:) * exp(:h_4:) ...  * exp(:h_n:)
 */
gtpsa::tpsa M_to_h_DF(const gtpsa::ss_vect<gtpsa::tpsa> &t_map)
{
  return M_to_h(M_to_M_fact(t_map));
}

#if 1
/**
 *
 * @brief Dragt-Finn factorised Lie generator to Taylor map
 *
 * Compute map from Dragt-Finn factorisation
 *
 * forward (not reverse):
 *  M = exp(:h_3:) * exp(:h_4:) ...  * exp(:h_n:) * X
 * reverse:
 *   M = exp(:h_n:) * exp(:h_no-1:) ... * exp(:h_3:) * X
 *
 * @param Lie_DF_gen
 * @param x
 * @param k1 lowest order?
 * @param k2 highest order?
 * @param reverse
 * @return
 *
 * Fexpo in Forest's F77 LieLib.
 */
gtpsa::ss_vect<gtpsa::tpsa> h_DF_M
(const gtpsa::tpsa &Lie_DF_gen, const gtpsa::ss_vect<gtpsa::tpsa> &x, const int k1, const int k2,
 const bool reverse)
{
  auto desc = x[0].getDescription();
  auto mo = x.getMaximumOrder();
  gtpsa::ss_vect<gtpsa::tpsa> a_map(desc, mo);
  gtpsa::tpsa h_k(desc, mo);

  const int n_max = 100;

  a_map.set_identity();
  for (int k = k2; k >= k1; k--) {
      h_k.clear();
      h_k.rgetOrder(Lie_DF_gen, k);
      if (!reverse) {
          a_map = gtpsa::compose(a_map, exp_h_to_M(h_k, x, eps_tps, n_max));
      } else {
          a_map = gtpsa::compose(exp_h_to_M(h_k, x, eps_tps, n_max), a_map);
      }
  }
  return a_map;
}
#endif

//------------------------------------------------------------------------------

#if 0
/**
 *
 * @param h
 * @param map
 * @param k1
 * @param k2
 * @param reverse
 * @return
*
 * @todo: consider to splt up functionallity in two functions or provide 2 wrappers?
* Fexpo in Forest's LieLib.
  * Compute map from Dragt-Finn factorisation:
   * not reverse:
   *   exp(:h_3:) exp(:h_4:) ... exp(:h_no:)
    * reverse:
   *   exp(:h_no:) exp(:h_no-1:) ... exp(:h_3:)
 */
gtpsa::ss_vect<gtpsa::tpsa> compute_Taylor_map
        (const gtpsa::tpsa& h, const gtpsa::ss_vect<gtpsa::tpsa> &t_map, const int k1, const int k2,
         const int reverse)
{
    gtpsa::ss_vect<gtpsa::tpsa> map1 = t_map.allocateLikeMe();

    map1.set_identity();
    for (int k = k2; k >= k1; k--) {
        auto h_k = h.getOrder(k);
        if (!reverse)
            map1 = map1*LieExp_JB(h_k, t_map);
        else
            map1 = LieExp_JB(h_k, t_map) * map1;
    }
    return map1;
}
#endif

/**
 * @brief Lie factorisation of a numerically computed Taylor map
 *
 * @param map
 * @return
 *
 * Dragt, J. Finn "Lie Series and Invariant Functions for Analytic
 * Symplectic maps" J. Math. Phys. 17, 2215-2227 (1976).
 * Dragt-Finn factorization:
 *   M = exp(:h_no:) exp(:h_no-1:)... exp(:h_4:) exp(:h_3:) M_lin
 */
gtpsa::tpsa
Lie_factorisation(const gtpsa::ss_vect<gtpsa::tpsa> &t_map)
{
    //
    auto a_desc = t_map[0].getDescription();
    const int  mo = t_map.getMaximumOrder();

    gtpsa::ss_vect<gtpsa::tpsa> Id = t_map.allocateLikeMe();
    Id.set_identity();

    // ss vect containing only the inverse of the linear part
    auto map_lin_inv = t_map.allocateLikeMe();
    arma::mat jac = t_map.jacobian();
    map_lin_inv.setJacobian(jac);

    auto map1 = gtpsa::compose(t_map, map_lin_inv);

    gtpsa::tpsa h(a_desc, mo);
    h.clear();
    auto tmp = t_map.allocateLikeMe();

#warning order or degree of freedom
    for (int k = 3; k <= mo; k++) {
	tmp.set_zero();
        // get only the second order
	tmp.rgetOrder(map1, k - 1);
#warning "do I need to use -1e0? !"
	h += tmp
        h +=  //compute_Lie_exp(tmp,);
        map1 *= compute_Dragt_Finn_Map(-h_k, Id, k, k, true);
    }
    return h;
}
