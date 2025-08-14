#include "private/iter_internal.h"
#include <string.h>

void cgraphGetIdRange(const CGraph *graph, CGraphSize *vert, CGraphSize *edge) {
  if (vert) *vert = graph->manager.view.vertRange;
  if (edge) *edge = graph->manager.view.edgeRange;
}

void cgraphGetIndegree(const CGraph *const graph, CGraphInt indegree[]) {
  const CGraphView *view = VIEW(graph);
  memset(indegree, 0, view->vertRange * sizeof(CGraphInt));
  FOREACH_EDGE(view, from, eid, to) { ++indegree[to]; }
}