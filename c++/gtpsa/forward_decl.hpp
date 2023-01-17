#ifndef _GTPSA_FORWARED_DECL_H_
#define _GTPSA_FORWARED_DECL_H_ 1

#define __USE_C99_MATH

#include <stdbool.h>

#ifdef GTPSA_DEFINE_BOOL
extern "C" {
  /* a non standard hack */
  // typedef bool _Bool;
  typedef bool _Bool;

}
#endif

#include <gtpsa/mad/forward_decl.hpp>

#include <gtpsa/mad/forward_decl.hpp>

#include <gtpsa/mad/forward_decl.hpp>

#include <gtpsa/mad/forward_decl.hpp>

namespace gtpsa {


    template<typename T, typename B, typename D, typename Bridge, typename BridgeBase, typename BridgeContainer>
    struct GTpsaTypeInfo
    {
	using ptr_type =  T;
	using base_type =  B;
	using tpsa_type = D;
	using bridge_type = Bridge;
	using bridge_base_type = BridgeBase; // required ? just need to extract pointer in the bridge container later on
	using bridge_container_type = BridgeContainer;
    };

    class tpsa;
    class ctpsa;


}
#endif /* _GTPSA_FORWARED_DECL_H_ */
