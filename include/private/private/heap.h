#ifndef HEAP_H
#define HEAP_H

#include "graph/type.h"

typedef struct {
  GraphSize capacity, size;
  const WeightType *weights;
  GraphId elems[1];
} GraphHeap;

GraphHeap *graphHeapCreate(GraphSize capacity, const WeightType *weights);

void graphHeapPush(GraphHeap *heap, GraphId id);

GraphId graphHeapPop(GraphHeap *heap);

void graphHeapRelease(GraphHeap *heap);

GraphHeap *graphHeapBuild(GraphSize capacity, const WeightType *weights);

static inline GraphBool graphHeapEmpty(const GraphHeap *const heap) {
  return heap->size == 0;
}

#endif // HEAP_H
