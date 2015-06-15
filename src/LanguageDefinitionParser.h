#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "Production.h"
#include "LexemSet.h"

#define MAX_NUM_LEXEMS 20
#define MAX_NUM_PRODS 20

class LanguageDefinitionParser  {
  private:
    LexemSet *lexems;

  public:
    LanguageDefinitionParser();
    int parse(const char* filename, Production *prods[], int *production_count);
};
