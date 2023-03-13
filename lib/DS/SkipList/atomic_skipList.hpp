#pragma once
#include <atomic>
#include <cstdlib>
#include <iostream>
#include <vector>


const int MaxLevel = 16;

class Node {
 public:
  int key;
  std::vector<std::atomic<Node*>> next;
  Node(int level, int key) : key(key), next(level + 1) {}
};

class SkipList {
 public:


    Node* head;
    std::atomic<int> level;
    float prop = 0.4;

  SkipList(){
    this->head = new Node(0,0);
    this->level = {MaxLevel};
  }



  bool Insert(int key) {
    std::vector<Node*> update(MaxLevel);
    Node* node = head;
    for (int i = level; i >= 0; i--) {
      Node* next = node->next[i].load();
      while (next != nullptr && next->key < key) {
        node = next;
        next = node->next[i].load();
      }
      update[i] = node;
    }
    Node* new_node = update[0]->next[0].load();
    if (new_node == nullptr || new_node->key != key) {
      int new_level = RandomLevel();
      if (new_level > level) {
        for (int i = level + 1; i <= new_level; i++) {
          update[i] = head;
        }
        level = new_level;
      }
      new_node = new Node(new_level, key);
      for (int i = 0; i <= new_level; i++) {
        new_node->next[i].store(update[i]->next[i].load());
        update[i]->next[i].store(new_node);
      }
      return true;
    }
    return false;
  }

  int Delete(int key) {
    std::vector<Node*> update(MaxLevel);
    Node* node = head;
    for (int i = level; i >= 0; i--) {
      Node* next = node->next[i].load();
      while (next != nullptr && next->key < key) {
        node = next;
        next = node->next[i].load();
      }
      update[i] = node;
    }
    Node* del_node = node->next[0].load();
    if (del_node != nullptr && del_node->key == key) {
      for (int i = 0; i <= level; i++) {
        if (update[i]->next[i].load() != del_node) break;
        update[i]->next[i].store(del_node->next[i].load());
      }
      while (level > 0 && head->next[level].load() == nullptr) {
        level--;
      }
      return del_node->key;
    }
    return NULL;
  }

  bool Search(int key) {
    Node* node = head;
    for ( int i = level; i >= 0; i--) {
    Node* next = node->next[i].load();
    while (next != nullptr && next->key < key) {
        node = next;
        next = node->next[i].load();
        }
    }
    node = node->next[0].load();
    if (node != nullptr && node->key == key) {
        return true;
    }
    return false;
    }

    private:

    int RandomLevel(){
    float random = (float)rand()/RAND_MAX;
    int lvl = 0;
    while(random<this->prop && lvl<MaxLevel){
        lvl++;
        random = (float)rand()/RAND_MAX;
    }
    return lvl;
}


};