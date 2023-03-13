#include "../includes/Benchmark.hpp"
#include "../../lib/DS/Stacks/StackSLL/stackSll.hpp"
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

    StackSll <int> stack;
    methods<int> funcs;

    funcs.add = std::bind(&StackSll<int>::push,&stack,std::placeholders::_1);
    funcs.remove = std::bind(&StackSll<int>::pop,&stack);
    funcs.size = std::bind(&StackSll<int>::size,&stack);
    funcs.find = std::bind(&StackSll<int>::find,&stack,std::placeholders::_1);
    funcs.empty = std::bind(&StackSll<int>::empty,&stack);
    #ifdef VALIDATE
        Validator<StackSll<int>,int> valid(&stack,funcs,"Validation",10000,1);
        valid.validate();
        return 0;
    #endif

    #ifdef BENCH
        Benchmark<StackSll<int>,int> bench(&stack,funcs,test->threads,test->name,test->time,test->actions,test->threadPinning,test->localwork_status,test->localwork_time);
        bench.full_bench(test->iterations);
    #endif
}
