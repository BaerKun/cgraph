#include "private/iter_internal.h"
#include "private/structure/pairing_heap.h"
#include <stdlib.h>
#include <string.h>

void cgraphMSTPrim(const CGraph *graph, const WeightType weights[],
                   CGraphId predecessor[], const CGraphId root) {
  enum { NOT_SEEN = 0, IN_HEAP, DONE };
  const CGraphView *view = VIEW(graph);
  CGraphIter *iter = cgraphIterFromView(view);
  uint8_t *flags = calloc(view->vertRange, sizeof(uint8_t));
  WeightType *minWeight = malloc(view->vertRange * sizeof(WeightType));
  CGraphPairingHeap *heap = cgraphPairingHeapCreate(graph->vertNum, minWeight);
  memset(minWeight, UNREACHABLE_BYTE, view->vertRange * sizeof(WeightType));

  CGraphId eid, to;
  predecessor[root] = INVALID_ID;
  cgraphPairingHeapPush(heap, root);
  while (!cgraphPairingHeapEmpty(heap)) {
    const CGraphId from = cgraphPairingHeapPop(heap);
    flags[from] = DONE;

    while (cgraphIterNextEdge(iter, from, &eid, &to)) {
      uint8_t *flag = flags + to;
      if (*flag != DONE && weights[eid] < minWeight[to]) {
        minWeight[to] = weights[eid];
        predecessor[to] = from;

        if (*flag == IN_HEAP) {
          cgraphPairingHeapPush(heap, to);
        } else {
          *flag = IN_HEAP;
          cgraphPairingHeapUpdate(heap, to);
        }
      }
    }
  }

  free(flags);
  free(minWeight);
  cgraphIterRelease(iter);
  cgraphPairingHeapRelease(heap);
}

#include "private/structure/disjoint_set.h"
#include "private/structure/heap.h"

static void KruskalHeapInit(const CGraphView *view, CGraphHeap *heap) {
  CGraphBool *isInHeap = calloc(view->edgeRange, sizeof(CGraphBool));

  FOREACH_EDGE(view, from, eid, to) {
    // 去除反向边
    if (!isInHeap[eid]) {
      isInHeap[eid] = CGRAPH_TRUE;
      cgraphHeapPreBuild(heap, eid);
    }
  }
  free(isInHeap);
  cgraphHeapBuild(heap);
}

void cgraphMSTKruskal(const CGraph *graph, const WeightType weight[],
                      CGraphId edges[]) {
  const CGraphView *view = VIEW(graph);
  CGraphHeap *heap = cgraphHeapCreate(graph->edgeNum, weight);
  CGraphDisjointSet *disjointSet = cgraphDisjointCreate(graph->vertNum);
  KruskalHeapInit(view, heap);

  CGraphSize counter = 0;
  while (!cgraphHeapEmpty(heap)) {
    const CGraphId eid = cgraphHeapPop(heap);
    const CGraphEndpoint *edge = view->endpts + eid;
    const CGraphId cls1 = cgraphDisjointFind(disjointSet, edge->to);
    const CGraphId cls2 = cgraphDisjointFind(disjointSet, edge->from);

    if (cls1 != cls2) {
      edges[counter++] = eid;
      cgraphDisjointUnion(disjointSet, cls1, cls2);
    }
  }
  if (counter != graph->vertNum - 1) {
    // No spanning tree
  }

  cgraphHeapRelease(heap);
  cgraphDisjointRelease(disjointSet);
}
