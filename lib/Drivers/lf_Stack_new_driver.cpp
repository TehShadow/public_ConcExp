#include "../includes/Benchmark.hpp"
#include "../../lib/DS/Stacks/lfStack_new/ifStack_new.hpp"
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

    If_Stack_new<int> stack;
    methods<int> funcs;

    funcs.add = std::bind(&If_Stack_new<int>::push,&stack,std::placeholders::_1);
    funcs.remove = std::bind(&If_Stack_new<int>::pop,&stack);
    funcs.size = std::bind(&If_Stack_new<int>::size,&stack);
    funcs.find = std::bind(&If_Stack_new<int>::find,&stack,std::placeholders::_1);
    funcs.empty = std::bind(&If_Stack_new<int>::empty,&stack);

    #ifdef VALIDATE
        Validator<If_Stack_new<int>,int> valid(&stack,funcs,"Validation",10000,1);
        valid.validate();
        return 0;
    #endif

    #ifdef BENCH
        Benchmark<If_Stack_new<int>,int> bench(&stack,funcs,test->threads,test->name,test->time,test->actions,test->threadPinning,test->localwork_status,test->localwork_time);
        bench.full_bench(test->iterations);
    #endif
}