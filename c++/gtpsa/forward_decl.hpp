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

extern "C" {
#include <mad_mono.h>
}

namespace gtpsa {


    template<typename T, typename B, typename Bridge, typename BridgeBase, typename BridgeContainer>
    struct GTpsaTypeInfo
    {
	using ptr_type =  T;
	using base_type =  B;
	using bridge_type = Bridge;
    using bridge_base_type = BridgeBase; // required ? just need to extract pointer in the bridge container later on
    using bridge_container_type = BridgeContainer;
    };


    namespace mad {
	class _TpsaWrapper;
	class _CTpsaWrapper;
	class CTpsaWrapper;
	/*
	 *@brief how to initalise new object if an other is presented
	 */
	enum init {
	    default_ = ord_t(-1),
	    same     = ord_t(-2)
	};
	class desc;
    }

    class tpsa;
    class ctpsa;


}
#endif /* _GTPSA_FORWARED_DECL_H_ */
