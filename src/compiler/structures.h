#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <string>
#include <vector>
#include <map>

using namespace std;

// Symbol Table Entry
struct SymbolTableEntry {
    string name;
    string type;
    bool isTemporary;
    
    SymbolTableEntry(string n = "", string t = "int", bool temp = false)
        : name(n), type(t), isTemporary(temp) {}
};

// Quadruple Structure: (op, arg1, arg2, result)
struct Quadruple {
    string op;
    string arg1;
    string arg2;
    string result;
    
    Quadruple(string o = "", string a1 = "", string a2 = "", string r = "")
        : op(o), arg1(a1), arg2(a2), result(r) {}
};

// Triple Structure: (op, arg1, arg2)
struct Triple {
    int index;
    string op;
    string arg1;
    string arg2;
    
    Triple(int i = 0, string o = "", string a1 = "", string a2 = "")
        : index(i), op(o), arg1(a1), arg2(a2) {}
};

// TAC Instruction
struct TACInstruction {
    string result;
    string arg1;
    string op;
    string arg2;
    bool isUsed;
    
    TACInstruction(string r = "", string a1 = "", string o = "", string a2 = "")
        : result(r), arg1(a1), op(o), arg2(a2), isUsed(true) {}
};

// Global data structures
extern vector<SymbolTableEntry> symbolTable;
extern vector<TACInstruction> tacInstructions;
extern vector<Quadruple> quadruples;
extern vector<Triple> triples;
extern map<string, string> variableValues;
extern int tempVarCounter;
extern int tripleIndex;

#endif