%{
#include "parser.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
%}

%%

[ \t\n]+              { /* ignore whitespace */ }
[0-9]+                { yylval.ival = atoi(yytext); return NUMBER; }
[+*/-]                { return *yytext; }
"="                   { return '='; }
";"                   { return ';'; }
[a-zA-Z][a-zA-Z0-9]* { yylval.sval = strdup(yytext); return IDENTIFIER; }
"print"               { return PRINT; }
.                     { printf("Unknown character: %s\n", yytext); }

%%

int yywrap() {
    return 1;
}
