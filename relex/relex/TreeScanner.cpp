#include "TreeScanner.h"
#include "Tree.h"

#include <iostream>

void TreeScanner::scan(std::string regex, Tree &tree) {
    bool cat = false;
    bool trans = false;
    for (int i = 0; i < regex.size(); i++) {
        char c = regex[i];
        switch (c)
        {
        case '(': {
            if (cat) {
                opers_.push(OPER::CAT);
            }
            opers_.push(OPER::LBRACE);
            cat = false;
        } break;

        case ')': {
            opers_.push(OPER::RBRACE);
            loop_back(tree);
            cat = true;
        } break;

        case '*': {
            if (symbols_.empty()) {
                return error("no symbol before '*'");
            }
            opers_.push(OPER::STAR);
            create_expr(tree);
            cat = true;
        } break;

      

        case '?': {
            if (symbols_.empty()) {
                return error("no symbol before '?'");
            }
            opers_.push(OPER::OR);
            symbols_.push(Symbol::EMPTY_NODE);
            create_expr(tree);
            cat = true;
        } break;

        case '|': {
            
            while(check_priority(OPER::OR)) {
                if (symbols_.size() < 2) {
                    return error("'|' is a binary operator");
                }
                create_expr(tree);
            }
            opers_.push(OPER::OR);
            cat = false;
        } break;

        case '\\':
            c = regex[++i];
            trans = true;
        default: {
            while (check_priority(OPER::CAT)) {
                create_expr(tree);
            }
            if (cat) {
                
                opers_.push(OPER::CAT);
            }
            if (c == '.' && !trans) {
                symbols_.push('\0');
              
                 for (unsigned char j = '\x1'; j <= (unsigned char)0x7f; j++) {
                        if (j == '\n' || j == '#' || j == '\"')
                            continue;
                        opers_.push(OPER::OR);
                        symbols_.push(j);
                        create_expr(tree);
                  }

            }
            else
                symbols_.push(c);
            cat = true;
            trans = false;
            if (symbols_.top() == '#') {
                loop_back(tree);
            }
            
        } break;

            
        }
 
    }

    
}

void TreeScanner::error(std::string msg) {
    std::cout << "error: " << msg << std::endl;
}

bool TreeScanner::check_priority(OPER oper) {
    if (opers_.empty()) {
        return false;
    }
    return opers_.top() <= oper;
}

void TreeScanner::loop_back(Tree& tree) {
    if (opers_.top() == OPER::RBRACE) {
        // should pop out till LBRACE
        opers_.pop();
        while (opers_.top() != OPER::LBRACE) {
            create_expr(tree);
        }
        opers_.pop();

        //if (!opers_.empty() && (opers_.top() == OPER::CAT || opers_.top() == OPER::OR)) {
        //    create_expr(tree);
        //}
    }
    else if (symbols_.top() == '#') {
        while (!opers_.empty()) {
            create_expr(tree);
        }
    }
}

void TreeScanner::create_expr(Tree& tree) {
    switch (opers_.top())
    {
    case OPER::OR: {
        opers_.pop();
        char right = symbols_.top();
        symbols_.pop();
        char left = symbols_.top();
        symbols_.pop();

        tree.add_or_oper(left, right);
    } break;
    case OPER::CAT: {
        opers_.pop();
        char right = symbols_.top();
        symbols_.pop();
        char left = symbols_.top();
        symbols_.pop();

        tree.add_cat_oper(left, right);
    } break;
    case OPER::STAR: {
        opers_.pop();
        char child = symbols_.top();
        symbols_.pop();

        tree.add_star_oper(child);
    } break;
     
    
    default:
        break;
    }
    symbols_.push(Symbol::SUBTREE);
}