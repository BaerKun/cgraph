#include "graph/alg.h"
#include "graph/graph.h"
#include "graph/iter.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  GraphId from, to;
  GraphBool directed;
} EdgePack;

void graphAddEdges(Graph *graph, const GraphSize num, EdgePack edges[]) {
  for (GraphSize i = 0; i < num; ++i) {
    graphAddEdge(graph, edges[i].from, edges[i].to, edges[i].directed);
  }
}

int testIter() {
  Graph *graph = malloc(graphGetGraphSize());
  graphInit(graph, GRAPH_FALSE, 10, 10);

  graphReserveVert(graph, 5);
  graphAddEdge(graph, 0, 1, GRAPH_FALSE);
  graphAddEdge(graph, 1, 2, GRAPH_TRUE);
  graphAddEdge(graph, 2, 3, GRAPH_TRUE);
  graphDeleteEdge(graph, 0);
  graphAddEdge(graph, 3, 4, GRAPH_FALSE);
  graphDeleteEdge(graph, 2);
  graphAddEdge(graph, 4, 0, GRAPH_TRUE);

  GraphIter *iter = graphGetIter(graph);
  GraphId from, to, edge;
  printf("from\teid\t\tto\n");
  while (graphIterNextVert(iter, &from)) {
    while (graphIterNextEdge(iter, from, &edge, &to)) {
      printf("%lld\t\t%lld\t\t%lld\n", from, to, edge);
    }
  }
  graphIterRelease(iter);
  graphDestroy(graph);
  return 0;
}

int testMaxFlow() {
  Graph *graph = malloc(graphGetGraphSize());
  graphInit(graph, GRAPH_TRUE, 10, 15);

  graphReserveVert(graph, 6);
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
  const FlowType maxFlow = EdmondsKarpMaxFlow(graph, capacity, flow, 0, 5);

  printf("max flow: %lld\n", maxFlow);
  for (GraphSize i = 0; i < 9; ++i) {
    printf("%lld ", flow[i]);
  }
  putchar('\n');

  graphDestroy(graph);
  return 0;
}

int testWeightedPath() {
  Graph *graph = malloc(graphGetGraphSize());
  graphInit(graph, GRAPH_TRUE, 10, 15);

  graphReserveVert(graph, 7);
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
  GraphId predecessor[7];

  DijkstraShortest(graph, weights, predecessor, 0, 6);
  printf("Weighted Shortest Path\nDijkstra: 6");
  for (GraphId i = predecessor[6]; i != -1; i = predecessor[i]) {
    printf(" <- %lld", i);
  }

  BellmanFordShortest(graph, weights, predecessor, 0);
  printf("\nBellmanFord: 6");
  for (GraphId i = predecessor[6]; i != -1; i = predecessor[i]) {
    printf(" <- %lld", i);
  }
  putchar('\n');

  graphDestroy(graph);
  return 0;
}

int main() {
  testIter();
  testMaxFlow();
  testWeightedPath();
}