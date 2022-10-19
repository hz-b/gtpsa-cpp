#ifndef _GTPSA_SS_VECT_H_
#define _GTPSA_SS_VECT_H_ 1
#include <gtpsa/tpsa.hpp>
#include <algorithm>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <armadillo>

namespace gtpsa {
/**
 * state space, phase space vector
 *
 * assuming that all are of the same length ...
 *
 * @todo:
 *        review space state length
 *        review description the state space should have
 */
template<typename T>
class ss_vect{

public:
    inline ss_vect(const T& t, const size_t n=6)
	: state_space(n, 0e0)
	{}

    /*
    inline ss_vect(const T& t, std::vector<double> vec){
	this->state_space.reserve(vec.size());
	for(size_t i=0; i<vec.size(); ++i){
	    this->state_space.push_back(vec[i]);
	}
    }
    */

    inline ss_vect(const T& x, const T& px, const T& y, const T& py, const T& delta, const T& ct)
	: state_space{x, px, y, py, delta, ct}
	{ this->checkSize(this->state_space); }
/*
	this->state_space.reserve(6);
	this->state_space.push_back(x);
	this->state_space.push_back(px);
	this->state_space.push_back(y);
	this->state_space.push_back(py);
	this->state_space.push_back(delta);
	this->state_space.push_back(ct);
	}
*/

    inline ss_vect(const std::vector<T>& vec)
	: state_space(vec)
	{  this->checkSize(this->state_space); }

    inline void operator+= (const ss_vect<T>& o){ this->inplace_op(o, [] (T& t, const T&     o){ t += o;}); };
    inline void operator-= (const ss_vect<T>& o){ this->inplace_op(o, [] (T& t, const T&     o){ t -= o;}); };
    /*
    inline void operator*= (const ss_vect<T>& o){ this->inplace_op(o, [] (T& t, const T&     o){ t *= o;}); };
    inline void operator/= (const ss_vect<T>& o){ this->inplace_op(o, [] (T& t, const T&     o){ t /= o;}); };
    */

    inline void operator+= (const double      o){ this->inplace_op(o, [] (T& t, const double o){ t += o;}); };
    inline void operator-= (const double      o){ this->inplace_op(o, [] (T& t, const double o){ t -= o;}); };
    inline void operator*= (const double      o){ this->inplace_op(o, [] (T& t, const double o){ t *= o;}); };
    inline void operator/= (const double      o){ this->inplace_op(o, [] (T& t, const double o){ t /= o;}); };

    inline void set_zero(void);/*{
	std::for_each(this->state_space.begin(), this->state_space.end(), [](gtpsa::tpsa& v){v = 0.0;});
	}*/

    inline void set_identity(void){
	throw std::runtime_error("set_identity only implemented for tpsa");
	//
    }
    inline auto size(void) const  { return this->state_space.size(); }

#ifndef GTSPA_ONLY_OPTIMISED_OPS
    inline ss_vect<T> operator + (const ss_vect<T>& o) const { auto r = this->clone(); r += o; return r; }
    inline ss_vect<T> operator - (const ss_vect<T>& o) const { auto r = this->clone(); r -= o; return r; }
    /*
    inline ss_vect<T> operator * (const ss_vect<T>& o) const { auto r = this->clone(); r *= o; return r; }
    inline ss_vect<T> operator / (const ss_vect<T>& o) const { auto r = this->clone(); r /= o; return r; }
    */

    inline ss_vect<T> operator + (const double      o) const { auto r = this->clone(); r += o; return r; }
    inline ss_vect<T> operator - (const double      o) const { auto r = this->clone(); r -= o; return r; }
    inline ss_vect<T> operator * (const double      o) const { auto r = this->clone(); r *= o; return r; }
    inline ss_vect<T> operator / (const double      o) const { auto r = this->clone(); r /= o; return r; }
    /*
    ss_vect(const ss_vect& o) {
	this->state_space.reserve(o.state_space.size());
	std::copy(o.state_space.begin(), o.state_space.end(), std::back_inserter(this->state_space));
    }
    */
#else
#endif /* GTSPA_ONLY_OPTIMISED_OPS */
    ss_vect(const ss_vect& o) = delete;

    inline ss_vect(const ss_vect<T>&& o) noexcept : state_space(std::move(o.state_space)) {};
    inline ss_vect<T>& operator=(const ss_vect<T>&& o) noexcept {
	if (this == &o) {
	    return *this;
	}
	this->state_space = std::move(o.state_space);
	// o->state_space = nullptr;
	return *this;
    }

    /**
     * @brief allocates empty objects with the same property as this object
     *
     * similar to clone, but it does not copy the content of this object
     *
     *  allocateLikeMe ?
     */
    inline ss_vect<T> allocateLikeMe(void) const {
	// ss_vect<T> nv = ss_vect(vec[0], vec[1], vec[2], vec[3], vec[4], vec[5]);
	throw std::runtime_error("Implement me but decide on method name first!");
    }
    inline ss_vect<T> newFromThis(void) const {
	throw std::runtime_error("Implement me but decide on method name first!");
    }
    /**
     * @todo implement a more flexible clone
     */
    inline ss_vect<T> clone(void) const {
	const std::vector<T>& vec = this->state_space;
	this->checkSize(vec);

	std::vector<T> nvec;
	nvec.reserve(this->state_space.size());
        std::transform(vec.begin(), vec.end(), std::back_inserter(nvec), [](const T& elem)  -> T { return elem.clone(); });

	return ss_vect(nvec);
    }

    inline void _copyInPlace(const ss_vect<T>& o) {
        std::transform(o.state_space.begin(), o.state_space.end(), this->state_space.begin(), [](const T& elem)  -> T { return elem.clone(); });
    }

    inline       T& operator[](const int i)       { return this->state_space[i]; }
    inline const T& operator[](const int i) const { return this->state_space[i]; }

    inline void cst(ss_vect<double> &r)     const {
	for(size_t i = 0; i < this->size(); ++i){ r[i] = this->state_space[i].cst(); }
	//std::transform(begin(), this->state_space.end(), r.state_space.begin(), [](T& elem) { return elem.cst();});
    }

    inline ss_vect<double> cst(void)        const { ss_vect<double> r(0e0); this->cst(r); return r; }

    inline void show(std::ostream& strm, int level=1, bool with_endl=true) const {
	for(size_t i= 0; i<this->state_space.size(); ++i){
	    this->state_space[i].show(strm, level);
	    strm << " ";
	}
	if(with_endl) {	strm << "\n";        }
    }

    inline arma::mat toMatrix(void){
	    throw std::runtime_error("only implemented for tps(a)");
    }

private:
    inline void checkSize(const std::vector<T>& vec) const {
	if (vec.size() != 6){
	    std::ostringstream strm;
	    strm << "Currently ss_vect implemented for length 6 got length  " << vec.size() <<  "state space vector size "<< this->state_space.size() ;
	    abort();
	    throw std::runtime_error(strm.str());
	}
    }
    // I am sure there it is a way to use std algorithms ...
    inline void inplace_op(const ss_vect<T>& o, void f(T&, const T&)){
	for(size_t i=0; i<this->state_space.size(); ++i){
	    f(this->state_space[i], o.state_space[i]);
	}
    }
    inline void inplace_op(const double o, void f(T&, const double)){
	for(size_t i=0; i<this->state_space.size(); ++i){
	    f(this->state_space[i], o);
	}
    }
    //
    std::vector<T> state_space;
};


template<typename T> inline
std::ostream& operator<<(std::ostream& strm, ss_vect<T>& s)
{
    s.show(strm, 1);
    return strm;
}


template<>
inline ss_vect<gtpsa::tpsa>::ss_vect(const gtpsa::tpsa&t,  const size_t n)
    : state_space(n, gtpsa::tpsa(t, mad_tpsa_same) )
{}
/*
{


    // review this interface !
    // better just to clone
    // not all tpsa need to address the same number of variables or knobs

    this->state_space.reserve(n);
    for(size_t i=0; i<n; ++i){
	this->state_space.push_back(std::move(gtpsa::tpsa(t, mad_tpsa_same)));
    }
}
*/

template<>
inline void ss_vect<double>::show(std::ostream& strm, int level, bool with_endl) const {
    for(size_t i= 0; i<this->state_space.size(); ++i){
	strm << " " << this->state_space[i];
    }
    if(with_endl) {	strm << "\n";        }
}

template<>
inline void ss_vect<gtpsa::tpsa>::show(std::ostream& strm, int level, bool with_endl) const {
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

template<>
inline void ss_vect<gtpsa::tpsa>::set_zero(void)
{
    std::for_each(this->state_space.begin(), this->state_space.end(), [](gtpsa::tpsa& v){ v.clear(); });
}
template<>
inline void ss_vect<double>::set_zero(void)
{
    for(auto &v : this->state_space){v=0.0;};
}

template<>
inline ss_vect<double> ss_vect<double>::clone(void) const {
	this->checkSize(this->state_space);
	size_t size = this->state_space.size();
	std::vector<double> nvec;
	nvec.reserve(size);
	std::transform(this->state_space.begin(), this->state_space.end(),  nvec.begin(), [](const double& o){double r = o; return r;});
	ss_vect<double> nv = ss_vect(nvec[0], nvec[1], nvec[2], nvec[3], nvec[4], nvec[5]);
	return nv;
    }

template<>
inline void ss_vect<gtpsa::tpsa>::set_identity(void)
{

    this->checkSize(this->state_space);
    const size_t n = this->state_space.size();
    for(size_t i = 0; i < n; ++i){
	auto& t_tpsa = this->state_space[i];
	t_tpsa.clear();
	std::vector<num_t> vec(n);
	for(auto&e : vec) e = 0;
	vec[i] = 1;
	t_tpsa.setv(1, vec);
	// t_tpsa.setsm(std::vector<idx_t>{int(i+1), 1}, 0e0, 1e0);
    }
    // throw std::runtime_error("gtpsa set identity needs to be implemented!");
    //std::for_each(this->state_space.begin(), this->state_space.end(), [](gtpsa::tpsa& v){ v.clear(); };
}

template<>
inline arma::mat ss_vect<gtpsa::tpsa>::toMatrix(void){

    arma::mat mat(this->size(), this->size());

    //mat.fill(NAN);
    mat.fill(0e0);

    for (unsigned int j = 0; j < this->size(); j++) {
	auto& t_tpsa = this->state_space[j];
	for (unsigned int k = 0; k < this->size(); k++){
	    auto tmp = std::vector<idx_t>{int(k+1), 1};
	    if(t_tpsa.getDescription()->isvalidsm(tmp)) {
		mat(j, k) = t_tpsa.getsm(tmp);
	    } else {
		std::cerr << "toMatrix invalid k "  << tmp[0] << ", o "<< tmp[1] <<"\n";
	    }
	}
    }
    // mat(j, tps_n-1) = get_m_ij(map, j+1, 0);
    // mat(tps_n-1, tps_n-1) = 1e0;
    return mat;

}

template<>
inline void ss_vect<double>::_copyInPlace(const ss_vect<double>& o) {
	std::transform(o.state_space.begin(), o.state_space.end(), this->state_space.begin(), [](const double& elem) -> double { return elem; });
}

} /* namespace gtpsa */




#endif /* _GTPSA_SS_VECT_H_ */
/*
 * Local Variables:
 * mode: c++
 * End:
 */
