#pragma once
#include <iostream>
#include <mutex>
#include <vector>
#include <cstring>


template<class X>
class Node{
    public:
        X data;
        Node<X>** lvlArray;
        std::mutex nodeLock;

        Node(X data , int lvl){
            this->data = data;
            this->lvlArray = new Node<X>*[lvl+1];
            memset(this->lvlArray,0,sizeof(Node*)*(lvl+1));
        }

};

template<class X>
class SkipListLocks{
    public:
        int maxlvl = 10;
        Node<X>*sentinel;
        float prop;

    SkipListLocks(int maxlvl , float prop){
        this->maxlvl = maxlvl;
        this->prop = prop;
        this->sentinel = new Node<X>(-1,this->maxlvl);
    }


    int randomlvl();
    bool insert(X);
    Node<X>* remove(X);
    int find(X , Node<X>*[],Node<X>* []);
    void display();
};


template<class X>
int SkipListLocks<X>::randomlvl(){
    float random = (float)rand()/RAND_MAX;
    int lvl = 0;
    while(random<this->prop && lvl<this->maxlvl){
        lvl++;
        random = (float)rand()/RAND_MAX;
    }
    return lvl;
}

template <class X>
void SkipListLocks<X>::display(){
    for(int i= 0;i<=this->maxlvl;i++){
        Node<X> *node = this->sentinel->lvlArray[i];
        std::cout << "Level " << i << ": ";
        while(node != NULL){
            std::cout<<node->data<<" ";
            node = node->lvlArray[i];
        }
        std::cout<<"\n"; 
    }
}


template <class X>
int SkipListLocks<X>::find(X data , Node<X>* preds[] , Node<X>* succs[]){
    int lvlFound = -1;
    Node<X>* pred = this->sentinel;
    for(int level = this->maxlvl-1;level >=0;level--){
        Node<X>* curr = pred->lvlArray[level];
        while(data > curr->data){
            pred = curr;
            curr = pred->lvlArray[level];
        }
        if(lvlFound == -1 && data == curr->data){
            lvlFound = level;
        }
        preds[level] = pred;
        succs[level] = curr;
    }
    return lvlFound;
}

template <class X>
bool SkipListLocks<X>::insert(X data){

    Node<X>* preds[this->maxlvl+1];
    Node<X>* succs[this->maxlvl+1];
    int levelFind = -1;

    while(true){
        levelFind = find(data,preds,succs);
        if(levelFind != -1){
            preds[levelFind]->nodeLock.lock();
            succs[levelFind]->nodeLock.lock();

            Node<X>* nodeFound = succs[levelFind];
        }
        else return false;
        
        int highestLocked = -1;
        Node<X>* pred ,*succ , *prevPred = nullptr;
        int random_lvl = this->randomlvl() + 1;
        std::vector<std::mutex> locks;
        for(int lvl = 0; lvl <= random_lvl;lvl++){
            pred = preds[lvl];
            succ = succs[lvl];
            if(pred != prevPred){
                locks.emplace_back(pred->nodeLock);
                highestLocked = lvl;
                prevPred = pred;
            }
        }

        Node<X>* newNode = new Node<X>(data,levelFind);
        for (int lvl = 0; lvl<=random_lvl;lvl++){
            newNode->lvlArray[lvl] = succs[lvl];
            preds[lvl]->lvlArray[lvl] = newNode;
        }

        for(int lvl = 0; lvl <= highestLocked;lvl++){
            locks[lvl].unlock();
        }
        return true;
    }
};