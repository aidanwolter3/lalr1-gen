#ifndef LEXEM_SET
#define LEXEM_SET

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "Lexem.h"

#define MAX_LEXEM_SET_LEN 30

//A follow set for a production
class LexemSet {
  private:
    int sortedOrder[MAX_LEXEM_SET_LEN];

  public:
    Lexem **items;
    int size;

    //general constructor
    LexemSet();

    //add a new follow to the set. do not allow duplicates. sort the set afterwards
    void add(Lexem *l);

    //add all the items in one set to this one
    void add(LexemSet *s);

    //retrieve a lexem from the string
    Lexem* get(char *str);

    //remove a specific item
    int remove(Lexem *l);

    //check if the item exists in the set
    int has(Lexem *l);

    //check for equality between sets
    bool equals(LexemSet *f);

    //print the follow set
    void prettyPrint();

    //duplicate the follow set
    LexemSet* duplicate();
};

#endif
