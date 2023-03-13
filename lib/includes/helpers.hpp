#pragma once
#include <functional>
#include <random>

void backoff(int maxbackoff){
    int backoff;
    srand(time(0));
    backoff = (rand()%maxbackoff)+1;

    volatile int dummy=0;
    for(int i=0;i<backoff;i++){
        dummy += i;
    }
}


template <class X>
struct methods
{
    std::function<bool(X)> add;
    std::function<X()> remove;
    std::function<X(X)> removeElement;
    std::function<int()> size;
    std::function<X(X)> find;
    std::function<void()> empty;
};

template <class X,class Y>
struct StateDSmethods
{
    std::function<void(Y,X)>add;
    std::function<X(Y)>remove; 
};

enum test_types
{       
        ADD,
        REMOVE,
        ADD_REMOVE,
        R_ADD_REMOVE,
        T_ADD_REMOVE,
        T_R_ADD_REMOVE,
};

typedef struct results
{
    volatile long long time = 0;
    volatile long long actions = 0;
    volatile long long inserts = 0;
    volatile long long removes = 0;
} result;