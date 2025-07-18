#ifndef GRAPH_VERTEX_EDGE_H
#define GRAPH_VERTEX_EDGE_H

#include <stdbool.h>
#include <stdint.h>

#define GRAPH_TRUE true
#define GRAPH_FALSE false
#define INVALID_ID -1
#define UNREACHABLE 0x7f7f7f7f7f7f7f7f

typedef bool GraphBool;
typedef int64_t GraphId;
typedef int64_t GraphInt;
typedef uint64_t GraphSize;
typedef int64_t WeightType;

typedef WeightType TimeType; // aoa
typedef WeightType FlowType; // flow

typedef struct Graph_ Graph;
typedef struct GraphIter_ GraphIter;
typedef struct GraphLinkedNode_ GraphLinkedNode;

struct GraphLinkedNode_ {
  GraphLinkedNode *next;
  GraphId id;
};

typedef struct {
  GraphId to, from;
} GraphEndpoint;

#endif // GRAPH_VERTEX_EDGE_H
