#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define INITIAL_SIZE 32
#define MAX_HISTORY 10

typedef struct HistoryNode {
    char *command;
    struct HistoryNode *next;
} HistoryNode;

HistoryNode *historyHead = NULL;
int historyCount = 0;

void addHistory(const char *command) {
    if (strlen(command) == 0)
        return;

    HistoryNode *newNode = malloc(sizeof(HistoryNode));
    if (newNode == NULL) {
        perror("malloc");
        return;
    }

    newNode->command = malloc(strlen(command) + 1);
    if (newNode->command == NULL) {
        free(newNode);
        perror("malloc");
        return;
    }

    strcpy(newNode->command, command);
    newNode->next = NULL;

    if (historyHead == NULL) {
        historyHead = newNode;
    } else {
        HistoryNode *temp = historyHead;

        while (temp->next != NULL)
            temp = temp->next;

        temp->next = newNode;
    }

    historyCount++;

    /* Remove oldest command when history is full */
    if (historyCount > MAX_HISTORY) {
        HistoryNode *old = historyHead;
        historyHead = historyHead->next;

        free(old->command);
        free(old);
        historyCount--;
    }
}

char *getHistory(int index) {
    HistoryNode *temp = historyHead;
    int i = 0;

    while (temp != NULL) {
        if (i == index)
            return temp->command;

        temp = temp->next;
        i++;
    }

    return NULL;
}

void freeHistory() {
    HistoryNode *temp = historyHead;

    while (temp != NULL) {
        HistoryNode *next = temp->next;
        free(temp->command);
        free(temp);
        temp = next;
    }

    historyHead = NULL;
    historyCount = 0;
}

void enableRawMode(struct termios *original) {
    struct termios raw;

    tcgetattr(STDIN_FILENO, original);
    raw = *original;

    raw.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disableRawMode(struct termios *original) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, original);
}

void clearLine(int length) {
    for (int i = 0; i < length; i++)
        printf("\b \b");

    fflush(stdout);
}

int main() {
    struct termios original;

    char *buffer = malloc(INITIAL_SIZE);
    int bufferSize = INITIAL_SIZE;
    int length = 0;

    if (buffer == NULL) {
        perror("malloc");
        return 1;
    }

    printf("==============================================\n");
    printf("        OSSP SKILL - WEEK 3\n");
    printf("        COMMAND HISTORY & MEMORY\n");
    printf("==============================================\n");
    printf("Use UP/DOWN arrows for history.\n");
    printf("Type 'history' to display commands.\n");
    printf("Type 'exit' to quit.\n\n");

    while (1) {
        length = 0;
        buffer[0] = '\0';

        int historyPosition = historyCount;

        printf("ossp> ");
        fflush(stdout);

        enableRawMode(&original);

        while (1) {
            char ch;

            if (read(STDIN_FILENO, &ch, 1) <= 0)
                break;

            /* Enter */
            if (ch == '\n' || ch == '\r') {
                printf("\n");
                break;
            }

            /* Backspace */
            if (ch == 127 || ch == 8) {
                if (length > 0) {
                    length--;
                    buffer[length] = '\0';
                    printf("\b \b");
                    fflush(stdout);
                }
                continue;
            }

            /* Escape sequence */
            if (ch == 27) {
                char seq[2];

                if (read(STDIN_FILENO, &seq[0], 1) <= 0)
                    continue;

                if (read(STDIN_FILENO, &seq[1], 1) <= 0)
                    continue;

                /* UP arrow */
                if (seq[0] == '[' && seq[1] == 'A') {
                    if (historyPosition > 0) {
                        historyPosition--;

                        clearLine(length);

                        char *oldCommand =
                            getHistory(historyPosition);

                        if (oldCommand != NULL) {
                            strcpy(buffer, oldCommand);
                            length = strlen(buffer);

                            printf("%s", buffer);
                            fflush(stdout);
                        }
                    }
                }

                /* DOWN arrow */
                else if (seq[0] == '[' && seq[1] == 'B') {
                    if (historyPosition < historyCount - 1) {
                        historyPosition++;

                        clearLine(length);

                        char *oldCommand =
                            getHistory(historyPosition);

                        if (oldCommand != NULL) {
                            strcpy(buffer, oldCommand);
                            length = strlen(buffer);

                            printf("%s", buffer);
                            fflush(stdout);
                        }
                    } else {
                        historyPosition = historyCount;

                        clearLine(length);

                        length = 0;
                        buffer[0] = '\0';
                    }
                }

                continue;
            }

            /* Printable characters */
            if (ch >= 32 && ch <= 126) {
                if (length >= bufferSize - 1) {
                    int newSize = bufferSize * 2;

                    char *newBuffer =
                        realloc(buffer, newSize);

                    if (newBuffer == NULL) {
                        printf("\nMemory allocation failed.\n");
                        disableRawMode(&original);
                        free(buffer);
                        freeHistory();
                        return 1;
                    }

                    buffer = newBuffer;
                    bufferSize = newSize;
                }

                buffer[length++] = ch;
                buffer[length] = '\0';

                putchar(ch);
                fflush(stdout);
            }
        }

        disableRawMode(&original);

        if (strcmp(buffer, "exit") == 0) {
            printf("Exiting shell...\n");
            break;
        }

        if (strcmp(buffer, "history") == 0) {
            HistoryNode *temp = historyHead;
            int number = 1;

            while (temp != NULL) {
                printf("%d  %s\n", number, temp->command);
                temp = temp->next;
                number++;
            }

            continue;
        }

        if (length > 0) {
            printf("You entered: %s\n", buffer);
            addHistory(buffer);
        }
    }

    free(buffer);
    freeHistory();

    return 0;
}
