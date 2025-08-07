#include "graph/iter.h"
#include "private/graph_detail.h"
#include "private/heap.h"
#include "private/pairing_heap.h"
#include <stdlib.h>
#include <string.h>

void PrimMinSpanningTree(const Graph *graph, const WeightType weights[],
                         GraphId predecessor[], const GraphId root) {
  enum { UNVISITED, VISITED, OVER };
  const GraphView *view = VIEW(graph);
  GraphIter *iter = graphIterFromView(view);
  uint8_t *flags = calloc(view->vertRange, sizeof(GraphBool));
  WeightType *minWeight = malloc(view->vertRange * sizeof(WeightType));
  GraphPairingHeap *heap = graphPairingHeapCreate(graph->vertNum, minWeight);
  memset(minWeight, UNREACHABLE_BYTE, view->vertRange * sizeof(WeightType));

  GraphId id, to;
  predecessor[root] = INVALID_ID;
  graphPairingHeapPush(heap, root);
  while (!graphPairingHeapEmpty(heap)) {
    const GraphId from = graphPairingHeapPop(heap);
    flags[from] = OVER;

    while (graphIterNextEdge(iter, from, &id, &to)) {
      uint8_t *flag = flags + to;
      if (*flag != OVER && weights[id] < minWeight[to]) {
        minWeight[to] = weights[id];
        predecessor[to] = from;

        if (*flag == VISITED) {
          graphPairingHeapPush(heap, to);
        } else {
          *flag = VISITED;
          graphPairingHeapUpdate(heap, to);
        }
      }
    }
  }

  free(flags);
  free(minWeight);
  graphIterRelease(iter);
  graphPairingHeapRelease(heap);
}

#include "private/disjoint_set.h"

static void KruskalHeapInit(const GraphView *view, GraphHeap *heap) {
  GraphIter *iter = graphIterFromView(view);
  GraphId from, eid, to;
  while (graphIterNextVert(iter, &from)) {
    while (graphIterNextEdge(iter, from, &eid, &to)) {
      graphHeapPreBuild(heap, eid);
    }
  }
  graphHeapBuild(heap);
  graphIterRelease(iter);
}

void KruskalMinSpanningTree(const Graph *graph, const WeightType weight[],
                            GraphId edges[]) {
  const GraphView *view = VIEW(graph);
  GraphHeap *heap = graphHeapCreate(graph->edgeNum, weight);
  GraphDisjointSet *disjointSet = graphDisjointCreate(graph->vertNum);
  KruskalHeapInit(view, heap);

  GraphSize counter = 0;
  while (!graphHeapEmpty(heap)) {
    const GraphId eid = graphHeapPop(heap);
    const GraphEndpoint *edge = view->endpts + eid;
    const GraphId cls1 = graphDisjointFind(disjointSet, edge->to);
    const GraphId cls2 = graphDisjointFind(disjointSet, edge->from);

    if (cls1 != cls2) {
      edges[counter++] = eid;
      graphDisjointUnion(disjointSet, cls1, cls2);
    }
  }
  if (counter + 1 != graph->vertNum) {
    // No spanning tree
  }

  graphHeapRelease(heap);
  graphDisjointRelease(disjointSet);
}
