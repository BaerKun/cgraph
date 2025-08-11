#ifndef PAIRING_HEAP_H
#define PAIRING_HEAP_H

#include "graph/type.h"

// 视作只有右子树符合约定的搜索二叉树
typedef struct PairingHeapNode_ PairingHeapNode;
struct PairingHeapNode_ {
  WeightType weight;
  PairingHeapNode *left;  // sibling
  PairingHeapNode *right; // child
  PairingHeapNode **parent;
};

typedef struct {
  const WeightType *weights;
  PairingHeapNode *root;
  PairingHeapNode **stack;
  PairingHeapNode nodes[0];
} GraphPairingHeap;

GraphPairingHeap *graphPairingHeapCreate(GraphSize capacity,
                                         const WeightType *weights);

void graphPairingHeapRelease(GraphPairingHeap *heap);

void graphPairingHeapPush(GraphPairingHeap *heap, GraphId id);

GraphId graphPairingHeapPop(GraphPairingHeap *heap);

void graphPairingHeapUpdate(GraphPairingHeap *heap, GraphId id);

static inline GraphBool graphPairingHeapEmpty(const GraphPairingHeap *heap) {
  return !heap->root;
}

#endif // PAIRING_HEAP_H
