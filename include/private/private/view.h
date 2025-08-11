#ifndef GRAPH_VIEW_H
#define GRAPH_VIEW_H

#include "graph_detail.h"

GraphView *graphViewReserveEdge(const GraphView *view, GraphBool directed);
void graphViewCopyEdge(const GraphView *view, const GraphView *copy);

GraphId *graphFind(GraphId *next, GraphId *head, GraphId id);

static inline void graphUnlink(const GraphId *next, GraphId *predNext) {
  *predNext = next[*predNext];
}

static inline void graphInsert(GraphId *next, GraphId *predNext,
                               const GraphId id) {
  next[id] = *predNext;
  *predNext = id;
}

static inline void graphInsertEdge(const GraphView *view, const GraphId from,
                                   const GraphId did) {
  graphInsert(view->edgeNext, view->edgeHead + from, did);
}

#endif // GRAPH_VIEW_H
