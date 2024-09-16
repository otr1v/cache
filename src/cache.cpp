#include "cache.hpp"
#include "head.hpp"

template <typename elem_type, typename key_type = int>
extern std::vector<elem_type> read_input(std::istream& is, bool cache_type, std::size_t& cache_size, std::size_t& input_size);

// using namespace caches;
int main()
{
    std::size_t cache_size = 0;
    std::size_t input_size = 0;
    std::vector<int> all_elems;
    int cache_mode = 0;
    std::cin >> cache_mode;
    all_elems = read_input<int>(std::cin, PCA_CACHE, cache_size, input_size);
    caches::PCA_cache<int> my_cache(cache_size);
     for (std::size_t idx = 0; idx < input_size; idx++)
     {
         caches::PCA_cache<int>::list_elem list(all_elems[idx], all_elems[idx]);
         my_cache.lookup_update(list, all_elems);
     }
    // for (std::size_t idx = 0; idx < input_size; idx++)
    // {
    //     caches::LFU_cache<int>::list_elem list(all_elems[idx], all_elems[idx]);
    //     my_cache.lookup_update(list);
    // }
    //std::cin >> cache_size;
    // caches::LFU_cache<int> my_cache(cache_size);
    // caches::LFU_cache<int>::list_elem list(52, 52);
    // caches::LFU_cache<int>::list_elem list1(53, 53);
    // caches::LFU_cache<int>::list_elem list2(54, 54);
    // caches::LFU_cache<int>::list_elem list3(55, 55);
    // my_cache.lookup_update(list);
    // my_cache.lookup_update(list1);
    // my_cache.lookup_update(list2);
    // my_cache.lookup_update(list3);
    // my_cache.lookup_update(list3);
    // my_cache.erase_elem();
    return 0;
}