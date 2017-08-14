#include <stdio.h>
#include "tokens.h"

int yylex();
int yyparse();
int main(){
  yyparse();
  return 0;
}
