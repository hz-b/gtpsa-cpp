#include <gtpsa/tpsa.hpp>
#include <gtpsa/ctpsa.hpp>
#include <math.h>
#include <iostream>

int main(int argc, char *argv[])
{

    auto d = std::make_shared<gtpsa::desc>(6, 4);
    std::cout << "d   length=" << d->maxLen() << " coeffs" << std::endl;

    auto t1 = gtpsa::tpsa(d,  gtpsa::init::default_);

    std::vector<num_t> vec(101);
    vec[0] =  M_PI/6;
    for (size_t i = 1; i <= 100; ++ i){
	vec[i] = 1;
    }
    t1.setv(0, vec);

    t1.print("ini", 0, 0, 0);

    auto t2 = gtpsa::ctpsa(t1, gtpsa::init::same);

    gtpsa::sin_(t1, &t1);
    t2.print("sin_", 0, 0, 0);

    auto t3 = gtpsa::asin(t2);
    t3.print("asin", 0, 0, 0);

    return 0;

}
