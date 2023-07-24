#include <gtpsa/desc.hpp>
void gtpsa::mad::desc_info::show(std::ostream& strm) const
{
  strm << "\ninfo:\n"
       << "  nv = " << this->getNumberOfVariables()
       << " no = " << int(this->getVariablesMaximumOrder())
       << " np = " << this->getNumberOfParameters()
       << " po = " << int(this->getParametersMaximumOrder());

  strm  << "\n  orders:\n  [";

  bool first = true;
  for(auto&o: this->getOrderPerParameter()){
    if(first){
      /* No comma before the first one. */
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
#warning "desc: need to get orders"
  // this->getNo(orders.size(), orders.data());
  auto info = desc_info(nv, mo, np, po, orders);

  return info;
}
