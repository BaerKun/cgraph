#include "private/iter_internal.h"
#include "private/structure/pairing_heap.h"
#include <stdlib.h>
#include <string.h>

void PrimMinSpanningTree(const Graph *graph, const WeightType weights[],
                         GraphId predecessor[], const GraphId root) {
  enum { NOT_SEEN = 0, IN_HEAP, DONE };
  const GraphView *view = VIEW(graph);
  GraphIter *iter = graphIterFromView(view);
  uint8_t *flags = calloc(view->vertRange, sizeof(uint8_t));
  WeightType *minWeight = malloc(view->vertRange * sizeof(WeightType));
  GraphPairingHeap *heap = graphPairingHeapCreate(graph->vertNum, minWeight);
  memset(minWeight, UNREACHABLE_BYTE, view->vertRange * sizeof(WeightType));

  GraphId id, to;
  predecessor[root] = INVALID_ID;
  graphPairingHeapPush(heap, root);
  while (!graphPairingHeapEmpty(heap)) {
    const GraphId from = graphPairingHeapPop(heap);
    flags[from] = DONE;

    while (graphIterNextEdge(iter, from, &id, &to)) {
      uint8_t *flag = flags + to;
      if (*flag != DONE && weights[id] < minWeight[to]) {
        minWeight[to] = weights[id];
        predecessor[to] = from;

        if (*flag == IN_HEAP) {
          graphPairingHeapPush(heap, to);
        } else {
          *flag = IN_HEAP;
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

#include "private/structure/disjoint_set.h"
#include "private/structure/heap.h"

static void KruskalHeapInit(const GraphView *view, GraphHeap *heap) {
  GraphBool *isInHeap = calloc(view->edgeRange, sizeof(GraphBool));

  FOREACH_EDGE(view, from, eid, to) {
    // 去除反向边
    if (!isInHeap[eid]) {
      isInHeap[eid] = GRAPH_TRUE;
      graphHeapPreBuild(heap, eid);
    }
  }
  free(isInHeap);
  graphHeapBuild(heap);
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
  if (counter != graph->vertNum - 1) {
    // No spanning tree
  }

  graphHeapRelease(heap);
  graphDisjointRelease(disjointSet);
}
