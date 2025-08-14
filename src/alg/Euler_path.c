#include "cgraph/linked_list.h"
#include "private/iter_internal.h"
#include <stdlib.h>

// 递归实现
typedef struct {
  CGraphIter *iter;
  CGraphBool *visited;
  CGraphId dfsDst; // 当前深度优先搜索的目标顶点
} Package;         // 递归全局量

static CGraphBool getTargetEdge(const Package *pkg, const CGraphId from,
                                CGraphId *id, CGraphId *to) {
  while (cgraphIterNextEdge(pkg->iter, from, id, to)) {
    if (!pkg->visited[*id]) {
      pkg->visited[*id] = 1;
      return CGRAPH_TRUE;
    }
  }
  return CGRAPH_FALSE;
}

static CGraphBool EulerPath_recursive(Package *pkg,
                                      CGraphLinkedNode **const pred,
                                      const CGraphId from) {
  CGraphId id, to;
  while (1) {
    if (!getTargetEdge(pkg, from, &id, &to)) break;
    CGraphLinkedNode *const path = cgraphLinkedInsert(pred, id);
    if (!EulerPath_recursive(pkg, &path->next, to)) return CGRAPH_FALSE;
    pkg->dfsDst = from;
  }
  return from == pkg->dfsDst;
}

// 栈实现
typedef struct {
  CGraphLinkedNode **pred;
  CGraphId from;
} Argument; // 入栈量，与递归相对

static CGraphBool EulerPath_stack(Package *pkg, CGraphLinkedNode **const head,
                                  const CGraphId src,
                                  const CGraphSize edgeNum) {
  Argument *const stack = malloc(edgeNum * sizeof(Argument));
  Argument *ptr = stack; // 当前"函数"参数
  CGraphBool success = CGRAPH_FALSE;
  *ptr = (Argument){head, src};

  CGraphId id, to;
  while (1) {
    if (!getTargetEdge(pkg, ptr->from, &id, &to)) {
      if (ptr->from != pkg->dfsDst) break;

      // 结束
      if (ptr-- == stack) {
        success = CGRAPH_TRUE;
        break;
      }

      pkg->dfsDst = ptr->from;
      continue;
    }
    CGraphLinkedNode *const path = cgraphLinkedInsert(ptr->pred, id);

    // 调用
    *++ptr = (Argument){&path->next, to};
  }

  free(stack);
  return success;
}

void cgraphEulerPath(const CGraph *const graph, CGraphLinkedNode **const path,
                     const CGraphId src, const CGraphId dst) {
  const CGraphView *view = VIEW(graph);
  Package pkg;
  pkg.iter = cgraphIterFromView(view);
  pkg.visited = calloc(view->vertRange, sizeof(CGraphBool));
  pkg.dfsDst = dst;

  // clang-format off
  if (!
    // EulerPath_recursive(&pkg, path, src)
    EulerPath_stack(&pkg, path, src, graph->edgeNum)
  ) {
    // clang-format on
    cgraphLinkedClear(path);
  }

  free(pkg.visited);
  cgraphIterRelease(pkg.iter);
}

void cgraphEulerCircuit(const CGraph *const graph,
                        CGraphLinkedNode **const path, const CGraphId src) {
  cgraphEulerPath(graph, path, src, src);
}
