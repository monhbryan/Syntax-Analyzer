#pragma once
#ifndef lexer_h
#define lexer_h

#include <vector>
#include <string>
#include <map>
#include <fstream>
using namespace std;

struct Token {
	vector<string> lexeme;
	vector<string> token;
	int size = 0;

};


Token lexer1();



#endif 