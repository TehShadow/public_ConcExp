
#include <iostream>
#define __SWAP(A, B)  __sync_lock_test_and_set((long *)A, (long)B)

class CLHLock{

    struct CLHLockNode{
        bool locked;
        CLHLockNode(){
            this->locked = false;
        }
    };

    struct CLHLockStruct{
        volatile CLHLockNode *tail;
        volatile CLHLockNode **cur;
        volatile CLHLockNode **curPred;
        CLHLockStruct(int nthreads){
            this->tail = new CLHLockNode();
            this->cur = new CLHLockNode*[nthreads];
            this->curPred = new CLHLockNode*[nthreads];
            for(int i=0;i<nthreads;i++){
                this->cur[i] = new CLHLockNode();
                this->curPred[i] = NULL;
            }
        }
    };

    int nthreads;
    CLHLockStruct *L;


    CLHLock(int nthreads){
        this->nthreads = nthreads;
        this->L = new CLHLockStruct(nthreads);
    };

    void lock(int pid);
    void unlock(int pid);

};


void CLHLock::lock(int pid){
    this->L->cur[pid]->locked = true;
    this->L->curPred[pid] = (CLHLockNode *)__SWAP(&this->L->tail , this->L->cur[pid]);
    while(this->L->cur[pid]->locked == true){};
   
}

void CLHLock::unlock(int pid){
    this->L->cur[pid]->locked= false;
    this->L->cur[pid] = this->L->curPred[pid];
}