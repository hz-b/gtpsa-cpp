#include <algorithm>
#include <cassert>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#define _GTPSA_PYTHON_NAME_INDEX_INTERN_
#include <gtpsa/python/name_index.h>

namespace gpy = gtpsa::python;

/**
 * @brief check that all keys and indices are unique.
 *
 * @raises std::runtime_error if conditions not matched
 * Does not check if all keys can be used as python attributes
 *
 */
#if 0
static void check_unique_keys_index(const gpy::index_mapping_t& d, const std::string& info);
#endif

/**
 * @brief check that default mappings are matched
 *
 * E.g. thor scsi defines them to be at particular indices.
 *
 * @raises std::runtime_error if conditions not matched
 */
#if 0
static void check_default_keys_index(const gpy::index_mapping_t& d,
					  const gpy::index_mapping_t& default_mapping,
					  const std::string& info = "");
#endif
template<typename T>
static void check_unique_keys_index(const std::map<const std::string, const T>& d, const std::string& info)
{
    std::set<std::string> keys;
    std::set<T> indices;

    const std::string
	prefix = "gtpsa::python checking uniquess",
	suffix = "(both numbers must be the same).";

    for(const auto& [key, index]: d) {
	keys.insert(key);
	indices.insert(index);
    }

    if(keys.size() != d.size()) {
	std::stringstream strm;
	strm << prefix << " " << info << ": found " << keys.size() << " unique keys,"
	     << " but have "<< d.size() << " entries in the index map "
	     << suffix;
	throw std::runtime_error(strm.str());
    }

    if(indices.size() != d.size()) {
	std::stringstream strm;
	strm << prefix << " " << info << ": found " << indices.size() << " unique indices,"
	     << " but have "<< d.size() << " entries in the index map "
	     << suffix;
	throw std::runtime_error(strm.str());
    }

    /* no exception thrown ... all good */
}

static void check_default_keys_index(const gpy::index_mapping_t& d, const gpy::index_mapping_t& default_mapping,
				     const std::string& info)
{

    const std::string prefix = "gtpsa::python checking default indices";

    for(const auto& [key, index]: default_mapping) {
	std::string tkey = key;
	const auto check = d.find(key);

	if(check == d.end()) {
	    std::stringstream strm;
	    strm << prefix << " " << info << " : found no entry for default " << key;
	    throw std::runtime_error(strm.str());
	}

	assert(check->first == key);

	if(check->second != index) {
	    std::stringstream strm;
	    strm << prefix << " " << info << " : found that for key " << key
		 << " an index of " << check->second << " was found, but it should be "
		 << " of value " << index;
	    throw std::runtime_error(strm.str());
	}
    }

    /* no exception raised, all good */

}


template<typename T>
static std::vector<std::string> mapping_keys_for_dir(const std::map<const std::string, T>& im)
{
	std::vector<std::string> names;
	names.reserve(im.size());

	std::transform(im.begin(), im.end(), std::back_inserter(names),
		       [](const auto entry){return entry.first;});

	return names;
}

template<typename T>
static auto mapping_index(const std::map<const std::string, T>& im, const std::string& key)
{
	const auto& entry = im.find(key);
	if (entry == im.end()){
		std::stringstream strm;
		strm << "gtpsa::ss_vect can't find key '" << key << "'";
		throw gpy::key_error(strm.str());
	}
	return entry->second;
}

/**
 * @brief extract name index mapping from Hamiltonian canonical coordinates
 */
static const gpy::index_mapping_t from_hamiltion_index_mapping(const gpy::hamiltonian_pair_mapping_t& d)
{
	gpy::index_mapping_t res;
	// res.reserve(d.size() * 2);

	for(const auto& [key, val] : d){
		/* extract canonical variables: space and impuls ?  */
		const auto& base = std::get<0>(val);
		res.insert(base);
		const auto& deriv = std::get<1>(val);
		res.insert(deriv);
	}

	return res;
}

gpy::IndexMapping::IndexMapping(const index_mapping_t& d, const std::string info, const bool check_default_keys)
	: IndexMappingBase()
	, m_mapping(d)
	, m_info(info)
{

	check_unique_keys_index(this->m_mapping, this->m_info);
    if(check_default_keys){
        check_default_keys_index(this->m_mapping, default_mapping, this->m_info);
    }
}

std::vector<std::string> gpy::IndexMapping::pdir(void) const
{
	return mapping_keys_for_dir(this->m_mapping);
}

size_t  gpy::IndexMapping::index(const std::string key) const
{
	return mapping_index(this->m_mapping, key);
}

std::vector<size_t> gpy::IndexMapping::order_vector_from_power(const index_mapping_t& powers) const
{
	std::vector<size_t> p(this->m_mapping.size());
	std::transform(p.begin(), p.end(), p.begin(), [](const size_t val){return 0;});

	for(const auto& [key, t_power]: powers) {
		auto index = mapping_index(this->m_mapping, key);
		try {
		    p.at(index) = t_power;
		}
		catch(const std::out_of_range& ie) {
		    std::stringstream strm;
		    strm<< "gtpsa:: name to index, index = " << index
			<< " max size " << p.size();
		    strm<< ": " << ie.what();
		    throw std::out_of_range(strm.str());
		}
	}

	return p;
}


gpy::HamiltonianIndexMapping::HamiltonianIndexMapping(const hamiltonian_pair_mapping_t& d, const std::string info)
	: IndexMappingBase()
	, m_hamiltonian_mapping(d)
	, m_mapping(from_hamiltion_index_mapping(d))
	, m_info(info)
{

	check_unique_keys_index(this->m_mapping, this->m_info);
	check_default_keys_index(this->m_mapping, default_mapping, this->m_info);

	// default mapping for hamiltonian
	check_unique_keys_index(this->m_hamiltonian_mapping, this->m_info);
}


std::vector<std::string> gpy::HamiltonianIndexMapping::pdir(void) const
{
	return mapping_keys_for_dir(this->m_hamiltonian_mapping);
}

size_t  gpy::HamiltonianIndexMapping::index(const std::string key) const
{
    throw std::runtime_error("Hamiltoninan index mapping needs to be implemented");
	// return mapping_index(this->m_hamiltonian_mapping, key);
}


namespace gtpsa::python {
	const IndexMapping DefaultIndexMapping(gpy::default_mapping, "gtpsa::ss_vect");
	std::shared_ptr<IndexMapping> default_index_mapping_ptr = std::make_shared<IndexMapping>(gpy::default_mapping, "gtpsa::ss_vect");
} // namespace gtpa::python
