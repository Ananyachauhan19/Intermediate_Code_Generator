#include "tac_generator.h"
#include "structures.h"
#include "utils.h"
#include "symbol_table.h"
#include <stack>
#include <vector>

// Generate Three Address Code from postfix expression
string generateTAC(const string& postfix, const string& resultVar) {
    vector<string> tokens = tokenize(postfix);
    stack<string> operandStack;
    
    for (const string& token : tokens) {
        if (!isOperator(token[0]) || token.length() > 1) {
            // It's an operand
            operandStack.push(token);
        }
        else {
            // It's an operator
            if (operandStack.size() < 2) continue;
            
            string arg2 = operandStack.top(); operandStack.pop();
            string arg1 = operandStack.top(); operandStack.pop();
            
            string temp = newTemp();
            
            // Create TAC instruction
            TACInstruction tac(temp, arg1, token, arg2);
            tacInstructions.push_back(tac);
            
            // Create Quadruple
            Quadruple quad(token, arg1, arg2, temp);
            quadruples.push_back(quad);
            
            // Create Triple
            Triple triple(tripleIndex++, token, arg1, arg2);
            triples.push_back(triple);
            
            operandStack.push(temp);
        }
    }
    
    // Final assignment
    if (!operandStack.empty()) {
        string finalResult = operandStack.top();
        TACInstruction finalTac(resultVar, finalResult, "=", "");
        tacInstructions.push_back(finalTac);
        
        Quadruple finalQuad("=", finalResult, "", resultVar);
        quadruples.push_back(finalQuad);
        
        Triple finalTriple(tripleIndex++, "=", finalResult, "");
        triples.push_back(finalTriple);
        
        return finalResult;
    }
    
    return "";
}
