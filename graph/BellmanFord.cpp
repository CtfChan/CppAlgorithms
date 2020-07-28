#include <iostream>
#include <vector>
#include <limits> 


struct Edge {
    int from;
    int to;
    double cost;
};


std::vector<double> bellmanFord(const std::vector<Edge>& edges, int num_vertices, int start) {
    std::vector<double> cost_to_come(num_vertices, std::numeric_limits<double>::infinity());
    cost_to_come[start] = 0;

    bool relaxed_an_edge = true; // early stopping condition when no edges relaxed

    // for each vertex, relax edge
    for (int v = 0; v < num_vertices - 1 && relaxed_an_edge; v++) {
        relaxed_an_edge = false;
        for (const auto [from, to, cost] : edges) {
            if (cost_to_come[from] + cost < cost_to_come[to]) {
                cost_to_come[to] = cost_to_come[from] + cost;
                relaxed_an_edge = true;
            }
        }
    }

    // run second time to detect negative cycles
    relaxed_an_edge = true;
    for (int v = 0; v < num_vertices - 1 && relaxed_an_edge; v++) {
        relaxed_an_edge = false;
        for (const auto [from, to, cost] : edges) {
            if (cost_to_come[from] + cost < cost_to_come[to]) {
                cost_to_come[to] = -std::numeric_limits<double>::max();
                relaxed_an_edge = true;
            }
        }
    }

    return cost_to_come;

}



int main() {
    int E = 10;
    int V = 9;
    int start = 0;

    std::vector<Edge> edges = {
        {0, 1, 1.0},
        {1, 2, 1.0},
        {2, 4, 1.0},
        {4, 3, -3.0},
        {3, 2, 1.0},
        {1, 5, 4.0},
        {1, 6, 4.0},
        {5, 6, 5.0},
        {6, 7, 4.0},
        {5, 7, 3.0}
    };

    auto cost_to_come = bellmanFord(edges, V, start);

    std::cout << "neg inf: " << -std::numeric_limits<double>::infinity() << std::endl;

    for (int i = 0; i < V; ++i) {
        std::cout << "The cost to get from node " << start << " to " 
                    << i << " is " << cost_to_come[i] << std::endl;
    }
    // Output:
    // The cost to get from node 0 to 0 is 0.00
    // The cost to get from node 0 to 1 is 1.00
    // The cost to get from node 0 to 2 is -Infinity
    // The cost to get from node 0 to 3 is -Infinity
    // The cost to get from node 0 to 4 is -Infinity
    // The cost to get from node 0 to 5 is 5.00
    // The cost to get from node 0 to 6 is 5.00
    // The cost to get from node 0 to 7 is 8.00
    // The cost to get from node 0 to 8 is Infinity

}