#pragma once

#include <iostream>
#include <list>
#include <unordered_map>
#include <set>

namespace caches
{
size_t number_of_elems = 0;

template <typename elem_type, typename key_type = int>  
class LFU_cache
{
public:
    size_t size;
    
    struct list_elem
    {
        key_type key;
        elem_type value;
        list_elem(key_type k, elem_type elem): key(k), value(elem) {}
    };
    std::list<list_elem> my_list;
    using list_it = typename std::list<list_elem>::iterator;
    struct cache_elem
    {
        list_it list_iter;
        size_t counter;
        size_t age;
        cache_elem(list_it it): list_iter(it), counter(0) 
        {
            age = number_of_elems;
            number_of_elems++;
        }
        //list_it lowest_counter_and_oldest_iter;
    };
    LFU_cache(size_t sz): size(sz) {}
    std::unordered_map<key_type, cache_elem> my_hash;

    struct set_elem_cmp 
    {
        bool operator() (const cache_elem& lhs, const cache_elem& rhs) const 
        { 
            if (lhs.counter != rhs.counter)
            {
                return lhs.counter < rhs.counter;
            }
            else
            {
                return lhs.age < rhs.age;
            }
        }
    };
    std::set<cache_elem, set_elem_cmp> my_cache;

    bool cache_is_full() const
    {
        return (my_cache.size() == size);
    }

    
    bool cache_is_empty() const
    {
        return (my_cache.size() == 0);
    }


    void insert_elem(list_elem& elem)
    {
        my_list.emplace_front(elem.key, elem.value);
        my_cache.emplace(my_list.begin());
        my_hash.emplace(elem.key, my_list.begin());
    }

    void erase_elem()
    {
        typename std::set<cache_elem, set_elem_cmp>::const_iterator set_iter = my_cache.begin();
        my_cache.erase(set_iter);
        my_hash.erase(set_iter->list_iter->key);
        printf("%d", set_iter->list_iter->key);
        my_list.erase(set_iter->list_iter);
        
    }

    bool lookup_update(list_elem& elem)
    {
        auto hit = my_hash.find(elem.key);
        if (hit == my_hash.end()) //not found key in hash
        {
            if (cache_is_full())
            {
                erase_elem();
            }
            insert_elem(elem);
            return false;
        }
        else
        {
            printf("NOT DONE YET");
        }
        return true;
    }
}; /* class LFU_cache */
} /* namespace caches */