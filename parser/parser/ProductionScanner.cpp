#include "ProductionScanner.h"
#include <sstream>

Production ProductionScanner::scan(SymbolTable& table, std::string production) {
	std::stringstream ss(production);

	std::string symbol;
	int left;
	std::vector<int> right;

	ss >> symbol;
	left = table[symbol];
	ss >> symbol;

	while (ss >> symbol) {
		right.emplace_back(table[symbol]);
	}

	return Production(left, right);

}

std::string ProductionScanner::scan(SymbolTable& table, Production& production) {
	std::stringstream ss;

	int left = production.get_left();

	ss << table[left];

	ss << ' ' << "->" << ' ';

	std::vector<int>& right = production.get_right();

	for (int symbol : right) {
		ss << table[symbol] << ' ';
	}
	return ss.str();

}