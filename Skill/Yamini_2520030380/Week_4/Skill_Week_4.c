#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define MAX_TOKENS 50

typedef struct {
    char *text;
} Token;

void freeTokens(Token tokens[], int count) {
    for (int i = 0; i < count; i++) {
        free(tokens[i].text);
    }
}

int tokenize(char *input, Token tokens[]) {
    int count = 0;
    char *ptr = input;

    while (*ptr != '\0') {

        /* Skip whitespace */
        while (isspace((unsigned char)*ptr)) {
            ptr++;
        }

        if (*ptr == '\0')
            break;

        /* Delimiter: | */
        if (*ptr == '|') {
            tokens[count].text = malloc(2);
            if (tokens[count].text == NULL)
                return -1;

            strcpy(tokens[count].text, "|");
            count++;
            ptr++;
            continue;
        }

        /* Read a normal token */
        char buffer[MAX_INPUT];
        int length = 0;

        while (*ptr != '\0' &&
               !isspace((unsigned char)*ptr) &&
               *ptr != '|') {

            if (length < MAX_INPUT - 1) {
                buffer[length++] = *ptr;
            }

            ptr++;
        }

        buffer[length] = '\0';

        if (length > 0) {
            tokens[count].text = malloc(length + 1);

            if (tokens[count].text == NULL)
                return -1;

            strcpy(tokens[count].text, buffer);
            count++;
        }

        if (count >= MAX_TOKENS)
            break;
    }

    return count;
}

void displayTokens(Token tokens[], int count) {
    printf("\n========== TOKEN STREAM ==========\n");

    for (int i = 0; i < count; i++) {
        printf("Token %d : [%s]\n", i + 1, tokens[i].text);
    }

    printf("==================================\n");
}

void validateTokens(Token tokens[], int count) {

    if (count == 0) {
        printf("Validation: Empty command.\n");
        return;
    }

    if (strcmp(tokens[0].text, "|") == 0) {
        printf("Validation Error: Command cannot start with '|'.\n");
        return;
    }

    if (strcmp(tokens[count - 1].text, "|") == 0) {
        printf("Validation Error: Command cannot end with '|'.\n");
        return;
    }

    for (int i = 0; i < count - 1; i++) {
        if (strcmp(tokens[i].text, "|") == 0 &&
            strcmp(tokens[i + 1].text, "|") == 0) {

            printf("Validation Error: Empty command between '|'.\n");
            return;
        }
    }

    printf("Validation: Token stream is valid.\n");
}

int main() {

    char input[MAX_INPUT];

    printf("==============================================\n");
    printf("        OSSP SKILL - WEEK 4\n");
    printf("        COMMAND TOKEN PARSER\n");
    printf("==============================================\n");

    printf("Enter a command to parse.\n");
    printf("Use 'exit' to quit.\n\n");

    while (1) {

        printf("parser> ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL)
            break;

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            printf("Exiting parser...\n");
            break;
        }

        Token tokens[MAX_TOKENS];

        int count = tokenize(input, tokens);

        if (count < 0) {
            printf("Memory allocation failed.\n");
            break;
        }

        if (count == 0) {
            printf("Empty command detected.\n");
            continue;
        }

        displayTokens(tokens, count);

        validateTokens(tokens, count);

        printf("\nExecution Structure:\n");

        for (int i = 0; i < count; i++) {

            if (strcmp(tokens[i].text, "|") == 0) {
                printf("  -> PIPE\n");
            } else {
                printf("  -> COMMAND/TOKEN: %s\n",
                       tokens[i].text);
            }
        }

        printf("\n");

        freeTokens(tokens, count);
    }

    return 0;
}
