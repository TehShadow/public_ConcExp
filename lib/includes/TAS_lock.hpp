

#pragma one

#include <iostream>
#include <atomic>
#include "./helpers.hpp"


class TASLock{

    std::atomic_flag lock = {0};
    int maxtime = 1000;

    TASLock(int maxBackoff){
        this->maxtime = maxBackoff;
    };


    void lock(){
        while(this->lock.test_and_set()){
            backoff(this->maxtime);
        }
    };


    void unlock() { this->lock.clear(); }

};