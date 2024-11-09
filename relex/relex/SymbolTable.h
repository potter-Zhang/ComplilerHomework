#pragma once

#include <unordered_map>
#include <string>

class SymbolTable {
public:
	SymbolTable() = default;

	void add_nonterminal(std::string symbol);

	void print_symbols();
	void add_terminal(std::string symbol);

	int operator[](std::string symbol);
	std::string operator[](int idx);
	void reserve(int n);

private:
	int terminal_idx_ = 0;
	int nonterminal_idx_ = 0;
	std::unordered_map<int, std::string> idx2symbol_;
	std::unordered_map<std::string, int> symbol2idx_;
};