#include "../include/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void initializeTokenList(struct TokenList* list , size_t capacity){
  list->size = 0;
  list->capacity = capacity;
  list->tokens = (struct Token*)malloc(list->capacity * sizeof(struct Token));

  if (list->tokens == NULL){
    printf("Error while intitializing TokenList");
    exit(1);
  }
  return;
}

void freeTokenList(struct TokenList* list){
  free(list->tokens);
  return;
}




void addToken(struct TokenList* list, struct Token* token){
  if (list->size == list->capacity){
    size_t newCap = list->capacity * 2;
    if (newCap < 1){
      printf("TokenList capacity overflow, file is too big.");
      exit(2);
    }
    list->tokens = (struct Token*)realloc(list->tokens,newCap * sizeof(struct Token));
    if (list->tokens == NULL){
      printf("Error when resizing TokenList inside addToken");
      free(list->tokens);
      exit(3);
    }
    list->capacity = newCap;
  }
  list->tokens[list->size] = *token;
  list->size += 1;
  return;
  
}

int isSpecialType(char c){
  if (c == '#' || c == '_' || c == '*'){
    return 1;
  }
  return 0;
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

void printTokens(struct TokenList* list){
  for (size_t i = 0; i < list->size; ++i) {
      printf("Token %zu:\n", i);
      printf("  Type: %d\n", list->tokens[i].type);
      printf("  Content: %s\n", list->tokens[i].content);
      printf("  Quantity: %d\n", list->tokens[i].quantity);
  }
}

void tokenizeMarkdown(FILE* file, struct TokenList* list){
  int c = fgetc(file);
  int specialType = getMDType(c);
  int pc = ' '; // previous c
  char textBuffer[1024];
  int textLength = 0;
  int acceptFlag = 1;
  do {
    if ((isSpecialType(c) && (c == pc || pc == ' ')) || (c == '#') || (pc == '-' && c == ' ')){
      char* string;
      int counter = 1;


      if (textLength > 0){
        textBuffer[textLength - 1] = '\0';
        if (strlen(textBuffer) != 0){
          struct Token token = {0};
          token.type = PLAIN_TEXT;
          strncpy(token.content, textBuffer, sizeof(token.content) - 1);
          token.content[token.capacity -1] = '\0';
          token.quantity = 0; 
          addToken(list, &token);
        }
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
          if (pc != ' '){
            specialType = BOLD;
            string = getContent(file, c, 1);
            counter = 0;
            break;
          }
          acceptFlag = 0;
          break;
        case '_':
          specialType = ITALICS;
          string = getContent(file, c, 0);
          counter = 0;
          break;
      }

      if (pc == '-'){
        specialType = LIST;
        string = getContent(file, '\n',0);
        counter = 0;
      }

      if (acceptFlag != 0 ){
        struct Token token = {0};
        token.type = specialType;
        strncpy(token.content, string, sizeof(token.content) - 1);
        token.content[token.capacity - 1] = '\0';
        token.quantity = counter; 
        addToken(list, &token);
        free(string);

      }
      acceptFlag = 1;
    }
    else {
      if(textLength < 1023){
        textBuffer[textLength++] = c;
      }
      else{
        textBuffer[textLength] = '\0';
        struct Token token;
        token.type = PLAIN_TEXT;
        token.quantity = 0; 
        strncpy(token.content, textBuffer, sizeof(token.content) - 1);
        token.content[token.capacity-1] = '\0';
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
    strncpy(token.content, textBuffer, sizeof(token.content) - 1);
    token.content[token.capacity -1] = '\0';
    token.quantity = 0; 
    addToken(list, &token);
    textLength = 0;
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
void toHTML(struct TokenList* list){
  FILE* testFile = fopen("output.html", "w");
  if (testFile == NULL){
      printf("Failed to create test file.\n");
      exit(1);
  }
  fprintf(testFile, "<!DOCTYPE html>\n<html>\n<body>");
  for (int i = 0; i < list->size; i++){
    struct Token token = list->tokens[i];
    int type = token.type;
    switch (type) {
      case PLAIN_TEXT:
        fprintf(testFile, "%s",  token.content);
        break;
      case HEADING:
        fprintf(testFile, "<h%d>%s</h%d>", token.quantity , token.content, token.quantity);
        if (i + 1 < list->size && list->tokens[i+1].type != HEADING){
          fprintf(testFile, "<p>");
        }
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
        if ((i - 1 >= 0 && list->tokens[i-1].type != LIST) || (i == 0)){
          fprintf(testFile, "<ul>\n");
        }
        fprintf(testFile, "\t<li>%s</li>\n",  token.content);
        if (i + 1 < list->size && list->tokens[i+1].type != LIST){
          fprintf(testFile, "</ul>\n");
        }
        break;

      case BLOCKQUOTE:
      default:
        break;
    }
    if (i + 1 == list->size || list->tokens[i + 1].type == HEADING){
      fprintf(testFile, "</p>\n");
    }


  }

  fprintf(testFile, "\n</body>\n</html>");
  printTokens(list);
  fclose(testFile);
}
