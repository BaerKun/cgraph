#include "private/_iter.h"
#include <string.h>

void graphGetIdRange(const Graph *graph, GraphSize *vert, GraphSize *edge) {
  if (vert) *vert = graph->manager.view.vertRange;
  if (edge) *edge = graph->manager.view.edgeRange;
}

void graphGetIndegree(const Graph *const graph, GraphInt indegree[]) {
  const GraphView *view = VIEW(graph);
  memset(indegree, 0, view->vertRange * sizeof(GraphInt));
  FOREACH_EDGE(view, from, eid, to) { ++indegree[to]; }
}