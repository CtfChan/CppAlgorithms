#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <math.h>
#include <algorithm>

struct Node {
    int id;
    double value;
};

struct Edge {
    double cost;
    int from, to;
};
using Graph = std::vector<std::vector<Edge>>;

constexpr double POS_INF = std::numeric_limits<double>::infinity();

class Dijkstra {
private:   
    Graph graph_;
    int n_;
    std::vector<double> dist_;
    std::vector<int> prev_;


public:
    Dijkstra(Graph& graph) : n_(graph.size()), graph_(graph) {}

    double distanceToNode(int start, int end) {
        dist_ = std::vector<double>(n_, POS_INF);
        dist_[start] = 0;

        // min heap
        auto cmp = [](Node a, Node b ) { return a.value > b.value; };
        std::priority_queue<Node, std::vector<Node>, decltype(cmp)> pq(cmp);
        pq.push({start, 0});

        // visitation history
        std::vector<bool> visited(n_, false);
        prev_ = std::vector<int>(n_, -1);

        while (pq.empty() == false) {
            auto [id, value] = pq.top();
            pq.pop();

            // already found better path, can't get better
            if (dist_[id] < value ) 
                continue;

            for (auto [cost, from, to] : graph_[id]) {
                // can't get shorter path revisitng a node already visited
                if (visited[to] == true)
                    continue;
                
                // try to relax edges
                double new_dist = dist_[id] + cost;
                if (new_dist < dist_[to]) {
                    prev_[to] = from;
                    dist_[to] = new_dist;
                    pq.push({to, new_dist});
                }
            }

            // once we've visited all nodes spanning from end node, can't get better
            if (id == end)
                return dist_[end];
        }

        return POS_INF;
    }


    std::pair<double, std::vector<int>> reconstructPath(int start, int end) {
        std::vector<int> path;
        
        double dist = distanceToNode(start, end);
        if (isinf(dist) == true)
            return {dist, path};
        
        int at = end;
        while (at != start) {
            path.push_back(at);
            at = prev_[at];
        }
        path.push_back(start);

        std::reverse(path.begin(), path.end());
        return {dist, path};
    }

};

int main() {
    int n = 5;
    Graph g = std::vector<std::vector<Edge>>(n); // (cost, from, to)
    g[0].push_back({4, 0, 1});
    g[0].push_back({1, 0, 2});
    g[1].push_back({1, 1, 3});
    g[2].push_back({2, 2, 1});
    g[2].push_back({5, 2, 3});
    g[3].push_back({3, 3, 4});


    Dijkstra solver(g);
    auto [cost, path] = solver.reconstructPath(0, 4);

    std::cout << "Cost: " << cost << std::endl; // should be 7
    // cheapest path: 0 2 1 3 4    
    for (int n : path) 
        std::cout << n << " ";
    std::cout << std::endl;


    return 0;
}