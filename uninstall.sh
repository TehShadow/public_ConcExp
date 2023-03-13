#!/bin/bash

RED="\e[31m"
GREEN="\e[32m"
ENDCOLOR="\e[0m"

path="./makefile.template"
path_2="./makefile.start"
git="./.gitkeep"

clear_make(){
    cd Build/Makefiles
    echo -e "[${GREEN}Deleting makefiles.....${ENDCOLOR}]"
    var=$(find ./ -type f -name "makefile.*")
    for x in $var
    do  
        if [[ $x != $path ]]
        then
            rm $x  
        fi
    done
    cd ../../
}

clear_bin(){
    cd Build/Bin
    echo -e "[${GREEN}Deleting Binaries.....${ENDCOLOR}]"
    var=$(find ./ -type f -name "*")
    for x in $var
    do  
        if [[ $x != $git ]]
        then
            rm $x  
        fi
    done
    cd ../../
}

del_interface(){
    rm App
    rm config.conf
}

del_logs(){
    echo "Delete all data? (y/n) "
    read command
    if test $command = "y";
    then
        rm -rv ./Logs/Data/*
        rm -rv ./Logs/Plots/*
        rm -rv ./Logs/Perf/*
        rm -rv ./generatedScripts/*
    fi
}

clear_make
clear_bin
del_interface
del_logs

echo -e "[${GREEN}##################  Done  ##################${ENDCOLOR}]"