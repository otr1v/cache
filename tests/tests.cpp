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
    base_of_tests.one_test[0] = test1;
    base_of_tests.one_test[1] = test2;
    base_of_tests.one_test[2] = test3;
}


void run_tests()
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
        for (std::size_t elem = 0; elem < input_size; elem++)
        {
            caches::PCA_cache<int>::list_elem list1(base.one_test[idx].all_elems[elem], 
                                                    base.one_test[idx].all_elems[elem]);
            PCA_hit += my_PCA_cache.lookup_update(list1, base.one_test[idx].all_elems);
            caches::LFU_cache<int>::list_elem list2(base.one_test[idx].all_elems[elem], 
                                                    base.one_test[idx].all_elems[elem]);
            LFU_hit += my_LFU_cache.lookup_update(list2);
        }
        if (PCA_hit == base.one_test[idx].PCA_hits_answer && 
            LFU_hit == base.one_test[idx].LFU_hits_answer)
        {
            std::cout << "Test " << idx << " successfully passed" << std::endl;
        }
        else
        {
            std::cout << "Test " << idx << " failed" << std::endl;
        }
    }
        
}
int main()
{
    run_tests();
    return 0;
}
