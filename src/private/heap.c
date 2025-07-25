#include "private/heap.h"
#include <stdlib.h>

static void graphHeapifyDown(GraphHeap *heap, GraphSize father) {
  const GraphId top = heap->elems[father];
  const WeightType topValue = heap->weights[top];

  for (GraphSize child; (child = father << 1) <= heap->size; father = child) {
    if (child != heap->size &&
        heap->weights[heap->elems[child + 1]] < heap->weights[heap->elems[child]])
      ++child;
    if (heap->weights[heap->elems[child]] < topValue)
      heap->elems[father] = heap->elems[child];
    else
      break;
  }
  heap->elems[father] = top;
}

GraphHeap *graphHeapCreate(const GraphSize capacity, const WeightType *weights) {
  GraphHeap *heap = malloc(sizeof(GraphHeap) + capacity * sizeof(GraphId));
  heap->capacity = capacity;
  heap->size = 0;
  heap->weights = weights;
  return heap;
}

void graphHeapRelease(GraphHeap *heap) { free(heap); }

void graphHeapPush(GraphHeap *heap, const GraphId id) {
  const WeightType value = heap->weights[id];
  GraphSize child = ++heap->size;
  for (GraphSize father;
       ((father = child >> 1)) && value < heap->weights[heap->elems[father]];
       child = father) {
    heap->elems[child] = heap->elems[father];
  }
  heap->elems[child] = id;
}

GraphId graphHeapPop(GraphHeap *heap) {
  GraphId *top = heap->elems + 1;
  const GraphId ret = *top;
  *top = heap->elems[heap->size--];
  graphHeapifyDown(heap, 1);
  return ret;
}

GraphHeap *graphHeapBuild(const GraphSize capacity, const WeightType *weights) {
  GraphHeap *heap = graphHeapCreate(capacity, weights);
  heap->size = capacity;

  GraphId *init = heap->elems + 1;
  for (GraphId i = 0; i != capacity; ++i) init[i] = i;

  for (uint64_t i = capacity >> 1; i; --i) graphHeapifyDown(heap, i);
  return heap;
}
