#ifndef ITER_INTERNAL_H
#define ITER_INTERNAL_H

#include "graph_detail.h"

#define VIEW(graph) (&(graph)->manager.view)

#define FOREACH_EDGE(view, from, eid, to)                                      \
  for (CGraphId from = view->vertHead; from != INVALID_ID;                     \
       from = view->vertNext[from])                                            \
    for (CGraphId did_ = view->edgeHead[from], eid, to; did_ != INVALID_ID;    \
         did_ = view->edgeNext[did_])                                          \
      if (parseForward(view, did_, &eid, &to), 1)

CGraphIter *cgraphIterFromView(const CGraphView *view);
void cgraphIterRelease(CGraphIter *iter);
void cgraphIterResetEdge(CGraphIter *iter, CGraphId from);

inline void cgraphIterResetVert(CGraphIter *iter) {
  iter->vertCurr = iter->view->vertHead;
}

static inline void parseForward(const CGraphView *view, const CGraphId did,
                                CGraphId *eid, CGraphId *to) {
  // 高度重复可预测，保留分支版本
  if (view->directed) {
    *eid = did;
    *to = view->endpts[did].to;
  } else {
    *eid = did >> 1;
    *to = ((CGraphId *)view->endpts)[did];
  }
}

static inline void parseBackward(const CGraphView *view, const CGraphId did,
                                 CGraphId *eid, CGraphId *from) {
  if (view->directed) {
    *eid = did;
    *from = view->endpts[did].from;
  } else {
    *eid = did >> 1;
    *from = ((CGraphId *)view->endpts)[REVERSE(did)];
  }
}

static inline CGraphBool
cgraphIterNextDirect(CGraphIter *iter, const CGraphId from, CGraphId *did) {
  CGraphId *curr = iter->edgeCurr + from;
  if (*curr == INVALID_ID) return CGRAPH_FALSE;
  *did = *curr;
  *curr = iter->view->edgeNext[*curr];
  return CGRAPH_TRUE;
}

inline CGraphBool cgraphIterNextVert(CGraphIter *iter, CGraphId *vid) {
  if (iter->vertCurr == INVALID_ID) return CGRAPH_FALSE;
  *vid = iter->vertCurr;
  iter->vertCurr = iter->view->vertNext[iter->vertCurr];
  return CGRAPH_TRUE;
}

inline CGraphBool cgraphIterNextEdge(CGraphIter *iter, const CGraphId from,
                                     CGraphId *eid, CGraphId *to) {
  CGraphId *curr = iter->edgeCurr + from;
  if (*curr == INVALID_ID) return CGRAPH_FALSE;
  parseForward(iter->view, *curr, eid, to);
  *curr = iter->view->edgeNext[*curr];
  return CGRAPH_TRUE;
}

#endif // ITER_INTERNAL_H
