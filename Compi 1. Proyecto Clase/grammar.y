%{
    #include <stdio.h>

    extern int yylineno;
    int yylex();

    void yyerror(const char * msg) {
      printf("ERROR at line:%d %s\n", yylineno, msg);
    }
%}

%token TK_EOL
%token TK_ASSIGN
%token TK_ACCESSOR
%token OP_ADD "+"
%token OP_SUB "-"
%token OP_MULT "*"
%token OP_DIV "/"
%token TK_OPEN_PAR
%token TK_CLOSE_PAR
%token KW_PRINT
%token KW_IF
%token KW_STRUCT
%token KW_END
%token TK_LIT_NUM
%token TK_ID
%token TK_ERROR

%%
code: statement_list
;

statement_list: statement_list statement
  | statement
;

statement: assign_statement

E: E "+" T
  | E "-" T
  | T
;

T: T "*" F
  | T "/" F
  | F
;

F: TK_LIT_NUM
;
