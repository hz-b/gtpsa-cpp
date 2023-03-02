#ifndef _GTPSA_PYTHON_NAME_INDEX_H_
#define _GTPSA_PYTHON_NAME_INDEX_H_ 1

/**
 * provide mapping from name to index similar.
 *
 * Compare to e.g. pandas and xarray handle indices of the
 * panda DataFrame or xarray's DataArray
 *
 * @todo is there a library providing this functionality?
 */

#include <map>
#include <memory>
#include <vector>

namespace gtpsa::python {
    typedef std::map<const std::string, const size_t> index_mapping;


    /**
     * @brief default mapping for thor scsi
     *
     * @brief to be moved
     *
     * Python would call it index key
     */
    const index_mapping default_mapping = {
	{ "x"     , 0 },
	{ "px"    , 1 },

	{ "y"     , 2 },
	{ "py"    , 3 },

	{ "delta" , 4 },
	{ "ct"    , 5 },

    };

    /**
     * @brief a key_error similar as in pybind
     *
     * but to avoid a dependency on pybind here. User must translate it to
     * pybind11:keyerror where appropriate
     */
    class key_error : public std::runtime_error {
	  using std::runtime_error::runtime_error;
    };

    /**
     * @brief map keys to index
     *
     * Support to access entries in a vector by name. Here provides mapping
     * from name to index
     */
    class IndexMapping {
	index_mapping m_mapping;
	std::string m_info;

    public:
	/**
	 * @brief
	 */
	IndexMapping(const index_mapping& d, const std::string info="");

	IndexMapping(IndexMapping&&) = default;

	/**
	 * @brief provide list of keys from mapping as required for python __dir__
	 */
	std::vector<std::string> pdir(void) const;

	/**
	 * @brief provide index associated with key
	 *
	 * @throws keyerror if key is not found.
	 */
	size_t index(const std::string key) const;

	/**
	 * @brief create vector of powers assuming not mentiond ones are zero
	 *
	 * @throws keyerror if key is not found.
	 */
	std::vector<size_t> order_vector_from_power(const index_mapping& d) const;
    };

    extern const IndexMapping DefaultIndexMapping;
    /**
     * @brief common object in python
     */
    extern std::shared_ptr<IndexMapping> default_index_mapping_ptr;
} /* namespace gtpsa::python */
#endif /* _GTPSA_PYTHON_NAME_INDEX_H_ */
