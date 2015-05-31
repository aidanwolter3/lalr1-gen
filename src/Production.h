#ifndef PRODUCTION
#define PRODUCTION

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "Set.h"

#define MAX_PROD_STR_LEN 20

//A single production which holds the left origin and right derivations. The
//object tracks how far the production has been parsed with a mark.
class Production {
  public:

    // ex. 'S -> .ABC'
    char left;  // ex. 'S'
    char *right;// ex. 'ABC'
    int mark;   // ex. 0
    Set *followSet;
    int id;

    //track whether this production has already been check so we do not have
    //to bother removing it from the state
    bool completed;

    //general constructor
    Production(int id, char left, char *right);
    Production(int id, char left);

    //duplicate the production
    Production* duplicate();

    //print the production
    void prettyPrint();

    //check for equality in productions
    bool equals(Production *prod);
};

#endif
