#include "codegen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

void generateCode(char *source) {
    Token *token = getNextToken(source);

    // Generate the C code for the main function
    printf("#include <stdio.h>\n\n");
    printf("int main() {\n");

    // Generate code for the body of the main function
    while (token->type != TOKEN_RBRACE) {
        if (token->type == TOKEN_INT) {
            // For simplicity, assume declarations are always of the form "int <variable_name>;"
            token = getNextToken(source); // Move past the 'int' token
            printf("    int %s;\n", token->value); // Print the variable declaration
        }
        token = getNextToken(source);
    }

    // Generate code for printing the sum of two variables
    printf("\n"); // Add a newline for readability
    printf("    // Printing the sum of two variables\n");
    printf("    printf(\"Sum: %d\\n\", a + b);\n");

    // End of the main function
    printf("    return 0;\n");
    printf("}\n");

    // Free the memory allocated for tokens
    free(token->value);
    free(token);
}
