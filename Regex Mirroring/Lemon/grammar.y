%include{
  #include<stdio.h>
  #include<assert.h>
  #include<iostream>
  #include<stdlib.h>
  #include<string>

  using namespace std;

  extern int yylineno;
}

%parse_accept {
  printf("parsing complete!\n");
}
%syntax_error {
  printf("Syntax error at line %d \n", yylineno);
}

%token_type { string* }
%default_type { string* }
%type e { string* }
%type t { string* }
%type f { string* }
%type r { string* }
%type kleene_optional { string* }

%start_symbol input

input ::= e(S1) . { printf("reverse: %s\n", S1->c_str()); delete S1; }

e(SS) ::= e(S1) OP_PIPE t(S3) . { SS = new string(*S1 + "+" + *S3); delete S1;delete S3;}
e(SS) ::= t(S1) . { SS = S1; }

t(SS) ::= t(S1) OP_CONCAT f(S3) . { SS = new string( *S3 + "." + *S1); delete S3;delete S1;}
t(SS) ::=  f(S1) . { SS = S1; }

f(SS) ::= TK_LEFT_PAR e(S2) TK_RIGHT_PAR kleene_optional(S4) . { SS = new string("("+ *S2 +")"+ *S4); delete S2;delete S4; }
f(SS) ::= r(S1) kleene_optional(S2) . { SS = new string(*S1 + *S2); delete S1;delete S2; }

kleene_optional(SS) ::= OP_KLEENE . { SS = new string("*"); }
kleene_optional(SS) ::= . { SS = new string(); }

r(SS) ::= LIT_INT(S1) . { SS = S1; }
r(SS) ::= LIT_LETTER(S1) . { SS = S1; }
