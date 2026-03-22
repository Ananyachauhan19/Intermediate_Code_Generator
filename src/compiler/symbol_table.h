#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>

using namespace std;

// Add symbol to symbol table if not exists
void addToSymbolTable(const string& name, bool isTemp = false);

// Generate new temporary variable
string newTemp();

#endif