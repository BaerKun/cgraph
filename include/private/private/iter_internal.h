#ifndef ITER_INTERNAL_H
#define ITER_INTERNAL_H

#include "graph_detail.h"

#define VIEW(graph) (&(graph)->manager.view)

#define FOREACH_EDGE(view, from, eid, to)                                      \
  for (GraphId from = view->vertHead; from != INVALID_ID;                      \
       from = view->vertNext[from])                                            \
    for (GraphId did_ = view->edgeHead[from], eid, to; did_ != INVALID_ID;     \
         did_ = view->edgeNext[did_])                                          \
      if (parseForward(view, did_, &eid, &to), 1)

GraphIter *graphIterFromView(const GraphView *view);
void graphIterRelease(GraphIter *iter);
void graphIterResetAllEdges(GraphIter *iter);

inline void graphIterResetVert(GraphIter *iter) {
  iter->vertCurr = iter->view->vertHead;
}

inline void graphIterResetEdge(GraphIter *iter, const GraphId from) {
  if (from == INVALID_ID) return graphIterResetAllEdges(iter);
  iter->edgeCurr[from] = iter->view->edgeHead[from];
}

static inline void parseForward(const GraphView *view, const GraphId did,
                                GraphId *eid, GraphId *to) {
  // 高度重复可预测，保留分支版本
  if (view->directed) {
    *eid = did;
    *to = view->endpts[did].to;
  } else {
    *eid = did >> 1;
    *to = ((GraphId *)view->endpts)[did];
  }
}

static inline void parseBackward(const GraphView *view, const GraphId did,
                                 GraphId *eid, GraphId *from) {
  if (view->directed) {
    *eid = did;
    *from = view->endpts[did].from;
  } else {
    *eid = did >> 1;
    *from = ((GraphId *)view->endpts)[REVERSE(did)];
  }
}

static inline GraphBool graphIterNextDirect(GraphIter *iter, const GraphId from,
                                            GraphId *did) {
  GraphId *curr = iter->edgeCurr + from;
  if (*curr == INVALID_ID) return GRAPH_FALSE;
  *did = *curr;
  *curr = iter->view->edgeNext[*curr];
  return GRAPH_TRUE;
}

inline GraphBool graphIterNextVert(GraphIter *iter, GraphId *vid) {
  if (iter->vertCurr == INVALID_ID) return GRAPH_FALSE;
  *vid = iter->vertCurr;
  iter->vertCurr = iter->view->vertNext[iter->vertCurr];
  return GRAPH_TRUE;
}

inline GraphBool graphIterNextEdge(GraphIter *iter, const GraphId from,
                                   GraphId *eid, GraphId *to) {
  GraphId *curr = iter->edgeCurr + from;
  if (*curr == INVALID_ID) return GRAPH_FALSE;
  parseForward(iter->view, *curr, eid, to);
  *curr = iter->view->edgeNext[*curr];
  return GRAPH_TRUE;
}

#endif // ITER_INTERNAL_H
