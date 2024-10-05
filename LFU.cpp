#include "head.hpp"
#include "cache.hpp"

template <typename elem_type, typename key_type = int>
extern std::vector<elem_type> read_input(
    std::istream& is, 
    std::size_t& cache_size, 
    std::size_t& input_size,
    std::unordered_map<elem_type, std::list<std::size_t>>& PCA_map);

int main()
{
    std::size_t cache_size = 0;
    std::size_t input_size = 0;
    std::size_t LFU_hit = 0;

    std::unordered_map<int, std::list<std::size_t>> PCA_map;
    std::vector<int> all_elems;
    all_elems = read_input<int>(std::cin, cache_size, input_size, PCA_map);
    
    caches::LFU_cache<int> my_LFU_cache(cache_size);
    
    for (std::size_t idx = 0; idx < input_size; idx++)
    {
         caches::LFU_cache<int>::list_elem list(all_elems[idx], all_elems[idx]);
         LFU_hit += my_LFU_cache.lookup_update(list);
    }
    std::cout << LFU_hit << std::endl;
    return 0;
}