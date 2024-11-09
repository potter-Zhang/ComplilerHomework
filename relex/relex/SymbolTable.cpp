#include "SymbolTable.h"
#include <iostream>

void SymbolTable::print_symbols() {
	for (auto p : idx2symbol_) {
		std::cout << p.first << "|" << p.second << std::endl;
	}
}

void SymbolTable::add_nonterminal(std::string symbol) {
	nonterminal_idx_--;
	idx2symbol_.insert({ nonterminal_idx_, symbol });
	symbol2idx_.insert({ symbol, nonterminal_idx_ });
}

void SymbolTable::add_terminal(std::string symbol) {
	terminal_idx_++;
	idx2symbol_.insert({ terminal_idx_, symbol });
	symbol2idx_.insert({ symbol, terminal_idx_ });
}

int SymbolTable::operator[](std::string symbol) {
	auto it = symbol2idx_.find(symbol);
	if (it != symbol2idx_.end()) {
		return it->second;
	}
	std::cout << "error => " << symbol << std::endl;
	return 0;
}

std::string SymbolTable::operator[](int idx) {
	auto it = idx2symbol_.find(idx);
	if (it != idx2symbol_.end()) {
		return it->second;
	}
	return "";
}

void SymbolTable::reserve(int n)
{
	terminal_idx_ = n;
	nonterminal_idx_ = -n;
}
