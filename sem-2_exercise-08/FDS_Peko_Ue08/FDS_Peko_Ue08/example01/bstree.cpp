#include "bstree.h"
#include <iomanip>
#include <string>

bstree::bstree() : root(nullptr), node_count(0) {}

bstree::bstree(bstree const& src) : root(nullptr), node_count(0) {
    root = copy_tree(src.root);
    node_count = src.node_count;
}

bstree::~bstree() {
    destroy_tree(root);
}

void bstree::apply(function_t f) {
    apply_recursive(root, f);
}

bool bstree::at(int n, value_t& value) const {
    if (n < 0 || n >= node_count) {
        return false;
    }
    
    int current_index = 0;
    return at_recursive(root, current_index, n, value);
}

int bstree::clear() {
    int old_count = node_count;
    destroy_tree(root);
    root = nullptr;
    node_count = 0;
    return old_count;
}

bool bstree::contains(value_t const& value) const {
    return contains_recursive(root, value);
}

int bstree::count(value_t const& value) const {
    return count_recursive(root, value);
}

bool bstree::empty() const {
    return root == nullptr;
}

bool bstree::equals(bstree const& rhs) const {
    if (node_count != rhs.node_count) {
        return false;
    }
    return equals_recursive(root, rhs.root);
}

void bstree::insert(value_t const& value) {
    if (root == nullptr) {
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

std::ostream& bstree::print(std::ostream& out) const {
    if (root == nullptr) {
        out << "Empty tree";
    } else {
        print_recursive(out, root);
    }
    return out;
}

std::ostream& bstree::print_2d(std::ostream& out) const {
    if (root == nullptr) {
        out << "Empty tree";
    } else {
        std::vector<bool> vert_lines;
        print_2d_recursive(out, root, 0, vert_lines);
    }
    return out;
}

std::ostream& bstree::print_2d_upright(std::ostream& out) const {
    if (root == nullptr) {
        out << "Empty tree";
    } else {
        print_2d_upright_recursive(out, root, 0);
    }
    return out;
}

bool bstree::remove(value_t const& value) {
    if (root == nullptr) {
        return false;
    }
    
    bool removed = false;
    root = remove_node(root, value, removed);
    
    if (removed) {
        node_count -= 1;
    }
    
    return removed;
}

int bstree::remove_all(value_t const& value) {
    int removed_count = remove_all_recursive(root, value);
    node_count -= removed_count;
    return removed_count;
}

int bstree::size() const {
    return node_count;
}

// Private helper methods
void bstree::destroy_tree(node_t* node) {
    if (node == nullptr) {
        return;
    }
    
    destroy_tree(node->left);
    destroy_tree(node->right);
    delete node;
}

bstree::node_t* bstree::copy_tree(node_t* node) {
    if (node == nullptr) {
        return nullptr;
    }
    
    node_t* new_node = new node_t(node->value);
    new_node->left = copy_tree(node->left);
    new_node->right = copy_tree(node->right);
    
    return new_node;
}

void bstree::apply_recursive(node_t* node, function_t f) {
    if (node == nullptr) {
        return;
    }
    
    apply_recursive(node->left, f);
    f(node->value);
    apply_recursive(node->right, f);
}

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
    
    current_index += 1;
    
    return at_recursive(node->right, current_index, target_index, value);
}

bool bstree::contains_recursive(node_t* node, value_t const& value) const {
    if (node == nullptr) {
        return false;
    }
    
    if (node->value == value) {
        return true;
    }
    
    if (value < node->value) {
        return contains_recursive(node->left, value);
    } else {
        return contains_recursive(node->right, value);
    }
}

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

std::ostream& bstree::print_recursive(std::ostream& out, node_t* node) const {
    if (node == nullptr) {
        return out;
    }
    
    print_recursive(out, node->left);
    out << node->value << " ";
    print_recursive(out, node->right);
    
    return out;
}

void bstree::print_2d_recursive(std::ostream& out, node_t* node, int space, std::vector<bool>& vert_lines) const {
    if (node == nullptr) {
        return;
    }
    
    // Increase distance between levels
    int level_space = 4;
    space += level_space;
    
    // Store current size of vert_lines
    int prev_size = vert_lines.size();
    
    // Ensure vert_lines has enough space for this level
    while (vert_lines.size() < space / level_space) {
        vert_lines.push_back(false);
    }
    
    // Set vertical line for current node's level
    if (space > level_space) {
        vert_lines[space / level_space - 1] = true;
    }
    
    // Process right child first (goes to top of the output)
    print_2d_recursive(out, node->right, space, vert_lines);
    
    // Line for this node
    out << std::endl;
    
    // Print vertical lines for upper levels
    for (int i = 0; i < space - level_space; i++) {
        if (i % level_space == 0 && vert_lines[i / level_space]) {
            out << "|";
        } else {
            out << " ";
        }
    }
    
    // Print connector and node value
    if (space > level_space) {
        out << "-";
    }
    out << node->value;
    
    // Turn off vertical line for this level before processing left child
    if (space > level_space) {
        vert_lines[space / level_space - 1] = false;
    }
    
    // Process left child
    print_2d_recursive(out, node->left, space, vert_lines);
    
    // Restore vert_lines to previous size
    vert_lines.resize(prev_size);
}

void bstree::print_2d_upright_recursive(std::ostream& out, node_t* node, int space) const {
    if (node == nullptr) {
        return;
    }
    
    space += 2;
    
    print_2d_upright_recursive(out, node->left, space);
    
    out << std::endl;
    for (int i = 2; i < space; i++) {
        out << " ";
    }
    out << node->value << std::endl;
    
    print_2d_upright_recursive(out, node->right, space);
}

bstree::node_t* bstree::remove_node(node_t* node, value_t const& value, bool& removed) {
    if (node == nullptr) {
        return nullptr;
    }
    
    if (value < node->value) {
        node->left = remove_node(node->left, value, removed);
    } else if (value > node->value) {
        node->right = remove_node(node->right, value, removed);
    } else {
        removed = true;
        
        if (node->left == nullptr) {
            node_t* temp = node->right;
            delete node;
            return temp;
        } else if (node->right == nullptr) {
            node_t* temp = node->left;
            delete node;
            return temp;
        }
        
        node_t* temp = find_min_value_node(node->right);
        node->value = temp->value;
        node->right = remove_node(node->right, temp->value, removed);
        removed = true; // Reset to true since we successfully removed the node
    }
    
    return node;
}

bstree::node_t* bstree::find_min_value_node(node_t* node) {
    node_t* current = node;
    
    while (current && current->left != nullptr) {
        current = current->left;
    }
    
    return current;
}

int bstree::remove_all_recursive(node_t*& node, value_t const& value) {
    if (node == nullptr) {
        return 0;
    }
    
    int count = 0;
    count += remove_all_recursive(node->left, value);
    count += remove_all_recursive(node->right, value);
    
    if (node->value == value) {
        if (node->left == nullptr) {
            node_t* temp = node->right;
            delete node;
            node = temp;
            count += 1;
        } else if (node->right == nullptr) {
            node_t* temp = node->left;
            delete node;
            node = temp;
            count += 1;
        } else {
            node_t* temp = find_min_value_node(node->right);
            node->value = temp->value;
            count += remove_all_recursive(node->right, temp->value);
        }
    }
    
    return count;
} 