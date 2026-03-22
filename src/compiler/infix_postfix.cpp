#include "infix_postfix.h"
#include "utils.h"
#include "symbol_table.h"
#include <stack>
#include <vector>

// Convert infix expression to postfix notation
string infixToPostfix(const string& infix) {
    vector<string> tokens = tokenize(infix);
    stack<string> operatorStack;
    string postfix = "";
    
    for (const string& token : tokens) {
        // If operand (variable or number)
        if (!isOperator(token[0]) || (token.length() > 1)) {
            if (token != "(" && token != ")") {
                postfix += token + " ";
                // Add to symbol table if it's a variable
                if (!isNumber(token)) {
                    addToSymbolTable(token, false);
                }
            }
        }
        // If opening parenthesis
        else if (token == "(") {
            operatorStack.push(token);
        }
        // If closing parenthesis
        else if (token == ")") {
            while (!operatorStack.empty() && operatorStack.top() != "(") {
                postfix += operatorStack.top() + " ";
                operatorStack.pop();
            }
            if (!operatorStack.empty()) {
                operatorStack.pop(); // Remove '('
            }
        }
        // If operator
        else if (isOperator(token[0])) {
            while (!operatorStack.empty() && 
                   operatorStack.top() != "(" &&
                   getPrecedence(operatorStack.top()[0]) >= getPrecedence(token[0])) {
                postfix += operatorStack.top() + " ";
                operatorStack.pop();
            }
            operatorStack.push(token);
        }
    }
    
    // Pop remaining operators
    while (!operatorStack.empty()) {
        postfix += operatorStack.top() + " ";
        operatorStack.pop();
    }
    
    return postfix;
}