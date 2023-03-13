#include "../includes/Benchmark.hpp"
#include "../../lib/DS/Queues/Queue-Ms/queueMS.hpp"
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

    MsQueue <int> q;
    methods<int> funcs;

    funcs.add = std::bind(&MsQueue<int>::enqueue,&q,std::placeholders::_1);
    funcs.remove = std::bind(&MsQueue<int>::dequeue,&q);
    funcs.size = std::bind(&MsQueue<int>::size,&q);
    funcs.find = std::bind(&MsQueue<int>::find,&q,std::placeholders::_1);
    funcs.empty = std::bind(&MsQueue<int>::empty,&q);

    #ifdef VALIDATE
        Validator<MsQueue<int>,int> valid(&q,funcs,"Validation",10000,1);
        valid.validate();
        return 0;
    #endif

    #ifdef BENCH
        Benchmark<MsQueue<int>,int> bench(&q,funcs,test->threads,test->name,test->time,test->actions,test->threadPinning,test->localwork_status,test->localwork_time);
        bench.full_bench(test->iterations);
    #endif
}
