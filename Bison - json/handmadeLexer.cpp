#include <stdio.h>
#include <iostream>
#include <string>
#include "tokens.h"
using namespace std;
string buffer="";

FILE* f;
char getNextInput(){
  return fgetc(f);
}
char * yytext;
int yylex(void){
  char character = getNextInput();
  if(character == EOF)
    return 0;
  while(character == ' ' || character == '\t' || character == '\r' || character == '\n' )
    character = getNextInput();


  if(character == '\"'){
    buffer = "\"";

    while ((character = getNextInput()) != '\"') {
      buffer += character;
    }
    buffer += "\"";

    yytext = (char*)buffer.c_str();

    return TK_ID;
  }else if(character >='0' && character <= '9' ){
    buffer = "";
    while (character >='0' && character <= '9') {
      buffer += character;
      character = getNextInput();
    }
    yytext = (char*)buffer.c_str();

    return TK_NUMBER;
  }else if(character == '{'){
    yytext = "{";
    return TK_LEFT_BRACE;
  }else if(character == '}'){
    yytext = "}";
    return TK_RIGHT_BRACE;
  }else if(character == '['){
    yytext = "[";
    return TK_LEFT_BRACKET;
  }else if(character == ']'){
    yytext = "]";
    return TK_RIGHT_BRACKET;
  }else if(character == ':'){
    yytext = ":";
    return OP_COLON;
  }else if(character == ','){
    yytext = ",";
    return OP_COMMA;
  }

  return 0;
}
