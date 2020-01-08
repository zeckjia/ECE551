#include <cstdio>
#include <cstdlib>

#include "bstmap.h"

template<typename K, typename T>
void testAdd(BstMap<K, T> * bst, K * keys, T * values, int size) {
  for (int i = 0; i < size; i++) {
    bst->add(keys[i], values[i]);
  }
}

template<typename K, typename T>
void testAdd(BstMap<K, T> * bst, K key, T value) {
  bst->add(key, value);
}

template<typename K, typename T>
void testLookup(BstMap<K, T> * bst, K * keys, int size) {
  for (int i = 0; i < size; i++) {
    try {
      std::cout << bst->lookup(keys[i]) << std::endl;
    }
    catch (std::invalid_argument & e) {
      std::cout << e.what() << keys[i] << std::endl;
    }
  }
}

template<typename K, typename T>
void testLookup(BstMap<K, T> * bst, K key) {
  try {
    std::cout << bst->lookup(key) << std::endl;
  }
  catch (std::invalid_argument & e) {
    std::cout << e.what() << key << std::endl;
  }
}

template<typename K, typename T>
void testRemove(BstMap<K, T> * bst, K * keys, int size) {
  for (int i = 0; i < size; i++) {
    bst->remove(keys[i]);
  }
}

template<typename K, typename T>
void testRemove(BstMap<K, T> * bst, K key) {
  bst->remove(key);
}

int main(void) {
  BstMap<int, char> a;
  int keys1[9] = {50, 40, 70, 20, 55, 60, 80, 65, 63};
  char values1[9] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'};
  testAdd<int, char>(&a, keys1, values1, 9);

  BstMap<int, char> b(a);
  BstMap<int, char> c = a;

  // test add
  testAdd<int, char>(&a, 8, 'x');
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
  testAdd(&b, 30, 'y');
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
