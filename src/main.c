#include "..//include/parser.h"



int main() {
    const char* testMarkdown = "../test.md";


    struct TokenList tokenList;
    initializeTokenList(&tokenList, 10);


    tokenizeMarkdown(testMarkdown, &tokenList);
    toHTML(&tokenList);
    freeTokenList(&tokenList);
}

