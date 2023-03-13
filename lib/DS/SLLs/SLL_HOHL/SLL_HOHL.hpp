#pragma once

#include "mutex"
#include "../SLL.hpp"


template<class T>
struct Node{
    T data;
    Node<T>* volatile nextNode;
    std::mutex nodeLock;
    Node(T data){
        this->data = data;
    };
};

template <class T>
class SLL_hand : public SLL<T>{
    private:
        Node<T>* volatile head;
        std::mutex listLock;
        
    public:
        SLL_hand(){
            this->head = NULL;
        }
        
        bool insert(T);
        T remove(T);
        T removeFirst();
        T search(T);
        long long size();
        void printList();
        void empty();
};


template <class T>
T SLL_hand<T>::removeFirst(){
    this->listLock.lock();
    if (this->head == NULL) {
        this->listLock.unlock();
        return -1;
    }
    this->head->nodeLock.lock();
    if(this->head){
        Node<T> *next = this->head;
        this->head = this->head->nextNode;
        next->nodeLock.unlock();
        this->listLock.unlock();
        return next->data;
    }
    return -1;
}
template <class T>
void SLL_hand<T>::empty(){
    while(this->removeFirst()!=-1){};
}


template <class T>
bool SLL_hand<T>::insert(T data){
    Node<T>* newNode = new Node<T>(data);

    //list is empty
    this->listLock.lock();
    if(this->head == NULL){
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
        Node<T> *current, *prev=NULL;
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

template <class T>
T SLL_hand<T>::remove(T data){
    this->listLock.lock();
    if (this->head == NULL) {
        this->listLock.unlock();
        return -1;
    }
    
    this->head->nodeLock.lock();
    if(this->head->data == data ){
        Node<T> *next = this->head;
        this->head = this->head->nextNode;
        next->nodeLock.unlock();
        this->listLock.unlock();
        return data;
    }else{
        Node<T> *current , *prev=NULL ;

        current = this->head;
        this->listLock.unlock();

        while(current->nextNode != NULL && current->nextNode->data < data){
            prev = current;
            current = current->nextNode;
            current->nodeLock.lock();
            prev->nodeLock.unlock();
        }

        T ret_val = -1;
        if(current->nextNode != NULL && current->nextNode->data == data){
            Node<T> *next = current->nextNode;
            next->nodeLock.lock();
            current->nextNode = current->nextNode->nextNode;
            next->nodeLock.unlock();
            ret_val = data;
        }
        current->nodeLock.unlock();
        return ret_val;
    }
}

template <class T>
long long SLL_hand<T>::size(){
    int counter = 0;
    Node<T>* current = this->head;
    while(current != NULL){
        current = current->nextNode;
        counter++;
    }
    return counter;
}
template <class T>
T SLL_hand<T>::search(T data){
    Node<T>* current = this->head;
    T ret_val = -1;
    while(current != NULL){
        if(current->data == data){
            ret_val = current->data;
            return ret_val;
            }
        current = current->nextNode;
    }
    return ret_val;
}

template <class T>
void SLL_hand<T>::printList(){
    Node<T>* current = this->head;
    int counter = 1;
    while(current != NULL){
        std::cout<<counter<<") "<<current->data<<std::endl;
        current = current->nextNode;
        counter++;
    }
}