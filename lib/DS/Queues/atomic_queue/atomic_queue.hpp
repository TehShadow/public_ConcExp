#pragma once
#include "atomic"
#include "pthread.h"
#include "vector"
#include <thread>
#include "./../Queue.hpp"

template <class X>
class atomic_queue:public Queue<X>{
    X* arr;
    volatile int capacityVar = 4;
    std::atomic<int> count{0};
    std::atomic<size_t> index{0};
    std::mutex arrayLock;
    public:
        atomic_queue(){
            arr = new X[capacityVar];
            count = 0;
            index = -1;
        }
        ~atomic_queue(){
            delete[] arr;
        }


        void empty();
        bool isEmpty();
        bool full();
        long long capacity();
        long long size();
        bool enqueue(X x);
        X dequeue();
        void printQueue();
        X find(X);

};

template <class X>
X atomic_queue<X>::find(X data){
    for(int i=0;i<this->size();i++){
        if(this->arr[i] == data) return this->arr[i]; 
    }
    return -1;
}

template <class X>
void atomic_queue<X>::empty(){
    while(this->dequeue() != -1){}
}


template <class X>
bool atomic_queue<X>::isEmpty(){
    if(index == 0) return true;
    else return false;
}


template <class X>
bool atomic_queue<X>::full(){
    if(this->count == capacityVar) return true;
    else return false;
}

template<class X>
long long atomic_queue<X>::capacity(){
    return capacityVar;
}

template<class X>
long long atomic_queue<X>::size(){
    return this->count;
}


template<class X>
bool atomic_queue<X>::enqueue(X x){
    
    this->arrayLock.lock();
    if(full()){
        capacityVar = capacityVar*2;
        X* temp = new X[capacityVar];
        for(int i=0;i<this->count;i++) temp[i]=arr[i];
        delete[] arr;
        arr = temp;
    }
    this->arrayLock.unlock();

    index.fetch_add(1,std::memory_order_release);
    this->arr[index] = x;
    count.fetch_add(1,std::memory_order_release);

    return true;
}


template<class X>
X atomic_queue<X>::dequeue(){
    X temp=-1;
    if(this->isEmpty()) {
        return temp;
    }

    this->arrayLock.lock();
    if(count<capacityVar/2){        
        capacityVar = capacityVar/2;
        X* temp = new X[capacityVar];
        for(int i=0;i<this->count;i++) temp[i]=arr[i];
        delete[] arr;
        arr = temp;
    }
    this->arrayLock.unlock();

    index.fetch_sub(1,std::memory_order_release);
    temp = arr[index];
    arr[index] = 0;
    count.fetch_sub(1,std::memory_order_release);
    return temp;
}


template<class X>
void atomic_queue<X>::printQueue(){
    for(int i=0;i<this->size();i++){
        std::cout<<(i+1)<<" : "<<arr[i]<<std::endl;
    }
}