#pragma once
# define maxArray 23

#include <mutex>
#include <iostream>

class Node{
    public:
        int data;
        Node *volatile nextNode;
        std::mutex nodeLock;
        Node(int data){
            this->data = data;
            this->nextNode = nullptr;
        }
        Node(){
            this->data = -1;
            this->nextNode = nullptr;
        }
};

class SLL_hand{
    private:
        std::mutex listLock;
        
    public:
        Node* volatile head;
        SLL_hand(){
            Node *sent = new Node();
            this->head = sent;
        }
        
        bool insert(int);
        int remove(int);
        int removeFirst();
        int search(int);
        long long size();
        void printList();
        void empty();
};

int SLL_hand::removeFirst(){
    this->listLock.lock();
    if (this->head == NULL) {
        this->listLock.unlock();
        return -1;
    }
    this->head->nodeLock.lock();
    if(this->head){
        Node *next = this->head;
        this->head = this->head->nextNode;
        next->nodeLock.unlock();
        this->listLock.unlock();
        return next->data;
    }
    return -1;
}

void SLL_hand::empty(){
    while(this->removeFirst()!=-1){};
}

bool SLL_hand::insert(int data){
    Node* newNode = new Node(data);

    //list is empty
    this->listLock.lock();
    if(this->head->data == -1){
        this->head = newNode;
        this->listLock.unlock();
        return true;
    }
    else if(this->head->data == data){
        this->listLock.unlock();
        return false;
    }
    else if(this->head->data > data){
        newNode->nextNode = this->head;
        this->head = newNode;
        this->listLock.unlock();
        return true;
    }else{
        Node *current, *prev=NULL;
        current = this->head;
        current->nodeLock.lock();
        this->listLock.unlock();

        while(current->nextNode && current->nextNode->data < data){
            prev = current;
            current = current->nextNode;
            current->nodeLock.lock();
            prev->nodeLock.unlock();
        }

        if(current->nextNode && current->nextNode->data == data){
            current->nodeLock.unlock();
            return false;
        }

        newNode->nextNode = current->nextNode;
        current->nextNode = newNode;
        current->nodeLock.unlock();
        
        return true;
    }
}

int SLL_hand::remove(int data){
    this->listLock.lock();
    if (this->head == NULL) {
        this->listLock.unlock();
        return -1;
    }
    
    this->head->nodeLock.lock();
    if(this->head->data == data ){
        Node *next = this->head;
        this->head = this->head->nextNode;
        next->nodeLock.unlock();
        this->listLock.unlock();
        return data;
    }else{
        Node *current , *prev=NULL ;

        current = this->head;
        this->listLock.unlock();

        while(current->nextNode != NULL && current->nextNode->data < data){
            prev = current;
            current = current->nextNode;
            current->nodeLock.lock();
            prev->nodeLock.unlock();
        }

        int ret_val = -1;
        if(current->nextNode != NULL && current->nextNode->data == data){
            Node *next = current->nextNode;
            next->nodeLock.lock();
            current->nextNode = current->nextNode->nextNode;
            next->nodeLock.unlock();
            ret_val = data;
        }
        current->nodeLock.unlock();
        return ret_val;
    }
}

long long SLL_hand::size(){
    int counter = 0;
    Node* current = this->head;
    while(current != NULL){
        current = current->nextNode;
        counter++;
    }
    return counter;
}

int SLL_hand::search(int data){
    Node* current = this->head;
    int ret_val = -1;
    while(current != NULL){
        if(current->data == data){
            ret_val = current->data;
            return ret_val;
            }
        current = current->nextNode;
    }
    return ret_val;
}

void SLL_hand::printList(){
    Node* current = this->head;
    int counter = 1;
    while(current != NULL){
        std::cout<<counter<<") "<<current->data<<std::endl;
        current = current->nextNode;
        counter++;
    }
}

class SH_Hashtable{
    SLL_hand hashtable[maxArray];
    int size = maxArray;

    public:
        SH_Hashtable(){};
        bool insert(int data);
        int remove(int data);
        int find(int data);
        void print();
        void print(int idx);
        int count();
        void empty();

        int hash_function(int data) {
            long long i = 0;
            for (int j=0; j<data; j++)
                i += j*data/3;
            return i % maxArray;
        }
};

void SH_Hashtable::empty(){
    for(int i=0;i<maxArray;i++){
       while(hashtable[i].head != NULL){
        hashtable[i].removeFirst();
       }
    }
}

int SH_Hashtable::find(int data){
    int pos = this->hash_function(data);
    int res = hashtable[pos].search(data);
    return res;
}

bool SH_Hashtable::insert(int data){
    int pos = this->hash_function(data);
    bool res = hashtable[pos].insert(data);
    return res;
}

int SH_Hashtable::remove(int data){
    int pos = this->hash_function(data);
    int res = hashtable[pos].remove(data);
    return res;

}

void SH_Hashtable::print(){
    for(int i=0;i<this->size;i++){
        std::cout<<"Single Linked List Number #"<<i<<std::endl; 
        hashtable[i].printList();
    }
}

void SH_Hashtable::print(int idx){
    if(idx > this->size) return;
    else hashtable[idx].printList();
}

int SH_Hashtable::count(){
    int count = 0;
    for(int i=0;i<this->size;i++){
        std::cout<<i<<") "<<hashtable[i].size()<<std::endl;
        count += this->hashtable[i].size();
    }
    return count;
}