// main.c
#include "compiler.h"

int main() {
    const char *input = "x = 3 +2;";
    Node *ast = parse_statement(&input);

    // Generate code
    generate_code(ast);

    return 0;
}
