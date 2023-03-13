#pragma once
#include "iostream"
#include "atomic"

class Spinlock{
    private:
        std::atomic_flag flag = ATOMIC_FLAG_INIT;
    public:
        void lock(){
                while(flag.test_and_set(std::memory_order_acquire));
        }
        void unlock(){
            flag.clear(std::memory_order_release);
        }
};