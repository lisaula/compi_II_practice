%option noyywrap
%option yylineno
%x multiline_comment
%x include_state
%x include_state_2

%{
  #include <stdio.h>
  #include "token.h"
  #include "string.h"
  #include "ast.h"

  extern int yychar;
  typedef struct include_stack {
    struct include_stack *prev;
    FILE* f;
    int lineno;
    char* filename;
    YY_BUFFER_STATE bs;
  } include_stack_t;

  include_stack_t* inc_stack = NULL;
  char* yyfilename;
  int push_file(char* filename);
  int pop_file();

  token_t* global_token;
%}

ID       ("_"([A-Za-z])+[0-9a-zA-Z]*)|([A-Za-z])[a-zA-Z0-9"_"]*
%%
[ \t]   { /*Nada*/ }
^"#"[ \t]*"include"[ \t]\"  { BEGIN(include_state); /*^ obliga que la expression debe estar al inicio del file*/}
^"#"[ \t]*"include"[ \t]"<"  { BEGIN(include_state_2); }
"/*"    { BEGIN(multiline_comment); }
"//".*\n  { return TK_EOL; }
"+"     { return OP_ADD; }
"-"     { return OP_SUB; }
"*"     { return OP_MUL; }
"/"     { return OP_DIV; }
"("     { return TK_LEFT_PAR; }
")"     { return TK_RIGHT_PAR; }
[0-9]+  { global_token = (token_t*)malloc(sizeof(token_t)); global_token->int_value = atoi(yytext); return TK_NUMBER; }
"\n"    { return TK_EOL; }
"="     { return OP_ASSIGN; }
"print" { return KW_PRINT; }
"while" { return KW_WHILE; }
"HEX" { return KW_HEX; }
"BIN" { return KW_BIN; }
"DEC" { return KW_DEC; }
"function" { return KW_FUNC; }
"return"  { return KW_RETURN; }
"if"  { return KW_IF; }
"else"  { return KW_ELSE; }
"=="  { return OP_EQUAL; }
"!="  { return OP_NOT_EQUAL; }
"<"  { return OP_LESS_THAN; }
">"  { return OP_GREATER_THAN; }
"<="  { return OP_LESS_OR_EQUAL; }
">="  { return OP_GREATER_OR_EQUAL; }
"{" { return TK_OPEN_BRAKET; }
"}" { return TK_CLOSE_BRAKET; }

"," { return OP_COMMA; }
{ID}  { global_token = (token_t*)malloc(sizeof(token_t)); global_token->char_value = strdup(yytext);return TK_ID; }
<INITIAL><<EOF>> { if(pop_file() != 0) yyterminate(); }
.       { printf("unexpected syntax %s\n", yytext);yyterminate();}


<include_state>[^\"\n]+ {
                          {
                            int c;
                            if((c = yyinput()) && c != '\"'){
                              fprintf(stderr, "Line %d: Include statement not closed. Expected '\"'\n",yylineno);
                              printf("c: %c. y yytext: %s\n ",c, yytext);
                              yyterminate();
                            }
                            if(push_file(yytext) != 0)
                              yyterminate();
                            BEGIN(INITIAL);
                          }
                        }

<include_state_2>[^">"\n]+ {
                          {
                            int c;
                            if((c = yyinput()) && c != '>'){
                              fprintf(stderr, "Line %d: Include statement not closed. Expected '>'\n",yylineno);
                              printf("c: %c. y yytext: %s\n ",c, yytext);
                              yyterminate();
                            }
                            if(push_file(yytext) != 0)
                              yyterminate();
                            BEGIN(INITIAL);
                          }
                        }


<multiline_comment>"*/"["\n"]* { BEGIN(INITIAL);}
<multiline_comment>.|\n { /*nada*/ }
<multiline_comment><<EOF>>  { printf("Line %d: Comment block not end\n",yylineno); return 0; }

%%

int push_file(char* filename){
  FILE* f = fopen(filename, "r");
  if(f == NULL){
    fprintf(stderr, "File %s not found.\n", filename);
    return 1;
  }
  include_stack_t* is = (include_stack_t*)malloc(sizeof(include_stack_t));
  if(!is){
    fprintf(stderr, "Out of memory.\n");
    return 1;
  }
  is->f = f;
  is->lineno = yylineno;
  is->filename = yyfilename;
  is->bs = yy_create_buffer(f, YY_BUF_SIZE);
  is->prev = inc_stack;
  inc_stack = is;
  yylineno = 1;
  yyfilename = filename;
  yy_switch_to_buffer(is->bs);
  return 0;
}

int pop_file(){
  include_stack_t* prev_is = inc_stack->prev;
  fclose(inc_stack->f);
  yy_delete_buffer(inc_stack->bs);
  yylineno = inc_stack->lineno;
  yyfilename = inc_stack->filename;

  free(inc_stack);

  if(prev_is == NULL) return 1;

  yy_switch_to_buffer(prev_is->bs);
  inc_stack = prev_is;
  return 0;
}
