#include "private/iter_internal.h"
#include "private/structure/stack.h"
#include "private/view.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
  CGraphIter *iter;
  CGraphView *reverse;
  CGraphStack *stack;
  CGraphBool *flag;
  CGraphId *connectionId;
  CGraphId counter;
} Package;

static void forward(Package *pkg, const CGraphId from) {
  CGraphId did, eid, to;
  pkg->flag[from] = 1;
  while (cgraphIterNextDirect(pkg->iter, from, &did)) {
    parseForward(pkg->iter->view, did, &eid, &to);
    if (!pkg->flag[to]) forward(pkg, to);
    cgraphInsertEdge(pkg->reverse, to, REVERSE(did)); // 边转向
  }
  cgraphStackPush(pkg->stack, from);
}

static void backward(Package *pkg, const CGraphId from) {
  CGraphId eid, to;
  pkg->connectionId[from] = pkg->counter;
  pkg->flag[from] = 0;
  while (cgraphIterNextEdge(pkg->iter, from, &eid, &to)) {
    if (pkg->flag[to]) backward(pkg, to);
  }
}

void cgraphFindScc(const CGraph *graph, CGraphId connectionId[]) {
  const CGraphView *view = VIEW(graph);
  CGraphIter *iter = cgraphIterFromView(view);
  CGraphView *reverse = cgraphViewReserveEdge(view, CGRAPH_TRUE);
  CGraphStack *stack = cgraphStackCreate(graph->vertNum);
  CGraphBool *flag = calloc(view->vertRange, sizeof(CGraphBool));
  Package pkg = {iter, reverse, stack, flag, connectionId, 0};
  memset(reverse->edgeHead, INVALID_ID, view->vertRange * sizeof(CGraphId));
  memset(connectionId, INVALID_ID, view->vertRange * sizeof(CGraphId));

  // 正序
  CGraphId from;
  while (cgraphIterNextVert(pkg.iter, &from)) {
    if (flag[from] == 0) forward(&pkg, from);
  }

  // 逆序
  iter->view = reverse;
  cgraphIterResetAllEdges(pkg.iter);
  while (!cgraphStackEmpty(stack)) {
    const CGraphId vert = cgraphStackPop(stack);
    if (flag[vert] == 1) backward(&pkg, vert);
    ++pkg.counter;
  }

  free(flag);
  free(reverse);
  cgraphIterRelease(iter);
  cgraphStackRelease(stack);
}
