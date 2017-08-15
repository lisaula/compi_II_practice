#include<stdio.h>
#include<iostream>

using namespace std;

int yylex();
extern char*yytext;
int push_buffer(char * filename);

void yyparse();
int main(int argc, char* args[] ){
  push_buffer(args[1]);
  yyparse();
  return 0;
}
