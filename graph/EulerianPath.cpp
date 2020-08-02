#include <iostream>
#include <vector>
#include <algorithm>

using Graph = std::vector<std::vector<int>>;

class EulerianPathSolver {
private:
    Graph graph_;
    int num_nodes_ = 0;
    int edge_count_ = 0;

    std::vector<int> in_, out_; // degree of each node
    std::vector<int> path_;

    // count number of edges and populates in_ and out_
    void analyzeEdges() {
        in_ = std::vector<int>(num_nodes_, 0);
        out_ = std::vector<int>(num_nodes_, 0);

        for (int from = 0; from < num_nodes_; ++from) {
            for (int to : graph_[from]) {
                ++in_[to];
                ++out_[from];
                ++edge_count_;
            }
        }
    }

    void dfs(int at) {
        while (out_[at] != 0) {
            --out_[at];
            int next = graph_[at][out_[at]];
            dfs(next);
        }
        path_.push_back(at);
    }



public:
    EulerianPathSolver(Graph& graph) : graph_(graph), num_nodes_(graph.size()) {}

    bool graphHasEulerianPath() {
        if (edge_count_ == 0)
            return false;
        int start_node = 0, end_node = 0; // count possible start & end nodes
        
        for (int i = 0; i < num_nodes_; ++i) {
            if (in_[i] - out_[i] > 1 || out_[i] - in_[i] > 1)
                return false;
            if (in_[i] - out_[i] == 1)
                ++end_node;
            if (out_[i] - in_[i] == 1)
                ++start_node;
        }

        // for there to be eulerian path
        // either all nodes have even degree OR two vertices have odd degree
        return (start_node == 0 && end_node == 0) || 
               (start_node == 1 && end_node == 1);
    }

    int findStartNode() {
        int start = 0;
        for (int i = 0; i < num_nodes_; ++i) {
            // found unique start node
            if (out_[i] - in_[i] == 1)
                return i;
            if (out_[i] > 0)
                start = i;
        }
        return start;
    }



    std::vector<int> getEulerianPath() {
        analyzeEdges();

        if (graphHasEulerianPath() == false)
            return {};
        
        dfs(findStartNode());

        if (path_.size()-1 != edge_count_)
            return {};
        
        // when we backtrack nodes are in reverse order
        std::reverse(path_.begin(), path_.end()); 
        return path_;
    }

};


void addDirectedEdge(Graph& graph, int from, int to) {
    graph[from].push_back(to);
}


int main() {
    Graph graph(7);

    addDirectedEdge(graph, 1, 2);
    addDirectedEdge(graph, 1, 3);
    addDirectedEdge(graph, 2, 2);
    addDirectedEdge(graph, 2, 4);
    addDirectedEdge(graph, 2, 4);
    addDirectedEdge(graph, 3, 1);
    addDirectedEdge(graph, 3, 2);
    addDirectedEdge(graph, 3, 5);
    addDirectedEdge(graph, 4, 3);
    addDirectedEdge(graph, 4, 6);
    addDirectedEdge(graph, 5, 6);
    addDirectedEdge(graph, 6, 3);

    EulerianPathSolver solver(graph);
    auto path = solver.getEulerianPath();

    std::cout << "path size: " << path.size() << std::endl;
    // Outputs path: [1, 3, 5, 6, 3, 2, 4, 3, 1, 2, 2, 4, 6]
    for (auto n : path) 
        std::cout << n << " ";
    std::cout << std::endl;


    return 0;
}