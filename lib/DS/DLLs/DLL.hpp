#pragma once

#include "iostream"

template<class X>
class DoublyLinkedList{
    ///@brief inserts on the head of the list
    virtual bool insert(X) = 0;
    ///@brief inserts on the tail of the list
    virtual bool insertTail(X) = 0;
    ///@brief remove a node from the list
    virtual T remove(T) = 0;
    ///@brief searches for an element on the list
    virtual T search(T) = 0;
    ///@brief returns the size of the list
    virtual int size() = 0;
    ///@brief prints the whole list
    virtual void printList() = 0;
};