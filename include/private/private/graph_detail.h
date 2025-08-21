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

// 增加ID管理
typedef struct {
  CGraphId vertFree, edgeFree;
  CGraphView view;
} CGraphManager;

// 增加属性及扩容
struct CGraph_ {
  CGraphSize vertCap, edgeCap;
  CGraphSize vertNum, edgeNum;
  CGraphManager manager;
  CGraphAttribute *vertAttr, *edgeAttr;
};

struct CGraphIter_ {
  const CGraphView *view;
  CGraphId vertCurr;
  CGraphId edgeCurr[0];
};

struct CGraphAttribute_ {
  uint64_t hash[2];
  CGraphAttribute *next;
  void *vector;
};

#define REVERSE(did) ((did) ^ 1)

#endif // GRAPH_DETAIL_H
