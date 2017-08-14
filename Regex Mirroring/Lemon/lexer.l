%option noyywrap
%option yylineno

%{
  /*Includes area*/
#include "tokens.h"
#include <iostream>
using namespace std;
string* token;
%}

%%
[0-9] { token = new string(yytext) ; return LIT_INT; }
[A-Za-z] {  token = new string(yytext) ; return LIT_LETTER; }
\n {  }
"+" { return OP_PIPE; }
"*" { return OP_KLEENE; }
"." { return OP_CONCAT; }
"(" { return TK_LEFT_PAR; }
")" { return TK_RIGHT_PAR; }

%%
