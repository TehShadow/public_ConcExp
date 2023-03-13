#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <vector>
#include <filesystem>
#include <dirent.h>
#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include <fstream>

class ui{

    std::string RED="\033[0;31m";
    std::string GREEN="\033[1;32m";
    std::string ENDCOLOR="\33[0;37m";

    std::vector<int> queue;
    std::vector<std::string> bins;
    std::vector<std::string> fmake;
    std::string DSbinpath = "./Build/Bin";
    std::string makepath = "./Build/Makefiles/";
    std::string erase = "makefile.";
    std::string config_path = "./config.conf";
    std::string save_script = "/generatedScripts/";
    std::string temp_script = "/lib/template.sh";
    std::string makepath_script = "/Build/Makefiles/";
    std::string makeTemplatePath = "/Build/Makefiles/makefile.template";
    char path[256];
    std::string stringPath;
    



    private:
        std::string eraseSubStr(std::string mainStr,std::string toErase);
        std::vector<int> formatString(std::string &mainStr);
        void print_config();
        void edit_config();
        void edit_helper(int command);
        int getMode();
        int getIter();
        void generate_script();
        void generate_script_helper(std::vector<std::string>);
        std::string ds_menu();
        void run_test(std::string &command);
        int getConfigLineLength();

    public:
        
        ui(){
            getcwd(this->path,256);
            this->stringPath = path;
        }

        void getDS();
        void getfmake();
        void printMenu();
        void formatVector();
        void choise();
        void printLogo();
        void main_menu();

};

int ui::getConfigLineLength(){
    std::string line;
    int line_num = 1;
    std::ifstream filein(config_path);


    if(!filein){
        std::cout<<"Error opening config file..."<<std::endl;
        return 0;
    }
    
    while (std::getline(filein,line)) line_num++;
    return line_num;
}



void ui::generate_script(){
    std::string choise = ds_menu();
    std::vector<int> res = formatString(choise);
    std::string command;
    int count = 1;
    std::vector<std::string> toWrite;

    for(auto x : res){
        for(auto y : fmake){
            if(bins[x-1] == eraseSubStr(y,erase)) {
                std::cout<<stringPath<<std::endl;
                std::cout<< bins[x-1] << " is equal to " << y <<std::endl;
                command = "make -f " + stringPath + makepath_script + y + " run";
                toWrite.push_back(command);
                count++;
                break;
            }
        }
    }

    generate_script_helper(toWrite);
}

void ui::generate_script_helper(std::vector<std::string> toWrite){
    std::string script_name;
    std::string path;
    std::string template_path;
    std::string line;
    std::string lineBreak = "run_script(){";
    std::cout<<"Please enter script name : ";
    std::cin>>script_name;

    path = this->stringPath + save_script + script_name + ".sh";
    template_path = this->stringPath + temp_script;

    std::cout<< GREEN << "Genarating script...."<<ENDCOLOR<<std::endl;
    std::cout<<"Script generated in : "<<path<<std::endl;

    std::ofstream fileout(path);
    std::ifstream filein(template_path);

    while(std::getline (filein,line)){
        fileout << line <<std::endl;
        if(line == lineBreak){
            for(auto x : toWrite){
                fileout << x << std::endl;
            }
        }
    }

    fileout.close();
    filein.close();

    std::cout<< GREEN << "Successfully Generated script!!"<<ENDCOLOR<<std::endl;
}

int ui::getIter(){
    std::string line;
    int line_num = 1;
    int count = 0;
    std::ifstream filein(config_path);
    std::string erase_iter = "iterasions = ";


    if(!filein){
        std::cout<<"Error opening config file..."<<std::endl;
        return 0;
    }
    
    while (std::getline(filein,line)){
        if(line_num == 3){
            count = atoi(eraseSubStr(line,erase_iter).c_str());
            return count;
        }
        line_num++;
    }
    std::cout<<"could not find option iterasions"<<std::endl;
    return -1;
}



void ui::main_menu(){
    system ("clear");
    int command;
    bool status=true;

    getDS();
    getfmake();
    formatVector();
    printLogo();
    
    while(status){
        std::cout<<"############## Main menu ##############"<<std::endl;
        std::cout<<"1. Run Benchmarks"<<std::endl;
        std::cout<<"2. Generate Script"<<std::endl;
        std::cout<<"3. Edit config"<<std::endl;
        std::cout<<"4. Print config"<<std::endl;
        std::cout<<"5. Open Server"<<std::endl;
        std::cout<<"6. Exit"<<std::endl;
        std::cin >> command;
        std::cout<<"\n\n\n";
        if(command == 1){
            choise();
        }else if(command == 2){
            generate_script();
        }else if(command == 3){
            system("python3 ./Helpers/edit_config.py");
            system("bash ./Helpers/edit_make.sh");
         }else if(command == 4){
            print_config();
        }else if(command == 5){
            system("cd client; npm run build; cd ..;");
            system("node server.js");
        }
        else if(command == 6){
            status=false;
        }else{
            std::cout<<"Wrong input ... please choose an action (1-5)"<<std::endl;
        }
    }
}

void ui::print_config(){
    std::string line;
    int counter = 1;
    std::ifstream file(config_path);
    while(std::getline(file,line)){
        std::cout <<counter <<" ) "<<line<<std::endl;
        counter++;
    }
    file.close();
    std::cout<<"\n\n";
}


void ui::printLogo(){
    std::cout<<R"(

   ______                ________                       ________                                                        __       
 .' ___  |              |_   __  |                     |_   __  |                                                      [  |  _   
/ .'   \_| .--.  _ .--.   | |_ \_|_   __ _ .--.          | |_ \_| .--.  ,--.  _ .--..--.  .---. _   _   __  .--.  _ .--.| | / ]  
| |      / .'`\ [ `.-. |  |  _| _[ \ [  | '/'`\ \        |  _| [ `/'`\]`'_\ :[ `.-. .-. |/ /__\[ \ [ \ [  ] .'`\ [ `/'`\] '' <   
\ `.___.'\ \__. || | | | _| |__/ |> '  < | \__/ |______ _| |_   | |    // | |,| | | | | || \__.,\ \/\ \/ /| \__. || |   | |`\ \  
 `.____ .''.__.'[___||__]________[__]`\_]| ;.__/_______|_____| [___]   \'-;__[___||__||__]'.__.' \__/\__/  '.__.'[___] [__|  \_] 
                                        [__|                                                                                     

)"<<std::endl;
}

void ui::getfmake(){
    DIR *dir;
    struct dirent *diread;
    if((dir = opendir(makepath.c_str()))!= nullptr){
        while((diread = readdir(dir))!= nullptr){
            fmake.push_back(diread->d_name);
        }
        closedir(dir);
    }
}

std::vector<int> ui::formatString(std::string &mainStr){
    std::vector<int> res;
    std::string delimiter = " ";
    std::string temp;
    size_t pos=0;

    while((pos = mainStr.find(delimiter))!= std::string::npos){
        temp = mainStr.substr(0,pos);
        res.push_back(atoi(temp.c_str()));
        mainStr.erase(0,pos + delimiter.length());
    }
    res.push_back(atoi(mainStr.c_str()));

    return res;
}

void ui::printMenu(){
    int length = bins.size();
    for(int i=0;i<length;i++){
        std::cout<<i+1<<")  "<<bins[i]<<std::endl;
    }
}

void ui::formatVector(){
    int count = 0;
    for(auto x : bins){
        if(x == ".gitkeep") bins.erase(bins.begin()+count);
        else if(x == "..") bins.erase(bins.begin()+count);
        else if(x == ".") bins.erase(bins.begin()+count);
        count++;

    }

}

std::string ui::ds_menu(){
    bool status = true;
    std::string choise;
    while(status){    
    printMenu();
    std::cout<<"Make a choise : ";
    std::cin.ignore();
    std::getline(std::cin,choise);
    if(choise == "exit") return NULL;
    else if(choise == "") continue;
    else if (choise == "0") continue; 
    status = false;
    }
    return choise;
}

void ui::run_test(std::string &command){
    for(int i=0;i<getIter();i++){
        std::cout<<"here"<<std::endl;
        system(command.c_str());
        std::cout<< GREEN << "Iteration : "<< i+1 <<"/"<< getIter()<<ENDCOLOR<<std::endl;
    }
}

void ui::choise(){
    std::string choise = ds_menu();
    std::vector<int> res = formatString(choise);
    std::string command;
    int count = 1;

    // for(auto x: res){
    //     std::cout<<"res is  = " <<x<<std::endl;
    // }

    for(auto x : res){
        for(auto y : fmake){
            if(bins[x-1] == eraseSubStr(y,erase)) {
                // std::cout<< bins[x-1] << " is equal to " << y <<std::endl;
                command = "make -f " + makepath + y + " run";
                std::cout<<command<<std::endl;
                run_test(command);
                std::cout<< GREEN << "Running Benchmarks : "<< count <<"/"<< res.size()<<ENDCOLOR<<std::endl;
                count++;
                break;
            }
        }
    }

    std::cout<< GREEN << "Plotting results...."<<ENDCOLOR<<std::endl;
    system("python3 ./Helpers/visualizer.py");
}

void ui::getDS(){
    DIR *dir;
    struct dirent *diread;
    if((dir = opendir(DSbinpath.c_str()))!= nullptr){
        while((diread = readdir(dir))!= nullptr){
            bins.push_back(diread->d_name);
        }
        closedir(dir);
    }
}


std::string ui::eraseSubStr(std::string mainStr,std::string toErase){
    size_t pos = mainStr.find(toErase);
    std::string res;

    if(pos != std::string::npos){
        res = mainStr.erase(pos,toErase.length());
    }
    return res;
}

#endif