#include "../include/parser.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
  const char* testMD = "test.md";
  FILE* file = fopen(testMD, "r");
  if (file == NULL){
    printf("Error opening file");
    return 1;
  }
  struct TokenList tokenList;
  initializeTokenList(&tokenList, 32);
  tokenizeMarkdown(file, &tokenList);
  toHTML(&tokenList);

  freeTokenList(&tokenList);
  printf("Something");
  fclose(file);
}
