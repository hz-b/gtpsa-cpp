#include <stdbool.h>
#include <gtpsa/lielib.hpp>
#include <gtpsa/ss_vect.h>
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
  const double eps = 1e-30;

  for (auto k = 0; k < M.size(); k++) 
    M[k].print(str.c_str(), eps, 0);
}


void print_vec(const std::string &str, const std::vector<num_t> &v)
{
  std::cout << str << "\n";
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
    map_fact *= -1e0;
    map_res = gtpsa::exppb(map_fact, map_res);
  }

  return map_fact;
}

#if 1

inline void print_ind(const std::vector<ord_t> &ind)
{
  for (auto i: ind)
    std::cout << std::setw(2) << (int)i;
}


inline int compute_ord(const std::vector<ord_t> &ind)
{
  const int ps_dim = 6;

  ord_t ord = 0;
  for (auto k = 0; k < ps_dim; k++)
    ord += ind[k];
  return (int)ord;
}


void scl_mns
(const int nv, const int k_ind, const int mn_n, gtpsa::tpsa &mn)
{
  const int ps_dim = 6;

  std::vector<num_t> v(mn_n);
  std::vector<ord_t> ind(nv);

  mn.getv(0, &v);
  for (auto k = 0; k < v.size(); k++) {
    auto ord = mn.mono(k, &ind);
    if (v[k] != 0e0)
      v[k] /= compute_ord(ind);
  }
  mn.setv(0, v);
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
gtpsa::tpsa M_to_h(const gtpsa::ss_vect<gtpsa::tpsa> &M)
{
  const int ps_dim = 6;

  const auto desc = M[0].getDescription();
  const auto info = desc->getInfo();
  const auto nv   = info.getNumberOfVariables();
  const auto no   = info.getVariablesMaximumOrder();

  auto mn   = gtpsa::tpsa(desc, no);
  auto ps_k = gtpsa::tpsa(desc, no);
  auto h    = gtpsa::tpsa(desc, no);

  const int mn_n = desc->maxLen(no);

  h.clear();
  for (auto k = 0; k < ps_dim; ++k) {
    auto index = (k % 2 == 0)? k+2 : k;
    ps_k.clear();
    ps_k.setVariable(0e0, index, 0e0);
    mn = M[k]*ps_k;
    scl_mns(nv, index, mn_n, mn);
    h += (k % 2 == 0)? -mn : mn;
  }
  return h;
}

#else

// The gtpsa function fld2vec doesn't work for parameter dependence.
gtpsa::tpsa M_to_h(const gtpsa::ss_vect<gtpsa::tpsa> &t_map)
{
  auto no   = t_map.getMaximumOrder();
  auto desc = t_map[0].getDescription();
  auto h    = gtpsa::tpsa(desc, no);
  h.clear();
  // In ../gtpsa/mad-ng/src/mad_tpsa_mops.c.
  const auto info = desc->getInfo();
  t_map.fld2vec(&h);
  return h;
}

#endif

void param_to_tps(const gtpsa::tpsa &a, gtpsa::tpsa &b)
{
  std::vector<num_t> v(a.length());
  a.getv(0, &v);
  b.setv(0, v);
}


void param_to_ss_vect
(const gtpsa::ss_vect<gtpsa::tpsa> &A, gtpsa::ss_vect<gtpsa::tpsa> &B)
{
  for (auto k = 0; k < A.size(); k++)
    param_to_tps(A[k], B[k]);
}


void tps_to_param(const gtpsa::tpsa &a, gtpsa::tpsa &b)
{
  std::vector<num_t> v(a.length());
  a.getv(0, &v);
  b.setv(0, v);
}


void ss_vect_to_param
(const gtpsa::ss_vect<gtpsa::tpsa> &A, gtpsa::ss_vect<gtpsa::tpsa> &B)
{
  for (auto k = 0; k < A.size(); k++)
    tps_to_param(A[k], B[k]);
}


inline void print_mn
(const int k, std::vector<num_t> &v, const gtpsa::tpsa &a)
{
  const int  n_dec = 3;
  const auto desc  = a.getDescription();
  const auto info  = desc->getInfo();
  const auto nv    = info.getNumberOfVariables();

  std::vector<ord_t> ind(nv);

  auto ord = a.mono(k, &ind);

  std::cout << std::scientific << std::setprecision(n_dec)
	    << std::setw(3) << k << std::setw(11) << v[k];
  print_ind(ind);
  std::cout << "\n";
}


void print_tps(const gtpsa::tpsa &a)
{
  std::vector<num_t> v(a.length());

  a.getv(0, &v);
  for (auto k = 0; k < v.size(); k++)
    print_mn(k, v, a);
}


gtpsa::tpsa gtpsa::M_to_h_DF(const gtpsa::ss_vect<gtpsa::tpsa> &M)
{
  // Workaround for gtpsa map compose with parameter dependence.
  const auto desc0 = M[0].getDescription();
  const auto info  = desc0->getInfo();
  const auto nv    = info.getNumberOfVariables();
  const auto no    = info.getVariablesMaximumOrder();
  const auto np    = info.getNumberOfParameters();

  auto h = gtpsa::tpsa(desc0, no);

  if (np != 0) {
    const auto desc1 = std::make_shared<gtpsa::desc>(nv+np, no);
    const auto desc2 = std::make_shared<gtpsa::desc>(nv, no, np, no);

    auto h1 = gtpsa::tpsa(desc1, no);
    auto h2 = gtpsa::tpsa(desc2, no);
    auto M1 = gtpsa::ss_vect<gtpsa::tpsa>(desc1, no);

    param_to_ss_vect(M, M1);
    M1[6].set(7, 0e0, 1e0);
    h1 = M_to_h(M_to_M_fact(M1));
    tps_to_param(h1, h2);
    return h2;
  } else {
    h = M_to_h(M_to_M_fact(M));
    return h;
  }
}
