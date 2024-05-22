#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"
#include "parser.h"

#define MAX_SYMBOLS 100

typedef struct {
    char name[MAX_TOKEN_LENGTH];
    int value;
} Symbol;

Symbol symbolTable[MAX_SYMBOLS];
int symbolCount = 0;

int lookup(char* name) {
    for (int i = 0; i < symbolCount; ++i) {
        if (strcmp(symbolTable[i].name, name) == 0) {
            return symbolTable[i].value;
        }
    }
    fprintf(stderr, "Error: Undefined variable '%s'\n", name);
    exit(EXIT_FAILURE);
}

void insert(char* name, int value) {
    for (int i = 0; i < symbolCount; ++i) {
        if (strcmp(symbolTable[i].name, name) == 0) {
            symbolTable[i].value = value;
            return;
        }
    }
    if (symbolCount < MAX_SYMBOLS) {
        strcpy(symbolTable[symbolCount].name, name);
        symbolTable[symbolCount].value = value;
        ++symbolCount;
    } else {
        fprintf(stderr, "Error: Symbol table full\n");
        exit(EXIT_FAILURE);
    }
}

int evaluateExpression(Node* node) {
    if (node->token.type == TOKEN_NUMBER) {
        return atoi(node->token.value);
    } else if (node->token.type == TOKEN_IDENTIFIER) {
        return lookup(node->token.value);
    } else if (node->token.type == TOKEN_OPERATOR) {
        int leftValue = evaluateExpression(node->left);
        int rightValue = evaluateExpression(node->right);
        switch (node->token.value[0]) {
            case '+': return leftValue + rightValue;
            case '-': return leftValue - rightValue;
            case '*': return leftValue * rightValue;
            case '/': return leftValue / rightValue;
            default:
                fprintf(stderr, "Error: Unknown operator '%s'\n", node->token.value);
                exit(EXIT_FAILURE);
        }
    }
    fprintf(stderr, "Error: Invalid expression\n");
    exit(EXIT_FAILURE);
}

void evaluate(Node* node) {
    if (node == NULL) return;

    if (node->token.type == TOKEN_PRINT) {
        int value = evaluateExpression(node->right);
        printf("%d\n", value);
    } else if (node->token.type == TOKEN_ASSIGN) {
        int value = evaluateExpression(node->right);
        insert(node->left->token.value, value);
    } else {
        fprintf(stderr, "Error: Invalid statement\n");
        exit(EXIT_FAILURE);
    }
}

void freeSymbolTable() {
    symbolCount = 0;  // Reset the symbol table
}
