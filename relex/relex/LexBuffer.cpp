#include "LexBuffer.h"
#include <algorithm>

char LexBuffer::next_char()
{
	if (forward == buffer_.size())
		return EOF;
	return buffer_[forward++];
}

void LexBuffer::reset()
{
	forward = lex_begin;
}

void LexBuffer::consume(int n)
{
	std::string s = buffer_.substr(lex_begin, n);
	int new_line = std::count(s.begin(), s.end(), '\n');
	line += new_line;

	size_t pos;
	
	if (new_line != 0 && (pos = s.find_last_of('\n')) != s.npos) {
		character = s.length() - pos;
	}
	else {
		character += n;
	}

	lex_begin += n;
	forward = lex_begin;
	
}

void LexBuffer::read_file(std::fstream& f)
{
	std::istreambuf_iterator<char> beg(f), end;
	buffer_.assign(beg, end);
}

int LexBuffer::buf_size()
{
	return buffer_.size();
}

std::string LexBuffer::get_lexeme(int n)
{
	return buffer_.substr(lex_begin, n);
}
