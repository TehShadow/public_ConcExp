#include "../includes/Benchmark.hpp"
#include "../../lib/DS/Stacks/atomic_stack/atomic_stack.hpp"
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
    AtomicStack stack;
    methods<int> funcs;

    funcs.add = std::bind(&AtomicStack::push,&stack,std::placeholders::_1);
    funcs.remove = std::bind(&AtomicStack::pop,&stack);

    #ifdef VALIDATE
        Validator<AtomicStack,int> valid(&stack,funcs,"Validation",10000,1);
        valid.validate();
        return 0;
    #endif

    #ifdef BENCH
        Benchmark<AtomicStack,int> bench(&stack,funcs,test->threads,test->name,test->time,test->actions,test->threadPinning,test->localwork_status,test->localwork_time);
        
        bench.full_bench(test->iterations);
        
    #endif
}