#include "../includes/Benchmark.hpp"
#include "../../lib/DS/SLLs/SLL_HOHL_spin/SLL_HOHL_spin.hpp"
#include "../includes/validator.hpp"


int main(int argc,char* argv[]){

    #ifdef BENCH
    args *test;
    test = getArgs(argc,argv);
    if(argc<9){
        show_help();
        return 1;
    }
        printArgs(test);

    #endif
        
    SLL_spin<int> sll;
    methods<int> funcs;

    funcs.add = std::bind(&SLL_spin<int>::insert,&sll,std::placeholders::_1);
    funcs.removeElement = std::bind(&SLL_spin<int>::remove,&sll,std::placeholders::_1);
    funcs.size = std::bind(&SLL_spin<int>::size,&sll);
    funcs.find = std::bind(&SLL_spin<int>::search,&sll,std::placeholders::_1);
    funcs.empty = std::bind(&SLL_spin<int>::empty,&sll);

    #ifdef VALIDATE
        Validator<SLL_spin<int>,int> valid(&sll,funcs,"Validation",10000,1);
        valid.validate();
        return 0;;
    #endif

    #ifdef BENCH
        Benchmark<SLL_spin<int>,int> bench(&sll,funcs,test->threads,test->name,test->time,test->actions,test->threadPinning,test->localwork_status,test->localwork_time);
        bench.full_bench(test->iterations);
    #endif
}