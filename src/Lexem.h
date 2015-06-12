#ifndef LEXEM
#define LEXEM

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define MAX_LEXEM_SIZE 20

class Lexem {
  public:
    int t;
    char *l;

    Lexem(int t, char *l);
    int compare(Lexem *l);
    Lexem* duplicate();
    void prettyPrint();
};

#endif
