//
// Created by mfp on 30.11.22.
//
#include <gtpsa/tpsa.hpp>
#include <gtpsa/ss_vect.h>
#include <iostream>
#include <sstream>

int main(int argc, char* argv[])
        {
                auto desc = std::make_shared<gtpsa::desc>(6, 4);

        gtpsa::ss_vect<gtpsa::tpsa> vec(desc, 2);
        arma::mat jac = arma::mat(6, 6, arma::fill::eye);
        vec.setJacobian(jac);
        //test_identity(vec);
        // used for tracing variables in debugger
        vec[0].setName("vec_0");
        vec[1].setName("vec_1");
        vec[2].setName("vec_2");
        vec[3].setName("vec_3");
        vec[4].setName("vec_4");
        vec[5].setName("vec_5");
        gtpsa::ss_vect<gtpsa::tpsa> v1 = vec.clone();//, v2 = vec.clone();
        gtpsa::ss_vect<gtpsa::tpsa> v2 = vec.clone();
        v1[0].setName("v1_0");
        v1[1].setName("v1_1");
        v1[2].setName("v1_2");
        v1[3].setName("v1_3");
        v1[4].setName("v1_4");
        v1[5].setName("v1_5");
        v2[0].setName("v2_0");
        v2[1].setName("v2_1");
        v2[2].setName("v2_2");
        v2[3].setName("v2_3");
        v2[4].setName("v2_4");
        v2[5].setName("v2_5");
        //test_identity(vec);
        //test_identity(v1);
        //test_identity(v2);

        /*
        std::vector<double> unit_vector {1,1,1,1,1,1};
        v1.setConstant(unit_vector);
        v2.setConstant(unit_vector);
        vec.setConstant(unit_vector);
        */
            std::cout << "v1\n" << v1 << std::endl;
            std::cout << "v2\n" << v2 << std::endl;
            std::cout << "vec\n" << vec << std::endl;

            arma::mat fill(6, 6, arma::fill::zeros);
        fill.fill( NAN);

        vec.setJacobian(fill);
        std::cout << "=== rcompose =========================================================" << std::endl;
        vec.rcompose(v1, v2);
        std::cout << "end rcompose =========================================================" << std::endl;
        std::cout << "composed identity: " << vec << std::endl;
        //test_identity(vec);

            return 0;
        }