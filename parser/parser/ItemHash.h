#pragma once
#include "Item.h"


struct ItemHash {
	std::size_t operator() (const Item &item) const {
		std::hash<int> f;
		std::hash<int> s;
		return f(item.get_position_idx()) ^ (s(item.get_production_idx()) << 1);
	}
};
