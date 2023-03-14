#pragma once
#include <iostream>
using namespace std;

struct Node{
    int data;
    Node* nextNode;
    Node(int data){
        this->data = data;
        this->nextNode = NULL;
    };
};


class SLL{
    Node* head;
    Node* tail;
    public:
        SLL(){
            this->head = NULL;
        }
    
    int insert(int data){
        Node* newNode = new Node(data);
        newNode->nextNode = this->head;
        this->head = newNode;
        return 1;
    }

    int remove(){
        if(!this->head) return -1;
        Node* tempNode = this->head;
        Node* delNode;
        while(tempNode->nextNode != this->tail){
            tempNode = tempNode->nextNode;
        }
        delNode = this->tail;
        this->tail = tempNode;
        this->tail->nextNode = NULL;
        return 1;

    }

    int size(){
        Node* current = head;
        int count = 0;
        while(current){
            count++;
            current = current->nextNode;
        }
        return count;
    }

    int count_doubles_in_seq(){
        Node* x = head;
        Node* y = head->nextNode;
        int count = 0;
        while(x && y){
            if(x->data == y->data){
                count++;
            }
            else x = y;
            y = y->nextNode;
        }
        return count;
    }

    void print() {
        Node* current = head;
        while (current) {
            std::cout << current->data << " ";
            current = current->nextNode;
        }
        std::cout << std::endl;
    }

        SLL* copy() {
            SLL* newList = new SLL();
            if (!this->head) {
                return newList; 
            }
            Node* newHead = new Node(head->data);
            Node* current = newHead;
            Node* currentOld = head->nextNode;
            while (currentOld) {
                current->nextNode = new Node(currentOld->data); 
                current = current->nextNode;
                currentOld = currentOld->nextNode;
            }
            newList->head = newHead;
            return newList;
    }

    void mergeSort(){
        this->head = mergeSort(this->head);
    }

private:
    Node* mergeSort(Node *n) {
        if (!n|| !n->nextNode) return n;
        
        Node* slow = n;
        Node* fast = n->nextNode;
        
        while (fast && fast->nextNode) {
            slow = slow->nextNode;
            fast = fast->nextNode->nextNode;
        }
        
        Node* right = slow->nextNode;
        slow->nextNode = nullptr;
        
        Node* left = mergeSort(n);
        right = mergeSort(right);
        
        return merge(left, right);
    }
    
    Node* merge(Node* l1, Node* l2) {
        if (!l1) return l2;
        if (!l2) return l1;

        Node* dummy = new Node(0);
        Node* tail = dummy;

        while (l1 && l2) {
            if (l1->data < l2->data) {
                tail->nextNode = l1;
                l1 = l1->nextNode;
            }
            else {
                tail->nextNode = l2;
                l2 = l2->nextNode;
            }
            tail = tail->nextNode;
        }

        if (l1) tail->nextNode = l1;
        if (l2) tail->nextNode = l2;

        Node* head = dummy->nextNode;
        delete dummy;
        return head;
    }

};