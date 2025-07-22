#include "graph/iter.h"
#include "private/graph_detail.h"
#include "private/pairing_heap.h"
#include "private/queue.h"
#include <stdlib.h>
#include <string.h>

void DijkstraShortest(const Graph *const graph, const WeightType weights[],
                      GraphId predecessor[], const GraphId source,
                      const GraphId target) {
  enum { UNVISITED, VISITED, OVER };
  const GraphView *view = VIEW(graph);
  GraphIter *iter = graphGetIter(graph);
  uint8_t *flags = calloc(view->vertRange, sizeof(GraphBool));
  WeightType *distance = malloc(view->vertRange * sizeof(WeightType));
  GraphPairingHeap *heap = graphPairingHeapCreate(graph->vertNum, distance);
  memset(distance, UNREACHABLE_BYTE, view->vertRange * sizeof(WeightType));
  memset(predecessor, INVALID_ID, view->vertRange * sizeof(GraphId));

  GraphId id, to;
  distance[source] = 0;
  graphPairingHeapPush(heap, source);
  while (!graphPairingHeapEmpty(heap)) {
    const GraphId from = graphPairingHeapPop(heap);
    if (from == target) break;
    flags[from] = OVER;

    while (graphIterNextEdge(iter, from, &id, &to)) {
      uint8_t *flag = flags + to;
      if (*flag != OVER && distance[from] + weights[id] < distance[to]) {
        distance[to] = distance[from] + weights[id];
        predecessor[to] = from;

        if (*flag == VISITED) {
          graphPairingHeapUpdate(heap, to);
        } else {
          *flag = VISITED;
          graphPairingHeapPush(heap, to);
        }
      }
    }
  }

  free(flags);
  free(distance);
  graphIterRelease(iter);
  graphPairingHeapRelease(heap);
}

// 无负值圈
void BellmanFordShortest(const Graph *const graph, const WeightType weights[],
                         GraphId predecessor[], const GraphId source) {
  const GraphView *view = VIEW(graph);
  GraphIter *iter = graphGetIter(graph);
  GraphQueue *queue = graphNewQueue(graph->vertNum);
  GraphBool *isInQueue = calloc(view->vertRange, sizeof(GraphBool));
  WeightType *distance = malloc(view->vertRange * sizeof(WeightType));
  memset(distance, UNREACHABLE_BYTE, view->vertRange * sizeof(WeightType));
  memset(predecessor, INVALID_ID, view->vertRange * sizeof(GraphId));

  GraphId id, to;
  distance[source] = 0;
  // isInQueue[source] = GRAPH_TRUE;
  graphQueuePush(queue, source);
  while (!graphQueueEmpty(queue)) {
    const GraphId from = graphQueuePop(queue);
    isInQueue[from] = GRAPH_FALSE;

    while (graphIterNextEdge(iter, from, &id, &to)) {
      if (distance[to] <= distance[from] + weights[id]) continue;

      distance[to] = distance[from] + weights[id];
      predecessor[to] = from;

      if (!isInQueue[to]) {
        graphQueuePush(queue, to);
        graphIterResetEdge(iter, to);
        isInQueue[to] = GRAPH_TRUE;
      }
    }
  }

  free(isInQueue);
  free(distance);
  graphIterRelease(iter);
  graphQueueRelease(queue);
}
