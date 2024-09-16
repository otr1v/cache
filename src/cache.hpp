#pragma once

#include "head.hpp"

namespace caches
{
using size_type = std::size_t;
static std::size_t NumberOfElems = 0;
static std::size_t MaxCacheSize = UINT32_MAX;

template <typename elem_type, typename key_type = int>  
class LFU_cache
{
public:
    const size_type size;
    
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
        size_type counter;
        size_type age;
        cache_elem(list_it it): list_iter(it), counter(1), age(NumberOfElems)
        {
            printf("HAS CREATED %ld\n", NumberOfElems);
        }
    };
    LFU_cache(const size_type sz): size(sz) {};
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
        NumberOfElems++;
    }

    void erase_elem()
    {
        typename std::set<cache_elem, set_elem_cmp>::const_iterator set_iter = my_cache.begin();
        my_cache.erase(set_iter);
        my_hash.erase(set_iter->list_iter->key);
        printf("%d", set_iter->list_iter->key);
        my_list.erase(set_iter->list_iter);
        
    }
    
    void update_elem(auto iter)
    {
        //printf("before update %d\n", iter->second.counter);
        iter->second.counter++;
        //printf("after update %d\n", NumberOfElems);
        iter->second.age = NumberOfElems;
        NumberOfElems++;
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
            update_elem(hit);
        }
        return true;
    }
}; /* class LFU_cache */

/* Perfect Caching Algorithm */
template <typename elem_type, typename key_type = int> 
class PCA_cache
{
public:
    const size_type size;
    size_type current_elem;
    struct list_elem
    {
        key_type key;
        elem_type value;
        size_type distance;
        list_elem(key_type k, elem_type elem): key(k), value(elem), distance(MaxCacheSize) {}
    };
    using list = std::list<list_elem>;
    list my_list;
    using list_it = typename std::list<list_elem>::iterator;
    struct cache_elem
    {
        list_it list_iter;
        
        cache_elem(list_it it): list_iter(it)
        {
            //printf("created ideal cache elem");
        }
    };

    PCA_cache(const size_type sz): size(sz), current_elem(0) {};

    std::unordered_map<key_type, cache_elem> my_cache;
    bool cache_is_full() const
    {
        return (my_cache.size() == size);
    }

    
    bool cache_is_empty() const
    {
        return (my_cache.size() == 0);
    }

    //template <typename elem_type>
    list_it count_distance(std::vector<elem_type>& all_elems)
    {
        list_it list_iter = my_list.begin();
        list_it iter_to_delete;
        size_type max_distance = 0;
        size_type all_elems_size = all_elems.size();
        for (size_type idx = 0; idx < size; idx++)
        {
            
            elem_type elem_value = list_iter->value;
            size_type current_distance = list_iter->distance;
            for (size_type idx1 = current_elem; idx1 < all_elems_size; idx1++)
            {
                if (elem_value == all_elems[idx1])
                {
                    current_distance = idx1 - current_elem;
                    break;
                }
            }
            if(current_distance == MaxCacheSize)
            {
                iter_to_delete = list_iter;
                break;
            }
            else if (current_distance > max_distance)
            {
                max_distance = current_distance;
                iter_to_delete = list_iter;
            }
            list_iter++;
        }

        return iter_to_delete;
        
    }

    void insert_elem(list_elem& elem)
    {
        my_list.emplace_front(elem.key, elem.value);
        my_cache.emplace(elem.key, my_list.begin());
    }

    void erase_elem(list_it& iter_to_delete)
    {
        printf(" value %d", iter_to_delete->value);
        my_cache.erase(iter_to_delete->key);
        my_list.erase(iter_to_delete);
    }

    //template <typename elem_type>
    bool lookup_update(list_elem& elem, std::vector<elem_type>& all_elems)
    {
        current_elem++;
        auto hit = my_cache.find(elem.key);
        list_it iter_to_delete;
        if (hit == my_cache.end()) //not found key in hash
        {
            if (cache_is_full())
            {
                iter_to_delete = count_distance(all_elems);
                erase_elem(iter_to_delete);
            }
            printf("before");
            insert_elem(elem);
             printf("after");
             printf("%d", my_cache.size());
        }
        else
        {
            printf("HIT\n");
            /* do nothing */
        }
        return 1;
    }
}; /* class PCA_cache*/
} /* namespace caches */