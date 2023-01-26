//
// Created by mfp on 28.11.22.
//
// guard defines omitted intentionally
// #ifndef  GTPSA_MAD_GTPSA_CONTAINER_WRAPPER_H
// #define GTPSA_MAD_GTPSA_CONTAINER_WRAPPER_H

#include <vector>

namespace gtpsa::mad {

    /**
     * @brief support methods using a container or sequence of (c)tpsa objects
     *
     * These sequences are used to implement e.g. differential maps operators.
     *
     * @warning This class does only process vectors of pointers to objects.
     *          The user of this class is responsible to ensure that the
     *          objects live while they are used by this class.
     *
     *  This class is not expected to be used directly but by
     *  a TpsaBridgeContainer (which is also internal to gtpsa's c++ wrapper).
     *
     *  This class is responsible to extract the tpsa_t pointers of the different
     *   objects and put them in a buffer so that gtpsa can process them.
     */
    class GTPSA_CLASS(ContainerWrapper) {
        std::vector<GTPSA_CLASS(Wrapper) *> m_vec;

    public:
        inline GTPSA_CLASS(ContainerWrapper)(std::vector<GTPSA_CLASS(Wrapper) *> a_vec)
                : m_vec(a_vec) {}

        inline size_t size(void) const {
            return this->m_vec.size();
        }

        inline int getMaximumOrder (void) const {
            auto tmp = this->getConstBufferPtrs();
            return GTPSA_METH(ordn)(tmp.size(), tmp.data());
        }

        inline void rcompose(const GTPSA_CLASS(ContainerWrapper) &ma, const GTPSA_CLASS(ContainerWrapper) &mb) {
            /*
             * following mad gtpsa documentation.
             *
             * gtpsa checks that internally. Review if leave it here too.
             * At time of implementation gtpsa's error handler was not properly implemented
             */
            if (ma.size() != this->size()) {
                std::stringstream strm;
                strm << "ma size" << ma.size() << " !=  mc size " << this->size();
                throw std::runtime_error(strm.str());
            }

            auto tmpa = ma.getConstBufferPtrs();
            auto tmpb = mb.getConstBufferPtrs();
            auto tmpc = this->getBufferPtrs();

            GTPSA_METH(compose)(tmpa.size(), tmpa.data(), tmpb.size(), tmpb.data(), tmpc.data());
        }

    protected:
        inline std::vector<GTPSA_PTR_T *> getBufferPtrs(void) const {
            std::vector<GTPSA_PTR_T *> vec(this->m_vec.size());
            std::transform(this->m_vec.begin(), this->m_vec.end(), vec.begin(),
                           [](GTPSA_CLASS(Wrapper) *elem) { return elem->getPtr(); }
            );
            return vec;
        }

        inline std::vector<const GTPSA_PTR_T *> getConstBufferPtrs(void) const {
            std::vector<const GTPSA_PTR_T *> vec(this->m_vec.size());
            std::transform(this->m_vec.begin(), this->m_vec.end(), vec.begin(),
                           [](const GTPSA_CLASS(Wrapper) *elem) { return elem->getPtr(); }
            );
            return vec;
        }
    };

} // namespace gtpsa::mad

// guard defines omitted intentionally
// #endif //GTPSA_MAD_GTPSA_CONTAINER_WRAPPER_H
