#pragma once

#include <iostream>
#include <cstring>


template <class X>
class Node{
    public:
        X data;
        Node<X>** lvlsArray;

        Node(X data, int lvl){
            this->data = data;
            lvlsArray = new Node<X>*[lvl+1];
            memset(lvlsArray,0,sizeof(Node*)*(lvl+1));
        }
};
 


template<class X>
class SkipList{

    public:

    int maxlvl;
    float prop;
    Node<X>*sentinel;

    SkipList(int maxlvl,float prop){
        this->maxlvl = maxlvl;
        this->prop = prop;
        this->sentinel = new Node<X>(NULL,this->maxlvl);
    }

    bool insert(X);
    Node<X>* remove(X);
    Node<X>* find(X);
    void display();
    int randomlvl();
};

template<class X>
int SkipList<X>::randomlvl(){
    float random = (float)rand()/RAND_MAX;
    int lvl = 0;
    while(random<this->prop && lvl<this->maxlvl){
        lvl++;
        random = (float)rand()/RAND_MAX;
    }
    return lvl;
}

template<class X>
bool SkipList<X>::insert(X data){

    Node<X> *current = this->sentinel;
    Node<X> *update[this->maxlvl+1];
    int level = this->maxlvl;
    memset(update,0,sizeof(Node<X>*)*(maxlvl+1));

    for(int i=level;i>=0;i--){
        while(current->lvlsArray[i] != NULL && 
            current->lvlsArray[i]->data < data) 
                current = current->lvlsArray[i];
        
        update[i] = current;
    }

    current = current->lvlsArray[0];

    if(current == NULL || current->data != data){
        int randomLevel = this->randomlvl();

        if(randomLevel > level){
            for(int i=level+1;i<randomLevel+1;i++) update[i] = this->sentinel;

            level = randomLevel;
            this->maxlvl = randomLevel;
        }

        Node<X>* n = new Node<X>(data,level);

        for(int i=0;i<=randomLevel;i++){
            n->lvlsArray[i] = update[i]->lvlsArray[i];
            update[i]->lvlsArray[i] = n;
        }

        std::cout<<"Successfully Inserted key "<< data <<std::endl;
        return true;
    }
    else return false;
}

template <class X>
void SkipList<X>::display(){
    for(int i= 0;i<=this->maxlvl;i++){
        Node<X> *node = this->sentinel->lvlsArray[i];
        std::cout << "Level " << i << ": ";
        while(node != NULL){
            std::cout<<node->data<<" ";
            node = node->lvlsArray[i];
        }
        std::cout<<"\n"; 
    }
}

template<class X>
Node<X>* SkipList<X>::remove(X data){
    Node<X> *current = this->sentinel;
    Node<X> *temp = NULL;
    Node<X>* update[this->maxlvl+1];
    memset(update,0,sizeof(Node<X>*)*(this->maxlvl+1));
    int level = this->maxlvl;
    
    for(int i=level;i >=0;i--){
        while(current->lvlsArray[i]!=NULL &&
                current->lvlsArray[i]->data < data)
            current = current->lvlsArray[i];
        update[i] = current;
    }

    current = current->lvlsArray[0];

    if(current != NULL && current->data == data) {

        temp = current;
        for(int i=0;i<=level;i++){
            if(update[i]->lvlsArray[i] != current) break;

            update[i]->lvlsArray[i] = current->lvlsArray[i];
        }

        while(level>0 && this->sentinel->lvlsArray[level]==0)
            level--;
            this->maxlvl--;

    }

    return temp;

}

template<class X>
Node<X>* SkipList<X>::find(X data){
    Node<X>* current = this->sentinel;
    Node<X>* temp = NULL;
    int level = this->maxlvl;
    for(int i=level;i>=0;i--){
        while(current->lvlsArray[i]&&
            current->lvlsArray[i]->data < data)
                current = current->lvlsArray[i];
    }

    current = current->lvlsArray[0];

    if(current && current->data == data)
        temp = current;

    return temp;
}


