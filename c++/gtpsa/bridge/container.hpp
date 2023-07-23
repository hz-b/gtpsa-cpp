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
    protected:
	using ContainerBridge = typename T::bridge_container_type;
        std::vector<TpsaBridge<T> *> m_vec;

    public:
        inline TpsaBridgeContainer(const std::vector<TpsaBridge<T> *> a_vec)
	    : m_vec(a_vec)
	    {}

        inline size_t size(void) const {
            return this->m_vec.size();
        }

        inline int getMaximumOrder(void) const {
            typename T::bridge_container_type cb(this->getBridgePtrs());
            return cb.getMaximumOrder();
        }

        inline int computeNorm(void) const {
            typename T::bridge_container_type cb(this->getBridgePtrs());
            return cb.computeNorm();
        }

	inline void rvec2fld(const typename T::tpsa_type& a) {
	    typename T::bridge_container_type mc_c(this->getBridgePtrs());
	    mc_c.rvec2fld(a.m_impl);
	}

	inline void fld2vec(typename T::tpsa_type * r) const {
	    typename T::bridge_container_type mc_c(this->getBridgePtrs());
	    mc_c.fld2vec(&r->m_impl);
	}

	inline void fgrad(typename T::tpsa_type * b, typename T::tpsa_type * r) const {
	    typename T::bridge_container_type mc_c(this->getBridgePtrs());
	    mc_c.fgrad(&b->m_impl, &r->m_impl);
	}

        inline void rliebra(const TpsaBridgeContainer<T> &ma, const TpsaBridgeContainer<T> &mb) {
            const ContainerBridge ma_c(ma.getBridgePtrs()), mb_c(mb.getBridgePtrs());
            ContainerBridge mc_c(this->getBridgePtrs());
            mc_c.rliebra(ma_c, mb_c);
	}

	inline void rexppb(const TpsaBridgeContainer<T> &ma, const TpsaBridgeContainer<T> &mb) {
            const ContainerBridge ma_c(ma.getBridgePtrs()), mb_c(mb.getBridgePtrs());
            ContainerBridge mc_c(this->getBridgePtrs());
            mc_c.rexppb(ma_c, mb_c);
        }

        inline void rlogpb(const TpsaBridgeContainer<T> &ma, const TpsaBridgeContainer<T> &mb) {
            const ContainerBridge ma_c(ma.getBridgePtrs()), mb_c(mb.getBridgePtrs());
            ContainerBridge mc_c(this->getBridgePtrs());
            mc_c.rlogpb(ma_c, mb_c);
        }

        inline void rcompose(const TpsaBridgeContainer<T> &ma, const TpsaBridgeContainer<T> &mb) {
            const ContainerBridge ma_c(ma.getBridgePtrs()), mb_c(mb.getBridgePtrs());
            ContainerBridge mc_c(this->getBridgePtrs());

            mc_c.rcompose(ma_c, mb_c);
        }

        inline void rcompose_jb(const TpsaBridgeContainer<T> &ma, const TpsaBridgeContainer<T> &mb) {
            const ContainerBridge ma_c(ma.getBridgePtrs()), mb_c(mb.getBridgePtrs());
            ContainerBridge mc_c(this->getBridgePtrs());

            mc_c.rcompose_jb(ma_c, mb_c);
        }

        inline void rminv(const TpsaBridgeContainer<T> &ma) {
            const ContainerBridge ma_c(ma.getBridgePtrs());
            ContainerBridge mc_c(this->getBridgePtrs());

            mc_c.rminv(ma_c);
        }

        inline void rpminv(const TpsaBridgeContainer<T> &ma, std::vector<idx_t>& select) {
            const ContainerBridge ma_c(ma.getBridgePtrs());
            ContainerBridge mc_c(this->getBridgePtrs());

            mc_c.rpminv(ma_c, select);
        }


    protected:
        inline std::vector<typename T::bridge_base_type *> getBridgePtrs(void) const {
            std::vector<typename T::bridge_base_type *> vec(this->m_vec.size());
            std::transform(this->m_vec.begin(), this->m_vec.end(), vec.begin(),
                           [](TpsaBridge<T> *ptr) { return (&(ptr->m_impl)); });
            return vec;
        }
        inline std::vector<const typename T::bridge_base_type *> getBridgePtrsConst(void) const {
            std::vector<typename T::bridge_base_type *> vec(this->m_vec.size());
            std::transform(this->m_vec.begin(), this->m_vec.end(), vec.begin(),
                           [](const TpsaBridge<T> *ptr) { return (&(ptr->m_impl)); });
            return vec;
        }

    };

    /**
     * tpsa_bridge instead of templated one
     *
     * Todo:
     *    Find a proper pattern for handling these down casts
     */
    class TTpsaBridgeContainer : public TpsaBridgeContainer<gtpsa::TpsaTypeInfo> {
	/* warning ... this vector required for this down casting step */
	std::vector<tpsa_bridge *> m_vec_t;
	using base = TpsaBridgeContainer<gtpsa::TpsaTypeInfo>;
	using base_ptr = TpsaBridge<gtpsa::TpsaTypeInfo>;

    public:
	inline TTpsaBridgeContainer(const std::vector<tpsa_bridge *> a_vec)
	    : TpsaBridgeContainer<gtpsa::TpsaTypeInfo>({})
	    , m_vec_t(a_vec)
	    {
		auto vec = this->getBridgePtrs();
		m_vec = vec;
	    }

    protected:
        inline std::vector<base_ptr *> getBridgePtrs(void) {
            std::vector<base_ptr *> vec(this->m_vec_t.size());
            std::transform(this->m_vec_t.begin(), this->m_vec_t.end(), vec.begin(),
                           [](auto *ptr) { return ((base_ptr*)(ptr)); });
            return vec;
        }
	/*
        inline auto getBridgePtrsConst(void) const {
            std::vector<const mad::TpsaWrapper *> vec(this->size());
            std::transform(this->m_vec.begin(), this->m_vec.end(), vec.begin(),
                           [](const TpsaBridge<TpsaTypeInfo> *ptr) { return (&(ptr->m_impl)); });
            return vec;
        }
	*/
    };

}
#endif //GTPSA_BRIDGE_CONTAINER_H
