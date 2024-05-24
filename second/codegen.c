// codegen.c
#include "compiler.h"

void generate_code(Node *node) {
    if (node->type == NODE_ASSIGN) {
        generate_code(node->assign.right);
        printf("STORE %s\n", node->assign.left->name);
    } else if (node->type == NODE_INT) {
        printf("PUSH %d\n", node->value);
    } else if (node->type == NODE_VAR) {
        printf("LOAD %s\n", node->name);
    } else if (node->type == NODE_PLUS) {
        generate_code(node->binop.left);
        generate_code(node->binop.right);
        printf("ADD\n");
    } else if (node->type == NODE_MINUS) {
        generate_code(node->binop.left);
        generate_code(node->binop.right);
        printf("SUB\n");
    } else if (node->type == NODE_MULTIPLY) {
        generate_code(node->binop.left);
        generate_code(node->binop.right);
        printf("MUL\n");
    } else if (node->type == NODE_DIVIDE) {
        generate_code(node->binop.left);
        generate_code(node->binop.right);
        printf("DIV\n");
    } else {
        printf("Unknown node type: %d\n", node->type);
    }
}
