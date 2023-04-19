#ifndef _GTPSA_DESC_H_
#define _GTPSA_DESC_H_ 1
#include <gtpsa/forward_decl.hpp>
extern "C" {
#include <mad_desc.h>
}

#include <memory>
#include <vector>
#include <ostream>
#include <sstream>
#include <string>
#include <iostream>

/**
 * gtpsa description object handling
 *
 * Description object contains precomputed coefficients. gtpsa will only
 * allocate a new object if its signature is different to the already known
 * ones.
 *
 * Furthermore generating (and thus also deleteing) description objects is not
 * thread safe. It is the user's responsibility that desc objects are created
 * (and deleted) in a thread safe manner.
 *
 * Currently the desc objects will not be deleted. Roughly a hundred objects
 * can be created.
 *
 *
 * A good overview of gtpsa is given by the original authors at
 * L. Deniau, C. I. Tomoiagă, "Generalised Truncated Power series algebra
 * for fast particle accelerator transport maps", MOPJE039
 * 6th International Particle Accelerator Conference IPAC2015,
 * Richmond, VA, USA
 */
namespace gtpsa::mad {


    /**
     * @brief manages ptr lifetime
     */
    class desc_mgr{
    public:
	inline const desc_t *  getPtr(void) const { return this->ptr; }
	friend class desc;
	inline desc_mgr(const desc_t *p) : ptr(p) { if (!p) { throw std::runtime_error("out of memory"); } }
	inline ~desc_mgr(void)                    {
            if(!this->ptr){
                std::cerr << "gtpsa::desc: would delete underlaying desc_t pointer a second time" << std::endl;
                return;
            }
#warning "gtpsa::desc: not deleting underlying gtpsa desc_t object. see comment in <gtpsa/desc.hpp>"
        //mad_desc_del(this->ptr);
        this->ptr=nullptr;
    }

    private:
	// inline desc_mgr(const desc& o)           = delete;
	desc_mgr(const desc_mgr& o)           = delete;
	desc_mgr& operator= (const desc_mgr&) = delete;

	const desc_t * ptr = nullptr;
    };

    /**
     * @ parameters of the description
     *
     * Inspired by python data classes
     */
    class desc_info {
	const int m_nv, m_np;
	const ord_t m_no, m_po;
	const std::vector<ord_t> m_orders;

    public:
	inline desc_info(int nv, ord_t no, int np, ord_t po, const std::vector<ord_t> orders)
	    : m_nv(nv)
	    , m_np(np)
	    , m_no(no)
	    , m_po(po)
	    , m_orders(orders)
	    {}

	inline auto getNumberOfVariables      ( void ) const { return this->m_nv; }
	inline auto getNumberOfParameters     ( void ) const { return this->m_np; }
	inline auto getVariablesMaximumOrder  ( void ) const { return this->m_no; }
	inline auto getParametersMaximumOrder ( void ) const { return this->m_po; }

	/**
	 * @brief number of variables and number of parameters
	 */
	inline auto getTotalNumber            ( void ) const {
	    return this->getNumberOfVariables() + this->getNumberOfParameters();
	}

	inline auto getOrderPerParameter     ( void ) const {
	    return this->m_orders;
	}

	void show(std::ostream& strm) const;
    };

    class desc{
    public:
	inline desc(int nv,         ord_t mo                                  )
	    : dm( std::make_unique<desc_mgr>( mad_desc_newv  (nv,     mo    ) ) )
	    {}
	inline desc(int nv, ord_t mo, int np, ord_t po = 0 )
	    : dm( std::make_unique<desc_mgr>( mad_desc_newvp (nv, mo, np, po) ) )
	    {}
	inline desc(int nv, ord_t mo, int np, ord_t po, const ord_t no[/*nv+np?*/] )
	    : dm( std::make_unique<desc_mgr>( mad_desc_newvpo(nv, mo, np, po, no) ) )
	    {}
	inline desc(int nv, ord_t mo, int np, ord_t po, const std::vector<ord_t> no)
	    : dm( nullptr )
	    {
		auto req_elem = nv+np;
		if(int(no.size()) <= req_elem){
		    std::stringstream strm;
		    strm << "nv= "<<nv<<"+ np="<<np <<"="<<req_elem<<"req. elem, got only "<<no.size()<<"elem";
		    std::runtime_error(strm.str());
		}
		this->dm = std::make_unique<desc_mgr> (mad_desc_newvpo(nv, mo, np, po, no.data()));
	    }
	// inline desc(const desc_t* a_desc)                             { this->t_desc = a_desc;                            }
	// inline desc(const desc&& o) : dm(std::move(o.dm))     {}
	inline desc(const desc& o) = delete;

	inline ~desc(void)
	    {}

    private:
	inline const desc_t *  getPtr(void)                        const { return this->dm->getPtr(); }

    public:

	inline int   getNv(ord_t *mo_=0, int *np_=0, ord_t *po_=0) const { return mad_desc_getnv (this->getPtr(), mo_, np_, po_); }
	// inline ord_t getNo(int nn, ord_t *no_)                     const { return mad_desc_getno (this->getPtr(), nn, no_);       }
	inline ord_t maxOrd(int nn=0, ord_t *no=nullptr)           const { return mad_desc_maxord(this->getPtr(), nn, nullptr);                }
	inline ssz_t maxLen(ord_t mo)                              const { return mad_desc_maxlen(this->getPtr(), mo);                }
	// inline ssz_t ordLen(const ord_t mo)                        const { return mad_desc_ordlen(this->getPtr(), mo);            }
	inline ssz_t trunc(const ord_t to)                         const { return mad_desc_gtrunc(this->getPtr(), to);            }

        /*
	 * @brief info
	 */
	desc_info  getInfo() const;

	inline void info(FILE * fp = nullptr)                      const { mad_desc_info(this->getPtr(), fp);                    }
	inline void info_s(std::string* buf)                       const {
	    mad_desc_info_s(this->getPtr(), buf->size(), buf->data());
	}
	std::string info_s(void)                                   const {
	    std::string buf(256, '\0');
	    this->info_s(&buf);
	    return buf;
	}
	/* consider removing methods that don't use containers */
	inline log_t isvalid   (const std::string& s, const ssz_t n=0) const {
	    return mad_desc_isvalids  (this->getPtr(), (n == 0) ? s.size() : n, s.c_str() );
	}

	inline log_t isvalid   (const std::vector<ord_t>& m)     const {
	    return mad_desc_isvalidm  (this->getPtr(), m.size(), m.data());
	}
	inline log_t isvalidsm (const std::vector<idx_t>& m)     const {
	    return mad_desc_isvalidsm (this->getPtr(), m.size(), m.data());
	}
#if 0
	inline log_t isvalid   (const ssz_t n, const str_t  s ) const { return mad_desc_isvalids  (this->getPtr(), n, s );  }
	inline log_t isvalid   (const ssz_t n, const ord_t *m )	const { return mad_desc_isvalidm  (this->getPtr(), n, m );  }
	inline log_t isvalid   (const ssz_t n, const idx_t *m )	const { return mad_desc_isvalidsm (this->getPtr(), n, m );  }
#endif

	inline idx_t idx       (const std::string& s, const ssz_t n=0) const {
	    return mad_desc_idxs      (this->getPtr(), (n == 0) ? s.size() : n, s.c_str() );
	}
	inline log_t idx       (const std::vector<ord_t>& m)    const  {
	    return mad_desc_idxm      (this->getPtr(), m.size(), m.data());
	}
	/**
	 * @note assuming that this is similar to tpsa idxsm (sparse approach)
	 *       thus not overloading
	 */
	inline log_t idxsm     (const std::vector<idx_t> m)     const { return mad_desc_idxsm     (this->getPtr(), m.size(), m.data()); }
#if 0
	inline idx_t idx       (const ssz_t n, const str_t s )	const { return mad_desc_idxs      (this->getPtr(), n, s );  }
	inline idx_t idx       (const ssz_t n, const ord_t *m )	const { return mad_desc_idxm      (this->getPtr(), n, m );  }
	inline idx_t idx       (const ssz_t n, const idx_t *m )	const { return mad_desc_idxsm     (this->getPtr(), n, m );  }
#endif

	inline idx_t nxtbyvar  (std::vector<ord_t> m )          const { return mad_desc_nxtbyvar  (this->getPtr(), m.size(), m.data() );  }
	// inline idx_t nxtbyvar  (ssz_t n,       ord_t *m)     const { return mad_desc_nxtbyvar  (this->getPtr(), n, m );  }
	inline idx_t nxtbyord  (std::vector<ord_t> m )          const { return mad_desc_nxtbyord  (this->getPtr(), m.size(), m.data() );  }
	// inline idx_t nxtbyord  (ssz_t n,       ord_t *m )    const { return mad_desc_nxtbyord  (this->getPtr(), n, m );  }

	inline ord_t mono      (const idx_t i, std::vector<ord_t>* m) const { return mad_desc_mono      (this->getPtr(), i, m->size(), m->data()); }


	inline std::string repr(void)                           const { return this->info_s();	}
	inline void show(std::ostream& o)                       const { o << this->repr(); }

	friend std::ostream& operator<<(std::ostream&, const desc& d);
	friend class  mad::_TpsaWrapper;
	//friend class   TpsaWrapper;
	friend class mad::_CTpsaWrapper;
	friend class mad:: CTpsaWrapper;

private:
	std::unique_ptr<desc_mgr> dm;

};
    inline std::ostream& operator<<(std::ostream& o, const desc& d) { d.show(o); return o; }
    inline std::ostream& operator<<(std::ostream& o, const desc_info& i) { i.show(o); return o; }


} /* namespace gtpsa mad */
namespace gtpsa {
    using mad::desc;
    using mad::init;
}
#endif /* _GTPSA_DESC_H_ */
