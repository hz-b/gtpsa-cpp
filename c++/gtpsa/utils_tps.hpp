#ifndef _GTPSA_UTIL_TPS_H_
#define _GTPSA_UTIL_TPS_H_ 1

#include <tps/tps_type.h>
namespace gtpsa {
    template<>
    inline tps same_as_instance(const tps& v){ return tps(); }
} // namespace gtpa

#endif /* _GTPSA_UTIL_TPS_H_ */
