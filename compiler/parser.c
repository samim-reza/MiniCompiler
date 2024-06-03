#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

void parse(char *source) {
    Token *token = getNextToken(source);

    // Parse the main function
    if (token->type != TOKEN_INT) {
        printf("Error: Expected 'int' in main function declaration.\n");
        exit(1);
    }
    token = getNextToken(source);
    if (token->type != TOKEN_IDENTIFIER || strcmp(token->value, "main") != 0) {
        printf("Error: Expected 'main' function name.\n");
        exit(1);
    }
    token = getNextToken(source);
    if (token->type != TOKEN_LPAREN) {
        printf("Error: Missing '(' after main function name.\n");
        exit(1);
    }
    token = getNextToken(source);
    if (token->type != TOKEN_RPAREN) {
        printf("Error: Missing ')' after '(' in main function declaration.\n");
        exit(1);
    }
    token = getNextToken(source);
    if (token->type != TOKEN_LBRACE) {
        printf("Error: Missing '{' after main function declaration.\n");
        exit(1);
    }

    // Parse the body of the main function
    while (token->type != TOKEN_RBRACE) {
        // Skip tokens until we find the closing brace '}' of the main function
        token = getNextToken(source);
    }

    // Parse the return statement
    token = getNextToken(source);
    if (token->type != TOKEN_RETURN) {
        printf("Error: Expected 'return' statement in main function.\n");
        exit(1);
    }
    token = getNextToken(source);
    if (token->type != TOKEN_INT_LITERAL) {
        printf("Error: Expected integer literal after 'return' statement.\n");
        exit(1);
    }

    // Parse should stop at the end of the input, so this last token must be EOF
    token = getNextToken(source);
    if (token->type != TOKEN_EOF) {
        printf("Error: Unexpected token after main function.\n");
        exit(1);
    }

    printf("Parsing successful.\n");
    free(source);
}
