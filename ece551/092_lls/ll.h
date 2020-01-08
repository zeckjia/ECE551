#ifndef _LL_H_
#define _LL_H_
#include <assert.h>

#include <cstdio>
#include <cstdlib>
#include <exception>

//YOUR CODE GOES HERE
template<typename T>
class testIndex : public std::exception {
  int index;

 public:
  testIndex() : index(0) {}
  testIndex(int i) : index(i) {}
};

template<typename A>
void swap(A & lhs, A & rhs) {
  A temp = lhs;
  lhs = rhs;
  rhs = temp;
}

template<typename T>
class LinkedList {
 private:
  class Node {
   public:
    T data;
    Node * next;
    Node * prev;
    Node() : next(NULL), prev(NULL) {}
    Node(const T & item) : data(item), next(NULL), prev(NULL) {}
  };
  Node * head;
  Node * tail;
  int size;

 public:
  LinkedList() : head(NULL), tail(NULL), size(0) {}

  LinkedList(const LinkedList & rhs) : head(NULL), tail(NULL), size(0) {
    for (int i = 0; i < rhs.size; i++) {
      addBack(rhs[i]);
    }
  }

  LinkedList & operator=(const LinkedList & rhs) {
    if (this != &rhs) {
      LinkedList temp(*this);
      while (temp.size > 0) {
        temp.remove(temp[0]);
      }

      for (int i = 0; i < rhs.size; i++) {
        temp.addBack(rhs[i]);
      }

      swap(head, temp.head);
      swap(tail, temp.tail);
      swap(size, temp.size);
    }
    return *this;
  }

  ~LinkedList() {
    Node * curr = head;
    for (int i = 0; i < size; i++) {
      Node * temp = curr->next;
      delete curr;
      curr = temp;
    }
  }

  void addFront(const T & item) {
    // empty list
    if (head == NULL) {
      head = new Node(item);
      tail = head;
    }
    else {
      head->prev = new Node(item);
      head->prev->next = head;
      head = head->prev;
    }
    size++;
  }

  void addBack(const T & item) {
    // empty list
    if (tail == NULL) {
      tail = new Node(item);
      head = tail;
    }
    else {
      tail->next = new Node(item);
      tail->next->prev = tail;
      tail = tail->next;
    }
    size++;
  }

  bool remove(const T & item) {
    // empty list
    if (head == NULL) {
      return false;
    }

    // find item
    Node * rmPtr = head;
    do {
      if (rmPtr->data == item) {
        break;
      }
      rmPtr = rmPtr->next;
    } while (rmPtr != NULL);
    // fail to find item
    if (rmPtr == NULL) {
      return false;
    }

    // if the item is the head
    if (rmPtr == head) {
      head = rmPtr->next;  // change head
    }
    // if the item is the tail
    if (rmPtr == tail) {
      tail = rmPtr->prev;  // change tail
    }
    if (rmPtr->next != NULL) {
      rmPtr->next->prev = rmPtr->prev;  // change rmPtr->next->prev
    }
    if (rmPtr->prev != NULL) {
      rmPtr->prev->next = rmPtr->next;  // change rmPtr->prev->next
    }
    delete rmPtr;
    size--;
    return true;
  }

  T & operator[](int index) {
    if (index < 0 || index >= size) {
      throw testIndex<T>(index);
    }

    Node * ans = head;
    for (int i = 0; i < size; i++) {
      if (i == index) {
        break;
      }
      ans = ans->next;
    }
    return ans->data;
  }

  const T & operator[](int index) const {
    if (index < 0 || index >= size) {
      throw testIndex<T>(index);
    }

    Node * ans = head;
    for (int i = 0; i < size; i++) {
      if (i == index) {
        break;
      }
      ans = ans->next;
    }
    return ans->data;
  }

  int find(const T & item) const {
    Node * curr = head;
    for (int i = 0; i < size; i++) {
      if (curr->data == item) {
        return i;
      }
      curr = curr->next;
    }
    return -1;
  }

  int getSize() const { return size; }
};

#endif
