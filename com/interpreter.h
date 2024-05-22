#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"

void evaluate(Node* node);
void freeSymbolTable();

#endif // INTERPRETER_H