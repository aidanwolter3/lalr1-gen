#include "lexer.h"

Lexer::Lexer(char *filename) {
  this->filename = (char*)malloc(strlen(filename));
  strcpy(this->filename, filename);
  this->file = fopen(filename, "r");
  newlineLast = false;
}

Token* Lexer::nextToken() {

  Token *newToken = new Token();

  //if newline hit last, return newline token
  if(newlineLast == true) {
    newlineLast = false;
    newToken->id = 0;
    return newToken;
  }

  //get the lexem of the token
  char nextChar = fgetc(this->file);
  while(nextChar != ' ' && nextChar != '\n' && nextChar != '\t') {
    newToken->addChar(nextChar);
    nextChar = fgetc(this->file);
  }

  //determine the id of the token
  //production
  if(newToken->lexem[0] >= 'A' && newToken->lexem[0] <= 'Z') {
    newToken->id = 1;
  }

  //assign
  else if(strcmp(newToken->lexem, "->") == 0) {
    newToken->id = 2;
  }

  //literal
  else {
    newToken->id = 3;
  }
  
  //check if token was newline so that next time we return a newline token
  if(nextChar == '\n') {
    newlineLast = true;
  }
  
  return newToken;
}
