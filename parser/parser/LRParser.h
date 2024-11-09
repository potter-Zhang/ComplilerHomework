#pragma once
#include "Production.h"
#include "Item.h"
#include "ItemHash.h"
#include "SymbolTable.h"
#include "Checkpoint.h"
#include "ErrorMessage.h"
#include <vector>
#include <stack>
#include <deque>	
#include <unordered_set>
#include <unordered_map>

class LRParser {
public: 
	enum SpecialToken { EMPTY = 1, START = -1, END = 0 };
	enum Action { NONE, GOTO, SHIFT, REDUCE, ACCEPT };
	typedef struct {
		int next;
		Action action;
	} ActionState;

	void add_production(Production production);
	void set_start_symbol(int symbol);
	void generate_table();
	int find_state(std::unordered_set<Item, ItemHash>& state);
	std::string get_action_str(Action action);

	void closure(std::unordered_set<Item, ItemHash> &I);
	std::unordered_set<Item, ItemHash> go_to(std::unordered_set<Item, ItemHash>& I, int symbol);
	std::unordered_set<int> follow(int symbol);
	void follow_internal(std::unordered_set<int>& follow_set, int symbol);
	std::unordered_set<int> first(int symbol);
	bool parse(std::vector<int> &tokens, std::vector<std::pair<int, int>> &loc, SymbolTable &table);
	bool error_handler(std::vector<int>& tokens, int &token, SymbolTable& table);
	bool error_handler_internal(std::vector<int>& tokens, int replace_token);
	void save_checkpoint(int i);


private:
	const int num_checkpoints_ = 3;
	int start_symbol_;
	int state_id_ = -1;
	std::vector<Production> productions_;
	std::vector<std::unordered_set<Item, ItemHash>> states_;
	std::vector<std::unordered_map<int, ActionState>> trans_;
	std::vector<ErrorMessage> error_msg_;
	std::unordered_set<int> visited_;
	std::stack<int> stack_;
	std::deque<Checkpoint> checkpoints_;
};