#ifndef TAC_GENERATOR_H
#define TAC_GENERATOR_H

#include <string>

using namespace std;

// Generate Three Address Code from postfix expression
string generateTAC(const string& postfix, const string& resultVar);

#endif
