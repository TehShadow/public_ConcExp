#include "../includes/FCBenchmark.hpp"
#include "../DS/FC/fcStack/fcStack.hpp"
#include "../includes/validator.hpp"
#include "../includes/helpers.hpp"
#include <vector>



int main(int argc,char* argv[]){
    
        args *test;
        test = getArgs(argc,argv);
        if(argc<9){
            show_help();
            // return 1;
        }
        printArgs(test);

    


    FcStack stack(test->threads);
    StateDSmethods<int,FcStack::FcThreadState> funcs;
    std::vector<FcStack::FcThreadState> state_vect;

    for(int i=0;i<test->threads;i++){
        FcStack::FcThreadState st_thread(i,stack.S);
        state_vect.push_back(st_thread);
    }

    // funcs.add = std::bind(&FcStack::FcStackPush,&stack,std::placeholders::_2);
    // funcs.remove = std::bind(&FcStack::FcStackPop,&stack,std::placeholders::_1);
    

    Benchmark<FcStack,int,FcStack::FcThreadState> bench(&stack,state_vect,test->threads,test->name,test->time,test->actions,test->threadPinning,test->localwork_status,test->localwork_time);

    

    bench.full_bench(test->iterations);
}
