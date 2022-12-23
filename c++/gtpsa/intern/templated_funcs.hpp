/* -*- c++ -*- */
/*
 * Utility functions for handling the different tpsa operators or functions
 */
#ifndef _GTPSA_TPSA_WITH_TEMPLATED_FUNCS_H_
#define _GTPSA_TPSA_WITH_TEMPLATED_FUNCS_H_ 1

namespace gtpsa {
    /*
     * functions that bridge to the adaptor
     */
    /*
     * functions that create the return object
     */
    template <typename Interface>
    inline Interface apply(const Interface& i,
			   void(f)(const Interface&, Interface* r)) {
	Interface r = i.newFromThis();
	f(i, &r);
	return r;
    }

    template <typename Interface>
    inline Interface apply2(const Interface& a, const Interface& b,
			    void(f)(const Interface& a, const Interface& b, Interface* r)) {
	Interface r = a.newFromThis();
	f(a, b, &r);
	return r;
    }

    template <typename Interface, typename base_t>
    inline Interface apply2_base(const Interface& a, const base_t b,
				 void(f)(const Interface& a, const base_t b, Interface* r)) {
	Interface r = a.newFromThis();
	f(a, b, &r);
	return r;
    }


} // namespace gtpsa

#endif /* _GTPSA_TPSA_WITH_TEMPLATED_FUNCS_H_ */
