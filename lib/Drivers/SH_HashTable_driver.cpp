#include "../includes/Benchmark.hpp"
#include "../DS/HashTables/SH_HashTables/SH_HashTable.hpp"
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

    SH_Hashtable q;
    methods<int> funcs;
    
    funcs.add = std::bind(&SH_Hashtable::insert,&q,std::placeholders::_1);
    funcs.removeElement = std::bind(&SH_Hashtable::remove,&q,std::placeholders::_1);
    funcs.size = std::bind(&SH_Hashtable::count,&q);
    funcs.find = std::bind(&SH_Hashtable::find,&q,std::placeholders::_1);
    funcs.empty = std::bind(&SH_Hashtable::empty,&q);
    
    #ifdef VALIDATE
        Validator<SH_Hashtable,int> valid(&q,funcs,"Validation",10000,1);
        valid.validate();
        return 0;
    #endif
    #ifdef BENCH
        Benchmark<SH_Hashtable,int> bench(&q,funcs,test->threads,test->name,test->time,test->actions,test->threadPinning,test->localwork_status,test->localwork_time);
        bench.full_bench(test->iterations);
    #endif
}
