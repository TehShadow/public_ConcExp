#include "../Stack.hpp"
#include "../../../includes/cas.hpp"

template <class T>
struct Node{
    T data;
    Node<T>* next;
    Node(T data){
        this->data = data;
    };
};


template <class T>
class lfStack : public Stack<T>{
    private:
        Node<T>* Top;
    public:
        lfStack(){
            this->Top = NULL;
        }

        bool push(T);
        T pop();
        long long size();
        void printStack();
        void BackoffDelay();
        void empty();
        T find(T);
};

template <class T>
T lfStack<T>::find(T data){
    Node<T>* temp = this->Top;
    while(temp->next != NULL){
        if(temp->data == data) return data;
        temp = temp->next;
    }
    return -1;
}

template <class T>
void lfStack<T>::empty(){
    while(this->pop()!=-1){};
}

template <class T>
void lfStack<T>::BackoffDelay(){
    srand(10);
    int num = rand()%512;
    volatile int counter = 0;
    for(int i =0;i<num;i++) counter+i;
}

template<class T>
bool lfStack<T>::push(T data) {
    Node<T>* newnode = new Node<T>(data);

    do {
        Node<T> *old_Top = (Node<T>*) this->Top; // Top is volatile
        newnode->next = old_Top;
        if (CAS(&this->Top, old_Top, newnode) == true)
            return true;    
        else
            BackoffDelay();
    } while (true);

    return false;
}

template<class T>
inline T lfStack<T>::pop() {
    BackoffDelay();
    do {
        Node<T> *old_Top = (Node<T>*)this->Top;
        if (old_Top == NULL)
            return -1;
        if (CAS(&this->Top, old_Top, old_Top->next))
            return old_Top->data;
        else
            BackoffDelay();
    } while (true);
}


template <class T>
long long lfStack<T>::size(){
    int counter = 0;
    Node<T>* current = this->Top;
    while(current != NULL){
        current = current->next;
        counter++;
    }
    return counter;
}


template <class T>
void lfStack<T>::printStack(){
    if(this->Top) return ;
    Node<T>* current = this->Top;
    int counter = 1;
    while(current != NULL){
        std::cout<<counter<<") "<<current->data<<std::endl;
        current = current->next;
        counter++;
    }
}