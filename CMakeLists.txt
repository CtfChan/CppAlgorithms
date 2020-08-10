cmake_minimum_required(VERSION 3.1)

project (CppAlgorithms)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)


# data structures
add_executable(union_find
    data_structures/UnionFind.cpp
)

add_executable(min_indexed_d_heap
    data_structures/MinIndexedDHeap.cpp
)

add_executable(sparse_tables
    data_structures/SparseTables.cpp
)

add_executable(fenwick_tree
    data_structures/FenwickTree.cpp
)



# graph
add_executable(breadth_first_search
    graph/BreadthFirstSearch.cpp
)

add_executable(depth_first_search
    graph/DepthFirstSearch.cpp
)



add_executable(bellman_ford 
    graph/BellmanFord.cpp
)

add_executable(floyd_warshall 
    graph/FloydWarshall.cpp
)

add_executable(topological_sort 
    graph/TopologicalSort.cpp
)

add_executable(tarjan_strongly_connected_component
    graph/TarjanStronglyConnectedComponent.cpp
)

add_executable(dijkstra
    graph/Dijkstra.cpp
)

add_executable(lazy_prim
    graph/LazyPrim.cpp
)

add_executable(kruskals_edge
    graph/KruskalsEdge.cpp
)

add_executable(rooting_tree
    graph/RootingTree.cpp
)

add_executable(tree_center
    graph/TreeCenter.cpp
)

add_executable(tree_isomorphism
    graph/TreeIsomorphism.cpp
)

add_executable(eulerian_path
    graph/EulerianPath.cpp
)

add_executable(lowest_common_ancestor
    graph/LowestCommonAncestor.cpp
)

add_executable(ford_fulkerson
    graph/FordFulkerson.cpp
)

add_executable(edmonds_karp
    graph/EdmondsKarp.cpp
)

add_executable(capacity_scaling
    graph/CapacityScaling.cpp
)

add_executable(dinic
    graph/Dinic.cpp
)


# dynamic programming
add_executable(knapsack
    dynamic_programming/KnapsackProblem.cpp
)