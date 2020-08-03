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

class DinicSolver : public NetworkFlowSolverBase {
private:
    std::vector<int> level_;
    
    // constructs level graph
    // returns false if not possible
    bool constructLevelGraph() {
        level_ = std::vector<int>(num_nodes_, -1);
        level_[src_node_] = 0;

        std::queue<int> q;
        q.push(src_node_);

        while(q.empty() == false) {
            int node = q.front();
            q.pop();

            for (auto edge_ptr : graph_[node]) {
                if (edge_ptr->remainingCapacity() > 0 && level_[edge_ptr->to] == -1) {
                    level_[edge_ptr->to] = level_[node] + 1;
                    q.push(edge_ptr->to);
                }
            }
        }

        return level_[sink_node_] != -1;
    }

    // finds augmented path and fills in bottle neck value and returns it
    long dfs(int at, std::vector<int>& next, long flow) {
        if (at == sink_node_)
            return flow;

        const int num_edges = graph_[at].size();
        for (; next[at] < num_edges; next[at]++) {
            auto& edge_ptr = graph_[at][next[at]];
            long remaining_cap = edge_ptr->remainingCapacity();
            if (remaining_cap > 0 && level_[edge_ptr->to] == level_[at] + 1) {
                long bottle_neck = dfs(edge_ptr->to, next, std::min(flow, remaining_cap));

                if (bottle_neck > 0) {
                    edge_ptr->augment(bottle_neck);
                    return bottle_neck;
                } 
            }

        }

        return 0;
    }

public:
    DinicSolver(int num_nodes, int src_node, int sink_node) :
        NetworkFlowSolverBase(num_nodes, src_node, sink_node) {
        level_.resize(num_nodes_);
    }
    
    void solve() override {
        // allows us to remember if a node lead to deadend 
        std::vector<int> next(num_nodes_, 0);
        
        static constexpr long INF = std::numeric_limits<long>::max() / 2;

        while (constructLevelGraph()) {
            next = std::vector<int>(num_nodes_, 0);
            for (long flow = dfs(src_node_, next, INF); flow != 0; flow = dfs(src_node_, next, INF)) {
                max_flow_ += flow;
            }
        }
    }

};


void testSmallFlowGraph() {
    int num_nodes = 6;
    int src_node = num_nodes - 1;
    int sink_node = num_nodes - 2;

    DinicSolver solver(num_nodes, src_node, sink_node);
 
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
    testSmallFlowGraph();

    return 0;
}