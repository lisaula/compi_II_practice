#ifndef _AST_H
#define _AST_H

class Statement {
    public:
        Statement() {}
        virtual void exec() = 0;
};

class Expression {
    public:
        Expression(){}
        virtual int eval() = 0;
};

class AddExpression : public Expression {
    public:
        AddExpression(int val1, int val2){
            this->val1 =  val1;
            this->val2 = val2;
        }

        int val1, val2;
        int eval();
};

class SubExpression : public Expression {
    public:
        SubExpression(int val1, int val2){
            this->val1 =  val1;
            this->val2 = val2;
        }

        int val1, val2;
        int eval();
};

class DivExpression : public Expression {
    public:
        DivExpression(int val1, int val2){
            this->val1 =  val1;
            this->val2 = val2;
        }

        int val1, val2;
        int eval();
};

class MultExpression : public Expression {
    public:
        MultExpression(int val1, int val2){
            this->val1 =  val1;
            this->val2 = val2;
        }

        int val1, val2;
        int eval();
};

class VarExpression : public Expression {
    public:
        VarExpression(char *name) {
            this->name = name;
        }

        char * name;
        int eval();
}

class AssignStatement : public Statement {
    public:
        AssignStatement(char * name, Expression* expr){
            this->name = name;
            this->expr = expr;
        }

        char *name;
        Expression * expr;

        int eval();
};

class PrintStatement : public Statement {
    public:
        PrintStatement(char * name) {
            this->name = name;
        }

        char * name;

        void exec();
};

class StructDeclaration : public Statement {
    public:
        StructDeclaration(char * name) {
            this->name = name;
        }

        char *name;
        vector<Statement*> statements;
        void exec();

        void addStatement(Statement *statement) {
            this->statements.push_back(statement);
        }
};

class VariableDeclaration : public Statement {
    public:
        VariableDeclaration(char *name, char * type) {
            this->name = name;
            this->type = type;
        }

        char *name, type;
        void exec();
        void exec(map<string, int>);
};

class ReadStatement : public Statement {
    public:
        ReadStatement(){}
        void exec();
};

#endif