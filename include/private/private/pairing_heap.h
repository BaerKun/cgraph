#ifndef PAIRING_HEAP_H
#define PAIRING_HEAP_H

#include "graph/type.h"

typedef struct  {
  GraphId child;
  GraphId sibling;
  GraphId parent;
} PairingHeapNode;

typedef struct {
  GraphId root;
  const WeightType *weights;
  PairingHeapNode nodes[0];
} GraphPairingHeap;

GraphPairingHeap *graphPairingHeapCreate(GraphSize capacity,
                                         const WeightType *weights);

void graphPairingHeapRelease(GraphPairingHeap *heap);

void graphPairingHeapPush(GraphPairingHeap *heap, GraphId id);

GraphId graphPairingHeapPop(GraphPairingHeap *heap);

void graphPairingHeapUpdate(GraphPairingHeap *heap, GraphId id);

static inline GraphBool graphPairingHeapEmpty(const GraphPairingHeap *heap) {
  return heap->root == INVALID_ID;
}

#endif // PAIRING_HEAP_H
