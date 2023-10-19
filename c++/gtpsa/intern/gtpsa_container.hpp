//
// Created by mfp on 24.11.22.
//

#ifndef _GTPSA_CONTAINER_H_
#define _GTPSA_CONTAINER_H_

#include <algorithm>
#include <vector>
#include <gtpsa/bridge/bridge.hpp>
#include <gtpsa/bridge/container.hpp>

namespace gtpsa {
  template<typename B, class T>
  struct FilterBasePointers{
    std::vector<TpsaBridge<T> *> as_non_const (std::vector<B>& v) const {
      std::vector<TpsaBridge<T> *> vec;
      std::transform(v.begin(), v.end(), std::back_inserter(vec),
		     [](B& ptr) { return (&ptr); });
      return vec;
    }

    std::vector<TpsaBridge<T> *> as_const (const std::vector<B>& v)  const {
      std::vector<TpsaBridge<T> *> vec;
      std::transform(v.begin(), v.end(), std::back_inserter(vec),
		     [](const B& ptr) { return (B*)(&ptr); });
      return vec;
    }
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
#if 0
  template<typename B, class T>
  class _Container {
  public:
    inline size_t size(void) const { return this->m_vec.size(); }

    const std::vector<B> getVector(void) const { }

    inline int getMaximumOrder(void) const {
    }
  };

  template<typename B, class T>
  class ConstContainer : public _Container<B, T> {
    const std::vector<B> &m_vec;

  public:
    inline ConstContainer(const std::vector<B> &t_vec)
      : m_vec(t_vec) {}

    const std::vector<B>  getVector(void) const  { return this->m_vec; }
  };

  template<typename B, class T>
  class Container : public _Container<B, T> {
    std::vector<B> &m_vec;

  public:
    inline Container(std::vector<B> &t_vec)
      : m_vec(t_vec) {}

    const std::vector<B> getVector(void) const { return this->m_vec; }

    inline void rcompose(Container &ma, Container &mb) {

      using bridge_container_type = TpsaBridgeContainer<T>;
      const bridge_container_type
	ma_b(ma.getBasePtrs()), mb_b(mb.getBasePtrs());
      bridge_container_type mc_b(this->getBasePtrs());

      mc_b.rcompose(ma_b, mb_b);

    }
  private:
    std::vector<TpsaBridge<T> *> getBasePtrs() const {
      return getBasePtrs<B, T>(this->m_vec);
    }
  };
#endif

} // namespace gtpsa

#endif //_GTPSA_CONTAINER_H_
