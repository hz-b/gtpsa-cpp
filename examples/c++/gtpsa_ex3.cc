#include <gtpsa/tpsa.hpp>
#include <math.h>
#include <iostream>

int main(int argc, char *argv[])
{
    std::vector<ord_t> m{3,3,2,2,1,1};
    const int nv = 6, np=0;
    const ord_t mo=0, po=0;
    auto a_desc = std::make_shared<gtpsa::desc>(6, mo, np, po, m);
    std::cout << "desc " << a_desc << std::endl;

    auto t1 = gtpsa::tpsa(a_desc, gtpsa::init::default_);
    auto t2 = gtpsa::tpsa(t1, gtpsa::init::same);

    std::vector<num_t> nums = {M_PI/6, 1,1,1,1,1,1};

    t1.setv(0, nums);
    t1.print("ini", 0, 0, 0);

    gtpsa::sin_(t1, &t2);
    t2.print("sin_", 0, 0, 0);

    auto t3 = gtpsa::asin(t2);
    t3.print("asin", 0, 0, 0);

    return 0;

}
