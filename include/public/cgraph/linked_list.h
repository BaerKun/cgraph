#ifndef GRAPH_LINKED_PATH_H
#define GRAPH_LINKED_PATH_H

#include "cgraph/type.h"

CGraphLinkedNode *cgraphLinkedInsert(CGraphLinkedNode **predNextPtr,
                                     CGraphId id);
CGraphLinkedNode *cgraphLinkedUnlink(CGraphLinkedNode **predNextPtr);
void cgraphLinkedClear(CGraphLinkedNode **path);

#endif // GRAPH_LINKED_PATH_H
