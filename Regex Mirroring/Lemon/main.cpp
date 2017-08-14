#include<stdio.h>
#include<string>

using namespace std;

void *ParseAlloc(void *(*allocProc)(size_t));
void *Parse(void *, int, string*);
void *ParseFree(void *, void (*freeProc)(void *));
extern string* token;
int yylex();

int main(){

  void* pParser = ParseAlloc( malloc );
  int input;
  while((input = yylex()) != 0){
    Parse(pParser, input, token);
  }
  Parse(pParser, 0, 0);
  ParseFree(pParser, free );

  return 0;
}
