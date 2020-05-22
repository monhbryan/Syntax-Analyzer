//Analyzer.cpp
//Bryan Monh
#include <iostream>
#include "lexer.h"
using namespace std;
// GLOBAL VARIABLES
int GLOBALcounter = 0;
multimap<int, string> errorMap;
multimap<int, string> prodrule;
/* RULES
<Expression> -> <Term> <ExpressionPrime>
<ExpressionPrime> -> +<Term><ExpressionPrime> | -<Term><ExpressionPrime> | epsilon
<Term> -> <Factor><<TermPrime>>
<<TermPrime>> -> *<Factor><T>'| /<Factor><<TermPrime>> | epsilon
<Factor> -> <ID> | ( <Expression> )
<ID> -> id
<Statement> -> <Assign> | <Declarative>
<Assign> -> <ID> = <Expression>;
<Declarative> -> <Type> <ID>
*/
//Rule Functions
bool expression(Token s);
bool expression2(Token s);
bool factor(Token s);
bool term(Token s);
bool term2(Token s);
//bool num(Token s); (implemented individually)
//bool id(Token s); (implemented individually) 
bool statement(Token s);
bool assign(Token s);
bool declarative(Token s);
bool type(Token s);

//Helper Functions
void increment(Token s);
void printType(string token, string lexeme);
void errorReport(string error);
void storeRule(Token s, string rule);
//BEGIN GRAMMAR FUNCTIONS
bool expression(Token s) { // <Expression> -> <Term> <ExpressionPrime> where <ExpressionPrime> == expression2
	bool result = false;
	cout << "<Expression> -> <Term> <ExpressionPrime> " << endl;
	storeRule(s, "<Expression > -> <Term> <ExpressionPrime>");
	if (term(s)) {

		if (expression2(s)) {

			result = true;
		}
		else {
			errorReport("Expected <ExpressionPrime>");
		}
	}

	return result;
}
bool expression2(Token s) { // <ExpressionPrime> -> +ExpressionPrime | -ExpressionPrime
	bool result = false;

	if (s.lexeme[GLOBALcounter][0] == '+' || s.lexeme[GLOBALcounter][0] == '-') {
		cout << "<ExpressionPrime> -> +<Term><ExpressionPrime> || -<Term><ExpressionPrime> " << endl;
		storeRule(s, "<ExpressionPrime> -> +<Term><ExpressionPrime> || -<Term><ExpressionPrime>");
		increment(s);

		if (term(s)) {
			if (expression2(s)) {
				result = true;
			}
			else {
				errorReport("Expected <Expression Prime>");
			}
		}
		else {
			errorReport("Expected <Term>");
		}
	}
	else if (s.lexeme[GLOBALcounter][0] == ')' || s.lexeme[GLOBALcounter][0] == ';' || s.lexeme[GLOBALcounter][0] == '$') {
		result = true;
		cout << "<ExpressionPrime> -> epsilon " << endl;
		storeRule(s, "<ExpressionPrime> -> epsilon ");

	}

	return result;
}
bool term(Token s) { // <Term> -> <Factor> <TermPrime>
	bool result = false;
	int b = GLOBALcounter;
	printType(s.token[GLOBALcounter], s.lexeme[GLOBALcounter]);
	cout << "<Term> -> <Factor> <TermPrime>" << endl;
	storeRule(s, "<Term> -> <Factor> <TermPrime>");
	if (factor(s)) {
		if (term2(s)) {
			cout << "<Term> -> <Factor>1 <TermPrime> " << endl;
			//storeRule(s,"<Term> -> <Factor> <TermPrime> " );
			result = true;
		}
		/*else {
			errorReport("Expected <TermPrime>");
		}*/
	}
	else {
		errorReport("Expected <Term>");
	}
	return result;
}

bool term2(Token s) { // <<TermPrime>> -> *<TermPrime> | /<TermPrime> | epsilon
	bool result = false;

	if (s.lexeme[GLOBALcounter][0] == '*' || s.lexeme[GLOBALcounter][0] == '/' ){
		printType(s.token[GLOBALcounter], s.lexeme[GLOBALcounter]);
		cout << "<TermPrime> -> *<TermPrime> | /<TermPrime> " << endl;
		storeRule(s, "<TermPrime> -> *<TermPrime> | /<TermPrime> ");
		increment(s);
		printType(s.token[GLOBALcounter], s.lexeme[GLOBALcounter]);
		if (factor(s)) {
			if (term2(s)) {
				result = true;
			}
			else
				errorReport("Expected <TermPrime>");
		}
		else
			errorReport("Expected <Factor>");
		}
	else if (s.lexeme[GLOBALcounter][0] == '+' || s.lexeme[GLOBALcounter][0] == '-' || s.lexeme[GLOBALcounter][0] == ')' || s.lexeme[GLOBALcounter][0] == ';' || s.lexeme[GLOBALcounter][0] == '$') {
		printType(s.token[GLOBALcounter], s.lexeme[GLOBALcounter]);
		result = true;
		cout << "<TermPrime> -> epsilon " << endl;
		storeRule(s, "<TermPrime> -> epsilon ");
	}
	return result;
}

bool factor(Token s) {// <Factor> -> <ID> | ( <Expression> ) | <num>
	bool result = false;
	int b = GLOBALcounter;
	if (isalpha(s.lexeme[GLOBALcounter][0]) && s.token[GLOBALcounter] != "KEYWORD") {// <ID>
		cout << "<Factor> -> id " << endl;
		storeRule(s,"<Factor> -> id " ); 
		increment(s);
		result = true;
	}

	if (isdigit(s.lexeme[GLOBALcounter][0])) {// <num>
		cout << "<Factor> -> num " << endl;
		storeRule(s, "<Factor> -> num ");
		increment(s);
		result = true;
	}
	if (s.lexeme[GLOBALcounter][0] == '(') {
		cout << "<Factor> -> ( <Expression> ) " << endl;
		storeRule(s,"<Factor> -> ( <Expression> ) ");
		increment(s);
		if (expression(s)) {
			if (s.lexeme[GLOBALcounter][0] == ')') {
				increment(s);
				result = true;
			}
		}
		else
			errorReport("Expected <Expression>");
	}

	return result;
}
/*bool id(Token s) {
	bool result = false;
	if (isalpha(s.lexeme[GLOBALcounter][0])) {// <ID>
		increment(s);
		result = true;
	}
	return result;
}*/


/*bool num(Token s) {
	bool result = false;

	if (isdigit(s.lexeme[GLOBALcounter][0])) {// <num>
		increment(s);
		result = true;
	}
	return result;
}*/

bool statement(Token s) {//<Statement> -> <Assign>
	bool result = false;
	printType(s.token[GLOBALcounter], s.lexeme[GLOBALcounter]);
	if (declarative(s)) {
		cout << "<Statement>1 -> <Declarative> " << endl;
		storeRule(s, "<Statement>1 -> <Declarative> ");
		result = true;
	}
	else
		errorReport("Expected <Declarative>");

	cout << "<Statement> -> <Assign> " << endl;
	storeRule(s,"<Statement> -> <Assign> " );
	if (assign(s)) {
		result = true;
	}
	else
		errorReport("Expected <Assign>");


	return result;
}

bool assign(Token s) {//<Assign> -> <ID> = <Expression>;
	bool result = false;
	cout << "<Assign> -> <ID> = <Expression> " << endl;
	storeRule(s,"<Assign> -> <ID> = <Expression> " );
	if (isalpha(s.lexeme[GLOBALcounter][0])) {// <ID>
		cout << "<ID> -> id " << endl;
		storeRule(s,"<ID> -> id " );
		increment(s);
		if (s.lexeme[GLOBALcounter][0] == '=') {
			printType(s.token[GLOBALcounter], s.lexeme[GLOBALcounter]);
			increment(s);
			if (expression(s)) {
				result = true;
			}
			else
				errorReport("Expected <Expression>");
		}
	}

	return result;
}
	bool declarative(Token s) {//<Declarative> -> <Type> <ID>
		bool result = false;

		if (type(s) && s.token[GLOBALcounter+1] != "OPERATOR") {
			if (isalpha(s.lexeme[GLOBALcounter][0])) {// <ID>
				printType(s.token[GLOBALcounter], s.lexeme[GLOBALcounter]);
				cout << "<ID> -> id " << endl;
				storeRule(s,"<ID> -> id " );
				increment(s);
				if (s.lexeme[GLOBALcounter][0] == ';') {
					printType(s.token[GLOBALcounter], s.lexeme[GLOBALcounter]);
					cout << "<Declarative> -> <Type> <ID>; " << endl;
					storeRule(s, "<Declarative> -> <Type> <ID>; ");
					increment(s);
					result = true;
				}
				else
					errorReport("Expected ;");
			}
		}
		return result;
	}

bool type(Token s) { // <Type> -> type
	bool result = false;
	vector<string> keytypes = { "int", "bool", "double", "char", "float", "boolean" };
	for (int i = 0; i < keytypes.size(); i++) {
		if (s.lexeme[GLOBALcounter] == keytypes[i]) {
			cout << "<Type> = keyword " << endl;
			storeRule(s,"<Type> = keyword " );
			increment(s);
			result = true;
		}
	}
	return result;
}
//END GRAMMAR RULES


void increment(Token S) { // helper function for global variable ...
	if (GLOBALcounter < S.size) {
		GLOBALcounter++;
		cout <<"____________________" << endl;
	}
}


//Prints out type of token and lexeme identified
void printType(string token, string lexeme) {
	cout << "Token: " << token << "---------------Lexeme: " << lexeme << endl; 
}

//Helper function to report errors - go through later to check for errors.
//int contains position of error -> due to how Token is stored - we are able to check for lexeme + token.
void errorReport(string error) {
	errorMap.insert(make_pair(GLOBALcounter, error));
}

//Helper function to store prodrules
void storeRule(Token s, string prod) {
	prodrule.insert(make_pair(GLOBALcounter,prod));
}

int main() {
	Token inputToken = lexer1();

	inputToken.lexeme.push_back("$");
	inputToken.token.push_back("END OF LINE");
	for (int i = 0; i < inputToken.size; i++) {
		printType(inputToken.token[i], inputToken.lexeme[i]); 
	}
	cout << endl;
	bool expFlag = 0; // If found, stop checking
	bool stateFlag = 0;
	while(GLOBALcounter < inputToken.size){
		expFlag = 0;
		stateFlag = 0;
		if (expression(inputToken) == true) {
			cout << GLOBALcounter << endl;
			cout << endl << "EXPRESSION TRUE" << endl;
			expFlag = 1;
		}
		else {
			cout << "expError " << endl << endl;
			GLOBALcounter--;
		}
		if (statement(inputToken) == true) {
				cout << GLOBALcounter << endl;
				cout << "Statement TRUE" << endl;
				stateFlag = 1;
		}
		else  {
			cout << "stateError " << endl << endl;
			GLOBALcounter--;
		}
		
		increment(inputToken);
	} // end of while

	//FILE OUTPUT SECTION
	typedef multimap<int, string>::iterator MMAPIterator;
	ofstream ofFile;
	ofFile.open("outputAnalyze.txt");
	ofFile << "BEGINNING OF SYNTAX ANALYZER" << endl;
	for (unsigned int i = 0; i < inputToken.size; i++) {
		ofFile << endl << "Token: " << inputToken.token[i] << " || Lexeme: " << inputToken.lexeme[i] << endl;

		pair<MMAPIterator, MMAPIterator> result = prodrule.equal_range(i);
		pair<MMAPIterator, MMAPIterator> errresult = errorMap.equal_range(i);
		for (MMAPIterator it = result.first; it != result.second; it++) {
			ofFile << "   " << it->second << endl;
		}
		for (MMAPIterator it = errresult.first; it != errresult.second; it++) {
			
			ofFile << "Character count #: " << i << " error:  " << it->second << endl;
		}
	}
	ofFile << "END OF SYNTAX ANALYZER" << endl;
	ofFile.close();

	return 0;
}