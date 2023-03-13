# pragma once

#include <iostream>


template <class X>
class Queue{
        public:
                ///@brief enqueue a node to the queue
                ///@param parameter-value to enqueue to queue
                virtual bool enqueue(X x) = 0;
                ///@brief dequeue a node
                virtual X dequeue() = 0;
                ///@brief returns the size of the queue
                virtual long long size() = 0 ;
                virtual void empty() = 0 ;
};