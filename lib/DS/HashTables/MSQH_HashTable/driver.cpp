#include "./MSQH_HashTable.hpp"
#include "./../../../includes/millis.hpp"
#include <vector>
#include "mutex"
#include "pthread.h"
#include "random"
#include <thread>

#define THREAD_NUM 8
#define ELEMENTS 65543
#define PARTITIONS ELEMENTS/THREAD_NUM

MSQH_Hashtable q;
pthread_barrier_t barrier;


void thread_add(int y){
    int a , b;
    pthread_barrier_wait(&barrier);
    if(y == 0){
        a = GetTimeMillis();
    }
    for(long i=PARTITIONS*y;i<PARTITIONS*(y+1);i++){
        q.insert(i);
    }
    if(y == 0){
        b = GetTimeMillis();
        std::cout<<"Time is : "<<b-a<<" ms"<<std::endl;
    }
}
void thread_remove(int y){
        for(int i=0;i<ELEMENTS;i++){
        q.remove(i*y);
    }
}


int main(int argc,char* argv[]){

    
    pthread_barrier_init(&barrier,NULL,THREAD_NUM);


    std::vector<std::thread> thread_vector;
    int threadId[THREAD_NUM];
        cpu_set_t cpuset;
        for(int i=0;i<THREAD_NUM;i++){
        if(true){
            CPU_ZERO(&cpuset);
            CPU_SET(i,&cpuset);
        }
        std::thread th(thread_add,i);

        if(true){
            int rc = pthread_setaffinity_np(th.native_handle(),sizeof(cpu_set_t),&cpuset);
            if(rc != 0){
                std::cerr <<"Error calling pthread setaffinity np:"<<rc<<"\n";
            }
        }
        thread_vector.push_back(move(th));
    }
    for(auto& t: thread_vector){
            t.join();
        }

        thread_vector.clear();
        
        q.print();

    
    // NO THREADPINNING
    // for(int i=0;i<THREAD_NUM;i++){
    //     std::cout<<"Thread # "<<i<<std::endl;
    //     std:: thread th (thread_add,i);
    //     thread_vector.push_back(move(th));

    // }
    // for(auto& t: thread_vector){
    //         t.join();
    // }

    // thread_vector.clear();

    // for(int i=0;i<THREAD_NUM;i++){

    //     std:: thread th (thread_remove,i);
    //     thread_vector.push_back(move(th));

    // }
    // for(auto& t: thread_vector){
    //         t.join();
    // }

    // q.print();

    // std::cout<<"Count num : "<< q.count()<<std::endl;

}

