#include "private/_iter.h"
#include "private/structure/queue.h"

void graphIndegreeInit(const GraphView *view, const GraphInt indegree[],
                       GraphQueue *queue) {
  FOREACH_EDGE(view, from, id, to) {
    if (indegree[to] == 0) graphQueuePush(queue, to);
  }
}
