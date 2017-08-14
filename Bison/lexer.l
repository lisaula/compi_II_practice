%option noyywrap
%option yylineno

%{
  /*Includes area*/
#include "tokens.h"
#include <iostream>
using namespace std;

%}

%%
[0-9] { yylval.string_t =  new string(yytext) ;return LIT_INT; }
[A-Za-z] {  yylval.string_t =  new string(yytext) ;return LIT_LETTER; }
\n {  }
"+" { return OP_PIPE; }
"*" { return OP_KLEENE; }
"." { return OP_CONCAT; }
"(" { return TK_LEFT_PAR; }
")" { return TK_RIGHT_PAR; }
.   { return TK_ERROR; }

%%
