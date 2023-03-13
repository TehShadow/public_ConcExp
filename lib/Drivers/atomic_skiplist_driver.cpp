#include "../includes/Benchmark.hpp"
#include "../../lib/DS/SkipList/atomic_skipList.hpp"
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
    SkipList List;
    methods<int> funcs;

    funcs.add = std::bind(&SkipList::Insert,&List,std::placeholders::_1);
    funcs.removeElement = std::bind(&SkipList::Delete,&List,std::placeholders::_1);
    funcs.find = std::bind(&SkipList::Search,&List,std::placeholders::_1);

    #ifdef VALIDATE
        Validator<SkipList,int> valid(&List,funcs,"Validation",10000,1);
        valid.validate();
        return 0;
    #endif

    #ifdef BENCH
        Benchmark<SkipList,int> bench(&List,funcs,test->threads,test->name,test->time,test->actions,test->threadPinning,test->localwork_status,test->localwork_time);
        
        bench.full_bench(test->iterations);
        
    #endif
}