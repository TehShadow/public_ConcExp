#include "./SkipListLocks.hpp"


int main(int argc , char* argv[]){

    srand((unsigned)time(0));


    SkipListLocks<int> list(10,0.4);


    for(int i=0;i<28712;i+=3){
        list.insert(i);
        list.insert(i);
}


    list.display();


}