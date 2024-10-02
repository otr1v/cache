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
    std::size_t PCA_hit = 0;

    std::unordered_map<int, std::list<std::size_t>> PCA_map;
    std::vector<int> all_elems;
    all_elems = read_input<int>(std::cin, cache_size, input_size, PCA_map);
    
    #ifdef DEBUG
    for (auto it = PCA_map.begin(); it != PCA_map.end(); it++)
    {
        std::cout << "elem is " << it->first << std::endl;
        auto start = it->second.begin();
        for (int i = 0; i < it->second.size(); i++)
        {
            std::cout << "list member" << *start << std::endl;
            start++;
        }
    }
    #endif /* DEBUG*/
    
    caches::PCA_cache<int> my_PCA_cache(cache_size);

    for (std::size_t idx = 0; idx < input_size; idx++)
    {
         caches::PCA_cache<int>::list_elem list(all_elems[idx], all_elems[idx]);
         PCA_hit += my_PCA_cache.lookup_update(list, PCA_map);
    }
    std::cout << "PCA_hits - " << PCA_hit << std::endl;
    return 0;
}