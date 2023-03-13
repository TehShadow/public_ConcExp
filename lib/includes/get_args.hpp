#pragma once
#include <iostream>

struct args{
    int threads = 1;
    long long actions = 10000;
    long long time = 100000;
    std::string name = "Default_name";
    bool threadPinning = false;
    bool localwork_status = false;
    int localwork_time = 512;
    int iterations = 1;
};

/// @brief print help
void show_help();

/// @brief gets args from the console
args* getArgs(int argv,char* argc[]);

/// @brief print the set arguments to the console
void printArgs(args* args);





void show_help(){
    std::cout<<"-i || --iterations [Number of iterations]\n-t || --thread [Number of thread]\n-p || --pinning [ThreadPinning 1:on , 0:off(default)]\n-a || --actions [number of actions]\n-T || --Time [time for testing]\n-o || --save [name for the result log]\n-ls || --localworkStatus [0 or 1 // on or off]\n-lt || --localworkTimer [int 0 - 2,147,483,647]\n"<<std::endl;

}

args* getArgs(int argv , char* argc[]){
    args *testArgs = (args*)malloc(sizeof(args));
        for (int i=1;i<argv;i++){
        std::string arg = argc[i];
        if((arg == "-i") || (arg == "--iterations")){
            if(argc[i+1]) testArgs->iterations = std::atoi(argc[i+1]);
        }
        if((arg == "-t") || (arg == "--thread")) {
            if(argc[i+1]) testArgs->threads = std::atoi(argc[i+1]);
            }
        if((arg == "-a") || (arg == "--action")){
            if(argc[i+1]) testArgs->actions = std::atoi(argc[i+1]);
        }
        if((arg == "-T") || (arg == "--Time")){
            if(argc[i+1]) testArgs->time = std::atol(argc[i+1]);
        } 
        if((arg == "-o") || (arg == "--save")){
            if(argc[i+1]) testArgs->name = argc[i+1];
        }
        if((arg == "-p") || (arg == "--pinning")){
            if(argc[i+1]) {
                int choice = std::atoi(argc[i+1]);
                if(choice) testArgs->threadPinning = true;
                else testArgs->threadPinning = false;

            }
        }
        if((arg == "-ls") || (arg == "--localworkStatus")){
            if(argc[i+1]){
                bool tempStat = std::atoi(argc[i+1]);
                if(tempStat) testArgs->localwork_status = true;
                else testArgs->localwork_status = false;
            }
        }
        if((arg == "-lt") || (arg == "--localworkTimer")){
            if(argc[i+1]) testArgs->localwork_time = std::atoi(argc[i+1]);
        }
    }

    return testArgs;
}


void printArgs(args* testArgs){
    std::cout<<"Threads : "<<testArgs->threads<<std::endl;
    std::cout<<"Actions : "<<testArgs->actions<<std::endl;
    std::cout<<"Time : "<<testArgs->time<<std::endl;
    std::cout<<"Name : "<<testArgs->name<<std::endl;
    std::cout<<"ThreadPinning : "<<testArgs->threadPinning<<std::endl;
    std::cout<<"Iterations : "<<testArgs->iterations<<std::endl;
    if(testArgs->localwork_status) std::cout<<"LocalWork : "<<testArgs->localwork_time<<std::endl;  
}