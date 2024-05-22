
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "lexer.h"

Node* createNode(Token token) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->token = token;
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node* parseExpression(const char** input);

Node* parseFactor(const char** input) {
    Token token = getNextToken(input);
    Node* node = createNode(token);

    if (token.type == TOKEN_NUMBER || token.type == TOKEN_IDENTIFIER) {
        return node;
    } else if (token.type == TOKEN_OPERATOR && token.value[0] == '(') {
        node = parseExpression(input);
        token = getNextToken(input);
        if (token.type != TOKEN_OPERATOR || token.value[0] != ')') {
            fprintf(stderr, "Error: Expected ')'\n");
            exit(EXIT_FAILURE);
        }
        return node;
    }

    fprintf(stderr, "Error: Unexpected token '%s'\n", token.value);
    exit(EXIT_FAILURE);
    return NULL;
}

Node* parseTerm(const char** input) {
    Node* node = parseFactor(input);
    Token token = getNextToken(input);

    while (token.type == TOKEN_OPERATOR && (token.value[0] == '*' || token.value[0] == '/')) {
        Node* newNode = createNode(token);
        newNode->left = node;
        newNode->right = parseFactor(input);
        node = newNode;
        token = getNextToken(input);
    }

    *input -= strlen(token.value);
    return node;
}

Node* parseExpression(const char** input) {
    Node* node = parseTerm(input);
    Token token = getNextToken(input);

    while (token.type == TOKEN_OPERATOR && (token.value[0] == '+' || token.value[0] == '-')) {
        Node* newNode = createNode(token);
        newNode->left = node;
        newNode->right = parseTerm(input);
        node = newNode;
        token = getNextToken(input);
    }

    *input -= strlen(token.value);
    return node;
}

Node* parseStatement(const char** input) {
    Token token = getNextToken(input);
    Node* node = NULL;

    if (token.type == TOKEN_IDENTIFIER) {
        Node* idNode = createNode(token);
        token = getNextToken(input);
        if (token.type == TOKEN_ASSIGN) {
            Node* assignNode = createNode(token);
            assignNode->left = idNode;
            assignNode->right = parseExpression(input);
            node = assignNode;
            token = getNextToken(input);
            if (token.type != TOKEN_SEMICOLON) {
                fprintf(stderr, "Error: Expected ';'\n");
                exit(EXIT_FAILURE);
            }
        } else {
            fprintf(stderr, "Error: Expected '='\n");
            exit(EXIT_FAILURE);
        }
    } else if (token.type == TOKEN_PRINT) {
        Node* printNode = createNode(token);
        printNode->right = parseExpression(input);
        node = printNode;
        token = getNextToken(input);
        if (token.type != TOKEN_SEMICOLON) {
            fprintf(stderr, "Error: Expected ';'\n");
            exit(EXIT_FAILURE);
        }
    } else {
        fprintf(stderr,"Error: Unexpected token '%s'\n", token.value);
exit(EXIT_FAILURE);
}
return node;
}

void printParseTree(Node* node) {
if (node == NULL) return;
printf("Node: %s\n", node->token.value);
printParseTree(node->left);
printParseTree(node->right);
}

void freeParseTree(Node* node) {
if (node == NULL) return;
freeParseTree(node->left);
freeParseTree(node->right);
free(node);
}