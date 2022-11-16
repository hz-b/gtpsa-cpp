#include <gtpsa/ctpsa.hpp>
#include <gtpsa/ss_vect.h>


template<>
void gtpsa::ss_vect<double>::show(std::ostream& strm, int level, bool with_endl) const
{
    for(size_t i= 0; i<this->state_space.size(); ++i){
	strm << " " << this->state_space[i];
    }
    if(with_endl) {	strm << "\n";        }
}

template<typename T>
void gtpsa::ss_vect<T>::show(std::ostream& strm, int level, bool with_endl) const
{
    int precision = 6;

    strm << std::scientific << std::setprecision(precision);
    strm  << "cst\n";
    for(size_t i= 0; i<this->state_space.size(); ++i){
	auto& t_tpsa = this->state_space[i];
	auto val = t_tpsa.getsm(std::vector<idx_t>{0, 0});
	strm << std::setw(14) << val << " ";
    }
    if(with_endl || (level >= 1)) {	strm << "\n";        }
    if(level == 0){
	return;
    }

    strm  << "map\n";
    // preserve order
    for(size_t i= 0; i<this->state_space.size(); ++i){
	auto& t_tpsa = this->state_space[i];
	for (int j = 0; j<6; ++j){
	    // todo: validate index
	    auto val = t_tpsa.getsm(std::vector<idx_t>{int(j+1), 1});
	    strm << std::setw(14) << val << " ";
	}
	strm << "\n";
    }
}

template void gtpsa::ss_vect<gtpsa::tpsa>::show(std::ostream& strm, int level, bool with_endl) const;
template void gtpsa::ss_vect<gtpsa::ctpsa>::show(std::ostream& strm, int level, bool with_endl) const;

template<typename T>
std::string gtpsa::ss_vect<T>::pstr(void)
{
    std::stringstream strm;
    this->show(strm);
    return strm.str();
}

template<typename T>
std::string gtpsa::ss_vect<T>::repr(void)
{
    std::stringstream strm;
    this->show(strm, 10);
    return strm.str();
}


template std::string gtpsa::ss_vect<double>::repr(void);
template std::string gtpsa::ss_vect<double>::pstr(void);
template std::string gtpsa::ss_vect<gtpsa::tpsa>::repr(void);
template std::string gtpsa::ss_vect<gtpsa::tpsa>::pstr(void);
template std::string gtpsa::ss_vect<gtpsa::ctpsa>::repr(void);
template std::string gtpsa::ss_vect<gtpsa::ctpsa>::pstr(void);
