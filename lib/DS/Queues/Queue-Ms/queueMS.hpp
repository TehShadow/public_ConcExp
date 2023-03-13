#include "../Queue.hpp"
#include "../../../includes/cas.hpp"

template<class T>
struct Node{
    T data;
    Node<T>* next;
    //  T padding[48/4];
        Node(T data){
            this->next = NULL;
            this->data = data;
    };
};
// } __attribute__((packed)) ;


template <class T>
class MsQueue: public Queue<T>{
    private:
         Node<T>*  head;
         Node<T>*  tail;
    public:
        MsQueue(){
            Node<T>* newNode = new Node<T>(-1);
            newNode->data = -1;
            newNode->next = NULL;
            this->head = newNode;
            this->tail = newNode;
        }
        ~MsQueue(){};

        bool enqueue(T);
        T dequeue();
        long long size();
        void printQueue();
        void BackoffDelay();
        void empty();
        T find(T data);
};

template <class T>
T MsQueue<T>::find(T data){
    Node<T>* current = this->head;
    while(current != NULL){
        if(current->data == data)
            return current->data;
    }
    return -1;
}

template <class T>
void MsQueue<T>::empty(){
    int size = this->size();
    for(int i=0;i<=size;i++){
        this->dequeue();
    }
}

template <class T>
void MsQueue<T>::BackoffDelay(){
    srand(10);
    int num = rand()%1024;
    volatile int counter = 0;
    for(int i =0;i<num;i++) counter+i;
}

template <class T>
bool MsQueue<T>::enqueue(T data){
    Node<T>* newnode = new Node<T>(data);
    Node<T>* last , *next;

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

template<class T>
T MsQueue<T>::dequeue(){
    Node<T>* first , *last , *next;
    T ret_val = -1;
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

template <class T>
long long MsQueue<T>::size(){
    long long counter = 0;
    Node<T>* current = this->head;
    while(current != NULL){
        current = current->next;
        counter++;
    }
    return counter;
}

template <class T>
void MsQueue<T>::printQueue(){
    Node<T>* current = this->head;
    int counter = 1;
    while(current != NULL){
        std::cout<<counter<<") "<<current->data<<std::endl;
        current = current->next;
        counter++;
    }
}