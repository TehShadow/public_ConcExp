#include "./SkipList.hpp"


int main(int argc , char* argv[]){

    srand((unsigned)time(0));


    SkipList<int> list(10,0.4);


    for(int i=0;i<28712;i+=3){
        list.insert(i);
        list.insert(i);
}

    Node<int>* deltemp = list.remove(27207);
    Node<int>* findtemp = list.find(1782);

    list.display();
    std::cout<<deltemp->data<<std::endl;
    std::cout<<findtemp->data<<std::endl;


}