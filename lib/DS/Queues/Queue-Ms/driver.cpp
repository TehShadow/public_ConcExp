#include "../Queue.hpp"
#include "../../../includes/cas.hpp"
#include "./queueMS.hpp"
#include "../../../includes/millis.hpp"
#include "mutex"
#include <vector>
#include <thread>
#include <pthread.h>

MsQueue<int> q;
pthread_barrier_t barrier;


#define THREAD_NUM 8
#define ELEMENTS 1000000
#define PARTITIONS ELEMENTS/THREAD_NUM

void thread_add(int y){
    int a , b;
    pthread_barrier_wait(&barrier);
    if(y == 0){
        a = GetTimeMillis();
    }   
    for(long i=PARTITIONS*y;i<PARTITIONS*(y+1);i++){
        q.enqueue(i);
    }
    if(y == 0){
        b = GetTimeMillis();
        std::cout<<"Time is : "<<b-a<<" ms"<<std::endl;
    }
}
void thread_remove(int y){
        for(int i=0;i<ELEMENTS;i++){
        q.dequeue();
    }
}

int main(int argc,char* argv[]){

    pthread_barrier_init(&barrier,NULL,THREAD_NUM);
    std::thread threads[THREAD_NUM];
    int threadId[THREAD_NUM];
    for(int i=0;i<THREAD_NUM;i++){
        threads[i] = std::thread(thread_add,i);
    }

    for(int i=0;i<THREAD_NUM;i++){
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(i,&cpuset);
        pthread_setaffinity_np(threads[i].native_handle(),sizeof(cpu_set_t),&cpuset);
    }

    for(int i=0;i<THREAD_NUM;i++){
        threads[i].join();
    }

}
