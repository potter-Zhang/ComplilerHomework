#pragma once
#include <stack>

struct Checkpoint {
	std::stack<int> d_stack;
	int current_token;
};
