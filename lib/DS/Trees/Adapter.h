#include <iostream>
#include "./ellen_ext_bst_lf/adapter.h"
// #include "./natarajan_ext_bst_lf/adapter.h"

#include "./../../includes/benchmark.hpp"

template <typename T, typename V>
class Adapter
{
private:
    ds_adapter<T, V> *ds;

public:
    Adapter(args *test)
    {
        const int NUM_THREADS = test->threads;
        const long long KEY_NEG_INFTY = std::numeric_limits<long long>::min() + 1;
        const long long KEY_POS_INFTY = std::numeric_limits<long long>::max() - 1;
        long long unused2 = 0;
        Random64 *const unused3 = NULL;

        this->ds = new ds_adapter<T, V>(NUM_THREADS, KEY_NEG_INFTY, KEY_POS_INFTY, unused2, unused3);

        // const int NUM_THREADS = test->threads;
        // const long long unused1 = 0;
        // const long long KEY_POS_INFTY = std::numeric_limits<int>::max();
        // long long VALUE_RESERVED = (long long)(uintptr_t)(-1);
        // Random64 *const unused2 = NULL;

        // this->ds = new ds_adapter<T, V>(NUM_THREADS, unused1, KEY_POS_INFTY, VALUE_RESERVED, unused2);

        // int threadId[NUM_THREADS];
        for (int i = 0; i < NUM_THREADS; i++)
        {
            this->ds->initThread(i);
        }
    }
    V insert(int thrId, T key, V value)
    {
        return ds->insertIfAbsent(thrId, key, value);
    }

    V insert(int thrId, T key)
    {
        return ds->insertIfAbsent(thrId, key, key);
    }

    void empty(int thrId,T key){
        while(this->remove(thrId,key)!=NULL){};
    }

    V remove(int thrId, T key)
    {
        return ds->erase(thrId, key);
    }

    V search(int thrId, T key)
    {
        return ds->find(thrId, key);
    }

    void initThread(int thrId)
    {
        this->ds->initThread(thrId);
    }
};