#pragma once

#include "head.hpp"

namespace caches
{
using size_type = std::size_t;

/* !!! Needs only for LFU_cache.
    Global is used to count number of elems that we have as input.
    Is used to assign to each element his own identifier(born_data).
    Is used to define what element we need to erase when we have the same counter:
    If born_data of elem1 < born_data of elem2, we erase elem1, because he is older.
     */
static std::size_t NumberOfElems = 0;

static std::size_t MaxCacheSize = UINT32_MAX;
static std::size_t InvalidValue = 0xDEADBABE;


/* Least Frequently used cache */
template <typename elem_type, typename key_type = int>  
class LFU_cache
{
private:
    size_type size;

public:
    struct list_elem
    {
        key_type key;
        elem_type value;
        list_elem(key_type k, elem_type elem): key(k), value(elem) {}
    };

private:
    std::list<list_elem> my_list;
    using list_it = typename std::list<list_elem>::iterator;
    struct cache_elem
    {
        list_it list_iter;
        size_type counter;
        size_type born_data;
        cache_elem(list_it it): list_iter(it), counter(1), born_data(NumberOfElems) { }
    };


    std::unordered_map<key_type, cache_elem> my_hash;

    /* set comparator */
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
                return lhs.born_data < rhs.born_data;
            }
        }
    };

public:
    /* set is used for convinient storage for elems, sorted firstly by counter
       if the counter is the same -> sort by data_born */
    std::set<cache_elem, set_elem_cmp> my_cache;

    LFU_cache(const size_type sz): size(sz) {}; 

    ~LFU_cache()
    {
        my_cache.clear();
        my_hash.clear();
        my_list.clear();
        size = InvalidValue;
    }

    bool check_errors() const
    {
        if (size <= 0)
        {
            std::cout << "size of LFU cache <= 0!" << std::endl;
            return true;
        }
        return false;
    }

private:
    bool cache_is_full() const
    {
        #ifdef DEBUG
            if (check_errors()) return false;
        #endif /* DEBUG */
        return (my_cache.size() == size);
    }

    
    bool cache_is_empty() const
    {
        #ifdef DEBUG
            if (check_errors()) return false;
        #endif /* DEBUG */
        return (my_cache.size() == 0);
    }


    void insert_elem(list_elem& elem)
    {
        #ifdef DEBUG
            if (check_errors()) return;
        #endif /* DEBUG */
        my_list.emplace_front(elem.key, elem.value);
        my_cache.emplace(my_list.begin());
        my_hash.emplace(elem.key, my_list.begin());
        NumberOfElems++;
    }

    void erase_elem()
    {
        #ifdef DEBUG
            if (check_errors()) return;
        #endif /* DEBUG */
        typename std::set<cache_elem, set_elem_cmp>::const_iterator set_iter = my_cache.begin();
        my_cache.erase(set_iter);
        my_hash.erase(set_iter->list_iter->key);
        my_list.erase(set_iter->list_iter);
        
    }
    
    void update_elem(auto iter)
    {
        #ifdef DEBUG
            if (check_errors()) return;
        #endif /* DEBUG */
        iter->second.counter++;
        iter->second.born_data = NumberOfElems;
        NumberOfElems++;
    }

    void print_cache()
    {
        list_it list_iter = my_list.begin();
        std::cout << "CURRENT LFU CACHE SIZE " << size << std::endl;
        for (size_type idx = 0; idx < size; idx++)
        {
            std::cout << list_iter->value << " ";
            list_iter++;
        }
        std::cout << std::endl;
    }

public:
    bool lookup_update(list_elem& elem)
    {
        #ifdef DEBUG
            if (check_errors()) return false;
            print_cache();
        #endif /* DEBUG */
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
private:
    size_type size;
    size_type current_elem;

public:
    struct list_elem
    {
        key_type key;
        elem_type value;
        size_type distance;
        list_elem(key_type k, elem_type elem): key(k), value(elem), distance(MaxCacheSize) {}
    };

private:
    using list = std::list<list_elem>;
    list my_list;
    using list_it = typename std::list<list_elem>::iterator;
    struct cache_elem
    {
        list_it list_iter;
        
        cache_elem(list_it it): list_iter(it) { }
    };

public:
    std::unordered_map<key_type, cache_elem> my_cache;

    PCA_cache(const size_type sz): size(sz), current_elem(0) {};

    ~PCA_cache()
    {
        my_cache.clear();
        my_list.clear();
        current_elem = InvalidValue;
        size = InvalidValue;
    }
    bool check_errors() const
    {
        if (size <= 0)
        {
            std::cout << "size of PCA cache <= 0!" << std::endl;
            return true;
        }
        return false;
    }

private:
    bool cache_is_full() const
    {
        #ifdef DEBUG
            if (check_errors()) return false;
        #endif /* DEBUG */
        return (my_cache.size() == size);
    }

    
    bool cache_is_empty() const
    {
        #ifdef DEBUG
            if (check_errors()) return false;
        #endif /* DEBUG */
        return (my_cache.size() == 0);
    }

    /* This function is used to find the most useless element of cache
       (if it's not worse than inserted elem)
       returns list_it = std::list<list_elem>::iterator to the most useless element
       or my_list.end(); - if the inserted elem is more useless than all elems in cache */
    list_it count_distance(list_elem& inserted_elem, std::vector<elem_type>& all_elems)
    {
        #ifdef DEBUG
            if (check_errors()) return my_list.end();
        #endif /* DEBUG */
        list_it list_iter = my_list.begin();
        list_it iter_to_delete;
        size_type max_distance = 0;
        size_type all_elems_size = all_elems.size();

        inserted_elem.distance = MaxCacheSize;

        for (size_type idx = 0; idx < size; idx++)
        {
            elem_type elem_value = list_iter->value;
            elem_type inserted_elem_value = inserted_elem.value;
            size_type current_distance = list_iter->distance;

            for (size_type idx1 = current_elem; idx1 < all_elems_size; idx1++)
            {
                if (elem_value == all_elems[idx1])
                {
                    current_distance = idx1 - current_elem;
                    break;
                }
                else if (inserted_elem_value == all_elems[idx1] && idx1 != current_elem)
                {
                    inserted_elem.distance = idx1 - current_elem;
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
        if (inserted_elem.distance == MaxCacheSize)
        {
            return my_list.end();
        }
        return iter_to_delete;
        
    }

    void insert_elem(list_elem& elem)
    {
        #ifdef DEBUG
            if (check_errors()) return;
        #endif /* DEBUG */
        my_list.emplace_front(elem.key, elem.value);
        my_cache.emplace(elem.key, my_list.begin());
    }

    void erase_elem(list_it& iter_to_delete)
    {
        #ifdef DEBUG
            if (check_errors()) return;
            std::cout << "deleting elem" << iter_to_delete->value << std::endl;
        #endif /* DEBUG */
        my_cache.erase(iter_to_delete->key);
        my_list.erase(iter_to_delete);
    }

    void print_cache()
    {
        list_it list_iter = my_list.begin();
        std::cout << "CURRENT PCA CACHE SIZE " << size << std::endl;
        for (size_type idx = 0; idx < size; idx++)
        {
            std::cout << list_iter->value << " ";
            list_iter++;
        }
        std::cout << std::endl;
    }

public:
    bool lookup_update(list_elem& elem, std::vector<elem_type>& all_elems)
    {
        #ifdef DEBUG
            if (check_errors()) return false;
            print_cache();
        #endif /* DEBUG */
        current_elem++;
        auto hit = my_cache.find(elem.key);
        if (hit == my_cache.end()) //not found key in hash
        {
            list_it iter_to_delete;
            if (cache_is_full())
            {
                iter_to_delete = count_distance(elem, all_elems);
                if (iter_to_delete != my_list.end())
                {
                    erase_elem(iter_to_delete);
                }
            }
            if (iter_to_delete != my_list.end())
            {
                insert_elem(elem);
            }
        }
        else
        {
            /* do nothing */
            return true;
        }
        return false;
    }
}; /* class PCA_cache*/
} /* namespace caches */

