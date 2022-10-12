#include <gtpsa/tpsa.hpp>
#include <gtpsa/ss_vect.h>
#include <iostream>

int main(int argc, char *argv[])
{
    auto a_desc = std::make_shared<gtpsa::desc>(6, 4);

    auto t1 = gtpsa::tpsa(a_desc, 3);
    auto t2 = gtpsa::tpsa(a_desc, 3);

    {
	std::vector<double> nums = {3e0, 1,0,0,0,0,0};
	// t1.setv(0, &nums);
	t1.setv(0, nums);
    }
    {
	std::vector<double> nums = {5e0, 0,1,0,0,0,0};
	t2 = 5e0;
	t2.setv(1, {0,1, 0,0, 0,0});
    }


    t1.print("t1", 0, 0, 0);
    t2.print("t2", 0, 0, 0);

    auto r = t1 * t2;
    r.print("t1 * t2", 0, 0, 0);

    std::cout << "t1 len " << t1.len() << ", t2 len " << t2.len() << ", r len " << r.len() << std::endl;
    std::vector<double> v(r.len());
    r.getv(0, &v);
    std::cout << "r v:";
    for(auto& e : v) std::cout << e << " ";
    std::cout << std::endl;


#if 0
    gtpsa::ss_vect<gtpsa::tpsa> vec1(t1), vec2(t1);
    vec1.set_identity();
    vec2.set_identity();

    // vec1.set(0, 1);
    // vec2.set(0, 1);
    vec1[0].set(0, 42);
    vec2[2].set(0, 355/113e0);
    auto vec3 = vec1 * vec2;

    std::cout << "vec1\n"  << vec1 << "vec2\n"  << vec2
	      << "vec1 * vec2\n" << vec3 << std::endl;
#endif

    return 0;

}
