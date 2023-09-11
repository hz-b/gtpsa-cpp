#include <gtpsa/desc.hpp>
#include <cassert>
#include <iostream>

void gtpsa::mad::desc_info::show(std::ostream& strm) const
{
	strm << "description object"
	     << "\n\t number of variables:          "           << this->getNumberOfVariables()
	     << "\n\t maximum order for variables:  "   << int(this->getVariablesMaximumOrder())
	     << "\n\t number of parameters:         "          << this->getNumberOfParameters()
	     << "\n\t maximum order for parameters: "  << int(this->getParametersMaximumOrder());

	strm  << "\n\t orders [";

	bool first = true;
	for(auto&o: this->getOrderPerParameter()){
		if(first){
			/* no colon before the first one */
			first = false;
		} else {
			strm << ", ";
		}
		strm << int(o);
	}

	strm << "]";
}

// need to upgrade to c++20
#define ensure(arg) assert(arg)

gtpsa::mad::desc_info gtpsa::mad::desc::getInfo() const
{
	ord_t mo=-1, po=-1, max_order=-1;
	int np = -1;
	auto nv =  mad_desc_getnv (this->getPtr(), &mo, &np, &po);

	std::vector<ord_t> orders(nv + np);
	max_order = mad_desc_maxord(this->getPtr(), orders.size(), orders.data());
	ensure(max_order>=0 && max_order!=-1);

	return desc_info(nv, mo, np, po, orders);
}

void gtpsa::mad::desc::show(std::ostream& o) const
{
    o << this->getInfo();
}

std::string gtpsa::mad::desc::repr() const
{
    std::stringstream o;
    this->show(o);
    return o.str();
}
