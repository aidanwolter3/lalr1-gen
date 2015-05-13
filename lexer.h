#include "stdio.h"
#include "stdlib.h"
#include "string.h"

class Token {
  public:
    char lexem[256];
    int size;
    int id;
    Token() {
      size = 0;
      id = 0;
    }
    void addChar(char c) {
      lexem[size++] = c;
    }
    void prettyPrint() {
      printf("Token %d: %s\n", id, lexem);
    }
};

class Lexer {
  private:
    char *filename;
    FILE *file;
    bool newlineLast;
  public:
    Lexer(char *filename);
    Token* nextToken();
};
