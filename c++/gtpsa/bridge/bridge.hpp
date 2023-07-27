
/* -*- c++ -*- */
#ifndef _GTPSA_TPSA_BRIDGE_H_
#define _GTPSA_TPSA_BRIDGE_H_ 1
namespace gtpsa {

  /*
   * forward declaration for declaring as friend class
   *
   *  some functions e.g. compose use not a single tpsa object but a compound
   *  their of. This class takes care
   *  of combining the information to one
   */
  template<class T, typename = typename T::bridge_type,
	   typename = typename T::base_type, typename = typename T::ptr_type>
  class TpsaBridgeContainer;

  /**
   * @brief bridge to the mad (g)tpsa implementation
   */
  template<class T, typename = typename T::bridge_type,
	   typename = typename T::base_type, typename = typename T::ptr_type>
  class TpsaBridge
  {
    friend class TpsaBridgeContainer<T>;
    //protected:
  public:
    /*
     * the implementation to bridge, access needed in subclasses
     *
     */
    typename T::bridge_type m_impl;

  public:

    using base_type = typename T::base_type;
    using bridge_type = typename T::bridge_type;

    inline TpsaBridge(std::shared_ptr<mad::desc> desc, const ord_t mo)
      : m_impl(desc, mo)
    { }

    inline TpsaBridge(const TpsaBridge<T>& t, const ord_t mo)
      :  m_impl(t.m_impl, mo)
    { }

#ifndef GTSPA_ONLY_OPTIMISED_OPS
    inline TpsaBridge(const TpsaBridge<T>& o)
      : m_impl(o.m_impl)
    { }
#else /* GTSPA_ONLY_OPTIMISED_OPS */
    inline TpsaBridge(const TpsaBridge<T>& o) = delete;
#endif /* GTSPA_ONLY_OPTIMISED_OPS */

    /**
     * move ctor
     */
    // inline TpsaBridge<T>(TpsaBridge<T>&& o)                   = default;
    //  noexcept :  ltm(std::move(o.tm)), t_desc(std::move(o.t_desc)) { };
    //inline TpsaBridge<T>& operator= (TpsaBridge<T> && o)      = default;


    inline TpsaBridge<T> clone(void) const
    { TpsaBridge<T> res(*this, mad::init::same);
      res._copyInPlace(*this);
      return res; }

    /* O----------------------------------------------------------------O
       | methods required for the private bridge                        |
       |                                                                |
       | @todo review if the vector to buffer conversion should not be  |
       |       part of the functions on this side                       |
       |                                                                |
    */
    /*
     * @brief create a new instance with similar order / desc property as this
     *
     * required as the constructor is now only be used within this class or
     * base class. Avoids that many functions like process and so on need to
     * be declared as friend classes
     *
     * not needed as this class adds functionality to the base class.
     * Still kept as it seems to make further code easier to understand....
     */
    inline void _copyInPlace(const TpsaBridge<T>& o)
    { this->m_impl._copyInPlace(o.m_impl); }

    inline TpsaBridge<T> newFromThis(void) const {
      return TpsaBridge<T>(*this, mad::init::same);
    }


    /* a method to implement the different arithmentic functions */
    inline void apply_with_return_object
    (const TpsaBridge& o, void(f)(const bridge_type& a, bridge_type* r)) {
      f(o.m_impl, &this->m_impl);
    }

#if 0
    inline void apply2_with_return_object
    (const TpsaBridge& a, const TpsaBridge& b,
     void(f)(const bridge_type& a, const bridge_type& b, bridge_type* r)) {
      const bridge_type& a_impl = a.m_impl, &b_impl = b.m_impl;
      bridge_type* r_impl = &this->m_impl;
      f(a_impl, b_impl, r_impl);
    }

    inline void apply2_base_with_return_object
    (const TpsaBridge& a, const base_type& b,
     void(f)(const bridge_type& a, const base_type& b, bridge_type* r)) {
      const bridge_type& a_impl = a.m_impl;
      bridge_type* r_impl = &this->m_impl;
      f(a_impl, b, r_impl);
    }
#endif

    inline std::pair<idx_t,base_type> cycle
    (const idx_t i, std::vector<ord_t>* m) const {
      base_type v = NAN;
      idx_t r = this->m_impl.cycle(i, m, &v);
      return std::pair<idx_t, base_type>(r, v);
    }
    inline auto getDescription(void) const
    { return this->m_impl.getDescription(); }

    inline auto uid(int32_t uid)
    { return this->m_impl.uid(uid); }
    inline auto length() const { return this->m_impl.len(); }
    inline auto order() const { return this->m_impl.ord(); }
    inline auto name() const { return this->m_impl.nam(); }
    inline auto setName(std::string s) { this->m_impl.setnam(s); }
    inline void clear() { this->m_impl.clear(); }
    inline int  isNull() const { return this->m_impl.isNull(); }

    /**
     * @brief sparse mono "[(i, o)]*"
     */
    /**
     * @brief indexing / monomials (return idx_t = -1 if invalid)
     */
    inline auto mono(idx_t i, std::vector<ord_t> *m)
    { return this->m_impl.mono(i, m); }

    /**
     *  @brief string mono "[0-9]*"
     */
    inline auto index(std::string s) const { return this->m_impl.idxs(s); }
    inline auto index(const std::vector<ord_t>& m) const
    { return this->m_impl.idxm(m); }

    inline auto get(void) const
    { return this->m_impl.get0();  }
    inline auto get(const idx_t i) const
    { return this->m_impl.geti(i); }
    inline auto get(std::string s) const
    { return this->m_impl.gets(s); }
    inline auto get(const std::vector<ord_t>& m) const
    { return this->m_impl.getm(m); }
    /**
     * @brief sparse mono [(i,o)]
     * @note not using overload ... could be misleading
     */
    inline auto indexsm(const std::vector<int> m)
    { return  this->m_impl.idxsm(m); }

    /*
     */
    inline auto getsm(const std::vector<int>& m) const
    { return this->m_impl.getsm(m); }

    /*
     * for pybind 11
     */
    inline void _set0(base_type a, base_type b) { this->set(a, b);}
    inline void _setm
    (const std::vector<ord_t>& m, base_type a, base_type b)
    { this->set(m, a, b);}

    /**
     * @brief a*x[0]+b
     */
    inline void set(base_type a, base_type b) { this->m_impl.set0(  a, b); }

    /**
     * @brief a*x[i]+b
     */
    inline void set(const idx_t i, base_type a, base_type b)
    { this->m_impl.seti(i, a, b); }
    /**
     * @brief a*x[m]+b
     */
    inline void set(const std::string& s, base_type a, base_type b)
    { this->m_impl.sets(s, a, b); }
    /**
     * @brief a*x[m]+b
     */
    inline void set(const std::vector<ord_t>& m , base_type a, base_type b)
    { this->m_impl.setm(m, a, b); }

    /**
     * @brief a*x[m]+b, sparse mono [(i, o)]
     * @todo vector of pairs?
     */
    inline auto setsm(const std::vector<int> m, base_type a, base_type b)
    { this->m_impl.setsm(m, a, b); }

    inline void getv(idx_t i, std::vector<base_type> *v) const
    { this->m_impl.getv(i, v); }
    inline void setv(idx_t i, const std::vector<base_type> &v)
    { this->m_impl.setv(i, v); }


    inline void setVariable
    (const base_type v, const idx_t iv= 0, const base_type scale = 0) {
      this->m_impl.setvar(v, iv, scale);
    }

    inline auto rgetOrder (const TpsaBridge& src, const size_t order)
    { return this->m_impl.rgetOrder(src.m_impl, order); };
    /**
     *
     * @todo interface needs to be reviewed
     */
    inline void print
    (str_t name = nullptr, num_t eps = 0, int nohdr = 0,
     FILE *stream = nullptr) const {
      this->m_impl.print(name, eps, nohdr, stream);
    }

    inline auto cst(void) const { return this->get(); }

    /* results stored in this */
    inline void pow(const TpsaBridge<T>& a, const TpsaBridge<T>& b)
    { this->m_impl.pow(a.m_impl, b.m_impl);  }
    inline void pow(const TpsaBridge<T>& a, const int i)
    { this->m_impl.pow(a.m_impl, i);  }
    inline void pow(const TpsaBridge<T>& a, const base_type v)
    { this->m_impl.pow(a.m_impl, v);  }


    /* results stored in this */
    inline void add(const TpsaBridge<T>& a, const TpsaBridge<T>& b)
    { this->m_impl.add(a.m_impl, b.m_impl); }
    inline void dif(const TpsaBridge<T>& a, const TpsaBridge<T>& b)
    { this->m_impl.dif(a.m_impl, b.m_impl); }
    inline void sub(const TpsaBridge<T>& a, const TpsaBridge<T>& b)
    { this->m_impl.sub(a.m_impl, b.m_impl); }
    inline void mul(const TpsaBridge<T>& a, const TpsaBridge<T>& b)
    { this->m_impl.mul(a.m_impl, b.m_impl); }
    inline void div(const TpsaBridge<T>& a, const TpsaBridge<T>& b)
    { this->m_impl.div(a.m_impl, b.m_impl); }

    inline void acc(const TpsaBridge<T>& a, const num_t v)
    { this->m_impl.acc(a, v); }
    inline void scl(const TpsaBridge<T>& a, const num_t v)
    { this->m_impl.scl(a, v); }
    inline void inv(const TpsaBridge<T>& a, const num_t v)
    { this->m_impl.inv(a, v); }
    inline void invsqrt(const TpsaBridge<T>& a, const num_t v)
    { this->m_impl.invsqrt(a, v); }

    inline void rderiv(const TpsaBridge<T>& o, const int iv)
    { this->m_impl.rderiv(o.m_impl, iv); }
    inline void rinteg(const TpsaBridge<T>& o, const int iv)
    { this->m_impl.rinteg(o.m_impl, iv);   }

    /* |                                                                |
       O----------------------------------------------------------------O */

  };

} // namespace gtpsa

#endif //  _GTPSA_TPSA_BRIDGE_H_
