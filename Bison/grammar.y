%code requires {
  #include<iostream>
  #include<stdlib.h>
  #include<string>

  using namespace std;
}

%{
  /*include area*/
  #include<stdio.h>
  extern int yylineno;
  int yylex();
  void yyerror(const char* msg) {
    printf("Line %d: %s\n",yylineno,msg);
  }

%}

%union {
  string* string_t;
}

%token<string_t> LIT_INT LIT_LETTER
%token TK_EOF TK_ERROR
%token OP_PIPE "+"
%token OP_KLEENE "*"
%token OP_CONCAT "."
%token TK_LEFT_PAR "("
%token TK_RIGHT_PAR ")"
%type<string_t> E T F R Kleene_optional

%%

input: E { printf("reverse: %s\n", $1->c_str()); delete $1; }
;

E: E "+" T { $$ = new string(*$1 + "+" + *$3); delete $1;delete $3;}
 | T { $$ = $1; }
;

T: T "." F { $$ = new string( *$3 + "." + *$1); delete $3;delete $1;}
 | F { $$ = $1; }
;

F: "(" E ")" Kleene_optional { $$ = new string("("+ *$2 +")"+ *$4); delete $2;delete $4; }
  | R Kleene_optional { $$ = new string(*$1 + *$2); delete $1;delete $2; }
;

Kleene_optional: "*" { $$ = new string("*"); }
               | { $$ = new string(); }
;

R: LIT_INT { $$ = $1; }
 | LIT_LETTER { $$ = $1; }
;
