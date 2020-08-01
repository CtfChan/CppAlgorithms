#include <iostream>
#include <vector>
#include <memory>

using Graph = std::vector<std::vector<int>>;


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


void addUndirectedEdge(Graph& graph, int from, int to) {
    graph[from].push_back(to);
    graph[to].push_back(from);
}

int main() {
    Graph graph = std::vector<std::vector<int>>(9);

    addUndirectedEdge(graph, 0, 1);
    addUndirectedEdge(graph, 2, 1);
    addUndirectedEdge(graph, 2, 3);
    addUndirectedEdge(graph, 3, 4);
    addUndirectedEdge(graph, 5, 3);
    addUndirectedEdge(graph, 2, 6);
    addUndirectedEdge(graph, 6, 7);
    addUndirectedEdge(graph, 6, 8);


    // Rooted at 6 the tree should look like:
    //           6
    //      2    7     8
    //    1   3
    //  0   4  5

    TreeNodeSharedPtr root = rootTree(graph, 6);

    // [6]
    std::cout << root->id << std::endl;

    // [2, 7, 8]
    for (auto child : root->children) 
        std::cout << child->id << " ";
    std::cout << std::endl;
    
    // [1, 3]
    for (auto child : root->children[0]->children) 
        std::cout << child->id << " ";
    std::cout << std::endl;

    // [0], [4, 5]
    for (auto child : root->children[0]->children[0]->children) 
        std::cout << child->id << " ";
    for (auto child : root->children[0]->children[1]->children) 
        std::cout << child->id << " ";
    std::cout << std::endl;


    return 0;
}