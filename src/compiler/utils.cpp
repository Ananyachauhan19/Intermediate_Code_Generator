#include "utils.h"
#include <cctype>

// Check if string is a number
bool isNumber(const string& str) {
    if (str.empty()) return false;
    for (size_t i = 0; i < str.length(); i++) {
        if (i == 0 && str[i] == '-') continue;
        if (!isdigit(str[i])) return false;
    }
    return true;
}

// Check if character is an operator
bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

// Get operator precedence
int getPrecedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Tokenize expression
vector<string> tokenize(const string& expr) {
    vector<string> tokens;
    string token = "";
    
    for (size_t i = 0; i < expr.length(); i++) {
        char c = expr[i];
        
        if (isspace(c)) {
            if (!token.empty()) {
                tokens.push_back(token);
                token = "";
            }
        }
        else if (isOperator(c) || c == '(' || c == ')') {
            if (!token.empty()) {
                tokens.push_back(token);
                token = "";
            }
            tokens.push_back(string(1, c));
        }
        else {
            token += c;
        }
    }
    
    if (!token.empty()) {
        tokens.push_back(token);
    }
    
    return tokens;
}