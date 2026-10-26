%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"
%}

%%

"if"                        { return IF; }

"<="|">="|"=="|"!="|"<"|">" {
                                yylval.str = strdup(yytext);
                                return RELOP;
                            }

[0-9]+                      {
                                yylval.str = strdup(yytext);
                                return NUM;
                            }

[a-zA-Z_][a-zA-Z_0-9]*      {
                                yylval.str = strdup(yytext);
                                return ID;
                            }

[ \t\n]+                    { /* Skip whitespace */ }

.                           { return yytext[0]; }

%%

int yywrap(void)
{
    return 1;
}