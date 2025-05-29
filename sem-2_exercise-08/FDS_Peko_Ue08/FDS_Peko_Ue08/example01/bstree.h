#pragma once

#include <iostream>
#include <cstddef>
#include <vector>

class bstree {
  public:
    using value_t = int;
    using function_t = void (*)(value_t&);

    bstree();
    bstree(bstree const& src);
    ~bstree();

    void apply(function_t f);
    bool at(int n, value_t& value) const;
    int clear();
    bool contains(value_t const& value) const;
    int count(value_t const& value) const;
    bool empty() const;
    bool equals(bstree const& rhs) const;
    void insert(value_t const& value);
    std::ostream& print(std::ostream& out = std::cout) const;
    std::ostream& print_2d(std::ostream& out = std::cout) const;
    std::ostream& print_2d_upright(std::ostream& out = std::cout) const;
    bool remove(value_t const& value);
    int remove_all(value_t const& value);
    int size() const;

  private:
    struct node_t {
      value_t value;
      node_t* left;
      node_t* right;
      
      node_t(value_t v) : value(v), left(nullptr), right(nullptr) {}
    };

    node_t* root;
    int node_count;

    // Helper methods
    void destroy_tree(node_t* node);
    node_t* copy_tree(node_t* node);
    void apply_recursive(node_t* node, function_t f);
    bool at_recursive(node_t* node, int& current_index, int target_index, value_t& value) const;
    bool contains_recursive(node_t* node, value_t const& value) const;
    int count_recursive(node_t* node, value_t const& value) const;
    bool equals_recursive(node_t* lhs, node_t* rhs) const;
    std::ostream& print_recursive(std::ostream& out, node_t* node) const;
    void print_2d_recursive(std::ostream& out, node_t* node, int depth) const;
    void print_2d_upright(std::ostream& out, node_t* node) const;
    int calculate_space_required_upright(node_t* node, int current_depth) const;
    std::vector<node_t*> nodes_at_depth(node_t* node, int depth) const;
    node_t* remove_node(node_t* node, value_t const& value, bool& removed);
    node_t* find_min_value_node(node_t* node);
    int remove_all_recursive(node_t*& node, value_t const& value);
}; 