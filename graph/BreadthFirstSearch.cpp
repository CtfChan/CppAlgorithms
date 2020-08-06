#include <vector>
#include <iostream>
#include <exception>
#include <queue>
#include <algorithm>

struct Edge {
    int from, to, cost;
};

using Graph = std::vector<std::vector<Edge>>;

class BreadthFirstSearchSolver {
private:
    int n_;
    std::vector<int> prev_; 
    Graph graph_;

    void breadthFirstSearch(int start) {
        std::vector<bool> visited(n_, false);
        std::queue<int> q;
        prev_ = std::vector<int>(n_, -1);

        // std::cout << "n " << n_ << std::endl;
        // std::cout << "start " << start << std::endl;

        q.push(start);
        visited[start] = true;


        while (q.empty() == false) {
            int curr = q.front();
            q.pop();

            for (const Edge& edge : graph_[curr]) {
                if (visited[edge.to] == false) {
                    q.push(edge.to);
                    prev_[edge.to] = curr;
                    visited[edge.to] = true;
                }
            }
        }
    }


public:
    BreadthFirstSearchSolver(Graph& graph) {
        if (graph.empty())
            throw std::invalid_argument("graph cannot be empty");
        graph_ = graph;
        n_ = graph.size();

    }

    std::vector<int> reconstructPath(int start, int end) {
        breadthFirstSearch(start);

        std::vector<int> path;
        for (int at = end; at != start; at = prev_[at]) {
            path.push_back(at);
            if (at == -1)
                return {};
        }
        path.push_back(start);
        
        std::reverse(path.begin(), path.end());
        return path;
    }
};

void addUndirectedEdge(Graph& graph, int from, int to, int cost=0) {
    graph[from].push_back({from, to, cost});
    graph[to].push_back({to, from, cost});
}


int main() {

    int n = 13;
    Graph graph(n);

    addUndirectedEdge(graph, 0, 7);
    addUndirectedEdge(graph, 0, 9);
    addUndirectedEdge(graph, 0, 11);
    addUndirectedEdge(graph, 7, 11);
    addUndirectedEdge(graph, 7, 6);
    addUndirectedEdge(graph, 7, 3);
    addUndirectedEdge(graph, 6, 5);
    addUndirectedEdge(graph, 3, 4);
    addUndirectedEdge(graph, 2, 3);
    addUndirectedEdge(graph, 2, 12);
    addUndirectedEdge(graph, 12, 8);
    addUndirectedEdge(graph, 8, 1);
    addUndirectedEdge(graph, 1, 10);
    addUndirectedEdge(graph, 10, 9);
    addUndirectedEdge(graph, 9, 8);

    BreadthFirstSearchSolver solver(graph);

    int start = 10, end = 5;
    auto path = solver.reconstructPath(start, end);

    // The shortest path from 10 to 5 is: [10 -> 9 -> 0 -> 7 -> 6 -> 5]
    for (int p : path)
        std::cout << p << " ";
    std::cout << std::endl;


    return 0;
}