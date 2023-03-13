#include "./sim.hpp"
#include "./SLL.hpp"
#include <thread>
#include <functional>
#define NTHREADS 2
#define ACTIONS 1000
using namespace std;

template<class X>
struct addSum{
    X count = 0;
    addSum(){
        this->count = 0 ;
    }
    int insert(X data){
        this->count += data;
        return 1;
    }

    addSum* copy(){
        addSum* cp = new addSum;
        cp->count = this->count;
        return cp;
    }

    X remove(){
        this->count -= 1;
        return -1;
    }

    void print(){
        cout<<"Counter >> "<<count<<endl;
    }   
};

addSum<int> sum_test;
func_array<addSum<int>,int> f_array;
Psim<addSum<int>,int> sim(&sum_test,NTHREADS,f_array);
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
        // req->func = removeType;
        // sim.PsimApplyOp(*req,pid);
    }
}

void printAllObjects(){
    addSum<int> temp;
    for(int i=0;i<NTHREADS;i++){
        for(int y=0;y<2;y++){
            temp = *sim.St[y][i].object;
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

    auto sum_continuous_numbers = [](int actions){ return (actions * (actions+1))/2;};

    sum_test = *sim.curr_state->state->object;
    cout<<"Final List : ";
    sum_test.print();
    int must_sum = sum_continuous_numbers(ACTIONS);
    cout<<"Should be : "<<must_sum<<endl;
    cout<<"CAS changes : "<<sim.cas_suc<<endl;
}

