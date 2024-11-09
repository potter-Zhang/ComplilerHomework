#include "LexScanner.h"
#include <iostream>

void LexScanner::add_dfa(DFA& dfa, std::function<void(std::string lexeme)> callback)
{
	dfas_.emplace_back(dfa);
	callbacks_.emplace_back(callback);
}

void LexScanner::add_regex(std::string regex, std::function<void(std::string lexeme)> callback)
{
	DFA dfa;
	dfa.create(regex);
	
	add_dfa(dfa, callback);
}

void LexScanner::add_non_greedy_regex(std::string regex, std::function<void(std::string lexeme)> callback)
{
	add_regex(regex, callback);
	dfas_[dfas_.size() - 1].set_non_greedy();
}

void LexScanner::read_file(std::fstream &file)
{
	buffer_.read_file(file);
}

void LexScanner::set_error_handler(std::function<void(std::string)> error)
{
	error_ = error;
}

int LexScanner::scan()
{
	int prefix_len = 0;
	int dfa_id = -1;
	std::string lexeme;

	if (buffer_.next_char() == EOF) {
		return -1;
	}
	buffer_.reset();

	for (int i = 0; i < dfas_.size(); i++) {
		int len = dfas_[i].recognize([&]() { return buffer_.next_char(); });
		if (len > prefix_len) {
			prefix_len = len;
			dfa_id = i;
			lexeme = buffer_.get_lexeme(prefix_len);
		}
		buffer_.reset();
	}

	if (dfa_id != -1) {
		if (!error_msg_.empty()) {

			error_msg_ += " line:" + std::to_string(buffer_.get_line_num()) + " ch:" + std::to_string(buffer_.get_character_num() - error_msg_.length());
			error_(error_msg_);
			error_msg_.clear();
		}
		lexeme = lexeme + " (" + std::to_string(buffer_.get_line_num()) + "," + std::to_string(buffer_.get_character_num() - error_msg_.length()) + ")";
		callbacks_[dfa_id](lexeme);
		buffer_.consume(prefix_len);
		return 1;
	}
	error_msg_ += buffer_.next_char();
	buffer_.consume(1);
	return 0;

}
