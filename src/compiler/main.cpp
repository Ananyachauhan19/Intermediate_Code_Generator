#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "structures.h"
#include "utils.h"
#include "symbol_table.h"
#include "infix_postfix.h"
#include "tac_generator.h"
#include "optimizer.h"
#include "display.h"

using namespace std;

// Initialize global variables
vector<SymbolTableEntry> symbolTable;
vector<TACInstruction> tacInstructions;
vector<Quadruple> quadruples;
vector<Triple> triples;
map<string, string> variableValues;
int tempVarCounter = 1;
int tripleIndex = 0;

static string trimCopy(const string& s) {
    size_t start = 0;
    while (start < s.size() && isspace(static_cast<unsigned char>(s[start]))) start++;

    if (start == s.size()) return "";

    size_t end = s.size() - 1;
    while (end > start && isspace(static_cast<unsigned char>(s[end]))) end--;

    return s.substr(start, end - start + 1);
}

static string formatTokenList(const vector<string>& tokens) {
    ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < tokens.size(); i++) {
        oss << tokens[i];
        if (i + 1 < tokens.size()) oss << ",";
    }
    oss << "]";
    return oss.str();
}

static bool parseAssignment(const string& statement, string& lhsOut, string& rhsOut) {
    size_t equalPos = statement.find('=');
    if (equalPos == string::npos) {
        return false;
    }

    string lhs = statement.substr(0, equalPos);
    string rhs = statement.substr(equalPos + 1);

    lhs = trimCopy(lhs);
    rhs = trimCopy(rhs);

    // Remove spaces from LHS identifier
    lhs.erase(remove_if(lhs.begin(), lhs.end(), ::isspace), lhs.end());

    if (lhs.empty() || rhs.empty()) return false;

    lhsOut = lhs;
    rhsOut = rhs;
    return true;
}

// Process a single assignment statement
void processStatement(const string& lhs, const string& rhs, const string& postfix) {
    // Add LHS to symbol table
    addToSymbolTable(lhs, false);

    // Generate TAC from already-computed postfix
    generateTAC(postfix, lhs);
}

// Reset all state variables
void resetState() {
    symbolTable.clear();
    tacInstructions.clear();
    quadruples.clear();
    triples.clear();
    variableValues.clear();
    tempVarCounter = 1;
    tripleIndex = 0;
}

// Write TAC (original or optimized) to a file
void writeTACToFile(const string& filename, bool optimized) {
    ofstream out(filename.c_str());
    if (!out.is_open()) {
        return;
    }

    int lineNum = 1;
    for (const auto& tac : tacInstructions) {
        if (optimized && !tac.isUsed) continue;

        out << lineNum++ << ". ";

        if (tac.op == "=") {
            // Check if source is a constant
            if (variableValues.find(tac.arg1) != variableValues.end() && optimized) {
                out << tac.result << " = " << variableValues[tac.arg1];
            } else {
                out << tac.result << " = " << tac.arg1;
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

            out << tac.result << " = " << a1 << " " << tac.op << " " << a2;
        }
        out << '\n';
    }
}

// Write Quadruples table to a file
void writeQuadruplesToFile(const string& filename) {
    ofstream out(filename.c_str());
    if (!out.is_open()) {
        return;
    }

    out << "Index\tOp\tArg1\tArg2\tResult" << '\n';
    for (size_t i = 0; i < quadruples.size(); i++) {
        const auto& quad = quadruples[i];
        out << i << '\t'
            << quad.op << '\t'
            << quad.arg1 << '\t'
            << (quad.arg2.empty() ? "-" : quad.arg2) << '\t'
            << quad.result << '\n';
    }
}

// Write Symbol Table to a file
void writeSymbolTableToFile(const string& filename) {
    ofstream out(filename.c_str());
    if (!out.is_open()) {
        return;
    }

    out << "Variable\tType\tIsTemporary" << '\n';
    for (const auto& entry : symbolTable) {
        out << entry.name << '\t'
            << entry.type << '\t'
            << (entry.isTemporary ? "Yes" : "No") << '\n';
    }
}

int main() {
    cout << "\n";
    cout << "********************************************\n";
    cout << "*    INTERMEDIATE CODE GENERATOR (ICG)    *\n";
    cout << "*         Compiler Design Project         *\n";
    cout << "********************************************\n";
    
    vector<string> statements;
    string input;
    
    cout << "\nEnter assignment statements (one per line).\n";
    cout << "Type 'END' when finished:\n\n";
    
    // Read multiple statements
    while (true) {
        cout << "> ";

        // Stop if we hit EOF or input error (e.g., when input is piped
        // from a file and ends without an explicit "END" line).
        if (!getline(cin, input)) {
            break;
        }
        
        if (input == "END" || input == "end") {
            break;
        }
        
        if (!input.empty()) {
            statements.push_back(input);
        }
    }
    
    if (statements.empty()) {
        cout << "\nNo statements entered. Exiting...\n";
        return 0;
    }
    
    // Process each statement
    cout << "\n\n";
    cout << "========================================\n";
    cout << "         PROCESSING STATEMENTS\n";
    cout << "========================================\n";
    
    ofstream parserOut("outputs/expression_parser.txt");

    for (size_t i = 0; i < statements.size(); i++) {
        cout << (i + 1) << ". " << statements[i] << endl;

        string lhs, rhs;
        if (!parseAssignment(statements[i], lhs, rhs)) {
            cout << "Error: Invalid assignment statement\n";
            continue;
        }

        vector<string> tokens = tokenize(rhs);
        string postfix = infixToPostfix(rhs);
        postfix = trimCopy(postfix);

        if (parserOut.is_open()) {
            if (i > 0) parserOut << "\n\n";
            parserOut << "EXPRESSION PARSER │\n";
            parserOut << "  • Tokenize: " << formatTokenList(tokens) << "\n";
            parserOut << "  • Convert: " << postfix;
        }

        processStatement(lhs, rhs, postfix);
    }
    
    // Close the parser output file
    if (parserOut.is_open()) {
        parserOut.close();
    }
    
    // Display all outputs
    displayTAC(false);
    displayQuadruples();
    displayTriples();
    displaySymbolTable();

    // Also write outputs to files for the web frontend
    writeTACToFile("outputs/tac.txt", false);
    writeQuadruplesToFile("outputs/quadruples.txt");
    writeSymbolTableToFile("outputs/symbol_table.txt");
    
    // Optimization
    optimizeCode();
    displayTAC(true);
    writeTACToFile("outputs/optimized_tac.txt", true);
    
    cout << "\n";
    cout << "========================================\n";
    cout << "    COMPILATION COMPLETED SUCCESSFULLY\n";
    cout << "========================================\n";
    cout << "\n";
    
    return 0;
}