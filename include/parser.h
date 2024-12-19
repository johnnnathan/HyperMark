#include <stddef.h>
#include <stdio.h>

enum Type {
    HEADING,
    BOLD,
    ITALICS,
    PLAIN_TEXT, // For unformatted content
    LINK,
    IMAGE,
    CODE_BLOCK,
    LIST,
    BLOCKQUOTE
};

struct Token {
  enum Type type;  
  char* content;
  int quantity;
};

struct TokenList{
  struct Token* tokens;
  size_t size;
  size_t capacity;
};

struct CountResult{
  int count;
  int nextChar;
};

void initializeTokenList(struct TokenList* list , size_t capacity);
void addToken(struct TokenList* list, struct Token* token);
void freeTokenList(struct TokenList* list);
void tokenizeMarkdown(const char* filename, struct TokenList* list);
enum Type detectType(const char* markdown);
struct CountResult countConsecutiveChars(FILE* file, char ch);
char* getContent(FILE* file, char endChar, int duplicateEndChar);
enum Type getMDType(char c);
