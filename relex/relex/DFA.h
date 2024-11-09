#pragma once

#include "Tree.h"
#include<queue>
#include<unordered_map>
#include<functional>
class DFA {
public:
	DFA() = default;

	void create(Tree& tree);

	void create(std::string regex);

	void get_union(int state, char c, Tree& tree, std::unordered_set<int> &u);

	int inDstates(std::unordered_set<int>& u);

	void minimize();

	bool recognize(std::string s, std::function<void(void)> callback);

	int recognize(std::function<char(void)> next_char);

	void print_out();

	void set_non_greedy();
	

private:
	std::vector<std::unordered_set<int>> Dstates_;
	std::queue<int> unmark_;
	std::vector<std::unordered_map<char, int>> Dtran_;
	std::vector<bool> is_end_state_;
	bool greedy_ = true;
	int id_ = 0;
};
