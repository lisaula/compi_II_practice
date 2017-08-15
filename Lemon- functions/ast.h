#ifndef _AST_H
#define _AST_H

#include <list>
#include <stdio.h>
#include "utils.h"
#include <map>
#include<string>

using namespace std;

typedef struct Token{
  char * char_value;
  int int_value;
} token_t;


#define DEFINE_BINARY_EXPR(name) \
  class name##Expr: public BinaryExpr { \
  public: \
    name##Expr(Expr *expr1,Expr *expr2): BinaryExpr(expr1,expr2) {} \
    int eval(); \
  }



class Expr {
public:
  virtual int eval() = 0;
};

class BinaryExpr: public Expr {
protected:
  BinaryExpr(Expr *expr1,Expr *expr2){
    this->expr1 = expr1;
    this->expr2 = expr2;
  }
public:
  Expr *expr1, *expr2;
};

DEFINE_BINARY_EXPR(Add);
DEFINE_BINARY_EXPR(Sub);
DEFINE_BINARY_EXPR(Mul);
DEFINE_BINARY_EXPR(Div);
DEFINE_BINARY_EXPR(Equal);
DEFINE_BINARY_EXPR(NotEqual);
DEFINE_BINARY_EXPR(Grea);
DEFINE_BINARY_EXPR(Less);
DEFINE_BINARY_EXPR(GreaEq);
DEFINE_BINARY_EXPR(LessEq);

class NumberExpr:public Expr {
public:
  NumberExpr(int value) {
    this->value = value;
  }
  int eval(){
    return value;
  }
  int value;
};

class VarExpr:public Expr {
public:
  VarExpr(char* index){
    this->index = index;
  }

  int eval();
  char* index;
};

class Statement {
public:
  virtual void exec() = 0;
};

class AssignStatement: public Statement {
public:
  AssignStatement(char* varIndex, Expr* expr){
    this->varIndex = varIndex;
    this->expr = expr;
  }

  void exec();

  char* varIndex;
  Expr *expr;
};

class PrintStatement: public Statement {
public:
  PrintStatement(Expr *expr){
    this->expr = expr;
    this->type = DEC;
  }

  PrintStatement(Expr *expr, int type){
    this->expr = expr;
    this->type = type;
  }
  void exec();

  Expr *expr;
  int type;
};

class BlockStatement: public Statement {
public:
  BlockStatement(){}

  void addStatement(Statement *stmt) {
    statementList.push_back(stmt);
  }

  void exec();

  list<Statement*> statementList;
};

class IfStatement : public Statement {
public:
  IfStatement(Expr *condition_expr, Statement *body, Statement *else_st){
    this->condition_expr = condition_expr;
    this->body = body;
    this->else_st = else_st;
  }
  IfStatement(Expr *condition_expr, Statement *body){
    this->condition_expr = condition_expr;
    this->body = body;
    this->else_st = NULL;
  }
  void addElseStatement(Statement *else_st){
    this->else_st = else_st;
  }
  void exec();

  Expr *condition_expr;
  Statement *body;
  Statement *else_st;
};

class ElseStatement: public Statement {
public:
  ElseStatement(Statement *body){
    this->body = body;
  }

  void exec();
  Statement *body;

};

class WhileStatement: public Statement {
public:
  WhileStatement(Expr* expr,Statement *body){
    this->expr = expr;
    this->body = body;
  }

  void exec();
  Expr* expr;
  Statement *body;
};

class Parameters {
public:
  Parameters(){}
  list<string> params;

  void addParameter(char * param){
    params.push_back(param);
  }
};

class Arguments {
public:
  Arguments(){}

  list<Expr*> argus;
  void addArgument(Expr*  expr) {
    argus.push_back(expr);
  }
};

class FunctionDeclaration: public Statement {
public:
  FunctionDeclaration(char* name,Parameters* params, Statement *body){
    this->name = name;
    this->params = params;
    this->body =body;
  }
  void exec();
  char* name;
  Parameters* params;
  Statement *body;
};

class ReturnStatement: public Statement {
  ReturnStatement(Expr* expr){
    this->expr = expr;
  }

  Expr* expr;
}

class FunctionCall: public Statement {
public:
  FunctionCall(char* name,Arguments* arguments){
    this->name = name;
    this->arguments = arguments;
  }
  void exec();
  char* name;
  Arguments* arguments;
};



#endif
