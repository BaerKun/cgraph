#include "private/iter_internal.h"
#include "private/structure/queue.h"

void graphIndegreeInit(const CGraphView *view, const CGraphInt indegree[],
                       CGraphQueue *queue) {
  FOREACH_EDGE(view, from, id, to) {
    if (indegree[to] == 0) cgraphQueuePush(queue, to);
  }
}
