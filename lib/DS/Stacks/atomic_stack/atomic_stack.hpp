#include "iostream"
#include "atomic"
#include "thread"
#include "vector"
using namespace std;


struct Node{
    int val;
    Node* next;
    
    explicit Node(int val){
        this->val = val;
        this->next = nullptr;
    }
    explicit Node(){
        this->val = -1;
        this->next = nullptr;
    }


};

class AtomicStack{
    atomic<Node *> head;
    public:
        AtomicStack(){
            Node* head = new Node();
            this->head.store(head);
        }

        bool push(int val){
            Node* const newVal = new Node(val);
            newVal->next = atomic_load_explicit(&head, memory_order_relaxed);
            while (!atomic_compare_exchange_strong_explicit(&head, &newVal->next,newVal,memory_order_release, memory_order_relaxed)){}
            return true;
        }

        int pop(){
            Node* old_head = atomic_load_explicit(&head, std::memory_order_relaxed);
            while (old_head && !atomic_compare_exchange_strong_explicit( &head, &old_head, old_head->next,memory_order_acquire, memory_order_relaxed)) {}
        if (!old_head) return -1;
        int result = old_head->val;
        delete old_head;
        return result;

    }

    void print(){
    Node* current = atomic_load_explicit(&head, memory_order_relaxed);
    while (current) {
      cout << current->val<< std::endl;
      current = current->next;
        }
    }
};