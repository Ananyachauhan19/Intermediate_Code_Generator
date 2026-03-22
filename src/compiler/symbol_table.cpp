#include "symbol_table.h"
#include "structures.h"

// Add symbol to symbol table if not exists
void addToSymbolTable(const string& name, bool isTemp) {
    // Check if already exists
    for (const auto& entry : symbolTable) {
        if (entry.name == name) return;
    }
    symbolTable.push_back(SymbolTableEntry(name, "int", isTemp));
}

// Generate new temporary variable
string newTemp() {
    string temp = "t" + to_string(tempVarCounter++);
    addToSymbolTable(temp, true);
    return temp;
}