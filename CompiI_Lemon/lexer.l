%option noyywrap
%option yylineno

%{
    #include <iostream>
    #include <string>
    #include "tokens.h"
    using namespace std;

    string *token = new string("nada");
%}

%%

[ \t]                   {  }

"+"                     { return OP_PLUS; }
"-"                     { return OP_MINUS; }
"*"                     { return OP_MUL; }
"/"                     { return OP_DIV; }
"%"                     { return OP_MOD; }
"^"                     { return OP_POW; }

">"                     { return OP_GT; }
">="                    { return OP_GET; }
"<"                     { return OP_LT; }
"<="                    { return OP_LET; }
"="                     { return OP_ASSIGN; }
"=="                    { return OP_EQ; }
"!="                    { return OP_NEQ; }

";"                     { return TK_EOS; }
"."                     { return TK_DOT; }

"int"                   { return KW_INT; }
"bool"                  { return KW_BOOL; }
"char"                  { return KW_CHAR; }
"double"                { return KW_DOUBLE; }
"struct"                { return KW_STRUCT; }
"decl"                  { return KW_DECL; }
"string"                { return KW_STRING; }

"if"                    { return KW_IF; }
"else"                  { return KW_ELSE; }
"for"                   { return KW_FOR; }
"while"                 { return KW_WHILE; }
"print"                 { return KW_PRINT; }
"typeof"                { return KW_TYPEOF; }

"("                     { return TK_L_PAR; }
")"                     { return TK_R_PAR; }
"{"                     { return TK_L_BRACE; }
"}"                     { return TK_R_BRACE; }

[0-9]+                  { return LIT_INT; }
[0-9]*"."[0-9]+         { return LIT_DOUBLE; }
"'"."'"                 { return LIT_CHAR; }
["true""false"]         { return LIT_BOOL; }
\".*\"                  { return LIT_STRING; }


[_a-zA-Z]?[a-zA-z0-9_]+ { return TK_ID; }

\n                      { /*?*/ }
.                       { cout<<"Invalid character at "<<yylineno<<endl; }

%%