#pragma once

#include <vector>
#include "./timer_chrono.hpp"
#include "./helpers.hpp"
#include <iostream>
#include <string.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <thread>
#include <mutex>
#include <ctime>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>



inline void sighandler(int signo){
        switch (signo) {
        case SIGABRT: case SIGTERM:
            /* Do something */
            std::cout<<"Segfault caught_1"<<std::endl;
        case SIGILL:
            /* Do something */
            std::cout<<"Segfault caught_2"<<std::endl;
            break;
        case SIGSEGV:
            /* Do something */
            std::cout<<"Segfault caught_3"<<std::endl;
            break;
        case SIGURG:
            /* Do something */
            std::cout<<"Segfault caught_4"<<std::endl;
            break;
        case SIGUSR1: case SIGUSR2:
            /* Do something */
            std::cout<<"Segfault caught_5"<<std::endl;
            break;
        default:
            std::cout<<"default"<<std::endl;
            break;
    }
}

template <class X ,class  Y>
class Validator{


    private:
        std::string LogsPath = "/Logs/validationLogs";
        bool threadPinning;

        struct sigaction act;
        
        
        //Actions
        long long actions = 0;
        long long partitions = 0;
        long long totalElements = 0;

        std::string name = "Default";



    protected:
        
        std::mutex lock;
        X* ds;
        methods<Y> func;
        std::thread *threadhandles;

        void create_log_file(std::string,std::string);
        void create_dir(std::string, std::string);


    public:

        int threadNum = 1;
        int maxThreads = std::thread::hardware_concurrency();
        pthread_barrier_t barrier;

        //Globals

        int currThread = 2;
        int globalBuff = 0;

        Validator(X* ds , methods<Y> methods,
                std::string name,int actions , 
                bool threadPinning = true
        ){

            this->func = methods;
            this->threadPinning = threadPinning;
            this->name = name;
            this->actions = actions;
            pthread_barrier_init(&barrier,NULL,1);
            this->threadhandles = new std::thread [this->maxThreads];
            act.sa_handler = sighandler;
            sigemptyset(&act.sa_mask);
            act.sa_flags = 0;
            sigaction(SIGINT,&act,0);

        };

        ~Validator(){};


        std::string getLogsPath(){return this->LogsPath;}
        int getNumThreads(){return this->currThread;}
        bool getThreadPinning(){return this->threadPinning;}

        void WriteToLog(std::string);

        /// @brief validates the datastructure
        bool validate();

        bool Test_1();
        bool Test_2();
        bool Test_3();
        bool Test_4();
        bool Test_5();


        private:
        void thread_add(int thread_num);
        void thread_add_remove(int thread_num);
        void thread_remove(int thread_num);
        void thread_add_one_remove_one(int thread_num);
        void thread_add_count(int thread_num);
        void thread_add_remove_count(int thread_num);


};




template <class X , class Y>
void Validator<X,Y>::thread_add_remove_count(int thread_num){

    int localbuf = 0;
    Y data;

    pthread_barrier_wait(&this->barrier);
    for(int i=this->partitions*(thread_num);i<this->partitions*(thread_num+1);i++){
        this->func.add(i+1);
        localbuf +=(i+1);
    }
    for(int i=this->partitions*(thread_num);i<this->partitions*(thread_num+1);i++){
        data = this->func.remove();
        localbuf = localbuf - data;
    }

    {
        std::lock_guard<std::mutex> Guardlock(lock);
        this->globalBuff += localbuf;
    }

}

template <class X , class Y>
bool Validator<X,Y>::Test_5(){

    int sentinel=0;
    this->globalBuff=0;

    this->func.empty();

    if(this->func.size() == 1){ sentinel=1; }
    else { sentinel=0; }

    cpu_set_t cpuset;
    std::cout<<"Test 5"<<std::endl;
    for(int i=0;i<this->currThread;i++){

    if(this->threadPinning){
        CPU_ZERO(&cpuset);
        CPU_SET(i,&cpuset);
        }

    this->threadhandles[i] = std::thread(&Validator<X,Y>::thread_add_remove_count,this,i);

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
    

    long long realCounter = 0;
    while(this->func.size() > sentinel){
        realCounter += this->func.remove();
    }

    if(globalBuff == realCounter) return true;
    else{
        // write problem to buffer and then to log file
        std::cout<<"1)Real Total "<<realCounter<<"\n"<<"2)In Theory "<<globalBuff<<std::endl;
        return false;
    }
}

template <class X , class Y>
void Validator<X,Y>::thread_add_count(int thread_num){

    int localbuf = 0;
    pthread_barrier_wait(&this->barrier);
    for(int i=this->partitions*(thread_num);i<this->partitions*(thread_num+1);i++){
        this->func.add(i+1);
        localbuf +=(i+1);
    }

    {
        std::lock_guard<std::mutex> Guardlock(lock); 
        this->globalBuff += localbuf;
    }

}

/// @brief adds x elements count them in a local buffer then add to globalbuffer then compare with the actual ds
template <class X , class Y>
bool Validator<X,Y>::Test_4(){

    int sentinel=0;
    this->globalBuff=0;

    this->func.empty();
    
    if(this->func.size() == 1){ sentinel=1; }
    else { sentinel=0; }

    cpu_set_t cpuset;

    std::cout<<"Test 4"<<std::endl;
    for(int i=0;i<this->currThread;i++){

    if(this->threadPinning){
        CPU_ZERO(&cpuset);
        CPU_SET(i,&cpuset);
        }

    this->threadhandles[i] = std::thread(&Validator<X,Y>::thread_add_count,this,i);

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

    long long realCounter = 0;
    while(this->func.size() > sentinel){
        realCounter += this->func.remove();
    }

    if(globalBuff == realCounter) return true;
    else{
        // write problem to buffer and then to log file
        std::cout<<"1)Real Total "<<realCounter<<"\n"<<"2)In Theory "<<globalBuff<<std::endl;
        return false;
    }
}

template <class X , class Y>
void Validator<X,Y>::thread_add_one_remove_one(int thread_num){

    pthread_barrier_wait(&this->barrier);
    for(long i=this->partitions*(thread_num);i<this->partitions*(thread_num+1);i++){
        this->func.add(i+1);
        this->func.remove();
    }
};

template <class X , class Y>
bool Validator<X,Y>::Test_3(){

    this->func.empty();
     int prevActions = this->func.size();
    cpu_set_t cpuset;
    std::cout<<"Test 3"<<std::endl;
    for(int i=0;i<this->currThread;i++){

    if(this->threadPinning){
        CPU_ZERO(&cpuset);
        CPU_SET(i,&cpuset);
        }

    this->threadhandles[i] = std::thread(&Validator<X,Y>::thread_add_one_remove_one,this,i);

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


    int currAction = this->func.size();

    if(currAction == prevActions) return true;
    else{
        // write problem to buffer and then to log file
        std::cout<<"1)Real Total "<<currAction<<"\n"<<"2)In Theory "<<prevActions<<std::endl;
        return false;
    }
}


template <class X , class Y>
void Validator<X,Y>::thread_add_remove(int thread_num){
    pthread_barrier_wait(&this->barrier);
    for(long i=this->partitions*(thread_num);i<this->partitions*(thread_num+1);i++){
        this->func.add(i+1);
        
    }

    for(long i=this->partitions*(thread_num);i<this->partitions*(thread_num+1);i++){
        this->func.remove();
        
    }
};

/// @brief add x elements then remove x elements if prevSize == currSize true
template <class X , class Y>
bool Validator<X,Y>::Test_2(){

    this->func.empty();
    int prevActions = this->func.size();
    cpu_set_t cpuset;
    std::cout<<"Test 2"<<std::endl;



    for(int i=0;i<this->currThread;i++){

    if(this->threadPinning){
        CPU_ZERO(&cpuset);
        CPU_SET(i,&cpuset);
        }

    this->threadhandles[i] = std::thread(&Validator<X,Y>::thread_add_remove,this,i);

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


    int currActions = this->func.size();

    if(currActions == prevActions) return true;
    else{
        // write problem to buffer and then to log file
        std::cout<<"1)Real Total "<<currActions<<"\n"<<"2)In Theory "<<prevActions<<std::endl;
        return false;
    }
}


template<class X,class Y>
bool Validator<X,Y>::validate(){
    bool status = true;
    int count = 0;
    int testNum = 5;   
    int totalTests=0;
    for(int i=2;i<=this->maxThreads;i+=2){

        this->currThread = i;
        this->partitions = actions/i;
        this->threadNum = i;
        pthread_barrier_init(&barrier,NULL,this->currThread);


        std::cout<<"----------------- "<<"Num of Threads "<<i<<" -----------------"<<std::endl;
        status = this->Test_1();
        if(!status) std::cout<<"Test_1 failed\n"<<"See log file"<<std::endl;
        else count++;
        totalTests++;

        status = this->Test_2();
        if(!status) std::cout<<"Test_2 failed\n"<<"See log file"<<std::endl;
        else count++;
        totalTests++;

        status = this->Test_3();
        if(!status) std::cout<<"Test_3 failed\n"<<"See log file"<<std::endl;
        else count++;
        totalTests++;

        status = this->Test_4();
        if(!status) std::cout<<"Test_4 failed\n"<<"See log file"<<std::endl;
        else count++;
        totalTests++;

        status = this->Test_5();
        if(!status) std::cout<<"Test_5 failed\n"<<"See log file"<<std::endl;
        else count++;
        totalTests++;

    }
        if(count == totalTests){
            std::cout<<"Passed all tests!"<<std::endl;
            return true;
        }
        else{
            std::cout<<"Passed "<<count<<" out of "<<testNum*this->maxThreads/2<<" tests"<<std::endl;
            return false;
        }
};


template <class X , class Y>
void Validator<X,Y>::thread_add(int thread_num){
    pthread_barrier_wait(&this->barrier);
    for(long i=this->partitions*(thread_num);i<this->partitions*(thread_num+1);i++){
        this->func.add(i+1);
    }
};

/// @brief add x elements if size == logicalSize true
template <class X , class Y>
bool Validator<X,Y>::Test_1(){

    this->func.empty();
    int currSize = this->func.size();
    cpu_set_t cpuset;
    std::cout<<"Test 1"<<std::endl;



    for(int i=0;i<this->currThread;i++){

    if(this->threadPinning){
        CPU_ZERO(&cpuset);
        CPU_SET(i,&cpuset);
        }

    this->threadhandles[i] = std::thread(&Validator<X,Y>::thread_add,this,i);

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

    this->totalElements = this->func.size();
    long long goalElements = actions+currSize-(actions%this->currThread);



    if(goalElements == this->totalElements) return true;
    else{
        // write problem to buffer and then to log file
        std::cout<<"1)Real Total "<<this->totalElements<<"\n"<<"2)In Theory "<<goalElements<<std::endl;
        return false;
    }
}


template<class X,class Y>
void Validator<X,Y>::create_dir(std::string path, std::string name)
{

    std::string pathlocation = name;


    struct stat info;
    if (stat(pathlocation.c_str(), &info) == 0){
        return;
    }
    else if (mkdir(pathlocation.c_str(), 0777) == -1){
        std::cout << "Error creating a directory" << std::endl;
    }
}


template<class X,class Y>
void Validator<X,Y>::create_log_file(std::string name ,std::string errors){

    std::string location = "/" + name;
    time_t now = time(0);
    char* dt = ctime(&now);

    std::ofstream file(location,std::ios_base::app | std::ios_base::in);
    {
        
        if(file.is_open()) file <<"\n["<<dt<<"]      "<<errors<<"\n";
        else{
            std::cout<<"Unable to open file"<<std::endl;
            return;
        }
    }
};