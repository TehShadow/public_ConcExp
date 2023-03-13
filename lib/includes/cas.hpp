#pragma once

#include "iostream"
#include <atomic>
#include <cstdint>


template<class X>
bool CAS(X *ptr,X oldValue , X newValues){

    bool ret;
    // std::cout<<sizeof(X)<<std::endl;

    // std::cout<<"ptr : "<<ptr<<std::endl;
    // std::cout<<"oldV : "<<oldValue<<std::endl;
    // std::cout<<"newV : "<<newValues<<std::endl;

    if(sizeof(X) == 1){
        ret = __sync_bool_compare_and_swap_1((bool*)ptr,*((bool*)&oldValue),*((bool*)&newValues));
    }
    else if(sizeof(X) == 8){
        ret = __sync_bool_compare_and_swap_8((long*)ptr,*((long*)&oldValue),*((long*)&newValues));
    }
    else if(sizeof(X) == 4){
        ret = __sync_bool_compare_and_swap_4((int*)ptr,*((int*)&oldValue),*((int*)&newValues));
    }
    else{
        std::cout<<"Cas bad length"<<std::endl;
    }
    return ret;

}


