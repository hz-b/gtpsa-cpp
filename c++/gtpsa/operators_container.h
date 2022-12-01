//
// Created by mfp on 25.11.22.
//

#ifndef GTPSA_OPERATORS_CONTAINER_H
#define GTPSA_OPERATORS_CONTAINER_H

#include <gtpsa/with_operators.hpp>
#include <gtpsa/utils.hpp>
#include <gtpsa/mad_tpsa_wrapper.hpp>
#include <gtpsa/mad_ctpsa_wrapper.hpp>
#include <gtpsa/bridge.hpp>

#include <vector>

namespace gtpsa {
    template<typename T, typename = typename T::base_type, typename = typename T::bridge>
    inline void rcompose(const std::vector<const T *> &ma, const std::vector<const T *> &mb,
                         std::vector<T *> *mc) {
        auto ma_p = gtpsa::vector_type_to_const_base_ptr<T, typename T::bridge>(ma);
        auto mb_p = gtpsa::vector_type_to_const_base_ptr<T, typename T::bridge>(mb);
        auto mc_p = gtpsa::vector_type_to_const_base_ptr<T, typename T::bridge>(&mc);
        gtpsa::mad::rcompose(ma_p, mb_p, &mc_p);
    }


    inline void rcompose(const std::vector<const tpsa *> &ma, const std::vector<const tpsa *> &mb,
                         std::vector<tpsa *> *mc) {

        const auto ma_p = gtpsa::vector_ptr_to_const_base_ptr<tpsa, tpsa::bridge>(ma);
        const auto mb_p = gtpsa::vector_ptr_to_const_base_ptr<tpsa, tpsa::bridge>(mb);
        auto mc_p = gtpsa::vector_ptr_to_base_ptr<tpsa, tpsa::bridge>(*mc);
        gtpsa::bridge_rcompose(ma_p, mb_p, &mc_p);
    }


} // namespace gtpsa
#endif //GTPSA_OPERATORS_CONTAINER_H
