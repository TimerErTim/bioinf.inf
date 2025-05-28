#include "bstree.h"
#include <iomanip>
#include <string>

// Private helper methods

// Destroys the tree by recursively deleting all subnodes
void bstree::destroy_tree(node_t* node) {
    if (node == nullptr) {
        return;
    }
    
    destroy_tree(node->left);
    destroy_tree(node->right);
    delete node;
}

// Copies the tree by recursively copying all subnodes
bstree::node_t* bstree::copy_tree(node_t* node) {
    if (node == nullptr) {
        return nullptr;
    }
    
    node_t* new_node = new node_t(node->value);
    new_node->left = copy_tree(node->left);
    new_node->right = copy_tree(node->right);
    
    return new_node;
}

// Applies a function to all nodes in the tree by recursively traversing the tree in-order
void bstree::apply_recursive(node_t* node, function_t f) {
    if (node == nullptr) {
        return;
    }
    
    apply_recursive(node->left, f);
    f(node->value);
    apply_recursive(node->right, f);
}

// Checks if the tree contains a value by recursively traversing the tree
bool bstree::at_recursive(node_t* node, int& current_index, int target_index, value_t& value) const {
    if (node == nullptr) {
        return false;
    }
    
    if (at_recursive(node->left, current_index, target_index, value)) {
        return true;
    }

    if (current_index == target_index) {
        value = node->value;
        return true;
    }
    
    // Increment the current index for the next node if in order
    current_index += 1;
    
    return at_recursive(node->right, current_index, target_index, value);
}

// Checks if the tree contains a value by recursively searching the tree
bool bstree::contains_recursive(node_t* node, value_t const& value) const {
    if (node == nullptr) {
        return false;
    }
    
    if (node->value == value) {
        return true;
    }
    
    if (value < node->value) {
        // Only check left side which only contains smaller values
        return contains_recursive(node->left, value);
    } else {
        // Only check right side which only contains larger values
        return contains_recursive(node->right, value);
    }
}

// Counts the number of occurrences of a value using recursion
int bstree::count_recursive(node_t* node, value_t const& value) const {
    if (node == nullptr) {
        return 0;
    }
    
    int count = 0;
    if (node->value == value) {
        count = 1;
    }
    
    count += count_recursive(node->left, value);
    count += count_recursive(node->right, value);
    
    return count;
}

// Checks if two trees are equal by recursively comparing all nodes
bool bstree::equals_recursive(node_t* lhs, node_t* rhs) const {
    if (lhs == nullptr && rhs == nullptr) {
        return true;
    }
    
    if (lhs == nullptr || rhs == nullptr) {
        return false;
    }
    
    return lhs->value == rhs->value &&
           equals_recursive(lhs->left, rhs->left) &&
           equals_recursive(lhs->right, rhs->right);
}

// Prints the tree in a human-readable format by recursively traversing the tree in-order
std::ostream& bstree::print_recursive(std::ostream& out, node_t* node) const {
    if (node == nullptr) {
        return out;
    }
    
    print_recursive(out, node->left);
    if (node->left != nullptr) {
        out << " < ";
    }
    out << node->value;
    if (node->right != nullptr) {
        out << " > ";
    }
    print_recursive(out, node->right);
    
    return out;
}

void bstree::print_2d_recursive(std::ostream& out, node_t* node, int depth) const {
    if (node == nullptr) {
        return;
    }

    int step_size = 3;

    print_2d_recursive(out, node->left, depth + 1);

    for (int i = 0; i < depth; i++) {
        for (int j = 0; j < step_size; j++) {
        out << " ";
    }
    out << node->value << std::endl;

    print_2d_recursive(out, node->right, depth + 1);
}

void bstree::print_2d_upright_recursive(std::ostream& out, node_t* node, int depth) const {
    if (node == nullptr) {
        return 0;
    }
    
    print_2d_upright_recursive(out, node->left, depth + 1);
    
    out << std::endl;
    for (int i = 2; i < depth; i++) {
        out << " ";
    }
    out << node->value << std::endl;
    
    print_2d_upright_recursive(out, node->right, depth + 1);

    return 0;
}

// Removes a node from the tree, guaranteeing that the tree is still a BST
bstree::node_t* bstree::remove_node(node_t* node, value_t const& value, bool& removed) {
    if (node == nullptr) {
        return nullptr;
    }
    
    if (value < node->value) {
        // Replace left side with the result of removing the node from the left side
        node->left = remove_node(node->left, value, removed);
    } else if (value > node->value) {
        // Replace right side with the result of removing the node from the right side
        node->right = remove_node(node->right, value, removed);
    } else {
        // We arrived at the node we want to remove
        if (node->left == nullptr) {
            // If the left side is empty, simply replace the node with the right side
            node_t* temp = node->right;
            delete node;
            return temp;
        } else if (node->right == nullptr) {
            // If the right side is empty, simply replace the node with the left side
            node_t* temp = node->left;
            delete node;
            return temp;
        }
        
        // If the node has both left and right sides, find the minimum value in the right side
        node_t* temp = find_min_value_node(node->right);
        // Replace the node with the minimum value
        node->value = temp->value;
        // Remove the minimum value from the right side
        node->right = remove_node(node->right, temp->value, removed);
        removed = true; // Set to true since we successfully removed the node
    }
    
    return node;
}

// Finds the minimum value in the tree by finding the leftmost node
bstree::node_t* bstree::find_min_value_node(node_t* node) {
    node_t* current = node;
    
    while (current && current->left != nullptr) {
        current = current->left;
    }
    
    return current;
}

// Removes all occurrences of a value from the tree by recursively traversing the tree
int bstree::remove_all_recursive(node_t*& node, value_t const& value) {
    if (node == nullptr) {
        return 0;
    }
    
    int count = 0;
    // Recursively remove all occurrences of the value from children nodes
    count += remove_all_recursive(node->left, value);
    count += remove_all_recursive(node->right, value);
    
    // Remove the current node if it matches the value
    if (node->value == value) {
        if (node->left == nullptr) {
            // If the left side is empty, simply replace the node with the right side
            node_t* temp = node->right;
            delete node;
            node = temp;
        } else if (node->right == nullptr) {
            // If the right side is empty, simply replace the node with the left side
            node_t* temp = node->left;
            delete node;
            node = temp;
        } else {
            // If the node has both left and right sides, find the minimum value in the right side
            node_t* temp = find_min_value_node(node->right);
            // Replace the node with that minimum value
            node->value = temp->value;
            // Remove the minimum value from the right side
            remove_all_recursive(node->right, temp->value);
        }
        count += 1;
    }
    
    return count;
} 


// Public methods

// Default constructor
bstree::bstree() : root(nullptr), node_count(0) {}

// Copy constructor
bstree::bstree(bstree const& src) : root(nullptr), node_count(0) {
    root = copy_tree(src.root);
    node_count = src.node_count;
}

// Destructor
bstree::~bstree() {
    destroy_tree(root);
}

void bstree::apply(function_t f) {
    apply_recursive(root, f);
}

// Returns the value at the nth position in the tree
bool bstree::at(int n, value_t& value) const {
    if (n < 0 || n >= node_count) {
        return false;
    }
    
    int current_index = 0;
    return at_recursive(root, current_index, n, value);
}

// Clears the tree and returns the number of nodes removed
int bstree::clear() {
    int old_count = node_count;
    destroy_tree(root);
    root = nullptr;
    node_count = 0;
    return old_count;
}

// Checks if the tree contains a value
bool bstree::contains(value_t const& value) const {
    return contains_recursive(root, value);
}

// Counts the number of occurrences of a value
int bstree::count(value_t const& value) const {
    return count_recursive(root, value);
}

// Checks if the tree is empty
bool bstree::empty() const {
    return root == nullptr;
}

// Checks if two trees are equal
bool bstree::equals(bstree const& rhs) const {
    if (node_count != rhs.node_count) {
        return false;
    }
    return equals_recursive(root, rhs.root);
}

// Inserts a value into the tree
void bstree::insert(value_t const& value) {
    if (root == nullptr) {
        // Initial case where the tree is empty
        root = new node_t(value);
        node_count += 1;
        return;
    }
    
    node_t* current = root;
    while (true) {
        if (value < current->value) {
            if (current->left == nullptr) {
                current->left = new node_t(value);
                node_count += 1;
                return;
            }
            current = current->left;
        } else {
            if (current->right == nullptr) {
                current->right = new node_t(value);
                node_count += 1;
                return;
            }
            current = current->right;
        }
    }
}


// Prints a flat representation of the tree
std::ostream& bstree::print(std::ostream& out) const {
    if (root == nullptr) {
        out << "[]";
    } else {
        out << "[";
        print_recursive(out, root) << "]";
    }
    return out;
}

// Prints a 2D representation of the tree
std::ostream& bstree::print_2d(std::ostream& out) const {
    if (root == nullptr) {
        out << "nulltree";
    } else {
        std::vector<bool> vert_lines;
        print_2d_recursive(out, root, 0, vert_lines);
    }
    return out;
}

// Prints a 2D representation of the tree in an upright format
std::ostream& bstree::print_2d_upright(std::ostream& out) const {
    if (root == nullptr) {
        out << "nulltree";
    } else {
        print_2d_upright_recursive(out, root, 0);
    }
    return out;
}

// Removes a node from the tree
bool bstree::remove(value_t const& value) {
    if (root == nullptr) {
        return false;
    }
    
    bool removed = false;
    root = remove_node(root, value, removed);
    
    if (removed) {
        // Decrement the node count since we removed a node
        node_count -= 1;
    }
    
    return removed;
}

// Removes all occurrences of a value from the tree
int bstree::remove_all(value_t const& value) {
    int removed_count = remove_all_recursive(root, value);
    node_count -= removed_count;
    return removed_count;
}

// Returns the number of nodes in the tree
int bstree::size() const {
    return node_count;
}
