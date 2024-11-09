#pragma once
#include <utility>

class Item {
public:
	Item(int idx, int pos);


	int get_production_idx() const { return item_.first; }
	int get_position_idx() const { return item_.second; }

	bool operator==(const Item &other) const {
		return item_.first == other.item_.first && item_.second == other.item_.second;
	}
	
private:
	std::pair<int, int> item_;
};