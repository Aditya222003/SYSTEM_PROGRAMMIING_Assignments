%{
#include <stdio.h>
#include <stdlib.h>
extern int yylex(void);
extern int yyerror(char *s);
%}

%token NUMBER

%%

input: /* empty */
     | input line
     ;

line: '\n'
    | exp '\n' { printf("Result: %d\n", $1); }
    ;

exp: exp '+' exp  { $$ = $1 + $3; }
   | exp '-' exp  { $$ = $1 - $3; }
   | exp '*' exp  { $$ = $1 * $3; }
   | exp '/' exp  { $$ = $1 / $3; }
   | '(' exp ')'  { $$ = $2; }
   | NUMBER       { $$ = $1; }
   ;

%%

int main() {
    yyparse();
    return 0;
}

int yyerror(char *s) {
    fprintf(stderr, "Error: %s\n", s);
    return 0;
}
