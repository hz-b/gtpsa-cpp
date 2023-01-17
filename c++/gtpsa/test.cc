#include <stdexcept>
typedef bool _Bool;
extern "C" {
#include <mad_tpsa.h>
#include <mad_ctpsa.h>
}

namespace gtpsa {
    // forward declaration
    template <typename P>
    class Wrapper;

    /**
     * @brief manages lifetime of underlying C object
     *
     * Expected to be handled by std::unique_ptr
     */
    template <typename P>
    class LifeTimeManager {
    public:
	inline LifeTimeManager(P *p)
	    : ptr(p)
	    { }
	/**
	 * @brief: how the warn on the memory leak ?
	 */
	// inline ~LifeTimeManager(void) {  this->ptr = nullptr; }

    private:
	LifeTimeManager             (const LifeTimeManager& o) = delete;
	LifeTimeManager & operator= (const LifeTimeManager& o) = delete;

	inline const P*    getPtr(void) const { return this->ptr; }
	inline       P*    getPtr(void)       { return this->ptr; }

	P* ptr=nullptr;

	//friend class GTPSA_CLASS(Wrapper);
    }; /* class GTPSA_CLASS(LifeTimeManager) */

    template<>
    class LifeTimeManager<tpsa_t> {
	tpsa_t* ptr=nullptr;
    public:
	inline ~LifeTimeManager(void) {  mad_tpsa_del(this->ptr); this->ptr = nullptr; }
    };

    template<>
    class LifeTimeManager<ctpsa_t> {
	ctpsa_t* ptr=nullptr;
    public:
	inline ~LifeTimeManager(void) {  mad_ctpsa_del(this->ptr); this->ptr = nullptr; }
    };
}





int main(int argc, char *argv[])
{
    gtpsa::LifeTimeManager<tpsa_t> mt(nullptr);
    gtpsa::LifeTimeManager<ctpsa_t> cmt(nullptr);
}
