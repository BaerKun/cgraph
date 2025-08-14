#ifndef GRAPH_VIEW_H
#define GRAPH_VIEW_H

#include "graph_detail.h"

CGraphView *cgraphViewReserveEdge(const CGraphView *view, CGraphBool directed);
void cgraphViewCopyEdge(const CGraphView *view, const CGraphView *copy);

CGraphId *cgraphFind(CGraphId *next, CGraphId *head, CGraphId id);

static inline void cgraphUnlink(const CGraphId *next, CGraphId *predNext) {
  *predNext = next[*predNext];
}

static inline void cgraphInsert(CGraphId *next, CGraphId *predNext,
                                const CGraphId id) {
  next[id] = *predNext;
  *predNext = id;
}

static inline void cgraphInsertEdge(const CGraphView *view, const CGraphId from,
                                    const CGraphId did) {
  cgraphInsert(view->edgeNext, view->edgeHead + from, did);
}

#endif // GRAPH_VIEW_H
