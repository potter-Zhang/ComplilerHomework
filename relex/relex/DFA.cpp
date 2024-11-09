#include "DFA.h"
#include <iostream>

void DFA::create(Tree& tree) {
	std::unordered_set<int> u;
	
	// add start state
	tree.root()->firstpos(u);
	unmark_.push(id_);
	
	is_end_state_.emplace_back(tree.contains_end_tag(u));
	Dstates_.emplace_back(u);
	u.clear();
	Dtran_.resize(1);
	
	while (!unmark_.empty()) {
		int s = unmark_.front();
		unmark_.pop();

		for (char a : tree.symbols()) {
			get_union(s, a, tree, u);
			int next_state;
			if ((next_state = inDstates(u)) == -1) {
				
				unmark_.push(++id_);
				Dtran_.resize(Dtran_.size() + 1);
				is_end_state_.emplace_back(tree.contains_end_tag(u));
				Dstates_.emplace_back(u);
				next_state = id_;
			}
			Dtran_[s][a] = next_state;

			u.clear();
		}
	}
}

void DFA::create(std::string regex)
{
	Tree tree;
	tree.create(regex);
	tree.calc_followpos();
	create(tree);
	minimize();
}

void DFA::get_union(int state, char c, Tree& tree, std::unordered_set<int>& u)
{
	std::vector<std::shared_ptr<ValueNode>>& leaf = tree.leaf();
	std::vector<int> ids;

	for (int i : Dstates_[state]) {
		if (c == leaf[i]->symbol()) {
			ids.emplace_back(i);
		}
	}

	for (int id : ids) {
		u.insert(leaf[id]->followpos().begin(), leaf[id]->followpos().end());
	}
	
}

int DFA::inDstates(std::unordered_set<int>& u)
{
	for (int i = 0; i < Dstates_.size(); i++) {
		if (u == Dstates_[i]) {
			return i;
		}
	}
	return -1;
	
}

void DFA::minimize()
{
	std::vector<int> group_id; // group_id[i] group id of i
	std::vector<int> group_num;	// group_num[i] number of states in group i
	std::unordered_map<int, int> group_mapper;
	group_num.resize(3);
	int old_id = 0, new_id = 3;
	for (int i = 0; i < is_end_state_.size(); i++) {
		if (Dstates_[i].size() == 0) {
			// dead state
			group_id.emplace_back(0);
			group_num[0]++;
		}
		else {
			group_id.emplace_back(is_end_state_[i] + 1);
			group_num[is_end_state_[i] + 1]++;
		}
	}

	while (old_id != new_id) {
		old_id = new_id;

		for (int i = 0; i < group_id.size(); i++) {
			int id = group_id[i];
			if (group_num[id] <= 1)
				continue;
			for (auto next_state : Dtran_[i]) {
				if (abs(group_id[next_state.second]) == id)
					continue;
				group_num[id]--;
				group_id[i] = -id;
				if (group_mapper[-id] == 0) {
					group_mapper[-id] = new_id++;
					group_num.emplace_back(0);
				}
				group_num[group_mapper[-id]]++;
				break;
				
			}
		}

		for (int i = 0; i < group_id.size(); i++) {
			if (group_mapper[group_id[i]] != 0) {
				group_id[i] = group_mapper[group_id[i]];
			}
		}
		group_mapper.clear();
	}

	new_id = 0;

	for (int i = 0; i < group_id.size(); i++) {
		int neg_group_id = -group_id[i] - 1;
		if (neg_group_id == -1) {
			// dead state
			group_mapper[neg_group_id] = -1;
			group_mapper[i] = -1;
			continue;
		}
		if (group_mapper[neg_group_id] == 0) {
			group_mapper[neg_group_id] = new_id;
			is_end_state_[new_id++] = is_end_state_[i];
		}
		old_id = group_mapper[neg_group_id];
		group_mapper[i] = old_id;
		is_end_state_[old_id] = is_end_state_[old_id] || is_end_state_[i];
	}

	int idx = 0;
	for (auto dit = Dtran_.begin(); dit != Dtran_.end(); ) {
		for (auto it = dit->begin(); it != dit->end(); ) {
			if (group_mapper[it->second] == -1) {
				dit->erase(it++);
			}
			else {
				it->second = group_mapper[it->second];
				++it;
			}
		}
		/*for (std::pair<const char, int>& t : Dtran_[i]) {
			t.second = group_mapper[t.second];
		}*/
		if (dit->empty() && group_mapper[idx] == -1) {
			dit = Dtran_.erase(dit);
		}
		else {
			++dit;
			++idx;
		}
	}

	
	is_end_state_.resize(Dtran_.size());

}

bool DFA::recognize(std::string s, std::function<void(void)> callback)
{
	int current_state = 0;
	for (auto c : s) {
		if (Dtran_[current_state].find(c) != Dtran_[current_state].end()) {
			current_state = Dtran_[current_state][c];
		}
		else {
			return false;
		}
	}
	if (is_end_state_[current_state]) {
		callback();
		return true;
	}
	return false;
	
}

int DFA::recognize(std::function<char(void)> next_char)
{
	int current_state = 0, i = 0;
	char c;

	int state_mark = -1;
	int recognize_len = 0;

	while ((c = next_char()) != EOF) {
		if (Dtran_[current_state].find(c) != Dtran_[current_state].end()) {
			if (is_end_state_[current_state]) {
				state_mark = current_state;
				recognize_len = i;
				if (!greedy_)
					break;
			}
			i++;
			current_state = Dtran_[current_state][c];
		}
		else {
			break;
		}
	}
	
	if (is_end_state_[current_state]) {
		return i;
	}

	if (recognize_len) {
		return recognize_len;
	}
	return 0;
}

void DFA::print_out()
{
	for (int i = 0; i < Dtran_.size(); i++) {
		std::unordered_map<char, int>& tran = Dtran_[i];
		for (auto t : tran) {
			std::cout << i << " -" << t.first << "-> " << t.second << std::endl;
		}
	}

	for (int i = 0; i < Dtran_.size(); i++) {
		if (is_end_state_[i]) {
			std::cout << "[" << i << "]";
		}
	}
	std::cout << std::endl;
}

void DFA::set_non_greedy()
{
	greedy_ = false;
}





