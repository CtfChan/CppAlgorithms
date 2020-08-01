#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>

using Graph = std::vector<std::vector<int>>;

/**
 * Root Tree Source Code
 * 
 */
struct TreeNode;
using TreeNodeSharedPtr = std::shared_ptr<TreeNode>;
using TreeNodeWeakPtr = std::weak_ptr<TreeNode>;

struct TreeNode {
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
    auto node_parent = node->parent.lock();
    for (int child_id : graph[node->id]) {
        // don't add edges that will lead us back to the parent 
        if (node_parent != nullptr && child_id == node_parent->id)
            continue;
        
        auto child_ptr = std::make_shared<TreeNode>(child_id, node);
        node->children.push_back(child_ptr);

        buildTree(graph, child_ptr);
    }
    return node;
}

TreeNodeSharedPtr rootTree(Graph& graph, int id) {
    auto root = std::make_shared<TreeNode>(id);
    return buildTree(graph, root);
}


std::vector<int> findTreeCenters(Graph& tree) {
    int n = tree.size();
    std::vector<int> degree(n, 0);
    std::vector<int> leaves; leaves.reserve(n);

    // compute degree of all nodes and remember initial leaves
    for (int node = 0; node < n; ++node) {
        degree[node] = tree[node].size();
        if (degree[node] <= 1) {
            leaves.push_back(node);
            degree[node] = 0;
        }
    }

    // iteratively peel off nodes and decrement degree of neighbours of leaves
    int processed_leaves = leaves.size();
    while (processed_leaves < n) {
        std::vector<int> new_leaves;
        for (int leaf : leaves) {
            for (int neighbour : tree[leaf]) {
                --degree[neighbour];
                if (degree[neighbour] == 1) {
                    new_leaves.push_back(neighbour);
                }
            }
            degree[leaf] = 0;
        }
        leaves = new_leaves;
        processed_leaves += leaves.size();
    }

    return leaves;
}



// recursively encode children 
std::string encode(TreeNodeSharedPtr node) {
    if (node == nullptr)
        return "";

    std::vector<std::string> labels;
    for (auto child_ptr : node->children) {
        labels.push_back(encode(child_ptr));
    }

    std::sort(labels.begin(), labels.end());
    std::string children_encoding = "";
    for (auto label : labels) 
        children_encoding += label;
    
    return "(" + children_encoding + ")";
}



bool treesAreIsomorphic(Graph& tree1, Graph& tree2) {
    if (tree1.empty() || tree2.empty())
        return false;

    // find centers for each tree
    auto centers1 = findTreeCenters(tree1);
    auto centers2 = findTreeCenters(tree2);

    // root each tree1 at center and encode
    auto rooted_tree1 = rootTree(tree1, centers1[0]);
    auto encoding_tree1 = encode(rooted_tree1);

    // compare tree1 encoding to possible encodings of tree2 
    // there could be 1 or 2 centers
    for (int center : centers2) {
        auto rooted_tree2 = rootTree(tree2, center);
        auto encoding_tree2 = encode(rooted_tree2);
        if (encoding_tree1 == encoding_tree2)
            return true;
    }

    return false;
}





void addUndirectedEdge(Graph& graph, int from, int to) {
    graph[from].push_back(to);
    graph[to].push_back(from);
}


void testEncoding() {
    Graph tree = std::vector<std::vector<int>>(10);
    addUndirectedEdge(tree, 0, 2);
    addUndirectedEdge(tree, 0, 1);
    addUndirectedEdge(tree, 0, 3);
    addUndirectedEdge(tree, 2, 6);
    addUndirectedEdge(tree, 2, 7);
    addUndirectedEdge(tree, 1, 4);
    addUndirectedEdge(tree, 1, 5);
    addUndirectedEdge(tree, 5, 9);
    addUndirectedEdge(tree, 3, 8);

    TreeNodeSharedPtr root = rootTree(tree, 0);

    std::string encoding = encode(root);
    std::cout << "Computed: " << encoding << std::endl;
    std::cout << "Target:   " << "(((())())(()())(()))" << std::endl;
    std::cout << "Match results: " << (encoding == "(((())())(()())(()))") << std::endl;
}

void testIsomorphism() {
    Graph tree1 = std::vector<std::vector<int>>(5);
    addUndirectedEdge(tree1, 2, 0);
    addUndirectedEdge(tree1, 3, 4);
    addUndirectedEdge(tree1, 2, 1);
    addUndirectedEdge(tree1, 2, 3);

    Graph tree2 = std::vector<std::vector<int>>(5);
    addUndirectedEdge(tree2, 1, 0);
    addUndirectedEdge(tree2, 2, 4);
    addUndirectedEdge(tree2, 1, 3);
    addUndirectedEdge(tree2, 1, 2);

    std::cout << "The 2 trees should be isomorphic: " << treesAreIsomorphic(tree1, tree2) << std::endl;


}

int main() {
    testEncoding();
    testIsomorphism();

    return 0;
}