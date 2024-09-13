#include "cache.hpp"

// using namespace caches;
int main()
{
    size_t cache_size = 0;
    std::cin >> cache_size;
    caches::LFU_cache<int> my_cache(cache_size);
    caches::LFU_cache<int>::list_elem list(52, 52);
    caches::LFU_cache<int>::list_elem list1(53, 53);
    caches::LFU_cache<int>::list_elem list2(54, 54);
    caches::LFU_cache<int>::list_elem list3(55, 55);
    my_cache.lookup_update(list);
    my_cache.lookup_update(list1);
    my_cache.lookup_update(list2);
    my_cache.lookup_update(list3);
    my_cache.lookup_update(list3);
    my_cache.erase_elem();
    return 0;
}