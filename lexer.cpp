#include "lexer.h"

Lexer::Lexer(char *filename) {
  this->filename = (char*)malloc(strlen(filename));
  strcpy(this->filename, filename);
  this->file = fopen(filename, "r");
  newlineLast = false;
}

Token* Lexer::nextToken() {

  Token *newToken = new Token();
  newToken->id = -1;

  //if newline hit last, return newline token
  if(newlineLast == true) {
    newlineLast = false;
    newToken->id = 0;
    return newToken;
  }

  //get the lexem of the token
  char nextChar = fgetc(this->file);

  if(nextChar == EOF) {
    return newToken;
  }
  
  while(nextChar != ' ' && nextChar != '\n' && nextChar != '\t') {
    newToken->addChar(nextChar);
    nextChar = fgetc(this->file);
  }

  //assign
  if(strcmp(newToken->lexem, "->") == 0) {
    newToken->id = 2;
  }
  else {
    newToken->id = 1;
  }
  
  //check if token was newline so that next time we return a newline token
  if(nextChar == '\n') {
    newlineLast = true;
  }
  
  return newToken;
}
