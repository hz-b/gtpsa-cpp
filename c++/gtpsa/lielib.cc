#include <stdbool.h>
#include <gtpsa/lielib.hpp>
#include <assert.h>

// Gtpsa map operations are in:
//   ..gtpsa/mad-ng/src]/mad_tpsa_mops.c

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


void print_map(const std::string &str, const gtpsa::ss_vect<gtpsa::tpsa> &M)
{
  for (auto k = 0; k < M.size(); k++) 
    M[k].print(str.c_str(), 1e-30, 0);
}


void print_vec(std::vector<num_t> &v)
{
  for (auto mn: v)
    std::cout << std::scientific << std::setprecision(3)
	      << std::setw(11) << mn;
  std::cout << "\n";
}


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

  auto Id = t_map.allocateLikeMe();
  Id.set_identity();

  map_lin.rgetOrder(t_map, 1);
  map_lin_inv = gtpsa::minv(map_lin);
  
  auto map_fact = t_map.allocateLikeMe();
  auto map_res  = gtpsa::compose(t_map, map_lin_inv);
  auto map_k    = t_map.allocateLikeMe();

  map_fact.set_zero();
  for(int k = 2; k < t_map.getMaximumOrder(); ++k) {
    map_k.rgetOrder(map_res, k);
    map_fact += map_k;
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
#if 1
gtpsa::tpsa M_to_h(const gtpsa::ss_vect<gtpsa::tpsa> &map)
{
  const int ps_dim = 6;

  auto max_ord = map.getMaximumOrder();
  auto desc    = map[0].getDescription();
  auto f_x     = gtpsa::tpsa(desc, max_ord);
  auto f_px    = gtpsa::tpsa(desc, max_ord);
  auto Id      = gtpsa::ss_vect<gtpsa::tpsa>(desc, max_ord);
  auto h       = gtpsa::tpsa(desc, max_ord);

  Id.set_identity();
  h.clear();
  for (auto k = 0; k < ps_dim/2; ++k) {
    f_x.clear();
    f_px.clear();
    f_x.rinteg(map[2*k+1], 2*k+1) ;
    f_px.rinteg(map[2*k], 2*(k+1)) ;
    h += f_x - f_px;
  }
  return h;
}

#else

// Does not work for reimplementation of compose with parameter dependence.
gtpsa::tpsa M_to_h(const gtpsa::ss_vect<gtpsa::tpsa> &t_map)
{
  auto max_ord = t_map.getMaximumOrder();
  auto desc    = t_map[0].getDescription();
  auto h       = gtpsa::tpsa(desc, max_ord);
  h.clear();
  // In ../gtpsa/mad-ng/src/mad_tpsa_mops.c.
  const auto info = desc->getInfo();
  std::cout << "\nM_to_h - info:\n" << info << "\n";
  t_map.fld2vec(&h);
  return h;
}

#endif

gtpsa::tpsa param_to_tps(const int nm, const gtpsa::tpsa &a, gtpsa::tpsa &b)
{
  std::vector<num_t> v(nm);
  a.getv(0, &v);
  b.setv(0, v);
  return b;
}


gtpsa::ss_vect<gtpsa::tpsa> param_to_ss_vect
(const int nm, const gtpsa::ss_vect<gtpsa::tpsa> &A,
 gtpsa::ss_vect<gtpsa::tpsa> &B)
{
  for (auto k = 0; k < A.size(); k++)
    param_to_tps(nm, A[k], B[k]);

  return B;
}


gtpsa::tpsa tps_to_param(const int nm, const gtpsa::tpsa &a, gtpsa::tpsa &b)
{
  std::vector<num_t> v(nm);
  a.getv(0, &v);
  b.setv(0, v);
  return b;
}


gtpsa::ss_vect<gtpsa::tpsa> ss_vect_to_param
(const int nm, const gtpsa::ss_vect<gtpsa::tpsa> &A,
 gtpsa::ss_vect<gtpsa::tpsa> &B)
{
  for (auto k = 0; k < A.size(); k++)
    param_to_tps(nm, A[k], B[k]);

  return B;
}


/**
 * Liefact in Forest's F77 LieLib.
 * A. Dragt, J. Finn "Lie Series and Invariant Functions for Analytic
 * Symplectic maps" J. Math. Phys. 17, 2215-2227 (1976).
 * Dragt-Finn factorization:
 *   M ->  M_lin * exp(:h_3:) * exp(:h_4:) ...  * exp(:h_n:)
 */
gtpsa::tpsa gtpsa::M_to_h_DF(const gtpsa::ss_vect<gtpsa::tpsa> &M)
{
  // Workaround for gtpsa map compose with parameter dependence.
  const auto desc0 = M[0].getDescription();
  const auto info  = desc0->getInfo();
  const auto nv    = info.getNumberOfVariables();
  const auto no    = info.getVariablesMaximumOrder();
  const auto np    = info.getNumberOfParameters();
  const auto po    = info.getParametersMaximumOrder();
  const auto desc1 = std::make_shared<gtpsa::desc>(nv+np, no);
  const int  nm    = desc0->maxLen(no);

  auto h = gtpsa::tpsa(desc0, no);
  auto h1 = gtpsa::tpsa(desc1, no);
  auto M1 = gtpsa::ss_vect<gtpsa::tpsa>(desc1, no);

  print_map("\nM:", M);
  param_to_ss_vect(desc0->maxLen(no), M, M1);
  M1[6].set(7, 0e0, 1e0);
  print_map("\nM1:", M1);
  h1 = M_to_h(M_to_M_fact(M1));
  h1.print("\nh1:", 1e-30, 0);
  tps_to_param(desc0->maxLen(no), h1, h);

  return h;
}
