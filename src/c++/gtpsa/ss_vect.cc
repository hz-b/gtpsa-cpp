#include <gtpsa/ctpsa.hpp>
#include <gtpsa/ss_vect.h>
#include <gtpsa/intern/gtpsa_container.hpp>
#include <sstream>

void gtpsa::report_vector_dimension_mismatch(size_t nv,  const size_t n)
{
    std::stringstream strm;
    strm << "gtpsa::ss_vect<gtpsa::(c)tpsa> "
	 << " state space of  dimension " << n <<  " requested"
	 << " tpsa object has only "<< nv << " number of variables";
    throw std::runtime_error(strm.str());
}

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
	const auto& info = t_tpsa.getDescription()->getInfo();
	const int nn = info.getNumberOfVariables() + info.getNumberOfParameters();
	std::vector<ord_t> ord(nn);
	for (int j = 0; j<nn; ++j){
	    // reset to zero
	    for(auto &e: ord) e =  0;
	    ord[j] = 1;
	    // todo: validate index
	    auto val = t_tpsa.get(ord);
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




/**
 * @brief start of derivative
 *
 * @param nv number of variables
 *
 * estimate the first argument for getv / setv
 * @todo recheck if not contained in Laurent's gtpsa,
 * @todo how to handle knob variables
 */
size_t gtpsa::estimate_start_order(size_t order, size_t nv)
{
    size_t result = 0;
    switch(order){
        case 3: result += nv * nv; /* fallthrough */
        case 2: result += nv; /* fallthrough */
        case 1: result += 1; /* fallthrough */
        case 0: result += 0; return result; break;
        default:
            throw std::runtime_error("not implemented order");
    }
}

template<>
arma::mat gtpsa::ss_vect<gtpsa::tpsa>::jacobian(void) const
{

    auto desc = this->state_space.at(0).getDescription();
    size_t nv = desc->getNv();


    arma::mat mat(this->size(), nv, arma::fill::zeros);
    mat.fill(NAN);

    // std::cout << "get jacobian .." << std::endl;

    std::vector<ord_t> ord(nv);
    for(size_t slice = 0; slice < this->size(); ++slice) {
	auto &t = this->state_space[slice];
	for(size_t row = 0; row < nv; ++row){
	    // set ord to zero
	    for(auto& e: ord) e=0;
	    // compute exponent array for this entry
	    ord.at(row) += 1;
	    if(t.index(ord) < 0){
		std::cerr << "Invalid index {" << int(ord.at(0)) << ", " << int(ord.at(1)) << "," << int(ord.at(2)) << "}" <<std::endl;
		continue;
	    }
	    mat(slice, row) = t.get(ord);
	}
    }
    return mat;

    /*
      original version... I prefer to do proper look up ..

      const size_t start = estimate_start_order(1, nv);
      std::vector<num_t> vec(nv);
      for(size_t row = 0; row < this->state_space.size(); ++row){
      for(auto& e: vec) e = NAN;
      auto &t = this->state_space[row];
        t.getv(start, &vec);
        for(size_t col = 0; col < nv; ++col){
            mat(row, col) = vec[col];
        }
    }


    return mat;
    */

}
template<>
void gtpsa::ss_vect<gtpsa::tpsa>::setJacobian(arma::mat& jac) {



    auto desc = this->state_space.at(0).getDescription();
    size_t nv = desc->getNv();

    if (jac.n_rows != this->state_space.size()) {
        std::stringstream strm;
        strm << "Jacobian matrix has " << jac.n_rows << " rows, but state space has size "
             << this->state_space.size();
        throw std::runtime_error(strm.str());
    }

    if (jac.n_cols != nv) {
        std::stringstream strm;
        strm << "Jacobian matrix has " << jac.n_cols << " cols, but tpsa expects  "
             << nv << " values!";
        throw std::runtime_error(strm.str());
    }

    std::vector<ord_t> ord(nv);
    for(size_t slice = 0; slice < this->size(); ++slice) {
	auto &t = this->state_space[slice];
	for(size_t row = 0; row < nv; ++row){
	    // set ord to zero
	    for(auto& e: ord) e=0;
	    // compute exponent array for this entry
	    ord[row] += 1;
	    if(t.index(ord) < 0){
		std::stringstream strm;
		strm << "gtpsa::ss_vect::setJacobian: ";
		strm << "Invalid index {" << int(ord[0]) << ", " << int(ord[1]) << "," << int(ord[2]) << "}" <<std::endl;
		throw std::runtime_error(strm.str());
		continue;
	    }
	    t.set(ord, 0e0, jac(slice, row));
	}
    }

/*
    // throw std::runtime_error("needs to be reimplemented");
    const size_t start = estimate_start_order(1, nv);
    for(size_t row = 0; row < this->state_space.size(); ++row){
        arma::mat row_vec = jac.row(row);
        std::vector<double> v(jac.n_cols);
        for(size_t col=0; col < jac.n_cols; ++col){
            v.at(col) = row_vec[col];
        }
        this->state_space[row].setv(start, v);
    }
*/
}

/**
 * @brief extract the hessian from the gtpsa based ss_vect
 * @return  arma::cube as hessian
 */
template<>
arma::cube gtpsa::ss_vect<gtpsa::tpsa>::hessian() const
{
    auto desc = this->state_space.at(0).getDescription();
    size_t nv = desc->getNv();

    arma::cube cube(this->size(), nv, nv);
    cube.fill(NAN);
    std::vector<num_t> vec(nv * nv);

    /*
     * tried to start to prepare for a general access ....
     * but hessian is stored sparse ... seems that one has to
     * retrieve element by element
     */
    //const size_t start = estimate_start_order(2, nv);
    for(size_t slice = 0; slice < this->size(); ++slice) {
        for(auto& e: vec) e = NAN;
        auto &t = this->state_space[slice];

        std::vector<ord_t> ord(nv);
        for(size_t col = 0; col < nv; ++col){
            for(size_t row = 0; row < this->state_space.size(); ++row){
                // set ord to zero
                for(auto& e: ord) e=0;
                // compute exponent array for this entry
                ord[row] += 1;
                ord[col] += 1;
                if(t.index(ord) < 0){
                    std::cerr << "Invalid index {" << int(ord[0]) << ", " << int(ord[1]) << "," << int(ord[2]) << "}" <<std::endl;
                    continue;
                }
                cube(row, col, slice) = t.get(ord);
            }
        }
    }

    return cube;
}

template<>
void gtpsa::ss_vect<gtpsa::tpsa>::setHessian(arma::cube& jac)
{
    throw std::runtime_error("setHessian not yet implemented for gtpsa::ss_vect<gtpsa::tpsa>");
}


using bridge_container_type = gtpsa::TpsaBridgeContainer<gtpsa::TpsaTypeInfo>;
static gtpsa::FilterBasePointers <gtpsa::tpsa, gtpsa::TpsaTypeInfo> filter;
template<>
int gtpsa::ss_vect<gtpsa::tpsa>::getMaximumOrder(void) const
{
    TpsaBridgeContainer<gtpsa::TpsaTypeInfo> bc(filter.as_const(this->state_space));
    return bc.getMaximumOrder();
}


template<>
void  gtpsa::ss_vect<gtpsa::tpsa>::rliebra(const gtpsa::ss_vect<gtpsa::tpsa>& a, const gtpsa::ss_vect<gtpsa::tpsa>& b)
{
    FilterBasePointers <gtpsa::tpsa, gtpsa::TpsaTypeInfo> filter;
    const bridge_container_type ma_b(filter.as_const(a.state_space)), mb_b(filter.as_const(b.state_space));
    bridge_container_type mc_b(filter.as_non_const(this->state_space));

    mc_b.rliebra(ma_b, mb_b);
}

template<>
void  gtpsa::ss_vect<gtpsa::tpsa>::rexppb(const gtpsa::ss_vect<gtpsa::tpsa>& a, const gtpsa::ss_vect<gtpsa::tpsa>& b)
{
    FilterBasePointers <gtpsa::tpsa, gtpsa::TpsaTypeInfo> filter;
    const bridge_container_type ma_b(filter.as_const(a.state_space)), mb_b(filter.as_const(b.state_space));
    bridge_container_type mc_b(filter.as_non_const(this->state_space));

    mc_b.rexppb(ma_b, mb_b);
}

template<>
void  gtpsa::ss_vect<gtpsa::tpsa>::rlogpb(const gtpsa::ss_vect<gtpsa::tpsa>& a, const gtpsa::ss_vect<gtpsa::tpsa>& b)
{
    FilterBasePointers <gtpsa::tpsa, gtpsa::TpsaTypeInfo> filter;
    const bridge_container_type ma_b(filter.as_const(a.state_space)), mb_b(filter.as_const(b.state_space));
    bridge_container_type mc_b(filter.as_non_const(this->state_space));

    mc_b.rlogpb(ma_b, mb_b);
}



template<>
void gtpsa::ss_vect<gtpsa::tpsa>::rvec2fld(const gtpsa::tpsa& a) {
    bridge_container_type mc_b(filter.as_non_const(this->state_space));
    mc_b.rvec2fld(a);
}

template<>
void gtpsa::ss_vect<gtpsa::tpsa>::fld2vec(gtpsa::tpsa * r) const {
    const bridge_container_type mc_b(filter.as_const(this->state_space));
    mc_b.fld2vec(r);
}

template<>
void gtpsa::ss_vect<gtpsa::tpsa>::fgrad(gtpsa::tpsa * b, gtpsa::tpsa * r) const {
    const bridge_container_type mc_b(filter.as_const(this->state_space));
    mc_b.fgrad(b, r);
}


template<>
void  gtpsa::ss_vect<gtpsa::tpsa>::rcompose(const gtpsa::ss_vect<gtpsa::tpsa>& a, const gtpsa::ss_vect<gtpsa::tpsa>& b)
{
    FilterBasePointers <gtpsa::tpsa, gtpsa::TpsaTypeInfo> filter;

    const bridge_container_type ma_b(filter.as_const(a.state_space)), mb_b(filter.as_const(b.state_space));
    bridge_container_type mc_b(filter.as_non_const(this->state_space));

    mc_b.rcompose(ma_b, mb_b);
}

template<>
void  gtpsa::ss_vect<gtpsa::tpsa>::rminv(const gtpsa::ss_vect<gtpsa::tpsa>& a)
{
    FilterBasePointers <gtpsa::tpsa, gtpsa::TpsaTypeInfo> filter;

    const bridge_container_type ma_b(filter.as_const(a.state_space));
    bridge_container_type mc_b(filter.as_non_const(this->state_space));

    mc_b.rminv(ma_b);
}

template<>
void  gtpsa::ss_vect<gtpsa::tpsa>::rpminv(const gtpsa::ss_vect<gtpsa::tpsa>& a, std::vector<idx_t>& select)
{
    FilterBasePointers <gtpsa::tpsa, gtpsa::TpsaTypeInfo> filter;

    const bridge_container_type ma_b(filter.as_const(a.state_space));
    bridge_container_type mc_b(filter.as_non_const(this->state_space));

    mc_b.rminv(ma_b);
}



template<>
double gtpsa::ss_vect<gtpsa::tpsa>::computeNorm(void) const
{
    TpsaBridgeContainer<gtpsa::TpsaTypeInfo> bc(filter.as_const(this->state_space));
    return bc.computeNorm();
}
template<>
void  gtpsa::ss_vect<gtpsa::tpsa>::rgetOrder(const gtpsa::ss_vect<gtpsa::tpsa>& a, const int order)
{
    for(size_t i=0; i<this->state_space.size(); ++i){
        (*this)[i].rgetOrder(a[i], order);
    }
}

template<>
void  gtpsa::ss_vect<gtpsa::tpsa>::rderiv(const gtpsa::ss_vect<gtpsa::tpsa>& a, const int order)
{
    for(size_t i=0; i<this->state_space.size(); ++i){
        (*this)[i].rderiv(a[i], order);
    }
}
