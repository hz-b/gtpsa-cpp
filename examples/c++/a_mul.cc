#include <gtpsa/tpsa.hpp>
// #include <tps.h>
#include <iostream>

int main(int argc, char *argv[])
{
    auto a_desc = std::make_shared<gtpsa::desc>(6, 4);
    const double val = 335.;
    auto a = gtpsa::tpsa(a_desc, 1);
    a.set(0, val);
    std::cout << "a 42\n" << a << std::endl;


    auto x = gtpsa::tpsa(a_desc, 1);
    x.setv(1, {1,0, 0,0, 0,0});
    auto y = gtpsa::tpsa(a_desc, 1);
    y.setv(1, {0,0, 1,0, 0,0});
    y.set(0, 1./113e0);

    auto tmp = a.clone();
    std::cout << "a clone \n" << tmp << std::endl;
    tmp *= x;
    std::cout << "a clone * x \n" << tmp << std::endl;

    std::cout << "x\n" << x << std::endl;
    std::cout << "y\n" << y << "\n" << std::endl;

    std::cout << "a + x\n" << a + x << std::endl;
    std::cout << "a + y\n" << a + y << "\n" << std::endl;

    std::cout << "a * x\n" << a * x << std::endl;
    std::cout << "a * y\n" << a * y << "\n" << std::endl;

    std::cout << "val * x\n" << val * x << std::endl;
    std::cout << "val * y\n" << val * y << "\n" << std::endl;

    std::cout << "x * y\n" << x * y << "\n" << std::endl;

#if 0

    auto t1 = gtpsa::tpsa(a_desc, 1);
    auto t2 = gtpsa::tpsa(a_desc, 1);

    {
	std::vector<double> nums = {0e0, 1,0,0,0,0,0};
	// t1.setv(0, &nums);
	t1.setv(0, nums);
    }
    {
	std::vector<double> nums = {0e0, 0,0,0,0,0,0};
	// t2 = 5e0;
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
