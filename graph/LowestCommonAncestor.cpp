#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

using Graph = std::vector<std::vector<int>>;


struct TreeNode;
using TreeNodeSharedPtr = std::shared_ptr<TreeNode>;
using TreeNodeWeakPtr = std::weak_ptr<TreeNode>;

struct TreeNode {
    int nodes_in_subtree;
    int id;
    std::vector<TreeNodeSharedPtr> children;
    TreeNodeWeakPtr parent;

    TreeNode (int input_id) : 
        id(input_id) {}
    
    TreeNode (int input_id, TreeNodeSharedPtr p) : 
        id(input_id), parent(p) {}
};


// Use DFS to construct tree 
TreeNodeSharedPtr buildTree(Graph& graph, TreeNodeSharedPtr node) {
    int subtree_count = 1; // include yourself
    auto node_parent = node->parent.lock();
    for (int child_id : graph[node->id]) {
        // don't add edges that will lead us back to the parent 
        if (node_parent != nullptr && child_id == node_parent->id)
            continue;
        
        auto child_ptr = std::make_shared<TreeNode>(child_id, node);
        node->children.push_back(child_ptr);

        buildTree(graph, child_ptr);

        subtree_count += child_ptr->nodes_in_subtree;
    }

    node->nodes_in_subtree = subtree_count;
    return node;
}

TreeNodeSharedPtr rootTree(Graph& graph, int id) {
    auto root = std::make_shared<TreeNode>(id);
    return buildTree(graph, root);
}


class LowestCommonAncestorEulerTour {
private:
    int num_nodes_;

    // both of these will be the size of the euler tour (2n-1)
    std::vector<long> depth_;
    std::vector<TreeNodeSharedPtr> node_order_;

    // last occurence of a particular TreeNode
    std::vector<int> last_; 

    int tour_index = 0;

    void dfs(TreeNodeSharedPtr node, int depth) {
        if (node == nullptr)
            return;
        
        visit(node, depth); // first encounter with node
        for (auto child : node->children) {
            dfs(child, depth + 1);
            visit(node, depth); // secondary encounter
        }  
    }

    void visit(TreeNodeSharedPtr node, int depth) {
        node_order_[tour_index] = node;
        depth_[tour_index] = depth;
        last_[node->id] = tour_index;
        ++tour_index;
    }

public:
    LowestCommonAncestorEulerTour(TreeNodeSharedPtr root) {
        num_nodes_ = root->nodes_in_subtree;

        int euler_tour_size = 2 * num_nodes_ - 1;   
        
        depth_ = std::vector<long>(euler_tour_size);
        node_order_ = std::vector<TreeNodeSharedPtr>(euler_tour_size);
        last_ = std::vector<int>(num_nodes_);

        dfs(root, 0);
    }

    TreeNodeSharedPtr lowestCommonAncestor(int idx1, int idx2) {
        int left = std::min(last_[idx1], last_[idx2]);
        int right = std::max(last_[idx1], last_[idx2]);

        auto depth_min = std::min_element(depth_.begin()+left, depth_.begin()+right);
        int idx = std::distance(depth_.begin(), depth_min);
        return node_order_[idx];
    }

};

void addUndirectedEdge(Graph& graph, int from, int to) {    
    graph[from].push_back(to);
    graph[to].push_back(from);
}

int main() {
    Graph tree(17);

    addUndirectedEdge(tree, 0, 1);
    addUndirectedEdge(tree, 0, 2);
    addUndirectedEdge(tree, 1, 3);
    addUndirectedEdge(tree, 1, 4);
    addUndirectedEdge(tree, 2, 5);
    addUndirectedEdge(tree, 2, 6);
    addUndirectedEdge(tree, 2, 7);
    addUndirectedEdge(tree, 3, 8);
    addUndirectedEdge(tree, 3, 9);
    addUndirectedEdge(tree, 5, 10);
    addUndirectedEdge(tree, 5, 11);
    addUndirectedEdge(tree, 7, 12);
    addUndirectedEdge(tree, 7, 13);
    addUndirectedEdge(tree, 11, 14);
    addUndirectedEdge(tree, 11, 15);
    addUndirectedEdge(tree, 11, 16);

    auto root = rootTree(tree, 0);

    LowestCommonAncestorEulerTour solver(root);

    // 2
    auto lca = solver.lowestCommonAncestor(13, 14);
    std::cout << "lca(13,14): " << lca->id << std::endl;

    // 0
    lca = solver.lowestCommonAncestor(9, 11);
    std::cout << "lca(9,11): " << lca->id << std::endl;

    // 12
    lca = solver.lowestCommonAncestor(12, 12);
    std::cout << "lca(12,12): " <<  lca->id << std::endl;

    return 0;
}