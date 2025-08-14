#include "private/iter_internal.h"
#include "private/structure/queue.h"
#include <stdlib.h>
#include <string.h>

void cgraphIndegreeInit(const CGraphView *view, const CGraphInt indegree[],
                        CGraphQueue *queue);

void cgraphTopoPath(const CGraph *const graph, const CGraphInt indegree[],
                    CGraphId predecessor[]) {
  const CGraphView *view = VIEW(graph);
  CGraphIter *iter = cgraphIterFromView(view);
  CGraphQueue *queue = cgraphQueueCreate(graph->vertNum);
  cgraphIndegreeInit(view, indegree, queue);
  CGraphInt *copyIndeg = malloc(view->vertRange * sizeof(CGraphInt));
  memcpy(copyIndeg, indegree, view->vertRange * sizeof(CGraphInt));
  memset(predecessor, INVALID_ID, view->vertRange * sizeof(CGraphId));

  CGraphInt counter = 0;
  CGraphId id, to;
  while (!cgraphQueueEmpty(queue)) {
    const CGraphId from = cgraphQueuePop(queue);
    ++counter;
    while (cgraphIterNextEdge(iter, from, &id, &to)) {
      if (predecessor[to] == -1) predecessor[to] = from;
      if (--copyIndeg[to] == 0) cgraphQueuePush(queue, to);
    }
  }

  if (counter != graph->vertNum) { /* ERROR: 圈 */
  }

  free(copyIndeg);
  cgraphIterRelease(iter);
  cgraphQueueRelease(queue);
}

void cgraphTopoSort(const CGraph *const graph, const CGraphInt indegree[],
                    CGraphId sort[]) {
  const CGraphView *view = VIEW(graph);
  CGraphIter *iter = cgraphIterFromView(view);
  CGraphQueue *queue = cgraphQueueCreate(graph->vertNum);
  cgraphIndegreeInit(view, indegree, queue);
  CGraphInt *copyIndeg = malloc(view->vertRange * sizeof(CGraphInt));
  memcpy(copyIndeg, indegree, view->vertRange * sizeof(CGraphInt));

  CGraphInt counter = 0;
  CGraphId id, to;
  while (!cgraphQueueEmpty(queue)) {
    const CGraphId from = cgraphQueuePop(queue);
    sort[counter++] = from;

    while (cgraphIterNextEdge(iter, from, &id, &to)) {
      if (--copyIndeg[to] == 0) cgraphQueuePush(queue, to);
    }
  }

  if (counter != graph->vertNum) { /* ERROR: 圈 */
  }

  free(copyIndeg);
  cgraphIterRelease(iter);
  cgraphQueueRelease(queue);
}
