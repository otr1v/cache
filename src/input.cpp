#include "head.hpp"
#include "cache.hpp"

/* key_type = elem_type !!!! */
template <typename elem_type, typename key_type = int>
std::vector<elem_type> read_input(std::istream& is, std::size_t& cache_size, std::size_t& input_size)
{
    std::vector<elem_type> all_elems;
    
    is >> cache_size;
    is >> input_size;

    elem_type elem = 0;
    key_type key = 0;
    for (std::size_t idx = 0; idx < input_size; idx++)
    {
        is >> elem;
        key = elem; // FIXME
        //caches::LFU_cache<elem_type>::list_elem list(key, elem);
        all_elems.push_back(elem);
    }

    return all_elems;
}


template
std::vector<int> read_input<int>(std::istream& is, std::size_t& cache_size, std::size_t& input_size);
