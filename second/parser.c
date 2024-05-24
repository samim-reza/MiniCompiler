// parser.c
#include "compiler.h"

// Function to create a new integer node
Node *new_int_node(int value) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NODE_INT;
    node->value = value;
    return node;
}

// Function to create a new variable node
Node *new_var_node(const char *name) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NODE_VAR;
    node->name = strdup(name);
    return node;
}

// Function to create a new binary operation node
Node *new_binop_node(NodeType type, Node *left, Node *right) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = type;
    node->binop.left = left;
    node->binop.right = right;
    return node;
}

// Function to create a new assignment node
Node *new_assign_node(Node *var, Node *expr) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = NODE_ASSIGN;
    node->assign.left = var;
    node->assign.right = expr;
    return node;
}

// Match and consume the current token
void match(TokenType type, const char **input, Token **current_token) {
    if ((*current_token)->type != type) {
        printf("Syntax error: expected %d, got %d\n", type, (*current_token)->type);
        exit(1);
    }
    next_token(input, current_token);
}

// Get the next token and update the current token
void next_token(const char **input, Token **current_token) {
    free((*current_token)->value);
    free(*current_token);
    *current_token = get_next_token(input);
}

// Parse primary expressions (integer literals, variables, and parentheses)
Node *parse_primary(const char **input, Token **current_token) {
    if ((*current_token)->type == TOKEN_INT) {
        Node *node = new_int_node(atoi((*current_token)->value));
        next_token(input, current_token);
        return node;
    }
    if ((*current_token)->type == TOKEN_IDENTIFIER) {
        Node *node = new_var_node((*current_token)->value);
        next_token(input, current_token);
        return node;
    }
    if ((*current_token)->type == TOKEN_LPAREN) {
        next_token(input, current_token);
        Node *node = parse_expression(input, current_token);
        match(TOKEN_RPAREN, input, current_token);
        return node;
    }
    printf("Syntax error: unexpected token %d\n", (*current_token)->type);
    exit(1);
}

// Parse multiplication and division
Node *parse_term(const char **input, Token **current_token) {
    Node *node = parse_primary(input, current_token);
    while ((*current_token)->type == TOKEN_MULTIPLY || (*current_token)->type == TOKEN_DIVIDE) {
        TokenType op = (*current_token)->type;
        next_token(input, current_token);
        Node *right = parse_primary(input, current_token);
        node = new_binop_node(op == TOKEN_MULTIPLY ? NODE_MULTIPLY : NODE_DIVIDE, node, right);
    }
    return node;
}

// Parse addition and subtraction
Node *parse_expression(const char **input, Token **current_token) {
    Node *node = parse_term(input, current_token);
    while ((*current_token)->type == TOKEN_PLUS || (*current_token)->type == TOKEN_MINUS) {
        TokenType op = (*current_token)->type;
        next_token(input, current_token);
        Node *right = parse_term(input, current_token);
        node = new_binop_node(op == TOKEN_PLUS ? NODE_PLUS : NODE_MINUS, node, right);
    }
    return node;
}

// Parse statements (currently only assignments)
Node *parse_statement(const char **input) {
    Token *current_token = get_next_token(input);
    Node *var = parse_primary(input, &current_token);
    match(TOKEN_ASSIGN, input, &current_token);
    Node *expr = parse_expression(input, &current_token);
    match(TOKEN_SEMICOLON, input, &current_token);
    return new_assign_node(var, expr);
}
