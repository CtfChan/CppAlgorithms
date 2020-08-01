#include <iostream>
#include <vector>
#include <queue>

using Graph = std::vector<std::vector<int>>;

struct Edge {
    int to;
    int cost;
};

int prims(Graph& graph) {
    int n = graph.size();
    long sum = 0;
    long visited_nodes = 1;

    auto cmp = [](Edge& e1, Edge& e2){ return e1.cost < e2.cost; };
    std::priority_queue<Edge, std::vector<Edge>, decltype(cmp)> pq(cmp); // min heap

    std::vector<bool> connected(n, false);
    
    // visit node 0 and push all edges into pq
    connected[0] = true;
    for (int i = 1; i < n; ++i) 
        if (graph[0][i] != -1)
            pq.push({i, graph[0][i]});
    
    // keep connecting to new nodes while you can 
    while (visited_nodes != n && pq.empty() == false) {
        auto [to, cost] = pq.top();
        pq.pop();

        // examine edges of `to` if we've never been here before
        if (connected[to] == false) {
            for (int i = 0; i < n; ++i ) {
                if (connected[i] == false && graph[to][i] != -1)
                    pq.push({i, graph[to][i]});
            }

            connected[to] = true;
            sum += cost;
            ++visited_nodes;
        }
    }

    // make sure visited every node
    if (visited_nodes != n)
        return -1;

    return sum;

}

int main() {
    int num_nodes = 10;
    Graph graph(num_nodes, std::vector<int>(num_nodes, -1)); // -1 means not traversable

    // Make an undirected graph
    graph[0][1] = graph[1][0] = 1;
    graph[0][3] = graph[3][0] = 4;
    graph[0][4] = graph[4][0] = 5;
    graph[1][3] = graph[3][1] = 2;
    graph[1][2] = graph[2][1] = 1;
    graph[2][3] = graph[3][2] = 5;
    graph[2][5] = graph[5][2] = 7;
    graph[3][4] = graph[4][3] = 2;
    graph[3][6] = graph[6][3] = 2;
    graph[3][5] = graph[5][3] = 11;
    graph[4][7] = graph[7][4] = 4;
    graph[5][6] = graph[6][5] = 1;
    graph[5][8] = graph[8][5] = 4;
    graph[6][7] = graph[7][6] = 4;
    graph[6][8] = graph[8][6] = 6;
    graph[7][8] = graph[8][7] = 1;
    graph[7][9] = graph[9][7] = 2;
    graph[8][9] = graph[9][8] = 0;

    long min_cost = prims(graph);
    std::cout << "Minimum Spanning Tree Cost: " << min_cost << std::endl;

    return 0;
}
