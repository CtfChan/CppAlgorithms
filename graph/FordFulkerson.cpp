#include <vector>
#include <iostream>
#include <memory>
#include <limits>
#include <algorithm>

struct Edge {
    int from, to;
    std::weak_ptr<Edge> residual;
    long flow;
    long capacity;

    Edge(int from, int to, long capacity) {
        this->from = from;
        this->to = to;
        this->capacity = capacity;
    }

    bool isResidual() {
        return capacity == 0;
    }
    
    long remainingCapacity() {
        return capacity - flow;
    }

    void augment(long bottle_neck) {
        flow += bottle_neck;
        auto res_ptr = residual.lock();
        res_ptr->flow -= bottle_neck;
    }

    std::string toString(int src, int sink) {
        std::string u = (from == src) ? "s" : ((from == sink) ? "t" : std::to_string(from));
        std::string v = (to == src) ? "s" : ((to == sink) ? "t" : std::to_string(to));
        std::string res = "Edge " + u + " -> " + v  +
                          " | flow = " + std::to_string(flow) + 
                          " | capacity = " + std::to_string(capacity) + 
                          " | is residual = " + std::to_string(isResidual());
        return res; 
    }

};

using Graph = std::vector<std::vector<std::shared_ptr<Edge> > >;


class NetworkFlowSolverBase {
protected:
    int num_nodes_, src_node_, sink_node_;

    bool solved_ = false;

    Graph graph_;

    int visited_token_ = 1;
    std::vector<int> visited_;

    long max_flow_ = 0;

    void execute() {
        if (solved_)
            return;
        solved_ = true;
        solve();
    }


public:
    NetworkFlowSolverBase(int num_nodes, int src_node, int sink_node) :
        num_nodes_(num_nodes), src_node_(src_node), sink_node_(sink_node) {
            graph_.resize(num_nodes);
            visited_.resize(num_nodes);
    }

    Graph getGraph() {
        execute();
        return graph_;
    }

    void addEdge(int from, int to, int capacity) {
        if (capacity <= 0) 
            return;

        auto e1 = std::make_shared<Edge>(from, to, capacity);
        auto e2 = std::make_shared<Edge>(to, from, 0);
        e1->residual = e2;
        e2->residual = e1;

        graph_[from].push_back(e1);
        graph_[to].push_back(e2);
    }

    long getMaxFlow() {
        execute();
        return max_flow_;
    }

    virtual void solve() = 0;

};

class FordFulkersonDfsSolver : public NetworkFlowSolverBase {
private:
    long dfs(int node, long flow) {
        if (node == sink_node_)
            return flow;
        
        visited_[node] = visited_token_;

        for (auto edge_ptr : graph_[node]) {
            if (edge_ptr->remainingCapacity() > 0 && visited_[edge_ptr->to] != visited_token_) {
                long limiting_flow = std::min(flow, edge_ptr->remainingCapacity());
                long bottle_neck = dfs(edge_ptr->to, limiting_flow);

                if (bottle_neck > 0) {
                    edge_ptr->augment(bottle_neck);
                    return bottle_neck;
                }

            }
        }
        
        return 0;
    }

public:
    FordFulkersonDfsSolver(int num_nodes, int src_node, int sink_node) :
        NetworkFlowSolverBase(num_nodes, src_node, sink_node) {}
    
    void solve() override {
        // very large value to simulate infinite src
        static constexpr long INF = std::numeric_limits<long>::max() / 2;
        for (long flow = dfs(src_node_, INF); flow != 0; flow = dfs(src_node_, INF)) {
            ++visited_token_; // tells me whether or not a node is part of my current augmented path
            max_flow_ += flow;
        }
    }

};



int main() {
    int num_nodes = 12;
    int src_node = num_nodes - 2;
    int sink_node = num_nodes - 1;

    FordFulkersonDfsSolver solver(num_nodes, src_node, sink_node);

     // Edges from source
    solver.addEdge(src_node, 0, 10);
    solver.addEdge(src_node, 1, 5);
    solver.addEdge(src_node, 2, 10);

    // Middle edges
    solver.addEdge(0, 3, 10);
    solver.addEdge(1, 2, 10);
    solver.addEdge(2, 5, 15);
    solver.addEdge(3, 1, 2);
    solver.addEdge(3, 6, 15);
    solver.addEdge(4, 1, 15);
    solver.addEdge(4, 3, 3);
    solver.addEdge(5, 4, 4);
    solver.addEdge(5, 8, 10);
    solver.addEdge(6, 7, 10);
    solver.addEdge(7, 4, 10);
    solver.addEdge(7, 5, 7);

    // Edges to sink
    solver.addEdge(6, sink_node, 15);
    solver.addEdge(8, sink_node, 10);

    // Maximum Flow is: 23
    std::cout << "Maximum flow is: " << solver.getMaxFlow() << std::endl;

    auto g = solver.getGraph();
    for (auto& edges : g) {
        for (auto& edge_ptr : edges) 
            std::cout << edge_ptr->toString(src_node, sink_node) << std::endl;
    }

    return 0;
}