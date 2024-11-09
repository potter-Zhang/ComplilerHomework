
#include "TreeNode.h"
/**
 * StarNode
*/

void StarNode::firstpos(std::unordered_set<int>& set) {
    child_->firstpos(set);
}

void StarNode::lastpos(std::unordered_set<int>& set) {
    child_->lastpos(set);
}

/**
 * CatNode
*/
bool CatNode::nullable() {
    return nullable_;
}

void CatNode::firstpos(std::unordered_set<int>& set) {
    if (left_child_->nullable()) {
        left_child_->firstpos(set);
        right_child_->firstpos(set);
    }
    else {
        left_child_->firstpos(set);
    }
}

void CatNode::lastpos(std::unordered_set<int>& set) {
    if (right_child_->nullable()) {
        right_child_->lastpos(set);
        left_child_->lastpos(set);
    }
    else {
        right_child_->lastpos(set);
    }
}

/**
 * OrNode
*/

bool OrNode::nullable() {
    return nullable_;
}

void OrNode::firstpos(std::unordered_set<int>& set) {
    left_child_->firstpos(set);
    right_child_->firstpos(set);
}

void OrNode::lastpos(std::unordered_set<int>& set) {
    left_child_->lastpos(set);
    right_child_->lastpos(set);
}
