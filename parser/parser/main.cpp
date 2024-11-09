#include<iostream>
#include<fstream>
#include <vector>
#include "SymbolTable.h"
#include "LRParser.h"
#include "ProductionScanner.h"
#include <string>
#include <fstream>
#include <iostream>

int main() {
	std::fstream syntax_file;
	syntax_file.open("C:\\Users\\Harry\\Desktop\\syntax.txt");

	SymbolTable table;
	LRParser parser;
	ProductionScanner scanner;

	table.reserve(3);

	std::string line;
	int part = 0;
	while (std::getline(syntax_file, line)) {
		if (line.size() == 0 || line == "\n")
			continue;
		if (line[0] == '%') {
			if (line == "% nonterminal") {
				part = 0;
			}
			else if (line == "% terminal") {
				part = 1;
			}
			else if (line == "% production") {
				part = 2;
			}
			else {
				std::cout << "error in syntax file" << std::endl;
			}
			continue;
		}

		if (part == 0) {
			table.add_nonterminal(line);
		}
		else if (part == 1) {
			table.add_terminal(line);
		}
		else {
			parser.add_production(scanner.scan(table, line));
		}
	}
	syntax_file.close();
	//table.print_symbols();

	parser.set_start_symbol(table["<PROGRAM>"]);
	parser.generate_table();

	std::fstream token_file;
	std::vector<int> tokens;
	std::vector<std::pair<int, int>> loc;
	token_file.open("C:\\Users\\Harry\\Desktop\\sym_table.txt");
	while (token_file >> line) {
		if (line[0] == '[' && line[line.length() - 1] == '>') {
			line = line.substr(line.find_first_of(']') + 1);
			//std::cout << line << " " << table[line] << std::endl;
			if (line == "<BAD_SYMBOL>") {
				std::cout << "<BAD_SYMBOL>" << std::endl;
				return 1;
			}
				
			tokens.emplace_back(table[line]);
		} 
		else if (line[0] == '(' && line[line.length() - 1] == ')') {
			
			size_t comma = line.find_last_of(',');
			int line_num = atoi(line.substr(1, comma - 1).c_str());
			int ch_num = atoi(line.substr(comma + 1, line.length() - comma - 2).c_str());
			loc.emplace_back(std::make_pair( line_num, ch_num ));
		}
			
		
	}
	table.print_symbols();
	parser.parse(tokens, loc, table);


	/*
	table.reserve(3);
	table.add_nonterminal("E");
	table.add_nonterminal("T");
	table.add_nonterminal("F");
	table.add_terminal("(");
	table.add_terminal(")");
	table.add_terminal("+");
	table.add_terminal("*");
	table.add_terminal("id");

	table.print_symbols();

	ProductionScanner scanner;
	std::vector<Production> productions;
	std::vector<int> tokens{ table["id"], table["*"], table["id"], table["+"], table["id"] };

	productions.emplace_back(scanner.scan(table, "E -> E + T"));
	productions.emplace_back(scanner.scan(table, "E -> T"));
	productions.emplace_back(scanner.scan(table, "T -> T * F"));
	productions.emplace_back(scanner.scan(table, "T -> F"));
	productions.emplace_back(scanner.scan(table, "F -> ( E )"));
	productions.emplace_back(scanner.scan(table, "F -> id"));

	int i = 0;
	i = (i++) + (++i);
	std::cout << i << std::endl;

	
	LRParser parser;
	parser.set_start_symbol(table["E"]);
	for (int i = 0; i < productions.size(); i++) {
		parser.add_production(productions[i]);
	}
	parser.generate_table();
	parser.parse(tokens);
	*/

	return 0;

}