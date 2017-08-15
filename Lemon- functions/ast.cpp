#include "ast.h"
#include "utils.cpp"
#include <string>
#define IMPLEMENT_BINARY_EXPR_EVAL(name, op) \
  int name##Expr::eval() { \
    int a = expr1->eval(); \
    int b = expr2->eval(); \
    return a op b; \
  }

map<string, int>vars;


IMPLEMENT_BINARY_EXPR_EVAL(Add, +);
IMPLEMENT_BINARY_EXPR_EVAL(Sub, -);
IMPLEMENT_BINARY_EXPR_EVAL(Mul, *);
IMPLEMENT_BINARY_EXPR_EVAL(Div, /);
IMPLEMENT_BINARY_EXPR_EVAL(Equal, ==);
IMPLEMENT_BINARY_EXPR_EVAL(NotEqual, !=);
IMPLEMENT_BINARY_EXPR_EVAL(Grea, >);
IMPLEMENT_BINARY_EXPR_EVAL(Less, <);
IMPLEMENT_BINARY_EXPR_EVAL(GreaEq, >=);
IMPLEMENT_BINARY_EXPR_EVAL(LessEq, <=);


int VarExpr::eval(){
  //printf("Eval index:|%s|\n",index);
  return vars.at(index);
}

void AssignStatement::exec(){
  //printf("AssignStatement |%s|\n",varIndex );
  map<string, int>::iterator it = vars.find(varIndex);
  if(it != vars.end()){
    vars[varIndex] = expr->eval();
    //printf("entro if\n" );
  }else{
    vars.insert(pair<string, int>(varIndex, expr->eval()));
    //printf("entro else\n" );
  }
  //printf("Salio\n" );
}
void PrintStatement::exec(){
  int value = expr->eval();
  if(type == DEC)
    printf("Decimal %d\n", value);
  else if(type == HEX)
    printf("Hexadecimal: %x\n", value);
  else
    utility::print_int2bin(value);
}

void BlockStatement::exec(){
  for(list<Statement*>::iterator it = statementList.begin(); it != statementList.end(); it++){
    //printf("block\n");
    (*it)->exec();
  }
  //printf("Salio\n");
}

void ElseStatement::exec(){
  body->exec();
}

void IfStatement::exec(){
  if(condition_expr->eval())
    body->exec();
  else{
    if(else_st)
      else_st->exec();
  }
}

void WhileStatement::exec(){
  while(expr->eval())
    body->exec();
}

void FunctionCall::exec(){

}

void FunctionDeclaration::exec(){
  
}
