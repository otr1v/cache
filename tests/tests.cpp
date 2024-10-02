#include "cache.hpp"
#include "head.hpp"

struct test
{
    std::size_t cache_size;
    std::size_t input_size;
    std::vector<int> all_elems;
    std::size_t PCA_hits_answer;
    std::size_t LFU_hits_answer;
};

struct tests
{
    test one_test[NumberOfTests];
};


void test_base(tests& base_of_tests) 
{
    test test1 = {2, 7,  {1, 2, 3, 1, 2, 3, 1},          3, 0};
    test test2 = {3, 10, {1, 1, 2, 2, 3, 3, 4, 1, 2, 3}, 6, 3};
    test test3 = {1, 6,  {1, 1, 2, 1, 2, 1},             3, 1};
    test test4 = {5, 30, {4, 2, 1, 2, 5, 4, 1, 6, 3, 2, 
                          10, 2, 9, 2, 7, 5, 10, 2, 6, 1, 
                          0, 1, 2, 4, 10, 5, 9, 10, 2, 5}, 18, 11};
    base_of_tests.one_test[0] = test1;
    base_of_tests.one_test[1] = test2;
    base_of_tests.one_test[2] = test3;
    base_of_tests.one_test[3] = test4;
}


void run_unit_tests()
{
    tests base;
    test_base(base);
    
    for (std::size_t idx = 0; idx < NumberOfTests; idx++)
    {
        std::size_t PCA_hit = 0;
        std::size_t LFU_hit = 0;

        caches::PCA_cache<int> my_PCA_cache(base.one_test[idx].cache_size);
        caches::LFU_cache<int> my_LFU_cache(base.one_test[idx].cache_size);

        std::size_t input_size = base.one_test[idx].input_size;
        std::unordered_map<int, std::list<std::size_t>> PCA_map;

        for (std::size_t elem = 0; elem < input_size; elem++)
        {
            caches::LFU_cache<int>::list_elem list2(base.one_test[idx].all_elems[elem], 
                                                    base.one_test[idx].all_elems[elem]);
            LFU_hit += my_LFU_cache.lookup_update(list2);
            
            auto hit = PCA_map.find(base.one_test[idx].all_elems[elem]);
            if (hit == PCA_map.end()) //not found key in hash
            {
                std::list<std::size_t> new_list;
                new_list.emplace_back(elem);
                PCA_map.emplace(base.one_test[idx].all_elems[elem], new_list);
            }
            else
            {
                hit->second.emplace_back(elem);
            }
        }
        for (std::size_t elem = 0; elem < input_size; elem++)
        {
            caches::PCA_cache<int>::list_elem list1(base.one_test[idx].all_elems[elem], 
                                                    base.one_test[idx].all_elems[elem]);
            PCA_hit += my_PCA_cache.lookup_update(list1, PCA_map);
        }
        if (PCA_hit == base.one_test[idx].PCA_hits_answer && 
            LFU_hit == base.one_test[idx].LFU_hits_answer)
        {
            std::cout << "Unit Test " << idx << " successfully passed" << std::endl;
        }
        else
        {
            std::cout << "Unit Test " << idx << " failed" << std::endl;
        }
    }
}
template <typename elem_type = int>
void run_e2e_tests()
{
    std::vector<elem_type> all_elems;
    std::unordered_map<int, std::list<std::size_t>> PCA_map;

    std::size_t cache_size = 0, input_size = 0, LFU_hit = 0, PCA_hit = 0;
    elem_type elem = 0;

    std::ifstream is("tests/tests.txt");
    is >> cache_size;
    is >> input_size;

    caches::PCA_cache<int> my_PCA_cache(cache_size);
    caches::LFU_cache<int> my_LFU_cache(cache_size);

    for (std::size_t idx = 0; idx < input_size; idx++)
    {
        is >> elem;
        all_elems.push_back(elem);
        caches::LFU_cache<int>::list_elem list2(elem, elem);
        LFU_hit += my_LFU_cache.lookup_update(list2);

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
    }
    for (std::size_t idx = 0; idx < input_size; idx++)
    {
        caches::PCA_cache<int>::list_elem list(all_elems[idx], all_elems[idx]);
        PCA_hit += my_PCA_cache.lookup_update(list, PCA_map);
    }
    std::cout << "e2e results:" << std::endl << "LFU hits " << LFU_hit << std::endl;
    std::cout << "PCA hits " << PCA_hit << std::endl;
    
}

int main()
{
    run_unit_tests();
    run_e2e_tests();
    return 0;
}
