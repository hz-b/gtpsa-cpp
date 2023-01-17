#include <stdexcept>
#include <iostream>
#include <complex>
#include <ccomplex>
#include <typeinfo>
#include <vector>

struct tpsa_t;
struct ctpsa_t;

typedef double num_t;
typedef double _Complex  cnum_t;

template<typename T, typename B>
struct _GTpsaBaseInfo
{
	using ptr_type = typeof(T);
	using base_type = typeof(B);

};

struct  TpsaBaseInfo: public _GTpsaBaseInfo<tpsa_t, num_t>
{
};

struct  CTpsaBaseInfo: public _GTpsaBaseInfo<ctpsa_t, cnum_t>
{
};

template<typename T, typename = typename T::ptr_type, typename = typename T::base_type>
struct _TpsaExt;


template<typename T, typename = typename T::ptr_type, typename = typename T::base_type>
struct _TpsaBase
{
	using base_type  = typename T::base_type;
	inline _TpsaBase  operator +  ( const base_type o) const { }

	friend class _TpsaExt<T>;

};

template<typename T, typename , typename>
struct _TpsaExt
{
};


struct Tpsa : public _TpsaBase<TpsaBaseInfo>
{
	using base_type  = typeof (num_t);
};


struct CTpsa : public _TpsaBase<CTpsaBaseInfo>
{
	using base_type  = typeof(cnum_t);
};

template<typename T>
T foo(T& a, T& b);

template<typename T, typename = typename T::base_type>
T foo(T& a, typename T::base_type& b);


template<typename T>
T foo(T& a, T& b){}

template<typename T, typename = typename T::base_type>
T foo(T& a, typename T::base_type& b){}


template<typename T>
struct ss_vect
{
	auto f(T& o) { T tmp = o; return tmp; }


	void cst(std::vector<typename T::base_type>& vec) {};
	auto cst()   {
		typename T::base_type tmp;
		return tmp;
	}
};

int main(int argc, char *argv[])
{

	ss_vect<Tpsa> vec ;
	vec.cst();

	typename Tpsa::base_type bb;
	Tpsa a, b;

	foo(a, b);
	foo(a, bb);
	//foo<>()
	//ss_vect<tpsa<std::complex<double>>> cvec ;
	return 0;
}
