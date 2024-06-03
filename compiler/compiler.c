#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    // Read input file
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open input file '%s'\n", argv[1]);
        return 1;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    // Allocate memory to store file contents
    char *source = (char *)malloc(file_size + 1);
    if (source == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(file);
        return 1;
    }

    // Read file contents into source buffer
    fread(source, file_size, 1, file);
    source[file_size] = '\0'; // Null-terminate the string

    // Close the file
    fclose(file);

    // Lexical analysis
    Token *token;
    while ((token = getNextToken(source))->type != TOKEN_EOF) { // Adjusted to use TOKEN_EOF
        printf("Token type: %d, value: %s\n", token->type, token->value);
        free(token->value);
        free(token);
    }

    // Parsing
    parse(source); // Pass source to the parse function

    // Code generation
    generateCode(source); // Pass source to the code generation function

    // Free memory
    free(source);

    return 0;
}
