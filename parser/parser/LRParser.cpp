#include "LRParser.h"
#include <algorithm>
#include <iostream>

void LRParser::add_production(Production production) {
	productions_.emplace_back(production);
}

void LRParser::set_start_symbol(int symbol)
{
	start_symbol_ = symbol;
}

void LRParser::generate_table()
{
	// add another start token
	std::vector<int> start = { start_symbol_ };
	Production production(START, start);

	productions_.emplace_back(production);

	std::unordered_set<Item, ItemHash> items;
	std::vector<std::unordered_set<Item, ItemHash>> new_states;
	
	items.insert(Item(productions_.size() - 1, 0));

	closure(items);
	states_.emplace_back(items);

	int new_id = states_.size();

	bool added = true;
	trans_.resize(1);
	for (int i = 0; i < states_.size(); i++) {
		
		for (Item item : states_[i]) {
			Production& item_production = productions_[item.get_production_idx()];
			std::vector<int>& right = item_production.get_right();
			int pos = item.get_position_idx();

			if (pos < right.size()) {
				int symbol = right[pos];
				std::unordered_set<Item, ItemHash> new_state = go_to(states_[i], symbol);
				int state_id = find_state(new_state);
				if (!new_state.empty() && state_id == -1) {
					new_states.emplace_back(new_state);
					++new_id;
					state_id = new_id - 1;
				}
				if (symbol > 0)
					trans_[i][symbol] = ActionState{ state_id, SHIFT };
				else 
					trans_[i][symbol] = ActionState{ state_id, GOTO };
			}
			else {
				if (item_production.get_left() == START) {
					trans_[i][END] = ActionState{ 0, ACCEPT };
					continue;
				}
				std::unordered_set<int> follow_set = follow(item_production.get_left());
				for (int follow_symbol : follow_set) {
					trans_[i][follow_symbol] = ActionState{ item.get_production_idx(), REDUCE };
				}
			}
		}
		states_.insert(states_.end(), new_states.begin(), new_states.end());
		new_states.clear();
		trans_.resize(states_.size());
		
	}
	

}

int LRParser::find_state(std::unordered_set<Item, ItemHash>& state)
{
	for (int i = 0; i < states_.size(); i++) {
		if (state == states_[i]) {
			return i;
		}
	}

	return -1;
}

std::string LRParser::get_action_str(Action action)
{
	switch (action)
	{
	case LRParser::NONE:
		return "none";
		break;
	case LRParser::GOTO:
		return "goto";
		break;
	case LRParser::SHIFT:
		return "shift";
		break;
	case LRParser::REDUCE:
		return "reduce";
		break;
	case LRParser::ACCEPT:
		return "accept";
		break;
	default:
		return "undefined";
		break;
	}
	return std::string();
}

void LRParser::closure(std::unordered_set<Item, ItemHash>& I)
{
	//std::unordered_set<Item, ItemHash> J;
	std::unordered_set<Item, ItemHash> new_I;
	bool added = true;
	while (added) {
		added = false;
		I.insert(new_I.begin(), new_I.end());
		new_I.clear();
		for (Item item : I) {
			int idx = item.get_production_idx();
			int position = item.get_position_idx();
			if (position >= productions_[idx].get_right().size() ||
				productions_[idx].get_right()[position] > 0)
				continue;

			int left = productions_[idx].get_right()[position];

			for (int i = 0; i < productions_.size(); i++) {
				if (productions_[i].get_left() != left)
					continue;

				Item item(i, 0);
				if (I.find(item) == I.end()) {
					new_I.insert(item);
					added = true;
				}
			}
		}
	}
	
}

std::unordered_set<Item, ItemHash> LRParser::go_to(std::unordered_set<Item, ItemHash>& I, int symbol) {
	std::unordered_set<Item, ItemHash> go_to_set;
	std::unordered_set<Item, ItemHash> new_I;

	for (Item item : I) {
		std::vector<int>& right = productions_[item.get_production_idx()].get_right();
		if (item.get_position_idx() < right.size() && right[item.get_position_idx()] == symbol) {
			Item new_item(item.get_production_idx(), item.get_position_idx() + 1);
			new_I.insert(new_item);
			closure(new_I);
			go_to_set.insert(new_I.begin(), new_I.end());
			new_I.clear();
		}
	}

	return go_to_set;
}

std::unordered_set<int> LRParser::follow(int symbol)
{
	visited_.clear();
	std::unordered_set<int> follow_set;
	follow_internal(follow_set, symbol);
	
	return follow_set;
}

void LRParser::follow_internal(std::unordered_set<int>& follow_set, int symbol)
{
	if (symbol == START)
		follow_set.insert(END);
	visited_.insert(symbol);
	for (Production production : productions_) {
		std::vector<int>& right = production.get_right();
		for (int i = 0; i < ((int)right.size() - 1); i++) {
			if (right[i] == symbol) {
				if (right[i + 1] > 0) {
					follow_set.insert(right[i + 1]);
					break;
				}
				else {
					std::unordered_set<int> inner_first_set = first(production.get_left());
					if (inner_first_set.find(EMPTY) != inner_first_set.end()) {
						inner_first_set.erase(EMPTY);
						follow_set.insert(inner_first_set.begin(), inner_first_set.end());
						if (production.get_left() != symbol) {
							follow_internal(follow_set, production.get_left());
						}
					}
				}
				if (production.get_left() != symbol) {
					follow_internal(follow_set, production.get_left());
				}
			}
		}
		if (!right.empty() && right[right.size() - 1] == symbol && production.get_left() != symbol && visited_.find(production.get_left()) == visited_.end()) {
			follow_internal(follow_set, production.get_left());
		}
	}
}

std::unordered_set<int> LRParser::first(int symbol) {
	std::unordered_set<int> first_set;

	for (Production production : productions_) {
		if (production.get_left() != symbol)
			continue;

		std::vector<int>& right = production.get_right();
		for (int i = 0; i < right.size(); i++) {
			if (right[i] > 0) {
				first_set.insert(right[i]);
				break;
			}
			else {
				if (right[i] == symbol) {
					break;
				}
				std::unordered_set<int> tmp = first(right[i]);
				if (tmp.find(EMPTY) == tmp.end()) {
					first_set.insert(tmp.begin(), tmp.end());
					break;
				}
				else if (i == right.size() - 1) {
					first_set.insert(tmp.begin(), tmp.end());
				}
				else {
					tmp.erase(EMPTY);
					first_set.insert(tmp.begin(), tmp.end());
				}
			}
		}
		if (right.empty()) {
			first_set.insert(EMPTY);
		}
	}
	return first_set;
}

bool LRParser::parse(std::vector<int>& tokens, std::vector<std::pair<int, int>>& loc, SymbolTable& table)
{
	const int k = 8;

	tokens.push_back(END);
	loc.push_back(loc.back());
	stack_.push(END);
	stack_.push(0);

	int i = 0;
	int token = tokens[i++];
	bool recover = false;

	while(true) {
		int current_state = stack_.top();

		ActionState action_state = trans_[current_state][token];
		if (action_state.action == SHIFT) {
			stack_.push(token);
			stack_.push(action_state.next);
			save_checkpoint(i);
			token = tokens[i++];
			
		}
		else if (action_state.action == REDUCE) {
			Production& production = productions_[action_state.next];
			std::cout << table[production.get_left()] << " -> ";
			for (auto ele : production.get_right()) {
				std::cout << table[ele] << ' ';
			}
			std::cout << std::endl;
			for (int i = 0; i < production.get_right().size() * 2; i++) {
				stack_.pop();
			}
			int reduce_current_state = stack_.top();
			ActionState reduce_action_state = trans_[reduce_current_state][production.get_left()];
			// should be goto
			stack_.push(production.get_left());
			stack_.push(reduce_action_state.next);
		}
		else if (action_state.action == ACCEPT) {
			if (!recover) {
				std::cout << "accept" << std::endl;
				return true;
			}
			else {
				std::cout << "syntax error" << std::endl;
				for (int i = 0; i < error_msg_.size(); i++) {
					std::cout << "============ error " << i << " ============" << std::endl;
					std::cout << error_msg_[i].act << table[tokens[error_msg_[i].token]] << " at line: " << loc[error_msg_[i].token].first << " ch: " << loc[error_msg_[i].token].second << std::endl;
				}
				return false;
			}
			
		}
		else {
			recover = true;
			int replace_token;
			if (!error_handler(tokens, replace_token, table)) {
				std::cout << "syntax error" << std::endl;
				std::cout << "fatal error: unexpected token " << table[token] << " at line: " << loc[i - 1].first << " ch: " << loc[i - 1].second << std::endl;
				
				return false;
			}
			
			
			token = replace_token;
			stack_ = checkpoints_.back().d_stack;
			i = checkpoints_.back().current_token;
		}

	}
	return false;
}

void LRParser::save_checkpoint(int i) {
	if (checkpoints_.size() > num_checkpoints_) {
		checkpoints_.pop_front();
	}
	checkpoints_.emplace_back(Checkpoint{ stack_, i });
}

bool LRParser::error_handler(std::vector<int>& tokens, int &token, SymbolTable& table)
{
	while (!checkpoints_.empty()) {
		std::stack<int>& error_stack = checkpoints_.back().d_stack;
		int current_state = error_stack.top();
		for (auto p : trans_[current_state]) {
			if (p.first <= 0 || p.second.action == NONE) {
				continue;
			}
			bool recover = error_handler_internal(tokens, p.first);
			if (recover) {
				
				token = p.first;
				error_msg_.emplace_back(ErrorMessage{ checkpoints_.back().current_token, "insert: new token " + table[token] + " before "});
				return true;
			}
			
			checkpoints_.back().current_token += 1;

			recover = error_handler_internal(tokens, p.first);
			if (recover) {
				token = p.first;
				error_msg_.emplace_back(ErrorMessage{ checkpoints_.back().current_token - 1, "replace with new token: " + table[token] + " original token: "});

				return true;
			}

			checkpoints_.back().current_token -= 1;

		}
		
		checkpoints_.pop_back();
		
	}
	return false;
	
}

bool LRParser::error_handler_internal(std::vector<int>& tokens, int replace_token)
{
	int length = 0;
	const int min_check = 5;

	std::stack<int> error_stack = checkpoints_.back().d_stack;
	int i = checkpoints_.back().current_token;
	int current_state = error_stack.top();
	int token = replace_token;

	while (length < min_check) {
		int current_state = error_stack.top();

		ActionState action_state = trans_[current_state][token];
		if (action_state.action == SHIFT) {
			error_stack.push(token);
			error_stack.push(action_state.next);
			if (i >= tokens.size()) {
				return false;
			}
			token = tokens[i++];
			length++;

		}
		else if (action_state.action == REDUCE) {
			Production& production = productions_[action_state.next];
			for (int i = 0; i < production.get_right().size() * 2; i++) {
				error_stack.pop();
			}
			int reduce_current_state = error_stack.top();
			ActionState reduce_action_state = trans_[reduce_current_state][production.get_left()];
			// should be goto
			error_stack.push(production.get_left());
			error_stack.push(reduce_action_state.next);
		}
		else if (action_state.action == ACCEPT) {
			return true;
		}
		else {
			return false;
		}
	}
	return true;
}



