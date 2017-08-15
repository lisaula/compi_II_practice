
%include {
  #include <stdio.h>
  #include "utils.h"
  #include "ast.h"
  #include <assert.h>
  extern int yylineno;

}

%parse_accept {
  printf("parsing complete!\n");
}
%syntax_error {
  printf("Syntax error at line %d \n", yylineno);
}



%token_type { Token* }
%default_type { int }

%type stmt { Statement* }
%type assign_st { Statement* }
%type print_st { Statement* }
%type stmts { Statement* }
%type embededd_statement { Statement* }
%type while_st { Statement* }
%type optional_else { Statement* }
%type local_variable_declaration { Statement* }
%type braket_block_stmts { Statement* }
%type if_st { Statement* }
%type function_call { Statement* }
%type function_declaration { Statement* }
%type fixed_parameters { Parameters* }
%type arguments { Arguments* }
%type expr { Expr* }
%type term { Expr* }
%type factor { Expr* }
%type conditional_expr { Expr* }
%type equality_expr { Expr* }
%type rational_expr { Expr* }

%start_symbol input

input::= eols_op stmts(B) eols_op . { B->exec(); }

eols_op ::= eols .
eols_op ::= .

eols ::= TK_EOL .

stmts(A) ::= stmts(B) eols stmt(C) . { A = B; ((BlockStatement *)A)->addStatement(C); }
stmts(A) ::= stmt(B) . { A = new BlockStatement; ((BlockStatement *)A)->addStatement(B); }

stmt(A) ::= local_variable_declaration(B) . { A = B; }
stmt(A) ::= embededd_statement(B) . { A = B; }

local_variable_declaration(A) ::= print_st(B) . { A = B; }
local_variable_declaration(A) ::= assign_st(B) . { A = B; }

embededd_statement(A) ::= if_st(B) . { A = B; }
embededd_statement(A) ::= braket_block_stmts(B) . { A = B; }
embededd_statement(A) ::= while_st(B) .  { A = B; }
embededd_statement(A) ::= function_declaration(B) .  { A = B; }
embededd_statement(A) ::= function_call(B) .  { A = B; }
embededd_statement(A) ::= jump_st(B) . { A=B; }

jump_st(A) ::= KW_RETURN expr(B) . { A = new ReturnStatement(B); } 

function_declaration(A) ::= KW_FUNC TK_ID(B) TK_LEFT_PAR fixed_parameters(C) TK_RIGHT_PAR eols braket_block_stmts(D) . { A = new FunctionDeclaration(B->char_value,C,D); }

function_call(A) ::= TK_ID(B) TK_LEFT_PAR arguments(C) TK_RIGHT_PAR . { A = new FunctionCall(B->char_value, C); }

fixed_parameters(A) ::= fixed_parameters(B) OP_COMMA TK_ID(C) . { A = B; ((Parameters*)A)->addParameter(C->char_value); }
fixed_parameters(A) ::= TK_ID(B) . { A = new Parameters(); ((Parameters*)A)->addParameter(B->char_value); }
fixed_parameters(A) ::= . { A = NULL; }

arguments(A) ::= arguments(B) OP_COMMA expr(C) . { A=B; ((Arguments*)A)->addArgument(C); }
arguments(A) ::= expr(B) . { A = new Arguments(); ((Arguments*)A)->addArgument(B); }
arguments(A) ::= . { A = NULL; }

while_st(A) ::= KW_WHILE TK_LEFT_PAR conditional_expr(B) TK_RIGHT_PAR eols stmt(C) . { A = new WhileStatement(B,C); }

if_st(A) ::= KW_IF TK_LEFT_PAR conditional_expr(B) TK_RIGHT_PAR eols local_variable_declaration(C) optional_else(D) . { A = new IfStatement(B,C,D); }
if_st(A) ::= KW_IF TK_LEFT_PAR conditional_expr(B) TK_RIGHT_PAR eols embededd_statement(C) optional_else(D) . { A = new IfStatement(B,C,D); }

optional_else(A) ::= KW_ELSE eols stmt(B) . { A = B; }
optional_else(A) ::= . { A = NULL; }

braket_block_stmts(A) ::= TK_OPEN_BRAKET eols_op stmts(B) eols_op TK_CLOSE_BRAKET . { A = B; }
braket_block_stmts(A) ::= TK_OPEN_BRAKET eols_op TK_CLOSE_BRAKET . { A = NULL; }

print_st(A) ::= KW_PRINT expr(B) . { A = new PrintStatement(B); }
print_st(A) ::= KW_PRINT expr(B) OP_COMMA KW_DEC . { A = new PrintStatement(B, DEC); }
print_st(A) ::= KW_PRINT expr(B) OP_COMMA KW_HEX . { A = new PrintStatement(B, HEX); }
print_st(A) ::= KW_PRINT expr(B) OP_COMMA KW_BIN . { A = new PrintStatement(B, BIN); }

assign_st(A) ::= TK_ID(B) OP_ASSIGN expr(C) . { A= new AssignStatement(B->char_value, C); free(B); }

conditional_expr(A) ::= equality_expr(B) . { A = B; }

equality_expr(A) ::= rational_expr(B) OP_EQUAL expr(C) . { A = new EqualExpr(B,C); }
equality_expr(A) ::= rational_expr(B) OP_NOT_EQUAL expr(C) . { A = new NotEqualExpr(B,C); }
equality_expr(A) ::= rational_expr(B) . { A=B; }

rational_expr(A) ::= expr(B) OP_GREATER_THAN expr(C) . { A = new GreaExpr(B, C); }
rational_expr(A) ::= expr(B) OP_LESS_THAN expr(C) . { A = new LessExpr(B, C); }
rational_expr(A) ::= expr(B) OP_GREATER_OR_EQUAL expr(C) . { A = new GreaEqExpr(B, C); }
rational_expr(A) ::= expr(B) OP_LESS_OR_EQUAL expr(C) . { A = new LessEqExpr(B, C); }
rational_expr(A) ::= expr(B) . { A=B; }

expr(A) ::= expr(B) OP_ADD term(C) . { A = new AddExpr(B, C); }
expr(A) ::= expr(B) OP_SUB term(C) . { A = new SubExpr(B, C); }
expr(A) ::= term(B) . { A = B; }

term(A) ::= term(B) OP_MUL factor(C) . { A = new MulExpr(B,C); }
term(A) ::= term(B) OP_DIV factor(C) . { A = new DivExpr(B,C); }
term(A) ::= factor(B) . { A=B; }

factor(A) ::= TK_NUMBER(B) . {A = new NumberExpr(B->int_value); free(B); }
factor(A) ::= TK_LEFT_PAR expr(B) TK_RIGHT_PAR . { A=B; }
factor(A) ::= TK_ID(B) . { A= new VarExpr(B->char_value); free(B); }
