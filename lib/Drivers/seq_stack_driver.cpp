#include "../includes/PsimBenchmark.hpp"
#include "../../lib/DS/Stacks/seq_stack/SLL.hpp"
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

    SLL stack;
    func_array<SLL,int> funcs;

    #ifdef VALIDATE
        Validator<lfStack<int>,int> valid(&stack,funcs,"Validation",10000,1);
        valid.validate();
        return 0;
    #endif

    #ifdef BENCH
        Benchmark<SLL,int> bench(&stack,funcs,test->threads,test->name,test->time,test->actions,test->threadPinning,test->localwork_status,test->localwork_time);
        bench.full_bench(test->iterations);
    #endif
}