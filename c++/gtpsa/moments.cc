//
// Created by mfp on 25.01.23.
//
#include <utility>
#include <gtpsa/tpsa.hpp>
#include <gtpsa/ss_vect.h>
#include <armadillo>

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
