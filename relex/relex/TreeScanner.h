#pragma once

#include <stack>
#include <string>

class Tree;

class TreeScanner {
public: 
    enum OPER {
        STAR, CAT, OR, LBRACE, RBRACE
    };
    TreeScanner() = default;

    void scan(std::string regex, Tree &tree);

    bool check_priority(OPER oper);

    void loop_back(Tree &tree);

    void create_expr(Tree &tree);
    
    void error(std::string msg);

private: 
    std::stack<char> symbols_;
    std::stack<OPER> opers_;
};