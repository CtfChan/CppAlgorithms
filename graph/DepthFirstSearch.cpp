#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <stack>


struct Edge {
    int from, to, cost;
};

using Graph = std::unordered_map<int, std::vector<Edge>>;


void addDirectedEdge(Graph& graph, int from, int to, int cost) {
    graph[from].push_back({from, to, cost});
}


int depthFirstSearch(Graph& graph, int start) {
    int count = 0;
    std::stack<int> stack;
    std::unordered_set<int> visited;

    stack.push(start);

    while (stack.empty() == false) {
        int curr = stack.top();
        stack.pop();

        if (visited.find(curr) == visited.end()) {
            ++count;
            visited.insert(curr);

            for (Edge& edge : graph[curr]) {
                if (visited.find(edge.to) == visited.end())
                    stack.push(edge.to);
            }
        }
    }

    return count;

}

int main() {
    // Create a fully connected graph
    //           (0)
    //           / \
    //        5 /   \ 4
    //         /     \
    // 10     <   -2  >
    //   +->(2)<------(1)      (4)
    //   +--- \       /
    //         \     /
    //        1 \   / 6
    //           > <
    //           (3)
    Graph graph;
    addDirectedEdge(graph, 0, 1, 4);
    addDirectedEdge(graph, 0, 2, 5);
    addDirectedEdge(graph, 1, 2, -2);
    addDirectedEdge(graph, 1, 3, 6);
    addDirectedEdge(graph, 2, 3, 1);
    addDirectedEdge(graph, 2, 2, 10); // Self loop
   
    long node_count = depthFirstSearch(graph, 0);
    std::cout << node_count << std::endl; // 4

    node_count = depthFirstSearch(graph, 4);
    std::cout << node_count << std::endl; // 1

 
    return 0;
}