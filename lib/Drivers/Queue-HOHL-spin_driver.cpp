#include "../includes/Benchmark.hpp"
#include "../../lib/DS/Queues/QueueSLL_spin/queueSLL_spin.hpp"
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

    QueueSll_spin <int> q;

    methods<int> funcs;

    funcs.add = std::bind(&QueueSll_spin<int>::enqueue,&q,std::placeholders::_1);
    funcs.remove = std::bind(&QueueSll_spin<int>::dequeue,&q);
    funcs.size = std::bind(&QueueSll_spin<int>::size,&q);
    funcs.find = std::bind(&QueueSll_spin<int>::find,&q,std::placeholders::_1);
    funcs.empty = std::bind(&QueueSll_spin<int>::empty,&q);

    #ifdef VALIDATE
        Validator<QueueSll_spin<int>,int> valid(&q,funcs,"Validation",10000,1);
        valid.validate();
        return 0;
    #endif

    #ifdef BENCH
        Benchmark<QueueSll_spin<int>,int> bench(&q,funcs,test->threads,test->name,test->time,test->actions,test->threadPinning,test->localwork_status,test->localwork_time);
        bench.full_bench(test->iterations);
    #endif
}