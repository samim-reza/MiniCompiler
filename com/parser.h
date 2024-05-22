#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef struct Node {
Token token;
struct Node *left;
struct Node *right;
} Node;

Node* parseStatement(const char** input);
void printParseTree(Node* node);
void freeParseTree(Node* node);

#endif // PARSER_H