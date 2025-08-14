#ifndef GRAPH_VERTEX_EDGE_H
#define GRAPH_VERTEX_EDGE_H

#include <stdbool.h>
#include <stdint.h>

#define CGRAPH_TRUE true
#define CGRAPH_FALSE false
#define INVALID_ID (-1)
#define UNREACHABLE 0x7f7f7f7f7f7f7f7f

typedef bool CGraphBool;
typedef int64_t CGraphId;
typedef int64_t CGraphInt;
typedef uint64_t CGraphSize;
typedef int64_t WeightType;

typedef WeightType TimeType; // aoa
typedef WeightType FlowType; // flow

typedef struct CGraph_ CGraph;
typedef struct CGraphIter_ CGraphIter;
typedef struct CGraphLinkedNode_ CGraphLinkedNode;

struct CGraphLinkedNode_ {
  CGraphLinkedNode *next;
  CGraphId id;
};

typedef struct {
  CGraphId to, from;
} CGraphEndpoint;

#endif // GRAPH_VERTEX_EDGE_H
