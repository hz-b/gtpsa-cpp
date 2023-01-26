//
// Created by mfp on 25.01.23.
//
#include <utility>
#include <gtpsa/tpsa.hpp>
#include <gtpsa/ss_vect.h>
#include <armadillo>


//------------------------------------------------------------------------------

tps get_tps_k(const tps &h, const int k)
{
  // Take in Forest's F77 LieLib.
  // Get monomials of order k.
  long int no;
  tps      h_k;

  no = getno_();
  danot_(k-1);
  h_k = -h;
  danot_(k);
  h_k += h;
  danot_(no);
  return h_k;
}


ss_vect<tps> get_M_k(const ss_vect<tps> &x, const int k)
{
  // Taked in Forest's F77 LieLib.
  int          i;
  ss_vect<tps> map_k;

  for (i = 0; i < nv_tps; i++)
    map_k[i] = get_tps_k(x[i], k);
  return map_k;
}


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


tps M_to_h(const ss_vect<tps> &map)
{
  // Intd in Forest's F77 LieLib.
  // E. Forest, M. Berz, J. Irwin "Normal Form Methods for Complicated
  // Periodic Systems: A Complete Solution Using Differential Algebra and Lie
  // Operators" Part. Accel. 24, 91-107 (1989):
  //   Eqs. (34)-(37).
  // Integrate monomials:
  //   M -> exp(:h:)
  int          k;
  tps          f_x, f_px, h;
  ss_vect<tps> Id;

  Id.identity();
  h = 0e0;
  for (k = 0; k < nd_tps; k++) {
    // Integrate monomials.
    f_x = tps_fun(map[2*k+1], f_int_mon)*Id[2*k];
    f_px = tps_fun(map[2*k], f_int_mon)*Id[2*k+1];
    h += f_x - f_px;
  }
  return h;
}


ss_vect<tps> h_to_v(const tps &h)
{
  // Difd in Forest's F77 LieLib:
  // Compute vector flow operator from Lie operator :h:
  //   v = Omega * [del_x H, del_px H]^T
  int          k;
  ss_vect<tps> v;

  for (k = 0; k < nd_tps; k++) {
    v[2*k+1] = Der(h, 2*k+1);
    v[2*k] = -Der(h, 2*k+2);
  }
  return v;
}


tps v_to_tps(const ss_vect<tps> &v, const tps &x)
{
  // Daflo in Forest's F77 LieLib.
  //   y = v * nabla * x
  int k;
  tps y;

  y = 0e0;
  for (k = 0; k < 2*nd_tps; k++)
    y += v[k]*Der(x, k+1);
  return y;
}


tps exp_v_to_tps(const ss_vect<tps> &v, const tps &x, const double eps,
	      const int n_max)
{
  // Expflo in Forest's F77 LieLib:
  //   y = exp(v*nabla) * x
  int    k;
  double eps1;
  tps    y_k, y;

  y_k = y = x;
  for (k = 1; k <= n_max; k++) {
    y_k = v_to_tps(v, y_k/k);
    y += y_k;
    eps1 = abs(y_k);
    if (eps1 < eps)
      break;
  }
  if (eps1 < eps)
    return y;
  else {
    printf("\n*** exp_v_to_tps: did not converge eps = %9.3e (eps = %9.3e)"
	   " n_max = %1d\n", eps1, eps, n_max);
    return NAN;
  }
}


tps exp_v_to_tps(const ss_vect<tps> &v, const tps &x, const int k1,
	      const int k2, const double scl, const bool reverse)
{
  // Facflo in Forest's F77 LieLib.
  // not reverse:
  //   y = exp(D_k1) * exp(D_k1+1) ...  * exp(D_k2) * x
  // reverse:
  //   y = exp(D_k2) * exp(D_k2-1) ... * exp(D_k1) * x
  int          k;
  tps          y;
  ss_vect<tps> v_k;

  const int n_max = 100; 

  y = x;
  if (!reverse) {
    for (k = k1; k <= k2; k++) {
      v_k = scl*get_M_k(v, k);
      y = exp_v_to_tps(v_k, y, eps_tps, n_max);
    }
  } else {
    for (k = k2; k >= k1; k--) {
      v_k = scl*get_M_k(v, k);
      y = exp_v_to_tps(v_k, y, eps_tps, n_max);
    }
  }
  return y;
}


ss_vect<tps>M_to_M_fact(const ss_vect<tps> &map)
{
  // Flofac in Forest's F77 LieLib.
  // Factor map:
  //   M = M_2 ... * M_n
  int          j, k;
  ss_vect<tps> map_lin, map_res, map_fact;

  map_lin = get_M_k(map, 1);
  map_res = map*Inv(map_lin);
  map_fact.zero();
  for (k = 2; k <= no_tps; k++) {
    map_fact += get_M_k(map_res, k);
    for (j = 0; j < 2*nd_tps; j++)
      map_res[j] = exp_v_to_tps(map_fact, map_res[j], k, k, -1e0, false);
  }
  return map_fact;
}


tps exp_h_to_tps(const tps &h, const tps &x, const double eps,
		   const int n_max)
{
  // Exp1d in Forest's F77 LieLib.
  //   y = exp(:h:) x
  return exp_v_to_tps(h_to_v(h), x, eps, n_max);
}


ss_vect<tps> exp_h_to_M(const tps &h, const ss_vect<tps> &x, const double eps,
		       const int n_max)
{
  // Expnd2 in Forest's F77 LieLib.
  //   Y = exp(:h:) X
  int          k;
  ss_vect<tps> y;

  y = x;
  for (k = 0; k < 2*nd_tps; k++)
    y[k] = exp_h_to_tps(h, y[k], eps, n_max);
  return y;
}


tps M_to_h_DF(const ss_vect<tps> &map)
{
  // Liefact in Forest's F77 LieLib.
  // A. Dragt, J. Finn "Lie Series and Invariant Functions for Analytic
  // Symplectic maps" J. Math. Phys. 17, 2215-2227 (1976).
  // Dragt-Finn factorization:
  //   M ->  M_lin * exp(:h_3:) * exp(:h_4:) ...  * exp(:h_n:)
  return M_to_h(M_to_M_fact(map));
}


ss_vect<tps> h_DF_M
(const tps &Lie_DF_gen, const ss_vect<tps> &x, const int k1, const int k2,
 const bool reverse)
{
  // Fexpo in Forest's F77 LieLib.
  // Compute map from Dragt-Finn factorisation:
  // not reverse:
  //   M = exp(:h_3:) * exp(:h_4:) ...  * exp(:h_n:) * X
  // reverse:
  //   M = exp(:h_n:) * exp(:h_no-1:) ... * exp(:h_3:) * X
  int          k;
  tps          h_k;
  ss_vect<tps> map;

  const int n_max = 100;

  map.identity();
  for (k = k2; k >= k1; k--) {
    h_k = get_tps_k(Lie_DF_gen, k);
    if (!reverse)
      map = map*exp_h_to_M(h_k, x, eps_tps, n_max);
    else
      map = exp_h_to_M(h_k, x, eps_tps, n_max)*map;
  }
  return map;
}

//------------------------------------------------------------------------------


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
    std::shared_ptr<gtpsa::desc> a_desc = t_map[0].getDescription();
    static const int max_order = t_map.getMaximumOrder();

    gtpsa::ss_vect<gtpsa::tpsa> Id = t_map.allocateLikeMe();
    Id.set_identity();

    // ss vect containing only the inverse of the linear part
    auto map_lin_inv = t_map.allocateLikeMe();
    arma::mat jac = t_map.jacobian();
    map_lin_inv.setJacobian(jac);

    auto map1 = gtpsa::compose(t_map, map_lin_inv);
    gtpsa::tpsa h(desc, mo);

    for (k = 3; k <= a_desc->getOrder(); k++) {
        // get only the second order
#warning "Check that getord does whot one expects!"
        auto map_km1 = map1.getOrder(k - 1);
        h_k = compute_Lie_exp(map_km1, -1e0);
        h += h_k;
        auto map1 = map1 * compute_Dragt_Finn_Map(-h_k, Id, k, k, true);
    }
    return h;
}
