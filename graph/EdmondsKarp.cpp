#include <vector>
#include <iostream>
#include <memory>
#include <limits>
#include <algorithm>
#include <queue>

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

    void visit(int i) {
        visited_[i] = visited_token_;
    }

    // whether or not a node has been visited
    bool visited(int i) {
        return visited_[i] == visited_token_;
    }

    // marking unvisited is O(1)
    void markAllNodesAsUnvisited() {
        ++visited_token_;
    }

};

class EdmondsKarpSolver : public NetworkFlowSolverBase {
private:
    long bfs() {
        std::queue<int> q;
        visit(src_node_);
        q.push(src_node_);

        std::vector<std::shared_ptr<Edge>> prev(num_nodes_, nullptr);
        while (q.empty() == false) {
            int node = q.front();
            q.pop();

            if (node == sink_node_)
                break;

            for (auto edge_ptr : graph_[node]) {
                if (edge_ptr->remainingCapacity() > 0 && visited(edge_ptr->to) == false) {
                    prev[edge_ptr->to] = edge_ptr;
                    visit(edge_ptr->to);
                    q.push(edge_ptr->to);
                }
            }
        }

        // sink not reached
        if (prev[sink_node_] == nullptr)
            return 0;

        // find bottle neck value
        long bottle_neck = std::numeric_limits<long>::max();
        for (auto edge_ptr = prev[sink_node_]; edge_ptr != nullptr; edge_ptr = prev[edge_ptr->from])
            bottle_neck = std::min(bottle_neck, edge_ptr->remainingCapacity());

        // retrace augmented path and update flow
        for (auto edge_ptr = prev[sink_node_]; edge_ptr != nullptr; edge_ptr = prev[edge_ptr->from])
            edge_ptr->augment(bottle_neck);

        return bottle_neck;
    }

public:
    EdmondsKarpSolver(int num_nodes, int src_node, int sink_node) :
        NetworkFlowSolverBase(num_nodes, src_node, sink_node) {}
    
    void solve() override {
        long flow;
        do {
            markAllNodesAsUnvisited();
            flow = bfs();
            max_flow_ += flow;
        } while (flow != 0);
    }

};



int main() {
    int num_nodes = 12;
    int src_node = num_nodes - 2;
    int sink_node = num_nodes - 1;

    EdmondsKarpSolver solver(num_nodes, src_node, sink_node);

    // Edges from source
    solver.addEdge(src_node, 0, 5);
    solver.addEdge(src_node, 1, 10);
    solver.addEdge(src_node, 2, 5);

    // Middle edges
    solver.addEdge(0, 3, 10);
    solver.addEdge(1, 0, 15);
    solver.addEdge(1, 4, 20);
    solver.addEdge(2, 5, 10);
    solver.addEdge(3, 4, 25);
    solver.addEdge(3, 6, 10);
    solver.addEdge(4, 2, 5);
    solver.addEdge(4, 7, 30);
    solver.addEdge(5, 7, 5);
    solver.addEdge(5, 8, 10);
    solver.addEdge(7, 3, 15);
    solver.addEdge(7, 8, 5);

    // Edges to sink
    solver.addEdge(6, sink_node, 5);
    solver.addEdge(7, sink_node, 15);
    solver.addEdge(8, sink_node, 10);

    // Maximum Flow is: 20
    std::cout << "Maximum flow is: " << solver.getMaxFlow() << std::endl;

    auto g = solver.getGraph();
    for (auto& edges : g) {
        for (auto& edge_ptr : edges) 
            std::cout << edge_ptr->toString(src_node, sink_node) << std::endl;
    }

    return 0;
}