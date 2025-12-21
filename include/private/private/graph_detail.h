#ifndef GRAPH_DETAIL_H
#define GRAPH_DETAIL_H

#include "cgraph/type.h"

#define UNREACHABLE_BYTE 0x7f

typedef struct CGraphAttribute_ CGraphAttribute;

// 最小图结构
typedef struct {
  CGraphSize vertRange;
  CGraphId vertHead, *vertNext;

  CGraphBool directed;
  CGraphSize edgeRange;
  CGraphId *edgeHead, *edgeNext;
  CGraphEndpoint *endpts;
} CGraphView;

struct CGraph_ {
  CGraphSize vertCap, edgeCap;
  CGraphSize vertNum, edgeNum;
  CGraphId vertFree, edgeFree;
  CGraphView view;
};

struct CGraphIter_ {
  const CGraphView *view;
  CGraphId vertCurr;
  CGraphId edgeCurr[0];
};

#define REVERSE(did) ((did) ^ 1)
#define VIEW(graph) (&(graph)->view)

#endif // GRAPH_DETAIL_H