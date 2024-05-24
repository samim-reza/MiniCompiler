// tokenizer.c
#include "compiler.h"

// Function to create a new token
Token *new_token(TokenType type, const char *value) {
    Token *token = (Token *)malloc(sizeof(Token));
    token->type = type;
    token->value = strdup(value);
    return token;
}

// Simple tokenizer
Token *get_next_token(const char **input) {
    while (isspace(**input)) (*input)++;
    
    if (**input == '\0') return new_token(TOKEN_EOF, "");

    if (isdigit(**input)) {
        const char *start = *input;
        while (isdigit(**input)) (*input)++;
        return new_token(TOKEN_INT, strndup(start, *input - start));
    }

    if (isalpha(**input)) {
        const char *start = *input;
        while (isalnum(**input)) (*input)++;
        return new_token(TOKEN_IDENTIFIER, strndup(start, *input - start));
    }

    if (**input == '=') {
        (*input)++;
        return new_token(TOKEN_ASSIGN, "=");
    }

    if (**input == ';') {
        (*input)++;
        return new_token(TOKEN_SEMICOLON, ";");
    }

    if (**input == '+') {
        (*input)++;
        return new_token(TOKEN_PLUS, "+");
    }

    if (**input == '-') {
        (*input)++;
        return new_token(TOKEN_MINUS, "-");
    }

    if (**input == '*') {
        (*input)++;
        return new_token(TOKEN_MULTIPLY, "*");
    }

    if (**input == '/') {
        (*input)++;
        return new_token(TOKEN_DIVIDE, "/");
    }

    if (**input == '(') {
        (*input)++;
        return new_token(TOKEN_LPAREN, "(");
    }

    if (**input == ')') {
        (*input)++;
        return new_token(TOKEN_RPAREN, ")");
    }

    printf("Unknown character: %c\n", **input);
    exit(1);
}
