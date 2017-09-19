#ifndef _AST_H
#define _AST_H

#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <map>
#include <stack>
#include <set>
#include <stdexcept>
using namespace std;

#define DEC 2
#define BIN 0
#define HEX 1

// typedef struct variable_info{
//   string identifier;
//   int value;
// } variable_info_t;

typedef struct mips_data{
  string code;
  string place;
} mips_data_t;

extern set<string> global_vars;
extern stack<string> temps;
extern unsigned int next_label;
extern unsigned int next_var;
extern map<string, int> variables;

void initialize_temps();
void generate_header();
void generate_tail();

class Token
{
public:
  Token()
  {
    this->str_value = new string();
  }
  Token(string *str, int ent)
  {
    this->str_value = str;
    this->int_value = ent;
  }
  string *str_value;
  int int_value;
};

class Expr
{
public:
  virtual int eval() = 0;
  virtual void generate_code(mips_data_t *ret) = 0;
};

class BinaryExpr : public Expr
{
protected:
  BinaryExpr(Expr *expr1, Expr *expr2)
  {
    this->expr1 = expr1;
    this->expr2 = expr2;
  }

public:
  Expr *expr1, *expr2;
};

class FnCallExpr : public Expr
{
public:
  FnCallExpr(string *fn_name, list<Expr *> *args) : Expr(){
    this->fn_name = *fn_name;
    this->args = args;
  }
  string fn_name;
  list<Expr *> *args;
  int eval();
  void generate_code(mips_data_t *ret);
  // ~FnCallExpr();
  
};

class AddExpr : public BinaryExpr
{
public:
  AddExpr(Expr *expr1, Expr *expr2) : BinaryExpr(expr1, expr2) {}
  int eval();
  void generate_code(mips_data_t *ret);
};

class SubExpr : public BinaryExpr
{
public:
  SubExpr(Expr *expr1, Expr *expr2) : BinaryExpr(expr1, expr2) {}
  int eval();
  void generate_code(mips_data_t *ret);
};

class MulExpr : public BinaryExpr
{
public:
  MulExpr(Expr *expr1, Expr *expr2) : BinaryExpr(expr1, expr2) {}
  int eval();
  void generate_code(mips_data_t *ret);
};

class DivExpr : public BinaryExpr
{
public:
  DivExpr(Expr *expr1, Expr *expr2) : BinaryExpr(expr1, expr2) {}
  int eval();
  void generate_code(mips_data_t *ret);
};

class EqualRelationalExpr : public BinaryExpr
{
public:
  EqualRelationalExpr() : BinaryExpr(NULL, NULL) {}
  EqualRelationalExpr(Expr *epxr1, Expr *expr2) : BinaryExpr(expr1, expr2) {}

  int eval();
  void generate_code(mips_data_t *ret);
};

class NotEqualRelationalExpr : public BinaryExpr
{
public:
  NotEqualRelationalExpr() : BinaryExpr(NULL, NULL) {}
  NotEqualRelationalExpr(Expr *epxr1, Expr *expr2) : BinaryExpr(expr1, expr2) {}

  int eval();
  void generate_code(mips_data_t *ret);
};

class GreaterThanRelationalExpr : public BinaryExpr
{
public:
  GreaterThanRelationalExpr() : BinaryExpr(NULL, NULL) {}
  GreaterThanRelationalExpr(Expr *epxr1, Expr *expr2) : BinaryExpr(expr1, expr2) {}

  int eval();
  void generate_code(mips_data_t *ret);
};

class GreaterOrEqualThanRelationalExpr : public BinaryExpr
{
public:
  GreaterOrEqualThanRelationalExpr() : BinaryExpr(NULL, NULL) {}
  GreaterOrEqualThanRelationalExpr(Expr *epxr1, Expr *expr2) : BinaryExpr(expr1, expr2) {}

  int eval();
  void generate_code(mips_data_t *ret);
};

class LessThanRelationalExpr : public BinaryExpr
{
public:
  LessThanRelationalExpr() : BinaryExpr(NULL, NULL) {}
  LessThanRelationalExpr(Expr *epxr1, Expr *expr2) : BinaryExpr(expr1, expr2) {}

  int eval();
  void generate_code(mips_data_t *ret);
};

class LessOrEqualThanRelationalExpr : public BinaryExpr
{
public:
  LessOrEqualThanRelationalExpr() : BinaryExpr(NULL, NULL) {}
  LessOrEqualThanRelationalExpr(Expr *epxr1, Expr *expr2) : BinaryExpr(expr1, expr2) {}

  int eval();
  void generate_code(mips_data_t *ret);
};

class NumberExpr : public Expr
{
public:
  NumberExpr(int number)
  {
    this->number = number;
  }
  int eval() { return number; }
  int number;
  void generate_code(mips_data_t *ret);
};

class VarExpr : public Expr
{
public:
  VarExpr(string *identifier)
  {
    this->identifier = *identifier;
    // cout << "VarExpr: " << this->identifier << endl;
    // printf("%\np", identifier);
  }
  int eval();
  string identifier;
  void generate_code(mips_data_t *ret);
};

extern map<string,string>* local_vars;
string find_var(string id) {
  if(!local_vars)
    return "";

  return (*local_vars)[id];  
}

class Statement
{
public:
  virtual void exec() = 0;
  virtual void generate_code(mips_data_t *ret) = 0;
};

class PrintStatement : public Statement
{
public:
  PrintStatement() : Statement() {}
  PrintStatement(Expr *expr, int format) : Statement()
  {
    this->expr = expr;
    this->format = format;
  }

  void exec();
  void generate_code(mips_data_t *ret);

  Expr *expr;
  int format;
};

class AssignStatement : public Statement
{
public:
  AssignStatement() : Statement() {}
  AssignStatement(Expr *expr, string *identifier) : Statement()
  {
    this->expr = expr;
    this->identifier = *identifier;
  }

  void exec();
  void generate_code(mips_data_t *ret);

  Expr *expr;
  string identifier;
};

class BlockStatement : public Statement
{
public:
  BlockStatement() : Statement() {}

  void addStatement(Statement *statement)
  {
    statementList.push_back(statement);
  }

  void exec();
  void generate_code(mips_data_t *ret);

  list<Statement *> statementList;
};

class ConditionalStatement : public Statement
{
protected:
  ConditionalStatement(Expr *expr)
  {
    this->expr = expr;
  }

public:
  Expr *expr;
};

class IfStatement : public ConditionalStatement
{
public:
  IfStatement(Expr *expr, Statement *body, Statement *else_body) : ConditionalStatement(expr)
  {
    this->body = body;
    this->else_body = else_body;
    cond = expr;
  }

  void exec();
  void generate_code(mips_data_t *ret);

  Statement *body;
  Statement *else_body;
  Expr *cond;
};

class WhileStatement : public Statement
{
public:
  WhileStatement(Expr *expr, Statement *body) : Statement()
  {
    this->expr = expr;
    this->body = body;
  }

  void exec();
  void generate_code(mips_data_t *ret);

  Expr *expr;
  Statement *body;
};

class FunctionDefinitionStatement : public Statement
{
public:
  FunctionDefinitionStatement(string *name, list<string> *parameters, Statement *body) : Statement()
  {
    this->name = *name;
    this->parameters = parameters;
    this->body = body;
  }
  void exec();
  void generate_code(mips_data_t *ret);
  string name;
  list<string> *parameters;
  Statement *body;
};

class ReturnStatement : public Statement
{
public:
  ReturnStatement(Expr *expr) : Statement()
  {
    this->expr = expr;
  }

  Expr *expr;
  void exec();
  void generate_code(mips_data_t *ret);
};

class FunctionCallStatement : public Statement
{
public:
  FunctionCallStatement(string *name, list<Expr *> *args) : Statement()
  {
    this->fn_name = *name;
    this->args = args;
  }

  string fn_name;
  list<Expr *> *args;
  void exec();
  void generate_code(mips_data_t *ret);
};

#endif