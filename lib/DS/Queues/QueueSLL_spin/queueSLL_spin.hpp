#include "../../../includes/spinlock_2.hpp"
#include "../Queue.hpp"


template <class T>
struct Node{
    T data;
    Node<T>* volatile next;
    Spinlock nodeLock;
    Node(T data){
        this->data = data;
    };
};


template <class T>
class QueueSll_spin : public Queue<T>{
    private:
        Node<T>* volatile head;
        Spinlock queueLock;
    public:
        QueueSll_spin(){
            this->head = NULL;
        }

        bool enqueue(T);
        T dequeue();
        long long size();
        void printQ();
        void empty();
        T find(T);
};


template <class T>
T QueueSll_spin<T>::find(T data){
    Node<T>* temp = this->head;
    while(temp->next != NULL){
        if(temp->data == data) return data;
        temp = temp->next;
    }
    return -1;
}

template <class T>
void QueueSll_spin<T>::empty(){
    while(this->dequeue() != -1){}
}


template <class T>
bool QueueSll_spin<T>::enqueue(T data){
    Node<T>* newNode = new Node<T>(data);

    this->queueLock.lock();
    if(this->head == NULL){
        this->head = newNode;
        this->queueLock.unlock();
        return true;
    }
    else if(this->head->data == data){
        this->queueLock.unlock();
        return false;
    }
    else if(this->head->data > data){
        newNode->next = this->head;
        this->head = newNode;
        this->queueLock.unlock();
        return true;
    }else{
        Node<T> *current, *prev=NULL;
        current = this->head;
        current->nodeLock.lock();
        this->queueLock.unlock();

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
        current->next = newNode;
        current->nodeLock.unlock();
        
        return true;
    }

}




template <class T>
T QueueSll_spin<T>::dequeue(){
    this->queueLock.lock();
    if(this->head == NULL){
        this->queueLock.unlock();
        return -1;
    }else{
        Node<T> *current , *prev = NULL;
        T ret_val = -1;
        current = this->head;
        if(current->next){
            ret_val = current->data;
            this->head = current->next;
            this->queueLock.unlock();
            return ret_val;
        }else{
            ret_val = this->head->data;
            this->head = NULL;
            this->queueLock.unlock();
            return ret_val;
        }
    }
    return -1;
}

template <class T>
long long QueueSll_spin<T>::size(){
    int counter = 0;
    Node<T>* current = this->head;
    while(current != NULL){
        current = current->next;
        counter++;
    }
    return counter;
}


template <class T>
void QueueSll_spin<T>::printQ(){
    Node<T>* current = this->head;
    int counter = 1;
    while(current != NULL){
        std::cout<<counter<<") "<<current->data<<std::endl;
        current = current->next;
        counter++;
    }
}