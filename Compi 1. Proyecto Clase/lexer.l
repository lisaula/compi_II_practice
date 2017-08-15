%option yylineno
%option noyywrap

%{
  #include "tokens.h"
%}


%%
[ \t\n] { /*do nothing*/ }
";" { return TK_EOL; }
"=" { return TK_ASSIGN; }
"." { return TK_ACCESSOR; }
"+" { return OP_ADD; }
"-" { return OP_SUB; }
"*" { return OP_MULT; }
"/" { return OP_DIV; }
"(" { return TK_OPEN_PAR; }
")" { return TK_CLOSE_PAR; }
"print" { return KW_PRINT; }
"if" {return KW_IF; }
"struct" { return KW_STRUCT; }
"end" { return KW_END; }
[0-9] { return TK_LIT_NUM; }
[A-Za-z] { return TK_ID; }
. { return TK_ERROR; }
%%
