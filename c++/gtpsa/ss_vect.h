#ifndef _GTPSA_SS_VECT_H_
#define _GTPSA_SS_VECT_H_ 1
#include <gtpsa/tpsa.hpp>
#include <algorithm>
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
    inline ss_vect(const T& t, const size_t n=7){
	this->state_space.reserve(n);
	for(size_t i=0; i<n; ++i){
	    this->state_space.push_back(0e0);
	}
    }

    inline void operator+= (const ss_vect<T>& o){ this->inplace_op(o, [] (T& t, const T& o){ t += o;}); };
    inline void operator-= (const ss_vect<T>& o){ this->inplace_op(o, [] (T& t, const T& o){ t -= o;}); };
    inline void operator*= (const ss_vect<T>& o){ this->inplace_op(o, [] (T& t, const T& o){ t *= o;}); };
    inline void operator/= (const ss_vect<T>& o){ this->inplace_op(o, [] (T& t, const T& o){ t /= o;}); };

    inline void set_zero(void){
	std::for_each(this->state_space.begin(), this->state_space.end(), [](gtpsa::tpsa& v){v = 0.0;});
    }

    inline void set_identity(void){
	throw std::runtime_error("set_identity only implemented for tpsa");
    }

#ifndef GTSPA_ONLY_OPTIMISED_OPS
    ss_vect(const ss_vect& o) {
	this->state_space.reserve(o.state_space.size());
	this->_copyInPlace(o);
    }
#else
#error "Currently supporting copy"
    ss_vect(const ss_vect& o) = delete;
#endif /* GTSPA_ONLY_OPTIMISED_OPS */

    inline ss_vect(const ss_vect<T>&& o) : state_space(std::move(o.state_space)) {};

    inline ss_vect<T> clone(void) {
	ss_vect<T> nv = ss_vect(this->state_space[0], this->state_space.size());
	nv._copyInPlace(*this);
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

template<>
inline ss_vect<gtpsa::tpsa>::ss_vect(const gtpsa::tpsa&t,  const size_t n)
{
    this->state_space.reserve(n);
    for(size_t i=0; i<n; ++i){
	this->state_space.push_back(std::move(gtpsa::tpsa(t, mad_tpsa_same)));
    }
}


template<>
inline void ss_vect<gtpsa::tpsa>::set_zero(void)
{
    std::for_each(this->state_space.begin(), this->state_space.end(), [](gtpsa::tpsa& v){ v.clear(); });
}

template<>
inline void ss_vect<gtpsa::tpsa>::set_identity(void)
{
    throw std::runtime_error("gtpsa set identity needs to be implemented!");
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
