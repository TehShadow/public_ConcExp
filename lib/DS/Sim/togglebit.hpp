#pragma once
#include <iostream>
#include <vector>
#include <atomic>
#include <mutex>

using namespace std;


class ToggleBit{
    public:
        bool *boolArray;
        int threadNum;
    ToggleBit(int nthread){
        this->boolArray = new bool[nthread];
        this->threadNum = nthread;
    }

//     ToggleBit* operator= (const ToggleBit other){
//         cout<<this->threadNum<<endl;
//         if (this != &other) {
//             delete[] this->boolArray;
//             boolArray = new bool[this->threadNum];
            
//             for (int i = 0; i < this->threadNum; i++) {
//                 this->boolArray[i] = other.boolArray[i];
//             }
//         }
//         return this;
// }

    void toggle(int index){
        this->boolArray[index] = !this->boolArray[index];
    }
    void resetAll(){
        for(int i=0;i<this->threadNum;i++){
            this->boolArray[i] = false;
        }
    }
    void printArray(){
        for(int i=0;i<this->threadNum;i++){
                cout<<this->boolArray[i]<<" ";
        }
        cout<<"\n"<<endl;
    }

    int findFirst(){
        for(int i=0;i<this->threadNum;i++){
            if(this->boolArray[i]){return i;}
        }
        return -1;
    }
        vector<int>* findActive(){
            vector<int> *openVec = new vector<int>;
            for(int i=0;i<this->threadNum;i++){
                if(this->boolArray[i]) openVec->push_back(i);
            }
            return openVec;
        }
};


class ToggleBit_atomic {
public:
    atomic<bool> *boolArray;
    int threadNum;

    ToggleBit_atomic(int nthread){
        this->boolArray = new atomic<bool>[nthread];
        for(int i=0;i<nthread;i++){
            this->boolArray[i].store(false,memory_order_relaxed);
        }
        this->threadNum = nthread;
    }

    // ToggleBit_atomic& operator=(const ToggleBit_atomic& other) {
    //     for (int i = 0; i < threadNum; ++i) {
    //         boolArray[i].store(other.boolArray[i].load(memory_order_relaxed), memory_order_relaxed);
    //     }
    //     return *this;
    // }

    // ToggleBit_atomic& operator=(const ToggleBit_atomic* other) {
    //     for (int i = 0; i < threadNum; ++i) {
    //         boolArray[i].store(other->boolArray[i].load(memory_order_relaxed), memory_order_relaxed);
    //     }
    //     return *this;
    // }

    void toggle(int index) {
        boolArray[index].store(!boolArray[index].load(memory_order_acquire), memory_order_acquire);
    }

    void resetAll() {
        for (int i = 0; i < threadNum; ++i) {
            boolArray[i].store(false, memory_order_relaxed);
        }
    }
    
    void printArray() {
        for (int i = 0; i < threadNum; ++i) {
            cout << boolArray[i].load(memory_order_relaxed) << endl;
        }
    }

    int findFirst() {
        for (int i = 0; i < threadNum; ++i) {
            if (boolArray[i].load(memory_order_acquire)) {
                return i;
            }
        }
        return -1;
    }

    vector<int> findActive(atomic<bool>* cur_status) {
        mutex guard;
        vector<int> openVec;
        for (int i = 0; i < threadNum; ++i) {
            if (cur_status[i].load(memory_order_acquire) != boolArray[i].load(memory_order_acquire)) {
                openVec.push_back(i);
            }
        }
        return openVec;
    }

};