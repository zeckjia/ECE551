#include <cassert>
#include <cstdio>

#include "il.h"

void testList(void) {
  IntList l1;
  assert(l1.getSize() == 0 && l1.size == 0);
  assert(l1.head == NULL && l1.tail == NULL);

  l1.addFront(1);
  assert(l1.getSize() == 1 && l1.size == 1);
  assert(l1.head == l1.tail && l1.head != NULL);
  assert(l1.head->next == NULL && l1.head->prev == NULL);

  IntList l2(l1);
  l2.addFront(2);
  assert(l2.getSize() == 2 && l2.size == 2);
  assert(l2.head == l2.head->next->prev);
  assert(l2.tail == l2.tail->prev->next);
  assert(l2.head->next->next == NULL && l2.head->prev == NULL);

  l2 = IntList();
  assert(l2.getSize() == 0 && l2.size == 0);
  assert(l2.head == NULL && l2.tail == NULL);

  l2.addFront(1);
  l2.addBack(2);
  assert(l2.getSize() == 2 && l2.size == 2);
  assert(l2.head == l2.head->next->prev);
  assert(l2.tail == l2.tail->prev->next);
  assert(l2.head->next->next == NULL && l2.head->prev == NULL);
  assert(l2[1] == 2);
  assert(l2[0] == 1);

  l2.addFront(3);
  l2.addFront(4);
  l2.addFront(5);
  l2.addFront(6);
  assert(l2.getSize() == 6 && l2.size == 6);
  assert(l2[0] == 6);
  assert(l2[1] == 5);
  assert(l2[2] == 4);
  assert(l2[3] == 3);
  assert(l2[4] == 1);
  assert(l2[5] == 2);
  assert(l2.find(1) == 4);
  assert(l2.find(2) == 5);
  assert(l2.find(3) == 3);
  assert(l2.find(4) == 2);
  assert(l2.find(5) == 1);
  assert(l2.find(6) == 0);
  assert(l2.find(99) == -1);

  assert(l2.remove(3) == 1);
  assert(l2.remove(4) == 1);
  assert(l2.remove(5) == 1);
  assert(l2.remove(6) == 1);
  assert(l2.remove(6) == 0);
  assert(l2.getSize() == 2 && l2.size == 2);

  IntList l3;
  l3 = l2;
  assert(l3.getSize() == 2 && l3.size == 2);
  assert(l3.head == l3.head->next->prev);
  assert(l3.tail == l3.tail->prev->next);
  assert(l3.head->next->next == NULL && l3.head->prev == NULL);
  assert(l3[1] == 2);
  assert(l3[0] == 1);

  assert(l3.remove(1) == 1);
  assert(l3.remove(2) == 1);
  assert(l3.getSize() == 0 && l3.size == 0);
  assert(l3.head == NULL && l3.tail == NULL);

  IntList l4(l2);
  assert(l4[1] == 2);
  assert(l4[0] == 1);
  assert(l4.remove(1) == 1);
  assert(l4.getSize() == 1 && l4.size == 1);
  assert(l4.head == l4.tail && l4.head != NULL);
  assert(l4.head->next == NULL && l4.tail->prev == NULL);
}

int main(void) {
  testList();
  return EXIT_SUCCESS;
}
