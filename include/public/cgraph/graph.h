#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

#include "cgraph/type.h"

CGraphSize cgraphGetGraphSize();
void cgraphInit(CGraph *graph, CGraphBool directed, CGraphSize vertCap,
                CGraphSize edgeCap);
void cgraphDestroy(const CGraph *graph);

CGraphId cgraphAddVert(CGraph *graph);
CGraphId cgraphAddEdge(CGraph *graph, CGraphId from, CGraphId to,
                       CGraphBool directed);
void cgraphReserveVert(CGraph *graph, CGraphSize num);
void cgraphDeleteVert(CGraph *graph, CGraphId id);
void cgraphDeleteEdge(CGraph *graph, CGraphId id);

void *cgraphGetEdgeAttribute(const CGraph *graph, const char name[16]);
void cgraphAddEdgeAttribute(CGraph *graph, const char name[16],
                            CGraphSize sizeOfElem);

void cgraphGetIdRange(const CGraph *graph, CGraphSize *vert, CGraphSize *edge);
void cgraphGetIndegree(const CGraph *graph, CGraphInt indegree[]);

#endif // GRAPH_GRAPH_H