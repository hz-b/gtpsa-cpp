#include <gtpsa/ss_vect.h>
#include <gtpsa/tpsa.hpp>
#include <iostream>

int main(int argc, char * argv[])
{
    auto desc = std::make_shared<gtpsa::desc>(1,0);
    std::cerr << "desc " << *desc << std::endl;

    auto tr = gtpsa::tpsa(desc, mad_tpsa_dflt);

    auto st1  = gtpsa::ss_vect<gtpsa::tpsa>(tr, mad_tpsa_dflt);
    auto st2  = gtpsa::ss_vect<gtpsa::tpsa>(tr, mad_tpsa_dflt);
    st1 += st2;

    double tmp;
    auto st4  = gtpsa::ss_vect<double>(tmp, mad_tpsa_dflt);
    auto st5  = gtpsa::ss_vect<double>(tmp, mad_tpsa_dflt);

    return 0;
}
