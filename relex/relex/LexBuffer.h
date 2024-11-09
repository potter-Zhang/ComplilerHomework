#pragma once
#include<string>
#include<fstream>

class LexBuffer {
public:
	LexBuffer(int n) {
		buffer_.reserve(n);
	}

	int get_line_num() { return line; }
	int get_character_num() { return character; }

	char next_char();
	void reset();
	void consume(int n);
	int buf_size();
	std::string get_lexeme(int n);

	void read_file(std::fstream &f);

private:
	int line = 1;
	int character = 1;
	int forward = 0;
	int lex_begin = 0;
	std::string buffer_;
};
