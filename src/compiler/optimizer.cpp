#include "optimizer.h"
#include "structures.h"
#include "utils.h"

// Evaluate constant expression
int evaluateConstant(int val1, char op, int val2) {
    switch(op) {
        case '+': return val1 + val2;
        case '-': return val1 - val2;
        case '*': return val1 * val2;
        case '/': return (val2 != 0) ? val1 / val2 : 0;
        default: return 0;
    }
}

// Perform constant folding optimization
void constantFolding() {
    for (auto& tac : tacInstructions) {
        if (tac.op == "=") continue;
        
        // Check if both operands are constants
        if (isNumber(tac.arg1) && isNumber(tac.arg2)) {
            int val1 = stoi(tac.arg1);
            int val2 = stoi(tac.arg2);
            int result = evaluateConstant(val1, tac.op[0], val2);
            
            // Store the constant value
            variableValues[tac.result] = to_string(result);
        }
    }
    
    // Replace variables with their constant values
    for (auto& tac : tacInstructions) {
        if (variableValues.find(tac.arg1) != variableValues.end()) {
            tac.arg1 = variableValues[tac.arg1];
        }
        if (variableValues.find(tac.arg2) != variableValues.end()) {
            tac.arg2 = variableValues[tac.arg2];
        }
        
        // Re-evaluate if both are constants now
        if (tac.op != "=" && isNumber(tac.arg1) && isNumber(tac.arg2)) {
            int val1 = stoi(tac.arg1);
            int val2 = stoi(tac.arg2);
            int result = evaluateConstant(val1, tac.op[0], val2);
            variableValues[tac.result] = to_string(result);
        }
    }
}

// Perform dead code elimination
void deadCodeElimination() {
    // Mark all variables that are used
    for (size_t i = 0; i < tacInstructions.size(); i++) {
        const auto& tac = tacInstructions[i];
        
        // Check if arg1 or arg2 use any result from previous instructions
        for (size_t j = 0; j < i; j++) {
            if (tacInstructions[j].result == tac.arg1 || 
                tacInstructions[j].result == tac.arg2) {
                tacInstructions[j].isUsed = true;
            }
        }
    }
    
    // The last instruction (final assignment) is always used
    if (!tacInstructions.empty()) {
        tacInstructions.back().isUsed = true;
        
        // Mark the source of final assignment as used
        string finalSource = tacInstructions.back().arg1;
        for (auto& tac : tacInstructions) {
            if (tac.result == finalSource) {
                tac.isUsed = true;
            }
        }
    }
}

// Apply all optimizations
void optimizeCode() {
    constantFolding();
    deadCodeElimination();
}