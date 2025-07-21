#include "private/heap.h"
#include <stdlib.h>

static void graphHeapifyDown(GraphHeap *heap, GraphSize father) {
  const GraphId top = heap->data[father];
  const WeightType topValue = heap->weight[top];

  for (GraphSize child; (child = father << 1) <= heap->size; father = child) {
    if (child != heap->size &&
        heap->weight[heap->data[child + 1]] < heap->weight[heap->data[child]])
      ++child;
    if (heap->weight[heap->data[child]] < topValue)
      heap->data[father] = heap->data[child];
    else
      break;
  }
  heap->data[father] = top;
}

GraphHeap *graphHeapCreate(const GraphSize capacity, const WeightType *weight) {
  GraphHeap *heap = malloc(sizeof(GraphHeap) + capacity * sizeof(GraphId));
  heap->capacity = capacity;
  heap->size = 0;
  heap->weight = weight;
  return heap;
}

void graphHeapRelease(GraphHeap *heap) { free(heap); }

void graphHeapPush(GraphHeap *heap, const GraphId id) {
  const WeightType value = heap->weight[id];
  GraphSize child = ++heap->size;
  for (GraphSize father;
       ((father = child >> 1)) && value < heap->weight[heap->data[father]];
       child = father) {
    heap->data[child] = heap->data[father];
  }
  heap->data[child] = id;
}

GraphId graphHeapPop(GraphHeap *heap) {
  GraphId *top = heap->data + 1;
  const GraphId ret = *top;
  *top = heap->data[heap->size--];
  graphHeapifyDown(heap, 1);
  return ret;
}

GraphHeap *graphHeapBuild(const GraphSize capacity, const WeightType *weight) {
  GraphHeap *heap = graphHeapCreate(capacity, weight);
  heap->size = capacity;

  GraphId *init = heap->data + 1;
  for (GraphId i = 0; i != capacity; ++i) init[i] = i;

  for (uint64_t i = capacity >> 1; i; --i) graphHeapifyDown(heap, i);
  return heap;
}
