#pragma once

#include "TreeNode.h"
#include <vector>
#include <string>
#include <stack>

enum Symbol { SUBTREE = 0x80, EMPTY_NODE = 0x81 };

class Tree {
public: 
    Tree() = default;
    void add_cat_oper(char left, char right) { add_binary_oper(NodeType::CAT, left, right); }
    void add_or_oper(char left, char right) { add_binary_oper(NodeType::OR, left, right); }
    void add_binary_oper(NodeType node_type, char left, char right);
    
    void add_star_oper(char child);

    void create(std::string regex);

    void calc_followpos();
    void calc_followpos_internal(std::shared_ptr<TreeNode> root);

    void print();
    void print_internal(std::shared_ptr<TreeNode> root, int level = 0);

    std::shared_ptr<TreeNode> root() { return roots_.top(); }

    std::vector<std::shared_ptr<ValueNode>>& leaf() { return leaf_; }
    std::unordered_set<char>& symbols() { return symbols_; }

    bool contains_end_tag(std::unordered_set<int>& u);

private:
    std::stack<std::shared_ptr<TreeNode>> roots_;
    std::vector<std::shared_ptr<ValueNode>> leaf_;
    std::unordered_set<char> symbols_;
    int end_index_ = -1;
    int index_ = -1;
};