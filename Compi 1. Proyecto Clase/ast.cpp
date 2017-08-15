#include "ast.h"

map<string, int> global_vars;

#define DEFINE_EVAL_EXPR(name, op) \
  int name##Expression::eval() { \
    int a = val1->eval(); \
    int b = val2->eval(); \
    return a op b; \
  }

DEFINE_EVAL_EXPR(Add, +);
DEFINE_EVAL_EXPR(Sub, -);
DEFINE_EVAL_EXPR(Div, /);
DEFINE_EVAL_EXPR(Mult, *);

int VarExpression::eval(){
  return global_vars.at(name);
}

void AssignStatement::exec(){
  map<string, int>::iterator it = global_vars
}
