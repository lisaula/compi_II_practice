%{
    #include <stdio.h>

    extern int yylineno;
    int yylex();

    void yyerror(const char * msg) {
      printf("ERROR at line:%d %s\n", yylineno, msg);
    }

    #define YYERROR_VERBOSE 1
%}

%union {
  int int_t;
  char char_t;
  bool bool_t;
  char *text;
}

%token TK_EOS ";"
%token TK_ASSIGN "="
%token TK_ACCESSOR
%token OP_ADD "+"
%token OP_SUB "-"
%token OP_MULT "*"
%token OP_DIV "/"
%token TK_OPEN_PAR
%token TK_CLOSE_PAR
%token KW_BOOL
%token TK_LIT_BOOL
%token KW_CHAR
%token KW_INT
%token KW_PRINT
%token KW_READ
%token KW_DECLARE
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
  | print_statement
  | struct_declaration
  | variable_declaration
  | read_statement
;

assign_statement: TK_ID "=" E ";"
;

print_statement: KW_PRINT E ";"
;

struct_declaration: KW_STRUCT TK_ID struct_attribute_list KW_END
;

struct_attribute_list: struct_attribute_list variable_declaration
  | variable_declaration
;

variable_declaration: KW_DECLARE type TK_ID ";"
;

type: KW_CHAR
  | KW_BOOL
  | KW_INT
;

print_statement: KW_PRINT TK_ID ";"
;

read_statement: KW_READ TK_ID ";"
;

E: E "+" T
  | E "-" T
  | T
;

T: T "*" F
  | T "/" F
  | F
;

F: TK_LIT_NUM
 | TK_LIT_BOOL
;
