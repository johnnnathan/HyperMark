#include "../include/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void initializeTokenList(struct TokenList* list , size_t capacity){
  list->tokens = malloc(sizeof(struct Token) * capacity);
  if (list->tokens == NULL){
    printf("Error when initializing TokenList");
    exit(1);
  }
  list->size = 0;
  list->capacity = capacity;
}

void addToken(struct TokenList* list, struct Token* token){
  if (list->capacity == list->size){
    size_t newCap = list->capacity * 2;
    struct Token* temp = realloc(list->tokens, newCap);
    if (temp == NULL){
      printf("Error while resizing TokenList");
      exit(2);
    }
    list->tokens = temp;
    list->capacity = newCap;
  }
  list->tokens[list->size++] = *token;
}


void freeTokenList(struct TokenList* list) {
  free(list->tokens);

}


int isSpecialType(char c){
  if (c == '#' || c == '_' || c == '*'){
    return 1;
  }
  return 0;
}


void tokenizeMarkdown(const char* filename, struct TokenList* list){
  FILE* file = fopen(filename, "r");
  if (!file){
    printf("Could not open file");
    exit(3);
  }
  int c = fgetc(file);
  int specialType = getMDType(c);
  int pc; // previous c
  char textBuffer[1024];
  int textLength = 0;
  do {
    if ((isSpecialType(c) && c == pc) || (c == '#')){
      char* string;
      int counter = 1;


      if (textLength > 0){
        textBuffer[textLength - 1] = '\0';
        struct Token token = {0};
        token.type = PLAIN_TEXT;
        token.content = strdup(textBuffer);
        token.quantity = 0; 
        addToken(list, &token);
        textLength = 0;
      }

      switch (c) {
        case '#':
          while ((c = fgetc(file)) == '#'){
            counter += 1; 
          }
          specialType = HEADING;
          string = getContent(file, '\n', 0);
          break;
        case '*':
        case '_':
          if (c == '*'){ specialType = BOLD;}
          if (c == '_'){ specialType = ITALICS;}
          string = getContent(file, c, 1);
          counter = 0;
          break;
      }

      struct Token token = {0};
      token.type = specialType;
      token.content = strdup(string);
      token.quantity = counter; 
      addToken(list, &token);
      free(string);
    }
    else {
      if(textLength < 1023){
        textBuffer[textLength++] = c;
      }
      else{
        textBuffer[textLength] = '\0';

        struct Token token;
        token.type = PLAIN_TEXT;
        token.content = strdup(textBuffer);
        token.quantity = 0; 
        addToken(list, &token);
        textLength = 0;
      }

    }
    pc = c;
  }while((c = fgetc(file)) != EOF);
  
  if (textLength > 0){

    textBuffer[textLength] = '\0';

    struct Token token;
    token.type = PLAIN_TEXT;
    token.content = strdup(textBuffer);
    token.quantity = 0; 
    addToken(list, &token);
    textLength = 0;
  }
  if (file == NULL){
    printf("ERROR: File not open");
    exit(1);
  }



}

enum Type getMDType(char c){
  switch (c) {
    case '#':
      return HEADING;
    case '*':
      return BOLD;
    case '_':
      return ITALICS;
    default:
      return PLAIN_TEXT;
  
  }
}

char* getContent(FILE* file, char endChar, int duplicateEndChar){
  int pc = fgetc(file);
  int c;
  if (pc == EOF) return NULL;
  char* content;
  size_t contCap = 128;
  content = malloc(contCap);
  if (content == NULL){
    printf("Error while initializing content inside getContent");
    exit(4);
  }
  content[0] = pc;
  int index = 1;
  while ((c = fgetc(file)) && c != EOF){
    if (!duplicateEndChar && endChar == c){
      content[index] = '\0';
      return content;
    }
    else if(duplicateEndChar && (endChar == c && c == pc)){
      content[index - 1] = '\0';
      return content;
    }
    if (index == contCap){
      char* temp = realloc(content, contCap * 2);
      if (temp == NULL){
        free(content);
        printf("Realloc failed");
        exit(5);
      }
      content = temp;
    }
    content[index] = c;
    index++;
    pc = c;
  }
  content[index] = '\0';
  return content;
}

void printTokens(struct TokenList* list) {
    for (size_t i = 0; i < list->size; ++i) {
        printf("Token %zu:\n", i);
        printf("  Type: %d\n", list->tokens[i].type);
        printf("  Content: %s\n", list->tokens[i].content);
        printf("  Quantity: %d\n", list->tokens[i].quantity);
    }
}


void toHTML(struct TokenList* list){
  FILE* testFile = fopen("../output.html", "w");
  if (!testFile) {
      printf("Failed to create test file.\n");
      exit(1);
  }
  for (int i = 0; i < list->size; i++){
    struct Token token = list->tokens[i];
    int type = token.type;
    switch (type) {
      case PLAIN_TEXT:
        fprintf(testFile, "%s",  token.content);
        break;
      case HEADING:
        fprintf(testFile, "<h%d>%s</h%d>", token.quantity , token.content, token.quantity);
        break;
      case BOLD:
        fprintf(testFile, "<b>%s</b>",  token.content);
        break;
      case ITALICS:
        fprintf(testFile, "<i>%s</i>",  token.content);
        break;
      case LINK:
      case IMAGE:
      case CODE_BLOCK:
      case LIST:
      case BLOCKQUOTE:
      default:
        break;
    }


  }
  fclose(testFile);
}
