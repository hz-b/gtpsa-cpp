#include <stdbool.h>

#include <eigen3/Eigen/Dense>
#include <complex>
#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>
#include <boost/math/special_functions/sign.hpp>

#include <gtpsa/lielib.hpp>
// #include <gtpsa/mad/tpsa_wrapper.hpp>
#include <assert.h>

// Gtpsa map operations are in:
//   ..gtpsa/mad-ng/src]/mad_tpsa_mops.c


// F. Klein 𝑉𝑒𝑟𝑔𝑙𝑒𝑖𝑐ℎ𝑒𝑛𝑑𝑒 𝐵𝑒𝑡𝑟𝑎𝑐ℎ𝑡𝑢𝑛𝑔𝑒𝑛 𝑢̈𝑏𝑒𝑟 𝑛𝑒𝑢𝑒𝑟𝑒 𝑔𝑒𝑜𝑚𝑒𝑡𝑟𝑖𝑠𝑐ℎ𝑒 𝐹𝑜𝑟𝑠𝑐ℎ𝑢𝑛𝑔𝑒𝑛
// (Deichert, 1872).
// Aka Felix Klein's Erlangen Program.
//  https://archive.org/details/abn7632.0001.001.umich.edu/page/n1/mode/2up


#define sqr(x) ((x)*(x))


const int
  X_     = 0,
  Y_     = 1,

  x_     = 0,
  px_    = 1,
  y_     = 2,
  py_    = 3,
  delta_ = 4,
  ct_    = 5;


gtpsa::ss_vect<gtpsa::tpsa> mat2map
(const std::shared_ptr<gtpsa::mad::desc> &desc, const Eigen::MatrixXd &A)
{
  const int
    ps_dim = 6,
    no     = desc->maxOrd();

  auto Id = gtpsa::ss_vect<gtpsa::tpsa>(desc, no);
  auto B  = gtpsa::ss_vect<gtpsa::tpsa>(desc, no);

  Id.set_identity();
  B.set_zero();
  for (auto j = 0; j < ps_dim; j++)
    for (auto k = 0; k < ps_dim; k++)
      B[j] += A(j, k)*Id[k];

  return B;
}


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


inline void print_ind(const std::vector<ord_t> &ind)
{
  for (auto i: ind)
    std::cout << std::setw(2) << (int)i;
}


inline void print_mn
(const int k, const num_t v_k, const std::vector<ord_t> &ind)
{
  const int n_dec = 3;

  std::cout << std::scientific << std::setprecision(n_dec)
	    << std::setw(3) << k << std::setw(n_dec+8) << v_k;
  print_ind(ind);
  std::cout << "\n";
}


void print_tpsa(const gtpsa::tpsa &a)
{
  const double eps = 1e-30;

  const auto nv = a.getDescription()->getNv();

  std::vector<ord_t> ind(nv);
  std::vector<num_t> v(a.length());

  a.getv(0, &v);
  for (auto k = 0; k < v.size(); k++) {
    a.mono(k, &ind);
    if (fabs(v[k]) > eps)
      print_mn(k, v[k], ind);
  }
}


void print_int_vec(const std::string &str, const Eigen::VectorXi &v)
{
  std::cout << str;
  for (auto k = 0; k < v.size(); k++)
    std::cout << std::setw(1) << v(k) << "\n";
}


void print_vec(const std::string &str, const Eigen::VectorXd &v)
{
  const int n_dec = 5;

  std::cout << str;
  for (auto k = 0; k < v.size(); k++)
    std::cout << std::scientific << std::setprecision(n_dec)
	      << std::setw(n_dec+8) << v(k) << "\n";
}


void print_mat(const std::string &str, const Eigen::MatrixXd &M)
{
  const int n_dec = 5;

  std::cout << str;
  for (auto j = 0; j < M.col(0).size(); j++) {
    for (auto k = 0; k < M.row(0).size(); k++)
      std::cout << std::scientific << std::setprecision(n_dec)
		<< std::setw(n_dec+8) << M(j, k);
    std::cout << "\n";
  }
}


void print_complex_vec(const std::string &str, const Eigen::VectorXcd &v)
{
  const int n_dec = 5;

  std::cout << str;
  for (auto k = 0; k < v.size(); k++)
    std::cout << std::scientific << std::setprecision(n_dec)
	      << std::setw(n_dec+8) << v(k).real()
	      << ((v(k).imag() > 0e0)? "+i":"-i")
	      << std::setw(n_dec+6) << fabs(v(k).imag()) << "\n";
}


void print_complex_mat(const std::string &str, const Eigen::MatrixXcd &M)
{
  const int n_dec = 5;

  std::cout << str;
  for (auto j = 0; j < M.col(0).size(); j++) {
    for (auto k = 0; k < M.row(0).size(); k++)
      std::cout << std::scientific << std::setprecision(n_dec)
		<< std::setw(n_dec+8) << M(j, k).real()
		<< ((M(j, k).imag() > 0e0)? "+i":"-i")
		<< std::setw(n_dec+6) << fabs(M(j, k).imag());
    std::cout << "\n";
  }
}


double acos2(const double sin, const double cos)
{
  if (fabs(cos) > 1e0) {
    std::cout << std::scientific << std::setprecision(5)
	      << "\nacos2 - argument > 1: " << cos << "\n";
    return NAN;
  }
  auto phi = acos(cos);
  if (sin < 0e0)
    phi = 2e0*M_PI - phi;
  return phi;
}


/**
 * Compute norm of tpsa:
 *    |a| = sum | a_k |
 *
 * Todo:
 *    Replace by mad_desc_gtrunc in mad_desc.c.
 */
double compute_norm(gtpsa::tpsa &a)
{
  const auto len = a.length();

  std::vector<num_t> v(len);

  a.getv(0, &v);
  auto norm = 0e0;
  for (auto k = 0; k < len; k++)
    norm += fabs(v[k]);
  return norm;
}


/**
 *  𝐷𝑎𝑓𝑙𝑜 in Forest's F77 LieLib.
 *    y = v * nabla * x
 */
gtpsa::tpsa v_to_tps(const gtpsa::ss_vect<gtpsa::tpsa> &v, const gtpsa::tpsa &x)
{
  const int ps_dim = 6;

  auto y = x.clone();

  y.clear();
  for (auto k = 0; k < ps_dim; k++)
    y += v[k]*deriv(x, k+1);
  return y;
}


/**
 * 𝐸𝑥𝑝𝑓𝑙𝑜 in Forest's F77 LieLib:
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
 */
gtpsa::tpsa exp_v_to_tps
(const gtpsa::ss_vect<gtpsa::tpsa> &v, const gtpsa::tpsa &x, const double eps,
 const int n_max)
{
  double eps1;
  auto   y_k = x.clone();
  auto   y   = x.clone();

  for (auto k = 1; k <= n_max; k++) {
    y_k = v_to_tps(v, y_k/k);
    y += y_k;
    eps1 = compute_norm(y_k);
    if (eps1 < eps)
      break;
  }
  if (eps1 < eps)
    return y;
  else {
    printf("\n*** exp_v_to_tps: did not converge eps = %9.3e (eps = %9.3e)"
	   " n_max = %1d\n", eps1, eps, n_max);
    return y;
  }
}


gtpsa::ss_vect<gtpsa::tpsa> exp_v_to_map
(const gtpsa::ss_vect<gtpsa::tpsa> &v, const gtpsa::ss_vect<gtpsa::tpsa> &map)
{
  const int
    ps_dim = 6,
    n_max  = 100;
  const double
    eps_tps = 1e-30;

  auto M = map.clone();

  for (auto k = 0; k < ps_dim; k++)
    M[k] = exp_v_to_tps(v, M[k], eps_tps, n_max);
  return M;
}


/**
 * @brief Factor map:
 *
 * 𝐹𝑙𝑜𝑓𝑎𝑐 in Forest's F77 LieLib.
 * Factor map:
 *   M = M_2 ... * M_n
  */
static gtpsa::ss_vect<gtpsa::tpsa>
M_to_M_fact(const gtpsa::ss_vect<gtpsa::tpsa> &t_map)
{
  const int
    ps_dim = 6,
    no     = t_map.getMaximumOrder();

  auto map_lin     = t_map.allocateLikeMe();
  auto map_lin_inv = t_map.allocateLikeMe();
  auto map_fact    = t_map.allocateLikeMe();
  auto map_k       = t_map.allocateLikeMe();

  map_lin.rgetOrder(t_map, 1);
  map_lin_inv = gtpsa::minv(map_lin);
  // map_lin_inv[ps_dim].setVariable(0e0);
  
  auto map_res = gtpsa::compose(t_map, map_lin_inv);

  map_fact.set_zero();
  for(int k = 2; k < no; ++k) {
    map_k.rgetOrder(map_res, k);
    map_fact += map_k;
    map_k.rgetOrder(map_fact, k);
    // Workaround for:
    //   operator *= -1e0.
    for (auto j = 0; j < map_k.size(); j++)
      map_k[j] = -map_k[j];
#if 1
    map_res = exp_v_to_map(map_k, map_res);
#else
    map_res = gtpsa::exppb(map_k, map_res);
#endif
  }

  return map_fact;
}

#if 1

inline int compute_ord(const std::vector<ord_t> &ind)
{
  const int ps_dim = 6;

  ord_t ord = 0;
  for (auto k = 0; k < ps_dim; k++)
    ord += ind[k];
  return (int)ord;
}


void scl_mns(gtpsa::tpsa &mn)
{
  const int  ps_dim = 6;
  const auto nv     = mn.getDescription()->getNv();

  std::vector<num_t> v(mn.length());
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
 * 𝐼𝑛𝑡𝑑 in Forest's F77 LieLib.
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

  auto mn   = M[0].clone();
  auto ps_k = M[0].clone();
  auto h    = M[0].clone();

  h.clear();
  for (auto k = 0; k < ps_dim; ++k) {
    auto index = (k % 2 == 0)? k+2 : k;
    ps_k.clear();
    ps_k.setVariable(0e0, index, 0e0);
    mn = M[k]*ps_k;
    // Integrate monomials.
    scl_mns(mn);
    h += (k % 2 == 0)? -mn : mn;
  }
  return h;
}

#else

// The gtpsa function fld2vec doesn't work for parameter dependence.
gtpsa::tpsa M_to_h(const gtpsa::ss_vect<gtpsa::tpsa> &t_map)
{
  auto h = t_map[0].clone();
  h.clear();
  // In ../gtpsa/mad-ng/src/mad_tpsa_mops.c.
  t_map.fld2vec(&h);
  return h;
}

#endif

double f_q_k_conj(const std::vector<ord_t> &jj)
{
  // Adjust the sign for the momenta for the oscillating planes.
  // Correct sign for complex vs. real momenta p_k.
  //   q_k =  (h_q_k^+ + h_q_k^-) / 2
  // i p_k = -(h_q_k^+ - h_q_k^-) / 2
  // Adjust the sign for the momenta for the oscillating planes.

  const int n_dof = 3;

  int ord, k, sgn = 0;

  // Compute the sum of exponents for the momenta for the oscillating planes:
  ord = 0;
  for (k = 0; k < n_dof; k++)
    ord += jj[2*k+1];
  ord = (ord % 4);
  //  Sum_k c_ijkl x^i p_x^j y^k p_y^l
  //  j + l mod 4 = [0, 3: +1; 1, 2: -1]
  switch (ord) {
  case 0:
  case 3:
    sgn = 1;
    break;
  case 1:
  case 2:
    sgn = -1;
    break;
  default:
    printf("\n: undefined case %d\n", ord);
    break;
  }
  return sgn;
}


gtpsa::tpsa tps_compute_function
(const gtpsa::tpsa &a, std::function<double (const std::vector<ord_t> &)> fun)
{
  const int  ps_dim = 6;
  const auto desc   = a.getDescription();
  const auto nv     = desc->getNv();
  const auto no     = desc->maxOrd();

  auto b = gtpsa::tpsa(desc, no);

  std::vector<num_t> v(a.length());
  std::vector<ord_t> ind(nv);

  a.getv(0, &v);
  for (auto k = 0; k < v.size(); k++) {
    auto ord = a.mono(k, &ind);
    if (v[k] != 0e0)
      v[k] *= fun(ind);
  }
  b.setv(0, v);

  return b;
}


gtpsa::tpsa q_k_conj(const gtpsa::tpsa &a)
{
  return tps_compute_function(a, f_q_k_conj);
}


void gtpsa::CtoR(const gtpsa::tpsa &a, gtpsa::tpsa &a_re, gtpsa::tpsa &a_im)
{

  const int n_dof = 3;

  const auto desc = a.getDescription();
  const auto no   = desc->maxOrd();

  auto b   = gtpsa::tpsa(desc, no);
  auto c   = gtpsa::tpsa(desc, no);

  auto Id  = gtpsa::ss_vect<gtpsa::tpsa>(desc, no);
  auto map = gtpsa::ss_vect<gtpsa::tpsa>(desc, no);
  auto tmp = gtpsa::ss_vect<gtpsa::tpsa>(desc, no);

  Id.set_identity();

  b = q_k_conj(a);

  // q_k -> (q_k + p_k) / 2
  // p_k -> (q_k - p_k) / 2
  // Complex space:
  // q_k =   (h_q_k^+ + h_q_k^-) / 2
  // p_k = i (h_q_k^+ - h_q_k^-) / 2
  map.set_identity();
  for (auto k = 0; k < n_dof; k++) {
    map[2*k]   = (Id[2*k]+Id[2*k+1])/2e0;
    map[2*k+1] = (Id[2*k]-Id[2*k+1])/2e0;
  }
  // b = gtpsa::compose(b, map);

  // q_k -> p_k
  // p_k -> q_k
  // Complex space:
  // i (q_k -/+ i p_k) = (i q_k +/- p_k)
  map.set_identity();
  for (auto k = 0; k < n_dof; k++) {
    map[2*k]   = Id[2*k+1];
    map[2*k+1] = Id[2*k];
  }
  tmp.set_identity();
  tmp[0] = b;
  c = gtpsa::compose(tmp, map)[0];

  a_re = (b+c)/2e0;
  a_im = (b-c)/2e0;
}


gtpsa::tpsa RtoC(gtpsa::tpsa &a_re, gtpsa::tpsa &a_im)
{
  const int n_dof = 3;

  const auto desc = a_re.getDescription();
  const auto no   = desc->maxOrd();

  auto b   = gtpsa::tpsa(desc, no);
  auto Id  = gtpsa::ss_vect<gtpsa::tpsa>(desc, no);
  auto map = gtpsa::ss_vect<gtpsa::tpsa>(desc, no);
  auto tmp = gtpsa::ss_vect<gtpsa::tpsa>(desc, no);

  Id.set_identity();

  b = a_re + a_im;

  // q_k -> q_k + p_k
  // p_k -> q_k - p_k
  // Complex space:
  // h_q_k^+ = q_k - i h_p_k
  // h_q_k^- = q_k + i h_p_k
  map.set_identity();
  for (auto k = 0; k < n_dof; k++) {
    map[2*k]   = Id[2*k] + Id[2*k+1];
    map[2*k+1] = Id[2*k] - Id[2*k+1];
  }
  tmp.set_identity();
  tmp[0] = b;
  b = gtpsa::compose(tmp, map)[0];
  b = q_k_conj(b);
  return b;
}


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


gtpsa::tpsa gtpsa::M_to_h_DF(const gtpsa::ss_vect<gtpsa::tpsa> &M)
{
  // Workaround because gtpsa map compose can't handle parameter dependence.
  ord_t no, po;
  int   np;

  const auto desc0 = M[0].getDescription();
  const auto nv    = M[0].getDescription()->getNv(&no, &np, &po);

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


Eigen::MatrixXd get_lin_map(gtpsa::ss_vect<gtpsa::tpsa> &map)
{
  const int ps_dim = 6;

  Eigen::MatrixXd M(ps_dim, ps_dim);

  for (auto j = 0; j < ps_dim; j++) {
    for (auto k = 0; k < ps_dim; k++)
      M(j, k) = map[j].get(k);
  }
  return M;
}


Eigen::VectorXd compute_nu_symp(const Eigen::MatrixXd &M)
{
  // Eigenvalues for a 4x4 symplectic periodic matrix.

  const int
    n_dof = 2,
    n_dim = 2*n_dof;

  double
    x[n_dof];
  Eigen::VectorXd
    nu(n_dof);
  Eigen::MatrixXd
    Id = Eigen::MatrixXd::Identity(n_dim, n_dim);

  auto Pp1 = (M-Id).determinant();
  auto Pm1 = (M+Id).determinant();
  auto p = (Pp1-Pm1)/8e0;
  auto q = (Pp1+Pm1)/8e0 - 1e0;
  auto sgn =
    (M.block(0, 0, n_dof, n_dof).trace()
     > M.block(n_dof, n_dof, n_dof, n_dof).trace())?
    1 : -1;
  x[0] = -p/2e0 + sgn*sqrt(sqr(p/2e0)-q);
  x[1] = -p/2e0 - sgn*sqrt(sqr(p/2e0)-q);
  for (auto k = 0; k < n_dof; k++) {
    nu[k] = acos(x[k])/(2e0*M_PI);
    if (M(2*k, 2*k+1) < 0e0)
      nu[k] = 1e0 - nu[k];
  }
  std::cout << std::fixed << std::setprecision(5)
	    << "\ncompute_nu:\n  nu = [" << nu[X_] << ", " << nu[Y_] << "]\n";

  return nu;
}


int find_closest_nu(const double nu, const Eigen::VectorXcd &w)
{
  int    ind;
  double nu_k, diff;

  auto min = 1e30;
  for (auto k = 0; k < w.size(); k++) {
    nu_k = acos2(w(k).imag(), w(k).real())/(2e0*M_PI);
    diff = fabs(nu_k-nu);
    if (diff < min) {
      ind = k;
      min = diff;
    }
  }
  return ind;
}


Eigen::VectorXi sort_eigen_vec
(const Eigen::VectorXd &nu, const Eigen::VectorXcd &w)
{
  const int
    n_dof = 2,
    n_dim = 2*n_dof;

  Eigen::VectorXi order(n_dim);

  for (auto k = 0; k < nu.size(); k++) {
    order(2*k) = find_closest_nu(nu[k], w);
    order(2*k+1) = find_closest_nu(1e0-nu[k], w);
  }
  return order;
}


Eigen::MatrixXd compute_S(const int n_dof)
{
  // Remark: Beware of sign for longitudinal dim.
  const int n_dim = 2*n_dof;

  Eigen::MatrixXd S = Eigen::MatrixXd::Zero(n_dim, n_dim);
  for (auto k = 0; k < n_dof; k++) {
    S(2*k, 2*k+1) = 1e0;
    S(2*k+1, 2*k) = -1e0;
  }

  return S;
}


Eigen::VectorXd compute_dispersion(const Eigen::MatrixXd &M)
{
  const int
    n_dof = 2,
    n_dim = 2*n_dof;
  const Eigen::MatrixXd
    Id = Eigen::MatrixXd::Identity(n_dim, n_dim);

  auto D = M.col(delta_).segment(0, n_dim);
  auto M_4x4 = M.block(0, 0, n_dim, n_dim);

  return (Id-M_4x4).inverse()*D;
}


Eigen::MatrixXd compute_A0(const Eigen::MatrixXd &M)
{
  const int
    n_dof = 2;

  Eigen::MatrixXd
    A0 = Eigen::MatrixXd::Identity(6, 6);

  auto eta = compute_dispersion(M);

  if (n_dof == 2) {
    // Coasting beam - translate to momentum dependent fix point.
    for (auto k = 0; k < n_dof; k++) {
      A0(2*k, delta_)   =  eta(2*k);
      A0(2*k+1, delta_) =  eta(2*k+1);
      A0(ct_, 2*k)      =  eta(2*k+1);
      A0(ct_, 2*k+1)    = -eta(2*k);
    }
  }

  return A0;
}


Eigen::MatrixXd compute_A1(const int n_dof, Eigen::MatrixXcd &u_ord)
{
  const int
    n_dim = 2*n_dof;
  const std::complex<double>
    I = std::complex<double>(0e0, 1e0);

  Eigen::MatrixXd
    A1 = Eigen::MatrixXd::Identity(6, 6);
  Eigen::MatrixXcd
    u(n_dim, n_dim);

  auto S = compute_S(n_dof);

  // Normalise eigenvectors: A^T.omega.A = omega.
  for (auto k = 0; k < n_dof; k++) {
    auto z = u_ord.col(2*k).real().dot(S*u_ord.col(2*k).imag());
    auto sgn_im = boost::math::sign(z);
    auto scl = sqrt(fabs(z));
    auto sgn_vec = boost::math::sign(u_ord(2*k, 2*k).real());
    u.col(2*k) =
      sgn_vec*(u_ord.col(2*k).real()+sgn_im*u_ord.col(2*k).imag()*I)/scl;
    u.col(2*k+1) =
      sgn_vec*(u_ord.col(2*k+1).real()+sgn_im*u_ord.col(2*k+1).imag()*I)/scl;
  }
    
  u_ord = u;

  for (auto k = 0; k < n_dof; k++) {
    A1.block(0, 0, n_dim, n_dim).col(2*k)   = u.col(2*k).real();
    A1.block(0, 0, n_dim, n_dim).col(2*k+1) = u.col(2*k).imag();
  }

  return A1;
}


void compute_M_diag(const std::shared_ptr<gtpsa::mad::desc> &desc, MNFType &MNF)
{
  const int
    n_dof = 2,
    n_dim = 2*n_dof;
  
  Eigen::VectorXd
    nu_eig(n_dim),
    nu_eig_ord(n_dim);
  Eigen::VectorXcd
    w_ord(n_dim);
  Eigen::MatrixXcd
    u(n_dim, n_dim),
    u_ord(n_dim, n_dim);

  auto M = get_lin_map(MNF.M);

  // Check if stable.
  auto Tr_x = M.block(0, 0, n_dof, n_dof).trace();
  if (fabs(Tr_x) >= 2e0) {
    std::cout << std::scientific << std::setprecision(5)
	      << "\nEigenvalues - unstable in the horizontal plane:"
	      << " Tr{M_x} = " << Tr_x << "\n";
    return;
  }
  auto Tr_y = M.block(n_dof, n_dof, n_dof, n_dof).trace();
  if (fabs(Tr_y) >= 2e0) {
    std::cout << std::scientific << std::setprecision(5)
	      << "\nEigenvalues - unstable in the vertical plane:"
	      << " Tr{M_y} = " << Tr_y << "\n";
    return;
  }

  auto M_4x4 = M.block(0, 0, n_dim, n_dim);
  Eigen::EigenSolver<Eigen::Matrix<double, n_dim, n_dim> > s(M_4x4);

  auto nu_symp = compute_nu_symp(M_4x4);

  for (auto k = 0; k < n_dim; k++)
    nu_eig[k] =
      acos2(s.eigenvalues()(k).imag(), s.eigenvalues()(k).real())/(2e0*M_PI);

  auto order = sort_eigen_vec(nu_symp, s.eigenvalues());

  for (auto k = 0; k < n_dim; k++) {
    w_ord(k) = s.eigenvalues()(order(k));
    u_ord.col(k) = s.eigenvectors().col(order(k));
    nu_eig_ord(k) =
      acos2(w_ord(k).imag(), w_ord(k).real())/(2e0*M_PI);
  }

  auto A1_mat = compute_A1(n_dof, u_ord);
  auto A0_mat = compute_A0(M);
  auto R_mat  = (A0_mat*A1_mat).inverse()*M*A0_mat*A1_mat;

  MNF.A0 = mat2map(desc, A0_mat);
  MNF.A1 = mat2map(desc, A1_mat);
  MNF.R  = mat2map(desc, R_mat);

  print_map("\nA0:\n",    MNF.A0);
  print_map("\nA1:\n",    MNF.A1);
  print_map("\nA0*A1:\n", gtpsa::compose(MNF.A0, MNF.A1));
  print_map("\nR:\n",     MNF.R);
}


gtpsa::tpsa get_g
(const gtpsa::tpsa nu_x, const gtpsa::tpsa nu_y, const gtpsa::tpsa &h)
{
  // Compute g = (1-R)^-1 * h

  const auto desc = h.getDescription();
  const auto no   = desc->maxOrd();
  const auto nv   = desc->getNv();

  std::vector<ord_t> jj1(nv), jj2(7);
  double             re, im;

  auto h_re  = gtpsa::tpsa(desc, no);
  auto h_im  = gtpsa::tpsa(desc, no);
  auto g_re  = gtpsa::tpsa(desc, no);
  auto g_im  = gtpsa::tpsa(desc, no);
  auto mn1   = gtpsa::tpsa(desc, no);
  auto mn2   = gtpsa::tpsa(desc, no);
  auto cotan = gtpsa::tpsa(desc, no);

  auto Id    = gtpsa::ss_vect<gtpsa::tpsa>(desc, no);

  CtoR(h, h_re, h_im);

  for (auto k = 0; k < nv; k++) {
    jj1[k] = jj2[k] = 0;
  }

  Id.set_identity();
  g_re = g_im = 0e0;
  for (auto i = 0; i <= no; i++) {
    jj1[x_] = jj2[px_] = i;
    for (auto j = 0; j <= no; j++) {
      jj1[px_] = jj2[x_] = j;
      for (auto k = 0; k <= no; k++) {
	jj1[y_] = jj2[py_] = k;
	for (auto l = 0; l <= no; l++) {
	  jj1[py_] = jj2[y_] = l;
	  if ((i+j+k+l <= no) && ((i-j != 0) || (k-l != 0))) {
	    cotan = 1e0/tan(((i-j)*nu_x+(k-l)*nu_y)*M_PI);
	    mn1 =
	      pow(Id[x_], i)*pow(Id[px_], j)*pow(Id[y_], k)*pow(Id[py_], l);
	    mn2 =
	      pow(Id[x_], j)*pow(Id[px_], i)*pow(Id[y_], l)*pow(Id[py_], k);

	    for (auto m = 0; m <= no-i-j-k-l; m++) {
	      jj1[delta_] = jj2[delta_] = m;
	      for (auto n = 0; n <= no-i-j-k-l; n++) {
		jj1[nv-1] = jj2[nv-1] = n;
		re = h_re.get(jj1);
		im = h_im.get(jj1);
		// Compute g.
		g_re +=
		  (re-cotan*im)*(mn1+mn2)*pow(Id[delta_], m)
		  *pow(Id[nv-1], n)/2e0;
		g_im +=
		  (im+cotan*re)*(mn1-mn2)*pow(Id[delta_], m)
		  *pow(Id[nv-1], n)/2e0;
		h_re.set(jj2, 0e0, 0e0);
		h_im.set(jj2, 0e0, 0e0);
	      }
	    }
	  }
	}
      }
    }
  }

  return RtoC(g_re, g_im);
}


gtpsa::tpsa get_Ker(const gtpsa::tpsa &h)
{
  const auto desc = h.getDescription();
  const auto no   = desc->maxOrd();
  const auto nv   = desc->getNv();

  std::vector<ord_t> ind(nv);

  auto h_Ke = gtpsa::tpsa(desc, no);
  auto Id   = gtpsa::ss_vect<gtpsa::tpsa>(desc, no);

  for (auto k = 0; k < nv; k++)
    ind[k] = 0;

  Id.set_identity();
  h_Ke = 0e0;
  for (auto i = 0; i <= no; i++) {
    ind[x_] = ind[px_] = i;
    for (auto j = 0; j <= no; j++) {
      ind[y_] = ind[py_] = j;
      for (auto k = 0; k <= no; k++) {
	ind[delta_] = k;
	for (auto l = 0; l <= no; l++) {
	  ind[nv-1] = l;
	  if ((2*i+2*j+k+l <= no) && ((i != 0) || (j != 0) || (k != 0))) {
	    h_Ke +=
	      h.get(ind)
	      *pow(Id[x_], i)*pow(Id[px_], i)
	      *pow(Id[y_], j)*pow(Id[py_], j)
	      *pow(Id[delta_], k)
	      *pow(Id[nv-1], l);
	  }
	}
      }
    }
  }

  return h_Ke;
}

#if 0

MNFType map_norm(const gtpsa::ss_vect<gtpsa::tpsa> &map)
{
  const auto desc = map[0].getDescription();
  const auto no   = desc->maxOrd();

  int
    n;
  double
    nu0[2];
  MNFType
    MNF;

  auto hn    = gtpsa::tpsa(desc, no);
  auto hn_re = gtpsa::tpsa(desc, no);
  auto hn_im = gtpsa::tpsa(desc, no);
  auto h_ke  = gtpsa::tpsa(desc, no);
  auto gn    = gtpsa::tpsa(desc, no);
  auto Kn    = gtpsa::tpsa(desc, no);

  auto Id   = gtpsa::ss_vect<gtpsa::tpsa>(desc, no);
  auto A    = gtpsa::ss_vect<gtpsa::tpsa>(desc, no);
  auto nus  = gtpsa::ss_vect<gtpsa::tpsa>(desc, no);
  auto M_Fl = gtpsa::ss_vect<gtpsa::tpsa>(desc, no);
  auto map2 = gtpsa::ss_vect<gtpsa::tpsa>(desc, no);

  Id.set_identity();

  danot_(no-1);

  MNF.M_res = MNF.M = map;

  danot_(no);

#if 0
  // Find fixed point.
  GoFix(map, MNF.A0, MNF.A0_inv, no);

  // Translate to fix point.
  map = MNF.A0_inv*map*MNF.A0;

  print_map("\nA0:", A0);
  print_map("\nM_map:", map);
#endif

  compute_M_diag(MNF);

  M_Fl = Inv(MNF.A0*MNF.A1)*MNF.M*MNF.A0*MNF.A1;

  print_map("\nM_Fl:\n", M_Fl);

  MNF.K = 0e0;
  for (auto k = 0; k < 2; k++) {
    nu0[k] = atan2(MNF.R[2*k][2*k+1], MNF.R[2*k][2*k]);
    if (nu0[k] < 0e0) nu0[k] += 2e0*M_PI;
    nu0[k] /= 2e0*M_PI;
    MNF.K -= M_PI*nu0[k]*(sqr(Id[2*k])+sqr(Id[2*k+1]));
  }
  std::cout << "\n";
  std::cout << std::fixed << std::setprecision(5)
       << "nu0 = (" << nu0[X_] << ", " << nu0[Y_] << ")" << "\n";

  // Coasting beam.
  MNF.K += h_ijklm(M_Fl[ct_], 0, 0, 0, 0, 1)*sqr(Id[delta_])/2e0;

#if 0
  MNF.g = 0e0;
  for (auto k = 3; k <= no; k++) {
    n = pow(2, k-3);

    map2 = M_Fl*Inv(MNF.R*FExpo(MNF.K, Id, 3, k-1, -1));
    hn = Intd(get_mns(map2, k-1, k-1), -1e0);
    gn = get_g(nu0[X_], nu0[Y_], hn);
    MNF.g += gn;
    CtoR(hn, hn_re, hn_im);
    Kn = RtoC(get_Ker(hn_re), get_Ker(hn_im));
    MNF.K += Kn;

    A = FExpo(gn, Id, k, k, -1);
    M_Fl = Inv(A)*M_Fl*A;
  }
#endif

  return MNF;
}

#endif
