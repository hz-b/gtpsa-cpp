//
// Created by mfp on 20.01.23.
//

#ifndef _GTPSA_VARIANT_FORWARD_DECL_HPP_
#define _GTPSA_VARIANT_FORWARD_DECL_HPP_

namespace gtpsa {
    // helper type for the visitor #4
    template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
    // explicit deduction guide (not needed as of C++20)
    template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

    // forward declarations for friend class
    class CTpsaOrComplex;
}
#endif /* _GTPSA_VARIANT_FORWARD_DECL_HPP_ */
