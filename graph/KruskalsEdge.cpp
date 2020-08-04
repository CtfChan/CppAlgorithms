#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

struct Edge {
    int from, to, cost;
};

class UnionFind {
private:
    std::vector<int> parent_;
    std::vector<int> size_;

public:
    UnionFind(int n) {
        parent_.resize(n);
        std::iota(parent_.begin(), parent_.end(), 0);
        size_ = parent_;
    }

    int find(int src) {
        // find root node
        int root = src;
        while (root != parent_[root])
            root = parent_[root];

        // path compression, traverse from src->root, set parents of all nodes to root
        int curr = src;
        while (curr != root) {
            int next = parent_[curr];
            parent_[curr] = root;
            curr = next;
        }      

        return root;
    }

    void createUnion(int src, int target) {
        int root_src = find(src);
        int root_target = find(target);
        if (root_src == root_target)
            return;
        
        // merge smaller one into the larger one
        if (size_[root_src] < size_[root_target]) {
            size_[root_target] += size_[root_src];
            parent_[root_src] = root_target;
        } else {
            size_[root_src] += size_[root_target];
            parent_[root_target] = root_src;
        }

    }

    bool connected(int src, int target) {
        return find(src) == find(target);
    }

    int size(int src) {
        return size_[src];
    }

};

class KruskalsEdgeSolver {
private:
    int num_nodes_;
    std::vector<Edge> edges_;

    bool solved_ = false;

    std::vector<Edge> mst_;
    bool mst_exists_ = false;
    long mst_cost_;

    void kruskals() {
        if (solved_)
            return;
        
        // heapify on vector in linear time, we want a min_heap
        auto cmp = [](const Edge& e1, const Edge& e2){ return e1.cost > e2.cost; };
        std::make_heap(edges_.begin(), edges_.end(), cmp);

        UnionFind uf(num_nodes_);

        mst_.reserve(num_nodes_ - 1);

        while (edges_.empty() == true) {
            std::pop_heap(edges_.begin(), edges_.end(), cmp); // moves element to the front
            auto edge = edges_.back();
            edges_.pop_back();

            if (uf.connected(edge.from, edge.to))
                continue;

            uf.createUnion(edge.from, edge.to);

            mst_cost_ += edge.cost;
            mst_.push_back(edge);

            // get out when we've connected all nodes
            if (uf.size(0) == num_nodes_)
                break;
        }

        mst_exists_ = (uf.size(0) == num_nodes_);
        solved_ = true;
    }


public:
    KruskalsEdgeSolver(int num_nodes, const std::vector<Edge>& edges) :
        num_nodes_(num_nodes), edges_(edges) {

    }

    long getMinSpanningTreeCost() {
        kruskals();
        return mst_exists_ ? mst_cost_ : -1;
    }

};


int main() {
    int num_nodes = 10;
    std::vector<Edge> edges;

    edges.push_back({0, 1, 5});
    edges.push_back({1, 2, 4});
    edges.push_back({2, 9, 2});
    edges.push_back({0, 4, 1});
    edges.push_back({0, 3, 4});
    edges.push_back({1, 3, 2});
    edges.push_back({2, 7, 4});
    edges.push_back({2, 8, 1});
    edges.push_back({9, 8, 0});
    edges.push_back({4, 5, 1});
    edges.push_back({5, 6, 7});
    edges.push_back({6, 8, 4});
    edges.push_back({4, 3, 2});
    edges.push_back({5, 3, 5});
    edges.push_back({3, 6,11});
    edges.push_back({6, 7, 1});
    edges.push_back({3, 7, 2});
    edges.push_back({7, 8, 6});


    KruskalsEdgeSolver solver(num_nodes, edges);

    // 14
    std::cout << solver.getMinSpanningTreeCost() << std::endl;

    return 0;
}