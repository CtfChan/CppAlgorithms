# Notes

## Dynamic Programming
Knapsack Problem
- num_items+1 x capacity+1 dp array
```
for item = 0 : num_items:
    for capacity = 0 : total_capacity
        dp[item][capacity] = max(don't put item in sack, put item in sack)
```


## Graphs
Single Source Shortest Path
- bellman ford
- dijkstra 

All pairs shorest path
- floyd warshall

Tarjan strongly connected component

Topological sorting

Trees
- rooting a tree
- tree center
- lowest common ancestor
- tree ismorphism

Minimum Spannng Tree
- Prim
- Kruskal (TODO)

Network Flow
- ford fulkerson
- edmond karp
- capacity scaling
- dinic


Travelling salesman Problem 


Eulerian Path and Circuits