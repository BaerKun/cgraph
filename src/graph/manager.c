#include "private/manager.h"
#include "private/view.h"
#include <stdlib.h>
#include <string.h>

void cgraphManagerInit(CGraphManager *mgr, const CGraphBool directed,
                       const CGraphSize vertCap, const CGraphSize edgeCap) {
  mgr->vertFree = mgr->edgeFree = 0;

  CGraphView *const view = &mgr->view;
  view->vertRange = 0;
  view->vertHead = INVALID_ID;
  view->vertNext = malloc(vertCap * sizeof(CGraphId));
  view->vertNext[vertCap - 1] = INVALID_ID;
  for (CGraphId i = (CGraphId)vertCap - 1; i; --i) view->vertNext[i - 1] = i;

  view->directed = directed;
  view->edgeRange = 0;
  view->edgeHead = malloc(vertCap * sizeof(CGraphId));
  memset(view->edgeHead, INVALID_ID, vertCap * sizeof(CGraphId));
  view->endpts = malloc(edgeCap * sizeof(CGraphEndpoint));

  if (directed) {
    view->edgeNext = malloc(edgeCap * sizeof(CGraphId));
    view->edgeNext[edgeCap - 1] = INVALID_ID;
    for (CGraphId i = (CGraphId)edgeCap - 1; i; --i) view->edgeNext[i - 1] = i;
  } else {
    view->edgeNext = malloc(2 * edgeCap * sizeof(CGraphId));
    view->edgeNext[2 * (edgeCap - 1)] = INVALID_ID;
    for (CGraphId i = 2 * ((CGraphId)edgeCap - 1); i; i -= 2)
      view->edgeNext[i - 2] = i;
  }
}

void cgraphManagerDestroy(const CGraphManager *mgr) {
  free(mgr->view.vertNext);
  free(mgr->view.edgeHead);
  free(mgr->view.edgeNext);
  free(mgr->view.endpts);
}

CGraphId cgraphManagerNewVert(CGraphManager *mgr) {
  CGraphView *view = &mgr->view;
  const CGraphId vid = mgr->vertFree;
  cgraphUnlink(view->vertNext, &mgr->vertFree);
  cgraphInsert(view->vertNext, &view->vertHead, vid);
  if (vid == view->vertRange) ++view->vertRange;
  return vid;
}

CGraphId cgraphManagerNewEdge(CGraphManager *mgr, const CGraphId from,
                              const CGraphId to, const CGraphBool directed) {
  CGraphView *view = &mgr->view;
  const CGraphId did = mgr->edgeFree;
  cgraphUnlink(view->edgeNext, &mgr->edgeFree);
  cgraphInsertEdge(view, from, did);
  if (!directed) cgraphInsertEdge(view, to, REVERSE(did));

  const CGraphId eid = view->directed ? did : (did >> 1);
  view->endpts[eid] = (CGraphEndpoint){.to = to, .from = from};
  if (eid == view->edgeRange) ++view->edgeRange;
  return eid;
}

void cgraphManagerDeleteVert(CGraphManager *mgr, const CGraphId vid) {
  CGraphView *view = &mgr->view;

  CGraphId *predNext = cgraphFind(view->vertNext, &view->vertHead, vid);
  cgraphUnlink(view->vertNext, predNext);
  cgraphInsert(view->vertNext, &mgr->vertFree, vid);
  if (vid == view->vertRange - 1) view->vertRange = vid;
}

void cgraphManagerDeleteEdge(CGraphManager *mgr, const CGraphId eid) {
  CGraphView *view = &mgr->view;
  const CGraphId did = view->directed ? eid : (eid << 1);
  const CGraphId to = view->endpts[eid].to;
  const CGraphId from = view->endpts[eid].from;

  CGraphId *predNext = cgraphFind(view->edgeNext, view->edgeHead + from, did);
  cgraphUnlink(view->edgeNext, predNext);
  cgraphInsert(view->edgeNext, &mgr->edgeFree, did);
  if (!view->directed) {
    predNext = cgraphFind(view->edgeNext, view->edgeHead + to, REVERSE(did));
    if (predNext) cgraphUnlink(view->edgeNext, predNext);
  }
  if (eid == view->edgeRange - 1) view->edgeRange = eid;
}