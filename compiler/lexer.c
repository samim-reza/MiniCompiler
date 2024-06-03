#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h> // Include <string.h> for strncpy and strdup

Token* getNextToken(char *source) {
    static int pos = 0;
    char current_char;

    while ((current_char = source[pos]) != '\0') {
        if (isspace(current_char)) {
            pos++;
            continue;
        }

        // Handle identifier and keywords
        if (isalpha(current_char) || current_char == '_') {
            int start_pos = pos;
            while (isalnum(source[pos]) || source[pos] == '_') {
                pos++;
            }
            int length = pos - start_pos;
            char *value = (char *)malloc(length + 1);
            strncpy(value, &source[start_pos], length); // Use strncpy
            value[length] = '\0';

            Token *token = (Token *)malloc(sizeof(Token));
            token->type = TOKEN_IDENTIFIER; // Simplified, treat everything as an identifier for now
            token->value = value;
            return token;
        }

        // Handle integers
        if (isdigit(current_char)) {
            int start_pos = pos;
            while (isdigit(source[pos])) {
                pos++;
            }
            int length = pos - start_pos;
            char *value = (char *)malloc(length + 1);
            strncpy(value, &source[start_pos], length); // Use strncpy
            value[length] = '\0';

            Token *token = (Token *)malloc(sizeof(Token));
            token->type = TOKEN_INT_LITERAL; // Use TOKEN_INT_LITERAL for integers
            token->value = value;
            return token;
        }

        // Handle other tokens
        switch (current_char) {
            case '+':
                pos++;
                return createToken(TOKEN_PLUS, "+");
            case '-':
                pos++;
                return createToken(TOKEN_MINUS, "-");
            case '*':
                pos++;
                return createToken(TOKEN_MULTIPLY, "*");
            case '/':
                pos++;
                return createToken(TOKEN_DIVIDE, "/");
            case '(':
                pos++;
                return createToken(TOKEN_LPAREN, "(");
            case ')':
                pos++;
                return createToken(TOKEN_RPAREN, ")");
            case '{':
                pos++;
                return createToken(TOKEN_LBRACE, "{");
            case '}':
                pos++;
                return createToken(TOKEN_RBRACE, "}");
            case ';':
                pos++;
                return createToken(TOKEN_SEMICOLON, ";");
            case '=':
                pos++;
                return createToken(TOKEN_ASSIGN, "=");
            default:
                printf("Error: Invalid character '%c'\n", current_char);
                exit(1);
        }
    }

    Token *token = (Token *)malloc(sizeof(Token));
    token->type = TOKEN_EOF; // Set type to TOKEN_EOF at the end of file
    token->value = strdup(""); // Use strdup
    return token;
}

Token* createToken(TokenType type, const char *value) {
    Token *token = (Token *)malloc(sizeof(Token));
    token->type = type;
    token->value = strdup(value); // Use strdup
    return token;
}
