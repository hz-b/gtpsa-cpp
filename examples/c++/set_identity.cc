#include <gtpsa/tpsa.hpp>
#include <gtpsa/ss_vect.h>
#include <math.h>
#include <iostream>
#include <sstream>

int main(int argc, char *argv[])
{
    auto a_desc = std::make_shared<gtpsa::desc>(6, 6);

    auto t1 = gtpsa::tpsa(a_desc, 1);
    t1.print("zeroed ", 0, 0, 0);
    t1.set(1, 0, 1);
    t1.print("step 1 ", 0, 0, 0);
    t1.clear();
    t1.set(6, 0, 1);
    t1.print("step 6 ", 0, 0, 0);

    auto ss = gtpsa::ss_vect<gtpsa::tpsa>(t1);
    ss.set_identity();
    for(unsigned int i = 0; i < ss.size(); ++i){
	std::stringstream strm;
	strm << "vec " << i;
	ss[i].print(strm.str().c_str(), 0, 0, 0);
    }

    std::cout << "identity vec\n " << ss;
    ss[0].setsm(std::vector<int>{4,1}, 0, -1);
    std::cout << "-identity vec\n " << ss;
}
