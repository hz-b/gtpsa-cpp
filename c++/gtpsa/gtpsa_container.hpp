//
// Created by mfp on 24.11.22.
//

#ifndef _GTPSA_CONTAINER_H_
#define _GTPSA_CONTAINER_H_

#include <algorithm>
#include <vector>
#include <gtpsa/bridge.hpp>
#include <gtpsa/bridge_container.hpp>

namespace gtpsa {
    // B tpsa type ...
    //class GTpsaContainerTypeInfo
    template<typename Base, typename Bridge, typename BridgeContainer>
    struct GTpsaBridgeContainerInfo {
        using base_type = Base;    ///< typically tpsa or ctpsa
        using bridge_type = Bridge; ///< typically templated TpsaBridge<> / tpsa_bridge
        using bridge_container_type = BridgeContainer; ///< typically templated TpsaBridgeContainer
    };

    /**
     * @brief  support methods using a container or sequence of (c)tpsa objects
     *
     *  This class creates bridge containers which are used for supporting
     *  methods requiring several tpsa objects
     *
     *  The implementation of this class differes from `TpsaBridgeContainer`
     *  and `GTPSA_CLASS(ContainerWrapper)` as it expects a reference to a
     *  vector of (c)tpsa object. In that manner it can be used as a
     *  delegator to a state space vector or differential map implementation.
     *  Thus creating pointers to (c)tpsa objects and preparing the buffers
     *  that the gtpsa C library expects is singled out from the state space
     *  vector.
     *
     *  @warning This class assumes that the lifetime of the objects pointed to
     *           is taken care of the user of this class.
     *
     * This class is not expected to be used directly but as a delegator to a
     * state space or differential map implementation.
     */
    template<typename B, class T>
    class Container {
        using base_type = B; ///< base type of the state space vector typically tpsa or ctpsa
        std::vector<base_type> &m_vec;


    public:
        inline Container(std::vector<base_type> &t_vec)
                : m_vec(t_vec) {}

        inline size_t size(void) const { return this->m_vec.size(); }

        inline void rcompose(Container &ma, Container &mb) {

            using bridge_container_type = TpsaBridgeContainer<T>;
            bridge_container_type ma_b(ma.getBasePtrs()), mb_b(mb.getBasePtrs());
            bridge_container_type mc_b(this->getBasePtrs());

            mc_b.rcompose(ma_b, mb_b);

        }

    private:

        std::vector<TpsaBridge<T> *> getBasePtrs(void) const {
            std::vector<TpsaBridge<T> *> vec;
            std::transform(this->m_vec.begin(), this->m_vec.end(), std::back_inserter(vec),
                           [](base_type &ptr) { return (&ptr); });
            return vec;
        }
    };


} // namespace gtpsa

#endif //_GTPSA_CONTAINER_H_
