int yylex();
void yyparse();

int main(int argc, char* argv[]) {
  yyparse();
  return 0;
}
