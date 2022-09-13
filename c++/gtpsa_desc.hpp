#ifndef _GTPSA_DESC_H_
#define _GTPSA_DESC_H_ 1
extern "C" {
    typedef bool _Bool;
#include <mad_desc.h>
};

#include <vector>
#include <string>
#include <ostream>
#include <sstream>

namespace gtpsa {
class desc{
public:
    inline desc(int nv, ord_t no_)                                { this->t_desc = mad_desc_newv(nv, no_);            }
    inline desc(int nv, ord_t np, ord_t no, ord_t po_)            { this->t_desc = mad_desc_newvp(nv, np, no, po_);   }
    inline desc(int nv, ord_t np, ord_t no[/*nv+np?*/], ord_t po_){ this->t_desc = mad_desc_newvpo(nv, np, no, po_);  }
    inline desc(int nv, ord_t np, std::vector<ord_t> no, ord_t po_){
	auto req_elem = nv+np;
	if(no.size() <= req_elem){
	    std::stringstream strm;
	    strm << "nv= "<<nv<<"+ np="<<np <<"="<<req_elem<<"req. elem, got only "<<no.size()<<"elem";
	    std::runtime_error(strm.str());
	}
	this->t_desc = mad_desc_newvpo(nv, np, no.data(), po_);
    }
    inline desc(const desc_t* a_desc)                             { this->t_desc = a_desc;                            }
    inline desc(const desc&& o) : t_desc(std::move(o.t_desc))     {}
    inline desc(const desc& o) = delete;

    inline ~desc(void)                                            { mad_desc_del(this->t_desc); }

    inline const desc_t *  getPtr(void)                     const { return this->t_desc; }

    inline int getNv(ord_t *mo_, int *np_, ord_t *po_)      const { return mad_desc_getnv(this->t_desc, mo_, np_, po_); }
    inline ord_t getNo(int nn, ord_t *no_)                  const { return mad_desc_getno(this->t_desc, nn, no_);       }
    inline ord_t maxOrd(void)                               const { return mad_desc_maxord(this->t_desc);               }
    inline ssz_t maxLen(void)                               const { return mad_desc_maxlen(this->t_desc);               }
    inline ssz_t ordLen(const ord_t mo)                     const { return mad_desc_ordlen(this->t_desc, mo);           }
    inline ssz_t trunc(const ord_t to)                      const { return mad_desc_gtrunc(this->t_desc, to);           }

    inline void info(FILE * fp = nullptr)                   const { mad_desc_info(this->t_desc, fp);                    }
    /* consider removing methods that don't use containers */
    inline log_t isvalid   (const std::string s, ssz_t n=0) const { return mad_desc_isvalids  (this->t_desc, n, s.c_str() );      }
    inline log_t isvalid   (const std::vector<ord_t> m)     const { return mad_desc_isvalidm  (this->t_desc, m.size(), m.data()); }
    inline log_t isvalid   (const std::vector<idx_t> m)     const { return mad_desc_isvalidsm (this->t_desc, m.size(), m.data()); }
#if 0
    inline log_t isvalid   (ssz_t n,       str_t s    )     const { return mad_desc_isvalids  (this->t_desc, n, s );  }
    inline log_t isvalid   (ssz_t n, const ord_t *m )	    const { return mad_desc_isvalidm  (this->t_desc, n, m );  }
    inline log_t isvalid   (ssz_t n, const idx_t *m )	    const { return mad_desc_isvalidsm (this->t_desc, n, m );  }
#endif

    inline idx_t idx       (const std::string s, ssz_t n=0) const { return mad_desc_idxs      (this->t_desc, n, s.c_str() );}
    inline log_t idx       (const std::vector<ord_t> m)     const { return mad_desc_idxm      (this->t_desc, m.size(), m.data()); }
    inline log_t idx       (const std::vector<idx_t> m)     const { return mad_desc_idxsm     (this->t_desc, m.size(), m.data()); }
#if 0
    inline idx_t idx       (ssz_t n,       str_t s )	    const { return mad_desc_idxs      (this->t_desc, n, s );  }
    inline idx_t idx       (ssz_t n, const ord_t *m )	    const { return mad_desc_idxm      (this->t_desc, n, m );  }
    inline idx_t idx       (ssz_t n, const idx_t *m )	    const { return mad_desc_idxsm     (this->t_desc, n, m );  }
#endif

    inline idx_t nxtbyvar  (std::vector<ord_t> m )          const { return mad_desc_nxtbyvar  (this->t_desc, m.size(), m.data() );  }
    // inline idx_t nxtbyvar  (ssz_t n,       ord_t *m)     const { return mad_desc_nxtbyvar  (this->t_desc, n, m );  }
    inline idx_t nxtbyord  (std::vector<ord_t> m )          const { return mad_desc_nxtbyord  (this->t_desc, m.size(), m.data() );  }
    // inline idx_t nxtbyord  (ssz_t n,       ord_t *m )    const { return mad_desc_nxtbyord  (this->t_desc, n, m );  }

    inline ord_t mono      (std::vector<ord_t> m, idx_t i)  const { return mad_desc_mono      (this->t_desc, m.size(), m.data(), i); }


    inline std::string repr(void)                           const {
	const int buf_len = 256;
	char buf[buf_len];
	mad_desc_info_s(this->t_desc, buf_len, buf);
	return std::string(buf);
    }
    inline void show(std::ostream& o) const { o << this->repr(); }
    friend std::ostream& operator<<(std::ostream&, const desc& d);

private:
    const desc_t * t_desc;

};
    inline std::ostream& operator<<(std::ostream& o, const desc& d) { d.show(o); return o; }


}; /* namespace gtpsa */
#endif /* _GTPSA_DESC_H_ */
