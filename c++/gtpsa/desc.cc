#include <gtpsa/desc.hpp>
void gtpsa::mad::desc_info::show(std::ostream& strm) const
{
	strm << " number of variables: "           << this->getNumberOfVariables()
	     << " maximum order for variables: "   << int(this->getVariablesMaximumOrder())
	     << " number of parameters: "          << this->getNumberOfParameters()
	     << " maximum order for parameters: "  << int(this->getParametersMaximumOrder());

	strm  << " orders [";

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

gtpsa::mad::desc_info gtpsa::mad::desc::getInfo() const
{
	ord_t mo=-1, po=-1;
	int np = -1;
	auto nv =  mad_desc_getnv (this->getPtr(), &mo, &np, &po);
	std::vector<ord_t> orders(nv + np);
	this->getNo(orders.size(), orders.data());
	auto info =  desc_info(nv, mo, np, po, orders);

	return info;
}
