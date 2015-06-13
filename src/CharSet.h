#ifndef SET
#define SET

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#define MAX_FOLLOW_SET_LEN 10

//A follow set for a production
class CharSet {
  public:
    char *items;
    int size;

    //general constructor
    CharSet();

    //sort the set for easy comparison
    void sort();

    //add a new follow to the set. do not allow duplicates. sort the set afterwards
    void add(char c);

    //add all the items in one set to this one
    void add(CharSet *s);

    //remove a specific item
    int remove(char c);

    //check if the item exists in the set
    bool has(char c);

    //check for equality between sets
    bool equals(CharSet *f);

    //print the follow set
    void prettyPrint();

    //set setter. copies string instead of reusing
    void set(char *items);

    //duplicate the follow set
    CharSet* duplicate();
};

#endif
