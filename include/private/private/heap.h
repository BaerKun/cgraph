#ifndef HEAP_H
#define HEAP_H

#include "graph/type.h"

typedef struct {
  GraphSize capacity, size;
  const WeightType *weights;
  GraphId elems[0];
  GraphId dummy;
} GraphHeap;

GraphHeap *graphHeapCreate(GraphSize capacity, const WeightType *weights);

void graphHeapPush(GraphHeap *heap, GraphId id);

GraphId graphHeapPop(GraphHeap *heap);

void graphHeapRelease(GraphHeap *heap);

void graphHeapBuild(GraphHeap *heap);

static inline GraphBool graphHeapEmpty(const GraphHeap *const heap) {
  return heap->size == 0;
}

static void graphHeapPreBuild(GraphHeap *heap, const GraphId id) {
  heap->elems[++heap->size] = id;
}

#endif // HEAP_H
