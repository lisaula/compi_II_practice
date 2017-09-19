#include "ast.h"

int vars[8];
map<string, int> variables;
map<string, FunctionDefinitionStatement*> funs;
set<string> global_vars;
map<string,string>* local_vars;
map<string, string> global_strings;
stack<string> temps;
unsigned int next_label;
unsigned int next_var;

void initialize_temps()
{
    for (int i = 0; i < 10; i++)
    {
        char str[4];
        sprintf(str, "$t%d", 9 - i);
        temps.push(string(str));
}
next_label = 0;
next_var = 0;
}

void release_temp(string temp)
{
    if (temp[0] == '$' && temp[1] == 't' && temp[2] >= '0' && temp[2] <= '9')
        temps.push(temp);
}

void generate_header()
{
    printf("#include \"screen.h\"\n");
    printf("#include \"system.h\"\n");
    printf(".global main\n\n");
    printf(".data\n");

    printf("enter: .asciz \"\\n\"\n");
    printf("quo_xyz: .word 0\n");
    printf("rem_xyz: .word 0\n");
    // printf("next_var: %d\n", next_var);
    map<string, string>::iterator i;
    for (i = global_strings.begin(); i != global_strings.end(); i++)
    {
        printf("%s: %s", (*i).first.c_str(), (*i).second.c_str());
    }

    set<string>::iterator it;
    for (it = global_vars.begin(); it != global_vars.end(); it++)
        printf("%s: .word 0\n", (*it).c_str());

    printf("\n");

    printf(".text\n\n");
    printf("main:\n");
    printf("li $a0, BRIGHT_WHITE\n");
    printf("li $a1, BLACK\n");
    printf("jal set_color\n");
    printf("jal clear_screen\n");
    printf("jal keypad_init\n\n");
}

void generate_tail()
{
    printf("jr $ra\n");
}

void reset_mips_data(mips_data_t* data)
{
    data->code = "";
    data->place = "";
}
//Expressions

int FnCallExpr::eval()
{
    printf("Function call expr: %s( ", this->fn_name.c_str());
    for(auto i = this->args->begin(); i != this->args->end(); i++)
        printf("%d, ", (*i)->eval());
    printf(" )\n");

    auto fn = funs[this->fn_name];
    fn->body->exec();
}

int AddExpr::eval()
{
    return expr1->eval() + expr2->eval();
}

void AddExpr::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    string code, place;
    expr1->generate_code(ret);
    code = ret->code;
    place = ret->place;
    ret->code = "";
    ret->place = "";
    expr2->generate_code(ret);
    release_temp(ret->place);
    code += ret->code;

    code += "add " + place + ", " + place + ", " + ret->place + "\n";

    ret->place = place;
    ret->code = code;
}

void FnCallExpr::generate_code(mips_data_t *ret){

    ret->code = ""; ret->place = "";
    if(args->size() > 4)
        throw std::invalid_argument("function:"+fn_name+", exceeds more than 4 arguments");

    int c = 0;
    for(auto it = args->begin(); it != args->end(); ++it){
        Expr* argu = *(it);
        mips_data_t a_ret;
        argu->generate_code(&a_ret);
        ret->code += a_ret.code+"\n";
        char str[2];
        sprintf(str, "%d",c++);
        ret->code += "move $a"+string(str)+", "+a_ret.place+"\n";
        release_temp(a_ret.place);
    }

    ret->code += "jal "+fn_name+"\n";
    ret->place = "$v0"; 
}

string pusha(){
    return "addi $sp, $sp, -24\nsw $a0, 0($sp)\nsw $a1, 4($sp)\nsw $a2, 8($sp)\nsw $a3, 12($sp)\nsw $ra, 16($sp)\nsw $fp, 20($sp)\nmove $fp, $sp\n";
}

string popa(){
    return "move $sp, $fp\nlw $a0, 0($sp)\nlw $a1, 4($sp)\nlw $a2, 8($sp)\nlw $a3, 12($sp)\nlw $ra, 16($sp)\nlw $fp, 20($sp)\naddi $sp, $sp, 24\n";
}

void FunctionDefinitionStatement::generate_code(mips_data_t *ret){
    reset_mips_data(ret);
    string code;
    code = name + ":\n";
    
    if(parameters->size() > 4)
        throw std::invalid_argument("function:"+name+", exceeds more than 4 arguments");

    /*
        Ritual:
        addi $sp, $sp, -24
        sw $a0, 0($sp)
        sw $a1, 4($sp)
        sw $a2, 8($sp)
        sw $a3, 12($sp)
        sw $ra, 16($sp)
        sw $fp, 20($sp)
        move $fp, $sp

        $a0-$a3
        $ra
        $fp
        Reservar: 24 bytes + size_to_reserve
        //obtener el espacio de vars
        //mapear las vars
    */

    code += pusha();

    int size_to_reserve = 0;

    int arg_counter = 0;
    //local_vars = new std::map<string, value> map;
    for(auto it = parameters->begin(); it != parameters->end(); ++it)
    {
        auto param = *it;
        (*local_vars)[param] = to_string(arg_counter)+"($fp)";// "$a" + to_string(arg_counter++);
        arg_counter += 4;
    }

    body->generate_code(ret);

    code += ret->code+"\n";

    code += popa();

    code += "jr $ra\n";

    ret->code = code;
}


void FunctionCallStatement::generate_code(mips_data_t *ret){
    ret->code = ""; ret->place = "";
    if(args->size() > 4)
        throw std::invalid_argument("function:"+fn_name+", exceeds more than 4 arguments");

    int c = 0;
    for(auto it = args->begin(); it != args->end(); ++it){
        Expr* argu = *(it);
        mips_data_t a_ret;
        argu->generate_code(&a_ret);
        ret->code += a_ret.code+"\n";
        char str[2];
        sprintf(str, "%d",c++);
        ret->code += "move $a"+string(str)+", "+a_ret.place+"\n";
        release_temp(a_ret.place);
    }

    ret->code += "jal "+fn_name+"\n";
}

void ReturnStatement::generate_code(mips_data_t *ret){
    ret->code= "";
    ret->place = "";

    expr->generate_code(ret);
    release_temp(ret->place);

    ret->code = ret->code + "move $v0, " + ret->place + "\n";
    ret->place = "$v0";
}

void SubExpr::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    string code, place;
    expr1->generate_code(ret);
    code = ret->code;
    place = ret->place;
    ret->code = "";
    ret->place = "";
    expr2->generate_code(ret);
    release_temp(ret->place);
    code += ret->code;

    code += "sub " + place + ", " + place + ", " + ret->place + "\n";

    ret->place = place;
    ret->code = code;
}

void MulExpr::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    string code, place;
    expr1->generate_code(ret);
    code = ret->code;
    place = ret->place;
    ret->code = "";
    ret->place = "";
    expr2->generate_code(ret);
    release_temp(ret->place);
    release_temp(place);
    code += ret->code;

    code += "move $a0, " + place + "\n";
    code += "move $a1, " + ret->place + "\n";
    code += "jal mult\n";

    ret->place = temps.top();
    temps.pop();
    code += "move " + ret->place + ", $v0\n";
    ret->code = code;
}

void DivExpr::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    string code, place;
    expr1->generate_code(ret);
    code = ret->code;
    place = ret->place;
    ret->code = "";
    ret->place = "";
    expr2->generate_code(ret);
    release_temp(ret->place);
    release_temp(place);
    code += ret->code;

    code += "move $a0, " + place + "\n";
    code += "move $a1, " + ret->place + "\n";
    code += "la $a2, quo_xyz\n";
    code += "la $a3, rem_xyz\n";
    code += "jal divide\n";

    ret->place = temps.top();
    temps.pop();
    code += "lw " + ret->place + ", quo_xyz\n";
    ret->code = code;
}

int SubExpr::eval()
{
    return expr1->eval() - expr2->eval();
}

int MulExpr::eval()
{
    return expr1->eval() * expr2->eval();
}

int DivExpr::eval()
{
    return expr1->eval() / expr2->eval();
}

int VarExpr::eval()
{
    // printf("Var {%s} = %d\n", this->identifier.c_str(), variables[this->identifier]);
    // cout << "Var Eval (" << this->identifier << ")= " << variables[this->identifier] << endl;
    return variables[identifier];
}

void VarExpr::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    string place = temps.top();
    temps.pop();
    ret->code += "lw " + place + ", " + this->identifier + "\n";
    ret->place = place;
}


void LessOrEqualThanRelationalExpr::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    string code, place;
    expr1->generate_code(ret);
    code = ret->code;
    place = ret->place;
    ret->code = "";
    ret->place = "";
    expr2->generate_code(ret);
    release_temp(ret->place);
    code += ret->code;

    code += "sle " + place + ", " + place + ", " + ret->place + "\n";

    ret->place = place;
    ret->code = code;
}

void GreaterOrEqualThanRelationalExpr::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    string code, place;
    expr1->generate_code(ret);
    code = ret->code;
    place = ret->place;
    ret->code = "";
    ret->place = "";
    expr2->generate_code(ret);
    release_temp(ret->place);
    code += ret->code;

    code += "sge " + place + ", " + place + ", " + ret->place + "\n";

    ret->place = place;
    ret->code = code;
}

void LessThanRelationalExpr::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    string code, place;
    expr1->generate_code(ret);
    code = ret->code;
    place = ret->place;
    ret->code = "";
    ret->place = "";
    expr2->generate_code(ret);
    release_temp(ret->place);
    code += ret->code;

    code += "slt " + place + ", " + place + ", " + ret->place + "\n";

    ret->place = place;
    ret->code = code;
}

void GreaterThanRelationalExpr::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    string code, place;
    expr1->generate_code(ret);
    code = ret->code;
    place = ret->place;
    ret->code = "";
    ret->place = "";
    expr2->generate_code(ret);
    release_temp(ret->place);
    code += ret->code;

    code += "sgt " + place + ", " + place + ", " + ret->place + "\n";

    ret->place = place;
    ret->code = code;
}

void NotEqualRelationalExpr::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    string code, place;
    expr1->generate_code(ret);
    code = ret->code;
    place = ret->place;
    ret->code = "";
    ret->place = "";
    expr2->generate_code(ret);
    release_temp(ret->place);
    code += ret->code;

    code += "sne " + place + ", " + place + ", " + ret->place + "\n";

    ret->place = place;
    ret->code = code;
}

void EqualRelationalExpr::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    string code, place;
    expr1->generate_code(ret);
    code = ret->code;
    place = ret->place;
    ret->code = "";
    ret->place = "";
    expr2->generate_code(ret);
    release_temp(ret->place);
    code += ret->code;

    code += "seq " + place + ", " + place + ", " + ret->place + "\n";

    ret->place = place;
    ret->code = code;
}

void NumberExpr::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    ret->place = temps.top();
    temps.pop();
    char val[25];
    sprintf(val, "%d", this->number);
    ret->code = "li " + ret->place + ", " + string(val) + "\n";
}

int EqualRelationalExpr::eval()
{
    return expr1->eval() == expr2->eval();
}

int NotEqualRelationalExpr::eval()
{
    return expr1->eval() != expr2->eval();
}

int GreaterThanRelationalExpr::eval()
{
    return expr1->eval() > expr2->eval();
}

int GreaterOrEqualThanRelationalExpr::eval()
{
    return expr1->eval() >= expr2->eval();
}

int LessThanRelationalExpr::eval()
{
    return expr1->eval() < expr2->eval();
}

int LessOrEqualThanRelationalExpr::eval()
{
    return expr1->eval() <= expr2->eval();
}

//Statements
void PrintStatement::exec()
{
    // printf("%d %d\n", number, option);
    if (format == DEC)
        printf("DEC: %d\n", expr->eval());
    else if (format == HEX)
        printf("HEX: %X\n", expr->eval());
    else
    {
        int number = expr->eval();
printf("BIN: ");
for (int j = 31; j >= 0; j--)
{
    int result;
    result = number & (1 << j);
    if (result)
        printf("1");
    else
        printf("0");
}
printf("\n");
}
}

void PrintStatement::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    // list<Expr *>::iterator it = lexpr.begin();
    string code;
    // code += "start print\n";

    // while (it != lexpr.end())
    // {
    //     Expr *expr = *it;

    expr->generate_code(ret);
    release_temp(ret->place);
    code += ret->code;
    code += "move $a0, " + ret->place + "\n";
    string fn = "put_udecimal";
    code += "jal " + fn + "\n";

        // it++;
    ret->code = "";
    ret->place = "";
    // }
    code += "la $a0, enter\n";
    code += "jal puts\n";
    ret->code = code;
    ret->place = "";
}

void AssignStatement::exec()
{
    // printf("Assign %d to {%s}\n", expr->eval(), this->identifier.c_str());
    // cout<<this->identifier<<endl;
    variables[identifier] = expr->eval();
}

void AssignStatement::generate_code(mips_data_t *ret)
{
    reset_mips_data(ret);

    expr->generate_code(ret);
    release_temp(ret->place);
    string padding = find_var(identifier);
    if(padding == ""){//not local
        if(global_vars.find(identifier) == global_vars.end())
        {
            global_vars.insert(identifier);
        }
        ret->code += "sw " + ret->place + ", " + identifier + "\n";
        ret->place = "";
    }else{
        ret->code += "sw "+ret->place+", "+padding+"\n";
        ret->place = "";
    }
}

void BlockStatement::exec()
{
    for (list<Statement *>::iterator i = statementList.begin(); i != statementList.end(); i++)
    {
        (*i)->exec();
}
}

void BlockStatement::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    list<Statement *>::iterator it = statementList.begin();
    string code;

    while (it != statementList.end())
    {
        ret->code = "";
        ret->place = "";
        Statement *st = *it;

        st->generate_code(ret);
code += ret->code;
it++;
}
ret->place = "";
ret->code = code;
}

void IfStatement::exec()
{
    if (expr->eval())
    {
        body->exec();
}
else if (else_body != NULL)
{
    else_body->exec();
}
}

void IfStatement::generate_code(mips_data_t *ret)
{
    ret->code = "";
    ret->place = "";

    string code = "";

    string if_false, endif;
    char str[25];
    sprintf(str, "if_false_%d", next_label);
    if_false = string(str);
    sprintf(str, "endif%d", next_label++);
    endif = string(str);

    cond->generate_code(ret);
    release_temp(ret->place);
    code += ret->code;
    code += "beqz " + ret->place + ", " + if_false + "\n";

    ret->code = "";
    ret->place = "";
    body->generate_code(ret);
    code += ret->code;

    code += "j " + endif + "\n";
    code += if_false + ": \n";

    ret->code = "";
    ret->place = "";
    if(else_body != NULL){
        else_body->generate_code(ret);
code += ret->code;
}

ret->code = "";
ret->place = "";
code += endif + ":\n";

ret->code = code;
}

void WhileStatement::exec()
{
    while (expr->eval())
    {
        body->exec();
}
}

void WhileStatement::generate_code(mips_data_t *ret)
{
    string code = "";
    ret->code = "";
    ret->place = "";

    expr->generate_code(ret);
    char str[25];
    sprintf(str, "while_%d", next_label);
    string w_start = string(str);
    sprintf(str, "end_while_%d", next_label++);
    string w_end = string(str);

    code += w_start + ":\n";
    code += ret->code;
    release_temp(ret->place);
    code += "beqz " + ret->place + ", " + w_end + "\n";

    ret->code = "";
    ret->place = "";
    body->generate_code(ret);
    code += ret->code;

    code += "j " + w_start + "\n";

    code += w_end + ":\n";

    ret->code = code;
}

void FunctionDefinitionStatement::exec()
{
    printf("Function definition: %s( ", this->name.c_str());
    for(auto i = this->parameters->begin(); i != this->parameters->end(); i++)
        printf("%s, ", (*i).c_str());
    printf(" )\n");

    funs[this->name] = this;

    // body->exec();
    // cout<<"Function definition: "<<this->name<<endl;
}

void ReturnStatement::exec()
{
    printf("Return: %d\n", this->expr->eval());
    // cout<<"Function definition: "<<this->name<<endl;
}

void FunctionCallStatement::exec()
{
    printf("Function call: %s( ", this->fn_name.c_str());
    for(auto i = this->args->begin(); i != this->args->end(); i++)
        printf("%d, ", (*i)->eval());
    printf(" )\n");

    auto fn = funs[this->fn_name];
    fn->body->exec();
}