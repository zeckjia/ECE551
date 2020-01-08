#include <exception>
#include <iostream>

#include "set.h"

template<typename T>
class BstSet : public Set<T> {
 private:
  class Node {
   public:
    T k;
    Node * left;
    Node * right;
    Node() : left(NULL), right(NULL) {}
    Node(const Node & rhs) : k(rhs.k), left(NULL), right(NULL) {}
    ~Node() {
      delete left;
      delete right;
    }
  };
  Node * root;

 public:
  BstSet<T>() : root(NULL) {}

  void copyHelper(Node * curr, BstSet & bstset) {
    if (curr != NULL) {
      bstset.add(curr->k);
      copyHelper(curr->left, bstset);
      copyHelper(curr->right, bstset);
    }
  }

  BstSet<T>(const BstSet & rhs) : root(NULL) { copyHelper(rhs.root, *this); }

  BstSet & operator=(const BstSet & rhs) {
    if (this != &rhs) {
      BstSet temp(rhs);
      Node * t = temp.root;
      temp.root = this->root;
      this->root = t;
    }
    return *this;
  }

  Node ** lookupHelper(Node ** ptr, const T & key) {
    if (*ptr == NULL) {
      return ptr;
    }
    else if ((*ptr)->k == key) {
      return ptr;
    }
    else if ((*ptr)->k > key) {
      return lookupHelper(&(*ptr)->left, key);
    }
    else {
      return lookupHelper(&(*ptr)->right, key);
    }
  }

  void add(const T & key) {
    Node ** ptr = lookupHelper(&root, key);
    if (*ptr == NULL) {
      *ptr = new Node();
      (*ptr)->k = key;
    }
  }

  bool contains(const T & key) const {
    Node * ptr = root;
    while (ptr != NULL) {
      if (ptr->k == key) {
        return true;
      }
      else if (ptr->k > key) {
        ptr = ptr->left;
      }
      else {
        ptr = ptr->right;
      }
    }
    return false;
  }

  void remove(const T & key) {
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
      T temp(curr->k);
      remove(curr->k);
      (*ptr)->k = temp;
    }
  }
  ~BstSet<T>() { delete root; }
};
