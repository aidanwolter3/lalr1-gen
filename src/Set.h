#ifndef SET
#define SET

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#define MAX_FOLLOW_SET_LEN 10

//A follow set for a production
class Set {
  public:
    char *items;
    int size;

    //general constructor
    Set();

    //sort the set for easy comparison
    void sort();

    //add a new follow to the set. do not allow duplicates. sort the set afterwards
    void add(char c);

    //add all the items in one set to this one
    void add(Set *s);

    //remove a specific item
    int remove(char c);

    //check if the item exists in the set
    bool has(char c);

    //check for equality between sets
    bool equals(Set *f);

    //print the follow set
    void prettyPrint();

    //set setter. copies string instead of reusing
    void set(char *items);

    //duplicate the follow set
    Set* duplicate();
};

#endif
