#ifndef _GTPSA_FORWARED_DECL_H_
#define _GTPSA_FORWARED_DECL_H_ 1

/* a non standard hack */
typedef bool _Bool;

extern "C" {
#include <mad_mono.h>
}

namespace gtpsa {


    template<typename T, typename B, typename Bridge>
    struct GTpsaTypeInfo
    {
	using ptr_type =  T;
	using base_type =  B;
	using bridge_type = Bridge;
    };

    //class TPSAWrapper;

    // class tpsa;
    // class ctpsa;

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
