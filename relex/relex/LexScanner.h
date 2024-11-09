#pragma once

#include<vector>
#include "DFA.h"
#include "LexBuffer.h"
#include<fstream>

class LexScanner {
public:
	LexScanner(int n):buffer_(n){}

	void add_dfa(DFA& dfa, std::function<void(std::string lexeme)> callback);

	void add_regex(std::string regex, std::function<void(std::string lexeme)> callback);

	void add_non_greedy_regex(std::string regex, std::function<void(std::string lexeme)> callback);

	void read_file(std::fstream &file);

	void set_error_handler(std::function<void(std::string)> error);

	int scan();


private: 
	std::function<void(std::string msg)> error_;
	std::vector<std::function<void(std::string lexeme)>> callbacks_;
	std::vector<DFA> dfas_;
	LexBuffer buffer_;
	std::string error_msg_;
};