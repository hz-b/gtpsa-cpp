//
// Created by mfp on 11.01.23.
//

#ifndef GTPSA_MAD_FORWARD_DECL_HPP
#define GTPSA_MAD_FORWARD_DECL_HPP

extern "C" {
#include <mad_mono.h>
}

namespace gtpsa::mad {
    /*
     * forward declarations required as real and complex
     *  implementation befriend each other
     */
    class _TpsaWrapper;
    class _CTpsaWrapper;
    class CTpsaWrapper;
    class TpsaWrapper;

    /*
	 * @brief how to initialise new object if another is presented
     *
     * mad's gtpsa exports it as an opaque external value
	 */
    enum init {
        default_ = ord_t(-1),
        same     = ord_t(-2)
    };
    class desc;

} // namespace gtpsa::mad
#endif //GTPSA_MAD_FORWARD_DECL_HPP
