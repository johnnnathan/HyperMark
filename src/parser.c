#include "../include/parser.h"
#include <stdio.h>
#include <stdlib.h>


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

void freeTokenList(struct TokenList* list){
  free(list->tokens);
  free(list);
}


void tokenizeMarkdown(const char* filename, struct TokenList* list){
  FILE* file = fopen(filename, "r");
  if (!file){
    printf("Could not open file");
    exit(3);
  }
  int c;
  int pc; // previous c
  pc = fgetc(file);
  while((c = fgetc(file)) != EOF){
    // If duplicate character or # with newline before it, indicating Heading
    if (c == pc || (c == '#' && pc == '\n')){
      char* string;
      int counter = 1;
      switch (c) {
        case '#':
          while ((c = fgetc(file)) == '#'){
            counter++; 
          }
          string = getContent(file, '\n', 0);
          break;
        case '*':
        case '_':
          string = getContent(file, c, 1);
          counter = 0;
          break;
      }
      struct Token token;
      token.type = getMDType(c);
      token.content = string;
      token.quantity = counter; 
      addToken(list, &token);
    }
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
