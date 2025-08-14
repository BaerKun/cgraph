#include "private/iter_internal.h"
#include "private/structure/pairing_heap.h"
#include "private/structure/queue.h"
#include <stdlib.h>
#include <string.h>

void cgraphShortestDijkstra(const CGraph *const graph,
                            const WeightType weights[], CGraphId predecessor[],
                            const CGraphId source, const CGraphId target) {
  enum { NOT_SEEN = 0, IN_HEAP, DONE };
  const CGraphView *view = VIEW(graph);
  CGraphIter *iter = cgraphIterFromView(view);
  uint8_t *flags = calloc(view->vertRange, sizeof(uint8_t));
  WeightType *distance = malloc(view->vertRange * sizeof(WeightType));
  CGraphPairingHeap *heap = cgraphPairingHeapCreate(graph->vertNum, distance);
  memset(distance, UNREACHABLE_BYTE, view->vertRange * sizeof(WeightType));
  memset(predecessor, INVALID_ID, view->vertRange * sizeof(CGraphId));

  CGraphId id, to;
  distance[source] = 0;
  cgraphPairingHeapPush(heap, source);
  while (!cgraphPairingHeapEmpty(heap)) {
    const CGraphId from = cgraphPairingHeapPop(heap);
    if (from == target) break;
    flags[from] = DONE;

    while (cgraphIterNextEdge(iter, from, &id, &to)) {
      uint8_t *flag = flags + to;
      if (*flag != DONE && distance[from] + weights[id] < distance[to]) {
        distance[to] = distance[from] + weights[id];
        predecessor[to] = from;

        if (*flag == IN_HEAP) {
          cgraphPairingHeapUpdate(heap, to);
        } else {
          *flag = IN_HEAP;
          cgraphPairingHeapPush(heap, to);
        }
      }
    }
  }

  free(flags);
  free(distance);
  cgraphIterRelease(iter);
  cgraphPairingHeapRelease(heap);
}

// 无负值圈
void cgraphShortestBellmanFord(const CGraph *const graph,
                               const WeightType weights[],
                               CGraphId predecessor[], const CGraphId source) {
  const CGraphView *view = VIEW(graph);
  CGraphIter *iter = cgraphIterFromView(view);
  CGraphQueue *queue = cgraphNewQueue(graph->vertNum);
  CGraphBool *isInQueue = calloc(view->vertRange, sizeof(CGraphBool));
  WeightType *distance = malloc(view->vertRange * sizeof(WeightType));
  memset(distance, UNREACHABLE_BYTE, view->vertRange * sizeof(WeightType));
  memset(predecessor, INVALID_ID, view->vertRange * sizeof(CGraphId));

  CGraphId id, to;
  distance[source] = 0;
  // isInQueue[source] = GRAPH_TRUE;
  cgraphQueuePush(queue, source);
  while (!cgraphQueueEmpty(queue)) {
    const CGraphId from = cgraphQueuePop(queue);
    isInQueue[from] = CGRAPH_FALSE;

    while (cgraphIterNextEdge(iter, from, &id, &to)) {
      if (distance[to] <= distance[from] + weights[id]) continue;

      distance[to] = distance[from] + weights[id];
      predecessor[to] = from;

      if (!isInQueue[to]) {
        cgraphQueuePush(queue, to);
        cgraphIterResetEdge(iter, to);
        isInQueue[to] = CGRAPH_TRUE;
      }
    }
  }

  free(isInQueue);
  free(distance);
  cgraphIterRelease(iter);
  cgraphQueueRelease(queue);
}
