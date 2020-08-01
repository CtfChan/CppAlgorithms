#include <iostream>
#include <vector>


using Graph = std::vector<std::vector<int>>;

std::vector<int> findTreeCenters(Graph& tree) {
    int n = tree.size();
    std::vector<int> degree(n, 0);
    std::vector<int> leaves; leaves.reserve(n);

    // compute degree of all nodes and remember initial leaves
    for (int node = 0; node < n; ++node) {
        degree[node] = tree[node].size();
        if (degree[node] <= 1) {
            leaves.push_back(node);
            degree[node] = 0;
        }
    }

    // iteratively peel off nodes and decrement degree of neighbours of leaves
    int processed_leaves = leaves.size();
    while (processed_leaves < n) {
        std::vector<int> new_leaves;
        for (int leaf : leaves) {
            for (int neighbour : tree[leaf]) {
                --degree[neighbour];
                if (degree[neighbour] == 1) {
                    new_leaves.push_back(neighbour);
                }
            }
            degree[leaf] = 0;
        }
        leaves = new_leaves;
        processed_leaves += leaves.size();
    }

    return leaves;
}

void addUndirectedEdge(Graph& graph, int from, int to) {
    graph[from].push_back(to);
    graph[to].push_back(from);
}

int main() {

    // Center = 2
    Graph graph = std::vector<std::vector<int>>(9);
    addUndirectedEdge(graph, 0, 1);
    addUndirectedEdge(graph, 2, 1);
    addUndirectedEdge(graph, 2, 3);
    addUndirectedEdge(graph, 3, 4);
    addUndirectedEdge(graph, 5, 3);
    addUndirectedEdge(graph, 2, 6);
    addUndirectedEdge(graph, 6, 7);
    addUndirectedEdge(graph, 6, 8);

    for (int center : findTreeCenters(graph)) 
        std::cout << center << " ";
    std::cout << std::endl;


    // Centers are 2,3
    Graph graph2 = std::vector<std::vector<int>>(7);
    addUndirectedEdge(graph2, 0, 1);
    addUndirectedEdge(graph2, 1, 2);
    addUndirectedEdge(graph2, 2, 3);
    addUndirectedEdge(graph2, 3, 4);
    addUndirectedEdge(graph2, 4, 5);
    addUndirectedEdge(graph2, 4, 6);
    
    for (int center : findTreeCenters(graph2)) 
        std::cout << center << " ";
    std::cout << std::endl;


    return 0;
}