%include{
    // #include "ast.h"
    #include<stdio.h>
    #include <iostream>
    #include <string>
    #include<assert.h>

    using namespace std;

    extern int yylineno;
}

%parse_accept{
    cout<<"Done!"<<endl;
}

%syntax_error{
    cout<<"Error at: "<<yylineno<<endl;
}

%token_type { string * }
%default_type { int }

// %type stmts     { BlockStatement * }
// %type stmt      { Statement * }
// %type expr_st   { Statement * }

code ::= stmts .
code ::= sumidero .

stmts ::= stmts stmt .
stmts ::= stmt .

stmt ::= st_options TK_EOS .

st_options ::= assign_st .
st_options ::= decl_st .
// st_options ::= conditional_st .
// st_options ::= loop_st .
// st_options ::= print_st .

assign_st ::= TK_ID OP_ASSIGN expr .

decl_st ::= KW_DECL type TK_ID opt_assign_st .
decl_st ::= KW_DECL KW_STRUCT TK_ID TK_L_BRACE attr_list TK_R_BRACE .

type ::= KW_INT .
type ::= KW_BOOL .
type ::= KW_CHAR .
type ::= KW_DOUBLE .
type ::= TK_ID .

opt_assign_st ::= OP_ASSIGN expr .
opt_assign_st ::=  .

attr_list ::= attr_list attr_decl .
attr_list ::= attr_decl .

attr_decl ::= KW_DECL type TK_ID TK_EOS.

// conditional_st ::= if_expr .

// if_expr ::= 

// loop_st ::= 

// print_st ::= 

expr ::= expr OP_PLUS term .
expr ::= expr OP_MINUS term .
expr ::= term .

term ::= term OP_MUL ssj_term .
term ::= term OP_DIV ssj_term .
term ::= term OP_MOD ssj_term .
term ::= ssj_term .

ssj_term ::= ssj_term OP_POW factor .
ssj_term ::= factor .

factor ::= TK_ID .
factor ::= literal .
factor ::= TK_L_PAR expr TK_R_PAR .

literal ::= LIT_INT .
literal ::= LIT_BOOL .
literal ::= LIT_CHAR .
literal ::= LIT_DOUBLE .

sumidero ::= OP_GT OP_GET OP_LET OP_LT OP_NEQ OP_EQ TK_DOT KW_IF KW_ELSE KW_PRINT KW_FOR KW_WHILE KW_TYPEOF .