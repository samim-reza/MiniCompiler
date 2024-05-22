#ifndef LEXER_H
#define LEXER_H

#define MAX_TOKEN_LENGTH 100

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_ASSIGN,
    TOKEN_PRINT,
    TOKEN_SEMICOLON,
    TOKEN_END
} TokenType;

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_LENGTH];
} Token;

Token getNextToken(const char** input);

#endif // LEXER_H
