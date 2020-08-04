#include <iostream>
#include <vector>
#include <numeric>


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
        return size_[find(src)];
    }

};



int main() {

    UnionFind uf(5);

    uf.createUnion(0, 1);
    uf.createUnion(0, 2);

    // 3
    std::cout << uf.size(0) << std::endl;

    // true
    std::cout << uf.connected(1, 2) << std::endl;

    // false
    std::cout << uf.connected(1, 3) << std::endl;

   
    return 0;
}