#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

void* ParseAlloc(void* (*allocProc)(size_t));
void* Parse(void*, int, string*);
void* ParseFree(void*, void (*freeProc)(void*));

int yylex();
extern string *token;

int main()
{
    int itoken;
    void* parser = ParseAlloc(malloc);
    while( (itoken = yylex()) != 0 ){
        Parse(parser, itoken, token);
    }
    Parse(parser, 0,0);
    ParseFree(parser, free);
    return 0;
}