#ifndef _GTPSA_SS_VECT_H_
#define _GTPSA_SS_VECT_H_ 1
#include <gtpsa/tpsa.hpp>
#include <algorithm>
#include <ostream>
#include <sstream>
#include <vector>

namespace gtpsa {
/**
 * state space, phase space vector
 *
 * assuming that all are of the same length ...
 */
template<typename T>
class ss_vect{

public:
    inline ss_vect(const T& t, const size_t n=6){
	this->state_space.reserve(n);
	for(size_t i=0; i<n; ++i){
	    this->state_space.push_back(0e0);
	}
    }
    /*
    inline ss_vect(const T& t, std::vector<double> vec){
	this->state_space.reserve(vec.size());
	for(size_t i=0; i<vec.size(); ++i){
	    this->state_space.push_back(vec[i]);
	}
    }
    */

    inline ss_vect(const T& x, const T& px, const T& y, const T& py, const T& delta, const T& ct){
	this->state_space.reserve(6);
	this->state_space.push_back(x);
	this->state_space.push_back(px);
	this->state_space.push_back(y);
	this->state_space.push_back(py);
	this->state_space.push_back(delta);
	this->state_space.push_back(ct);

    }

    /*
    inline ss_vect(const std::vector<T>& vec){
	this->state_space.reserve(vec.size());
	std::copy(vec.begin(), vec.end(), std::back_inserter(this->state_space));
    }
    */
    inline void operator+= (const ss_vect<T>& o){ this->inplace_op(o, [] (T& t, const T& o){ t += o;}); };
    inline void operator-= (const ss_vect<T>& o){ this->inplace_op(o, [] (T& t, const T& o){ t -= o;}); };
    inline void operator*= (const ss_vect<T>& o){ this->inplace_op(o, [] (T& t, const T& o){ t *= o;}); };
    inline void operator/= (const ss_vect<T>& o){ this->inplace_op(o, [] (T& t, const T& o){ t /= o;}); };


    inline void set_zero(void);/*{
	std::for_each(this->state_space.begin(), this->state_space.end(), [](gtpsa::tpsa& v){v = 0.0;});
	}*/

    inline void set_identity(void){
	throw std::runtime_error("set_identity only implemented for tpsa");
	//
    }

#ifndef GTSPA_ONLY_OPTIMISED_OPS
    inline ss_vect<T> operator + (const ss_vect<T>& o) const { auto r = this->clone(); r += o; return r; }
    inline ss_vect<T> operator - (const ss_vect<T>& o) const { auto r = this->clone(); r -= o; return r; }
    inline ss_vect<T> operator * (const ss_vect<T>& o) const { auto r = this->clone(); r *= o; return r; }
    inline ss_vect<T> operator / (const ss_vect<T>& o) const { auto r = this->clone(); r /= o; return r; }

    ss_vect(const ss_vect& o) {
	this->state_space.reserve(o.state_space.size());
	std::copy(o.state_space.begin(), o.state_space.end(), std::back_inserter(this->state_space));
    }
#else
    ss_vect(const ss_vect& o) = delete;
#endif /* GTSPA_ONLY_OPTIMISED_OPS */

    inline ss_vect(const ss_vect<T>&& o) : state_space(std::move(o.state_space)) {};
    inline ss_vect<T>& operator=(const ss_vect<T>&& o)  {return *this;};

    inline ss_vect<T> clone(void) const {
	const std::vector<T>& vec = this->state_space;
	if (vec.size() != 6){
	    std::ostringstream strm;
	    strm << "method clone implemented for length 6 got length  " << vec.size() <<  "state space vector size "<< this->state_space.size() ;
	    abort();
	    throw std::runtime_error(strm.str());
	}
	ss_vect<T> nv = ss_vect(vec[0], vec[1], vec[2], vec[3], vec[4], vec[5]);
	// nv.state_space.reserve(nv.state_space.size());
	// std::copy(this->state_space.begin(), this->state_space.end(), nv.state_space));
	return nv;
    }

    inline void _copyInPlace(const ss_vect<T>& o) {
        std::transform(o.state_space.begin(), o.state_space.end(), this->state_space.begin(), [](const T& elem)  -> T { return elem.clone(); });
    }

    inline T& operator[](const int i)             { return this->state_space[i]; }
    inline const T& operator[](const int i) const { return this->state_space[i]; }

    inline void cst(ss_vect<double> &r)     const {
	std::transform(this->state_space.begin(), this->state_space.end(), r.state_space.begin(), [](T& elem) { return elem.cst();});

    };
    inline void show(std::ostream& strm, int level, bool with_endl=true) const {
	for(size_t i= 0; i<this->state_space.size(); ++i){
	    this->state_space[i].show(strm, level);
	    strm << " ";
	}
	if(with_endl) {	strm << "\n";        }
    }

private:
    // I am sure there it is a way to use std algorithms ...
    inline void inplace_op(const ss_vect<T>& o, void f(T&, const T&)){
	for(size_t i=0; i<this->state_space.size(); ++i){
	    f(this->state_space[i], o.state_space[i]);
	}
    }
    //
    std::vector<T> state_space;
};

template<typename T> inline
std::ostream& operator<<(std::ostream& strm, ss_vect<T>& s)
{
    s.show(strm, 0);
    return strm;
}


template<>
inline ss_vect<gtpsa::tpsa>::ss_vect(const gtpsa::tpsa&t,  const size_t n)
{
    this->state_space.reserve(n);
    for(size_t i=0; i<n; ++i){
	this->state_space.push_back(std::move(gtpsa::tpsa(t, mad_tpsa_same)));
    }
}


template<>
inline void ss_vect<double>::show(std::ostream& strm, int level, bool with_endl) const {
    for(size_t i= 0; i<this->state_space.size(); ++i){
	strm << " " << this->state_space[i];
    }
    if(with_endl) {	strm << "\n";        }
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
inline void ss_vect<gtpsa::tpsa>::set_identity(void)
{

    for(size_t i = 0; i < this->state_space.size(); ++i){
	auto& t_tpsa = this->state_space[i];
	t_tpsa.set(i, 0e0, 1e0);
    }
    // throw std::runtime_error("gtpsa set identity needs to be implemented!");
    //std::for_each(this->state_space.begin(), this->state_space.end(), [](gtpsa::tpsa& v){ v.clear(); };
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
