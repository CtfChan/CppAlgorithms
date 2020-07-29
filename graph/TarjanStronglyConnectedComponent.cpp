#include <vector>
#include <unordered_map>
#include <stack>
#include <iostream>

using Graph = std::vector<std::vector<int>>;

class TarjanStronglyConnectedComponent {
private:
    int n_;
    int strong_connected_component_count_ = 0;
    int id_ = 0;

    bool solved_ = false;
    Graph graph_;

    std::vector<int> low_; // low link value
    std::vector<int> ids_; // id of each node, also tracks UNVISITED
    std::vector<bool> on_stack_; // track if node is on stack

    std::stack<int> stack_;

    constexpr static int UNVISITED = -1;

public:

    TarjanStronglyConnectedComponent(Graph& graph) : 
        n_(graph.size()),  graph_(graph) {
    }

    std::vector<int> getStronglyConnectedComponents() {
        if (solved_ == false)
            solve();
        return low_;
    }

    int getStronglyConnectedComponentsCount() {
        return strong_connected_component_count_;
    }

    void depthFirstSearch(int at) {
        stack_.push(at);
        on_stack_[at] = true;
        ids_[at] = id_;
        low_[at] = id_;
        ++id_;

        // visit all neighbours, after visit update low link value
        for (int to : graph_[at]) {
            if (ids_[to] == UNVISITED)
                depthFirstSearch(to);
            if (on_stack_[to]) 
                low_[at] = std::min(low_[at], low_[to]);
        }

        // if we're the start of SCC, empty the stack and make sure they have same id
        if (ids_[at] == low_[at]) {
            while (stack_.empty() == false) {
                int node = stack_.top();
                stack_.pop();
                on_stack_[node] = false;
                low_[node] = ids_[at];
                if (node == at)
                    break;
            }
            ++strong_connected_component_count_;
        }

    }


    void solve() {
        if (solved_ == true)
            return;
        
        low_ = std::vector<int>(n_);
        ids_ = std::vector<int>(n_, UNVISITED);
        on_stack_ = std::vector<bool>(n_);
        stack_ = std::stack<int>();

        for (int i = 0; i < n_; ++i) {
            if (ids_[i] == UNVISITED)
                depthFirstSearch(i);
        }

        solved_ = true;
    }

};


int main() {

    int n = 8;
    Graph graph = std::vector<std::vector<int>>(n);

    graph[6].push_back(0);
    graph[6].push_back(2);
    graph[3].push_back(4);
    graph[6].push_back(4);
    graph[2].push_back(0);
    graph[0].push_back(1);
    graph[4].push_back(5);
    graph[5].push_back(6);
    graph[3].push_back(7);
    graph[7].push_back(5);
    graph[1].push_back(2);
    graph[7].push_back(3);
    graph[5].push_back(0);

    auto solver = TarjanStronglyConnectedComponent(graph);
    auto scc = solver.getStronglyConnectedComponents();

    // 3
    std::cout << "scc count: " << solver.getStronglyConnectedComponentsCount() << std::endl;

    std::unordered_map<int, std::vector<int>> scc_map;
    for (int i = 0; i < scc.size(); ++i) {
        int component_num = scc[i];
        scc_map[component_num].push_back(i); 
    }

    // Nodes: [0, 1, 2] form a Strongly Connected Component.
    // Nodes: [3, 7] form a Strongly Connected Component.
    // Nodes: [4, 5, 6] form a Strongly Connected Component.
    for (auto [component_num, nums] : scc_map) {
        for (int num : nums) {
            std::cout << num << " "; 
        }
        std::cout << std::endl;
    }


    return 0;
}