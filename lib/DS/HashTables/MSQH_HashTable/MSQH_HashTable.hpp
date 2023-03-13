#pragma once
# define maxArray 23
#include "./../../../includes/cas.hpp"
#include "atomic"



#include <mutex>
#include <iostream>


struct Node{
    int data;
    Node* next;
    int idx;
    //  T padding[48/4];
        Node(int data){
            this->next = NULL;
            this->data = data;
    };
    Node(){
        this->next = nullptr;
        this->data = NULL;
    };
};
// } __attribute__((packed)) ;



class MsQueue{
    private:
         Node* head;
         Node* tail;
    public:
        MsQueue(){
            Node* newNode = new Node(-1);
            newNode->data = -1;
            newNode->next = NULL;
            this->head = newNode;
            this->tail = newNode;
        }
        ~MsQueue(){};

        bool enqueue(int);
        int dequeue();
        int find(int);
        long long size();
        void printQueue();
        void BackoffDelay();
        void empty();
};

int MsQueue::find(int data){
    Node *curr = this->head;
    while(curr->next != NULL){
        if(curr->data == data){
            return data;
        }
        else{
            return -1;
        }
    }
    return -1;
}


void MsQueue::empty(){
    int size = this->size();
    for(int i=0;i<=size;i++){
        this->dequeue();
    }
}


void MsQueue::BackoffDelay(){
    srand(10);
    int num = rand()%1024;
    volatile int counter = 0;
    for(int i =0;i<num;i++) counter+i;
}


bool MsQueue::enqueue(int data){
    Node* newnode = new Node(data);
    Node* last , *next;

    while(true){
        last = this->tail;
        next = last->next;
        if(last == this->tail){
            if(next == NULL){
                // this->BackoffDelay();
                if(CAS(&last->next,next,newnode))break;
            }else{
                CAS(&this->tail,last,next);
                // BackoffDelay();
            }
        }
    }
    return -1;
}


int MsQueue::dequeue(){
    Node* first , *last , *next;
    int ret_val = -1;
    while(true){
        first = this->head;
        last = this->tail;
        next = first->next;
        if(first == this->head){
            if(first == last){
                if(next == NULL) return -1;
                CAS(&this->tail,last,next);
                // BackoffDelay();
            }else {
                ret_val = next->data;
                if(CAS(&this->head,first,next)) break;
                // BackoffDelay();
            }
        }
    }
    return ret_val;
}


long long MsQueue::size(){
    long long counter = 0;
    Node* current = this->head;
    while(current != NULL){
        current = current->next;
        counter++;
    }
    return counter;
}

void MsQueue::printQueue(){
    Node* current = this->head;
    int counter = 1;
    while(current != NULL){
        std::cout<<counter<<") "<<current->data<<std::endl;
        current = current->next;
        counter++;
    }
}




class MSQH_Hashtable{
    MsQueue hashtable[maxArray];
    int size = maxArray;

    public:
        MSQH_Hashtable(){};
        bool insert(int data);
        int remove(int data);
        int find(int data);
        void print();
        void print(int idx);
        int count();

        int hash_function(int data) {
            long long i = 0;
            for (int j=0; j<data; j++)
                i += j*data/3;
            return i % maxArray;
        }
};


int MSQH_Hashtable::find(int data){
    int pos = this->hash_function(data);
    int res = hashtable[pos].find(data);
    return res;
}

bool MSQH_Hashtable::insert(int data){
    int pos = this->hash_function(data);
    bool res = hashtable[pos].enqueue(data);
    return res;
}

int MSQH_Hashtable::remove(int data){
    int pos = this->hash_function(data);
    int res = hashtable[pos].dequeue();
    return res;

}

void MSQH_Hashtable::print(){
    for(int i=0;i<this->size;i++){
        std::cout<<"Single Linked List Number #"<<i<<std::endl; 
        hashtable[i].printQueue();
    }
}

void MSQH_Hashtable::print(int idx){
    if(idx > this->size) return;
    else hashtable[idx].printQueue();
}

int MSQH_Hashtable::count(){
    int count = 0;
    for(int i=0;i<this->size;i++){
        std::cout<<i<<") "<<hashtable[i].size()<<std::endl;
        count += this->hashtable[i].size();
    }
    return count;
}