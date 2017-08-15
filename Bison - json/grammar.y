
%{
  #include <stdio.h>

  int yylex();
  extern int yylineno;
  extern char* current_filename;
  void yyerror(const char * msg){
    printf("%s: %s at %d\n",current_filename, msg, yylineno);
  }
  #define YYERROR_VERBOSE 1

%}

%token TK_ID
%token TK_NUMBER
%token TK_LEFT_BRACE
%token TK_RIGHT_BRACE
%token TK_LEFT_BRACKET
%token TK_RIGHT_BRACKET
%token OP_COMMA
%token OP_COLON
%token TK_ERROR

%%
input : TK_LEFT_BRACE options_list TK_RIGHT_BRACE
;

options_list: options_list OP_COMMA options
  | options
;

options: TK_ID OP_COLON TK_ID
  | TK_ID OP_COLON TK_NUMBER
;
