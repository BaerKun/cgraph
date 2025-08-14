#ifndef GRAPH_ALL_PAIRS_WEIGHTED_PATH_H
#define GRAPH_ALL_PAIRS_WEIGHTED_PATH_H

#include "cgraph/type.h"

// 不限正负权值
void FloydWarshallWeightedPath(WeightType **weight, CGraphSize vertNum,
                               CGraphId **path, WeightType **distance);

#endif // GRAPH_ALL_PAIRS_WEIGHTED_PATH_H
