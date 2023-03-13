#include "../includes/Benchmark.hpp"
#include "../../lib/DS/Queues/MPMCQueue.hpp"
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

    MPMCQueue <int> q(10000000);
    methods<int> funcs;

    funcs.add = std::bind(&MPMCQueue<int>::push,&q,std::placeholders::_1);
    funcs.remove = std::bind(&MPMCQueue<int>::pop,&q);

    #ifdef VALIDATE
        Validator<MPMCQueue<int>,int> valid(&q,funcs,"Validation",10000,1);
        valid.validate();
        return 0;
    #endif

    #ifdef BENCH
        Benchmark<MPMCQueue<int>,int> bench(&q,funcs,test->threads,test->name,test->time,test->actions,test->threadPinning,test->localwork_status,test->localwork_time);
        bench.full_bench(test->iterations);
    #endif
}
