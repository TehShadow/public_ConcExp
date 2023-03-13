#pragma once


#include <iostream>

template <class X>
class Stack{
    public:
        ///@brief pushes element to the stack
        virtual bool push(X x) = 0;
        ///@brief pops top element from the stack
        virtual X pop() = 0;
        ///@brief return size of the stack
        virtual long long size() = 0;

        virtual void empty() = 0;
};