%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s);
int yylex(void);
void print_value(int value);

typedef union {
    int ival;
    char *sval;
} YYSTYPE;

#define YYSTYPE_IS_DECLARED 1
%}

%token NUMBER IDENTIFIER PRINT
%left '+' '-'
%left '*' '/'

%%
program:
    program statement
    | statement
    ;

statement:
    assignment ';'
    | expression ';'      { print_value($1.ival); }
    | PRINT expression ';' { printf("%d\n", $2.ival); }
    ;

assignment:
    IDENTIFIER '=' expression { printf("Assigning %d to %s\n", $3.ival, $1.sval); free($1.sval); }
    ;

expression:
    term
    | expression '+' term { /* semantic action for addition */ }
    | expression '-' term { /* semantic action for subtraction */ }
    ;

term:
    factor
    | term '*' factor { /* semantic action for multiplication */ }
    | term '/' factor { /* semantic action for division */ }
    ;

factor:
    NUMBER              { /* semantic action for number */ }
    | IDENTIFIER        { /* semantic action for identifier */ }
    | '(' expression ')' { /* semantic action for expression within parentheses */ }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

void print_value(int value) {
    printf("Value: %d\n", value);
}

int main(void) {
    yyparse();
    return 0;
}
