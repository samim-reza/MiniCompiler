#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Token types
#define TOK_PRINT 0
#define TOK_ID 1
#define TOK_VAR 2
#define TOK_INT 3
#define TOK_FLOAT 4
#define TOK_TYPE 5
#define TOK_EQ 6
#define TOK_PLUS 7
#define TOK_MINUS 8
#define TOK_STAR 9
#define TOK_SLASH 10
#define TOK_LPAREN 11
#define TOK_RPAREN 12
#define TOK_COLON 13
#define TOK_WHILE 14
#define TOK_DO 15
#define TOK_DONE 16
#define TOK_SEMI 17
#define TOK_READ 18
#define TOK_GT 19
#define TOK_LT 20
#define TOK_GTE 21
#define TOK_LTE 22
#define TOK_EQEQ 23

// AST nodes
#define AST_DECL 0
#define AST_ASSIGN 1
#define AST_PRINT 2
#define AST_INT 3
#define AST_FLOAT 4
#define AST_ID 5
#define AST_BINOP 6
#define AST_WHILE 7
#define AST_READ 8

typedef struct
{
    int toktype;
    char *value;
} Token;

typedef struct ASTNode
{
    int nodetype;
    char *id;
    char *type;
    char *name;
    char *op;
    struct ASTNode *lhs;
    struct ASTNode *rhs;
    struct ASTNode *expr;
    struct ASTNode **body;
    int body_len;
    union
    {
        int int_value;
        float float_value;
    };
} ASTNode;

void error(const char *msg)
{
    fprintf(stderr, "Error: %s\n", msg);
    exit(1);
}

Token *tok(int ty, const char *val)
{
    Token *t = malloc(sizeof(Token));
    t->toktype = ty;
    t->value = val ? strdup(val) : NULL;
    return t;
}

ASTNode *astnode(int nodetype)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->nodetype = nodetype;
    node->id = NULL;
    node->type = NULL;
    node->name = NULL;
    node->op = NULL;
    node->lhs = NULL;
    node->rhs = NULL;
    node->expr = NULL;
    node->body = NULL;
    node->body_len = 0;
    return node;
}

void lex(const char *s, Token ***tokens, int *tokens_len)
{
    int i = 0;
    int len = strlen(s);
    *tokens = NULL;
    *tokens_len = 0;

    while (i < len)
    {
        char c = s[i];

        if (isspace(c))
        {
            // Skip spaces
        }
        else if (c == '#')
        {
            while (s[i] != '\n' && i < len)
                i++;
        }
        else if (c == '=')
        {
            if (s[i + 1] == '=')
            {
                *tokens = realloc(*tokens, sizeof(Token *) * (*tokens_len + 1));
                (*tokens)[*tokens_len] = tok(TOK_EQEQ, "==");
                (*tokens_len)++;
                i++;
            }
            else
            {
                *tokens = realloc(*tokens, sizeof(Token *) * (*tokens_len + 1));
                (*tokens)[*tokens_len] = tok(TOK_EQ, "=");
                (*tokens_len)++;
            }
        }
        else if (c == '+')
        {
            *tokens = realloc(*tokens, sizeof(Token *) * (*tokens_len + 1));
            (*tokens)[*tokens_len] = tok(TOK_PLUS, "+");
            (*tokens_len)++;
        }
        else if (c == '-')
        {
            *tokens = realloc(*tokens, sizeof(Token *) * (*tokens_len + 1));
            (*tokens)[*tokens_len] = tok(TOK_MINUS, "-");
            (*tokens_len)++;
        }
        else if (c == '*')
        {
            *tokens = realloc(*tokens, sizeof(Token *) * (*tokens_len + 1));
            (*tokens)[*tokens_len] = tok(TOK_STAR, "*");
            (*tokens_len)++;
        }
        else if (c == '/')
        {
            *tokens = realloc(*tokens, sizeof(Token *) * (*tokens_len + 1));
            (*tokens)[*tokens_len] = tok(TOK_SLASH, "/");
            (*tokens_len)++;
        }
        else if (c == '(')
        {
            *tokens = realloc(*tokens, sizeof(Token *) * (*tokens_len + 1));
            (*tokens)[*tokens_len] = tok(TOK_LPAREN, "(");
            (*tokens_len)++;
        }
        else if (c == ')')
        {
            *tokens = realloc(*tokens, sizeof(Token *) * (*tokens_len + 1));
            (*tokens)[*tokens_len] = tok(TOK_RPAREN, ")");
            (*tokens_len)++;
        }
        else if (c == ':')
        {
            *tokens = realloc(*tokens, sizeof(Token *) * (*tokens_len + 1));
            (*tokens)[*tokens_len] = tok(TOK_COLON, ":");
            (*tokens_len)++;
        }
        else if (c == ';')
        {
            *tokens = realloc(*tokens, sizeof(Token *) * (*tokens_len + 1));
            (*tokens)[*tokens_len] = tok(TOK_SEMI, ";");
            (*tokens_len)++;
        }
        else if (c == '>')
        {
            if (s[i + 1] == '=')
            {
                *tokens = realloc(*tokens, sizeof(Token *) * (*tokens_len + 1));
                (*tokens)[*tokens_len] = tok(TOK_GTE, ">=");
                (*tokens_len)++;
                i++;
            }
            else
            {
                *tokens = realloc(*tokens, sizeof(Token *) * (*tokens_len + 1));
                (*tokens)[*tokens_len] = tok(TOK_GT, ">");
                (*tokens_len)++;
            }
        }
        else if (c == '<')
        {
            if (s[i + 1] == '=')
            {
                *tokens = realloc(*tokens, sizeof(Token *) * (*tokens_len + 1));
                (*tokens)[*tokens_len] = tok(TOK_LTE, "<=");
                (*tokens_len)++;
                i++;
            }
            else
            {
                *tokens = realloc(*tokens, sizeof(Token *) * (*tokens_len + 1));
                (*tokens)[*tokens_len] = tok(TOK_LT, "<");
                (*tokens_len)++;
            }
        }
        else if (isdigit(c))
        {
            char num[32];
            int j = 0;
            while (isdigit(s[i]) && i < len)
            {
                num[j++] = s[i++];
            }
            if (s[i] == '.')
            {
                num[j++] = s[i++];
                while (isdigit(s[i]) && i < len)
                {
                    num[j++] = s[i++];
                }
                num[j] = '\0';
                *tokens = realloc(*tokens, sizeof(Token *) * (*tokens_len + 1));
                (*tokens)[*tokens_len] = tok(TOK_FLOAT, num);
                (*tokens_len)++;
            }
            else
            {
                num[j] = '\0';
                *tokens = realloc(*tokens, sizeof(Token *) * (*tokens_len + 1));
                (*tokens)[*tokens_len] = tok(TOK_INT, num);
                (*tokens_len)++;
            }
            i--;
        }
        else if (isalpha(c) || c == '_')
        {
            char ident[32];
            int j = 0;
            while ((isalnum(s[i]) || s[i] == '_') && i < len)
            {
                ident[j++] = s[i++];
            }
            ident[j] = '\0';
            i--;
            if (strcmp(ident, "print") == 0)
            {
                *tokens = realloc(*tokens, sizeof(Token *) * (*tokens_len + 1));
                (*tokens)[*tokens_len] = tok(TOK_PRINT, "print");
                (*tokens_len)++;
            }
            else if (strcmp(ident, "read") == 0)
            {
                *tokens = realloc(*tokens, sizeof(Token *) * (*tokens_len + 1));
                (*tokens)[*tokens_len] = tok(TOK_READ, "read");
                (*tokens_len)++;
            }
            else if (strcmp(ident, "var") == 0)
            {
                *tokens = realloc(*tokens, sizeof(Token *) * (*tokens_len + 1));
                (*tokens)[*tokens_len] = tok(TOK_VAR, "var");
                (*tokens_len)++;
            }
            else if (strcmp(ident, "while") == 0)
            {
                *tokens = realloc(*tokens, sizeof(Token *) * (*tokens_len + 1));
                (*tokens)[*tokens_len] = tok(TOK_WHILE, "while");
                (*tokens_len)++;
            }
            else if (strcmp(ident, "do") == 0)
            {
                *tokens = realloc(*tokens, sizeof(Token *) * (*tokens_len + 1));
                (*tokens)[*tokens_len] = tok(TOK_DO, "do");
                (*tokens_len)++;
            }
            else if (strcmp(ident, "done") == 0)
            {
                *tokens = realloc(*tokens, sizeof(Token *) * (*tokens_len + 1));
                (*tokens)[*tokens_len] = tok(TOK_DONE, "done");
                (*tokens_len)++;
            }
            else if (strcmp(ident, "int") == 0 || strcmp(ident, "float") == 0)
            {
                *tokens = realloc(*tokens, sizeof(Token *) * (*tokens_len + 1));
                (*tokens)[*tokens_len] = tok(TOK_TYPE, ident);
                (*tokens_len)++;
            }
            else
            {
                *tokens = realloc(*tokens, sizeof(Token *) * (*tokens_len + 1));
                (*tokens)[*tokens_len] = tok(TOK_ID, ident);
                (*tokens_len)++;
            }
        }
        else
        {
            char err[2] = {c, '\0'};
            error(err);
        }
        i++;
    }
}

int main(void)
{
    char *src = NULL;
    size_t len = 0;
    size_t read;
    char *buffer = NULL;

    printf("Reading input:\n");
    while ((read = getline(&buffer, &len, stdin)) != -1)
    {
        src = realloc(src, (src ? strlen(src) : 0) + read + 1);
        strcat(src, buffer);
        printf("%s", buffer); // Debugging line
    }

    Token **tokens;
    int tokens_len;
    lex(src, &tokens, &tokens_len);

    for (int i = 0; i < tokens_len; i++)
    {
        printf("Token: %d, Value: %s\n", tokens[i]->toktype, tokens[i]->value ? tokens[i]->value : "NULL");
        free(tokens[i]->value);
        free(tokens[i]);
    }
    free(tokens);
    free(src);
    free(buffer);

    return 0;
}
