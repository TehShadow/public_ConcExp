#include <iostream>
#include <atomic>
#include "./../Stack.hpp"


template<class X>
struct Node{
    X data;
    Node<X>* next;
    Node(X data) : data(data),next(nullptr){}
    Node(): data(NULL),next(nullptr){}
};


template<class X>
class If_Stack_new :public Stack<X>{

    private:
        std::atomic<Node<X>*> head = {nullptr};

    public:


    bool push(X);
    X pop();
    long long size();
    void empty();
    X find(X);

};

template <class X>
X If_Stack_new<X>::find(X data){
    Node<X>* temp = head.load();
    while(temp->next != NULL){
        if(temp->data == data) return data;
        temp = temp->next;
    }
    return -1;
}

template<class X>
void If_Stack_new<X>::empty(){
    while(this->pop() != -1){};
}   

template<class X>
long long If_Stack_new<X>::size(){
    int counter = 0;
    Node<X>* curr = head.load();
    while(curr->next != nullptr){
        counter++;
        curr = curr->next;
    }

    return counter;
}

template<class X>
bool If_Stack_new<X>::push(X data){
    Node<X>* new_node = new Node<X>(data);
    new_node->next = head.load();
    while(!head.compare_exchange_weak(new_node->next,new_node));
    return true;
}

template<class X>
X If_Stack_new<X>::pop(){
    Node<X>* got = head.load();
    Node<X>* nextNode = nullptr;

    do{
        if(got==nullptr){
            return -1;
        }
        nextNode = got->next;
    } while(!head.compare_exchange_weak(got,nextNode));
    return got->data;

}