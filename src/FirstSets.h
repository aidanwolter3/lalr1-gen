#ifndef FIRST_SETS
#define FIRST_SETS

#include "stdio.h"
#include "string.h"

#include "Set.h"
#include "Production.h"

#define MAX_NUM_PRODS 20

//a class which computes all the first sets recursively for the productions
class FirstSets {
  private:
    Production **prods;
    int productionCount;

  public:
    char lefts[MAX_NUM_PRODS];
    Set *sets[MAX_NUM_PRODS];
    int size;

    //general constructor
    FirstSets(Production *prods[], int productionCount);

    //generate all first sets
    void generate();

    //recursive function that generates a single first set by generating others
    Set* firstForProd(char c);

    //print the first sets nicely
    void prettyPrint();
};

#endif
