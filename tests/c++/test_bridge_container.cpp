//
// Created by mfp on 28.11.22.
//
#include <gtpsa/desc.hpp>
#include <gtpsa/tpsa.hpp>
#include <gtpsa/bridge/container.hpp>

#define BOOST_TEST_MODULE gtpsa_bridge_container
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test10_bridge_container)
{

    auto desc = std::make_shared<gtpsa::desc>(6, 2);
    gtpsa::tpsa_bridge
            t0(desc, 1),
            t1(desc, 1),
            t2(desc, 1),
            t3(desc, 1),
            t4(desc, 1),
            t5(desc, 1);

    std::vector<gtpsa::tpsa_bridge*> vec {&t0, &t1, &t2, &t3, &t4, &t5};

    const gtpsa::TTpsaBridgeContainer ba(vec), bb(vec);

    gtpsa::TTpsaBridgeContainer bc(vec);
    bc.rcompose(ba, bb);
}

/* currently only a compile test */
BOOST_AUTO_TEST_CASE(test20_bridge_container_liebra)
{

    auto desc = std::make_shared<gtpsa::desc>(6, 2);
    gtpsa::tpsa_bridge
            t0(desc, 1),
            t1(desc, 1),
            t2(desc, 1),
            t3(desc, 1),
            t4(desc, 1),
            t5(desc, 1);

    std::vector<gtpsa::tpsa_bridge*> vec {&t0, &t1, &t2, &t3, &t4, &t5};

    const gtpsa::TTpsaBridgeContainer ba(vec), bb(vec);

    gtpsa::TTpsaBridgeContainer bc(vec);
    bc.rliebra(ba, bb);
}
