#pragma once

#include <iostream>

template<class T>
class SLL{
    public:
        /// @brief insert a node to the list
        virtual bool insert(T) = 0;
        /// @brief remove a X node from a list
        /// @param parameter-T an element to remove
        virtual T remove(T) = 0;
        /// @brief search the list for an element
        /// @param parameter-T element to search for in the list
        virtual T search(T) = 0;
        /// @brief return the size of the list
        virtual long long size() = 0;
        /// @brief prints the list
        virtual void printList() = 0;
        virtual void empty() = 0;
};