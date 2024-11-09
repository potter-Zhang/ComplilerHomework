#include "Tree.h"
#include "TreeScanner.h"
#include<iostream>


void Tree::add_binary_oper(NodeType node_type, char left, char right) {
    std::shared_ptr<TreeNode> right_child;
    if (right == (char)Symbol::EMPTY_NODE) {
        std::shared_ptr<EmptyNode> right_leaf = std::make_shared<EmptyNode>();
        right_child = right_leaf;
    }
    else if (right != (char)Symbol::SUBTREE) {
        std::shared_ptr<ValueNode> right_leaf = std::make_shared<ValueNode>(++index_, right);
        right_child = right_leaf;
        leaf_.emplace_back(right_leaf);
        symbols_.insert(right);
    }
    else {
        right_child = std::move(roots_.top());
        roots_.pop();
    }

    std::shared_ptr<TreeNode> left_child;
    if (left == (char)Symbol::EMPTY_NODE) {
        std::shared_ptr<EmptyNode> left_leaf = std::make_shared<EmptyNode>();
        right_child = left_leaf;
    }
    else if (left != (char)Symbol::SUBTREE) {
        std::shared_ptr<ValueNode> left_leaf = std::make_shared<ValueNode>(++index_, left);
        left_child = left_leaf;
        leaf_.emplace_back(left_leaf);
        symbols_.insert(left);
    }
    else {
        left_child = std::move(roots_.top());
        roots_.pop();
    }
    
    

    if (node_type == NodeType::CAT) {
        bool nullable = left_child->nullable() && right_child->nullable();
        roots_.push(std::make_shared<CatNode>(left_child, right_child, nullable));
    }
    else {
        bool nullable = left_child->nullable() || right_child->nullable();
        roots_.push(std::make_shared<OrNode>(left_child, right_child, nullable));
    }
}

void Tree::add_star_oper(char child) {
    if (child == (char)Symbol::SUBTREE) {
        std::shared_ptr<StarNode> star_node = std::make_shared<StarNode>(roots_.top());
        roots_.pop();
        roots_.push(star_node);
        
    }
    else {
        std::shared_ptr<ValueNode> child_expr;
        child_expr = std::make_shared<ValueNode>(++index_, child);
        symbols_.insert(child);
        leaf_.emplace_back(child_expr);
        std::shared_ptr<TreeNode> root = child_expr;
        roots_.push(std::make_shared<StarNode>(root));
    }
   
}

void Tree::create(std::string regex) {
    TreeScanner scanner;
    scanner.scan(regex, *this);
}

void Tree::calc_followpos() {
    if (roots_.empty()) {
        return;
    }

    std::shared_ptr<TreeNode> root = roots_.top();
    calc_followpos_internal(root);
    symbols_.erase('#');
}

void Tree::calc_followpos_internal(std::shared_ptr<TreeNode> root) {
    if (root->node_type() == NodeType::CAT) {
        CatNode& cat_node = dynamic_cast<CatNode&>(*root);
        std::unordered_set<int> left_last_pos, right_first_pos;
        cat_node.left_child()->lastpos(left_last_pos);
        cat_node.right_child()->firstpos(right_first_pos);
        for (int i : left_last_pos) {
            leaf_[i]->set_followpos(right_first_pos);
        }
        calc_followpos_internal(cat_node.left_child());
        calc_followpos_internal(cat_node.right_child());
    }
    else if (root->node_type() == NodeType::STAR) {
        StarNode& star_node = dynamic_cast<StarNode&>(*root);
        std::unordered_set<int> left_last_pos, right_first_pos;
        star_node.child()->lastpos(left_last_pos);
        star_node.child()->firstpos(right_first_pos);
        for (int i : left_last_pos) {
            leaf_[i]->set_followpos(right_first_pos);
        }
        calc_followpos_internal(star_node.child());
    }
    else if (root->node_type() == NodeType::OR) {
        OrNode& or_node = dynamic_cast<OrNode&>(*root);
        calc_followpos_internal(or_node.left_child());
        calc_followpos_internal(or_node.right_child());
    }
}

void Tree::print() {
    if (roots_.empty()) {
        return;
    }

    std::shared_ptr<TreeNode> root = roots_.top();
    print_internal(root);
}

void Tree::print_internal(std::shared_ptr<TreeNode> root, int level) {
    
    if (root->node_type() == NodeType::VALUE) {
        ValueNode &value_node = dynamic_cast<ValueNode &>(*root);
        
        std::cout << std::string(level, '\t');
       
        std::cout << value_node.symbol() << std::endl;
    }
    else if (root->node_type() == NodeType::OR) {
        OrNode &or_node = dynamic_cast<OrNode &>(*root);
        print_internal(or_node.left_child(), level + 1);
        std::cout << std::string(level, '\t');
        std::cout << '|' << "-------|" << std::endl;
        print_internal(or_node.right_child(), level + 1);
    } 
    else if (root->node_type() == NodeType::CAT) {
        CatNode &cat_node = dynamic_cast<CatNode &>(*root);
        print_internal(cat_node.left_child(), level + 1);
        std::cout << std::string(level, '\t');
        std::cout << '+' << "-------|" << std::endl;
        print_internal(cat_node.right_child(), level + 1);
    }
    else if (root->node_type() == NodeType::STAR) {
        StarNode &star_node = dynamic_cast<StarNode &>(*root);
        std::cout << std::string(level, '\t');
        std::cout << '*' << "-------|" << std::endl;
        print_internal(star_node.child(), level + 1);
    }
    else {
        std::cout << std::string(level, '\t');
        std::cout << "epsilon" << std::endl;
    }
}

bool Tree::contains_end_tag(std::unordered_set<int>& u)
{
    if (end_index_ == -1) {
        for (int i = 0; i < leaf_.size(); i++) {
            if (leaf_[i]->symbol() == '#') {
                end_index_ = i;
                break;
            }
        }
    }

    return u.find(end_index_) != u.end();
}
