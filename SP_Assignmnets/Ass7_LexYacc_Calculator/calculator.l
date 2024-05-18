%{
#include "y.tab.h"
%}

%%

[ \t]       ; // Skip whitespace
[0-9]+      { yylval = atoi(yytext); return NUMBER; }
.           { return yytext[0]; }
\n          { return '\n'; }

%%

