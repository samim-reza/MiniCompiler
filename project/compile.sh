flex lexer.l
gcc lex.yy.c parser.tab.c -o mylang -lfl
