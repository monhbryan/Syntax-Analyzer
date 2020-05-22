// Lexer.h
// Bryan Monh

#include <iostream>
#include "lexer.h"
using namespace std;

/*struct Token {
	vector<string> lexeme;
	vector<string> token;
	int size = 0;
};
*/

/*
Invalid = 0
IDENTIFIER = 1 
Integer = 2
Real # = 3
Operator = 4
Space = 5
Unknown String = 6
Separators = 7
*/

int StateTable[][8] = {
	{0, 1, 2, 3, 4, 5, 6, 7},
	{1, 1, 1, 0, 0, 0, 0, 0},
	{2, 0, 2, 3, 0, 0, 0, 0},
	{3, 0, 3, 0, 0, 0, 0, 0},
	{4, 1, 0, 0, 0, 0, 0, 0},
	{5, 0, 0, 0, 0, 0, 0, 0},
	{6, 6, 6, 6, 6, 5, 6, 6},
	{7, 0, 0, 0, 0, 0, 0, 0},
};
int currentState = 0; //global variable to keep track of the state
int previousState = 0;


//Declare functions
bool checkOperator(char c);
bool checkAlpha(char c);
bool checkNum(char c);
bool checkSep(char c);
bool checkKeyword(string s);
int findColumn(char c);
string tokenName(int n);
void lexer(string s, Token& n);
Token lexer1();
//

bool checkOperator(char cChar) { //cChar = current Character
	if ((cChar == '*') || (cChar == '+') || (cChar == '-') || (cChar == '=') ||
		(cChar == '/') || (cChar == '>') || (cChar == '<') || (cChar == '%')) {
		return true;
	}
	return false;
}
bool checkAlpha(char cChar) {
	if (cChar >= 'a' && cChar <= 'z') //Checks lowercase ascii
		return true;
	if (cChar >= 'A' && cChar <= 'Z') // Uppercase ascii
		return true;
	if (cChar == '_') //special case _
		return true;
	return false;
}

bool checkNum(char cChar) {
	if (cChar >= '0' && cChar <= '9') // 0-9 in ascii.
		return true;
	if (cChar == '$')
		return true;
	return false;
}
bool checkSep(char cChar) {
	if ((cChar == '\'') || (cChar == '(') || (cChar == ')') || (cChar == '{') ||
		(cChar == '}') || (cChar == '[') || (cChar == ']') || (cChar == ',')  ||
		(cChar == '.') || (cChar == ':') || (cChar == ';')
		) {
		return true;
	}
	return false;
}

bool checkKeyword(string input) {
	vector<string> keyWords{ "int", "float", "bool", "true", "false", "if", "else", "then", "endif", "while", "whilened", "do", "doend", "for", "forend", "input", "output", "and", "or", "not" };
	for (int i = 0; i < keyWords.size(); i++) {
		if (input.compare(keyWords[i]) == 0)
			return true;
	}
	return false;
}

int findColumn(char cChar) {
	if (cChar == 32 || cChar == NULL )  {
		return 5; // Space (first because white spaces are most common)
	}
	else if (checkAlpha(cChar)){
		return 1; //String
	} 
	else if (checkNum(cChar)) {
		return 2; //Integer
	}
	else if (cChar == '.') {
		return 3; //Real Numbers (requires INTEGER prior state)
	}
	else if (checkOperator(cChar)) {
		return 4; //Operators
	}
	else if (checkSep(cChar)) {
		return 7;
	}
	return 6; // Invalid
}

string tokenName(int n) {
	switch (n) {
	case 1:
		return "IDENTIFIER ";
		break;
	case 2:
		return "INTEGER    ";
		break;
	case 3:
		return "REAL       ";
		break;
	case 4:
		return "OPERATOR   ";
		break;
	case 5:
		return "SPACE      ";
		break;
	case 7:
		return "SEPARATOR  ";
		break;
	case 99: //Special case (not in state table)
		return "KEYWORD    ";
		break;
	default:
		return "INVALID   ";
		break;
	}
}



void lexer(string fileLine, Token& inputToken) {
	char inputChar = ' ';
	int column;
	string lexeme = "";


	for (int i = 0; i < fileLine.length(); ) {
		inputChar = fileLine[i];
		column = findColumn(inputChar);
		currentState = StateTable[currentState][column];
		
		if (currentState == 0) {
			if (previousState != 5) {
				if (previousState == 1) {
					if(checkKeyword(lexeme) == true)
						previousState = 99; //Special Keyword case
				}

				inputToken.lexeme.push_back(lexeme);
				inputToken.token.push_back(tokenName(previousState));
				inputToken.size++;
			}
			lexeme = "";
		}
		else {
			lexeme += inputChar;
			i++;
		}
		previousState = currentState;
	}

	if (currentState != 5 && lexeme != "") {
		inputToken.lexeme.push_back(lexeme);
		inputToken.token.push_back(tokenName(currentState));
		inputToken.size++;
	}
}


Token lexer1() {
	ifstream inFile; 
	ofstream ofFile;
	string inFileStr; // string varaiable to hold a "line".
	string fileName = "input.txt"; 
	Token outputToken; // passed in via reference to lexer function
	
	inFile.open(fileName);
	if (!inFile) {
		cout << "Unable to open file" << endl;;
		ofFile.open("input.txt");
		ofFile << "Replace with code. " << endl;
		cout << "input.txt file created. " << endl << "Please modify \"input.txt\" with code." << endl;
		ofFile.close();
		exit(1);
	}

	cout << "BEGINNING OF FILE" << endl;

	while (getline(inFile, inFileStr)){
		lexer(inFileStr, outputToken);
	}
	cout << endl << "TOKENS            =      LEXEMES" << endl << endl;
	for (unsigned int i = 0; i < outputToken.size; i++) {
		cout << outputToken.token[i] << "       =        " << outputToken.lexeme[i] << endl;
	}

	cout << endl  << "END OF FILE" << endl;
	inFile.close();

	ofFile.open("outputLexical.txt");
	ofFile << "TOKENS            =      LEXEMES" << endl;
	for (unsigned int i = 0; i < outputToken.size; i++) {
		ofFile << outputToken.token[i] << "       =        " << outputToken.lexeme[i] << endl;
	}

	ofFile.close();
	
	return outputToken;
}