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

struct CountResult countConsecutiveChars(FILE* file, char ch){
  struct CountResult result;
  result.count = 1;
  int c;
  while ((c = fgetc(file)) == ch){
    result.count += 1;
  }
  result.nextChar = c;
  if (c == EOF){
    ungetc(c, file);
  }
  return result;
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
  int index = 0;
  while ((c = fgetc(file)) && c != EOF){
    if (!duplicateEndChar && endChar == c){
      content[index] = '\0';
      return content;
    }
    else if(duplicateEndChar && (endChar == c && c == pc)){
      content[index] = '\0';
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
