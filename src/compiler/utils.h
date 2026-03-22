#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

using namespace std;

// Check if string is a number
bool isNumber(const string& str);

// Check if character is an operator
bool isOperator(char c);

// Get operator precedence
int getPrecedence(char op);

// Tokenize expression
vector<string> tokenize(const string& expr);

#endif