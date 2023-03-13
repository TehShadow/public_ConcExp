#pragma once

#include <iostream>
#include <ctime>

class Cpu_time{
    private:
    /// @brief Starting time of the timer
        std::clock_t begin;
    /// @brief Ending point of the timer
        std::clock_t end;
    /// @brief Name of the Cpu_timer
        std::string name;
    /// @brief Status of the timer (True/False)
        volatile bool isRunning = false;
    public:
    /// @brief Cpu_time Constructor
    /// @param parameter-name name of the timer
        Cpu_time(std::string name){
            this->name = name;
            
        }
    /// @brief Cpu_timer Constructor - Default name
        Cpu_time(){
            this->name = "DEFAULT NAME";
        };
    /// @brief Deconstructor
        ~Cpu_time(){};
    /// @brief gets the current status of the timer
        bool getStatus(){return this->isRunning;}
    /// @brief sets the name of the timer
        void setName(char* name){this->name = name;}
    /// @brief gets the name of the timer
        std::string getName(){return this->name = name;}
    
    /// @brief Start timer
        void start();
    /// @brief Stop timer
        long long stop();
    /// @brief get current time of the timer without stoping it
        long long getCurrentTime();

};



void Cpu_time::start(){
    this->begin  = std::clock();
    this->isRunning = true;
}

long long Cpu_time::stop(){
    this->end = std::clock();
    auto time = 1000 * (this->end - this->begin);
    std::cout<<std::fixed<<"  CPU time finished with: "<<time <<" ms"<<std::endl;
    this->isRunning = false;
    return time ;
}

long long Cpu_time::getCurrentTime(){
    this->end = std::clock();
    auto time = (this->end - this->begin);
    return time;
}
