#include <mutex>
#include "../Stack.hpp"


template <class T>
struct Node{
    T data;
    Node<T>* volatile next;
    std::mutex nodeLock;
    Node(T data){
        this->data = data;
    };
};


template <class T>
class StackSll : public Stack<T>{
    private:
        Node<T>* volatile Top;
        std::mutex stackLock;
    public:
        StackSll(){
            this->Top = NULL;
        }

        bool push(T);
        T pop();
        long long size();
        void empty();
        void printStack();
        T find(T);
};


template <class T>
T StackSll<T>::find(T data){
    Node<T>* temp = this->Top;
    while(temp->next != NULL){
        if(temp->data == data) return data;
        temp = temp->next;
    }
    return -1;
}

template <class T>
void StackSll<T>::empty(){
    while(this->pop()!= -1){};
}

template <class T>
bool StackSll<T>::push(T data){
    Node<T>* newNode = new Node<T>(data);

    this->stackLock.lock();
    if(this->Top == NULL){
        this->Top = newNode;
        this->stackLock.unlock();
        return true;
    }else{
        newNode->next = this->Top;
        this->Top = newNode;
        this->stackLock.unlock();
        return true;
    }
    return false;

}

template <class T>
T StackSll<T>::pop(){
    this->stackLock.lock();
    if(this->Top == NULL){
        this->stackLock.unlock();
        return -1;
    }else{
        Node<T> *current , *prev = NULL;
        T ret_val = -1;
        current = this->Top;
        if(current->next){
            ret_val = current->data;
            this->Top = current->next;
            this->stackLock.unlock();
            return ret_val;
        }else{
            ret_val = this->Top->data;
            this->Top = NULL;
            this->stackLock.unlock();
            return ret_val;
        }
    }
}

template <class T>
long long StackSll<T>::size(){
    int counter = 0;
    Node<T>* current = this->Top;
    while(current != NULL){
        current = current->next;
        counter++;
    }
    return counter;
}


template <class T>
void StackSll<T>::printStack(){
    if(this->Top) return ;
    Node<T>* current = this->Top;
    int counter = 1;
    while(current != NULL){
        std::cout<<counter<<") "<<current->data<<std::endl;
        current = current->next;
        counter++;
    }
}