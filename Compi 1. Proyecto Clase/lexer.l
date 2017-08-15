%option yylineno
%option noyywrap

%{
  #include "tokens.h"
%}


%%
[ \t\n] { /*do nothing*/ }
";" { return TK_EOS; }
"=" { return TK_ASSIGN; }
"." { return TK_ACCESSOR; }
"+" { return OP_ADD; }
"-" { return OP_SUB; }
"*" { return OP_MULT; }
"/" { return OP_DIV; }
"(" { return TK_OPEN_PAR; }
")" { return TK_CLOSE_PAR; }
"declare" { return KW_DECLARE; }
"int" { return KW_INT; }
"bool" { return KW_BOOL; }
"char" { return KW_CHAR; }
"print" { return KW_PRINT; }
"read" { return KW_READ; }
"if" {return KW_IF; }
"struct" { return KW_STRUCT; }
"true" { return TK_LIT_BOOL; }
"false" { return TK_LIT_BOOL; }
"end" { return KW_END; }
[0-9] { return TK_LIT_NUM; }
[A-Za-z0-9]* { return TK_ID; }
. { return TK_ERROR; }
%%
