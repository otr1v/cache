#include "head.hpp"
#include "cache.hpp"

template <typename elem_type, typename key_type = int>
extern std::vector<elem_type> read_input(std::istream& is, std::size_t& cache_size, std::size_t& input_size);

int main()
{
    std::size_t cache_size = 0;
    std::size_t input_size = 0;
    std::vector<int> all_elems;
    int cache_mode = 0;
    all_elems = read_input<int>(std::cin, cache_size, input_size);
    int lfu_hit = 0;
    int pca_hit = 0;
    caches::PCA_cache<int> my_cache(cache_size);
    caches::LFU_cache<int> my_LFU_cache(cache_size);
    for (std::size_t idx = 0; idx < input_size; idx++)
    {
         caches::PCA_cache<int>::list_elem list(all_elems[idx], all_elems[idx]);
         pca_hit += my_cache.lookup_update(list, all_elems);
         printf("idx %d\n", all_elems[idx]);
         caches::LFU_cache<int>::list_elem list2(all_elems[idx], all_elems[idx]);
         lfu_hit += my_LFU_cache.lookup_update(list2);
    }
    printf("LFU hits - %d \nPCA hits - %d\n", lfu_hit, pca_hit);

    return 0;
}