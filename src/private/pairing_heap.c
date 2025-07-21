#include "private/pairing_heap.h"
#include <stdlib.h>
#include <string.h>

GraphPairingHeap *graphPairingHeapCreate(const GraphSize capacity,
                                         const WeightType *weights) {
  GraphPairingHeap *heap =
      malloc(sizeof(GraphPairingHeap) + capacity * sizeof(PairingHeapNode));
  heap->root = INVALID_ID;
  heap->weights = weights;
  memset(heap->nodes, INVALID_ID, capacity * sizeof(PairingHeapNode));
  return heap;
}

void graphPairingHeapRelease(GraphPairingHeap *heap) { free(heap); }

static GraphId pairingHeapMerge(GraphPairingHeap *heap, const GraphId l,
                                const GraphId r) {
  if (l == INVALID_ID) return r;
  if (r == INVALID_ID) return l;

  PairingHeapNode *left = heap->nodes + l;
  PairingHeapNode *right = heap->nodes + r;
  if (heap->weights[l] < heap->weights[r]) {
    right->sibling = left->child;
    left->child = r;
    right->parent = l;
    return l;
  }
  left->sibling = right->child;
  right->child = l;
  right->parent = r;
  return r;
}

void graphPairingHeapPush(GraphPairingHeap *heap, const GraphId id) {
  pairingHeapMerge(heap, heap->root, id);
}


GraphId graphPairingHeapPop(GraphPairingHeap *heap) {
  const GraphId root = heap->root;

  return root;
}

void graphPairingHeapUpdate(GraphPairingHeap *heap, GraphId id) {
  if (id == heap->root) return;

}
