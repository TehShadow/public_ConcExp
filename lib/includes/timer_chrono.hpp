#pragma once

#include <iostream>
#include <chrono>

class Cpu_timer{
    private:
    /// @brief Starting time of the timer
        std::chrono::_V2::system_clock::time_point begin;
    /// @brief Ending point of the timer
        std::chrono::_V2::system_clock::time_point end;
    /// @brief Name of the Cpu_timerrr
        std::string name;
    /// @brief Status of the timer (True/False)
        volatile bool isRunning = false;
    public:
    /// @brief Cpu_timer Constructor
    /// @param parameter-name name of the timer
        Cpu_timer(std::string name){
            this->name = name;
            
        }
    /// @brief Cpu_timerr Constructor - Default name
        Cpu_timer(){
            this->name = "DEFAULT NAME";
        };
    /// @brief Deconstructor
        ~Cpu_timer(){};
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



void Cpu_timer::start(){
    this->begin  = std::chrono::high_resolution_clock::now();
    this->isRunning = true;
}

long long Cpu_timer::stop(){
    this->end  = std::chrono::high_resolution_clock::now();
    std::chrono::milliseconds time = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin);

    std::cout<<std::fixed<<"  CPU time finished with: "<<time.count() <<" ms"<<std::endl;
    this->isRunning = false;
    return time.count();
}

long long Cpu_timer::getCurrentTime(){
    this->end  = std::chrono::high_resolution_clock::now();
    std::chrono::milliseconds time = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin);
    return time.count();
}
