#include <gtpsa/ss_vect.h>
#include <gtpsa/tpsa.hpp>

int main(int argc, char * argv[])
{
    auto a_desc = gtpsa::desc(1, 0);
    auto tr = gtpsa::tpsa(a_desc, mad_tpsa_default);

    auto st1  = gtpsa::ss_vect<gtpsa::tpsa>(tr, mad_tpsa_default);
    auto st2  = gtpsa::ss_vect<gtpsa::tpsa>(tr, mad_tpsa_default);
    st1 += st2;

    double tmp;
    auto st4  = gtpsa::ss_vect<double>(tmp, mad_tpsa_default);
    auto st5  = gtpsa::ss_vect<double>(tmp, mad_tpsa_default);

    return 0;
}
