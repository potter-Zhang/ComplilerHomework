#pragma once
#include "Production.h"
#include "SymbolTable.h"
#include <string>

class ProductionScanner {
public:
	ProductionScanner() = default;

	Production scan(SymbolTable &table, std::string production);
	std::string scan(SymbolTable& table, Production& production);

};
