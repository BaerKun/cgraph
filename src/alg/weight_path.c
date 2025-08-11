#include "private/_iter.h"
#include "private/structure/pairing_heap.h"
#include "private/structure/queue.h"
#include <stdlib.h>
#include <string.h>

void DijkstraShortest(const Graph *const graph, const WeightType weights[],
                      GraphId predecessor[], const GraphId source,
                      const GraphId target) {
  enum { NOT_SEEN = 0, IN_HEAP, DONE };
  const GraphView *view = VIEW(graph);
  GraphIter *iter = graphGetIter(graph);
  uint8_t *flags = calloc(view->vertRange, sizeof(uint8_t));
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
    flags[from] = DONE;

    while (graphIterNextEdge(iter, from, &id, &to)) {
      uint8_t *flag = flags + to;
      if (*flag != DONE && distance[from] + weights[id] < distance[to]) {
        distance[to] = distance[from] + weights[id];
        predecessor[to] = from;

        if (*flag == IN_HEAP) {
          graphPairingHeapUpdate(heap, to);
        } else {
          *flag = IN_HEAP;
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
