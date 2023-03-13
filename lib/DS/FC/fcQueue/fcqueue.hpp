


#pragma once
#include <iostream>
#include <atomic>
#include "../../../includes/cas.hpp"

#define _CAS32(A, B, C) __sync_bool_compare_and_swap(A, B, C)
#define CAS32(A,B,C) _CAS32((volatile int*)(A),(int)B,(int)(C))


class FcQueue{

    struct Node{
        int value;
        volatile Node *next;
        Node(int val){
            this->value = val;
            this->next = NULL;
        }
        Node(){
            this->value = -1;
            this->next = NULL;
        }
    };
    struct FcRequest{
        volatile FcRequest *next;
        volatile int value;
        volatile int age;
        volatile bool active;
        volatile bool pending;
    };

    struct FcStruct{
        volatile struct FcRequest *head;
        std::atomic <int> lock;
        FcRequest *nodes;
        volatile int counter;
        volatile int count;
        volatile int rounds;
        FcStruct(int nthreads){
            this->lock = 0;
            this->count = 0;
            this->counter = 0;
            this->head = NULL;
            this->rounds = 0;
            this->nodes = new FcRequest[nthreads];
        }
    };

    struct FcQueueStruct{
        int nthreads;
        FcStruct *enqueue;
        FcStruct *dequeue;

        volatile Node *last;
        volatile Node *first;

        FcQueueStruct(int nthreads){
            this->nthreads = nthreads;
            this->enqueue = new FcStruct(nthreads);
            this->dequeue = new FcStruct(nthreads);
            this->last = NULL;
            this->first = NULL;
        }
    };

    public:

        struct FcThreadState{
            FcRequest *node;
            FcThreadState(int pid , FcStruct *S){
            this->node = &S->nodes[pid];
            this->node->age = 0;
            this->node->active = false;
            }
        };

        struct FcQueueThreadState{
            FcThreadState *enqueue_thead_state;
            FcThreadState *dequeue_thread_state;

            FcQueueThreadState(int pid ,FcQueueStruct *S){
                this->enqueue_thead_state = new FcThreadState(pid,S->enqueue);
                this->dequeue_thread_state = new FcThreadState(pid,S->dequeue);
            }
        };

        int nthreads;
        FcQueueStruct *S;

        int FC_CLEANUP_FREQUENCY=100;
        int FC_CLEANUP_OLD_THRESHOLD=100;
        int FC_COMBINING_ROUNDS=3;

        FcQueue(int nthreads){
            this->nthreads = nthreads;
            this->S = new FcQueueStruct(nthreads);
        }

        int FcApplyOp(FcStruct *S,FcThreadState *st_thread,int value=-1);
        void FcEnqueueRequest(FcStruct *S,FcThreadState *st_thread);

        void FcQueueApplyEnqueue(FcQueueThreadState *st_thread,int value);
        int FcQueueApplyDequeue(FcQueueThreadState *st_thread); 

        int serialEnqueue(int value);
        int serialDequeue();

        int size();
        void printQueue();
        

};


void FcQueue::printQueue(){
    volatile Node* cur;
    for(cur = this->S->first; cur !=NULL;cur=cur->next){
        std::cout<<cur->value<<std::endl;
    }
}

int FcQueue::size(){
    int count = 0;
    volatile Node* cur;
    for(cur = this->S->first; cur !=NULL;cur=cur->next){
        count++;
    }
    return count;
}


void FcQueue::FcQueueApplyEnqueue(FcQueueThreadState *st_thread,int value){
    FcApplyOp(this->S->enqueue,st_thread->enqueue_thead_state,value);
}

int FcQueue::FcQueueApplyDequeue(FcQueueThreadState *st_thread){
    return FcApplyOp(this->S->dequeue,st_thread->dequeue_thread_state);
}


void FcQueue::FcEnqueueRequest(FcStruct *S, FcThreadState *st_thread){
    volatile FcRequest *request = st_thread->node;
    volatile FcRequest *supposed;
    request->active = true;

    do {
        supposed = S->head;
        request->next = supposed;
    } while (!CAS(&S->head,supposed,request));
}

int FcQueue::FcApplyOp(FcStruct *S,FcThreadState *st_thread,int value){
    FcRequest *request;
    int locked = 1;
    int unlocked = 0;

    request = st_thread->node;
    request->value = value;
    request->pending = true;

    while(true){
        if(S->lock == 0 && CAS32(&S->lock,0,1)){
            break;
        }
        else {
            while(S->lock && request->pending && request->active){};

            if(request->pending == false){
                return request->value;
            }
            else if(request->active ==false){
                FcEnqueueRequest(S,st_thread);
            }

        }
    }

    if(request->active == false) FcEnqueueRequest(S,st_thread);
    S->count = 1;
    int count = S->count;


    volatile FcRequest *cur;
    for(int i=0;i<this->FC_COMBINING_ROUNDS;i++){
        for(cur = S->head; cur !=NULL;cur=cur->next){
            if(cur->pending){
                if(value < 1){
                    cur->value = this->serialDequeue();
                }
                else cur->value = this->serialEnqueue(value);
                cur->pending = false;
                cur->age = count;
            }
        }
    }

    if(!(count % this->FC_CLEANUP_FREQUENCY)){
        volatile FcRequest *prev = S->head;
        while((cur=prev->next)){
            if((cur->age + this->FC_CLEANUP_OLD_THRESHOLD) < count){
                prev->next = cur->next;
                cur->active = false;
            }
            else prev = cur;
        }
    }

    S->lock = 0;

    return request->value;
}


int FcQueue::serialEnqueue(int value){
    volatile Node *newNode = new Node(value);
    if(this->S->last == NULL && this->S->first == NULL){
        this->S->first = newNode;
        this->S->last = newNode;
    }
    this->S->last->next = newNode;
    this->S->last = newNode;
    return 0;
}

int FcQueue::serialDequeue(){
    volatile Node *ret;
    if(this->S->first == this->S->last && this->S->first == NULL){
        return -1;
    }
    else if(this->S->first == this->S->last){
        ret = this->S->first;
        this->S->first = this->S->last = NULL;
        return ret->value;
    }
    else{
        ret = this->S->first;
        this->S->first = this->S->first->next;
        return ret->value;
    }
}