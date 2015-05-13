#include "lexer.h"

Lexer::Lexer(char *filename) {
  this->filename = (char*)malloc(strlen(filename));
  strcpy(this->filename, filename);
  this->file = fopen(filename, "r");
}

Token* Lexer::nextToken() {

  //get the lexem of the token
  Token *newToken = new Token();
  char nextChar = fgetc(this->file);
  while(nextChar != ' ' && nextChar != '\n' && nextChar != '\t') {
    newToken->addChar(nextChar);
    nextChar = fgetc(this->file);
  }

  //determine the id of the token

  //production
  if(newToken->lexem[0] >= 'A' && newToken->lexem[0] <= 'Z') {
    newToken->id = 0;
  }

  //assign
  else if(strcmp(newToken->lexem, "->") == 0) {
    newToken->id = 1;
  }

  //literal
  else {
    newToken->id = 2;
  }
  
  return newToken;
}
