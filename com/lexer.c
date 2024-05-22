#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"

Token getNextToken(const char** input) {
    Token token;
    token.type = TOKEN_END;
    int i = 0;

    while (**input && isspace(**input)) (*input)++;

    if (**input == '\0') {
        token.type = TOKEN_END;
        return token;
    }

    if (isalpha(**input)) {
        while (isalnum(**input)) {
            token.value[i++] = *(*input)++;
        }
        token.value[i] = '\0';
        if (strcmp(token.value, "print") == 0) {
            token.type = TOKEN_PRINT;
        } else {
            token.type = TOKEN_IDENTIFIER;
        }
    } else if (isdigit(**input)) {
        token.type = TOKEN_NUMBER;
        while (isdigit(**input)) {
            token.value[i++] = *(*input)++;
        }
        token.value[i] = '\0';
    } else if (**input == '=') {
        token.type = TOKEN_ASSIGN;
        token.value[i++] = *(*input)++;
    } else if (**input == ';') {
        token.type = TOKEN_SEMICOLON;
        token.value[i++] = *(*input)++;
    } else if (strchr("+-*/()", **input)) {
        token.type = TOKEN_OPERATOR;
        token.value[i++] = *(*input)++;
    } else {
        fprintf(stderr, "Error: Unknown token '%c'\n", **input);
        #include <stdlib.h>

        exit(EXIT_FAILURE);
    }

    token.value[i] = '\0';
    return token;
}
