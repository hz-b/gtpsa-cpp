#include <gtpsa/tpsa.hpp>
#include <math.h>
#include <iostream>

int main(int argc, char *argv[])
{
    // descriptor for TPSA with 6 variables of order 10,10,10,10,10,10 without parameters
    {
        const int nv = 6, np = 0;
        ord_t mo = 0, po = 0;
	std::vector<ord_t> m{10,10,10,10,10,10};
	auto d10 = std::make_shared<gtpsa::desc>(nv, mo, np, po, m);
	std::cout << "d10 length=" << d10->maxLen(gtpsa::mad::default_) << " coeffs" << std::endl;
    }

    const int nv = 6, np = 0;
    ord_t mo = 0, po = 0;
    std::vector<ord_t> m{2,2,2,2,1,10};

    auto d = std::make_shared<gtpsa::desc>(6, mo, np, po, m);
    std::cout << "d   length=" << d->maxLen(gtpsa::mad::default_) << " coeffs" << std::endl;

    auto t1 = gtpsa::tpsa(d,  gtpsa::init::default_);
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
