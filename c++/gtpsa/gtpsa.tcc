#include <ostream>
#include <sstream>


void gtpsa::GTPSA_CLASS(WithOp)::show(std::ostream& strm, int level) const
{

    strm << "gtpsa  cst:\n\t" << this->cst();
    if(this->ord()){
	// at least first order ...
	auto nv = this->getDescription()->getNv(0, 0, 0);
	std::vector<GTPSA_BASE_T> v(nv);
	this->getv(1, &v);

	strm  << "\ngtpsa linear :\n"
	      << std::scientific << std::setw(20);
	for(auto& e: v) strm <<  std::scientific << std::setw(20) << e << " ";
    }
    strm << "\n";
}



/**
 * @brief support python representation of this object
 */
std::string gtpsa::GTPSA_CLASS(WithOp)::repr(void) const
{
    std::stringstream strm;
    this->show(strm, 10);
    return strm.str();
}

/**
 * @brief support python __str__ of this object
 */
std::string gtpsa::GTPSA_CLASS(WithOp)::pstr(void) const {
    std::stringstream strm;
    this->show(strm, 10);
    return strm.str();
}
