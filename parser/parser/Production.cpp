#include "Production.h"

Production::Production(int left, std::vector<int>& right):left_(left), right_(std::move(right)) {}