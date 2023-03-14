#!/bin/bash
echo "\$0 : $0"

RED="\e[31m"
GREEN="\e[32m"
ENDCOLOR="\e[0m"

install_python(){
    echo -e "[${GREEN}Installing python.....${ENDCOLOR}]"
    install="python python3"
    sudo apt update
    for p in $install
    do
        echo "Installing $p package"
        sudo apt-get install "$p"
    done
}
install_pip(){
    echo -e "[${GREEN}Installing pip.....${ENDCOLOR}]"
    sudo apt install python3-pip
}
install_python_libs(){
    echo -e "[${GREEN}Installing dependencies.....${ENDCOLOR}]"
    install="numpy matplotlib pandas"
    for x in $install
    do
        echo "Installing $x package"
        pip install "$x"
    done
}

make_validations(){
    cd Build/Makefiles
    temp="./makefile.template"
    echo -e "[${GREEN}Compaling validation binaries.....${ENDCOLOR}]"
    var=$(find ./ -type f -name "makefile.*")
    for x in $var
    do
        make -f $x validate
    done
    cd ./../..
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

install_perf(){
   sudo apt-get install linux-tools-common linux-tools-generic linux-tools-`uname -r`
   sudo echo -1 >/proc/sys/kernel/perf_event_paranoid
   # find a way to edit perf_event_paranoid
}

install_jemalloc(){
   ./libraries/autogen.sh
   sudo make
   sudo make install
}



input_template="makefile.template"

RED="\e[31m"
GREEN="\e[32m"
ENDCOLOR="\e[0m"

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

make_menu(){
    echo -e "[${GREEN}############ Setting up the interface ############${ENDCOLOR}]"
    cd ./lib/Menu
    g++ menu_driver.cpp -o ../../App
    echo -e "[${GREEN}Interface is done!${ENDCOLOR}]"
}







make_config(){
    echo -e "[${GREEN}############ Writting Config file ############${ENDCOLOR}]"
    echo "actions = 100000" >> config.conf
    echo "time = 10000" >> config.conf
    echo "iterasions = 1" >> config.conf
    echo "numThreads = 8" >> config.conf
    echo "localworkStatus = 0" >> config.conf
    echo "localwork = 0" >> config.conf
    echo "threadPinning = 0" >> config.conf
    echo "allocator := " >> config.conf
    echo "jemalloc := -L`jemalloc-config --libdir` -Wl,-rpath,`jemalloc-config --libdir` -ljemalloc `jemalloc-config --libs`" >> config.conf
    echo "hoard_alloc := $(ROOT_dir)/../../libraries/Hoard-master/src/libhoard.so" >> config.conf
    echo "PERF_PRELOAD := perf stat -a -e cache-references,cache-misses,L1-dcache-load,L1-dcache-loads-misses,L1-dcache-stores,cache-references:u,cache-misses:u,L1-dcache-load:u,L1-dcache-loads-misses:u,L1-dcache-stores:u" >> config.conf

    echo -e "[${GREEN}Default Config Complete!${ENDCOLOR}]"
}

install_horder(){
    cd libraries/Hoarde/src
    make
}


# install_python
# install_pip
# install_python_libs
install_perf
install_jemalloc
install_horder
sudo apt-get install libnuma-dev
make_config
make_make
# make_validations
make_bins
make_menu

echo -e "[${GREEN}##################  Done  ##################${ENDCOLOR}]"
