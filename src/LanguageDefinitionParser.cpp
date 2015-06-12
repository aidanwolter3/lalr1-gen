#include "LanguageDefinitionParser.h"

LanguageDefinitionParser::LanguageDefinitionParser() {
  lexems = new LexemSet();
}

void LanguageDefinitionParser::parse(const char* filename, Production *prods[], int *production_count) {
  FILE *definitionFile = fopen(filename, "r");
  *production_count = 0;

  int mode = 0; //start looking for %%
  
  char *line = (char*)malloc(256*sizeof(char*));
  while(fgets(line, 100, definitionFile) != NULL) {

    //unit state machine
    switch(mode) {

      //looking for %%
      case 0: {
        if(line[0] == '%' && line[1] == '%') {
          mode++;
          break;
        }
        break;
      }

      //reading lexems and looking for %%
      case 1: {
        if(line[0] == '%' && line[1] == '%') {
          mode++;
          break;
        }
        if((line[0] >= 'a' && line[0] <= 'z') ||
           (line[0] >= 'A' && line[0] <= 'Z')) {
          char *left_str = (char*)malloc(256*sizeof(char*));
          int right = 0;
          int res = sscanf(line, "%s -> %d\n", left_str, &right);
          if(res != -1) {
            Lexem *l = new Lexem(right, left_str);
            if(lexems->has(l) == -1) {
              lexems->add(l);
            }
          }
        }
        break;
      }

      //reading prods and looking for %%
      case 2: {
        if(line[0] == '%' && line[1] == '%') {
          mode++;
          break;
        }
        if(line[0] >= 'A' && line[0] <= 'Z') {
          char *left_str = (char*)malloc(256*sizeof(char*));
          char *right_str = (char*)malloc(256*sizeof(char*));
          LexemSet *right = new LexemSet();
          sscanf(line, "%s -> %[a-zA-Z ]\n", left_str, right_str);

          Lexem *left = lexems->get(left_str);

          //cancel the current line if a token was not recognized
          if(left == NULL) {
            printf("Unrecognized token: %s\n", left_str);
            continue;
          }

          char *token = strtok(right_str, " ");
          while(token != NULL) {
            Lexem *right_lexem = lexems->get(token);

            //cancel the current line if a token was not recognized
            if(right_lexem == NULL) {
              printf("Unrecognized token: %s\n", token);
              break;
            }
            right->add(right_lexem);
            token = strtok(NULL, " ");
          }

          //cancel current line because toekn was not recognized
          if(token != NULL) {
            continue;
          }

          Production *new_prod = new Production(*production_count, left, right);
          prods[*production_count] = new_prod;
          *production_count += 1;
        }
        break;
      }

      //finished reading and waiting for EOF
      case 3: {
        break;
      }
    }
  }
}
