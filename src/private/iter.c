#include "private/iter_internal.h"
#include <stdlib.h>
#include <string.h>

CGraphIter *cgraphIterFromView(const CGraphView *view) {
  CGraphIter *iter =
      malloc(sizeof(CGraphIter) + view->vertRange * sizeof(CGraphId));
  iter->view = view;
  iter->vertCurr = view->vertHead;
  memcpy(iter->edgeCurr, view->edgeHead, view->vertRange * sizeof(CGraphId));
  return iter;
}

CGraphIter *cgraphGetIter(const CGraph *graph) {
  return cgraphIterFromView(VIEW(graph));
}

void cgraphIterRelease(CGraphIter *iter) { free(iter); }

void cgraphIterResetAllEdges(CGraphIter *iter) {
  const CGraphView *view = iter->view;
  memcpy(iter->edgeCurr, view->edgeHead, view->vertRange * sizeof(CGraphId));
}

void cgraphIterCurr(const CGraphIter *iter, CGraphId *from, CGraphId *eid,
                    CGraphId *to) {
  *from = iter->vertCurr;
  if (*from == INVALID_ID) return;
  *eid = iter->edgeCurr[*from];
  if (*eid == INVALID_ID) return;
  parseForward(iter->view, *eid, eid, to);
}

extern void cgraphIterResetVert(CGraphIter *iter);
extern void cgraphIterResetEdge(CGraphIter *iter, CGraphId from);
extern CGraphBool cgraphIterNextVert(CGraphIter *iter, CGraphId *vid);
extern CGraphBool cgraphIterNextEdge(CGraphIter *iter, CGraphId from,
                                     CGraphId *eid, CGraphId *to);