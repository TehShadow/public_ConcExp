#include "./sim.hpp"
#include "./SLL.hpp"
#include <thread>
#include <functional>
#define NTHREADS 8
#define ACTIONS 8000
using namespace std;

SLL yo;
func_array<SLL,int> f_array;
Psim<SLL,int> sim(&yo,NTHREADS,f_array);
mutex main_guardian;
void worker_add(int pid){
    Request<int> *req = new Request<int>();
    for(int i=(ACTIONS/NTHREADS)*pid;i<(ACTIONS/NTHREADS)*(pid+1);i++){
        #ifdef DEBUG 
            lock_guard<mutex> lock(main_guardian);
                {
                    cout<<"Thread : "<< pid << " Request : "<<i<<endl;
                }
        #endif
        req->func = addType;
        req->args = i;
        sim.PsimApplyOp(*req,pid);    
    }
}

void printAllObjects(){
    SLL temp;
    for(int i=0;i<NTHREADS;i++){
        for(int y=0;y<2;y++){
            temp = *sim.St[y*i].object;
            cout<<"Thread List "<<i<<" index "<< y <<" : ";
            temp.print();
        }
    }
}

int main(){

    thread threadhandle[NTHREADS];
    for(int i=0;i<NTHREADS;i++){
        threadhandle[i] = thread(worker_add,i);
    }

    for(int i=0;i<NTHREADS;i++){
        threadhandle[i].join();
    }


    printAllObjects();

    yo = *sim.curr_state->state->object;
    cout<<"Final List : ";
    yo.print();
    yo.mergeSort();
    cout<<"Ordered : ";
    yo.print();
    cout<<"Size : "<<yo.size()<<endl;
    cout<<"Doubles : "<<yo.count_doubles_in_seq()<<endl;
    cout<<"CAS succeeded: "<<sim.cas_suc.load()<<endl;
    cout<<"CAS failed: "<<sim.cas_fail.load()<<endl;
    cout<<"Total CAS : "<<sim.cas_fail.load() + sim.cas_suc.load()<<endl;
}

