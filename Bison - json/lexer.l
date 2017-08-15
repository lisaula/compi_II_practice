%option yylineno
%option noyywrap
%x include_state

%{
    #include "tokens.h"

    typedef struct inc_stack {
      char * prev_filename;
      int prev_line;
      FILE * file;
      YY_BUFFER_STATE bs;
      inc_stack * previous_stack;
    } inc_t;

    inc_t * current_stack;
    char * current_filename;
    int pop_stack();
    int push_buffer(char * filename);
    extern yy_size_t yyleng;
%}

%%
"#"[ \t]*"include"[ \t]*\" { BEGIN(include_state); }
[ \t\n\r] { /* do nothing */ }
\"[a-zA-z]*\" { return TK_ID; }
[0-9]+ { return TK_NUMBER; }
"{" { return TK_LEFT_BRACE; }
"}" { return TK_RIGHT_BRACE; }
"[" { return TK_LEFT_BRACKET; }
"]" { return TK_RIGHT_BRACKET; }
":" { return OP_COLON; }
"," { return OP_COMMA; }
<INITIAL><<EOF>> {if(pop_stack() != 0) yyterminate(); }
. { return TK_ERROR; }

<include_state>.*\" {
  {
    yytext[yyleng-1] = 0;
    if(push_buffer(yytext) != 0)
      yyterminate();
    BEGIN(INITIAL);
  }
}
%%

int pop_stack(){
  inc_t* previos_stack = current_stack->previous_stack;

  fclose(current_stack->file);
  yy_delete_buffer(current_stack->bs);
  current_filename = current_stack->prev_filename;
  yylineno = current_stack->prev_line;
  free(current_stack);

  if(!previos_stack)
    return 1;

  yy_switch_to_buffer(previos_stack->bs);
  current_stack = previos_stack;

  return 0;
}

int push_buffer(char * filename) {
  FILE * f = fopen(filename, "r");

  if(!f){
    fprintf(stderr,"File not found 404:%s\n", filename);
    return 1;
  }

  inc_t * new_stack = (inc_t*)malloc(sizeof(inc_t));
  if(!new_stack){
    fprintf(stderr,"Ou of memory.\n");
    return 1;
  }
  //guardando valores viejos
  new_stack->prev_line = yylineno;
  new_stack->prev_filename = current_filename;
  new_stack->previous_stack = current_stack;
  //switchando al nuevo
  new_stack->file = f;
  new_stack->bs = yy_create_buffer(f, YY_BUF_SIZE);
  current_stack = new_stack;
  current_filename = filename;
  yylineno = 1;

  yy_switch_to_buffer(current_stack->bs);
  return 0;
}
