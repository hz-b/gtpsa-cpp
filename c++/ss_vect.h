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
    inline ss_vect(T& t, const size_t n=7){
	this->state_space.reserve(n);
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

    ss_vect(const ss_vect& o) = delete;
    inline ss_vect(const ss_vect&& o) : state_space(std::move(o.state_space)) {};

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
inline ss_vect<gtpsa::tpsa>::ss_vect(gtpsa::tpsa&t,  const size_t n)
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

} /* namespace gtpsa */
#endif /* _GTPSA_SS_VECT_H_ */
/*
 * Local Variables:
 * mode: c++
 * End:
 */
