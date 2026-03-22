#include "display.h"
#include "structures.h"
#include <iostream>
#include <iomanip>

using namespace std;

// Display Three Address Code
void displayTAC(bool optimized) {
    cout << "\n";
    cout << "========================================\n";
    if (optimized) {
        cout << "    OPTIMIZED THREE ADDRESS CODE\n";
    } else {
        cout << "       THREE ADDRESS CODE (TAC)\n";
    }
    cout << "========================================\n";
    
    int lineNum = 1;
    for (const auto& tac : tacInstructions) {
        if (optimized && !tac.isUsed) continue;
        
        cout << lineNum++ << ". ";
        
        if (tac.op == "=") {
            // Check if source is a constant
            if (variableValues.find(tac.arg1) != variableValues.end() && optimized) {
                cout << tac.result << " = " << variableValues[tac.arg1];
            } else {
                cout << tac.result << " = " << tac.arg1;
            }
        } else {
            // Check for constant values
            string a1 = tac.arg1;
            string a2 = tac.arg2;
            
            if (optimized) {
                if (variableValues.find(tac.arg1) != variableValues.end()) {
                    a1 = variableValues[tac.arg1];
                }
                if (variableValues.find(tac.arg2) != variableValues.end()) {
                    a2 = variableValues[tac.arg2];
                }
            }
            
            cout << tac.result << " = " << a1 << " " << tac.op << " " << a2;
        }
        cout << endl;
    }
}

// Display Quadruples table
void displayQuadruples() {
    cout << "\n";
    cout << "========================================\n";
    cout << "            QUADRUPLES TABLE\n";
    cout << "========================================\n";
    cout << left << setw(8) << "Index" 
         << setw(8) << "Op" 
         << setw(10) << "Arg1" 
         << setw(10) << "Arg2" 
         << setw(10) << "Result" << endl;
    cout << "----------------------------------------\n";
    
    for (size_t i = 0; i < quadruples.size(); i++) {
        const auto& quad = quadruples[i];
        cout << left << setw(8) << i 
             << setw(8) << quad.op 
             << setw(10) << quad.arg1 
             << setw(10) << (quad.arg2.empty() ? "-" : quad.arg2)
             << setw(10) << quad.result << endl;
    }
}

// Display Triples table
void displayTriples() {
    cout << "\n";
    cout << "========================================\n";
    cout << "              TRIPLES TABLE\n";
    cout << "========================================\n";
    cout << left << setw(8) << "Index" 
         << setw(8) << "Op" 
         << setw(12) << "Arg1" 
         << setw(12) << "Arg2" << endl;
    cout << "----------------------------------------\n";
    
    for (const auto& triple : triples) {
        cout << left << setw(8) << triple.index 
             << setw(8) << triple.op 
             << setw(12) << triple.arg1 
             << setw(12) << (triple.arg2.empty() ? "-" : triple.arg2) << endl;
    }
}

// Display Symbol Table
void displaySymbolTable() {
    cout << "\n";
    cout << "========================================\n";
    cout << "            SYMBOL TABLE\n";
    cout << "========================================\n";
    cout << left << setw(15) << "Variable" 
         << setw(10) << "Type" 
         << setw(15) << "Is Temporary" << endl;
    cout << "----------------------------------------\n";
    
    for (const auto& entry : symbolTable) {
        cout << left << setw(15) << entry.name 
             << setw(10) << entry.type 
             << setw(15) << (entry.isTemporary ? "Yes" : "No") << endl;
    }
}