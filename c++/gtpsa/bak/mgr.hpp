#ifndef _GTPSA_MGR_H_
#define _GTPSA_MGR_H_ 1
namespace gtpsa {
    /**
     * @ brief manages ptr lifetime
     */
    template<T>
    class desc_mgr{
    public:
	inline desc_mgr(const T *p) : ptr(p) { }
	inline ~desc_mgr(void)               { this->ptr=nullptr; /*needs to be */ }

	inline desc_mgr(const desc_mgr& o)                         = delete;
	inline gtpsa::desc_mgr& operator= (const gtpsa::desc_mgr&) = delete;
	friend class desc;

    private:
	inline const desc_t *  getPtr(void)       const { return this->ptr; }
	const desc_t * ptr;

    };
}

#endif
