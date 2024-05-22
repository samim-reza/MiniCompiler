#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"

void readInput(char* buffer, size_t size) {
    printf(">> ");
    if (fgets(buffer, size, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        exit(EXIT_FAILURE);
    }
    // Remove newline character
    buffer[strcspn(buffer, "\n")] = 0;
}

int main() {
    const size_t BUFFER_SIZE = 256;
    char input[BUFFER_SIZE];

    while (1) {
        readInput(input, BUFFER_SIZE);

        if (strcmp(input, "exit") == 0) {
            break;
        }

        const char* sourceCode = input;

        while (*sourceCode) {
            Node* statement = parseStatement(&sourceCode);
            evaluate(statement);
            freeParseTree(statement);
        }
    }

    return 0;
}
