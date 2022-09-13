#include "gtpsa.hpp"
#include <math.h>
#include <iostream>

int main(int argc, char *argv[])
{
    auto a_desc = gtpsa::desc(4, 2, 4, 3);
    std::cout << "desc " << a_desc << std::endl;

    auto t1 = gtpsa::tpsa(a_desc, mad_tpsa_default);
    auto t2 = gtpsa::tpsa(t1, mad_tpsa_default);

    std::vector<num_t> nums = {M_PI/6, 1,1,1,1,1,1};

    t1.setv(0, &nums);
    t1.print("ini", 0, 0, 0);

    gtpsa::sin_(t1, &t2);
    t2.print("sin_", 0, 0, 0);

    auto t3 = gtpsa::asin(t1);
    t3.print("asin", 0, 0, 0);

    return 0;

}
