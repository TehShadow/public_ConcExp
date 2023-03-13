#include "./queueMS.hpp"
#include "../../../includes/Benchmark.hpp"\





int main(){

    MsQueue<int> q;
    methods<int> funcs;

    funcs.add = std::bind(&MsQueue<int>::enqueue,&q,std::placeholders::_1);
    funcs.remove = std::bind(&MsQueue<int>::dequeue,&q);
    funcs.size = std::bind(&MsQueue<int>::size,&q);
    funcs.find = std::bind(&MsQueue<int>::find,&q,std::placeholders::_1);
    funcs.empty = std::bind(&MsQueue<int>::empty,&q);

    int SystemCores = std::thread::hardware_concurrency();
    Benchmark<MsQueue<int>,int> bench(&q,funcs,8,"Default_name",10000,1000000,true,true,10);

    // bench.__ADD__();
    // bench.__REMOVE__();
    // bench.__ADD_THEN_REMOVE__();
    // bench.__RANDOM_ADD_OR_DEQUEUE__();
    // bench.__ADD_THEN_REMOVE_TIME__();
    // bench.__RANDOM_ADD_OR_DEQUEUE_TIME__();

    bench.full_bench(2);

    // q.printQueue();
}