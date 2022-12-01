//
// Created by mfp on 25.11.22.
//
#define BOOST_TEST_MODULE mad_gtpsa_container
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <gtpsa/mad_tpsa_wrapper.hpp>
#include <vector>
#include <gtpsa/desc.hpp>
#include <gtpsa/utils.hpp>


/*
 * just herre to see if it compiles
 */
BOOST_AUTO_TEST_CASE(test_10_rcompose)
{
    size_t n_dim = 6;
    auto desc = std::make_shared<gtpsa::desc>(n_dim, 2);
    std::vector<gtpsa::mad::TpsaWrapper> ma_t, mb_t;
    std::vector<gtpsa::mad::TpsaWrapper> mc_t;

    for(size_t cnt = 0; cnt < n_dim; ++cnt){
        ma_t.push_back(gtpsa::mad::TpsaWrapper(desc, 1));
        mb_t.push_back(gtpsa::mad::TpsaWrapper(desc, 1));
        mc_t.push_back(gtpsa::mad::TpsaWrapper(desc, 1));
    }

    std::vector<gtpsa::mad::_TpsaWrapper*> ma_t_v, mb_t_v;
    std::vector<gtpsa::mad::_TpsaWrapper*> mc_v;

    for(size_t cnt = 0; cnt < n_dim; ++cnt){
        ma_t_v.push_back(&ma_t[cnt]);
        mb_t_v.push_back(&mb_t[cnt]);
        mc_v.push_back(&mc_t[cnt]);
    }

    std::vector<gtpsa::mad::_TpsaWrapper*> ma_v(ma_t_v), mb_v(mb_t_v);

    gtpsa::mad::_TpsaContainerWrapper ma_c(ma_v), mb_c(mb_v);
    gtpsa::mad::_TpsaContainerWrapper mc_c(mc_v);

    mc_c.rcompose(ma_c, mb_c);
}