#include <gtpsa/tpsa.hpp>
#include <math.h>
#include <iostream>

int main(int argc, char *argv[])
{
    {
	// descriptor for TPSA with 100 variables of order 2 without parameters
	auto d2 = std::make_shared<gtpsa::desc>(100, 2);
	std::cout << "d2 length=" << d2->maxLen() << " coeffs" << std::endl;
    }

    // descriptor for TPSA with 6  variables  of order 2 and
    //                          94 parameters of order 1

    auto d = std::make_shared<gtpsa::desc>(6, 94, 2, 1);
    std::cout << "d   length=" << d->maxLen() << " coeffs" << std::endl;

    auto t1 = gtpsa::tpsa(d,  gtpsa::init::default_);
    auto t2 = gtpsa::tpsa(t1, gtpsa::init::same);

    std::vector<num_t> vec(101);
    vec[0] =  M_PI/6;
    for (size_t i = 1; i <= 100; ++ i){
	vec[i] = 1;
    }
    t1.setv(0, vec);

    t1.print("ini", 0, 0, 0);

    gtpsa::sin_(t1, &t2);
    t2.print("sin_", 0, 0, 0);

    auto t3 = gtpsa::asin(t2);
    t3.print("asin", 0, 0, 0);

    return 0;

}
