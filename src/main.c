#include "..//include/parser.h"
#include <stdio.h>
#include <string.h>



int main() {
    const char* testMarkdown = "../test.md";

    // Create a test markdown file
    FILE* testFile = fopen(testMarkdown, "w");
    if (!testFile) {
        printf("Failed to create test file.\n");
        return 1;
    }
    fprintf(testFile, 
        "**Bold text**\n"
        );
    fclose(testFile);

    // Initialize a TokenList
    struct TokenList tokenList;
    initializeTokenList(&tokenList, 10);

    // Tokenize the test markdown file
    tokenizeMarkdown(testMarkdown, &tokenList);

    // Verify the tokenized output
    printf("Tokens extracted:\n");
    printTokens(&tokenList);

    // Validate the tokens (manual checks or add assertions if using a test framework)
    for (size_t i = 0; i < tokenList.size; ++i) {
        struct Token* token = &tokenList.tokens[i];
        switch (i) {
            case 0:
                if (token->type != HEADING || token->quantity != 1 || strcmp(token->content, "Heading 1") != 0) {
                    printf("Test failed on token 0\n");
                }
                break;
            case 1:
                if (token->type != HEADING || token->quantity != 2 || strcmp(token->content, "Heading 2") != 0) {
                    printf("Test failed on token 1\n");
                }
                break;
            case 2:
                if (token->type != HEADING || token->quantity != 3 || strcmp(token->content, "Heading 3") != 0) {
                    printf("Test failed on token 2\n");
                }
                break;
            case 3:
                if (token->type != PLAIN_TEXT || strcmp(token->content, "Regular text") != 0) {
                    printf("Test failed on token 3\n");
                }
                break;
            case 4:
                if (token->type != BOLD || strcmp(token->content, "Bold text") != 0) {
                    printf("Test failed on token 4\n");
                }
                break;
            case 5:
                if (token->type != ITALICS || strcmp(token->content, "Italic text") != 0) {
                    printf("Test failed on token 5\n");
                }
                break;
            default:
                printf("Unexpected token\n");
        }
    }

    // Clean up
    freeTokenList(&tokenList);
    remove(testMarkdown); // Delete the temporary test file
}
