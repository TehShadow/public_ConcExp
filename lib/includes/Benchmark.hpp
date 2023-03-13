#pragma once 
#include "./timer_chrono.hpp"
#include <iostream>
#include <thread>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include "unistd.h"
#include <vector>
#include "random"
#include "mutex"
#include "./get_args.hpp"
#include "./helpers.hpp"

template <class X ,class  Y>
class Benchmark
{
    private:

        Cpu_timer cpu_timer = Cpu_timer();
        std::string saveLocation = "/Logs/Data/";
        std::vector<long long> timer_vector_storage;


        //actions
        volatile long long threadSum = 0;

        //Time
        volatile bool TimeFlag = false;
        volatile bool thread_ready = false;

    protected:
        int SystemCores = std::thread::hardware_concurrency();
        std::string Benchmark_name = "Default_name";
        test_types test_type;
        std::string get_current_dir();
        pthread_barrier_t barrier;
        std::thread *threadhandles;
        std::mutex guardian;

    //Bench options
        bool threadPinning = 0;
        int numThreads = 1;
        long long actions = 1000000;
        long long time = 10000;
        long long partitions = this->actions/this->numThreads;
        bool localwork_status;
        int localwork_time;
        X* ds;
        methods<Y> func;
        

    public:
        Benchmark(X* ds ,methods<Y> methods, 
                 int numThreads , 
                 std::string name ,
                 long long time,
                 long actions, 
                 bool threadPinning = true , 
                 bool localwork = false, 
                 int localwork_time = 10 ){

                this->ds = ds;
                this->func = methods;
                
                this->numThreads = numThreads;
                this->Benchmark_name = name;
                this->threadPinning = threadPinning;
                this->localwork_status = localwork;
                this->localwork_time = localwork_time;
                this->threadhandles = new std::thread [this->numThreads];
                this->actions = actions;
                this->time = time;
                this->partitions = this->actions/this->numThreads;

                pthread_barrier_init(&barrier,NULL,this->numThreads);

        }

        ~Benchmark(){};


    // Getters functions
    /// @brief Gets Number of threads available to the benchmark
    int getNumThreads() { return this->numThreads; }
    /// @brief Gets name of the benchmark
    std::string getName() { return this->Benchmark_name; }
    /// @brief Gets Save Location
    std::string getSaveLocation() {
        std::string current_dir = get_current_dir_name();
        std::string to_delete= "/generatedScripts";
        std::string pathname;
        std::string res;
        size_t pos = current_dir.find(to_delete);
        if(pos == std::string::npos){
            pathname = current_dir + this->saveLocation;
        }
        else{
            res = current_dir.erase(pos,to_delete.length());
            pathname = res + this->saveLocation;
        }
        return pathname;
        }
    // Print functions
    /// @brief print available core number
    void printSystemCores() { std::cout << this->SystemCores; }
    /// @brief print available threads
    void printAvailableThreads() { std::cout << this->numThreads; }
    /// @brief print name of the benchmark
    void printName() { std::cout << this->Benchmark_name; }
    // Timer functions
    /// @brief Start timer
    void start_timer() { cpu_timer.start(); }
    /// @brief Stop timer
    void stop_timer() { cpu_timer.stop(); }
    /// @brief Get current time without stoping the timer
    long long getCurrentTime() { return cpu_timer.getCurrentTime(); }
    /// @brief push current time to a vector
    void save_current_time() { timer_vector_storage.push_back(cpu_timer.getCurrentTime()); }
    /// @brief clear timer storage vector
    void clear_timer_storage() { timer_vector_storage.clear(); }
    /// @brief save vector to hard-disk at the save location
    /// @param parameter-actions number of total actions
    void save_logs(long long actions, long long time);
    /// @brief create a dir if it does not exist
    /// @param parameter-path path of the folder
    /// @param parameter-name name of the folder
    void create_dir(std::string path, std::string name);
    /// @brief extra random load (thread-work)
    void localwork();
    /// @brief starting the benchmark with full tests
    void full_bench(int iterations=1);
    /// @brief starting the bechmark with half tests
    void half_bench(int iterations=1);

    // BENCH_TESTS //

    ///@brief add x number of elements
    void __ADD__();
    ///@brief remove x number of elements
    void __REMOVE__();
    ///@brief add then remove same number of elements (const:actions)
    void __ADD_THEN_REMOVE__();
    ///@brief add or dequeue at random  (const:actions)
    void __RANDOM_ADD_OR_DEQUEUE__();
    ///@brief add then remove (const:time)
    void __ADD_THEN_REMOVE_TIME__();
    ///@brief add or dequeue at random (const:time)
    void __RANDOM_ADD_OR_DEQUEUE_TIME__();


    /// Thread works
    /// @brief threadFunctions (workers)
    /// @param id thread_id
    void thread_add(int id);
    void thread_remove(int id);
    void thread_add_then_remove(int id);
    void thread_random_add_or_dequeue(int id);
    // Time tests
    void thread_add_then_remove_time(int id);
    void thread_random_add_or_dequeue_time(int id);

};

//Time Tests
template<class X , class Y>
void Benchmark<X,Y>::__ADD_THEN_REMOVE_TIME__(){
    this->test_type = T_ADD_REMOVE;
    cpu_set_t cpuset;
    std::cout<<"Test ADD_THEN_REMOVE_TIME: "<<std::endl;



    for(int i=0;i<this->getNumThreads();i++){

    if(this->threadPinning){
        CPU_ZERO(&cpuset);
        CPU_SET(i,&cpuset);
        }

    this->threadhandles[i] = std::thread(&Benchmark<X,Y>::thread_add_then_remove_time,this,i);
    

    if(this->threadPinning){
        int rc = pthread_setaffinity_np(this->threadhandles[i].native_handle(),sizeof(cpu_set_t),&cpuset);
        if(rc != 0 ){
            std::cerr<<"Error calling pthread setaffinity np:"<<rc<<"\n";
            }
        }
    }

        while(!this->thread_ready){};
        std::this_thread::sleep_for(std::chrono::milliseconds(this->time));
        TimeFlag = true;


    for(int i=0;i<this->getNumThreads();i++){
        threadhandles[i].join();
    }

    TimeFlag = false;
    std::cout<<"Total actions "<<this->threadSum<<"\n"<<std::endl;
    this->save_logs(this->threadSum,this->time);
    this->threadSum = 0;
}

template<class X,class Y>
void Benchmark<X,Y>::thread_add_then_remove_time(int id){
    {
        std::lock_guard<std::mutex> Guardlock(guardian);
        std::cout << "  Thread #" << id<< ": on CPU " << sched_getcpu() << "\n";
    }

    pthread_barrier_wait(&this->barrier);
    if(id == 0) thread_ready = true;
    int count = 0;
    while(!TimeFlag){
        this->func.add(count+1);
        this->func.remove();
        if(this->localwork_status) this->localwork();
        count++;
    }
    {
        std::lock_guard<std::mutex> Guardlock(guardian);
        std::cout << "  Thread #" << id<< " finished with "<<count<<" actions"<<std::endl;
        this->threadSum += count;
    }
}

template<class X, class Y>
void Benchmark<X,Y>::__RANDOM_ADD_OR_DEQUEUE_TIME__(){
    this->test_type = T_R_ADD_REMOVE;
    cpu_set_t cpuset;
    std::cout<<"Test RANDOM_ADD_OR_DEQUEUE_TIME"<<std::endl;



    for(int i=0;i<this->getNumThreads();i++){

    if(this->threadPinning){
        CPU_ZERO(&cpuset);
        CPU_SET(i,&cpuset);
        }

    this->threadhandles[i] = std::thread(&Benchmark<X,Y>::thread_random_add_or_dequeue_time,this,i);
    

    if(this->threadPinning){
        int rc = pthread_setaffinity_np(this->threadhandles[i].native_handle(),sizeof(cpu_set_t),&cpuset);
        if(rc != 0 ){
            std::cerr<<"Error calling pthread setaffinity np:"<<rc<<"\n";
            }
        }
    }

        while(!this->thread_ready){};
        std::this_thread::sleep_for(std::chrono::milliseconds(this->time));
        TimeFlag = true;


    for(int i=0;i<this->getNumThreads();i++){
        threadhandles[i].join();
    }

    TimeFlag = false;
    std::cout<<"Total actions "<<this->threadSum<<"\n"<<std::endl;
    this->save_logs(this->threadSum,this->time);
    this->threadSum = 0;
}

template<class X,class Y>
void Benchmark<X,Y>::thread_random_add_or_dequeue_time(int id){
    srand(10);
    int random = rand();
    int choice = 0;
    {
        std::lock_guard<std::mutex> Guardlock(guardian);
        std::cout << "  Thread #" << id<< ": on CPU " << sched_getcpu() << "\n";
    }

    pthread_barrier_wait(&this->barrier);
    if(id == 0) thread_ready = true;
    int count = 0;
    while(!TimeFlag){
        choice = rand()%2;
        if(choice == 0){
            this->func.add(count+1);
            if(this->localwork_status) this->localwork();
        }
        else{
            this->func.remove();
            if(this->localwork_status) this->localwork();
        }
        count++;
    }
    {
        std::lock_guard<std::mutex> Guardlock(guardian);
        std::cout << "  Thread #" << id<< " finished with "<<count<<" actions"<<std::endl;
        this->threadSum += count;
    }
}

// Actions Tests
template<class X , class Y>
void Benchmark<X,Y>::__RANDOM_ADD_OR_DEQUEUE__(){
    this->test_type = R_ADD_REMOVE;
    cpu_set_t cpuset;
    std::cout<<"Test RANDOM_ADD_REMOVE: "<<std::endl;



    for(int i=0;i<this->getNumThreads();i++){

    if(this->threadPinning){
        CPU_ZERO(&cpuset);
        CPU_SET(i,&cpuset);
    }

    this->threadhandles[i] = std::thread(&Benchmark<X,Y>::thread_random_add_or_dequeue,this,i);

    if(this->threadPinning){
        int rc = pthread_setaffinity_np(this->threadhandles[i].native_handle(),sizeof(cpu_set_t),&cpuset);
        if(rc != 0 ){
            std::cerr<<"Error calling pthread setaffinity np:"<<rc<<"\n";
            }
        }
    }




    for(int i=0;i<this->getNumThreads();i++){
        threadhandles[i].join();
    }
}

template<class X, class Y>
void Benchmark<X,Y>::thread_random_add_or_dequeue(int id){
    srand(10);
    int random = rand();
    int choice = 0;
    {
        std::lock_guard<std::mutex> Guardlock(guardian);
        std::cout << "  Thread #" << id << ": on CPU " << sched_getcpu() << "\n";
    }
    pthread_barrier_wait(&this->barrier);
    if(id == 0) {
        this->start_timer();
    }
    for(long i=this->partitions*(id);i<this->partitions*(id+1);i++){
        choice = rand()%2;

        if(choice == 0){
            this->func.add(i);
        }
        else{
            this->func.remove();
        }
        if(this->localwork_status) this->localwork();
        
    }
    if(id == 0){
        long long time = this->getCurrentTime();
        this->stop_timer();
        this->save_logs(this->actions,time);
    }
}

template<class X, class Y>
void Benchmark<X,Y>::__ADD__(){
    this->test_type = ADD;
    cpu_set_t cpuset;
    std::cout<<"Test ADD: "<<std::endl;



    for(int i=0;i<this->getNumThreads();i++){

    if(this->threadPinning){
        CPU_ZERO(&cpuset);
        CPU_SET(i,&cpuset);
        }

    this->threadhandles[i] = std::thread(&Benchmark<X,Y>::thread_add,this,i);

    if(this->threadPinning){
        int rc = pthread_setaffinity_np(this->threadhandles[i].native_handle(),sizeof(cpu_set_t),&cpuset);
        if(rc != 0 ){
            std::cerr<<"Error calling pthread setaffinity np:"<<rc<<"\n";
            }
        }
    }




    for(int i=0;i<this->getNumThreads();i++){
        threadhandles[i].join();
    }
}

template<class X,class Y>
void Benchmark<X,Y>::thread_add(int id){
    {
        std::lock_guard<std::mutex> Guardlock(guardian);
        std::cout << "  Thread #" << id << ": on CPU " << sched_getcpu() << "\n";
    }
    pthread_barrier_wait(&this->barrier);
    if(id == 0) {
        this->start_timer();
    }
    for(long i=this->partitions*(id);i<this->partitions*(id+1);i++){
        this->func.add(i+1);
        if(this->localwork_status) this->localwork();
    }
    if(id == 0){
        long long time = this->getCurrentTime();
        this->stop_timer();
        this->save_logs(this->actions,time);
    }
}

template <class X, class Y>
void Benchmark<X,Y>::__REMOVE__(){
    this->test_type = REMOVE;
    cpu_set_t cpuset;
    std::cout<<"Test REMOVE: "<<std::endl;



    for(int i=0;i<this->getNumThreads();i++){

    if(this->threadPinning){
        CPU_ZERO(&cpuset);
        CPU_SET(i,&cpuset);
        }

    this->threadhandles[i] = std::thread(&Benchmark<X,Y>::thread_remove,this,i);

    if(this->threadPinning){
        int rc = pthread_setaffinity_np(this->threadhandles[i].native_handle(),sizeof(cpu_set_t),&cpuset);
        if(rc != 0 ){
            std::cerr<<"Error calling pthread setaffinity np:"<<rc<<"\n";
            }
        }
    }




    for(int i=0;i<this->getNumThreads();i++){
        threadhandles[i].join();
    }
}

template<class X,class Y>
void Benchmark<X,Y>::thread_remove(int id){
    {
        std::lock_guard<std::mutex> Guardlock(guardian);
        std::cout << "  Thread #" << id << ": on CPU " << sched_getcpu() << "\n";
    }
    pthread_barrier_wait(&this->barrier);
    if(id == 0) {
        this->start_timer();
    }
    for(long i=this->partitions*(id);i<this->partitions*(id+1);i++){
        this->func.remove();
        if(this->localwork_status) this->localwork();
    }
    if(id == 0){
        long long time = this->getCurrentTime();
        this->stop_timer();
        this->save_logs(this->actions,time);
    }
}

template<class X, class Y>
void Benchmark<X,Y>::__ADD_THEN_REMOVE__(){
    this->test_type = ADD_REMOVE;
    cpu_set_t cpuset;
    std::cout<<"Test ADD_REMOVE: "<<std::endl;



    for(int i=0;i<this->getNumThreads();i++){

    if(this->threadPinning){
        CPU_ZERO(&cpuset);
        CPU_SET(i,&cpuset);
        }

    this->threadhandles[i] = std::thread(&Benchmark<X,Y>::thread_add_then_remove,this,i);

    if(this->threadPinning){
        int rc = pthread_setaffinity_np(this->threadhandles[i].native_handle(),sizeof(cpu_set_t),&cpuset);
        if(rc != 0 ){
            std::cerr<<"Error calling pthread setaffinity np:"<<rc<<"\n";
            }
        }
    }




    for(int i=0;i<this->getNumThreads();i++){
        threadhandles[i].join();
    }
}

template<class X,class Y>
void Benchmark<X,Y>::thread_add_then_remove(int id){
    {
        std::lock_guard<std::mutex> Guardlock(guardian);
        std::cout << "  Thread #" << id << ": on CPU " << sched_getcpu() << "\n";
    }
    pthread_barrier_wait(&this->barrier);
    if(id == 0) {
        this->start_timer();
    }

    for(long i=this->partitions*(id);i<this->partitions*(id+1);i++){
        this->func.add(i);
        if(this->localwork_status) this->localwork();
    }

    for(long i=this->partitions*(id);i<this->partitions*(id+1);i++){
        this->func.remove();
        if(this->localwork_status) this->localwork();
    }

    if(id == 0){
        long long time = this->getCurrentTime();
        this->stop_timer();
        this->save_logs(this->actions,time);
    }
}

template <class X , class Y>
void Benchmark<X,Y>::full_bench(int iteration){
    std::cout<<"Running full tests"<<std::endl;
    for(int i=0;i<iteration;i++){
        std::cout<<"[ Iteration : "<<i+1<<"/"<<iteration<<" ]"<<std::endl;
        this->__ADD__();
        this->__REMOVE__();
        this->__ADD_THEN_REMOVE__();
        this->__RANDOM_ADD_OR_DEQUEUE__();
        this->__ADD_THEN_REMOVE_TIME__();
        this->__RANDOM_ADD_OR_DEQUEUE_TIME__();
    }
}

template <class X , class Y>
void Benchmark<X,Y>::half_bench(int iterations){
    std::cout<<"Running half tests"<<std::endl;
    for(int i=0;i<iterations;i++){
        std::cout<<"[ Iteration : "<<i+1<<"/"<<iterations<<" ]"<<std::endl;
        this->__ADD_THEN_REMOVE__();
        this->__RANDOM_ADD_OR_DEQUEUE__();
        this->__RANDOM_ADD_OR_DEQUEUE_TIME__();
    }
}

template <class X ,class  Y>
std::string Benchmark<X,Y>::get_current_dir(){
    char buff[FILENAME_MAX];
    getcwd(buff,FILENAME_MAX);
    std::string current_working_dir(buff);
    return current_working_dir;
}

template <class X ,class  Y>
void Benchmark<X ,Y>::save_logs(long long actions, long long time)
{   
    this->create_dir(this->getSaveLocation(), this->getName());
    std::string location;

    if (test_type == ADD)
    {
        location = this->getSaveLocation() + this->getName() + "/ADD" + ".txt";
    }
    else if (test_type == REMOVE)
    {
        location = this->getSaveLocation() + this->getName() + "/REMOVE" + ".txt";
    }
    else if (test_type == ADD_REMOVE)
    {
        location = this->getSaveLocation() + this->getName() + "/ADD_REMOVE" + ".txt";
    }
    else if (test_type == R_ADD_REMOVE)
    {
        location = this->getSaveLocation() + this->getName() + "/R_ADD_REMOVE" + ".txt";
    }

    else if (test_type == T_ADD_REMOVE)
    {
        location = this->getSaveLocation() + this->getName() + "/T_ADD_REMOVE" + ".txt";
    }

    else if (test_type == T_R_ADD_REMOVE)
    {
        location = this->getSaveLocation() + this->getName() + "/T_R_ADD_REMOVE" + ".txt";
    }

    std::ofstream file(location, std::ios_base::app | std::ios_base::in);
    if (file.is_open())
        file << this->getNumThreads() << " : " << time << " : " << actions << " : " << this->threadPinning << " : "  << this->localwork_time << " : " << this->localwork_status << "\n";
    else
    {
        std::cout << "Unable to open file"<<std::endl;
        return;
    }
}

template <class X ,class  Y>
void Benchmark<X,Y>::create_dir(std::string path, std::string name)
{

    std::string pathlocation = path + name;


    struct stat info;
    if (stat(pathlocation.c_str(), &info) == 0){
        return;
    }
    else if (mkdir(pathlocation.c_str(), 0777) == -1){
        std::cout << "Error creating a directory" << std::endl;
    }
  
}

template <class X ,class  Y>
void Benchmark<X,Y>::localwork()
{
    srand(10);
    int num = rand() % (this->localwork_time + 1) ;
    volatile int counter = 0;
    for (int i = 0; i < num; i++)
        counter + i;
}