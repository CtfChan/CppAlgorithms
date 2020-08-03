#include <vector>
#include <iostream>
#include <memory>
#include <limits>
#include <algorithm>
#include <queue>
#include <math.h>

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

    virtual void addEdge(int from, int to, long capacity) {
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

class CapacityScalingSolver : public NetworkFlowSolverBase {
private:
    long delta_ = 0;
    
    long dfs(int node, long flow) {
        if (node == sink_node_)
            return flow;

        visit(node);

        auto& edges = graph_[node];
        for (auto edge_ptr : edges) {
            long remaining_cap = edge_ptr->remainingCapacity();
            if (remaining_cap >= delta_ && visited(edge_ptr->to) == false) {
                long bottle_neck = dfs(edge_ptr->to, std::min(flow, remaining_cap));

                if (bottle_neck > 0) {
                    edge_ptr->augment(bottle_neck);
                    return bottle_neck;
                }
                
            }
        }
        
        return 0;
    }

public:

    CapacityScalingSolver(int num_nodes, int src_node, int sink_node) :
        NetworkFlowSolverBase(num_nodes, src_node, sink_node) {}
    
    void solve() override {
        // largest power of 2 < largest capacity
        // delta_ = (long) std::pow(2, (int)std::floor(std::log(delta_)/std::log(2)));
        long n = 1;
        while (std::pow(2, n) < delta_)
            ++n;
        delta_ = std::pow(2, --n);


        // repeatedly find aug path from s->t
        static constexpr long INF = std::numeric_limits<long>::max() / 2;
        for (long flow = 0; delta_ > 0; delta_ = delta_/2) {
            do {
                markAllNodesAsUnvisited();
                flow = dfs(src_node_, INF);
                max_flow_ += flow;
            } while (flow != 0);
        }

    }

    void addEdge(int from, int to, long capacity) override {
        NetworkFlowSolverBase::addEdge(from, to, capacity);
        delta_ = std::max(delta_, capacity);
    }

};


void testSmallExample() {
    int num_nodes = 6;
    int src_node = num_nodes - 1;
    int sink_node = num_nodes - 2;

    CapacityScalingSolver solver(num_nodes, src_node, sink_node);
 
    // Source edges
    solver.addEdge(src_node, 0, 10);
    solver.addEdge(src_node, 1, 10);

    // Sink edges
    solver.addEdge(2, sink_node, 10);
    solver.addEdge(3, sink_node, 10);

    // Middle edges
    solver.addEdge(0, 1, 2);
    solver.addEdge(0, 2, 4);
    solver.addEdge(0, 3, 8);
    solver.addEdge(1, 3, 9);
    solver.addEdge(3, 2, 6);

    
    // Maximum Flow is: 19
    std::cout << "Maximum flow is: " << solver.getMaxFlow() << std::endl;

}



int main() {
    testSmallExample();


    return 0;
}