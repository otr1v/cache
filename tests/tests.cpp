#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "cache.hpp"
#include "head.hpp"


std::pair<std::size_t, std::size_t> get_answer_unit_test(
    std::size_t cache_size, 
    std::size_t input_size, 
    std::vector<int> all_elems)
{
    std::unordered_map<int, std::list<std::size_t>> PCA_map;
    caches::PCA_cache<int> my_PCA_cache(cache_size);
    caches::LFU_cache<int> my_LFU_cache(cache_size);

    std::size_t LFU_hit = 0;
    std::size_t PCA_hit = 0;

    for (std::size_t idx = 0; idx < input_size; idx++)
    {
        caches::LFU_cache<int>::list_elem list2(all_elems[idx], all_elems[idx]);
        LFU_hit += my_LFU_cache.lookup_update(list2);

        auto hit = PCA_map.find(all_elems[idx]);
        if (hit == PCA_map.end()) //not found key in hash
        {
            std::list<std::size_t> new_list;
            new_list.emplace_back(idx);
            PCA_map.emplace(all_elems[idx], new_list);
        }
        else
        {
            hit->second.emplace_back(idx);
        }
    }
    for (std::size_t idx = 0; idx < input_size; idx++)
    {
        caches::PCA_cache<int>::list_elem list1(all_elems[idx], all_elems[idx]);
        PCA_hit += my_PCA_cache.lookup_update(list1, PCA_map);
    }
    return {PCA_hit, LFU_hit};
}

TEST(LFU_and_PCA, unit_test1)
{
    auto [PCA_hit, LFU_hit] = get_answer_unit_test(2, 7, {1, 2, 3, 1, 2, 3, 1});
    EXPECT_EQ(PCA_hit, 3);
    EXPECT_EQ(LFU_hit, 0);
}

TEST(LFU_and_PCA, unit_test2)
{
    auto [PCA_hit, LFU_hit] = get_answer_unit_test(3, 10, {1, 1, 2, 2, 3, 3, 4, 1, 2, 3});
    EXPECT_EQ(PCA_hit, 6);
    EXPECT_EQ(LFU_hit, 3);
}
TEST(LFU_and_PCA, unit_test3)
{
    auto [PCA_hit, LFU_hit] = get_answer_unit_test(1, 6, {1, 1, 2, 1, 2, 1});
    EXPECT_EQ(PCA_hit, 3);
    EXPECT_EQ(LFU_hit, 1);
}
TEST(LFU_and_PCA, unit_test4)
{
    auto [PCA_hit, LFU_hit] = get_answer_unit_test(5, 30, {4, 2, 1, 2, 5, 4, 1, 6, 3, 2,
                                                10, 2, 9, 2, 7, 5, 10, 2, 6, 1,
                                                0, 1, 2, 4, 10, 5, 9, 10, 2, 5});
    EXPECT_EQ(PCA_hit, 18);
    EXPECT_EQ(LFU_hit, 11);
}



template <typename elem_type = int>
void run_e2e_tests()
{
    std::vector<elem_type> all_elems;
    std::unordered_map<int, std::list<std::size_t>> PCA_map;

    std::size_t cache_size = 0, input_size = 0, LFU_hit = 0, PCA_hit = 0;
    elem_type elem = 0;

    std::ifstream is("tests.txt");
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

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    run_e2e_tests<int>();
    return result;
}