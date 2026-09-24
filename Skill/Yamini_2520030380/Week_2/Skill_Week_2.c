#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#define BUFFER_SIZE 100

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

int main() {
    char input[BUFFER_SIZE];
    int length;

    struct termios original;

    printf("==============================================\n");
    printf("        OSSP SKILL - WEEK 2\n");
    printf("        KEYBOARD INPUT HANDLING\n");
    printf("==============================================\n");
    printf("Type commands. Use Backspace to edit.\n");
    printf("Type 'exit' to quit.\n\n");

    while (1) {
        length = 0;
        input[0] = '\0';

        printf("ossp> ");
        fflush(stdout);

        enableRawMode(&original);

        while (1) {
            char ch;

            if (read(STDIN_FILENO, &ch, 1) <= 0)
                break;

            /* Enter key */
            if (ch == '\n' || ch == '\r') {
                printf("\n");
                break;
            }

            /* Backspace key */
            if (ch == 127 || ch == 8) {
                if (length > 0) {
                    length--;
                    input[length] = '\0';

                    printf("\b \b");
                    fflush(stdout);
                }
                continue;
            }

            /* Store printable characters */
            if (ch >= 32 && ch <= 126) {
                if (length < BUFFER_SIZE - 1) {
                    input[length++] = ch;
                    input[length] = '\0';

                    putchar(ch);
                    fflush(stdout);
                }
            }
        }

        disableRawMode(&original);

        if (strcmp(input, "exit") == 0) {
            printf("Exiting shell...\n");
            break;
        }

        if (length > 0) {
            printf("You entered: %s\n", input);
        }
    }

    return 0;
}

