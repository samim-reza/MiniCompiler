// compiler.h
#ifndef COMPILER_H
#define COMPILER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Token types
typedef enum {
    TOKEN_INT, TOKEN_IDENTIFIER, TOKEN_ASSIGN, TOKEN_SEMICOLON, TOKEN_PLUS,
    TOKEN_MINUS, TOKEN_MULTIPLY, TOKEN_DIVIDE, TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_EOF
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    char *value;
} Token;

// Node types
typedef enum {
    NODE_INT, NODE_VAR, NODE_ASSIGN, NODE_PLUS, NODE_MINUS, NODE_MULTIPLY, NODE_DIVIDE
} NodeType;

// AST Node structure
typedef struct Node {
    NodeType type;
    union {
        int value;        // for integer literals
        char *name;       // for variable names
        struct {
            struct Node *left;   // left operand
            struct Node *right;  // right operand
        } binop;          // for binary operations
        struct {
            struct Node *left;   // variable
            struct Node *right;  // expression
        } assign;         // for assignment nodes
    };
} Node;

// Tokenizer functions
Token *new_token(TokenType type, const char *value);
Token *get_next_token(const char **input);

// Parser functions
Node *parse_statement(const char **input);
Node *parse_expression(const char **input, Token **current_token);
Node *parse_term(const char **input, Token **current_token);
Node *parse_primary(const char **input, Token **current_token);
void match(TokenType type, const char **input, Token **current_token);
void next_token(const char **input, Token **current_token);

// Code generator function
void generate_code(Node *node);

#endif // COMPILER_H
