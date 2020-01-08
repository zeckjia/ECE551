#include <cstdio>
#include <cstdlib>

#include "bstset.h"

template<typename T>
void testAdd(BstSet<T> * bst, T * keys, int size) {
  for (int i = 0; i < size; i++) {
    bst->add(keys[i]);
  }
}

template<typename T>
void testAdd(BstSet<T> * bst, T key) {
  bst->add(key);
}

template<typename T>
void testLookup(BstSet<T> * bst, T key) {
  if (bst->contains(key)) {
    std::cout << "Find" << key << std::endl;
  }
  else {
    std::cout << "Could not find" << key << std::endl;
  }
}

template<typename T>
void testRemove(BstSet<T> * bst, T key) {
  bst->remove(key);
}

int main(void) {
  BstSet<int> a;
  int keys1[9] = {50, 40, 70, 20, 55, 60, 80, 65, 63};
  testAdd<int>(&a, keys1, 9);

  BstSet<int> b(a);
  BstSet<int> c = a;

  // test add
  testAdd<int>(&a, 8);
  testLookup(&a, 8);
  testLookup(&a, 20);
  testLookup(&a, 40);
  testLookup(&a, 7);

  // remove a leaf
  testRemove(&a, 8);
  testLookup(&a, 8);
  testLookup(&a, 20);
  testLookup(&a, 40);

  // remove a parent with only on child on the left
  testRemove(&a, 65);
  testLookup(&a, 63);
  testLookup(&a, 65);

  // remove a parent with only on child on the right
  testAdd<int>(&b, 30);
  testRemove(&b, 20);
  testLookup(&b, 20);
  testLookup(&b, 30);
  testLookup(&b, 40);

  // remove a parent with only on child on the right(child has one left child)
  testRemove(&b, 60);
  testLookup(&b, 63);
  testLookup(&b, 65);
  testLookup(&b, 70);
  testLookup(&b, 60);

  // remove a parent with two children (left child has a right child, which has a left child)
  testRemove(&c, 70);
  testLookup(&c, 60);
  testLookup(&c, 80);
  testLookup(&c, 65);
  testLookup(&c, 63);
  testLookup(&c, 70);

  return EXIT_SUCCESS;
}
