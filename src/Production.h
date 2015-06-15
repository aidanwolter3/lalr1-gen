#ifndef PRODUCTION
#define PRODUCTION

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "Lexem.h"
#include "LexemSet.h"

#define MAX_PROD_STR_LEN 20

//A single production which holds the left origin and right derivations. The
//object tracks how far the production has been parsed with a mark.
class Production {
  public:
    bool completed;

    // ex. 'S -> . A B C'
    int id;
    Lexem *left;  // ex. 'S'
    LexemSet *right;// ex. 'A B C'
    int mark;   // ex. 0
    LexemSet *followSet;

    //general constructor
    Production(int id, Lexem *left, LexemSet *right);

    //duplicate the production
    Production* duplicate();

    //print the production
    void prettyPrint();

    //check for equality in productions
    bool equals(Production *prod);

    void addFollow(Lexem *l);
    void addFollow(LexemSet *s);
};

#endif
