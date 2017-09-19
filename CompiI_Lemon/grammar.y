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

stmts ::= stmts stmt .
stmts ::= stmt .

opt_stmts ::= stmts .
opt_stmts ::= .

stmt ::= st_options TK_EOS .
stmt ::= conditional_st .
stmt ::= loop_st .

st_options ::= assign_st .
st_options ::= decl_st .
st_options ::= print_st .


assign_st ::= TK_ID OP_ASSIGN expr .

decl_st ::= KW_DECL type_w_id TK_ID opt_assign_st .
decl_st ::= KW_DECL KW_STRUCT TK_ID TK_L_BRACE attr_list TK_R_BRACE .

type ::= KW_INT .
type ::= KW_BOOL .
type ::= KW_CHAR .
type ::= KW_DOUBLE .
type ::= KW_STRING .

type_w_id ::= type .
type_w_id ::= TK_ID .

opt_assign_st ::= OP_ASSIGN expr .
opt_assign_st ::=  .

attr_list ::= attr_list attr_decl .
attr_list ::= attr_decl .

attr_decl ::= KW_DECL type TK_ID TK_EOS.

conditional_st ::= if_expr .

if_expr ::= KW_IF TK_L_PAR bool_expr TK_R_PAR block_st opt_else .

opt_else ::= KW_ELSE block_st .
opt_else ::= .

block_st ::= TK_L_BRACE opt_stmts TK_R_BRACE .

bool_expr ::= expr compare_option expr .

compare_option ::= OP_GT .
compare_option ::= OP_GET .
compare_option ::= OP_LT .
compare_option ::= OP_LET .
compare_option ::= OP_EQ .
compare_option ::= OP_NEQ .

loop_st ::= for_st .
loop_st ::= while_st .

for_st ::= KW_FOR TK_L_PAR decl_st TK_EOS bool_expr TK_EOS assign_st TK_R_PAR block_st .

while_st ::= KW_WHILE TK_L_PAR bool_expr TK_R_PAR block_st .

print_st ::= KW_PRINT TK_L_PAR expr TK_R_PAR .

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
factor ::= TK_ID TK_DOT TK_ID .
factor ::= literal .
factor ::= TK_L_PAR expr TK_R_PAR .
factor ::= KW_TYPEOF TK_ID .
factor ::= KW_TYPEOF TK_ID TK_DOT TK_ID .

literal ::= LIT_INT .
literal ::= LIT_BOOL .
literal ::= LIT_CHAR .
literal ::= LIT_DOUBLE .
literal ::= LIT_STRING .