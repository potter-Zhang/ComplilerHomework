#include "Tree.h"
#include "TreeNode.h"
#include "TreeScanner.h"
#include "DFA.h"
#include "LexScanner.h"
#include "SymbolTable.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <unordered_map>

SymbolTable table;

int index_of(std::vector<std::string>& vec, std::string& s);
void print_token(std::fstream& file, std::string type, std::string lexeme);
void register_symbols();

int main() {
    std::vector<std::string> ids;
    std::vector<std::string> consts;
    std::vector<std::string> string_consts;
    
    register_symbols();
    LexScanner scanner(200);
    std::fstream file;
    file.open("path/to/src/file");

    std::fstream sym_table;
    sym_table.open("path/to/symbol/table/file", std::ios::ate|std::ios::out);
    sym_table.flags(std::ios::left);

    scanner.read_file(file);

   
    scanner.add_regex("( |\t|\n)*#", [](std::string lexeme) {});

    scanner.add_regex("int#", [&](std::string lexeme) { print_token(sym_table, "<INT>", lexeme); });
    scanner.add_regex("float#", [&](std::string lexeme) { print_token(sym_table, "<FLOAT>", lexeme); });
    scanner.add_regex("bool#", [&](std::string lexeme) { print_token(sym_table, "<BOOL>", lexeme); });
    scanner.add_regex("string#", [&](std::string lexeme) { print_token(sym_table, "<STRING>", lexeme); });
    scanner.add_regex("void#", [&](std::string lexeme) { print_token(sym_table, "<VOID>", lexeme); });

    scanner.add_regex("if#", [&](std::string lexeme) { print_token(sym_table, "<IF>", lexeme); });
    scanner.add_regex("else#", [&](std::string lexeme) { print_token(sym_table, "<ELSE>", lexeme); });
    scanner.add_regex("while#", [&](std::string lexeme) { print_token(sym_table, "<WHILE>", lexeme); });
    scanner.add_regex("for#", [&](std::string lexeme) { print_token(sym_table, "<FOR>", lexeme); });
    scanner.add_regex("return#", [&](std::string lexeme) { print_token(sym_table, "<RETURN>", lexeme); });
    scanner.add_regex("switch#", [&](std::string lexeme) { print_token(sym_table, "<SWITCH>", lexeme); });
    scanner.add_regex("case#", [&](std::string lexeme) { print_token(sym_table, "<CASE>", lexeme); });
    scanner.add_regex("default#", [&](std::string lexeme) { print_token(sym_table, "<DEFAULT>", lexeme); });
    scanner.add_regex("true#", [&](std::string lexeme) { print_token(sym_table, "<TRUE>", lexeme); });
    scanner.add_regex("false#", [&](std::string lexeme) { print_token(sym_table, "<FALSE>", lexeme); });
    scanner.add_regex("break#", [&](std::string lexeme) { print_token(sym_table, "<BREAK>", lexeme); });
    scanner.add_regex("continue#", [&](std::string lexeme) { print_token(sym_table, "<CONTINUE>", lexeme); });

    scanner.add_regex("=#", [&](std::string lexeme) { print_token(sym_table, "<ASSIGN_OPER>", lexeme); });
    scanner.add_regex("==#", [&](std::string lexeme) { print_token(sym_table, "<EQ>", lexeme); });
    scanner.add_regex("<=#", [&](std::string lexeme) { print_token(sym_table, "<LTE>", lexeme); });
    scanner.add_regex(">=#", [&](std::string lexeme) { print_token(sym_table, "<GTE>", lexeme); });
    scanner.add_regex("<#", [&](std::string lexeme) { print_token(sym_table, "<LT>", lexeme); });
    scanner.add_regex(">#", [&](std::string lexeme) { print_token(sym_table, "<GT>", lexeme); });
    scanner.add_regex("!=#", [&](std::string lexeme) { print_token(sym_table, "<NEQ>", lexeme); });
    scanner.add_regex("+#", [&](std::string lexeme) { print_token(sym_table, "<ADD>", lexeme); });
    scanner.add_regex("-#", [&](std::string lexeme) { print_token(sym_table, "<SUB>", lexeme); });
    scanner.add_regex("\\*#", [&](std::string lexeme) { print_token(sym_table, "<MUL>", lexeme); });
    scanner.add_regex("/#", [&](std::string lexeme) { print_token(sym_table, "<DIV>", lexeme); });
    scanner.add_regex("&&#", [&](std::string lexeme) { print_token(sym_table, "<AND>", lexeme); });
    scanner.add_regex("\\|\\|#", [&](std::string lexeme) { print_token(sym_table, "<OR>", lexeme); });

    scanner.add_regex("++#", [&](std::string lexeme) { print_token(sym_table, "<SELF_ADD>", lexeme); });
    scanner.add_regex("--#", [&](std::string lexeme) { print_token(sym_table, "<SELF_SUB>", lexeme); });
    
    scanner.add_regex(";#", [&](std::string lexeme) { print_token(sym_table, "<DELIM>", lexeme); });
    scanner.add_regex("{#", [&](std::string lexeme) { print_token(sym_table, "<LBRACE>", lexeme); });
    scanner.add_regex("}#", [&](std::string lexeme) { print_token(sym_table, "<RBRACE>", lexeme); });
    scanner.add_regex("\\(#", [&](std::string lexeme) { print_token(sym_table, "<LPARENTHESES>", lexeme); });
    scanner.add_regex("\\)#", [&](std::string lexeme) { print_token(sym_table, "<RPARENTHESES>", lexeme); });
    scanner.add_regex("[#", [&](std::string lexeme) { print_token(sym_table, "<LBRACKET>", lexeme); });
    scanner.add_regex("]#", [&](std::string lexeme) { print_token(sym_table, "<RBRACKET>", lexeme); });
    scanner.add_regex(",#", [&](std::string lexeme) { print_token(sym_table, "<COMMA>", lexeme); });
    scanner.add_regex(":#", [&](std::string lexeme) { print_token(sym_table, "<COLON>", lexeme); });

    scanner.add_regex("//(.|\")*#", [](std::string lexeme) {});
    scanner.add_non_greedy_regex("/\\*(.|\n|\")*\\*/#", [](std::string lexeme) {});
   
    scanner.add_regex("\".*\"#", [&](std::string lexeme) { 
        sym_table << std::setw(32) << "[" + std::to_string(table["<STRING_CONST>"]) + "]" + "<STRING_CONST>" << std::setw(32) << lexeme;
        sym_table << std::setw(32) << index_of(consts, lexeme) << std::endl;
    });
    
    
    
    std::string letter_ = "(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z|A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z|_)";
    std::string digit = "(1|2|3|4|5|6|7|8|9|0)";
    std::string id = letter_ + "(" + letter_ + "|" + digit + ")*";
    
    scanner.add_regex(id + "#", [&](std::string lexeme) {
        sym_table << std::setw(32) << "[" + std::to_string(table["<ID>"]) + "]" + "<ID>" << std::setw(32) << lexeme;
        sym_table << std::setw(32) << index_of(ids, lexeme) << std::endl;
    });
    
    scanner.add_regex("(+|-)?" + digit + digit + "*#", [&](std::string lexeme) { 
        sym_table << std::setw(32) << "[" + std::to_string(table["<INTEGER_CONST>"]) + "]" + "<INTEGER_CONST>" << std::setw(32) << lexeme;
        
        sym_table << std::setw(32) << index_of(consts, lexeme) << std::endl;
    });
    
    scanner.add_regex("(+|-)?" + digit + "*\\." + digit + digit + "*#", [&](std::string lexeme) {
        sym_table << std::setw(32) << "[" + std::to_string(table["<FLOAT_CONST>"]) + "]" + "<FLOAT_CONST>" << std::setw(32) << lexeme;

        sym_table << std::setw(32) << index_of(consts, lexeme) << std::endl;
    });
    
    scanner.set_error_handler([&](std::string msg) { sym_table << std::setw(32) << "<BAD_SYMBOL>" << std::setw(32) << msg << std::endl; });
    sym_table << std::setw(32) << "TOKEN_TYPE" << std::setw(32) << "TOKEN" << std::setw(32) << "POINTER" << std::endl;
    sym_table << std::setw(32) << "----------" << std::setw(32) << "-----" << std::setw(32) << "-------" << std::endl;
    int code = 0;
    while ((code = scanner.scan()) != -1) 
        ;

    
    file.close();
    sym_table.close();

    std::fstream id_table;
    id_table.flags(std::ios::left);
    id_table.open("path/to/id/table", std::ios::ate | std::ios::out);
    id_table << std::setw(32) << "ADDRESS" << std::setw(32) << "VALUE" << std::endl;
    id_table << std::setw(32) << "-------" << std::setw(32) << "-----" << std::endl;
    for (int i = 1; i <= ids.size(); i++) {
        id_table << std::setw(32) << i << std::setw(32) << ids[i - 1] << std::endl;
    }
    id_table.close();

    std::fstream const_table;
    const_table.flags(std::ios::left);
    const_table.open("path/to/const/table", std::ios::ate | std::ios::out);
    const_table << std::setw(32) << "ADDRESS" << std::setw(32) << "VALUE" << std::endl;
    const_table << std::setw(32) << "-------" << std::setw(32) << "-----" << std::endl;
    for (int i = 1; i <= consts.size(); i++) {
        const_table << std::setw(32) << i << std::setw(32) << consts[i - 1] << std::endl;
    }
    const_table.close();

    return 0;
}

int index_of(std::vector<std::string>& vec, std::string& s) {
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] == s) {
            return i + 1;
        }
    }
    vec.emplace_back(s);
    return vec.size();
}


void print_token(std::fstream& file, std::string type, std::string lexeme) {
    file << std::setw(32) << "[" + std::to_string(table[type]) + "]" + type << std::setw(32) << lexeme << std::endl;
}

void register_symbols() {
        table.add_terminal("<INT>");
        table.add_terminal("<BOOL>");
        table.add_terminal("<FLOAT>");
        table.add_terminal("<STRING>");
        table.add_terminal("<VOID>");
        table.add_terminal("<IF>");
        table.add_terminal("<ELSE>");
        table.add_terminal("<WHILE>");
        table.add_terminal("<FOR>");
        table.add_terminal("<RETURN>");
        table.add_terminal("<SWITCH>");
        table.add_terminal("<CASE>");
        table.add_terminal("<DEFAULT>");
        table.add_terminal("<BREAK>");
        table.add_terminal("<CONTINUE>");
        table.add_terminal("<ASSIGN_OPER>");
        table.add_terminal("<EQ>");
        table.add_terminal("<LTE>");
        table.add_terminal("<GTE>");
        table.add_terminal("<LT>");
        table.add_terminal("<GT>");
        table.add_terminal("<NEQ>");
        table.add_terminal("<ADD>");
        table.add_terminal("<SUB>");
        table.add_terminal("<MUL>");
        table.add_terminal("<DIV>");
        table.add_terminal("<SELF_ADD>");
        table.add_terminal("<SELF_SUB>");
        table.add_terminal("<DELIM>");
        table.add_terminal("<LBRACE>");
        table.add_terminal("<RBRACE>");
        table.add_terminal("<LPARENTHESES>");
        table.add_terminal("<RPARENTHESES>");
        table.add_terminal("<LBRACKET>");
        table.add_terminal("<RBRACKET>");
        table.add_terminal("<COMMA>");
        table.add_terminal("<COLON>");
        table.add_terminal("<AND>");
        table.add_terminal("<OR>");
        table.add_terminal("<NOT>");
        table.add_terminal("<INTEGER_CONST>");
        table.add_terminal("<FLOAT_CONST>");
        table.add_terminal("<STRING_CONST>");
        table.add_terminal("<FALSE>");
        table.add_terminal("<TRUE>");

        table.add_terminal("<ID>");

}
                          



