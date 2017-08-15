#include <stdio.h>
#include "token.h"
#include "ast.h"

int yylex();
void yyparse();
extern FILE* yyin;
int push_file(char* filename);

void *ParseAlloc(void *(*allocProc)(size_t));
void *Parse(void *, int, Token*);
void *ParseFree(void *, void (*freeProc)(void *));

extern token_t* global_token;

int main(int argc, char* argv[]){
  if(argc != 2){
    fprintf(stderr,"Usage: %s <input file>\n",argv[0]);
    return 1;
  }
  if(push_file(argv[1]) !=0 )
    return 1;

  yyparse();
}

void yyparse(){
  void *parser = ParseAlloc(malloc);
  int token = yylex();

   while (token != 0)
   {
       if(token == TK_EOL){
           while( (token = yylex()) == TK_EOL );
           if(token == 0) break;

           if(token == KW_ELSE){
               Parse(parser, KW_ELSE, 0);
           }else{
               Parse(parser, TK_EOL, 0);
               Parse(parser, token, global_token);
           }
       }else{
           Parse(parser, token, global_token);
       }
       token = yylex();
   }
   Parse(parser, 0, 0);

   ParseFree(parser, free);
}

/*
void yy_switch_to_buffer (YY_BUFFER_STATE new_buffer  );
YY_BUFFER_STATE yy_create_buffer (FILE *file,int size  );
*/
