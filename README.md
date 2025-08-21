## CGraph
CGraph is a simple **C language** graph theory algorithm library.
It provides common graph algorithms and convenient iterator interfaces, suitable for learning.

## Features
- Topological Sort
- Unweighted shortest path (BFS)
- Weighted shortest path (Dijkstra, Bellman-Ford, Floyd-Warshall)
- Minimum Spanning Tree (Prim, Kruskal)
- Maximum Flow (Edmonds-Karp)
- Euler Path & Circuit
- Articulation Points Detection
- Strongly Connected Components
- AOA Network Critical Path Analysis (Critical Path Method)
- Iterator Interfaces (traverse vertices and edges)

## Story
这个项目最早可以追溯到作者 (Bear Kun) 大一时候，正在学习数据结构与算法，并储存在另一个仓库中。

最初，使用最简单的 **邻接表+结构体数组(AoS)** 组成图结构。
```
struct Edge {
    struct Edge *next;
    DataType1 a;
    ...
};
```

但是，当我逐渐加入越来越多图论算法时，发现其弊端：
> 不同算法所需的数据不尽相同，有时甚至大相径庭，但图的拓扑结构总是相同的，结果是难以扩展。

直到大二，我重新开始维护这个项目，尝试解决这个问题。

首先，尝试使用 **共用体(union)+内存动态分配**，发现这只能略微缓解问题，代价是复杂度提升，而且丑。
```
struct Edge {
    struct Edge *next;
    Vertex to;
    union {
        DataTypeAlg1 a;
        DataTypeAlg2 b;
        ...
    } tail;
};
```

后来，借鉴开源项目的思想，放弃使用结构体，改为 **ID+属性数值(SoA)**，发现可以完美解决问题。
同时，重构邻接表，使用`head[]`+`next[]`实现，贴合ID索引，并添加`free`头管理空闲ID，方便增删。
```
EdgeId free;
EdgeId head[vertCap];
EdgeId next[edgeCap];

DataType1 a[edgeCap];
DataType2 b[edgeCap];
...
```

但这之后，我发现一个新的问题：**如何管理无向边？** <br>
因为，无向边的ID是唯一的，即`next[id]`是唯一的，但需要同时插入到两个链表(`head[from]`和`head[to]`)。

首先，我想到了`~id`和`id+cap`，用它们中的其中一个搭配`id`表示无向边在邻接表的两个节点。
但问题很明显：<br>
- `~id`需要维护一个双向数组，即有正索引和负索引，复杂易错；
- `id+cap`需要知晓`cap`，不够灵活；
- 同时，两者都不能很好地扩容，需要额外的数据偏移。

最终，我发现使用**奇偶性**可以同时解决这些问题，即一个`id`分为`id * 2`和`id * 2 + 1`。
```
                b * 2
a * 2 --------> b * 2 + 1 ----> c * 2
a * 2 + 1                       c * 2 + 1
```

于是，CGraph成了现在的样子。

再后来，我才知道：这种结构叫做**链式向前星(Linked Forward Star)**。<br>
我说：该死，谁叫我出生在天文望远镜发明后的时代？

另，应该可以说，本项目的**Euler Path & Circuit**算法，也是本人独立构思的，
因为当时那本教科书连该问题的解决思路都没有。现在我知道它的名字是 Hierholzer。
