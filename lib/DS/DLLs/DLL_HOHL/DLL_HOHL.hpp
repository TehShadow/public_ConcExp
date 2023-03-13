#include <mutex>
#include "../DLL.hpp"


template<class X>
struct Node{
    X data;
    Node<X>* volatile next;
    Node<X>* volatile prev;
    std::mutex nodeLock;
    Node(X data){
        this->data = data;
    };
};


template<class X>
class DLL_hand : public DLL<X>{
    private:
        Node<X>* volatile head;
        Node<X>* volatile tail;
        std::mutex listLock;

    public:
        DLL_hand(){
            this->head = NULL;
            this->tail = NULL;
        }

        bool insert(X);
        bool insertTail(X);
        X remove(X);
        X search(X);
        int size();
        void printList();
};



template <class T>
bool DLL_hand<T>::insert(T data){
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
        newNode->next = this->head;
        this->head->prev = newNode;
        this->head = newNode;
        this->listLock.unlock();
        return true;
    }else{
        Node<T> *current, *prev=NULL;
        current = this->head;
        current->nodeLock.lock();
        this->listLock.unlock();

        while(current->next && current->next->data < data){
            prev = current;
            current = current->next;
            current->nodeLock.lock();
            prev->nodeLock.unlock();
        }

        if(current->next && current->next->data == data){
            current->nodeLock.unlock();
            return false;
        }
        

        newNode->next = current->next;
        newNode->prev = current;
        current->next = newNode;
        current->nodeLock.unlock();
        
        return true;
    }
}

template <class T>
bool DLL_hand<T>::insertTail(T data){
    Node<T>* newNode = new Node<T>(data);

    //list is empty
    this->listLock.lock();
    if(this->head == NULL){
        this->head = newNode;
        this->listLock.unlock();
        return true;
    }
    else if(this->tail->data == data){
        this->listLock.unlock();
        return false;
    }
    else if(this->tail->data < data){
        newNode->prev = this->tail;
        this->tail->next = newNode;
        this->tail = newNode;
        this->listLock.unlock();
        return true;
    }else{
        Node<T> *current, *next=NULL;
        current = this->tail;
        current->nodeLock.lock();
        this->listLock.unlock();

        while(current->prev && current->prev->data > data){
            next = current;
            current = current->prev;
            current->nodeLock.lock();
            prev->nodeLock.unlock();
        }

        if(current->prev && current->prev->data == data){
            current->nodeLock.unlock();
            return false;
        }
        

        newNode->prev = current->prev;
        newNode->next = current;
        current->prev = newNode;
        current->nodeLock.unlock();
        
        return true;
    }
}

template <class T>
T DLL_hand<T>::remove(T data){
    this->listLock.lock();
    if (this->head == NULL) {
        this->listLock.unlock();
        return NULL;
    }
    
    this->head->nodeLock.lock();
    if(this->head->data == data ){
        Node<T> *next = this->head;
        this->head = this->head->next;
        this->head->prev = NULL;
        next->nodeLock.unlock();
        this->listLock.unlock();
        return data;
    }else{
        Node<T> *current , *prev=NULL ;

        current = this->head;
        this->listLock.unlock();

        while(current->next != NULL && current->next->data < data){
            prev = current;
            current = current->next;
            current->nodeLock.lock();
            prev->nodeLock.unlock();
        }

        T ret_val = NULL;
        if(current->next != NULL && current->next->data == data){
            Node<T> *next = current->next;
            next->nodeLock.lock();
            current->next = current->next->next;
            current->next->prev = current;
            next->nodeLock.unlock();
            ret_val = data;
        }
        current->nodeLock.unlock();
        return ret_val;
    }
}



template <class X>
int DLL_hand<X>::size(){
    int counter = 0;
    Node<X>* current = this->head;
    while(current != NULL){
        current = current->next;
        counter++;
    }
    return counter;
}

template <class X>
X DLL_hand<X>::search(X data){
    Node<X>* current = this->head;
    T ret_val = NULL;
    while(current != NULL){
        if(current->data == data){
            ret_val = current->data;
            return ret_val;
            }
        current = current->next;
    }
    return ret_val;
}

template <class X>
void DLL_hand<X>::printList(){
    Node<X>* current = this->head;
    int counter = 1;
    while(current != NULL){
        std::cout<<counter<<") "<<current->data<<std::endl;
        current = current->next;
        counter++;
    }
}