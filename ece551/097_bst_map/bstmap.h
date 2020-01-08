#include <exception>
#include <iostream>

#include "map.h"

template<typename K, typename V>
class BstMap : public Map<K, V> {
 private:
  class Node {
   public:
    std::pair<K, V> kv;
    Node * left;
    Node * right;
    Node() : left(NULL), right(NULL) {}
    Node(const Node & rhs) : kv(rhs.kv), left(NULL), right(NULL) {}
    ~Node() {
      delete left;
      delete right;
    }
  };
  Node * root;

 public:
  BstMap() : root(NULL) {}

  void copyHelper(Node * curr, BstMap & bstmap) {
    if (curr != NULL) {
      bstmap.add(curr->kv.first, curr->kv.second);
      copyHelper(curr->left, bstmap);
      copyHelper(curr->right, bstmap);
    }
  }

  BstMap(const BstMap & rhs) : root(NULL) { copyHelper(rhs.root, *this); }

  BstMap & operator=(const BstMap & rhs) {
    if (this != &rhs) {
      BstMap temp(rhs);
      Node * t = temp.root;
      temp.root = this->root;
      this->root = t;
    }
    return *this;
  }

  Node ** lookupHelper(Node ** ptr, const K & key) {
    if (*ptr == NULL) {
      return ptr;
    }
    else if ((*ptr)->kv.first == key) {
      return ptr;
    }
    else if ((*ptr)->kv.first > key) {
      return lookupHelper(&(*ptr)->left, key);
    }
    else {
      return lookupHelper(&(*ptr)->right, key);
    }
  }

  virtual void add(const K & key, const V & value) {
    Node ** ptr = lookupHelper(&root, key);
    if (*ptr == NULL) {
      *ptr = new Node();
      (*ptr)->kv.first = key;
      (*ptr)->kv.second = value;
    }
    else {
      (*ptr)->kv.second = value;
    }
  }
  virtual const V & lookup(const K & key) const throw(std::invalid_argument) {
    Node * ptr = root;
    while (ptr != NULL) {
      if (ptr->kv.first == key) {
        return ptr->kv.second;
      }
      else if (ptr->kv.first > key) {
        ptr = ptr->left;
      }
      else {
        ptr = ptr->right;
      }
    }
    throw std::invalid_argument("Fail to find the key!");
  }

  virtual void remove(const K & key) {
    Node ** ptr = lookupHelper(&root, key);
    if ((*ptr) == NULL) {  // Could not find key;
      return;
    }

    if ((*ptr)->left == NULL) {
      Node * temp = *ptr;
      (*ptr) = (*ptr)->right;
      temp->right = NULL;
      delete temp;
    }
    else if ((*ptr)->right == NULL) {
      Node * temp = *ptr;
      (*ptr) = (*ptr)->left;
      temp->left = NULL;
      delete temp;
    }
    else {
      Node * curr = *ptr;
      curr = curr->left;
      while (curr->right != NULL) {
        curr = curr->right;
      }
      std::pair<K, V> temp(curr->kv);
      remove(curr->kv.first);
      (*ptr)->kv = temp;
    }
  }
  virtual ~BstMap<K, V>() { delete root; }
};
