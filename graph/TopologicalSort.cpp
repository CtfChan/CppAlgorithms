#include <iostream>
#include <unordered_map>
#include <vector>


struct Edge {
    int from, to, weight;
};
using Graph = std::unordered_map<int, std::vector<Edge>>;

int depthFirstSearch(int i, int at, 
        std::vector<bool>& visited, std::vector<int>& ordering, 
        Graph& graph) {
    visited[at] = true;

    auto neighbours = graph.find(at);
    if (neighbours != graph.end()) {
        for (Edge& neighbour : graph[at]) {
            if (visited[neighbour.to] == false) {
                i = depthFirstSearch(i, neighbour.to, visited, ordering, graph);       
            }
        }
    }
    
    ordering[i] = at;
    return i - 1;
}

std::vector<int> topologicalSort(Graph& graph, int num_nodes) {
    std::vector<int> ordering(num_nodes, 0);
    std::vector<bool> visited(num_nodes, false);
    
    int i = num_nodes - 1; // current idx in ordering array
    for (int at = 0; at < num_nodes; ++at ) 
        if (visited[at] == false)
            i = depthFirstSearch(i, at, visited, ordering, graph);   

    return ordering;
}


std::vector<int> dagShortestPath(Graph& graph, int start, int num_nodes) {
    auto topsort = topologicalSort(graph, num_nodes);
    std::vector<int> dist(num_nodes, -1);
    dist[start] = 0;

    for (int i = 0; i < num_nodes; ++i) {
        int node_idx = topsort[i];
        // only update neighbouring distances if traversable
        if (dist[node_idx] != -1) {
            for (Edge& edge : graph[node_idx]) {
                int new_dist = dist[node_idx] + edge.weight;
                dist[edge.to] = (dist[edge.to] == -1) ? new_dist :
                                std::min(dist[edge.to], new_dist);
            }
        }
    }

    return dist;
}

int main() {
    int N = 7;
    Graph graph;
    graph[0].push_back({0, 1, 3});
    graph[0].push_back({0, 2, 2});
    graph[0].push_back({0, 5, 3});
    graph[1].push_back({1, 3, 1});
    graph[1].push_back({1, 2, 6});
    graph[2].push_back({2, 3, 1});
    graph[2].push_back({2, 4, 10});
    graph[3].push_back({3, 4, 5});
    graph[5].push_back({5, 4, 7});

    std::vector<int> ordering = topologicalSort(graph, N);

    // Prints: [6, 0, 5, 1, 2, 3, 4]
    for (int num : ordering) 
        std::cout << num << " ";
    std::cout << std::endl;

    std::vector<int> dists = dagShortestPath(graph, 0, N);

    // 0->4 distance should print 8
    std::cout << dists[4] << std::endl;

    // 0->6 distance should print -1, unreachable
    std::cout << dists[6] << std::endl;

    return 0;
}