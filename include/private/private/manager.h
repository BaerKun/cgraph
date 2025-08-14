#ifndef MANAGER_H
#define MANAGER_H

#include "graph_detail.h"

void cgraphManagerInit(CGraphManager *mgr, CGraphBool directed, CGraphSize vertCap,
                      CGraphSize edgeCap);
void cgraphManagerDestroy(const CGraphManager *mgr);

CGraphId cgraphManagerNewVert(CGraphManager *mgr);
CGraphId cgraphManagerNewEdge(CGraphManager *mgr, CGraphId from, CGraphId to,
                            CGraphBool directed);

void cgraphManagerDeleteVert(CGraphManager *mgr, CGraphId vid);
void cgraphManagerDeleteEdge(CGraphManager *mgr, CGraphId eid);

#endif // MANAGER_H
