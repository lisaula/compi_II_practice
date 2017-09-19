#ifndef __AST_H
#define __AST_H

#define uint32 unsigned int

#include <string>

using namespace std;

class Variable
{
  public:
    Variable() {}
    Variable(string type, uint32 size){
        this->size = size;
        this->type = type;
        this->mem = (void*)malloc(size);
    }
    void *mem;
    string type;
    uint32 size;
};

class Statement
{
};

class Expr
{
};

#endif