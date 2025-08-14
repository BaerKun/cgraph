#include "cgraph/alg.h"
#include "cgraph/graph.h"
#include "cgraph/iter.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  CGraphId from, to;
  CGraphBool directed;
} EdgePack;

void graphAddEdges(CGraph *graph, const CGraphSize num, EdgePack edges[]) {
  for (CGraphSize i = 0; i < num; ++i) {
    cgraphAddEdge(graph, edges[i].from, edges[i].to, edges[i].directed);
  }
}

int testIter() {
  CGraph *graph = malloc(cgraphGetGraphSize());
  cgraphInit(graph, CGRAPH_FALSE, 10, 10);

  cgraphReserveVert(graph, 5);
  cgraphAddEdge(graph, 0, 1, CGRAPH_FALSE);
  cgraphAddEdge(graph, 1, 2, CGRAPH_TRUE);
  cgraphAddEdge(graph, 2, 3, CGRAPH_TRUE);
  cgraphDeleteEdge(graph, 0);
  cgraphAddEdge(graph, 3, 4, CGRAPH_FALSE);
  cgraphDeleteEdge(graph, 2);
  cgraphAddEdge(graph, 4, 0, CGRAPH_TRUE);

  CGraphIter *iter = cgraphGetIter(graph);
  CGraphId from, to, edge;
  printf("from\teid\t\tto\n");
  while (cgraphIterNextVert(iter, &from)) {
    while (cgraphIterNextEdge(iter, from, &edge, &to)) {
      printf("%lld\t\t%lld\t\t%lld\n", from, to, edge);
    }
  }
  cgraphIterRelease(iter);
  cgraphDestroy(graph);
  return 0;
}

int testMaxFlow() {
  CGraph *graph = malloc(cgraphGetGraphSize());
  cgraphInit(graph, CGRAPH_TRUE, 10, 15);

  cgraphReserveVert(graph, 6);
  graphAddEdges(graph, 9,
                (EdgePack[]){{0, 1, true},
                             {0, 3, true},
                             {1, 3, true},
                             {1, 2, true},
                             {4, 1, true},
                             {3, 4, true},
                             {4, 2, true},
                             {2, 5, true},
                             {4, 5, true}});

  const FlowType capacity[9] = {3, 5, 1, 4, 2, 2, 1, 5, 2};
  FlowType flow[9];
  const FlowType maxFlow = cgraphMaxFlowEdmondsKarp(graph, capacity, flow, 0, 5);

  printf("max flow: %lld\n", maxFlow);
  for (CGraphSize i = 0; i < 9; ++i) {
    printf("%lld ", flow[i]);
  }
  putchar('\n');

  cgraphDestroy(graph);
  return 0;
}

int testWeightedPath() {
  CGraph *graph = malloc(cgraphGetGraphSize());
  cgraphInit(graph, CGRAPH_TRUE, 10, 15);

  cgraphReserveVert(graph, 7);
  graphAddEdges(graph, 12,
                (EdgePack[]){{0, 1, true},
                             {0, 2, true},
                             {0, 3, true},
                             {1, 4, true},
                             {2, 1, true},
                             {1, 2, true},
                             {2, 3, true},
                             {3, 5, true},
                             {2, 4, true},
                             {2, 5, true},
                             {4, 6, true},
                             {5, 6, true}});

  const WeightType weights[12] = {11, 9, 12, 9, 4, 3, 5, 7, 13, 12, 15, 14};
  CGraphId predecessor[7];

  cgraphShortestDijkstra(graph, weights, predecessor, 0, 6);
  printf("Weighted Shortest Path\nDijkstra: 6");
  for (CGraphId i = predecessor[6]; i != -1; i = predecessor[i]) {
    printf(" <- %lld", i);
  }

  cgraphShortestBellmanFord(graph, weights, predecessor, 0);
  printf("\nBellmanFord: 6");
  for (CGraphId i = predecessor[6]; i != -1; i = predecessor[i]) {
    printf(" <- %lld", i);
  }
  putchar('\n');

  cgraphDestroy(graph);
  return 0;
}

int main() {
  testIter();
  testMaxFlow();
  testWeightedPath();
}