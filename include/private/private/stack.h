#ifndef GRAPH_STACK_H
#define GRAPH_STACK_H

#include "graph/type.h"

typedef struct {
  GraphSize size;
  GraphId elems[0];
} GraphStack;

GraphStack *graphNewStack(GraphSize capacity);

void graphStackRelease(GraphStack *stack);

static inline void graphStackPush(GraphStack *const stack, const GraphId item) {
  stack->elems[stack->size++] = item;
}

static inline GraphId graphStackPop(GraphStack *const stack) {
  return stack->elems[--stack->size];
}

static inline GraphBool graphStackEmpty(const GraphStack *const stack) {
  return stack->size == 0;
}

#endif // GRAPH_STACK_H
