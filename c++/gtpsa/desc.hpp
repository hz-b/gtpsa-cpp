#ifndef _GTPSA_DESC_H_
#define _GTPSA_DESC_H_ 1
#include <gtpsa/forward_decl.hpp>
extern "C" {
    typedef bool _Bool;
#include <mad_desc.h>
}


#include <vector>
#include <string>
#include <ostream>
#include <sstream>
#include <memory>

namespace gtpsa {
    /*
     *@brief how to initalise new object if an other is presented
     */
    enum init {
	default_ = ord_t(-1),
	same     = ord_t(-2)
    };

    /**
     * @brief manages ptr lifetime
     */
    class desc_mgr{
    public:
	inline const desc_t *  getPtr(void) const { return this->ptr; }
	friend class desc;
	inline desc_mgr(const desc_t *p) : ptr(p) { }
	inline ~desc_mgr(void)                    { mad_desc_del(this->ptr); this->ptr=nullptr; }

    private:
	// inline desc_mgr(const desc& o)           = delete;
	desc_mgr(const desc_mgr& o)           = delete;
	desc_mgr& operator= (const desc_mgr&) = delete;

	const desc_t * ptr;
    };


    class desc{
    public:
	inline desc(int nv,           ord_t no = 0)                         : dm( std::make_unique<desc_mgr>( mad_desc_newv  (nv,     no    ) ) ) { }
	inline desc(int nv, ord_t np, ord_t no,              ord_t po = 0 ) : dm( std::make_unique<desc_mgr>( mad_desc_newvp (nv, np, no, po) ) ) { }
	inline desc(int nv, ord_t np, ord_t no[/*nv+np?*/],  ord_t po = 0 ) : dm( std::make_unique<desc_mgr>( mad_desc_newvpo(nv, np, no, po) ) ) { }
	inline desc(int nv, ord_t np, std::vector<ord_t> no, ord_t po = 0 ) : dm( nullptr ) {
	auto req_elem = nv+np;
	if(int(no.size()) <= req_elem){
	    std::stringstream strm;
	    strm << "nv= "<<nv<<"+ np="<<np <<"="<<req_elem<<"req. elem, got only "<<no.size()<<"elem";
	    std::runtime_error(strm.str());
	}
	this->dm = std::make_unique<desc_mgr> (mad_desc_newvpo(nv, np, no.data(), po));
    }
	// inline desc(const desc_t* a_desc)                             { this->t_desc = a_desc;                            }
	// inline desc(const desc&& o) : dm(std::move(o.dm))     {}
	inline desc(const desc& o) = delete;

    inline ~desc(void)                                                    {  }

    private:
	inline const desc_t *  getPtr(void)                     const { return this->dm->getPtr(); }

    public:

	inline int   getNv(ord_t *mo_, int *np_, ord_t *po_)    const { return mad_desc_getnv (this->getPtr(), mo_, np_, po_); }
	inline ord_t getNo(int nn, ord_t *no_)                  const { return mad_desc_getno (this->getPtr(), nn, no_);       }
	inline ord_t maxOrd(void)                               const { return mad_desc_maxord(this->getPtr());                }
	inline ssz_t maxLen(void)                               const { return mad_desc_maxlen(this->getPtr());                }
	inline ssz_t ordLen(const ord_t mo)                     const { return mad_desc_ordlen(this->getPtr(), mo);            }
	inline ssz_t trunc(const ord_t to)                      const { return mad_desc_gtrunc(this->getPtr(), to);            }

	inline void info(FILE * fp = nullptr)                   const { mad_desc_info(this->getPtr(), fp);                    }
	/* consider removing methods that don't use containers */
	inline log_t isvalid   (const std::string s, ssz_t n=0) const { return mad_desc_isvalids  (this->getPtr(), (n == 0) ? s.size() : n, s.c_str() );      }
	inline log_t isvalid   (const std::vector<ord_t> m)     const { return mad_desc_isvalidm  (this->getPtr(), m.size(), m.data()); }
	inline log_t isvalidsm (const std::vector<idx_t> m)     const { return mad_desc_isvalidsm (this->getPtr(), m.size(), m.data()); }
#if 0
	inline log_t isvalid   (ssz_t n,       str_t s    )     const { return mad_desc_isvalids  (this->getPtr(), n, s );  }
	inline log_t isvalid   (ssz_t n, const ord_t *m )	const { return mad_desc_isvalidm  (this->getPtr(), n, m );  }
	inline log_t isvalid   (ssz_t n, const idx_t *m )	const { return mad_desc_isvalidsm (this->getPtr(), n, m );  }
#endif

	inline idx_t idx       (const std::string s, ssz_t n=0) const { return mad_desc_idxs      (this->getPtr(), (n == 0) ? s.size() : n, s.c_str() );}
	inline log_t idx       (const std::vector<ord_t> m)     const { return mad_desc_idxm      (this->getPtr(), m.size(), m.data()); }
	/**
	 * @note assuming that this is similar to tpsa idxsm (sparse approach)
	 *       thus not overloading
	 */
	inline log_t idxsm     (const std::vector<idx_t> m)     const { return mad_desc_idxsm     (this->getPtr(), m.size(), m.data()); }
#if 0
	inline idx_t idx       (ssz_t n,       str_t s )	const { return mad_desc_idxs      (this->getPtr(), n, s );  }
	inline idx_t idx       (ssz_t n, const ord_t *m )	const { return mad_desc_idxm      (this->getPtr(), n, m );  }
	inline idx_t idx       (ssz_t n, const idx_t *m )	const { return mad_desc_idxsm     (this->getPtr(), n, m );  }
#endif

	inline idx_t nxtbyvar  (std::vector<ord_t> m )          const { return mad_desc_nxtbyvar  (this->getPtr(), m.size(), m.data() );  }
	// inline idx_t nxtbyvar  (ssz_t n,       ord_t *m)     const { return mad_desc_nxtbyvar  (this->getPtr(), n, m );  }
	inline idx_t nxtbyord  (std::vector<ord_t> m )          const { return mad_desc_nxtbyord  (this->getPtr(), m.size(), m.data() );  }
	// inline idx_t nxtbyord  (ssz_t n,       ord_t *m )    const { return mad_desc_nxtbyord  (this->getPtr(), n, m );  }

	inline ord_t mono      (std::vector<ord_t> m, idx_t i)  const { return mad_desc_mono      (this->getPtr(), m.size(), m.data(), i); }


	inline std::string repr(void)                           const {
#if 1
	    throw std::runtime_error("Not implemented");
#else
	    const int buf_len = 256;
	    char buf[buf_len];
	    mad_desc_info_s(this->getPtr(), buf_len, buf);
	    return std::string(buf);
#endif
	}
	inline void show(std::ostream& o) const { o << this->repr(); }

	friend std::ostream& operator<<(std::ostream&, const desc& d);
	friend class tpsa;

private:
	std::unique_ptr<desc_mgr> dm;

};
    inline std::ostream& operator<<(std::ostream& o, const desc& d) { d.show(o); return o; }


} /* namespace gtpsa */
#endif /* _GTPSA_DESC_H_ */
