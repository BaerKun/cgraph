#include "private/iter_internal.h"
#include <stdlib.h>
#include <string.h>

GraphIter *graphIterFromView(const GraphView *view) {
  GraphIter *iter =
      malloc(sizeof(GraphIter) + view->vertRange * sizeof(GraphId));
  iter->view = view;
  iter->vertCurr = view->vertHead;
  memcpy(iter->edgeCurr, view->edgeHead, view->vertRange * sizeof(GraphId));
  return iter;
}

GraphIter *graphGetIter(const Graph *graph) {
  return graphIterFromView(VIEW(graph));
}

void graphIterRelease(GraphIter *iter) { free(iter); }

void graphIterResetAllEdges(GraphIter *iter) {
  const GraphView *view = iter->view;
  memcpy(iter->edgeCurr, view->edgeHead, view->vertRange * sizeof(GraphId));
}

void graphIterCurr(const GraphIter *iter, GraphId *from, GraphId *eid,
                   GraphId *to) {
  *from = iter->vertCurr;
  if (*from == INVALID_ID) return;
  *eid = iter->edgeCurr[*from];
  if (*eid == INVALID_ID) return;
  parseForward(iter->view, *eid, eid, to);
}

extern void graphIterResetVert(GraphIter *iter);
extern void graphIterResetEdge(GraphIter *iter, GraphId from);
extern GraphBool graphIterNextVert(GraphIter *iter, GraphId *vid);
extern GraphBool graphIterNextEdge(GraphIter *iter, GraphId from, GraphId *eid,
                                   GraphId *to);