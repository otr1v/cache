#include "head.hpp"
#include "cache.hpp"

/* key_type = elem_type !!!! */
template <typename elem_type, typename key_type = int>
std::vector<elem_type> read_input(
    std::istream& is, 
    std::size_t& cache_size, 
    std::size_t& input_size, 
    std::unordered_map<elem_type, std::list<std::size_t>>& PCA_map)
{
    std::vector<elem_type> all_elems;
    is >> cache_size;
    is >> input_size;
    elem_type elem = 0;
    for (std::size_t idx = 0; idx < input_size; idx++)
    {
        is >> elem;
        auto hit = PCA_map.find(elem);
        if (hit == PCA_map.end()) //not found key in hash
        {
            std::list<std::size_t> new_list;
            new_list.emplace_back(idx);
            PCA_map.emplace(elem, new_list);
        }
        else
        {
            hit->second.emplace_back(idx);
        }
        all_elems.push_back(elem);
    }

    return all_elems;
}


template
std::vector<int> read_input<int>(
    std::istream& is, 
    std::size_t& cache_size, 
    std::size_t& input_size, 
    std::unordered_map<int, std::list<std::size_t>>& PCA_map);
