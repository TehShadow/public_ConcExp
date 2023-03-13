
RED="\e[31m"
GREEN="\e[32m"
ENDCOLOR="\e[0m"


run_script(){

}


init_script(){
    echo "Enter number of Iterations : "
    read iterations
    for(( c=1; c<=$iterations; c++ ))
    do
        echo -e "[${GREEN}[ Iterations $c/$iterations ]${ENDCOLOR}]"
        run_script
    done
}

init_script