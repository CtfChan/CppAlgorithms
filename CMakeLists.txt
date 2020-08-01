cmake_minimum_required(VERSION 3.1)

project (CppAlgorithms)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)


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

add_executable(rooting_tree
    graph/RootingTree.cpp
)


add_executable(tree_center
    graph/TreeCenter.cpp
)