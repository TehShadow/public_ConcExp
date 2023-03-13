#pragma once
#include <iostream>
#include <atomic>
#include "../../../includes/cas.hpp"

#define _CAS32(A, B, C) __sync_bool_compare_and_swap(A, B, C)
#define CAS32(A,B,C) _CAS32((volatile int*)(A),(int)B,(int)(C))

class FcStack{

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
        volatile Node * volatile top;
        FcStruct(int nthreads){
            this->lock = 0;
            this->count = 0;
            this->counter = 0;
            this->head = NULL;
            this->rounds = 0;
            this->nodes = new FcRequest[nthreads];
            this->top = NULL;
        }
    };

public:
    struct FcThreadState {
        FcRequest *node;
        FcThreadState(int pid , FcStruct *S){
            this->node = &S->nodes[pid];
            this->node->age = 0;
            this->node->active = false;
        }
    };

    int nthreads;
    FcStruct *S;

    int FC_CLEANUP_FREQUENCY=100;
    int FC_CLEANUP_OLD_THRESHOLD=100;
    int FC_COMBINING_ROUNDS=3;

    
        FcStack(int nthreads){
            this->nthreads = nthreads;
            this->S = new FcStruct(nthreads);
        }


        int serialPushPop(int value);
        int FcApplyOp(FcThreadState *st_thread,int value);
        void FcEnqueueRequest(FcThreadState *st_thread);
        void FcStackPush(FcThreadState *st_thread,int value);
        int FcStackPop(FcThreadState *st_thread);
        void printQueue();
        int size();


};

void FcStack::printQueue(){
    volatile Node* cur;
    for(cur = this->S->top; cur !=NULL;cur=cur->next){
        std::cout<<cur->value<<std::endl;
    }
}

int FcStack::size(){
    int count = 0;
    volatile Node* cur;
    for(cur = this->S->top; cur !=NULL;cur=cur->next){
        count++;
    }
    return count;
}

void FcStack::FcStackPush(FcThreadState *st_thread,int value){
    this->FcApplyOp(st_thread,value);
}

int FcStack::FcStackPop(FcThreadState *st_thead){
    int value = -1;
    int ret = this->FcApplyOp(st_thead,value);
    return ret;
}

int FcStack::serialPushPop(int value){
    if(value < 0){
        volatile Node *node = this->S->top;

        if(this->S->top != NULL){
            this->S->top = this->S->top->next;
            return node->value;
        } else return -1;
    } else {
        Node *newNode = new Node(value);
        newNode->next = this->S->top;
        this->S->top = newNode;

        return 0;
    }
}

void FcStack::FcEnqueueRequest(FcThreadState *st_thread){
    volatile FcRequest *request = st_thread->node;
    volatile FcRequest *supposed;
    request->active = true;

    do {
        supposed = this->S->head;
        request->next = supposed;
    } while (!CAS(&this->S->head,supposed,request));
}


int FcStack::FcApplyOp(FcThreadState *st_thread,int value){
    FcRequest *request;
    int locked = 1;
    int unlocked = 0;

    request = st_thread->node;
    request->value = value;
    request->pending = true;

    while(true){
        if(this->S->lock == 0 && CAS32(&this->S->lock,0,1)){
            break;
        }
        else {
            while(this->S->lock && request->pending && request->active){};

            if(request->pending == false){
                return request->value;
            }
            else if(request->active ==false){
                FcEnqueueRequest(st_thread);
            }

        }
    }

    if(request->active == false) FcEnqueueRequest(st_thread);
    this->S->count = 1;
    int count = this->S->count;


    volatile FcRequest *cur;
    for(int i=0;i<this->FC_COMBINING_ROUNDS;i++){
        for(cur = this->S->head; cur !=NULL;cur=cur->next){
            if(cur->pending){
                cur->value = this->serialPushPop(value);
                cur->pending = false;
                cur->age = count;
            }
        }
    }

    if(!(count % this->FC_CLEANUP_FREQUENCY)){
        volatile FcRequest *prev = this->S->head;
        while((cur=prev->next)){
            if((cur->age + this->FC_CLEANUP_OLD_THRESHOLD) < count){
                prev->next = cur->next;
                cur->active = false;
            }
            else prev = cur;
        }
    }

    this->S->lock = 0;

    return request->value;
}