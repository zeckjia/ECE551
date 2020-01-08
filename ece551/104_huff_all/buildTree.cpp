#include "node.h"

Node * buildTree(uint64_t * counts) {
  // make a priority tree
  priority_queue_t pq;
  for (unsigned i = 0; i < 257; i++) {
    if (counts[i] != 0) {
      pq.push(new Node(i, counts[i]));
    }
  }

  // build tree
  while (pq.size() != 1) {
    Node * left = pq.top();
    pq.pop();
    Node * right = pq.top();
    pq.pop();
    Node * parent = new Node(left, right);
    pq.push(parent);
  }
  return pq.top();
}
