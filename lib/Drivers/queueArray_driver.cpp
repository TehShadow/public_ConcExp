#include "../includes/Benchmark.hpp"
#include "../../lib/DS/Queues/Queue_Array/queueArray.hpp"
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

    QueueArray <int> q;
    methods<int> funcs;

    funcs.add = std::bind(&QueueArray<int>::enqueue,&q,std::placeholders::_1);
    funcs.remove = std::bind(&QueueArray<int>::dequeue,&q);
    funcs.size = std::bind(&QueueArray<int>::size,&q);
    funcs.find = std::bind(&QueueArray<int>::find,&q,std::placeholders::_1);
    funcs.empty = std::bind(&QueueArray<int>::empty,&q);

    // #ifdef VALIDATE
    //     Validator<QueueArray<int>,int> valid(&q,funcs,"Validation",10000,1);
    //     valid.validate();
    //     return 0;
    // #endif
    
    #ifdef BENCH
        Benchmark<QueueArray<int>,int> bench(&q,funcs,test->threads,test->name,test->time,test->actions,test->threadPinning,test->localwork_status,test->localwork_time);
        bench.full_bench(test->iterations);
    #endif
}