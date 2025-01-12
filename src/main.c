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
  printf("Initializing Tokens...\n");
  initializeTokenList(&tokenList, 32);
  printf("Generating Tokens...\n");
  tokenizeMarkdown(file, &tokenList);
  printf("Translating to HTML...\n");
  toHTML(&tokenList);


  printf("Cleaning up tokens...\n");
  freeTokenList(&tokenList);
  fclose(file);
}
