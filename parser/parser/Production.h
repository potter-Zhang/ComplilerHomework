#pragma once

#include <vector>

class Production {
public:

	Production(int left, std::vector<int>& right);

	int get_left() { return left_; }
	std::vector<int>& get_right() { return right_; }

private:
	int left_;
	std::vector<int> right_;
};