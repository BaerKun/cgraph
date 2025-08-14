#include "private/attribute.h"
#include "private/manager.h"
#include <stdlib.h>

CGraphSize cgraphGetGraphSize() { return sizeof(CGraph); }

void cgraphInit(CGraph *const graph, const CGraphBool directed,
                const CGraphSize vertCap, const CGraphSize edgeCap) {
  graph->vertCap = vertCap;
  graph->edgeCap = edgeCap;
  graph->edgeNum = graph->vertNum = 0;
  cgraphManagerInit(&graph->manager, directed, vertCap, edgeCap);
  graph->vertAttr = graph->edgeAttr = NULL;
}

void cgraphDestroy(const CGraph *const graph) {
  cgraphManagerDestroy(&graph->manager);
  cgraphAttrDestroyList(graph->vertAttr);
  cgraphAttrDestroyList(graph->edgeAttr);
}

CGraphId cgraphAddVert(CGraph *const graph) {
  if (graph->vertNum++ == graph->vertCap) {
    // realloc
  }
  return cgraphManagerNewVert(&graph->manager);
}

void cgraphReserveVert(CGraph *graph, const CGraphSize num) {
  graph->vertNum += num;
  if (graph->vertNum > graph->vertCap) {
    // realloc
  }
  for (CGraphSize i = 0; i != num; ++i) {
    cgraphManagerNewVert(&graph->manager);
  }
}

CGraphId cgraphAddEdge(CGraph *const graph, const CGraphId from,
                       const CGraphId to, const CGraphBool directed) {
  if (graph->edgeNum++ == graph->edgeCap) {
    // realloc
  }
  return cgraphManagerNewEdge(&graph->manager, from, to, directed);
}

void cgraphDeleteVert(CGraph *graph, const CGraphId id) {
  cgraphManagerDeleteVert(&graph->manager, id);
  --graph->vertNum;
}

void cgraphDeleteEdge(CGraph *graph, const CGraphId id) {
  cgraphManagerDeleteEdge(&graph->manager, id);
  --graph->edgeNum;
}