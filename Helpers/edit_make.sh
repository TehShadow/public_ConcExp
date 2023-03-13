#!/bin/bash


RED="\e[31m"
GREEN="\e[32m"
ENDCOLOR="\e[0m"
path="./makefile.template"
script_dir=$(cd -- "$( dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)
git="./.gitkeep"
path_2="./makefile.start"


input_template="makefile.template"
make_make(){
    echo -e "[${GREEN}############ Genarating Makefiles ############${ENDCOLOR}]"
    cd lib/Drivers
    var=$(find ./ -type f -name "*.cpp")
    cd ./../../Build/Makefiles
    for x in $var
    do
        bin=$(echo $x | sed 's/^.//;s/^.//')
        name=$(echo $x | sed 's/_driver.cpp//;s/.//;s/^.//')

        if [ -f "makefile.$name" ];then
            echo -e "[${RED} $name already exists...${ENDCOLOR}]"
            continue
        fi
        touch makefile.$name
        input="makefile.$name"
        chmod 666 "makefile.$name"
        while IFS= read -r line
        do
            
            line=$(echo "${line//bin_name/"$name"}")
            line=$(echo "${line//driver_name/"$bin"}")
            echo "$line" >> "$input"
        done < "$input_template"
    echo -e "$name[${GREEN}DONE${ENDCOLOR}]"
    done
    echo -e "[${GREEN}Generated all makefiles!${ENDCOLOR}]"
}

make_bins(){
    cd Build/Makefiles
    temp="./makefile.template"
    echo -e "[${GREEN}Compaling binaries.....${ENDCOLOR}]"
    var=$(find ./ -type f -name "makefile.*")
    for x in $var
    do
        make -f $x build
    done
    cd ./../..
}

clear_make(){
    cd Build/Makefiles
    echo -e "[${GREEN}Deleting makefiles.....${ENDCOLOR}]"
    var=$(find ./ -type f -name "makefile.*")
    for x in $var
    do  
        if [ $x != $path ] && [ $x != $path_2 ]
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

clear_make
clear_bin
make_make
make_bins

