#pragma once

#include <iostream>
#include "../Queue.hpp"
#include <mutex>

template<class X>
class QueueArray: public Queue<X>{
    private:
        volatile int frontIndex;
        volatile int backIndex;
        X* arr;
        volatile int capacityVar = 4;
        volatile int count = 4;
        std::mutex arrayLock;
    public:
        QueueArray(){
            arr = new X[capacityVar];
            frontIndex = backIndex = -1;
        }
        ~QueueArray(){
            delete[] arr;
        }

        void empty();
        bool isEmpty();
        bool full();
        bool enqueue(X x);
        X dequeue();
        X front();
        X back();
        int capacity();
        long long size();
        X find(X);
};

template <class X>
X QueueArray<X>::find(X data){
    for(int i=0;i<this->size();i++){
        if(this->arr[i] == data) return data;
    }
    return -1;
}

template <class X>
void QueueArray<X>::empty(){
    while(this->dequeue() != 0){}
}

template <class X>
bool QueueArray<X>::isEmpty(){
    if(frontIndex == -1 && backIndex -1) return true;
    else return false;
}

template <class X>
bool QueueArray<X>::full(){
    if(this->count == capacityVar) return true;
    else return false;
}

template<class X>
int QueueArray<X>::capacity(){
    return capacityVar;
}

template<class X>
long long QueueArray<X>::size(){
    return this->count;
}

template<class X>
X QueueArray<X>::front(){
    if(empty()) return NULL;
    return arr[backIndex];
}

template<class X>
X QueueArray<X>::back(){
    if(this->isEmpty()) return NULL;
    return arr[frontIndex];
}


template<class X>
bool QueueArray<X>::enqueue(X x){
    this->arrayLock.lock();
    if(full()){
        capacityVar = capacityVar*2;

        X* temp = new X[capacityVar];

        for(int i=0;i<this->count;i++) temp[i]=arr[i];
        delete[] arr;
        arr = temp;
    }

    if(this->isEmpty()){
        frontIndex = backIndex = 0;
        arr[backIndex] = x;
        this->count++;
        this->arrayLock.unlock();
        return 1;
    }

    backIndex = (backIndex+1)%capacityVar;
    arr[backIndex]=x;
    this->count++;
    this->arrayLock.unlock();
    return 1;
}

template<class X>
X QueueArray<X>::dequeue(){
    this->arrayLock.lock();
    if(this->isEmpty()) {
        this->arrayLock.unlock(); 
        return 0;
    }
    if(frontIndex == backIndex){
        frontIndex = backIndex = -1;
        this->count--;
        this->arrayLock.unlock();
        return arr[frontIndex];
    }

    frontIndex = (frontIndex + 1)% capacityVar;
    this->count--;
    this->arrayLock.unlock();
    return arr[frontIndex];
}


