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
    std::vector<int> all_elems;
    std::unordered_map<int, std::list<std::size_t>> PCA_map;

    all_elems = read_input<int>(std::cin, cache_size, input_size, PCA_map);

    int LFU_hit = 0;
    int PCA_hit = 0;
    caches::PCA_cache<int> my_PCA_cache(cache_size);
    caches::LFU_cache<int> my_LFU_cache(cache_size);
    
    for (std::size_t idx = 0; idx < input_size; idx++)
    {
         caches::PCA_cache<int>::list_elem list(all_elems[idx], all_elems[idx]);
         PCA_hit += my_PCA_cache.lookup_update(list, PCA_map);
         caches::LFU_cache<int>::list_elem list2(all_elems[idx], all_elems[idx]);
         LFU_hit += my_LFU_cache.lookup_update(list2);
    }
    std::cout << "LFU_hits - " << LFU_hit << std::endl;
    std::cout << "PCA_hits - " << PCA_hit << std::endl;
    return 0;
}