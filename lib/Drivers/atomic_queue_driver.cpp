#include "../includes/Benchmark.hpp"
#include "../../lib/DS/Queues/atomic_queue/atomic_queue.hpp"
#include "../includes/validator.hpp"



int main(int argc,char* argv[]){
    #ifdef BENCH
        args *test;
        test = getArgs(argc,argv);
        if(argc<9){
            show_help();
            // return 1;
        }
        printArgs(test);
    #endif




    atomic_queue <int> q;
    methods<int> funcs;

    funcs.add = std::bind(&atomic_queue<int>::enqueue,&q,std::placeholders::_1);
    funcs.remove = std::bind(&atomic_queue<int>::dequeue,&q);
    funcs.size = std::bind(&atomic_queue<int>::size,&q);
    funcs.find = std::bind(&atomic_queue<int>::find,&q,std::placeholders::_1);
    funcs.empty = std::bind(&atomic_queue<int>::empty,&q);

    #ifdef VALIDATE
        Validator<atomic_queue<int>,int> valid(&q,funcs,"Validation",10000,1);
        valid.validate();
        return 0;
    #endif
    #ifdef BENCH

        Benchmark<atomic_queue<int>,int> bench(&q,funcs,test->threads,test->name,test->time,test->actions,test->threadPinning,test->localwork_status,test->localwork_time);
        bench.full_bench(test->iterations);
        return 0;
    #endif
}