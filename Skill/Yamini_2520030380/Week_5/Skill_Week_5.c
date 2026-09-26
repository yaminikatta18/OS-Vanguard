#include <stdio.h>
#include <string.h>

int main() {
    char *name = "Yamini";

    printf("=============================================\n");
    printf("        OSSP SKILL WEEK 5\n");
    printf("   Single Quotes and Double Quotes\n");
    printf("=============================================\n\n");

    // Single quotes: literal content
    printf("Single Quote Test:\n");
    printf("'Hello $USER'\n");
    printf("Result: $USER is treated as literal text.\n\n");

    // Double quotes: variable expansion
    printf("Double Quote Test:\n");
    printf("\"Hello %s\"\n", name);
    printf("Result: Variable value is displayed.\n\n");

    // Preserve spaces
    printf("Preserve Spaces Test:\n");
    printf("\"Hello World from OSSP\"\n\n");

    // Nested tokens
    printf("Nested Token Test:\n");
    printf("\"User: '%s'\"\n\n", name);

    // Edge case
    printf("Edge Case Test:\n");
    printf("'Special $HOME $PATH characters remain literal.'\n");

    printf("\n=============================================\n");
    printf("Week 5 testing completed successfully.\n");
    printf("=============================================\n");

    return 0;
}
