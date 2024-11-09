#pragma once
#include <memory>
#include <unordered_set>

enum NodeType { OR, CAT, STAR, VALUE, EMPTY };

class TreeNode {
public:
    TreeNode() = default;
    virtual bool nullable() = 0;
    virtual void firstpos(std::unordered_set<int>& set) = 0;
    virtual void lastpos(std::unordered_set<int>& set) = 0;
    
    virtual NodeType node_type() = 0;

};

class EmptyNode : public TreeNode {
public:
    bool nullable() { return true; }
    void firstpos(std::unordered_set<int>& set) { }
    void lastpos(std::unordered_set<int>& set) { }
    
    NodeType node_type() { return NodeType::EMPTY; }
};

class ValueNode : public TreeNode {
public:
    ValueNode() = default;
    ValueNode(int index, char symbol):index_(index), symbol_(symbol) {}
    bool nullable() { return false; }
    void firstpos(std::unordered_set<int>& set) { set.insert(index_); }
    void lastpos(std::unordered_set<int>& set) { set.insert(index_); }
   
    NodeType node_type() { return NodeType::VALUE; }

    void set_followpos(std::unordered_set<int> &followpos) { followpos_.insert(followpos.begin(), followpos.end()); }
    std::unordered_set<int> &followpos() { return followpos_; }
    char symbol() { return symbol_; }

private:
    int index_;
    char symbol_;
    std::unordered_set<int> followpos_;
};

class StarNode : public TreeNode {
public:
    StarNode() = default;
    StarNode(std::shared_ptr<TreeNode> child):child_(std::move(child)) {}
    bool nullable() { return true; }
    void firstpos(std::unordered_set<int>& set);
    void lastpos(std::unordered_set<int>& set);
    
    NodeType node_type() { return NodeType::STAR; }

    std::shared_ptr<TreeNode> &child() { return child_; }

private: 
    std::shared_ptr<TreeNode> child_;

};

class CatNode : public TreeNode {
public:
    CatNode() = default;
    CatNode(std::shared_ptr<TreeNode> left_child, std::shared_ptr<TreeNode> right_child, bool nullable):
        left_child_(std::move(left_child)), right_child_(std::move(right_child)), nullable_(nullable) {}
    bool nullable();
    void firstpos(std::unordered_set<int>& set);
    void lastpos(std::unordered_set<int>& set);

    NodeType node_type() { return NodeType::CAT; }

    std::shared_ptr<TreeNode>& left_child() { return left_child_; }
    std::shared_ptr<TreeNode>& right_child() { return right_child_; }


private:
    std::shared_ptr<TreeNode> left_child_;
    std::shared_ptr<TreeNode> right_child_;
    bool nullable_ = false;
};

class OrNode : public TreeNode {
public:
    OrNode() = default;
    OrNode(std::shared_ptr<TreeNode> left_child, std::shared_ptr<TreeNode> right_child, bool nullable):
        left_child_(std::move(left_child)), right_child_(std::move(right_child)), nullable_(nullable) {}
    bool nullable();
    void firstpos(std::unordered_set<int>& set);
    void lastpos(std::unordered_set<int>& set);

    NodeType node_type() { return NodeType::OR; }

    std::shared_ptr<TreeNode> &left_child() { return left_child_; }
    std::shared_ptr<TreeNode> &right_child() { return right_child_; }


private:
    std::shared_ptr<TreeNode> left_child_;
    std::shared_ptr<TreeNode> right_child_;
    bool nullable_ = false;
};


/*
class TreeNode {
public:
    TreeNode(NodeType node_type):node_type_(node_type){}
    TreeNode(NodeType node_type, std::shared_ptr<TreeNode> left_node, 
    std::shared_ptr<TreeNode> right_node): 
    node_type_(node_type), left_node_(std::move(left_node)), right_node_(std::move(right_node))
    {}

public:
    NodeType node_type() { return node_type_; }
    std::shared_ptr<TreeNode> &left_node() { return left_node_; }
    std::shared_ptr<TreeNode> &right_node() { return right_node_; }

protected:
    NodeType node_type_;
    std::shared_ptr<TreeNode> left_node_;
    std::shared_ptr<TreeNode> right_node_;
};
*/