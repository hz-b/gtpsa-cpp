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
#include <utility>
#include <vector>

namespace gtpsa::python {
    /**
     * @brief map a name to an index
     */
    typedef std::map<const std::string, const size_t> index_mapping_t;


    typedef std::pair<const std::string, const size_t> index_lookup_t;
    /**
     * @brief combine mappings to Hamiltonian pair
     */
    typedef std::pair<const index_lookup_t, const index_lookup_t> hamiltonian_pair_t;

    /**
     *
     */
    typedef std::map<const std::string, const hamiltonian_pair_t> hamiltonian_pair_mapping_t;

    /**
     * @brief default mapping for thor scsi
     *
     * @brief to be moved
     *
     * Python would call it index key
     */
    const index_mapping_t default_mapping = {
	{ "x"     , 0 },
	{ "px"    , 1 },

	{ "y"     , 2 },
	{ "py"    , 3 },

	{ "delta" , 4 },
	{ "ct"    , 5 },

    };

    const hamiltonian_pair_mapping_t default_hamiltonian_mapping = {
	{
	    "horizontal", {
		{ "x"     , 0 },
		{ "px"    , 1 }

	    }
	},
	{
	    "vertical", {
		{ "y"     , 2 },
		{ "py"    , 3 }
	    }
	},
	{
	    "longitudinal", {
		    { "ct"    , 5 },
		    { "delta" , 4 },
	    }
	}
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
    class IndexMappingBase {
    public:
        inline IndexMappingBase(void) {};
	    virtual ~IndexMappingBase(void) {};

	IndexMappingBase(IndexMappingBase&&) = default;

	/**
	 * @brief provide list of keys from mapping as required for python __dir__
	 */
	virtual std::vector<std::string> pdir(void) const = 0;

	/**
	 * @brief provide index associated with key
	 *
	 * @throws keyerror if key is not found.
	 */
	virtual size_t index(const std::string key) const = 0;

    };

    class IndexMapping : public IndexMappingBase {
	index_mapping_t m_mapping;
	std::string m_info;

    public:
	/**
	 * @brief
	 */
	IndexMapping(const index_mapping_t& d, const std::string info="");

	std::vector<std::string> pdir(void) const override final;
	size_t index(const std::string key) const override final;
	std::vector<size_t> order_vector_from_power(const index_mapping_t& d) const;

    };

    class HamiltonianIndexMapping : public IndexMappingBase {
	hamiltonian_pair_mapping_t m_hamiltonian_mapping;
	index_mapping_t m_mapping;
	std::string m_info;

    public:
	/**
	 * @brief
	 */
	HamiltonianIndexMapping(const hamiltonian_pair_mapping_t& d, const std::string info="");

	std::vector<std::string> pdir(void) const override final;
	size_t index(const std::string key) const override final;


    };


    extern const IndexMapping DefaultIndexMapping;
    /**
     * @brief common object in python
     */
    extern std::shared_ptr<IndexMapping> default_index_mapping_ptr;
} /* namespace gtpsa::python */
#endif /* _GTPSA_PYTHON_NAME_INDEX_H_ */
