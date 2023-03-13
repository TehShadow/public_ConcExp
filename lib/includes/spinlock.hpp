#pragma once
#include <atomic>

//ttas_lock
struct spinlock{
    std::atomic<bool> volatile varLock = {false};

    void lock() {
        for(;;){
            if(!varLock.exchange(true,std::memory_order_acquire)){
                return;
            }
            while(varLock.load(std::memory_order_relaxed)){
                __builtin_ia32_pause();
            }
        }
    }

    bool try_lock() {
        return !varLock.load(std::memory_order_relaxed) && !varLock.exchange(true,std::memory_order_acquire);
    }

    void unlock() {
        varLock.store(false,std::memory_order_release);
    }
};