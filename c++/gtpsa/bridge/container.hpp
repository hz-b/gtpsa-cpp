//
// Created by mfp on 28.11.22.
//

#ifndef GTPSA_BRIDGE_CONTAINER_H
#define GTPSA_BRIDGE_CONTAINER_H


#include <vector>
// bridge container forward declaration
#include <gtpsa/bridge/bridge.hpp>

namespace gtpsa {
    /**
     * @brief  support methods using a container or sequence of (c)tpsa objects
     *
     *  This class is responsible to create container of the bridge instances
     *  of the tpsa object.
     *
     *  See TpsaBridge for the functionality of the contained object.
     *
     *  @warning This class assumes that the lifetime of the objects pointed to
     *           is taken care of the user of this class.
     *
     * This class is not expected to be used directly but by
     *  a gtpsa::Container (which is also internal to gtpsa's c++ wrapper).
     */
    template<class T, typename, typename, typename>
    class TpsaBridgeContainer {
        std::vector<TpsaBridge<T> *> m_vec;

    public:
        inline TpsaBridgeContainer(const std::vector<TpsaBridge<T> *> a_vec)
                : m_vec(a_vec) {}

        inline size_t size(void) const {
            return this->m_vec.size();
        }

        inline int getMaximumOrder(void) const {
            typename T::bridge_container_type cb(this->getBridgePtrs());
            return cb.getMaximumOrder();
        }

        inline void rliebra(const TpsaBridgeContainer<T> &ma, const TpsaBridgeContainer<T> &mb) {
            using ContainerBridge = typename T::bridge_container_type;
            const ContainerBridge ma_c(ma.getBridgePtrs()), mb_c(mb.getBridgePtrs());
            ContainerBridge mc_c(this->getBridgePtrs());
            mc_c.rliebra(ma_c, mb_c);
	}

	inline void rexppb(const TpsaBridgeContainer<T> &ma, const TpsaBridgeContainer<T> &mb) {
            using ContainerBridge = typename T::bridge_container_type;
            const ContainerBridge ma_c(ma.getBridgePtrs()), mb_c(mb.getBridgePtrs());
            ContainerBridge mc_c(this->getBridgePtrs());
            mc_c.rexppb(ma_c, mb_c);
	}

	inline void rlogpb(const TpsaBridgeContainer<T> &ma, const TpsaBridgeContainer<T> &mb) {
            using ContainerBridge = typename T::bridge_container_type;
            const ContainerBridge ma_c(ma.getBridgePtrs()), mb_c(mb.getBridgePtrs());
            ContainerBridge mc_c(this->getBridgePtrs());
            mc_c.rlogpb(ma_c, mb_c);
	}

        inline void rcompose(const TpsaBridgeContainer<T> &ma, const TpsaBridgeContainer<T> &mb) {
            using ContainerBridge = typename T::bridge_container_type;
            const ContainerBridge ma_c(ma.getBridgePtrs()), mb_c(mb.getBridgePtrs());
            ContainerBridge mc_c(this->getBridgePtrs());

            mc_c.rcompose(ma_c, mb_c);
        }

    protected:
        inline std::vector<typename T::bridge_base_type *> getBridgePtrs(void) const {
            std::vector<typename T::bridge_base_type *> vec(this->size());
            std::transform(this->m_vec.begin(), this->m_vec.end(), vec.begin(),
                           [](TpsaBridge<T> *ptr) { return (&(ptr->m_impl)); });
            return vec;
        }
        inline std::vector<const typename T::bridge_base_type *> getBridgePtrsConst(void) const {
            std::vector<typename T::bridge_base_type *> vec(this->size());
            std::transform(this->m_vec.begin(), this->m_vec.end(), vec.begin(),
                           [](const TpsaBridge<T> *ptr) { return (&(ptr->m_impl)); });
            return vec;
        }

    };
}
#endif //GTPSA_BRIDGE_CONTAINER_H
